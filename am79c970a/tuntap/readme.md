ip addr add 172.20.10.150/24 dev tun0
ip link set tun0 up
ping -I tun0 172.20.10.81