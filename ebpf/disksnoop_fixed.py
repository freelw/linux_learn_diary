#!/usr/bin/python
# 2019 by Cyrus

from __future__ import print_function
from bcc import BPF

# load BPF program
b = BPF(text="""
BPF_HASH(time, u64);
BPF_HASH(size, u64, int);
TRACEPOINT_PROBE(block, block_rq_complete) {
        u64 pid = (u32)args->dev;
        u64 tsp = bpf_ktime_get_ns();
        u64 *tsp0 = time.lookup(&pid);
        int *blocksize = size.lookup(&pid);
        if (tsp0 != 0 && blocksize != 0) {
                bpf_trace_printk("%d,%d\\n", tsp - *tsp0, *blocksize);
        }
        return 0;
}
TRACEPOINT_PROBE(block, block_rq_issue) {
        u64 pid = args->dev;
        u64 tsp = bpf_ktime_get_ns();
        time.update(&pid, &tsp);
        int blocksize = args->bytes;
        size.update(&pid, &blocksize);
        return 0;
}
""")

# header
print("%-18s %-8s %-10s %s" % ("TIME(s)", "PID", "DELTA(ms)", "BLOCKSIZE(bytes)"))

# format output
while 1:
        try:
                (task, pid, cpu, flags, ts, msg) = b.trace_fields()
                delta, size = msg.decode().split(",")
        except ValueError:
                continue
        print("%-18.9f %-8s %-10d %s" % (ts, pid, int(delta)/100000, size))
