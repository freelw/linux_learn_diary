#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define GLOBALMEM_MAGIC 'g'
#define MEM_CLEAR _IO(GLOBALMEM_MAGIC, 0)
#define GLOBALMEM_SIZE 0x1000
#define GLOBALMEM_MAJOR 230
#define DEVICE_NUM 10

static int globalmem_major = GLOBALMEM_MAJOR;

module_param(globalmem_major, int , S_IRUGO);

struct globalmem_dev {
    struct cdev cdev;
    unsigned char mem[GLOBALMEM_SIZE];
};

struct globalmem_dev *globalmem_devp;

static ssize_t globalmem_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos);
static ssize_t globalmem_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos);
static loff_t globalmem_llseek(struct file *filp, loff_t offset, int orig);
static long globalmem_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
static int globalmem_open(struct inode *inode, struct file *filp);

static const struct file_operations globalmem_fops = {
    .owner = THIS_MODULE,
    .read = globalmem_read,
    .write = globalmem_write,
    .llseek = globalmem_llseek,
    .unlocked_ioctl = globalmem_ioctl,
    .open = globalmem_open,
};

static void globalmem_setup_cdev(struct globalmem_dev *dev, int index) {
    int err, devno = MKDEV(globalmem_major, index);
    cdev_init(&dev->cdev, &globalmem_fops);
    dev->cdev.owner = THIS_MODULE;
    err = cdev_add(&dev->cdev, devno, 1);
    if (err) {
        printk(KERN_NOTICE "Error %d adding globalmem%d", err, index);
    }
}

static int __init globalmem_init(void) {
    int ret = 0;
    int i;
    dev_t devno = MKDEV(globalmem_major, 0);
    printk(KERN_INFO "hello wangli.\n");
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
    //cdev_del(&globalmem_devp->cdev);
    int i;
    for (i = 0; i < DEVICE_NUM; ++i) {
        cdev_del(&(globalmem_devp+i)->cdev);
    }
    kfree(globalmem_devp);
    unregister_chrdev_region(MKDEV(globalmem_major, 0), DEVICE_NUM);
    printk(KERN_INFO "bye bye wangli.\n");
}

static ssize_t globalmem_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos) {
    unsigned long p = *ppos;
    unsigned int count = size;
    int ret = 0;
    struct globalmem_dev *dev = filp->private_data;
    if (p >= GLOBALMEM_SIZE) {
        return 0;
    }
    if (count > GLOBALMEM_SIZE - p) {
        count = GLOBALMEM_SIZE - p;
    }
    if (copy_to_user(buf, dev->mem +p, count)) {
        ret = -EFAULT;
    } else {
        *ppos += count;
        ret = count;
        printk(KERN_INFO "read %u byte(s) from %lu\n", count, p);
    }
    return ret;
}

static ssize_t globalmem_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos) {
    loff_t p = *ppos;
    size_t count = size;
    int ret = 0;
    struct globalmem_dev *dev = filp->private_data;
    if (p >= GLOBALMEM_SIZE) {
        return 0;
    }
    if (count > GLOBALMEM_SIZE - p) {
        count = GLOBALMEM_SIZE - p;
    }
    
    if (copy_from_user(dev->mem + p, buf, count)) {
        return -EFAULT;
    } else {
        *ppos += count; 
        ret = count;
        printk(KERN_INFO "written %lu byte(s) from %llu\n", count, p);
    }
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
        if ((unsigned int) offset > GLOBALMEM_SIZE) {
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
        memset(dev->mem, 0, GLOBALMEM_SIZE);
        printk(KERN_INFO "globalmem is set to zero\n");
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

module_init(globalmem_init);
module_exit(globalmem_exit);
MODULE_AUTHOR("wangli <826231693@qq.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("wangli learn cdev demo.");
MODULE_VERSION("V1.2");