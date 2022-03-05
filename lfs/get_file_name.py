input = 'wget-list-11.1'
prefix = 'https://lfs-11-1-1259536604.cos.ap-beijing.myqcloud.com/'
#prefix = 'http://mirrors.ustc.edu.cn/lfs/lfs-packages/11.1/'
for line in open(input):
    arr = line.strip().split('/')
    print (prefix + arr[-1])
