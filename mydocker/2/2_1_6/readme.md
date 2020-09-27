# 注意

这里参考了 https://github.com/xianlubird/mydocker/issues/3

我这里执行了

    grubby --args="user_namespace.enable=1" --update-kernel="$(grubby --default-kernel)"
    reboot
    echo 640 > /proc/sys/user/max_user_namespaces