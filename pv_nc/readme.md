# 使用pv nc 限速传输测试

    pv -L 200 Makefile | nc 127.0.0.1 8081
    nc -lk 8081 > out