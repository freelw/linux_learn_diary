# 网络配置方案：让B机器通过A机器访问外网

## 网络拓扑分析

```
Internet (192.168.1.0/24)
    |
    wlp2s0 (192.168.1.25)
    |
    A机器 (192.168.2.1)
    |
    enp1s0f0 (192.168.2.0/24)
    |
    B机器 (192.168.2.2)
```

- **A机器**: 双网卡配置
  - `wlp2s0`: 连接外网 (192.168.1.25/24)
  - `enp1s0f0`: 连接B机器 (192.168.2.1/24)
- **B机器**: 单网卡配置
  - `eno1-ovs`: 连接A机器 (192.168.2.2/24)

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

推荐使用**方案一（iptables NAT）**，因为：
- 配置简单直接
- 性能较好
- 兼容性强
- 易于调试和维护

配置完成后，B机器就可以通过A机器访问外网了。
