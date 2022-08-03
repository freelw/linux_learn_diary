# linux from scratch 流水账

## 11.1 stable book

[下载解压阅读 11.1](https://lfs-11-1-1259536604.cos.ap-beijing.myqcloud.com/LFS-BOOK-11.1.tar.xz)

## 检查host软件环境

[检查host软件环境](https://www.linuxfromscratch.org/lfs/view/stable/chapter02/hostreqs.html)
[version-check.sh](https://gitee.com/freelw/linux_learn_diary/tree/master/lfs/version-check.sh)

## wget-list
An easy way to download all of the packages and patches is by using wget-list as an input to wget. For example:

    wget https://lfs-11-1-1259536604.cos.ap-beijing.myqcloud.com/wget-list-11.1-tencent-cos
    wget https://lfs-11-1-1259536604.cos.ap-beijing.myqcloud.com/md5sums --directory-prefix=$LFS/sources
    wget --input-file=wget-list-11.1-tencent-cos --continue --directory-prefix=$LFS/sources
    pushd $LFS/sources
    md5sum -c md5sums
    popd

[wget-list-9.0-tencent-cos](https://lfs-11-1-1259536604.cos.ap-beijing.myqcloud.com/wget-list-11.1-tencent-cos)

[md5sums](https://lfs-11-1-1259536604.cos.ap-beijing.myqcloud.com/md5sums)