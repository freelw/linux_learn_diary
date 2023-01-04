#!/usr/bin/python
# 2019 by Cyrus

from __future__ import print_function
from bcc import BPF
from bcc.utils import printb

# load BPF program
b = BPF(text="""
#include <uapi/linux/ptrace.h>
struct data_t {
    u32 pid;
    u64 delta;
    u64 time;
};
BPF_HASH(last);
BPF_PERF_OUTPUT(result);
int do_trace(struct pt_regs *ctx) {
    u64 ts, *tsp, delta, key = 0;
    struct data_t data = {};
    // attempt to read stored timestamp
    tsp = last.lookup(&key);
    if (tsp != 0) {
        delta = bpf_ktime_get_ns() - *tsp;
        if (delta < 1000000000) {
            // output if time is less than 1 second
            data.pid = bpf_get_current_pid_tgid();
            data.delta = delta / 1000000;
            data.time = bpf_ktime_get_ns();
            result.perf_submit(ctx, &data, sizeof(data));
        }
        last.delete(&key);
    }
    // update stored timestamp
    ts = bpf_ktime_get_ns();
    last.update(&key, &ts);
    return 0;
}
""")

b.attach_kprobe(event=b.get_syscall_fnname("sync"), fn_name="do_trace")
print("Tracing for quick sync's... Ctrl-C to end")

# format output
start = 0
def print_event(cpu, data, size):
    global start
    event = b["result"].event(data)
    if start == 0:
        start = int(event.time)
    start_time = (int(event.time) - start) / 1000000;
    print("[PID:%6s] At time %d ms: multiple syncs detected, last %s ms ago" % (event.pid, start_time, event.delta))
b["result"].open_perf_buffer(print_event)

while 1:
    try:
        b.perf_buffer_poll()
    except KeyboardInterrupt:
        exit()
