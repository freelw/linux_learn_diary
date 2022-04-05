# 阻塞与非阻塞io学习

    sudo mknod /dev/globalmem0 c 230 0
    sudo mknod /dev/globalmem1 c 230 1
    sudo mknod /dev/globalmem9 c 230 9
    sudo mknod /dev/globalmem10 c 230 10

/dev/globalmem10 无法读写，原因是ko里面只有0-9 10个cdev

    echo 123 > /dev/globalmem0
    cat /dev/globalmem0

ko 加载之后 `/sys/module/hello/` 这里存放了内核模块当前的各种属性

cat /dev/globalmem0 如果没有数据的时候会阻塞住，这时往设备文件中写数据，可以看到cat进程有输出，说明io阻塞逻辑生效了

测试poll逻辑的方法

    1. 运行select_demo/globalmem_poll 一直打印 'Poll monitor: can be written'
    2. echo 123 > /dev/globalmem0 交替打印'Poll monitor: can be read Poll monitor: can be written'
    3. 运行 dd if=/dev/zero of=/dev/globalmem0 一直打印 'Poll monitor: can be read’
    4. 运行 cat /dev/globalmem0 一直打印 'Poll monitor: can be written'