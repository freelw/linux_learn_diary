# 注意

在容器中执行

    mount -t proc proc /proc/

之前，首先要在宿主机上执行

    mount --make-rprivate /proc

否则宿主机的/proc文件系统将受到影响