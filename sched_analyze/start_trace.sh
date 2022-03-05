#!/bin/bash

echo  'comm == "java"' > /sys/kernel/debug/tracing/events/sched/sched_wakeup_new/filter
echo  'comm == "java"' > /sys/kernel/debug/tracing/events/sched/sched_wakeup/filter
echo 1 > /sys/kernel/debug/tracing/events/sched/sched_wakeup_new/enable
echo 1 > /sys/kernel/debug/tracing/events/sched/sched_wakeup/enable

nohup cat /sys/kernel/debug/tracing/trace_pipe > wakeup_trace.log &