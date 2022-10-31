
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
        printk(KERN_INFO "hook_ttwu_do_wakeup calc_load_tasks_local %d %d\n", *calc_load_tasks_local, load_qos);
	struct task_struct *p = (struct task_struct *)regs->si;
	u64 curr_time = ktime_get_real_ns();
	if (*calc_load_tasks_local > load_qos)
	{
                printk(KERN_INFO "hook_ttwu_do_wakeup enter\n");
		if (!print_on)
			vring_write_buffer( "[%ld] ======== High load occurs ========\n", curr_time);
		print_on = 1;
		vring_write_buffer("[%ld] Task woke, pid: %d, comm: %s\n", curr_time, p->pid, p->comm);
                // wake_up_interruptible(&r_wait);
	}
	else
	{
		if (print_on) {
                        vring_write_buffer("[%ld] ======== High load ends ========\n", curr_time);
                        // wake_up_interruptible(&r_wait);
                }	
		print_on = 0;
	}
	return 0;
}



int pre_wake_up_new_task(struct kprobe *kp, struct pt_regs *regs)
{
	struct task_struct *p = (struct task_struct *)regs->di;  
        u64 curr_time = ktime_get_real_ns();
        
        printk(KERN_INFO "pre_wake_up_new_task calc_load_tasks_local %d\n", *calc_load_tasks_local);
        if (*calc_load_tasks_local > load_qos)

        {
                
                if (!print_on)
                        vring_write_buffer("[%ld] ======== High load occurs ========\n", curr_time);
                print_on = 1;
                vring_write_buffer("[%ld] Task created, A task is created but cannot been determined\n", curr_time);
                // wake_up_interruptible(&r_wait);
	}
        else
        {
                if (print_on) {
                        vring_write_buffer("[%ld] ======== High load ends ========\n", curr_time);
                        // wake_up_interruptible(&r_wait);
                }
                print_on = 0;
        }
	return 0;
}