# 网络配置方案：让B机器通过A机器访问外网

## 网络拓扑分析

```
Internet (192.168.1.0/24)
    |
    wlp2s0 (192.168.1.25)    C机器 (192.168.1.197)
    |                        |
    A机器 (192.168.2.1)      |
    |                        |
    enp1s0f0 (192.168.2.0/24)|
    |                        |
    B机器 (192.168.2.2)      |
```

- **A机器**: 双网卡配置
  - `wlp2s0`: 连接外网 (192.168.1.25/24)
  - `enp1s0f0`: 连接B机器 (192.168.2.1/24)
- **B机器**: 单网卡配置
  - `eno1-ovs`: 连接A机器 (192.168.2.2/24)
- **C机器**: MacBook M1 Air
  - 无线网卡: 连接外网 (192.168.1.197/24)

## 解决方案：配置NAT转发

### 方案一：使用iptables配置NAT（推荐）

#### 1. 在A机器上启用IP转发

```bash
# 临时启用（重启后失效）
echo 1 > /proc/sys/net/ipv4/ip_forward

# 永久启用
echo 'net.ipv4.ip_forward = 1' >> /etc/sysctl.conf
sysctl -p
```

#### 2. 查看当前iptables规则

```bash
# 查看当前filter表规则
iptables -L -n -v

# 查看当前nat表规则
iptables -t nat -L -n -v

# 查看当前mangle表规则
iptables -t mangle -L -n -v

# 查看当前raw表规则
iptables -t raw -L -n -v

# 查看规则编号（便于删除特定规则）
iptables -L --line-numbers
iptables -t nat -L --line-numbers
```

#### 3. 配置iptables NAT规则

```bash
# 清空现有规则（谨慎操作）
iptables -F
iptables -t nat -F

# 设置默认策略
iptables -P INPUT ACCEPT
iptables -P FORWARD ACCEPT
iptables -P OUTPUT ACCEPT

# 配置NAT转发：将来自192.168.2.0/24网段的流量通过wlp2s0转发出去
iptables -t nat -A POSTROUTING -s 192.168.2.0/24 -o wlp2s0 -j MASQUERADE

# 允许转发来自192.168.2.0/24网段的流量
iptables -A FORWARD -s 192.168.2.0/24 -j ACCEPT
iptables -A FORWARD -d 192.168.2.0/24 -j ACCEPT

# mac转发5666飞牛控制台到B机器
iptables -t nat -A PREROUTING -i wlp2s0 -p tcp --dport 5666 -j DNAT --to-destination 192.168.2.2:5666
iptables -A FORWARD -i wlp2s0 -p tcp --dport 5666 -d 192.168.2.2 -j ACCEPT
iptables -A FORWARD -i enp1s0f0 -p tcp --sport 5666 -s 192.168.2.2 -j ACCEPT

sudo netfilter-persistent save
```

#### 4. 保存iptables规则

```bash
# Ubuntu/Debian系统
iptables-save > /etc/iptables/rules.v4

# 或者使用iptables-persistent
apt-get install iptables-persistent
netfilter-persistent save
```

### 方案二：使用ufw配置（Ubuntu推荐）

#### 1. 启用IP转发

```bash
# 编辑ufw配置文件
sudo vim /etc/ufw/before.rules

# 在文件末尾添加：
*nat
:POSTROUTING ACCEPT [0:0]
-A POSTROUTING -s 192.168.2.0/24 -o wlp2s0 -j MASQUERADE
COMMIT
```

#### 2. 启用转发

```bash
# 编辑sysctl配置
echo 'net.ipv4.ip_forward=1' >> /etc/ufw/sysctl.conf

# 重启ufw
ufw reload
```

### 方案三：使用systemd-networkd配置

#### 1. 创建网络配置文件

```bash
# 创建enp1s0f0的配置文件
sudo vim /etc/systemd/network/20-enp1s0f0.network

[Match]
Name=enp1s0f0

[Network]
Address=192.168.2.1/24
IPForward=yes
```

#### 2. 重启网络服务

```bash
sudo systemctl restart systemd-networkd
```

## B机器配置

### 1. 设置默认网关

```bash
# 临时设置
sudo ip route add default via 192.168.2.1

# 永久设置（编辑网络配置）
sudo vim /etc/network/interfaces

# 添加或修改：
auto eno1-ovs
iface eno1-ovs inet static
    address 192.168.2.2
    netmask 255.255.255.0
    gateway 192.168.2.1
    dns-nameservers 8.8.8.8 8.8.4.4
```

### 2. 配置DNS

```bash
# 编辑resolv.conf
sudo vim /etc/resolv.conf

# 添加DNS服务器
nameserver 8.8.8.8
nameserver 8.8.4.4
```

## C机器（MacBook M1 Air）配置

### 方案一：通过A机器路由访问B机器（推荐）

#### 1. 在A机器上添加路由规则

```bash
# 添加路由：将来自C机器(192.168.1.197)访问B机器(192.168.2.2)的流量路由到内网
iptables -t nat -A PREROUTING -s 192.168.1.197 -d 192.168.2.2 -j DNAT --to-destination 192.168.2.2

# 允许C机器访问B机器
iptables -A FORWARD -s 192.168.1.197 -d 192.168.2.2 -j ACCEPT
iptables -A FORWARD -s 192.168.2.2 -d 192.168.1.197 -j ACCEPT
```

#### 2. 在MacBook上添加路由

```bash
# 临时添加路由（重启后失效）
sudo route add -net 192.168.2.0/24 192.168.1.25

# 永久添加路由（需要创建启动脚本）
sudo vim /Library/LaunchDaemons/com.user.route.plist
```

创建启动脚本内容：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>Label</key>
    <string>com.user.route</string>
    <key>ProgramArguments</key>
    <array>
        <string>/sbin/route</string>
        <string>add</string>
        <string>-net</string>
        <string>192.168.2.0/24</string>
        <string>192.168.1.25</string>
    </array>
    <key>RunAtLoad</key>
    <true/>
</dict>
</plist>
```

加载启动脚本：
```bash
sudo launchctl load /Library/LaunchDaemons/com.user.route.plist
```

### 方案二：使用SSH隧道（简单但需要SSH服务）

#### 1. 在A机器上启用SSH服务

```bash
# 安装SSH服务（如果未安装）
sudo apt-get install openssh-server

# 启动SSH服务
sudo systemctl start ssh
sudo systemctl enable ssh

# 检查SSH服务状态
sudo systemctl status ssh
```

#### 2. 在MacBook上创建SSH隧道

```bash
# 创建SSH隧道：将本地端口2222转发到B机器的22端口
ssh -L 2222:192.168.2.2:22 wangli@192.168.1.25

# 或者转发其他端口（如80端口）
ssh -L 8080:192.168.2.2:80 wangli@192.168.1.25
```

#### 3. 通过隧道访问B机器

```bash
# 通过SSH访问B机器
ssh -p 2222 username@localhost

# 通过浏览器访问B机器的Web服务
open http://localhost:8080
```

### 方案三：使用VPN（最安全但配置复杂）

#### 1. 在A机器上安装OpenVPN

```bash
# 安装OpenVPN
sudo apt-get install openvpn easy-rsa

# 生成证书
make-cadir ~/openvpn-ca
cd ~/openvpn-ca
source vars
./clean-all
./build-ca
./build-key-server server
./build-key client1
./build-dh
```

#### 2. 配置OpenVPN服务器

```bash
# 复制配置文件
sudo cp ~/openvpn-ca/keys/server.crt ~/openvpn-ca/keys/server.key ~/openvpn-ca/keys/ca.crt ~/openvpn-ca/keys/dh2048.pem /etc/openvpn/

# 创建服务器配置
sudo vim /etc/openvpn/server.conf
```

服务器配置内容：
```
port 1194
proto udp
dev tun
ca ca.crt
cert server.crt
key server.key
dh dh2048.pem
server 10.8.0.0 255.255.255.0
ifconfig-pool-persist ipp.txt
push "route 192.168.2.0 255.255.255.0"
push "dhcp-option DNS 8.8.8.8"
keepalive 10 120
cipher AES-256-CBC
user nobody
group nogroup
persist-key
persist-tun
status openvpn-status.log
verb 3
```

#### 3. 在MacBook上安装OpenVPN客户端

```bash
# 使用Homebrew安装
brew install openvpn

# 或者下载官方客户端
# https://openvpn.net/client-connect-vpn-for-mac-os/
```

## 验证配置

### 1. 在A机器上检查

```bash
# 检查IP转发是否启用
cat /proc/sys/net/ipv4/ip_forward

# 检查iptables规则
iptables -t nat -L -n -v

# 检查路由表
ip route show
```

### 2. 在B机器上测试

```bash
# 测试连通性
ping 192.168.2.1  # 测试到A机器的连通性
ping 8.8.8.8      # 测试外网连通性
ping google.com   # 测试DNS解析

# 检查路由
ip route show
```

### 3. 在C机器（MacBook）上测试

```bash
# 测试到A机器的连通性
ping 192.168.1.25

# 测试到B机器的连通性
ping 192.168.2.2

# 测试SSH连接（如果使用SSH隧道方案）
ssh wangli@192.168.1.25

# 检查路由表
netstat -rn

# 检查网络接口
ifconfig
```

## 故障排除

### 常见问题

1. **B机器无法ping通外网**
   - 检查A机器的IP转发是否启用
   - 检查iptables规则是否正确
   - 检查B机器的默认网关设置

2. **DNS解析失败**
   - 检查B机器的DNS配置
   - 在A机器上测试DNS转发

3. **连接不稳定**
   - 检查网线连接
   - 检查防火墙设置
   - 查看系统日志：`journalctl -f`

### 调试命令

```bash
# 在A机器上监控转发流量
tcpdump -i enp1s0f0
tcpdump -i wlp2s0

# 检查连接跟踪
conntrack -L

# 查看iptables日志
dmesg | grep iptables
```

## 安全考虑

1. **防火墙配置**
   - 只允许必要的端口转发
   - 限制内网访问范围

2. **访问控制**
   - 考虑使用MAC地址绑定
   - 设置访问时间限制

3. **监控**
   - 定期检查连接日志
   - 监控带宽使用情况

## 总结

### 对于B机器访问外网：
推荐使用**方案一（iptables NAT）**，因为：
- 配置简单直接
- 性能较好
- 兼容性强
- 易于调试和维护

### 对于C机器（MacBook）访问B机器：
推荐使用**方案一（路由转发）**，因为：
- 配置相对简单
- 性能最佳
- 支持所有网络协议
- 无需额外服务

### 替代方案：
- **SSH隧道**：适合临时访问或只需要特定端口
- **VPN**：适合需要加密通信的场景

配置完成后：
- B机器可以通过A机器访问外网
- C机器（MacBook）可以通过A机器访问B机器
