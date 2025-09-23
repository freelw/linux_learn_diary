# macbook 无线网卡 BCM4331 驱动安装

[安装包](https://packages.debian.org/search?keywords=firmware-b43-installer)

```
debian如何安装无线网卡驱动


在 Debian 上安装无线网卡驱动
在 Debian 上安装无线网卡驱动通常需要以下步骤：

1. 识别无线网卡型号
首先，你需要知道你的无线网卡的型号。打开终端，输入以下命令：

Bash

lspci -nnk | grep -i net
这个命令会列出你计算机上所有的网络设备，并显示它们的供应商 ID 和设备 ID。例如，你可能会看到类似下面的输出：

03:00.0 Network controller [0280]: Intel Corporation Wireless 8265 / 8275 [8086:24fd] (rev 78)

其中 [8086:24fd] 就是你的无线网卡的供应商 ID 和设备 ID。记下这些信息。

2. 安装非自由固件（Non-free Firmware）
Debian 默认只包含自由软件，而很多无线网卡需要非自由的固件（firmware）才能正常工作。 你需要修改 APT 配置文件，添加 non-free 和 contrib 软件库。

打开 /etc/apt/sources.list 文件：

Bash

sudo nano /etc/apt/sources.list
在每一行末尾添加 contrib non-free：

例如，将：

deb http://deb.debian.org/debian/ bookworm main

修改为：

deb http://deb.debian.org/debian/ bookworm main contrib non-free

对所有行（deb 和 deb-src）都进行同样的操作。

保存并退出文件（在 nano 编辑器中按 Ctrl+O，然后按 Enter 保存，再按 Ctrl+X 退出）。

更新软件包列表：

Bash

sudo apt update
3. 安装相应的驱动包
现在，你可以使用 APT 来安装所需的驱动包了。

通常，你只需要安装 firmware-linux 和 firmware-linux-nonfree 包。 这些包包含了大多数主流无线网卡（如 Intel、Broadcom、Realtek 等）的固件。

Bash

sudo apt install firmware-linux firmware-linux-nonfree
安装完成后，重启电脑，驱动应该就能正常工作了。

4. 针对特殊情况
如果以上步骤不起作用，你可能需要针对你的网卡型号来查找特定的固件包。

Intel 网卡：
通常在 firmware-iwlwifi 包中。

Bash

sudo apt install firmware-iwlwifi
Broadcom 网卡：
根据型号，可能需要 firmware-brcm80211 或 broadcom-sta-dkms。

Bash

sudo apt install firmware-brcm80211
# 如果不行，尝试这个：
sudo apt install broadcom-sta-dkms
Realtek 网卡：
常见的型号驱动在 firmware-realtek 包中。

Bash

sudo apt install firmware-realtek
如果你不知道确切的包名，可以使用 apt-cache search 命令来搜索：

Bash

apt-cache search firmware- [你的网卡品牌]
例如：apt-cache search firmware-intel

疑难解答
如果依旧无法连接： 检查你的无线网卡是否被硬体开关或软件禁用。使用 sudo rfkill list 命令可以查看所有无线设备的状态。如果显示 Soft blocked: yes，你可以使用 sudo rfkill unblock all 来解锁。

内核版本过旧： 有些非常新的无线网卡可能需要最新的 Linux 内核才能被支持。如果你的 Debian 版本较旧，可以考虑升级内核或者使用 Debian 的 backports 仓库。

希望这些步骤能帮助你成功安装无线网卡驱动！







02:00.0 Network controller [0280]: Broadcom Inc. and subsidiaries BCM4331 802.11a/b/g/n [14e4:4331] (rev 02)


Broadcom Inc. and subsidiaries BCM4331 802.11a/b/g/n [14e4:4331] (rev 02) is a network controller. To get it working on Debian, you'll need to install the broadcom-sta-dkms package from the non-free repository.

Here are the steps to do it:

Add the non-free repository to your sources.list file.
Open the file using a text editor with root privileges:

Bash

sudo nano /etc/apt/sources.list
For each line that starts with deb, add contrib non-free at the end. For example, change deb http://deb.debian.org/debian bookworm main to deb http://deb.debian.org/debian bookworm main contrib non-free.

Update your package list.
After saving and closing the file, run this command:

Bash

sudo apt update
This will refresh the list of available packages, including those from the newly added non-free repository.

Install the driver.
Now, you can install the required driver package for your Broadcom card:

Bash

sudo apt install broadcom-sta-dkms
Reboot your system.
After the installation is complete, reboot your computer to ensure the new driver is loaded correctly. Your wireless card should now be recognized and functional.

Bash

sudo reboot






然后怎么连接无线网


在 Debian 上连接无线网络

安装好无线网卡驱动后，连接无线网络通常有两种方法：使用桌面环境的网络管理器或通过命令行。

方法一：使用桌面环境的网络管理器
如果你的 Debian 安装了桌面环境（如 GNOME、KDE、XFCE 等），那连接网络会非常简单：

点击网络图标： 在屏幕的右上角或右下角，你会找到一个网络或无线网络图标。

选择无线网络： 点击该图标后，会弹出一个列表，显示你周围可用的无线网络。

输入密码： 选择你要连接的网络名称（SSID），然后系统会提示你输入密码。输入正确的密码后，点击连接即可。

方法二：使用命令行（适用于无桌面环境）
如果你的 Debian 是服务器版本或没有安装桌面环境，你需要使用 nmcli 或 iw 等命令行工具来连接。

这里以最常用的 nmcli 为例，它功能强大且易于使用。

1. 扫描可用的无线网络：

首先，你需要知道周围有哪些网络。输入以下命令：

Bash

nmcli device wifi list
这个命令会列出所有可用的无线网络，包括名称（SSID）、信号强度等信息。

2. 连接到无线网络：

找到你要连接的网络名称后，使用 nmcli 连接。将 <网络名称> 替换为你要连接的 SSID，将 <密码> 替换为网络密码。

Bash

nmcli device wifi connect <网络名称> password <密码>
示例：

如果你要连接名为 "MyHomeWiFi" 的网络，密码是 "password123"，命令就是：

Bash

nmcli device wifi connect MyHomeWiFi password password123
3. 检查连接状态：

连接成功后，你可以通过以下命令检查你的网络连接状态和 IP 地址：

Bash

nmcli device status
或

Bash

ip a
如果连接失败，请仔细检查你输入的网络名称和密码是否正确。

希望这些步骤能帮助你顺利连接到无线网络！







nmcli 如何安装


要安装 nmcli，你需要安装 network-manager 软件包。nmcli 是 network-manager 的一部分，提供了命令行界面。

在 Debian/Ubuntu 上安装 nmcli
nmcli 包含在 network-manager 包中。你可以使用 apt 包管理器来安装它：

Bash

sudo apt update
sudo apt install network-manager
sudo apt update：这个命令会更新你的本地软件包列表，确保你能获取到最新版本的软件。

sudo apt install network-manager：这个命令会下载并安装 network-manager 及其所有必需的依赖项，其中就包括 nmcli 工具。

安装完成后，network-manager 服务会自动启动。你可以立即开始使用 nmcli 命令来管理你的网络连接。

检查 nmcli 是否已安装
你可以通过以下命令来验证 nmcli 是否安装成功：

Bash

nmcli --version
如果命令返回版本号信息，说明 nmcli 已经成功安装并可以正常使用。如果提示找不到命令，请检查你是否正确执行了上述安装步骤。







 nmcli device wifi list 没有输出



显示思路
当 nmcli device wifi list 没有输出时，通常意味着以下几个可能的问题：

1. 您的无线网卡被禁用
即使驱动安装成功，无线网卡也可能因为软件或硬件开关而被禁用。

首先，检查无线网卡的状态：

Bash

nmcli device status
如果无线设备的状态是 unmanaged 或 unavailable，或者根本没有显示无线设备，那说明它没有被 NetworkManager 管理或识别。

接下来，检查是否有软件或硬件开关阻止了它：

Bash

rfkill list
如果输出显示 Soft blocked: yes 或 Hard blocked: yes，说明无线设备被禁用了。

Soft blocked: 使用以下命令解除软件禁用：

Bash

sudo rfkill unblock wifi
Hard blocked: 这通常是电脑上的一个物理开关或组合键（如 Fn + F2），你需要找到它并打开。

2. NetworkManager 配置问题
有时，NetworkManager 的配置可能会阻止它正确扫描无线网络。

您可以尝试修改配置文件 /etc/NetworkManager/NetworkManager.conf。在配置文件中添加以下内容，以禁用随机 MAC 地址扫描功能，这有时会解决扫描问题：

Bash

[device]
wifi.scan-rand-mac-address=no
保存并关闭文件，然后重启 NetworkManager 服务：

Bash

sudo systemctl restart NetworkManager
重新运行 nmcli device wifi list，看看是否能显示网络列表。

3. 驱动或固件问题
尽管您已经安装了 Broadcom 的驱动，但仍有可能是驱动或固件没有正确加载。

您可以通过 dmesg 命令查看系统日志，寻找与您的无线网卡相关的错误或警告信息：

Bash

dmesg | grep b43
dmesg | grep broadcom
如果日志中显示固件加载失败（Firmware missing/failed to load），您可能需要手动将固件文件放置到正确的位置，或者尝试重新安装驱动。

此外，您也可以使用 iw 命令来独立于 NetworkManager 进行扫描，这可以帮助诊断问题：

Bash

sudo iw dev <设备名称> scan | less
注意： <设备名称> 通常是 wlan0 或 wlp3s0。您可以通过 ip a 或 nmcli device status 来找到它。

如果 iw 命令能够成功扫描到网络，那么问题很可能出在 NetworkManager 的配置上。如果 iw 也无法扫描，则问题很可能出在驱动或固件上。







wangli@mac:~$ nmcli device status

DEVICE    TYPE      STATE         CONNECTION

lo        loopback  连接（外部）  lo

enp1s0f0  ethernet  未托管        --

wangli@mac:~$ ip a

1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000

    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00

    inet 127.0.0.1/8 scope host lo

       valid_lft forever preferred_lft forever

    inet6 ::1/128 scope host noprefixroute

       valid_lft forever preferred_lft forever

2: enp1s0f0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP group default qlen 1000

    link/ether 68:5b:35:81:68:38 brd ff:ff:ff:ff:ff:ff

    altname enx685b35816838

    inet 192.168.2.9/24 brd 192.168.2.255 scope global dynamic noprefixroute enp1s0f0

       valid_lft 3062sec preferred_lft 2612sec

    inet6 fdc5:d6f:d8c6:e80c:6a5b:35ff:fe81:6838/64 scope global dynamic mngtmpaddr proto kernel_ra

       valid_lft 2591913sec preferred_lft 604713sec

    inet6 fdc5:d6f:d8c6:e80c:142d:22f:5cba:7c94/64 scope global dynamic mngtmpaddr noprefixroute

       valid_lft 2591913sec preferred_lft 604713sec

    inet6 fe80::cc67:11cb:6fd9:9bba/64 scope link

       valid_lft forever preferred_lft forever



Based on the output you've provided, it appears your wireless network card is not being detected or managed by NetworkManager. The nmcli device status output only lists lo (loopback) and enp1s0f0 (an ethernet device), but no wireless device (wlan0, wlp..., etc.).

The ip a output confirms this by also only showing the lo and enp1s0f0 interfaces, with enp1s0f0 being an active ethernet connection. The fact that a wireless device is missing from both commands indicates that the driver for your wireless card has not been successfully loaded.

Here are the steps you should take to troubleshoot and resolve this:

1. Re-Verify Driver Installation
First, ensure the correct Broadcom driver package is installed. Your previous prompt showed your card is a BCM4331. The correct driver is broadcom-sta-dkms.

Run this command to check if it's installed:

Bash

dpkg -l | grep broadcom-sta-dkms
If the output shows ii in the first two columns, the package is installed. If not, reinstall it:

Bash

sudo apt update
sudo apt install broadcom-sta-dkms
sudo reboot
2. Check for Driver Blacklisting
Sometimes, other kernel modules for wireless cards can interfere and are blacklisted to prevent conflicts. Make sure b43 and ssb modules are blacklisted, as broadcom-sta-dkms (the wl module) is a proprietary alternative.

Check if they're blacklisted by looking at the content of /etc/modprobe.d/broadcom-sta-dkms.conf.

If the file doesn't exist or doesn't contain the following lines, create or add them:

Bash

sudo nano /etc/modprobe.d/broadcom-sta-dkms.conf
Add these lines to the file:

blacklist b43
blacklist ssb
blacklist bcm43xx
blacklist brcm80211
blacklist brcmfmac
blacklist brcmsmac
blacklist bcma
Save and close the file, then reboot.

3. Load the Driver Manually
After rebooting, if the wireless device is still not showing up, try to manually load the wl module (the main module from the broadcom-sta-dkms package).

Bash

sudo modprobe wl
Then, run nmcli device status again to see if the wireless card now appears. If it does, you can make the change permanent by adding wl to /etc/modules-load.d/broadcom-sta.conf.

4. Check for Firmware Errors in System Logs
If none of the above work, examine the system logs for any errors related to the driver.

Bash

dmesg | grep -i broadcom
dmesg | grep -i bcm4331
This will show you if the kernel encountered an error while trying to load the firmware or the driver module itself. This can help pinpoint the exact cause of the failure.







wangli@mac:~$ nmcli device status

DEVICE    TYPE      STATE         CONNECTION

lo        loopback  连接（外部）  lo

wlp2s0    wifi      已断开        --

enp1s0f0  ethernet  未托管        --



非常好，nmcli device status 的输出现在显示了一个名为 wlp2s0 的 wifi 设备！

这意味着你的无线网卡已经被系统和 NetworkManager 正确识别了。已断开 的状态是正常的，因为它还没有连接到任何网络。

现在，你可以继续下一步来连接到无线网络：

1. 扫描可用的无线网络
首先，使用以下命令来查找你周围的 Wi-Fi 网络：

Bash

nmcli device wifi list
这个命令会列出所有可用的网络，包括它们的 SSID（网络名称）。

2. 连接到你的无线网络
找到你要连接的网络名称（SSID）后，使用下面的命令进行连接。请将 <网络名称> 替换为你的 Wi-Fi 名称，将 <密码> 替换为你的 Wi-Fi 密码。

Bash

nmcli device wifi connect <网络名称> password <密码>
例如：

如果你要连接的网络名为 MyHomeWiFi，密码是 password123，命令就是：

Bash

nmcli device wifi connect MyHomeWiFi password password123
连接成功后，nmcli device status 的状态应该会从 已断开 变为 连接。如果连接失败，请仔细检查你输入的网络名称和密码是否正确。

如果一切顺利，你现在应该可以通过 Wi-Fi 访问互联网了。








wangli@mac:~$ nmcli device wifi connect Xiaomi_0B00_5G password 18514528299

错误：添加/激活新连接失败：Not authorized to control networking.



你遇到了一个权限错误。Not authorized to control networking 意味着你的普通用户 wangli 没有权限来修改网络设置。

这是 Linux 系统为了安全而设置的，只有具有管理员权限的用户才能更改网络配置。

解决方法
要解决这个问题，你需要在命令前加上 sudo，以管理员身份运行。

Bash

sudo nmcli device wifi connect Xiaomi_0B00_5G password 18514528299
运行后，系统会提示你输入当前用户的密码。输入密码后，NetworkManager 就会以管理员权限执行连接操作。

提示： 在大多数基于 Debian 的系统上，sudo 允许你以超级用户（root）的身份执行命令。
```