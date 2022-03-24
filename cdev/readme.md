# 测试方法

    sudo mknod /dev/globalmem0 c 230 0
    sudo mknod /dev/globalmem1 c 230 1
    sudo mknod /dev/globalmem9 c 230 9
    sudo mknod /dev/globalmem10 c 230 10

/dev/globalmem10 无法读写，原因是ko里面只有0-9 10个cdev

    echo 123 > /dev/globalmem0
    cat /dev/globalmem0