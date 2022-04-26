# tcpdump使用

    tcpdump -i eth0 -s0 -G 1200 -C 500 -Z root host 169.254.0.47 -w %Y_%m%d_%H%M_%S.pcap
    -s0 表示不截断包，默认是64字节
    -G 表示每1200s改变一下文件名，用来滚动
    -C 表示每500M滚动一个文件，这里1M是1000,000
