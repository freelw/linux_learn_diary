#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/sched/signal.h>
#include <linux/poll.h>

#define GLOBALMEM_MAGIC 'g'
#define MEM_CLEAR _IO(GLOBALMEM_MAGIC, 0)
#define GLOBALMEM_SIZE 0x1000
#define GLOBALMEM_MAJOR 230
#define DEVICE_NUM 10

static int globalmem_major = GLOBALMEM_MAJOR;

module_param(globalmem_major, int, S_IRUGO);

struct globalmem_dev {
    struct cdev cdev;
    unsigned int current_len;
    unsigned char mem[GLOBALMEM_SIZE];
    struct mutex mutex;
    wait_queue_head_t r_wait;
    wait_queue_head_t w_wait;
    struct fasync_struct *async_queue;
};

struct globalmem_dev *globalmem_devp;

static ssize_t globalmem_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos);
static ssize_t globalmem_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos);
static loff_t globalmem_llseek(struct file *filp, loff_t offset, int orig);
static long globalmem_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
static int globalmem_open(struct inode *inode, struct file *filp);
static unsigned int globalmem_poll(struct file *filp, poll_table *wait);
static int globalmem_fasync(int fd, struct file *filp, int mode);
static int globalmem_release(struct inode *inode, struct file *filp);

static const struct file_operations globalmem_fops = {
    .owner = THIS_MODULE,
    .read = globalmem_read,
    .write = globalmem_write,
    .llseek = globalmem_llseek,
    .unlocked_ioctl = globalmem_ioctl,
    .open = globalmem_open,
    .poll = globalmem_poll,
    .fasync = globalmem_fasync,
    .release = globalmem_release,
};

static void globalmem_setup_cdev(struct globalmem_dev *dev, int index) {
    int err, devno = MKDEV(globalmem_major, index);
    mutex_init(&dev->mutex);
    init_waitqueue_head(&dev->r_wait);
    init_waitqueue_head(&dev->w_wait);
    cdev_init(&dev->cdev, &globalmem_fops);
    dev->cdev.owner = THIS_MODULE;
    err = cdev_add(&dev->cdev, devno, 1 /*count*/);
    if (err) {
        printk(KERN_NOTICE "Error %d adding globalmem%d", err, index);
    }
}

static int __init globalmem_init(void) {
    int ret = 0;
    int i;
    dev_t devno = MKDEV(globalmem_major, 0);
    printk(KERN_INFO "globalmem_init.\n");
    if (globalmem_major) {
        ret = register_chrdev_region(devno, DEVICE_NUM, "globalmem");
    } else {
        ret = alloc_chrdev_region(&devno, 0, DEVICE_NUM, "globalmem");
        globalmem_major = MAJOR(devno);
    }
    if (ret < 0) {
        return ret;
    }
    globalmem_devp = kzalloc(sizeof(struct globalmem_dev) * DEVICE_NUM, GFP_KERNEL);
    if (!globalmem_devp) {
        ret = -ENOMEM;
        goto fail_malloc;
    }

    for (i = 0; i < DEVICE_NUM; ++i) {
        globalmem_setup_cdev(globalmem_devp + i, i);
    }
    return 0;
fail_malloc:
    unregister_chrdev_region(devno, DEVICE_NUM);
    return ret;
}

static void __exit globalmem_exit(void) {
    int i;
    for (i = 0; i < DEVICE_NUM; ++i) {
        cdev_del(&(globalmem_devp + i)->cdev);
    }
    kfree(globalmem_devp);
    unregister_chrdev_region(MKDEV(globalmem_major, 0), DEVICE_NUM);
    printk(KERN_INFO "globalmem_exit.\n");
}

static ssize_t globalmem_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos) {
    int ret = 0;
    struct globalmem_dev *dev = filp->private_data;
    DECLARE_WAITQUEUE(wait, current);
    mutex_lock(&dev->mutex);
    add_wait_queue(&dev->r_wait, &wait);
    while (dev->current_len == 0) {
        if (filp->f_flags & O_NONBLOCK) {
            ret = -EAGAIN;
            goto out;
        }
        set_current_state(TASK_INTERRUPTIBLE);
        mutex_unlock(&dev->mutex);
        schedule();
        if (signal_pending(current)) {
            ret = -ERESTARTSYS;
            goto out2;
        }
        mutex_lock(&dev->mutex);
    }
    if (count > dev->current_len) {
        count = dev->current_len;
    }
    if (copy_to_user(buf, dev->mem, count)) {
        ret = -EFAULT;
        goto out;
    } else {
        memcpy(dev->mem, dev->mem + count, dev->current_len - count);
        dev->current_len -= count;
        printk(KERN_INFO "read %lu byte(s), current_len: %u\n", count, dev->current_len);
        wake_up_interruptible(&dev->w_wait);
        ret = count;
    }
out:
    mutex_unlock(&dev->mutex);
out2:
    remove_wait_queue(&dev->r_wait, &wait);
    set_current_state(TASK_RUNNING);
    return ret;
}

static ssize_t globalmem_write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos) {
    int ret = 0;
    struct globalmem_dev *dev = filp->private_data;
    DECLARE_WAITQUEUE(wait, current);
    mutex_lock(&dev->mutex);
    add_wait_queue(&dev->w_wait, &wait);
    while (dev->current_len == GLOBALMEM_SIZE) {
        if (filp->f_flags & O_NONBLOCK) {
            ret = -EAGAIN;
            goto out;
        }
        set_current_state(TASK_INTERRUPTIBLE);
        mutex_unlock(&dev->mutex);
        schedule();
        if (signal_pending(current)) {
            ret = -ERESTARTSYS;
            goto out2;
        }
        mutex_lock(&dev->mutex);
    }
    if (count > GLOBALMEM_SIZE - dev->current_len) {
        count = GLOBALMEM_SIZE - dev->current_len;
    }
    if (copy_from_user(dev->mem + dev->current_len, buf, count)) {
        ret = -EFAULT;
        goto out;
    } else {
        dev->current_len += count;
        printk(KERN_INFO "written %lu byte(s), current_len: %u\n", count, dev->current_len);
        wake_up_interruptible(&dev->r_wait);
        if (dev->async_queue) {
            kill_fasync(&dev->async_queue, SIGIO, POLL_IN);
            printk(KERN_DEBUG "%s kill SIGIO\n", __func__);
        }
        ret = count;
    }
out:
    mutex_unlock(&dev->mutex);
out2:
    remove_wait_queue(&dev->w_wait, &wait);
    set_current_state(TASK_RUNNING);
    return ret;
}

static loff_t globalmem_llseek(struct file *filp, loff_t offset, int orig) {
    loff_t ret = 0;
    switch (orig) {
    case 0:
        if (offset < 0) {
            ret = -EINVAL;
            break;
        }
        if ((unsigned int)offset > GLOBALMEM_SIZE) {
            ret = -EINVAL;
            break;
        }
        filp->f_pos = (unsigned int)offset;
        ret = filp->f_pos;
        break;
    case 1:
        if ((filp->f_pos + offset) > GLOBALMEM_SIZE) {
            ret = -EINVAL;
            break;
        }
        if ((filp->f_pos + offset) < 0) {
            ret = -EINVAL;
            break;
        }
        filp->f_pos += offset;
        ret = filp->f_pos;
        break;
    default:
        ret = -EINVAL;
        break;
    }
    return ret;
}

static long globalmem_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    struct globalmem_dev *dev = filp->private_data;
    switch (cmd) {
    case MEM_CLEAR:
        mutex_lock(&dev->mutex);
        memset(dev->mem, 0, GLOBALMEM_SIZE);
        printk(KERN_INFO "globalmem is set to zero\n");
        mutex_unlock(&dev->mutex);
        break;
    default:
        return -EINVAL;
    }
    return 0;
}

static int globalmem_open(struct inode *inode, struct file *filp) {
    filp->private_data = container_of(inode->i_cdev, struct globalmem_dev, cdev);
    return 0;
}

static unsigned int globalmem_poll(struct file *filp, poll_table *wait) {
    unsigned int mask = 0;
    struct globalmem_dev *dev = filp->private_data;

    mutex_lock(&dev->mutex);
    poll_wait(filp, &dev->r_wait, wait);
    poll_wait(filp, &dev->w_wait, wait);

    if (dev->current_len != 0) {
        mask |= POLLIN | POLLRDNORM;
    }
    if (dev->current_len != GLOBALMEM_SIZE) {
        mask |= POLLOUT | POLLWRNORM;
    }
    mutex_unlock(&dev->mutex);
    return mask;
}

static int globalmem_fasync(int fd, struct file *filp, int mode) {
    struct globalmem_dev *dev = filp->private_data;
    return fasync_helper(fd, filp, mode, &dev->async_queue);
}

static int globalmem_release(struct inode *inode, struct file *filp) {
    globalmem_fasync(-1, filp, 0);
    return 0;
}

module_init(globalmem_init);
module_exit(globalmem_exit);
MODULE_AUTHOR("wangli <826231693@qq.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("wangli learn cdev demo.");
MODULE_VERSION("V1.2");

/*
    sudo mknod /dev/globalmem0 c 230 0
    sudo mknod /dev/globalmem1 c 230 1
    sudo mknod /dev/globalmem9 c 230 9
    sudo mknod /dev/globalmem10 c 230 10
*/