#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>

#define CREATE_TRACE_POINTS
#include "tracepoint_demo.h"

static int wangli_thread(void *arg) {
    static unsigned long count;
    while (!kthread_should_stop()) {
        set_current_state(TASK_INTERRUPTIBLE);
        schedule_timeout(HZ);
        trace_wangli_test(count);
        count++;
    }
    return 0;
}

static struct task_struct *wangli_task;

static __init int __module_init(void) {
    printk("Hello, %s.\n", __func__);
    wangli_task = kthread_run(wangli_thread, NULL, "wangli-thread");
    if (IS_ERR(wangli_task)) {
        return -1;
    }
    return 0;
}

static __exit void __module_exit(void) {
    kthread_stop(wangli_task);
    printk("Bye, %s.\n", __func__);
}

module_init(__module_init);
module_exit(__module_exit);

MODULE_LICENSE("GPL");