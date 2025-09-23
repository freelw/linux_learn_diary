我有两台机器网络配置如下

A机器

```
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
    inet 192.168.2.1/24 brd 192.168.2.255 scope global enp1s0f0
       valid_lft forever preferred_lft forever
3: wlp2s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP group default qlen 1000
    link/ether 70:56:81:9b:9b:cf brd ff:ff:ff:ff:ff:ff
    altname wlx7056819b9bcf
    inet 192.168.1.25/24 brd 192.168.1.255 scope global dynamic noprefixroute wlp2s0
       valid_lft 64815sec preferred_lft 64815sec
    inet6 2409:8a00:6c85:4520:dac7:f33e:85e0:1dcf/64 scope global dynamic noprefixroute
       valid_lft 189670sec preferred_lft 103270sec
    inet6 fe80::7b40:720f:d304:ce87/64 scope link noprefixroute
       valid_lft forever preferred_lft forever
```

A机器的interface配置如下

```
wangli@mac:/etc/network$ cat interfaces
# This file describes the network interfaces available on your system
# and how to activate them. For more information, see interfaces(5).

source /etc/network/interfaces.d/*

# The loopback network interface
auto lo
iface lo inet loopback

# The primary network interface
allow-hotplug enp1s0f0
auto enp1s0f0
iface enp1s0f0 inet static
        address 192.168.2.1
        netmask 255.255.255.0
```

B机器

```
wangli@fn-big:~$ ip a
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host noprefixroute
       valid_lft forever preferred_lft forever
2: eno1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast master ovs-system state UP group default qlen 1000
    link/ether 04:d9:f5:d4:b7:be brd ff:ff:ff:ff:ff:ff
    altname enp0s31f6
3: ovs-system: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN group default qlen 1000
    link/ether ce:c6:9c:55:6b:51 brd ff:ff:ff:ff:ff:ff
4: eno1-ovs: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UNKNOWN group default qlen 1000
    link/ether 04:d9:f5:d4:b7:be brd ff:ff:ff:ff:ff:ff
    inet 192.168.2.2/24 brd 192.168.2.255 scope global noprefixroute eno1-ovs
       valid_lft forever preferred_lft forever
    inet6 fe80::3985:20f4:9f45:7212/64 scope link noprefixroute
       valid_lft forever preferred_lft forever
5: docker0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN group default
    link/ether f6:57:47:19:d8:62 brd ff:ff:ff:ff:ff:ff
    inet 172.17.0.1/16 brd 172.17.255.255 scope global docker0
       valid_lft forever preferred_lft forever
```

B机器的eno1-ovs通过一根网线连上了A机器的enp1s0f0

A机器可以通过wlp2s0 访问外网

问：现在想在A机器上通过iptables或者其他方式，让B也能够访问外网，怎么做

------

追加一个需求

将访问A机器从wlp2s0进入的tcp流量 端口是5666的，都转发到B机器的5666端口