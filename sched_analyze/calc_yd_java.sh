#!/bin/

echo 'count'
cat /proc/trace_r/trace |grep -i java |wc -l
cat /proc/trace_r/trace |grep -i ydservice |wc -l
echo 'group by thread'
cat /proc/trace_r/trace |grep -i java | awk '{print $5}' | sort | uniq -c | wc -l
cat /proc/trace_r/trace |grep -i ydservice | awk '{print $5}' | sort | uniq -c | wc -l