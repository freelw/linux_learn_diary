# Flink-cdc Schema Evolution 详解

[flink-cdc-3 glimpse](https://medium.com/@ipolyzos_/a-glimpse-into-flink-cdc-3-0-a985fb5895a5)

### flink-cdc 3.0 重要特性

* 通过yaml文件定义pipeline
* 能够感知schema变更

## pipeline demo

我们使用一个特殊的sink类型“values”来观察各种事件的产生

values是专门为调试编写的一个sink，会将产生的事件打印在stdout

需要引入包`flink-cdc-pipeline-connector-values-3.3-SNAPSHOT.jar`，可以从flink-cdc工程中编译`flink-cdc-pipeline-connector-values`得到
![](img/image4.png)
### pipeline yaml
```
################################################################################
# Description: Sync MySQL all tables to Doris
################################################################################
source:
 type: mysql
 hostname: ${ip}
 port: ${port}
 username: ${username}
 password: ${password}
 tables: ${database}.${table}
 server-id: 5400-5404
 server-time-zone: UTC+8

sink:
  type: values
  name: values Sink

pipeline:
 name: Sync Mysql Database to Values
 parallelism: 2
```
**注意：parallelism > 1 时候一定要在flink中开启checkpoint**
### 提交flink-cdc任务
```
./bin/flink-cdc.sh mysql-to-values.yaml
Pipeline has been submitted to cluster.
Job ID: a03966de35dc3141c890250daeac9699
Job Description: Sync Mysql Database to Values
```
### 在mysql中执行变更操作，观察flink taskmanager日志
```
mysql> insert into t1 values(13, 'm');
Query OK, 1 row affected (0.01 sec)

mysql> alter table t1 add column c0 varchar(255);
Query OK, 0 rows affected (0.03 sec)
Records: 0  Duplicates: 0  Warnings: 0
```

### flink日志
![](img/image.png)

### 日志解析
注意看左侧的 ">"

<div align=center>
	<img src="img/image1.png"/>
</div>

由于yaml中设置的并发度是2，所有可以看到日志中有两个任务在打印

注意CreateTableEvent和AddColumnEvent这样的关于schema改变的事件会出现在两个并发中，而一个DataChangeEvent事件只会出现在单独一个并发中
![](img/image2.png)

flink-cdc 官方文档中描述: schema相关event与DataChangeEvent之间有如下的顺序保证
```
a CreateTableEvent must be emitted before any DataChangeEvent if a table is new to the framework, 
and SchemaChangeEvent must be emitted before any DataChangeEvent if the schema of a table is changed. 
This requirement makes sure that the framework has been aware of the schema before processing any data changes.
```
见[understand-flink-cdc-api](https://nightlies.apache.org/flink/flink-cdc-docs-release-3.2/docs/developer-guide/understand-flink-cdc-api/)
![](img/image3.png)

## schema evolution 实现原理
### 整体视角
![](img/image6.png)
SchemaRegistry运行在JobManager中，继承Coordinator与SchemaOperator交互，负责协调不同流水线中收到schema变更event后的同步

### 从yaml到pipeline的转化
* 入口`flink-cdc.sh`
```
exec "$JAVA_RUN" -classpath "$CLASSPATH" "${LOG_SETTINGS[@]}" org.apache.flink.cdc.cli.CliFrontend "$@"
```
* 入口类 `CliFrontend` 在 `CliFrontend.java`
```
main 调用
  createExecutor 调用
    new CliExecutor 其中 pipelineDefPath 是yaml文件的路径
```
* `CliExecutor.java`
```
1. 通过 YamlPipelineDefinitionParser 将 pipelineDefPath parse为pipelineDef
2. PipelineComposer 通过pipelineDef的定义调用flink的api构建流水线
```
* `FlinkPipelineComposer.java`
```
// Build Source Operator
        DataSourceTranslator sourceTranslator = new DataSourceTranslator();
        DataStream<Event> stream =
                sourceTranslator.translate(
                        pipelineDef.getSource(), env, pipelineDef.getConfig(), parallelism);

...

// Schema operator
        SchemaOperatorTranslator schemaOperatorTranslator =
                new SchemaOperatorTranslator(
                        schemaChangeBehavior,
                        pipelineDef.getConfig().get(PipelineOptions.PIPELINE_SCHEMA_OPERATOR_UID),
                        pipelineDef
                                .getConfig()
                                .get(PipelineOptions.PIPELINE_SCHEMA_OPERATOR_RPC_TIMEOUT));
        OperatorIDGenerator schemaOperatorIDGenerator =
                new OperatorIDGenerator(schemaOperatorTranslator.getSchemaOperatorUid());

...

// Build DataSink in advance as schema operator requires MetadataApplier
        DataSinkTranslator sinkTranslator = new DataSinkTranslator();
        DataSink dataSink =
                sinkTranslator.createDataSink(pipelineDef.getSink(), pipelineDef.getConfig(), env);

        stream =
                schemaOperatorTranslator.translate(
                        stream,
                        parallelism,
                        dataSink.getMetadataApplier()
                                .setAcceptedSchemaEvolutionTypes(
                                        pipelineDef.getSink().getIncludedSchemaEvolutionTypes()),
                        pipelineDef.getRoute());
```
这里可以看到从yaml的描述到stream的转化

stream 关联-> 当前 env 关联-> FlinkPipelineExecution
最终通过FlinkPipelineExecution.execute()调用用到env.executeAsync()

这里处理用户描述的source和sink节点，flink-cdc还自动插入了一个SchemaOperator节点
![](img/image5.png)

#### schema event的流动

SchemaOperator与sink绑定，这里绑定关系到之后的几个操作
1. 定义一个sink的时候要提供MetaApplier，运行在JobManager（上方），通过Rpc与SchemaOperator交互
```
        schemaOperatorTranslator.translate(
                        ...
                        dataSink.getMetadataApplier()
                                ...);
```
2. 所有的event都要经过SchemaOperator，SchemaOperator对于SchemaChangeEvent特殊处理 `SchemaOperator.java`
```
public void processElement(StreamRecord<Event> streamRecord)
            throws InterruptedException, TimeoutException, ExecutionException {
        Event event = streamRecord.getValue();
        if (event instanceof SchemaChangeEvent) {
            processSchemaChangeEvents((SchemaChangeEvent) event);
        } else if (event instanceof DataChangeEvent) {
           ...
    }
```
最终调用到handleSchemaChangeEvent
```
private void handleSchemaChangeEvent(TableId tableId, SchemaChangeEvent schemaChangeEvent)
            throws InterruptedException, TimeoutException {
           ...
        // The request will block if another schema change event is being handled
        SchemaChangeResponse response = requestSchemaChange(tableId, schemaChangeEvent);
        if (response.isAccepted()) {
            LOG.info("{}> Sending the FlushEvent for table {}.", subTaskId, tableId);
            output.collect(new StreamRecord<>(new FlushEvent(tableId)));
            ...
            // The request will block until flushing finished in each sink writer
            SchemaChangeResultResponse schemaEvolveResponse = requestSchemaChangeResult();
    }
```
回想一下刚才在mysql中alter table add column的场景，每一个并发度都有一个AddColumnEvent，都会去调用
requestSchemaChange，向Coordinator发送SchemaChangeRequest
```
private SchemaChangeResponse requestSchemaChange(
            TableId tableId, SchemaChangeEvent schemaChangeEvent)
            throws InterruptedException, TimeoutException {
       ...
        while (true) {
            SchemaChangeResponse response =
                    sendRequestToCoordinator(
                            new SchemaChangeRequest(tableId, schemaChangeEvent, subTaskId));
           ...
        }
    }
```
在`SchemaRegistry.java`响应请求
```
 public CompletableFuture<CoordinationResponse> handleCoordinationRequest(
            CoordinationRequest request) {
        ...
      
                        if (request instanceof SchemaChangeRequest) {
                            SchemaChangeRequest schemaChangeRequest = (SchemaChangeRequest) request;
                            requestHandler.handleSchemaChangeRequest(
                                    schemaChangeRequest, responseFuture);
                        } else if (request instanceof SchemaChangeResultRequest) {
                            requestHandler.getSchemaChangeResult(responseFuture);
                        }
                           ...
    }
```
这时两个请求只有一个会被处理，另外一个会被认为是duplicate

处理的步骤如下
* 发起schema变更请求, 这时天然hang住了上游的消息
* 如果被Coordinator Accept，执行`output.collect(new StreamRecord<>(new FlushEvent(tableId)));`
        * flushEvent在`PrePartitionOperator.java`被广播给下游所有的sink
        ```
        public void processElement(StreamRecord<Event> element) throws Exception {
              ...
              if (event instanceof FlushEvent) {
                // Broadcast FlushEvent
                broadcastEvent(event);
              }
              ...
        }
        ```
        * flushEvent在sink中会触发当前sink flush所有缓存的事件，之后通知Coordinator完成
        `DataSinkFunctionOperator.java`
        ```
        private void handleFlushEvent(FlushEvent event) throws Exception {
                userFunction.finish();
                schemaEvolutionClient.notifyFlushSuccess(
                        getRuntimeContext().getIndexOfThisSubtask(), event.getTableId());
        }
        ```


* 
### 如何安全变更后端schema

### 实现一个sink的基本要求