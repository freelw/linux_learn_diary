# 构建folly demo的方法
1.  申请aws一台 ubuntu的机器，2c 8g
2.  `sudo apt update`
3.  `sudo apt install gcc`
4.  `mkdir -p /home/ubuntu/folly_install`
5.  
```
git clone https://github.com/facebook/folly
cd folly
sudo ./build/fbcode_builder/getdeps.py install-system-deps --recursive
```
6. 比默认配置多指定了--scratch-path
```
python3 ./build/fbcode_builder/getdeps.py --allow-system-packages --scratch-path /home/ubuntu/folly_install build
```
7. 创建目录folly_test, 创建source.cpp和makefile
8. source.cpp

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
9. makefile (注意fmt-SuXDTpdKuhMrTm0IHfrTVn2LlQo1YD7ehH6GUsDviU4这个路径需要手动看一下installed目录下fmt子目录的真实名称，用来连接libfmt.a)

```
CXX=/usr/bin/g++

SCRATCH_PATH=/home/ubuntu/folly_install/installed
INCLUDE = -I$(SCRATCH_PATH)/folly/include
CXXFLAGS += -g -Wall
LDFLAGS += -L$(SCRATCH_PATH)/folly/lib
LDFLAGS += -L$(SCRATCH_PATH)/fmt-SuXDTpdKuhMrTm0IHfrTVn2LlQo1YD7ehH6GUsDviU4/lib
LDFLAGS += -lfolly -lglog -ldl -lpthread -lfmt

all:
	$(CXX) $(CXXFLAGS) $(INCLUDE)  -c source.cpp -o source.o
	$(CXX) source.o $(LDFLAGS)  -o folly_test
clean:
	rm folly_test source.o
```
10. make 后执行 `./folly_test`

```
~/folly_test$ ./folly_test
Hello, world!
Hello, world!
Hello, world!
Hello, world!
Hello, world!
``` 