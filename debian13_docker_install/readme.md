# debian 13 安装docker


```
source ~/.clashrc
sudo apt update
sudo apt upgrade -y
sudo apt install ca-certificates curl gnupg lsb-release -y

sudo install -m 0755 -d /etc/apt/keyrings

curl -fsSL https://download.docker.com/linux/debian/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
sudo chmod a+r /etc/apt/keyrings/docker.gpg

echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/debian \
  $(. /etc/os-release && echo "$VERSION_CODENAME") stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null


sudo apt update
sudo apt install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin -y

sudo groupadd docker

sudo usermod -aG docker $USER

newgrp docker

docker run hello-world
```


配置代理
```
sudo mkdir -p /etc/systemd/system/docker.service.d

sudo vi /etc/systemd/system/docker.service.d/http-proxy.conf
```


配置
```
[Service]
Environment="HTTP_PROXY=http://192.168.1.62:7890"
Environment="HTTPS_PROXY=http://192.168.1.62:7890"
Environment="NO_PROXY=localhost,127.0.0.1,docker-registry.somecorporation.com"
```

重启

```
# 重新加载 systemd 配置
sudo systemctl daemon-reload

# 重启 Docker 服务
sudo systemctl restart docker
```