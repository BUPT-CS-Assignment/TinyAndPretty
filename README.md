# **TinyAndPretty 自动化配置手册**

### 1.目录文件说明 ###
```
.
├── include             //存放头文件（接口声明）
│   ├── common.h
│   └── module1
│       └── head.h
├── Makefile
├── README.md
├── scripts             //存放Makefile脚本
│   └── colors.mk
├── src
│   ├── main.cpp        //程序入口
│   ├── module1
│   │   ├── m1in.cpp
│   │   └── te1.cpp
│   ├── module2
│   │   ├── case.cpp
│   │   └── print.cpp
│   ├── test-main.cpp   //测试用程序入口
│   └── t.h
└── utils               //存放工具组
```

### 2.自动化指令说明 ###
* i.编译全部工程. 若没有MODE一项则默认入口为`main.cpp`,否则为`test-main.cpp`
```
    make (MODE=debug)?
```
* ii.编译并运行. 若没有MODE一项则默认入口为`main.cpp`,否则为`test-main.cpp`
```
    make (MODE=debug)? run
```
* iii.清除`build`编译文件夹
```
    make clean
```

### 3.添加模块说明 ###
* `src/`目录下的每一个文件夹为一个独立的模块，脚本将自动检测并进行编译链接。
* 请在`include`文件夹下创建相应子目录，用于存放该模块的头文件（即接口）