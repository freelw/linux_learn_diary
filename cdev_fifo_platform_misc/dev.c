#include <linux/module.h>
#include <linux/platform_device.h>

static struct platform_device *globalmem_pdev;

static int __init globalmemdev_init(void) {
    int ret;
    globalmem_pdev = platform_device_alloc("globalmem_misc", -1);
    if (!globalmem_pdev) {
        return -ENOMEM;
    }
    ret = platform_device_add(globalmem_pdev);
    if (ret) {
        platform_device_put(globalmem_pdev);
        return ret;
    }
    return 0;
}

module_init(globalmemdev_init);

static void __exit globalmemdev_exit(void) {
    platform_device_unregister(globalmem_pdev);
}

module_exit(globalmemdev_exit);

MODULE_AUTHOR("wangli <826231693@qq.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("wangli learn platform misc dev demo.");
MODULE_VERSION("V1.3");
