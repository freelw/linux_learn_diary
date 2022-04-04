# 阻塞与非阻塞io学习

    sudo mknod /dev/globalmem0 c 230 0
    sudo mknod /dev/globalmem1 c 230 1
    sudo mknod /dev/globalmem9 c 230 9
    sudo mknod /dev/globalmem10 c 230 10

/dev/globalmem10 无法读写，原因是ko里面只有0-9 10个cdev

    echo 123 > /dev/globalmem0
    cat /dev/globalmem0

ko 加载之后 `/sys/module/hello/` 这里存放了内核模块当前的各种属性