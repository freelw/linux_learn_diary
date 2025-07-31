# linux_learn_diary

## 积小流 成江海

* 20200623

  
        配置了 ctags cscope tagbar nerdtree


* 20200624


        学习 git rebase
        修正路nerdtree配置
        配置了 ale youcompleteme
        注意使用python3 执行 install.py


* 20200626


        优化了.vimrc
        实验了gdb-multiarch 远程调试 tui真是不错
        开始编写第四章第一个内核模块


* 20200630

    
        实现了hello_world内核模块,在x86和arm32上运行
        实现了一个字符设备驱动,在arm32上运行

* 20200701

        实现了字符设备驱动客户端
        实现了misc设备

* 20200708

        实现了虚拟设备的FIFO读写

* 20200925

        自己动手写docker demo 2.1.2

* 20200926

        自己动手写docker demo 2.1.3
        自己动手写docker demo 2.1.4
        自己动手写docker demo 2.1.5

* 20210326

    [使用tun设备实现了一个vpndemo](https://github.com/freelw/vpndemo)

* 20210331

        使用腾讯云cvm搭建ubuntu远程桌面环境

* 20210408 todo

    [配置linux vscode环境](https://code.visualstudio.com/docs/cpp/config-linux) done

* 20210409 todo

    [automake介绍](http://www.gnu.org/software/automake/manual/automake.html#Modernize-AM_005fINIT_005fAUTOMAKE-invocation)

    [autoconf介绍](https://www.gnu.org/savannah-checkouts/gnu/autoconf/manual/autoconf-2.70/autoconf.html#Top)

    [Autotools Tutorial](https://www.lrde.epita.fr/~adl/autotools.html)

    [官方helloworld](https://www.gnu.org/software/automake/manual/html_node/Hello-World.html#Hello-World)

    [autotools 介绍pdf](https://www.lrde.epita.fr/~adl/dl/autotools.pdf)

    [autotools 学习笔记](https://github.com/freelw/linux_learn_diary/blob/master/amhello/readme.md)

* 20210527 todo

    研究kdump

        研究crash工具

* 20210803

    [perf编译相关](https://github.com/freelw/linux_learn_diary/tree/master/perflearn)
    研究crash工具

* 20220305

    研究 cdev

* 20220318 todo

    研究seq_file

    [把.clang-format放到工程中实现自动format](https://github.com/freelw/linux_learn_diary/blob/master/.clang-format)
    
    [使用方法](https://zhuanlan.zhihu.com/p/356143396)

    [seq_file_private_demo](https://github.com/freelw/linux_learn_diary/tree/master/seq_file_private_demo)

* 20220324 

    [globalmem 并发控制](https://github.com/freelw/linux_learn_diary/tree/master/cdev)

* 20220402

    [perf 火焰图学习](https://github.com/freelw/linux_learn_diary/tree/master/perf_flame)

* 20220404

    [cdev 阻塞与非阻塞io学习](https://github.com/freelw/linux_learn_diary/tree/master/cdev_fifo)

* 20220406

    [cdev aio学习](https://github.com/freelw/linux_learn_diary/tree/master/cdev_fifo_aio)

* 20220406

    [使用伪终端消除标准io库在使用管道时的4k buffer问题](https://github.com/freelw/linux_learn_diary/tree/master/pty_test)

* 20220426

    [tcpdump 笔记](https://github.com/freelw/linux_learn_diary/tree/master/tcpdump)

* 20220427

    [scf demo](https://github.com/freelw/linux_learn_diary/tree/master/scf)

    [shell garp](https://github.com/freelw/linux_learn_diary/tree/master/shell_garp)

* 20220507

    [platform miscdevice 学习](https://github.com/freelw/linux_learn_diary/tree/master/cdev_fifo_platform_misc)

    驱动实验环境
    Linux VM-0-13-ubuntu 5.4.0-90-generic #101-Ubuntu SMP Fri Oct 15 20:00:55 UTC 2021 x86_64 x86_64 x86_64 GNU/Linux

* 20220511

    [mac打开多个wireshark窗口](https://github.com/freelw/linux_learn_diary/tree/master/mac_multi_wireshark_window)

* 20220601

    [一个wireshark 过滤mysql参数的过滤器](https://github.com/freelw/linux_learn_diary/tree/master/wireshark_mysql)

* 20220607

    [wireshark 过滤出子机没有dns响应的query](https://github.com/freelw/linux_learn_diary/tree/master/wireshark_dns)

* 20220612

    [在服务器运行opengl程序，在客户端渲染](https://github.com/freelw/linux_learn_diary/tree/master/opengl_xforwarding)

* 20220620

    [dpdk 学习](https://github.com/freelw/linux_learn_diary/tree/master/dpdk_study)
    
    [icmp 没有reply的过滤器](https://github.com/freelw/linux_learn_diary/tree/master/wireshark_icmp)

    [meson 学习](https://github.com/freelw/linux_learn_diary/tree/master/meson_study)

* 20220627

    [tracepoint 学习](https://blog.csdn.net/rikeyone/article/details/116057261) doing

    [droptrace 学习](https://github.com/OpenCloudOS/nettrace/tree/future/droptrace) doing

* 20220628

    [ubuntu解压vmlinuz方法](https://github.com/freelw/linux_learn_diary/tree/master/ubuntu_vmlinux)

* 20220706

    [腾讯云创建辅助网卡实现源进源出](https://blog.csdn.net/LiWang112358/article/details/125647202?spm=1001.2014.3001.5502)

* 20220714

    [tencent os 源码学习环境搭建](https://github.com/freelw/linux_learn_diary/tree/master/linux_kernel_env)

* 20220719

    [numactl 内存速率测试](https://github.com/freelw/linux_learn_diary/tree/master/numactl)

* 20220726

    todo:

    [trace event](https://blog.csdn.net/u012849539/article/details/106771143)
    
    [lwn tracepoint](https://lwn.net/Articles/379903/)

    [tracepoint](https://blog.csdn.net/u012849539/article/details/106750627)

* 20220729

    [tracepoint_demo](https://github.com/freelw/linux_learn_diary/tree/master/tracepoint/demo)

* 20220803
    
    [linux from scratch 流水账](https://github.com/freelw/linux_learn_diary/tree/master/lfs)

    完成11.1版本的包迁移到腾讯云cos上

* 20220803

    [使用autossh 长期保持反向连接](https://github.com/freelw/linux_learn_diary/tree/master/autossh)

* 20220906

    好文章
    [TCP 半连接队列和全连接队列满了会发生什么？又该如何应对？](https://www.cnblogs.com/xiaolincoding/p/12995358.html)

* 20220915

    linux 监控文件 inotify_add_watch

* 20220920

    [批量将xps_cpus 绑定队列](https://github.com/freelw/linux_learn_diary/tree/master/xps_cmd_gen)

* 20221018

    [crash工具使用](https://www.jianshu.com/p/ad03152a0a53)

* 20221019

    查看进程调度器 命令行 pid
    ps -eo class,cmd,pid

    查看vruntime
    cat /proc/sched_debug
    cat /proc/$pid/sched

* 20221020

    [cscope使用](https://github.com/freelw/linux_learn_diary/tree/master/cscope)

* 20221024
    
    [D状态检测](https://github.com/freelw/linux_learn_diary/tree/master/traceD)
    [tencenos debuginfo](http://mirrors.tencent.com/os/tlinux_public/kernel/x86_64/x86-5.4.119-19-0009.1/)

* 20221101

    [sched 调度相关](https://github.com/freelw/linux_learn_diary/tree/master/sched_analyze)

* 20221105

    [cgroup 1](https://www.cnblogs.com/sparkdev/p/8296063.html)
    [cgroup 2](https://www.cnblogs.com/sparkdev/p/9523194.html)

* 20221207

    [ipsec xfrm](https://workerwork.github.io/posts/xfrm/)

* 20221220

    [perf_event_open](https://man7.org/linux/man-pages/man2/perf_event_open.2.html)
    PERF_COUNT_HW_INSTRUCTIONS 可以查看一段时间运行的指令数量，见最后的c程序demo

* 20221221
    
    [BPF CO-RE 示例代码解析](https://www.cnblogs.com/charlieroro/p/14357802.html)
    bpftool btf dump file /sys/kernel/btf/vmlinux format c 输出当前内核的所有数据结构定义

* 20221222

    [ebpf & bcc 中文教程及手册](https://blog.cyru1s.com/posts/ebpf-bcc.html)

* 20230104

    [linux文件系统基础--VFS中的file、dentry和inode--讲得非常透的一篇文章](https://blog.csdn.net/jinking01/article/details/105109089)

* 20230202

    [使用pv nc 限速传输测试](https://github.com/freelw/linux_learn_diary/tree/master/pv_nc)

* 20230329

    [folly 编译方法](https://github.com/freelw/linux_learn_diary/tree/master/folly_learn)

* 20230724

    [malloc vs mmap 内存首地址布局](https://github.com/freelw/linux_learn_diary/tree/master/mallocvsmmap)

* 20230926

    [从velox代码中抄下来的Perf.h](https://github.com/freelw/linux_learn_diary/tree/master/my_perf)

* 20231107

    [asan 内存定位工具使用](https://github.com/freelw/linux_learn_diary/tree/master/asan_test)

* 20241022

    [flink-cdc schema evolution 详解](https://github.com/freelw/linux_learn_diary/tree/master/flink-cdc-learn)

* 20241107

    [gdb pretty print demo](https://github.com/freelw/gdbprettyprintdemo)

* 20241114

    [FlinkPipelineComposer 详解](https://github.com/freelw/linux_learn_diary/tree/master/flink-pipeline-composer)

    [Flink Source 详解](https://github.com/freelw/linux_learn_diary/tree/master/flink-source)

* 20241211

    [spark-sql 写 iceberg](https://github.com/freelw/linux_learn_diary/tree/master/spark-sql-iceberg)

* 20250731
    
    [cmake 学习](cmake-play/readme.md)
    [ldconfig & so name](https://www.baeldung.com/linux/shared-object-filenames)
    [ldconfig-playground](https://github.com/freelw/ldconfig-playground)