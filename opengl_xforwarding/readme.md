# 在服务器运行opengl程序，在客户端渲染

## 准备

[glfw](https://www.glfw.org/)

## 系统

        Linux VM-0-13-ubuntu 5.4.0-90-generic #101-Ubuntu SMP Fri Oct 15 20:00:55 UTC 2021 x86_64 x86_64 x86_64 GNU/Linux

## 所需的依赖

        #!/bin/bash
        apt install cmake libx11-dev libxrandr-dev libxinerama-dev libsdl2-dev

## demo

[demo](https://www.glfw.org/documentation.html)

## mac端端准备

1. XQuartz 2.8.1（xorg-server 1.20.11）

2. 打开glx支持

        defaults write org.xquartz.X11 enable_iglx -bool true

        read一下看看有没有打开
        ➜  ~ defaults read org.xquartz.X11
        {
            SUHasLaunchedBefore = 1;
            SULastCheckTime = "2022-06-11 14:29:10 +0000";
            "app_to_run" = "/opt/X11/bin/xterm";
            "cache_fonts" = 1;
            "done_xinit_check" = 1;
            "enable_iglx" = 1; #这就表示打开了
            "login_shell" = "/bin/sh";
            "no_auth" = 0;
            "nolisten_tcp" = 1;
            "startx_script" = "/opt/X11/bin/startx -- /opt/X11/bin/Xquartz";
        }

3. 在mac上打开XQuartz

        执行
        ssh -X xxx@ip

        这样就带开了一个glx协议传输通道，在当前终端执行./demo，就会在mac端打开窗口
![效果](https://gitee.com/freelw/linux_learn_diary/raw/master/img/glxfwd.png)