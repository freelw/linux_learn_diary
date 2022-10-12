    
    echo 'r:wangli2 ./uprobe_test:0x1126' > /sys/kernel/debug/tracing/uprobe_events
    echo 'p:wangli1 ./uprobe_test:0x1126' >> /sys/kernel/debug/tracing/uprobe_events
    cat /sys/kernel/debug/tracing/uprobe_events
    echo 1 >/sys/kernel/debug/tracing/events/uprobes/enable

我终于理解为什么uprobe要填写偏移地址了

因为传入给uprobe机制的是两个参数 
1. 文件路径，最终会被转换为inode 
2.offset 代表要hook的点在文件中的偏移量，而不是反汇编时候的地址

uprobe 通过inode就可以知道二进制文件具体加载在进程地址空间的起始位置，再加上offset的信息，就可以找到hook点

为什么不是直接用二进制文件中的地址，而是采用重新计算一遍的方式呢，这两种方式得出的数值明明一样

我现在猜测是因为uprobe机制不但可以hook 可执行程序中的符号，还可以hook 共享库中的符号，共享库中的符号是地址无关的