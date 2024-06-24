# dpdk 学习


## 查看当前网卡绑定的驱动状态

    wangli@wangli-desktop:~/dpdk_learn/dpdk-22.03$ ./usertools/dpdk-devbind.py --status

    Network devices using kernel driver
    ===================================
    0000:02:00.0 'RTL8111/8168/8411 PCI Express Gigabit Ethernet Controller 8161' if=enp2s0 drv=r8169 unused=vfio-pci *Active*
    0000:07:00.0 'RTL8111/8168/8411 PCI Express Gigabit Ethernet Controller 8168' if=enp7s0 drv=r8169 unused=vfio-pci *Active*

    No 'Baseband' devices detected
    ==============================

    No 'Crypto' devices detected
    ============================

    No 'DMA' devices detected
    =========================

    No 'Eventdev' devices detected
    ==============================

    No 'Mempool' devices detected
    =============================

    No 'Compress' devices detected
    ==============================

    No 'Misc (rawdev)' devices detected
    ===================================

    No 'Regex' devices detected
    ===========================

## 给网卡绑定vfio-pci 驱动

    # 先关闭网卡
    sudo ifconfig enp2s0 down
    sudo ./usertools/dpdk-devbind.py --bind=vfio-pci enp2s0

## 重新观察驱动绑定情况

    wangli@wangli-desktop:~/dpdk_learn/dpdk-22.03$ ./usertools/dpdk-devbind.py --status

    Network devices using DPDK-compatible driver
    ============================================
    0000:02:00.0 'RTL8111/8168/8411 PCI Express Gigabit Ethernet Controller 8161' drv=vfio-pci unused=r8169

    Network devices using kernel driver
    ===================================
    0000:07:00.0 'RTL8111/8168/8411 PCI Express Gigabit Ethernet Controller 8168' if=enp7s0 drv=r8169 unused=vfio-pci *Active*

## 确认是否使用vfio

    lspci -nnv | grep "Kernel driver in use"

    02:00.0 Ethernet controller [0200]: Realtek Semiconductor Co., Ltd. RTL8111/8168/8411 PCI Express Gigabit Ethernet Controller [10ec:8161] (rev 15)
    Subsystem: Realtek Semiconductor Co., Ltd. TP-Link TG-3468 v4.0 Gigabit PCI Express Network Adapter [10ec:8168]
    Flags: fast devsel, IRQ 36, IOMMU group 16
    I/O ports at e000 [size=256]
    Memory at fb304000 (64-bit, non-prefetchable) [size=4K]
    Memory at fb300000 (64-bit, non-prefetchable) [size=16K]
    Capabilities: <access denied>
    Kernel driver in use: vfio-pci
    Kernel modules: r8169

## 配置hugepage

    sudo su
    echo 1024 > /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages

## 测试helloworld

    cd /home/wangli/dpdk_learn/dpdk-22.03/examples/helloworld/build
    wangli@wangli-desktop:~/dpdk_learn/dpdk-22.03/examples/helloworld/build$ sudo ./helloworld -l 0-3 -n 4
    EAL: Detected CPU lcores: 20
    EAL: Detected NUMA nodes: 1
    EAL: Detected shared linkage of DPDK
    EAL: Multi-process socket /var/run/dpdk/rte/mp_socket
    EAL: Selected IOVA mode 'VA'
    EAL: VFIO support initialized
    TELEMETRY: No legacy callbacks, legacy socket not created
    hello from core 1
    hello from core 2
    hello from core 3
    hello from core 0

## 安装 libpcap-dev 使用纯软件的pmd

[why](https://stackoverflow.com/questions/71657277/nic-is-unavailable-in-dpdk-application)

[hwoto](https://doc.dpdk.org/guides/nics/pcap_ring.html)

    The main reason why DPDK ports are not identified in your environment is because the NIC in use is not having a supported vendor Poll Mode Driver. Please refer to list of supported NIC from various vendor Realtek is not among them.

    sudo apt-get install libpcap-dev

[dpdk rte_eth_dev_count_avail()返回0 问题解决记录](http://t.csdn.cn/ybNeV)