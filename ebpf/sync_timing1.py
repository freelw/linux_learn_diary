#!/usr/bin/python
# 2019 by Cyrus

from __future__ import print_function
from bcc import BPF
from bcc.utils import printb

# load BPF program
b = BPF(text="""
#include <uapi/linux/ptrace.h>
BPF_HASH(last);
int do_trace(struct pt_regs *ctx) {
    u64 ts, *tsp, *cnt, delta, this_cnt, key = 0, cnt_key = 1;
    cnt = last.lookup(&cnt_key);
    if (cnt == 0) {
        this_cnt = 1;
    } else {
        this_cnt = ++*cnt;
        last.delete(&cnt_key);
    }
    last.update(&cnt_key, &this_cnt);
    // attempt to read stored timestamp
    tsp = last.lookup(&key);
    if (tsp != 0) {
        delta = bpf_ktime_get_ns() - *tsp;
        if (delta < 1000000000) {
            // output if time is less than 1 second
            bpf_trace_printk("%d,%d\\n", this_cnt, delta / 1000000);
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
print("Tracing and counting for sync's... Ctrl-C to end")

# format output
start = 0
while 1:
    try:
        (task, pid, cpu, flags, ts, msg) = b.trace_fields()
        [cnt, ms] = msg.split(b',')
        if start == 0:
            start = ts
        ts = ts - start
        printb(b"At time %.2f s: %s syncs detected, last %s ms ago" % (ts, cnt, ms))
    except KeyboardInterrupt:
        exit()
