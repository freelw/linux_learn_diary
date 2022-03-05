# platform miscdev 学习

1. driver.ko 注册了自己的名称叫globalmem_misc
2. dev.ko 新建了一个设备名称叫globalmem_misc
3. 两边通过名字呼应上了之后触发了driver的probe函数
4. probe创建了名叫globalmem_dev 的miscdev，会自动在/dev下创建/dev/globalmem_dev
5. 每次传入的filp->private_data的类型是miscdevice, 是globalmem_dev结构体中的miscdev对象，所以可以用container_of计算出结构体的指针
6. rmmod dev 或者 driver 后，设备文件消失

## 测试

    echo 123 >/dev/globalmem_dev
    cat /dev/globalmem_dev 