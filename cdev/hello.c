#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define GLOBALMEM_SIZE 0x1000
#define MEM_CLEAR 0x1
#define GLOBALMEM_MAJOR 230

static int globalmem_major = GLOBALMEM_MAJOR;

module_param(globalmem_major, int , S_IRUGO);

struct globalmem_dev {
    struct cdev cdev;
    unsigned char mem[GLOBALMEM_SIZE];
};

struct globalmem_dev *globalmem_devp;

static const struct file_operations globalmem_fops = {

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
    int ret;
    dev_t devno = MKDEV(globalmem_major, 0);
    if (globalmem_major) {
        ret = register_chrdev_region(devno, 1, "globalmem");
    } else {
        ret = alloc_chrdev_region(&devno, 0, 1, "globalmem");
        globalmem_major = MAJOR(devno);
    }
    if (ret < 0) {
        return ret;
    }
    globalmem_devp = kzalloc(sizeof(struct globalmem_dev), GFP_KERNEL);
    if (!globalmem_devp) {
        ret = -ENOMEM;
        goto fail_malloc;
    }
    globalmem_setup_cdev(globalmem_devp, 0);
    return 0;
fail_malloc:
    unregister_chrdev_region(devno, 1);
    return ret;
}

static void __exit globalmem_exit(void) {
    
}

module_init(globalmem_init);
module_exit(globalmem_exit);
MODULE_AUTHOR("wangli <826231693@qq.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("wangli learn cdev demo.");
MODULE_VERSION("V1.2");


