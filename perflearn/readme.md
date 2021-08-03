# perf相关

## perf静态编译

    #当前路径
    $ pwd
    /home/ubuntu/vscode_linux_dbg_env/build/linux-5.3.7
    #编译命令
    make LDFLAGS=-static -C tools/ perf
    #这样编译出来的perf文件是静态链接的
    $ file tools/perf/perf
    tools/perf/perf: ELF 64-bit LSB executable, x86-64, version 1 (GNU/Linux), statically linked, BuildID[sha1]=3e45cef36bce8ac3b5348bc70e143147c06a92d5, for GNU/Linux 3.2.0, with debug_info, not stripped
    $ ldd tools/perf/perf
        not a dynamic executable


## linux mount dbgfs

    mount -t debugfs none /sys/kernel/debug