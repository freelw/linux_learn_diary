# vpn demo
    tun.c 是服务端
    tun_client.c 是客户端
## 在server端
	ip addr add 172.20.10.150/24 dev tun0
	ip link set tun0 up
## 在client端
    ip addr add 172.20.10.151/24 dev tun0
	ip link set tun0 up
## 测试连通性
	ping -I tun0 172.20.10.151