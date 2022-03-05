# 获取系统启动时间

    date | cat > /dev/null # Make sure programs are in cache
    set -- $(date +%s.%N) $(cat /proc/uptime)
    echo "$1-$2" | bc

[获取系统启动时间](https://unix.stackexchange.com/questions/329742/how-to-get-epoch-time-with-trace-cmd-frontend-for-ftrace)