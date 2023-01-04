from bcc import BPF
BPF(text='int kprobe__sys_sync(void *ctx) { bpf_trace_printk("sync\\n"); return 0; }').trace_print()
