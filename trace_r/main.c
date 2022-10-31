
#include "header.h"


struct mutex trace_mutex;
wait_queue_head_t r_wait;
spinlock_t vring_lock;
spinlock_t vring_lock;
unsigned long flags;
char* vring_buffer;
char* show_buffer;
unsigned int vring_off = 0;
u8 vring_overflowed = 0;

u64 load_qos = 10;
u64 running_qos = 10;
u64 *calc_load_tasks_local;

#if LINUX_VERSION_CODE == KERNEL_VERSION(4, 14, 105)
struct rq_local* runqueues_local;
#else
#endif

u8 print_on = 0;
u8 enable = 0;

void vring_write_buffer(const char *buffer, ...)
{
	int len = 0;
	va_list args1, args2;
	
	va_start(args1, buffer);
	va_copy(args2, args1);
	
	spin_lock_irqsave(&vring_lock, flags);
	
	len = vscnprintf(vring_buffer + vring_off, BUFFER_SIZE - vring_off, buffer, args1);
	va_end(args1);

	if (vring_off + len >= BUFFER_SIZE - 1)
	{
		// memset(vring_buffer + vring_off, '*', BUFFER_SIZE - vring_off);
		vring_off = vscnprintf(vring_buffer, BUFFER_SIZE, buffer, args2);
		// vring_overflowed = 1;
	}
	else
	{
		vring_off += len;
	}
	
	vring_buffer[vring_off] = 0;

	spin_unlock_irqrestore(&vring_lock, flags);
	
	va_end(args2);
}
#define DEFINE_BLOCK_PROC_ATTR(name, __write)					\
	static int name##_open(struct inode *inode, struct file *file)	\
	{								\
		return seq_open(file, &trace_r_seq_ops);	\
	}								\
									\
	static const struct file_operations name##_fops = 		\
	{								\
		.owner = THIS_MODULE,					\
		.open = name##_open,					\
		.read = seq_read, 					\
		.write = __write,					\
		.llseek = seq_lseek,					\
		.release = seq_release, 				\
	}

#define DEFINE_PROC_ATTR(name, __write)								\
	static int name##_open(struct inode *inode, struct file *file)	\
	{																\
		return single_open(file, name##_show, PDE_DATA(inode));		\
	}																\
																	\
	static const struct file_operations name##_fops = 				\
	{																\
		.owner = THIS_MODULE,										\
		.open = name##_open,										\
		.read = seq_read, 											\
		.write = __write,											\
		.llseek = seq_lseek,										\
		.release = single_release, 									\
	}

#define DEFINE_PROC_ATTR_RW(name)									\
	static ssize_t name##_write(struct file *file, 					\
				    const char __user *buf, 						\
				    size_t count, loff_t *ppos)						\
	{																\
		return name##_store(PDE_DATA(file_inode(file)), buf, 		\
			count);													\
	}																\
	DEFINE_PROC_ATTR(name, name##_write)

#define DEFINE_PROC_ATTR_RO(name) DEFINE_PROC_ATTR(name, NULL)		\

#define DEFINE_BLOCK_PROC_ATTR_RO(name) DEFINE_BLOCK_PROC_ATTR(name, NULL)		\

static int running_qos_show(struct seq_file *m, void *ptr)
{
    seq_printf(m, "%llu\n", running_qos);

    return 0;
}

static ssize_t running_qos_store(void *priv, const char __user *buf, size_t count)
{
    u64 val;

    if(kstrtou64_from_user(buf, count, 0, &val))
        return -EINVAL;

    running_qos = val;

    return count;
}
DEFINE_PROC_ATTR_RW(running_qos);


static int load_qos_show(struct seq_file *m, void *ptr)
{
	seq_printf(m, "%llu\n", load_qos);

	return 0;
}

static ssize_t load_qos_store(void *priv, const char __user *buf, size_t count)
{
	u64 val;

	if(kstrtou64_from_user(buf, count, 0, &val))
		return -EINVAL;
	
	load_qos = val;
	
	return count;
}
DEFINE_PROC_ATTR_RW(load_qos);

static int enable_show(struct seq_file *m, void *ptr)
{
    seq_printf(m, "%u\n", enable);

    return 0;
}

static ssize_t enable_store(void *priv, const char __user *buf, size_t count)
{
	bool local_enable;

	if (kstrtobool_from_user(buf, count, &local_enable))
		return -EINVAL;

	if (!!enable == !!local_enable)
		return count;

	if (local_enable) 
	{
		// TODO: FIXME bug after echo 1, echo 0, echo 1
        kp_ttwu_do_wakeup.pre_handler = hook_ttwu_do_wakeup;
        kp_wake_up_new_task.pre_handler = pre_wake_up_new_task;
        //kp_wake_up_new_task.post_handler = post_wake_up_new_task;
		
		if (register_kprobe(&kp_ttwu_do_wakeup))
			return -EAGAIN;
        if (register_kprobe(&kp_wake_up_new_task))
			return -EAGAIN;
	} 
	else 
	{
		unregister_kprobe(&kp_ttwu_do_wakeup);
	    unregister_kprobe(&kp_wake_up_new_task);
	}

	enable = local_enable;

	return count;
}
DEFINE_PROC_ATTR_RW(enable);

static void *trace_start(struct seq_file *s, loff_t *pos) {
	printk(KERN_INFO "trace_start\n");
    
	*pos = 0;
    
    return show_buffer;
}


static void trace_stop(struct seq_file *s, void *v) {

}

static void *trace_next(struct seq_file *s, void *v, loff_t *pos) {
	printk(KERN_INFO "trace_next\n");
    DECLARE_WAITQUEUE(wait, current);
    
    add_wait_queue(&r_wait, &wait);
    set_current_state(TASK_INTERRUPTIBLE);
    printk(KERN_INFO "trace sleep\n");
    schedule();
	printk(KERN_INFO "wake up\n");
	if (signal_pending(current)) {
		return NULL;
	}

    spin_lock_irqsave(&vring_lock, flags);
    vring_off = 0;
    spin_unlock_irqrestore(&vring_lock, flags);
	*pos ++;
    return show_buffer;
}

static int trace_show(struct seq_file *m, void *ptr)
{
	spin_lock_irqsave(&vring_lock, flags);
	seq_printf(m, "123\n");
	/*if (!vring_overflowed)
	{
		seq_printf(m, vring_buffer);

		spin_unlock_irqrestore(&vring_lock, flags);

		return 0;
	}*/

	// memcpy(show_buffer, vring_buffer + vring_off + 1, BUFFER_SIZE - vring_off - 1);
	// memcpy(show_buffer + BUFFER_SIZE - vring_off - 1, vring_buffer, vring_off);

	memcpy(show_buffer, vring_buffer, vring_off);
	
	// TODO: BUG here, fix me
	seq_printf(m, show_buffer);
	
	spin_unlock_irqrestore(&vring_lock, flags);

	return 0;
}
static struct seq_operations trace_r_seq_ops = {
	.start = trace_start,
    .stop = trace_stop,
    .next = trace_next,
    .show = trace_show,
};
DEFINE_BLOCK_PROC_ATTR_RO(trace);
// DEFINE_PROC_ATTR_RO(trace);

static int __init trace_r_init(void)
{
	printk(KERN_INFO "trace_r_init\n");
	struct proc_dir_entry *parent_dir;

	calc_load_tasks_local = (u64 *)kallsyms_lookup_name("calc_load_tasks");

#if LINUX_VERSION_CODE == KERNEL_VERSION(4, 14, 105)
	runqueues_local = (struct rq_local *)kallsyms_lookup_name("runqueues");
#else
#endif
	mutex_init(&trace_mutex);
    init_waitqueue_head(&r_wait);
	spin_lock_init(&vring_lock);

	vring_buffer = (char*)vzalloc(sizeof(char) * (BUFFER_SIZE+1));
	show_buffer = (char*)vzalloc(sizeof(char) * (BUFFER_SIZE+1));

	parent_dir = proc_mkdir(PROC_DIR_TRACE_R, NULL);
	if (!parent_dir)
		goto quit;
	if (!proc_create_data("running_qos", 0644, parent_dir, &running_qos_fops, NULL))
		goto remove_proc;
	if (!proc_create_data("load_qos", 0644, parent_dir, &load_qos_fops, NULL))
        goto remove_proc;
	if (!proc_create_data("enable", 0644, parent_dir, &enable_fops, NULL))
        goto remove_proc;
	if (!proc_create_data("trace", 0, parent_dir, &trace_fops, NULL))
		goto remove_proc;

	return 0;

remove_proc:
	remove_proc_subtree(PROC_DIR_TRACE_R, NULL);

quit:
	return -ENOMEM;
}

static void __exit trace_r_exit(void)
{
	unregister_kprobe(&kp_ttwu_do_wakeup);
    unregister_kprobe(&kp_wake_up_new_task);
	
	spin_lock_irqsave(&vring_lock, flags);
	if (vring_buffer)
		vfree(vring_buffer);
	spin_unlock_irqrestore(&vring_lock, flags);

	// may CRASH when you cat /proc/trace_r/trace and rmmod concurrently, but just dont want another lock here 
	if (show_buffer)
		vfree(show_buffer);

	remove_proc_subtree(PROC_DIR_TRACE_R, NULL);
}


module_init(trace_r_init);
module_exit(trace_r_exit);
