#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>

MODULE_AUTHOR("wangli");

struct my_private_data {
    loff_t origin;
    loff_t spos;
};

static struct my_private_data d0, d1;

static int ct_seq_show(struct seq_file *s, void *v) {
    struct my_private_data *p = s->private;
    seq_printf(s, "%Ld\n", p->origin);
    return 0;
}

static int ct_open(struct inode *inode, struct file *file) {
    return single_open(file, ct_seq_show, PDE_DATA(inode));
};

static struct file_operations ct_file_ops = {
    .owner = THIS_MODULE,
    .open = ct_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

static int ct_init(void) {
    struct proc_dir_entry *entry;
    memset(&d0, 0, sizeof(struct my_private_data));
    memset(&d1, 0, sizeof(struct my_private_data));
    d0.origin = 100;
    d1.origin = 200;
    entry = proc_create_data("test_single_d0", 0, NULL, &ct_file_ops, &d0);
    if (entry == NULL) {
        return -ENOMEM;
    }
    entry = proc_create_data("test_single_d1", 0, NULL, &ct_file_ops, &d1);
    if (entry == NULL) {
        return -ENOMEM;
    }
    return 0;
}

static void ct_exit(void) {
    remove_proc_entry("test_single_d0", NULL);
    remove_proc_entry("test_single_d1", NULL);
}

module_init(ct_init);
module_exit(ct_exit);

/*
测试方法

dd if=/proc/test_single_d0 of=out1 count=1
dd if=/proc/test_single_d1 of=out2 count=1
*/
