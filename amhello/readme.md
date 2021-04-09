# autotools 学习笔记

## 试验步骤

[官方步骤](https://www.gnu.org/software/automake/manual/automake.html#Creating-amhello)

## 关于 configure.ac 和 Makefile.am 的理解

    20210409 先写下来一些想法明天继续补充
    
    信息流向

    目录树 ---> Makefile.am列表

    configure.ac 包含 Makefile.am列表

    autoreconf 调用autoconf和automake以及一些其他命令

    autoconf 将 configure.ac 生成 configure 可执行文件，这时 configure中包含了 Makefile.in列表 信息
    所以才能在执行的时候扫描每个 Makefile.in 文件

    automake 需要依赖 configure.ac 因为其中包含了Makefile.am列表信息
    automake 将所有的 Makefile.am 文件生成为 Makefile.in

    configure 在真正执行的时候，用执行时候的传入的环境变量，加上config.site的环境变量，共同填充所有的 Makefile.in 和 config.h.in 文件

    最终生成的顶层 Makefile 文件会知道目录树中还有多少Makefile文件，这样才能在执行make的时候递归执行

![](https://raw.githubusercontent.com/freelw/linux_learn_diary/master/amhello/img/autotool-min.png)
