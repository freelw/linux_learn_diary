# 在腾讯云cvm安装ubuntu 20.04 并配置远程桌面

* 申请腾讯云cvm

* [安装xfce 和 vncserver](https://www.digitalocean.com/community/tutorials/how-to-install-and-configure-vnc-on-ubuntu-20-04)

* 设置分辨率

        #!/bin/bash
        sudo vncserver -kill :1
        sudo vncserver -geometry 1920x1080



