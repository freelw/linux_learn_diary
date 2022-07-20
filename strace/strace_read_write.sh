#!/bin/bash
# -T 打印系统调用耗时
# -tt 打印系统调用发生时间
# -yy 解析fd详细信息
# -e trace=xxx 规定追踪的系统调用
# -p 规定追踪的进程id

strace -T -tt -yy -e trace=read,write -p $1