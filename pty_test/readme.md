# 使用伪终端消除标准io库在使用管道时的4k buffer问题

1. 在buffer_demo目录直接执行`./buffer_demo`会每隔一秒输出一行"123"
2. 在buffer_demo目录执行`./buffer_demo | tee a.txt` 会暂时不输出
3. 在forkpty_demo目录执行`./forkpty_demo ../buffer_demo/buffer_demo | tee a.txt` 会每隔一秒输出一行"123"
4. 在forkpty_demo目录执行`tail -f a.txt` 会每隔一秒输出一行"123"