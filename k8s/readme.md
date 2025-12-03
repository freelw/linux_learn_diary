# debian 13 k8s proxy

```
sudo mkdir -p /etc/systemd/system/containerd.service.d
sudo vi /etc/systemd/system/containerd.service.d/http-proxy.conf
```

```
[Service]
Environment="HTTP_PROXY=http://192.168.1.62:7890"
Environment="HTTPS_PROXY=http://192.168.1.62:7890"
# 请确保 NO_PROXY 包含了所有不需要走代理的地址（如本地回环、主机IP、Kubernetes内部网段等）
Environment="NO_PROXY=localhost,127.0.0.1,0.0.0.0/0,10.96.0.0/12,192.168.1.0/24"
```

## 自定义crd相关

