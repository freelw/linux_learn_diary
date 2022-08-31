#!/bin/bash
if [ `id -u` -eq 0 ];then
	echo "root用户!"
else
	echo "非root用户!"
	exit -1
fi

read -r -p '如果你确定继续mount vkfs 并且chroot，请输入[yes]' input

case ${input} in
    yes)
        echo -e '\033[31m[WARNING] 准备开始mount，你还有10s可以终止！\033[0m'
        sleep 10
        ;;
    *)
        echo '终止测试.'
        exit 1
        ;;
esac


mkdir -pv $LFS/{dev,proc,sys,run}
mknod -m 600 $LFS/dev/console c 5 1
mknod -m 666 $LFS/dev/null c 1 3
mount -v --bind /dev $LFS/dev
mount -v --bind /dev/pts $LFS/dev/pts
mount -vt proc proc $LFS/proc
mount -vt sysfs sysfs $LFS/sys
mount -vt tmpfs tmpfs $LFS/run
if [ -h $LFS/dev/shm ]; then
  mkdir -pv $LFS/$(readlink $LFS/dev/shm)
fi

echo "var LFS is $LFS"
echo -e '\033[31m[WARNING] 准备开始chroot，你还有10s可以终止！\033[0m'
sleep 10

chroot "$LFS" /usr/bin/env -i   \
    HOME=/root                  \
    TERM="$TERM"                \
    PS1='(lfs chroot) \u:\w\$ ' \
    PATH=/usr/bin:/usr/sbin     \
    /bin/bash --login