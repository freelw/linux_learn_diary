#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/module.h>
#include <linux/sizes.h>
#include <linux/version.h>
#include <linux/kprobes.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <trace/events/sched.h>
#include <asm/irq_regs.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/timekeeping.h>
#include <linux/tracepoint.h>

#include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/sched/stat.h>
#include <linux/sched/loadavg.h>
#include <linux/percpu-defs.h>
#include <asm-generic/percpu.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("johnazhang");

#define MAX_SYMBOL_LEN		64
#define PROC_DIR_TRACE_R	"trace_r"

// same as dmesg buffer, TODO: check __LOG_BUF_LEN to vary between machines
#define BUFFER_SIZE 524288

extern spinlock_t vring_lock;
extern char* vring_buffer;
extern char* show_buffer;
extern unsigned int vring_off;
extern u8 vring_overflowed;

extern u64 load_qos;
extern u64 running_qos;
extern u64 *calc_load_tasks_local;
//struct task_struct *curr;
extern u8 print_on;
extern u8 enable;

extern char symbol_ttwu_do_wakeup[MAX_SYMBOL_LEN];
extern char symbol_wake_up_new_task[MAX_SYMBOL_LEN];
extern struct kprobe kp_ttwu_do_wakeup;
extern struct kprobe kp_wake_up_new_task;

void vring_write_buffer(const char *buffer, ...);
int hook_ttwu_do_wakeup(struct kprobe *kp, struct pt_regs *regs);
int pre_wake_up_new_task(struct kprobe *kp, struct pt_regs *regs);

#if LINUX_VERSION_CODE == KERNEL_VERSION(4, 14, 105)
struct rq_local {
    raw_spinlock_t lock;
    unsigned int nr_running;
    unsigned int nr_numa_running;
    unsigned int nr_preferred_running;
    unsigned long cpu_load[5];
    unsigned long last_load_update_tick;
    unsigned long nohz_flags;
    struct load_weight load;
    unsigned long nr_load_updates;
    u64 nr_switches;
    //struct cfs_rq cfs;
    u8 cfs[312];
    //struct rt_rq rt;
    u8 rt[1728];
    //struct dl_rq dl;
    u8 dl[104];
    struct list_head leaf_cfs_rq_list;
    struct list_head *tmp_alone_branch;
    unsigned long nr_uninterruptible;
    struct task_struct *curr;
    struct task_struct *idle;
    struct task_struct *stop;
    unsigned long next_balance;
    struct mm_struct *prev_mm;
    unsigned int clock_update_flags;
    u64 clock;
    u64 clock_task;
    atomic_t nr_iowait;
    struct root_domain *rd;
    struct sched_domain *sd;
    unsigned long cpu_capacity;
    unsigned long cpu_capacity_orig;
    struct callback_head *balance_callback;
    unsigned char idle_balance;
    int active_balance;
    int push_cpu;
    //struct cpu_stop_work active_balance_work;
    u8 active_balance_work[40];
    int cpu;
    int online;
    struct list_head cfs_tasks;
    u64 rt_avg;
    u64 age_stamp;
    u64 idle_stamp;
    u64 avg_idle;
    u64 max_idle_balance_cost;
    u64 prev_steal_time;
    u64 prev_steal_time_rq;
    unsigned long calc_load_update;
    long calc_load_active;
    int hrtick_csd_pending;
    call_single_data_t hrtick_csd;
    struct hrtimer hrtick_timer;
    struct sched_info rq_sched_info;
    unsigned long long rq_cpu_time;
    unsigned int yld_count;
    unsigned int sched_count;
    unsigned int sched_goidle;
    unsigned int ttwu_count;
    unsigned int ttwu_local;
    struct llist_head wake_list;
    struct cpuidle_state *idle_state;
};

extern struct rq_local* runqueues_local;

// TODO: other kernel versions
#else 
#endif
