# iperf udp压测

    客户端
    iperf -c ${ip} -b 25000M -P 32 -u -t 300

    服务端
    iperf -s

    看客户端的带宽不能说明问题，因为udp不一定100%发送到，所以要看服务端带宽
    sar -n DEV 1

    看runq长度 和load avg
    sar -q 1

# iperf3 压测结果分析脚本

    work.py