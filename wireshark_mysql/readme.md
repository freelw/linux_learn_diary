# 一个wireshark 过滤mysql参数的过滤器

        frame.time >= "Jun  1, 2022 10:20:33.553972000" and frame.time <= "Jun  1, 2022 10:20:34" and mysql.exec.field.longlong==560008

        sql执行在网络中大概分为三步
        1. prepare， 这一步能看到带问好占位的sql语句
        2. execute， 这一步填充占位的参数
        3. close statement

        mysql.exec.field.longlong 用这样的关键字可以过滤参数