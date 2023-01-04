
## 查看tcp状态统计
    
    netstat -a | awk '/^tcp/ {++S[$NF]} END {for(a in S) print a, S[a]}'

    比如
    [root@VM-0-13-centos test]# netstat -a | awk '/^tcp/ {++S[$NF]} END {for(a in S) print a, S[a]}'
    LISTEN 5
    CLOSE_WAIT 1
    ESTABLISHED 9