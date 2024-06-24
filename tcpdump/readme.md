# tcpdump使用

    tcpdump -i eth0 -s0 -G 1200 -C 500 -Z root host 169.254.0.47 -w %Y_%m%d_%H%M_%S.pcap
    -s0 表示不截断包，默认是64字节
    -G 表示每1200s改变一下文件名，用来滚动
    -C 表示每500M滚动一个文件，这里1M是1000,000


    切割pcap文件
    editcap -c 10000  路径\src.pcap  路径\dest.pcap
    参数说明：10000是切割后的pcap文件中每个文件中所包含的包的个数，路径\src.pcap是要被切割的pcap文件，路径\dest.pcap指定切割后的pcap文件的路径和文件名，会自动加编号。

    按照时间切割

    ./editcap -A ' 2022-05-07 11:47:48' -B '2022-05-07 11:47:49' 2022_0507_1145_07.pcap9 out.pcap

    合并pcap文件
    mergecap -w destfile sourcefile1 sourcefile2 

    使用tcpdump 读取抓包的文件
    tcpdump -tttr *
    其中不同的t代表不同的打印时间的格式
    -t 在每行输出中不打印时间戳
    -tt 不对每行输出的时间进行格式处理(nt: 这种格式一眼可能看不出其含义, 如时间戳打印成1261798315)
    -ttt tcpdump 输出时, 每两行打印之间会延迟一个段时间(以毫秒为单位)
    -tttt 在每行打印的时间戳之前添加日期的打印

# 抓lacp报文

    tcpdump -i enP8p1s0f2 ether proto 0x8809

# tcpdump 直接过滤rst的包

    tcpdump "tcp[tcpflags] & (tcp-rst) != 0" and host 169.254.0.47 -r  a.pcap

