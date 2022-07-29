#!/bin/bash
echo 1 > /sys/kernel/debug/tracing/events/wangli/wangli_test/enable
cat /sys/kernel/debug/tracing/trace_pipe