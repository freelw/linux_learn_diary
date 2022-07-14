## 实验环境

    # TencentOS Server 3.1 (TK4)
    [root@VM-0-13-centos ~]# uname -a
    Linux VM-0-13-centos 5.4.119-19-0009.1 #1 SMP Sun Jan 23 23:20:30 CST 2022 x86_64 x86_64 x86_64 GNU/Linux

## 拉tencentos

    git clone https://gitee.com/Tencent/tencentos-kernel.git
    cd tencentos-kernel
    git checkout x86-5.4.119-19-0009.1

## 安装bear

    # 这里用2.4.4版本，否则依赖太多
    git clone  https://github.com/rizsotto/Bear.git
    cd Bear
    git checkout 2.4.4
    cd ..
    mkdir build
    cd build
    cmake ../Bear
    make all
    make check
    sudo make install

## 创建swap

    # 创建6G的swap
    dd if=/dev/zero of=/var/swap bs=1024 count=6144000
    mkswap -f /var/swap
    swapon /var/swap
    swapon -s
    free -h

## 编译kernel

    cp -v /boot/config-5.4.119-19-0009.1 .config
    make menuconfig
    bear make -j4
    
    