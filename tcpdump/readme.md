# tcpdump使用

    tcpdump -i eth0 -s0 -G 1200 -C 500 -Z root host 169.254.0.47 -w %Y_%m%d_%H%M_%S.pcap
    -s0 表示不截断包，默认是64字节
    -G 表示每1200s改变一下文件名，用来滚动
    -C 表示每500M滚动一个文件，这里1M是1000,000


    切割pcap文件
    editcap -c 10000  路径\src.pcap  路径\dest.pcap
    参数说明：10000是切割后的pcap文件中每个文件中所包含的包的个数，路径\src.pcap是要被切割的pcap文件，路径\dest.pcap指定切割后的pcap文件的路径和文件名，会自动加编号。
