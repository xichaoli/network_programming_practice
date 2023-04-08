# network_programming_practice
极客时间《网络编程实战》课程代码

# 编译和运行
## 使用 ninja
在Linux环境下，先创建build目录，再进到目录build下，执行cmake
```bash
mkdir build && cd build && cmake .. && make
```

## 使用 Makefile
```bash
mkdir build && cd build && cmake -G Ninja .. && make
```

生成的可执行程序在对应的课程目录（lesson-xx）下面。

## IDE
如果你有CLion或者其他可视化的IDE，可以直接在IDE中执行"Build Project"来完成整个项目的编译。
