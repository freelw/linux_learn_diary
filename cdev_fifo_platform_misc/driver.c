#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/sched/signal.h>
#include <linux/poll.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>

#define GLOBALMEM_MAGIC 'g'
#define MEM_CLEAR _IO(GLOBALMEM_MAGIC, 0)
#define GLOBALMEM_SIZE 0x1000

struct globalmem_dev {
    unsigned int current_len;
    unsigned char mem[GLOBALMEM_SIZE];
    struct mutex mutex;
    wait_queue_head_t r_wait;
    wait_queue_head_t w_wait;
    struct miscdevice miscdev;
};

static ssize_t globalmem_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos);
static ssize_t globalmem_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos);
static loff_t globalmem_llseek(struct file *filp, loff_t offset, int orig);
static long globalmem_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
static int globalmem_open(struct inode *inode, struct file *filp);
static unsigned int globalmem_poll(struct file *filp, poll_table *wait);

static const struct file_operations globalmem_fops = {
    .owner = THIS_MODULE,
    .read = globalmem_read,
    .write = globalmem_write,
    .llseek = globalmem_llseek,
    .unlocked_ioctl = globalmem_ioctl,
    .open = globalmem_open,
    .poll = globalmem_poll,
};

static ssize_t globalmem_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos) {
    int ret = 0;
    struct globalmem_dev *dev = container_of(filp->private_data, struct globalmem_dev, miscdev);
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
    struct globalmem_dev *dev = container_of(filp->private_data, struct globalmem_dev, miscdev);
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
    struct globalmem_dev *dev = container_of(filp->private_data, struct globalmem_dev, miscdev);
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
    struct miscdevice *miscdev = filp->private_data;
    struct device *this_device = miscdev->this_device;
    dev_info(this_device, "dev open.\n");
    return 0;
}

static unsigned int globalmem_poll(struct file *filp, poll_table *wait) {
    unsigned int mask = 0;
    struct globalmem_dev *dev = container_of(filp->private_data, struct globalmem_dev, miscdev);

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

static int globalmem_probe(struct platform_device *pdev) {
    struct globalmem_dev *gl;
    int ret;
    gl = devm_kzalloc(&pdev->dev, sizeof(*gl), GFP_KERNEL); // gfp is short for get free page
    if (!gl) {
        return -ENOMEM;
    }
    gl->miscdev.minor = MISC_DYNAMIC_MINOR;
    gl->miscdev.name = "globalmem_dev";
    gl->miscdev.fops = &globalmem_fops;
    mutex_init(&gl->mutex);
    init_waitqueue_head(&gl->r_wait);
    init_waitqueue_head(&gl->w_wait);
    platform_set_drvdata(pdev, gl);
    ret = misc_register(&gl->miscdev);
    if (ret < 0) {
        goto err;
    }
    dev_info(&pdev->dev, "globalmem_probe.\n");
    return 0;
err:
    return ret;
}

static int globalmem_remove(struct platform_device *pdev) {
    struct globalmem_dev *gl = platform_get_drvdata(pdev);
    misc_deregister(&gl->miscdev);
    dev_info(&pdev->dev, "globalmem_remove.\n");
    return 0;
}

static struct platform_driver globalmem_driver = {
    .driver = {
        .name = "globalmem_misc",
        .owner = THIS_MODULE,
    },
    .probe = globalmem_probe,
    .remove = globalmem_remove,
};

module_platform_driver(globalmem_driver);

MODULE_AUTHOR("wangli <826231693@qq.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("wangli learn platform misc dev demo.");
MODULE_VERSION("V1.3");