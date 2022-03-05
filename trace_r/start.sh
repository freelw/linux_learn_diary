#!/bin/bash
make clean
make
make remove
sync
make install
echo 1 > /proc/trace_r/enable
cat /proc/trace_r/trace