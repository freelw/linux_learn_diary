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