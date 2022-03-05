#!/bin/bash
pid=$1
sudo nohup unbuffer strace -tt -e trace=network -p ${pid} > strace_net.log &
