#!/bin/bash
echo 0 > /sys/kernel/debug/tracing/events/sched/sched_wakeup_new/enable
echo 0 > /sys/kernel/debug/tracing/events/sched/sched_wakeup/enable
pkill cat