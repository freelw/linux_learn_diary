# flink-pipeline-composer 详解

## 背景

在flink-cdc 3.0中引入了pipeline机制，提供了除Datastream api/flink sql以外的一种方式定义flink 任务

主要的形式是提供一个yaml文件，描述source sink transform等主要信息

由FlinkPipelineComposer解析，自动调用DataStream api构建任务

[官方样例](https://nightlies.apache.org/flink/flink-cdc-docs-master/zh/docs/core-concept/data-pipeline/)

```
 source:
     type: mysql
     hostname: localhost
     port: 3306
     username: root
     password: 123456
     tables: app_db.\.*

   sink:
     type: doris
     fenodes: 127.0.0.1:8030
     username: root
     password: ""

   pipeline:
     name: Sync MySQL Database to Doris
     parallelism: 2
```

目前可以通过source配置的源只有mysql 和 values

values是调试用的，所以可以说当前这个功能是专门为“mysql同步数据到各个sink”的场景使用的

目前可以使用的sink有

1. doris
2. elasticsearch
3. kafka
4. paimon
5. starrocks
6. values


## FlinkPipelineComposer

我们以mysql -> values来观察 FlinkPipelineComposer 是如何通过读取yaml文件的定义来构建DataStream的

values会将mysql产生的cdc消息打印到stdout上

```
################################################################################
# Description: Sync MySQL all tables to Doris
################################################################################
source:
 type: mysql
 hostname: x.x.x.x
 port: 3306
 username: username
 password: password
 tables: test.t1
 server-id: 5400-5404
 server-time-zone: UTC+8

sink:
  type: values
  name: values Sink

pipeline:
 name: Sync Mysql Database to Values
 parallelism: 2
```

首先来观察一下这个任务提交到flink集群后具体的链路构成

![1](image.png)

结合官方给出的架构

![alt text](image-1.png)

可以看出，我们的“一个source，一个sink”的定义，最终会生成5个operator

1. Souce: Flink CDC Event Source: mysql
2. SchemaOperator
3. PrePartition
-------------- shuffle --------------
4. PostPartion
5. Sink Writer: values Sink