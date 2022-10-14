    
[uprobe](https://lwn.net/Articles/499190/)

    echo 'p:func1 ./uprobe_test:0x1126' >> /sys/kernel/debug/tracing/uprobe_events
    echo 'r:func1_ret ./uprobe_test:0x1126' >> /sys/kernel/debug/tracing/uprobe_events
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
    就直接
    perf probe -x ./uprobe_test func1
    perf probe -x ./uprobe_test func1%return
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
    

## 内核流程

    uprobe_register
        __uprobe_register
            register_for_each_vma

                这里uprobe对象带有inode
                inode 有i_mapping信息
                i_mapping中有当前所有映射到inode的vma信息，也就是说，当前加载了这个二进制文件的内存映射关系都在i_mapping中 address_space 结构体记录这些信息
                这是存量hook，都install_breakpoint

                install_breakpoint 通过vma的信息拿到old_page，然后copy一个page，跟一个新的anon vma关联，将int 3 插入新page，这样每个进程被hook的vma都指向了一个新的页面，填充了int 3(0xcc)

                在hit breakpoint时 触发uprobe_notify_resume
    
    针对增量hook
        在uprobe_mmap中install_breakpoint
        uprobe_mmap 在各种创建vma的位置被调用，也就是说新加载的vma，都要过一下有没有uprobe要探测他的

[i_mapping](https://blog.csdn.net/jinking01/article/details/106490467)