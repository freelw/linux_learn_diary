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

* 20220305

    研究 cdev

* 20220318 todo

    研究seq_file

    [把.clang-format放到工程中实现自动format](https://gitee.com/freelw/linux_learn_diary/blob/master/.clang-format)
    
    [使用方法](https://zhuanlan.zhihu.com/p/356143396)

    [seq_file_private_demo](https://gitee.com/freelw/linux_learn_diary/tree/master/seq_file_private_demo)

* 20220324 

    [globalmem 并发控制](https://gitee.com/freelw/linux_learn_diary/tree/master/cdev)

* 20220402

    [perf 火焰图学习](https://gitee.com/freelw/linux_learn_diary/tree/master/perf_flame)

* 20220404

    [cdev 阻塞与非阻塞io学习](https://gitee.com/freelw/linux_learn_diary/tree/master/cdev_fifo)

* 20220406

    [cdev aio学习](https://gitee.com/freelw/linux_learn_diary/tree/master/cdev_fifo_aio)

* 20220406

    [使用伪终端消除标准io库在使用管道时的4k buffer问题](https://gitee.com/freelw/linux_learn_diary/tree/master/pty_test)

* 20220426

    [tcpdump 笔记](https://gitee.com/freelw/linux_learn_diary/tree/master/tcpdump)

* 20220427

    [scf demo](https://gitee.com/freelw/linux_learn_diary/tree/master/scf)

    [shell garp](https://gitee.com/freelw/linux_learn_diary/tree/master/shell_garp)

* 20220507

    [platform miscdevice 学习](https://gitee.com/freelw/linux_learn_diary/tree/master/cdev_fifo_platform_misc)

    驱动实验环境
    Linux VM-0-13-ubuntu 5.4.0-90-generic #101-Ubuntu SMP Fri Oct 15 20:00:55 UTC 2021 x86_64 x86_64 x86_64 GNU/Linux

* 20220511

    [mac打开多个wireshark窗口](https://gitee.com/freelw/linux_learn_diary/tree/master/mac_multi_wireshark_window)

* 20220601

    [一个wireshark 过滤mysql参数的过滤器](https://gitee.com/freelw/linux_learn_diary/tree/master/wireshark_mysql)

* 20220607

    [wireshark 过滤出子机没有dns响应的query](https://gitee.com/freelw/linux_learn_diary/tree/master/wireshark_dns)

* 20220612

    [在服务器运行opengl程序，在客户端渲染](https://gitee.com/freelw/linux_learn_diary/tree/master/opengl_xforwarding)

* 20220620

    [dpdk 学习](https://gitee.com/freelw/linux_learn_diary/tree/master/dpdk_study)
    
    [icmp 没有reply的过滤器](https://gitee.com/freelw/linux_learn_diary/tree/master/wireshark_icmp)

    [meson 学习](https://gitee.com/freelw/linux_learn_diary/tree/master/meson_study)

* 20220627

    [tracepoint 学习](https://blog.csdn.net/rikeyone/article/details/116057261) doing

    [droptrace 学习](https://github.com/OpenCloudOS/nettrace/tree/future/droptrace) doing

* 20220628

    [ubuntu解压vmlinuz方法](https://gitee.com/freelw/linux_learn_diary/tree/master/ubuntu_vmlinux)

* 20220706

    [腾讯云创建辅助网卡实现源进源出](https://blog.csdn.net/LiWang112358/article/details/125647202?spm=1001.2014.3001.5502)

* 20220714

    [tencent os 源码学习环境搭建](https://gitee.com/freelw/linux_learn_diary/tree/master/linux_kernel_env)

* 20220719

    [numactl 内存速率测试](https://gitee.com/freelw/linux_learn_diary/tree/master/numactl)

* 20220726

    todo:
    
    [tracepoint](https://blog.csdn.net/u012849539/article/details/106771143)
    [lwn tracepoint](https://lwn.net/Articles/379903/)
