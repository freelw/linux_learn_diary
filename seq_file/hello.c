#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static int __init wangli_seq_file_init(void) {
    return 0;
}

static void __exit wangli_seq_file_exit(void) {

}

module_init(wangli_seq_file_init);
module_exit(wangli_seq_file_exit);
MODULE_AUTHOR("wangli <826231693@qq.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("wangli learn seq_file demo.");
MODULE_VERSION("V1.0");