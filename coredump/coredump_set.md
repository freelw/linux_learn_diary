永久设置coredump大小
[root@localhost ~]# echo " * soft core 4194304" >> /etc/security/limits.conf
[root@localhost ~]# echo " * hard core 4194304" >> /etc/security/limits.conf