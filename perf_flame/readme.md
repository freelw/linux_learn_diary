# 火焰图学习

[阮一峰的网络日志](https://www.ruanyifeng.com/blog/2017/09/flame-graph.html)

* perf 命令

        sudo perf record -F 99 -p 13204 -g -- sleep 30
        -F 99 表示采样频率每秒99次
        -p <进程id>
        -g 记录调用栈
        -- 表示之后的字符串都是参数而不是options
        A -- signals the end of options and disables further option processing. Any arguments after the -- are treated as filenames and arguments.

* 生成火焰图

        1. git clone https://github.com/brendangregg/FlameGraph.git
        2. 用 perf script -i <perf.data> 可以解开折叠的数据
            perf script -i <perf.data> &> perf_data.unfold
            &> 符号可以表标准输出可错误输出都定向到perf_data.unfold中
        3. 用 stackcollapse-perf.pl 将 perf 解析出的内容 perf_data.unfold 中的符号进行折叠
            ./stackcollapse-perf.pl /tmp/perf_data.unfold &> /tmp/perf.folded
        4. 生成火焰图./flamegraph.pl /tmp/perf_data.folded > /root/perf.svg

* 简化命令

        perf script | /home/ubuntu/FlameGraph/stackcollapse-perf.pl | /home/ubuntu/FlameGraph/flamegraph.pl > process.svg
        可以使用perf_playground中的脚本简化操作

* 检测丢包

        下面用perf监控丢弃的数据包，使用 perf 监视 kfree_skb 事件：
        sudo perf record -g -a -e skb:kfree_skb
        sudo perf script

* ping响应耗时高，检测ksoftirqd调度延时

        sudo perf sched record # 记录调度数据
        [root@VM-0-13-centos perf_test]# sudo perf sched latency -p --sort max | grep soft
        ksoftirqd/1:16        |      0.016 ms |        3 | avg:    0.002 ms | max:    0.002 ms | max at: 769052.619357 s
        ksoftirqd/0:9         |      0.024 ms |        2 | avg:    0.001 ms | max:    0.002 ms | max at: 769056.226342 s

        perf_sched.sh 可以完成采集工作
