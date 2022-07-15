# ebpf

[[译] 使用 Linux tracepoint、perf 和 eBPF 跟踪数据包 (2017)](https://arthurchiao.art/blog/trace-packet-with-tracepoint-perf-ebpf-zh/)

## bcc tools 位置

    /usr/share/bcc/tools

## tencentos上bpftrace 段错误的问题解决方案
    
    回退bcc（当前是0.24.0）
    yum downgrade bcc-0.19.0-4.tl3.x86_64

[github上的解释](https://github.com/iovisor/bpftrace/issues/2173)