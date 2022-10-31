
#include "header.h"

extern wait_queue_head_t r_wait;
char symbol_ttwu_do_wakeup[MAX_SYMBOL_LEN] = "ttwu_do_wakeup";
module_param_string(symbol_ttwu_do_wakeup, symbol_ttwu_do_wakeup, sizeof(symbol_ttwu_do_wakeup), 0644);
struct kprobe kp_ttwu_do_wakeup = {
    .symbol_name    = symbol_ttwu_do_wakeup,
};

char symbol_wake_up_new_task[MAX_SYMBOL_LEN] = "wake_up_new_task";
module_param_string(symbol_wake_up_new_task, symbol_wake_up_new_task, sizeof(symbol_wake_up_new_task), 0644);
struct kprobe kp_wake_up_new_task = {
    .symbol_name    = symbol_wake_up_new_task,
};

int hook_ttwu_do_wakeup(struct kprobe *kp, struct pt_regs *regs)
{
        return 0;
	struct task_struct *p = (struct task_struct *)regs->si;
	u64 curr_time = ktime_get_real_ns();

	/* A feature to support per cpu runq qos, but may CRASH here? */
#if LINUX_VERSION_CODE == KERNEL_VERSION(4, 14, 105)
	struct rq_local *this_rq = (struct rq_local *)regs->di;

	if (*calc_load_tasks_local > load_qos || this_rq->nr_running > running_qos)
#else
	if (*calc_load_tasks_local > load_qos)
#endif
	{
		if (!print_on)
			vring_write_buffer( "[%ld] ======== High load occurs ========\n", curr_time);
		print_on = 1;
		vring_write_buffer("[%ld] Task woke, pid: %d, comm: %s\n", curr_time, p->pid, p->comm);
	}
	else
	{
		if (print_on)
			vring_write_buffer("[%ld] ======== High load ends ========\n", curr_time);	
		print_on = 0;
	}
        wake_up_interruptible(&r_wait);
	return 0;
}

/*
static void post_wake_up_new_task(struct kprobe *kp, struct pt_regs *regs, unsigned long flags)
{
	curr = (struct task_struct *)regs->bx;
}
*/

int pre_wake_up_new_task(struct kprobe *kp, struct pt_regs *regs)
{
        return 0;
	struct task_struct *p = (struct task_struct *)regs->di;  
        u64 curr_time = ktime_get_real_ns();
        
#if LINUX_VERSION_CODE == KERNEL_VERSION(4, 14, 105)
        struct rq_local *this_rq = (struct rq_local*)((unsigned long)__per_cpu_offset[task_cpu(p)] + (unsigned long)runqueues_local);

        if (*calc_load_tasks_local > load_qos || this_rq->nr_running > running_qos)
#else
        if (*calc_load_tasks_local > load_qos)
#endif
        {
                if (!print_on)
                        vring_write_buffer("[%ld] ======== High load occurs ========\n", curr_time);
                print_on = 1;
                vring_write_buffer("[%ld] Task created, A task is created but cannot been determined\n", curr_time);
	}
        else
        {
                if (print_on)
                        vring_write_buffer("[%ld] ======== High load ends ========\n", curr_time);
                print_on = 0;
        }
        wake_up_interruptible(&r_wait);	
	return 0;


        /* A feature to support per cpu runq qos, but may CRASH here? */
#if LINUX_VERSION_CODE == KERNEL_VERSION(4, 14, 105)
        if (*calc_load_tasks_local > load_qos || this_rq->nr_running > running_qos)
#else
        if (*calc_load_tasks_local > load_qos)
#endif
        {
                if (!print_on)
                        vring_write_buffer("[%ld] ======== High load occurs ========\n", curr_time);
                print_on = 1;
                vring_write_buffer("[%ld] Task created, pid: %d, comm: %s\n", curr_time, p->pid, p->comm);
        }
        else
        {
                if (print_on)
                        vring_write_buffer("[%ld] ======== High load ends ========\n", curr_time);
                print_on = 0;
        }
        wake_up_interruptible(&r_wait);
        return 0;
}