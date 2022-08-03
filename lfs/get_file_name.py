input = 'wget-list'
prefix = 'https://lfs-1252366230.cos.ap-beijing.myqcloud.com/'
for line in open(input):
    arr = line.strip().split('/')
    print (prefix + arr[-1])
