input = 'wget-list'
prefix = 'http://mirrors.ustc.edu.cn/lfs/lfs-packages/9.0/'
for line in open(input):
    arr = line.strip().split('/')
    print (prefix + arr[-1])
