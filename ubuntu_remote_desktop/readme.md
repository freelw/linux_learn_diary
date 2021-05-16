# 在腾讯云cvm安装ubuntu 20.04 并配置远程桌面 & 配置linux环境

* 申请腾讯云cvm

* [安装xfce 和 vncserver](https://www.digitalocean.com/community/tutorials/how-to-install-and-configure-vnc-on-ubuntu-20-04)

* 设置分辨率

        #!/bin/bash
        sudo vncserver -kill :1
        sudo vncserver -geometry 1920x1080

* [下载vscode linux版本](https://az764295.vo.msecnd.net/stable/2b9aebd5354a3629c3aba0a5f5df49f43d6689f8/code_1.54.3-1615806378_amd64.deb)

* 安装vscode

        sudo dpkg -i code_1.54.3-1615806378_amd64.deb

* [使用vs调试linux内核](https://howardlau.me/programming/debugging-linux-kernel-with-vscode-qemu.html)

* [add-auto-load-safe-path的含义](https://sourceware.org/gdb/onlinedocs/gdb/Auto_002dloading-safe-path.html)

* [setupCommands的含义](https://stackoverflow.com/questions/60278274/docs-for-setupcommands-in-vscode-cppdbg-launch-json)