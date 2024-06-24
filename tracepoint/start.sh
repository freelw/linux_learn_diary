#!/bin/bash
cat /sys/kernel/debug/tracing/trace  | grep -v KVM | grep -v vhost | python filter.py |grep -v swapper |grep -v qemu