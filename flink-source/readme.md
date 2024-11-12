# flink source 详解
[flip-27](https://cwiki.apache.org/confluence/display/FLINK/FLIP-27%3A+Refactor+Source+Interface)
FLIP-27 介绍了新版本Source 接口定义及架构

相比于SourceFunction，新版本的Source更具灵活性，原因是将“splits数据获取”与真“正数据获取”逻辑进行了分离
![alt text](image.png)

## 重要部件
Source 作为工厂类，会创建以下两个重要部件
1. SplitEnumerator 
    * 通过createEnumerator创建
    * SplitEnumerator 响应request split请求
        * handleSplitRequest
    * 工作在SourceCoordinator (官方描述如下)，可以理解为在JobMaster上运行一个单线程的逻辑，所以需要跟在worker上的reader通过rpc通信

        ```
        Where to run the enumerator
        There was a long discussion about where to run the enumerator which we documented in the appendix. The final approach we took was very similar to option 3 with a few differences. The approach is following.

        Each SplitEnumerator will be encapsulated in one SourceCoordinator. If there are multiple sources, multiple SourceCoordinator will there be. The SourceCoordinators will run in the JobMaster, but not as part of the ExecutionGraph. In this FLIP, we propose to failover the entire execution graph when the SplitEnumerator fails. A finer grained enumerator failover will be proposed in a later FLIP.
        ```

2. SourceReader
    * 通过createReader创建
    * 工作在worker
    * 由于单独实现SourceReader过于复杂，官方抽象了3种比较通用的模型供开发者使用，`MySqlSourceReader`就是继承了`SingleThreadMultiplexSourceReaderBase`
        1. Sequential Single Split (File, database query, most bounded splits)
        2. Multi-split multiplexed (Kafka, Pulsar, Pravega, ...)
        3. Multi-split multi-threaded (Kinesis, ...)
        ![alt text](image-1.png)
        ![alt text](image-2.png)
        ![alt text](image-3.png)
    * 使用了抽象后的类，开发者的关注点集中在实现一个SplitReader
        ```
        public interface SplitReader<E, SplitT extends SourceSplit> {
 
            RecordsWithSplitIds<E> fetch() throws InterruptedException;
        
            void handleSplitsChanges(Queue<SplitsChange<SplitT>> splitsChanges);
        
            void wakeUp();
        }
        ```
        1. fetch 获取数据，这里是包含了split信息的record
        2. 响应split改变
        3. 唤醒

3. RecordEmitter
    1. The RecordsWithSplitIds returned by the SplitReader will be passed to an RecordEmitter one by one.
    2. The RecordEmitter is responsible for the following:
        * Convert the raw record type <E> into the eventual record type <T>
        * Provide an event time timestamp for the record that it processes.
    3. 在 emitRecord 方法中实现

由于通信使用mail风格的rpc（单线程串行），所以响应函数需要保证非阻塞，所以后面可以看到无论enumerator还是reader的最终响应都是在异步线程池中

```
Non-blocking progress methods, to it supports running in an actor/mailbox/dispatcher style operator
```

## MysqlSource 举例

以flink cdc中的MysqlSource来举例分析

1. MysqlSource
    * createEnumerator
        * MySqlSourceEnumerator 
            * 初始化调用open 
                * splitAssigner 是获取/分配split动作的真正实现
                    * 调用splitAssigner.open()
                        * 创建异步线程，填充remainingSplits
                    * 主线程通过getNext获取最新的split
            * handleSplitRequest 响应空闲worker的请求
                * assignSplits
                    * splitAssigner.getNext()
                        * 从 remainingSplits 拿一个可用的split
        * MySqlSourceEnumerator 中 splitAssigner 的实现说明
            * splitAssigner 默认实现是 MySqlHybridSplitAssigner
                * hybrid的含义，启动分为两个步骤 1. 读取全量数据 2. 全量数据读取完毕后读取增量数据。所以MySqlSnapshotSplitAssigner可以创建两种split
                    1. 通过MySqlSnapshotSplitAssigner创建全量数据的split
                        * 在读取全量数据时通过chunckSplitter多线程并行读取
                    2. 通过 createBinlogSplit 创建增量数据的split
                        * 只assign一次binlog的split
        ![alt text](1.jpg)
    * createReader
        * MySqlSourceReader



## 其他

no watermark