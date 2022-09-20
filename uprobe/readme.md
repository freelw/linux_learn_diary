echo 'r:wangli2 ./uprobe_test:0x1126' > /sys/kernel/debug/tracing/uprobe_events
echo 'p:wangli1 ./uprobe_test:0x1126' >> /sys/kernel/debug/tracing/uprobe_events
cat /sys/kernel/debug/tracing/uprobe_events
echo 1 >/sys/kernel/debug/tracing/events/uprobes/enable