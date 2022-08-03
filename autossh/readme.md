# 使用autossh 长期保持反向连接

    autossh -M 2223 -fCNR 2222:localhost:22 root@152.136.137.213

    -M 2223 monitor 端口
    
[利用AutoSSH建立SSH隧道，实现内网穿透](https://zhuanlan.zhihu.com/p/112227542)