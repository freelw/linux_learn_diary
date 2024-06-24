from __future__ import print_function
from bcc import BPF
from time import sleep

# load BPF program
b = BPF(text="""
#include <uapi/linux/ptrace.h>

struct key_t {
    char c[80];
};
BPF_HASH(counts, struct key_t);

int count(struct pt_regs *ctx) {
    if (!PT_REGS_PARM1(ctx))
        return 0;

    struct key_t key = {};
    u64 zero = 0, *val;

    bpf_probe_read(&key.c, sizeof(key.c), (void *)PT_REGS_PARM1(ctx));
    // could also use `counts.increment(key)`
    val = counts.lookup_or_init(&key, &zero);
    (*val)++;
    return 0;
};
int ok(struct pt_regs *ctx) {
bpf_trace_printk("ok\\n");
return 0;
}
""")
b.attach_uprobe(name="c", sym="strlen", fn_name="count")
b.attach_uprobe(name="/root/test/a.out", sym="ok", fn_name="ok")

# header
print("Tracing strlen()... Hit Ctrl-C to end.")

while 1:
    try:
        (task, pid, cpu, flags, ts, msg) = b.trace_fields()
    except ValueError:
        continue
    print("%-18.9f %-16s %-6d %s" % (ts, task, pid, msg))

