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

static void *ct_seq_start(struct seq_file *s, loff_t *pos) {
    struct my_private_data *p = s->private;
    p->spos = *pos + p->origin;
    return &p->spos;
}

static void *ct_seq_next(struct seq_file *s, void *v, loff_t *pos) {
    loff_t *spos = (loff_t *)v;
    *pos = ++(*spos);
    return spos;
}

static void ct_seq_stop(struct seq_file *s, void *v) {
}

static int ct_seq_show(struct seq_file *s, void *v) {
    loff_t *spos = (loff_t *)v;
    seq_printf(s, "%Ld\n", *spos);
    return 0;
}

static struct seq_operations ct_seq_ops = {
    .start = ct_seq_start,
    .next = ct_seq_next,
    .stop = ct_seq_stop,
    .show = ct_seq_show,
};

static int ct_open(struct inode *inode, struct file *file) {
    int ret = 0;
    ret = seq_open(file, &ct_seq_ops);
    if (0 == ret) {
        struct seq_file *seq = file->private_data;
        // 5.4的内核已经屏蔽了 struct proc_dir_entry
        // 原来拿proc_dir_entry中private数据的方法是
        // 1. 通过PDE宏拿到 proc_dir_entry结构体指针
        // 2. 通过指针拿到private数据
        // 现在的方法是直接通过PDE_DATA宏拿到数据
        seq->private = PDE_DATA(inode);
    }
    return ret;
};

static struct file_operations ct_file_ops = {
    .owner = THIS_MODULE,
    .open = ct_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = seq_release};

static int ct_init(void) {
    struct proc_dir_entry *entry;
    memset(&d0, 0, sizeof(struct my_private_data));
    memset(&d1, 0, sizeof(struct my_private_data));
    d0.origin = 100;
    d1.origin = 200;
    entry = proc_create_data("test_d0", 0, NULL, &ct_file_ops, &d0);
    if (entry == NULL) {
        return -ENOMEM;
    }
    entry = proc_create_data("test_d1", 0, NULL, &ct_file_ops, &d1);
    if (entry == NULL) {
        return -ENOMEM;
    }
    return 0;
}

static void ct_exit(void) {
    remove_proc_entry("test_d0", NULL);
    remove_proc_entry("test_d1", NULL);
}

module_init(ct_init);
module_exit(ct_exit);

/*
测试方法

dd if=/proc/test_d0 of=out1 count=1
dd if=/proc/test_d1 of=out2 count=1
*/
