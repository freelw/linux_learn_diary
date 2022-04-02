# 火焰图学习

[阮一峰的网络日志](https://www.ruanyifeng.com/blog/2017/09/flame-graph.html)

* perf 命令

    sudo perf record -F 99 -p 13204 -g -- sleep 30
    -F 99 表示采样频率每秒99次
    -p <进程id>
    -g 记录调用栈
    -- 表示之后的字符串都是参数而不是options
    A -- signals the end of options and disables further option processing. Any arguments after the -- are treated as filenames and arguments.