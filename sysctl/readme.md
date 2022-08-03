# tcp mem 调优

    查看
    sysctl -a 2>/dev/null | grep 'net.*mem'


    sysctl -w net.core.netdev_max_backlog=5000

    sysctl -w net.core.rmem_max=33554432
    sysctl -w net.core.rmem_default=33554432
    sysctl -w net.core.wmem_max=33554432
    sysctl -w net.core.wmem_default=33554432

    sysctl -w net.ipv4.tcp_mem='262144 524288 20971520'
    sysctl -w net.ipv4.tcp_rmem='17520 524288 20971520'
    sysctl -w net.ipv4.tcp_wmem='17520 524288 20971520'