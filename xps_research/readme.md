# xps 机制研究

[sys下添加文件](https://blog.csdn.net/larry233/article/details/88094004)
[kobject](https://blog.csdn.net/jasonchen_gbd/article/details/78013643)


## 研究目标

    /sys/class/net/eth0/queues/tx-%d/xps_cpus 的行为

## 线索

1. netdev_queue_ktype net-sysfs.c 里面的sysfs_ops定义了属性的读写行为

    netdev_queue_default_groups 是通过宏 ATTRIBUTE_GROUPS(netdev_queue_default) 扩展而来

2. netdev_queue_default_attrs 中的 xps_cpus_attribute.attr 是关注点
    通过 __ATTR_RW(xps_cpus)展开
    要关注  xps_cpus_store xps_cpus_show 这两个是写入和读取的实现
    从哪里调用的呢
    从这里 netdev_queue_sysfs_ops
    netdev_queue_attr_show

    struct netdev_queue *queue = to_netdev_queue(kobj);

    先从kobj转出netdev_queue *
    在调用attr的show

    主要是根据queue的index 读写dev->xps_cpus_map

    在get_xps_queue这个函数中使用
    在netdev_pick_tx使用
    在netdev_core_pick_tx使用
    在__dev_queue_xmit使用

    txq = netdev_core_pick_tx(dev, skb, sb_dev);
    rc = __dev_xmit_skb(skb, q, dev, txq);


    从__dev_xmit_skb送出