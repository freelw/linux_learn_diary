strace -e 'trace=!all'
可以只追踪信号

strace -f -tt -e open -p ${pid}
追踪open
-f 是跟踪子进程