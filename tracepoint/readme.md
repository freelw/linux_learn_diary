# 查看一组cpu被除了qemu以外的进程占用情况

    1. echo 1 > /sys/kernel/debug/tracing/events/sched/sched_switch/enable
    2. bash start.sh