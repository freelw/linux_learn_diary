# 构建folly测试的方法
1.  申请aws一台 ubuntu的机器，2c 8g
2.  `sudo apt update`
3.  `sudo apt install gcc`
4.  `mkdir -p /home/ubuntu/folly_install`
5.  
    
        git clone https://github.com/facebook/folly
        cd folly
        sudo ./build/fbcode_builder/getdeps.py install-system-deps --recursive
6.
        python3 ./build/fbcode_builder/getdeps.py --allow-system-packages --scratch-path /home/ubuntu/folly_install build
7. 查看 scratch-path

        ./build/fbcode_builder/getdeps.py show-inst-dir

8. 自动化测试

        python3 ./build/fbcode_builder/getdeps.py 
        --allow-system-packages test

9. 创建目录folly_test, 创建source.cpp和makefile
10. source.cpp

    ```
    #include <folly/experimental/FunctionScheduler.h>
    #include <iostream>
    #include <chrono>
    #include <thread>

    int main() {
        // 创建一个调度器对象，周期为1秒
        folly::FunctionScheduler scheduler;
        scheduler.addFunction([] {
            std::cout << "Hello, world!" << std::endl;
        }, std::chrono::seconds(1));

        // 启动调度器
        scheduler.start();

        // 让程序睡眠一段时间，观察调度器是否正常工作
        std::this_thread::sleep_for(std::chrono::seconds(5));

        // 停止调度器
        scheduler.shutdown();

        return 0;
    }
    ```
11. makefile

    ```
    CXX=/usr/bin/g++

    SCRATCH_PATH=/home/ubuntu/folly_install
    INCLUDE = -I$(SCRATCH_PATH)/folly/include
    CXXFLAGS += -g -Wall
    LDFLAGS += -L$(SCRATCH_PATH)/folly/lib
    LDFLAGS += -L/tmp/fbcode_builder_getdeps-ZhomeZubuntuZfollyZbuildZfbcode_builder/installed/fmt-Pl3nxizTjzUQPALHXS-s59qcZkscq5e0_5X2m6kz5ag/lib
    LDFLAGS += -lfolly -lglog -ldl -lpthread -lfmt

    all:
        $(CXX) $(CXXFLAGS) $(INCLUDE)  -c source.cpp -o source.o
        $(CXX) source.o $(LDFLAGS)  -o folly_test
    clean:
        rm folly_test source.o
    ```
12. make 后执行 `./folly_test`

    ```
    ~/folly_test$ ./folly_test
    Hello, world!
    Hello, world!
    Hello, world!
    Hello, world!
    Hello, world!
    ```
    
