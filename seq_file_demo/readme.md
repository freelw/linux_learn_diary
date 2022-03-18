这是一个lwn上关于seq_file的demo
[源代码在这里](https://lwn.net/Articles/22359/)

修改了ct_init函数使之能在5.4内核上编译通过

    static int ct_init(void)
    {
        struct proc_dir_entry *entry;
        entry = proc_create("sequence", 0, NULL, &ct_file_ops);
        if(entry == NULL)
            return -ENOMEM;
        return 0;
    }

测试方法

    dd if=/proc/sequence of=out1 count=1
    dd if=/proc/sequence skip=1 of=out2 count=1

可以发现out1的最后一行和out2的第一行拼接起来才是完整的，这就是seq_file的好处
