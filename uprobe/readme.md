    
[uprobe](https://lwn.net/Articles/499190/)

    echo 'r:wangli2 /root/linux_learn_diary/uprobe/uprobe_test:0x1136' >> /sys/kernel/debug/tracing/uprobe_events
    echo 'p:wangli1 /root/linux_learn_diary/uprobe/uprobe_test:0x1136' >> /sys/kernel/debug/tracing/uprobe_events
    cat /sys/kernel/debug/tracing/uprobe_events
    echo 1 >/sys/kernel/debug/tracing/events/uprobes/enable
    echo 0 >/sys/kernel/debug/tracing/events/uprobes/enable
    echo > /sys/kernel/debug/tracing/uprobe_events

## 注意
    /sys/kernel/debug/tracing/tracing_on 需要设置为1

我终于理解为什么uprobe要填写偏移地址了

因为传入给uprobe机制的是两个参数 
1. 文件路径，最终会被转换为inode 
2.offset 代表要hook的点在文件中的偏移量，而不是反汇编时候的地址

uprobe 通过inode就可以知道二进制文件具体加载在进程地址空间的起始位置，再加上offset的信息，就可以找到hook点

为什么不是直接用二进制文件中的地址，而是采用重新计算一遍的方式呢，这两种方式得出的数值明明一样

我现在猜测是因为uprobe机制不但可以hook 可执行程序中的符号，还可以hook 共享库中的符号，共享库中的符号是地址无关的

[地址无关pie也是一个原因](https://stackoverflow.com/questions/37864773/computing-offset-of-a-function-in-memory)


    perf probe -x /lib64/libc.so.6 malloc
    perf probe -x /lib64/libc.so.6 printf
    可以快速设置uprobe event
    上面是快速设置libc中的符号hook

    注意perf设置的uprobe event在/sys/kernel/debug/tracing/events/probe_<xxx> 中
    需要打开/sys/kernel/debug/tracing/events/probe_<xxx>/enable

    比如./uprobe_test func1
    就直接perf probe -x ./uprobe_test func1
    然后cat /sys/kernel/debug/tracing/uprobe_events

    [root@VM-0-13-centos tracing]# cat /sys/kernel/debug/tracing/uprobe_events
    p:probe_uprobe_test/func1 /root/linux_learn_diary/uprobe/uprobe_test:0x0000000000001136

    发现在probe_uprobe_test这个命名空间下

    于是打开/sys/kernel/debug/tracing/events/probe_uprobe_test/enable
    echo 1 > /sys/kernel/debug/tracing/events/probe_uprobe_test/enable

    然后cat /sys/kernel/debug/tracing/trace_pipe观察输出
    在另一个终端执行./uprobe_test

    效果
    [root@VM-0-13-centos tracing]# cat /sys/kernel/debug/tracing/trace_pipe
     uprobe_test-14295   [001] d...  3710.507122: func1: (0x401136)