# **TinyAndPretty 自动化配置手册**
![avatar](https://badgen.net/badge/Language/C++17/orange)
![stars](https://badgen.net/badge/Dev%20Env./Linux/green)
![license](https://badgen.net/badge/License/Apache-2.0/blue)

## 一.目录文件说明 ##
```
.
├── include             //存放头文件（接口声明）
│   ├── common.h        //程序共用头
│   ├── interfaces.h    //模块接口,包含所有模块对外使用的函数定义
│   ├── Network         //网络服务模块
│   │   ├── HttpProtocal.h
│   │   ├── HttpServer.h
│   │   ├── Network.h
│   │   ├── ServerBase.h
│   │   └── URLParser.h
│   ├── libs
│   │   ├── NEdb.h      //数据库接口头文件
│   │   └── SimpleJson.hpp
│   ├── router.conf
│   └── test
│       └── define.h
├── lib
│   └── libnedb.so      //数据库动态库文件
├── Kconfig             //参数配置
├── LICENSE
├── Makefile
├── README.md
├── scripts             //存放脚本及文档
│   ├── build.mk
│   ├── colors.mk
│   └── config.mk
├── src                 //项目源代码
│   ├── main.cpp
│   ├── Network
│   │   ├── HttpBase.cpp
│   │   ├── HttpMessage.cpp
│   │   ├── HttpServer.cpp
│   │   ├── HttpUtils.cpp
│   │   ├── ServerBase.cpp
│   │   └── URLParser.cpp
│   └── test
│       └── test.cpp
└── utils               //存放工具组
```

## 二.自动化指令说明 ##
在项目文件夹根目录下使用如下指令，可快速使用相应功能。
* i . 编译全部工程. 
```
    $ make 
```
* ii . 编译并运行。
```
    $ make run
```
* iii . 清除`build`编译文件夹。`disrclean`可清除`menuconfig`生成的配置文件。
```
    $ make (dist)?clean
```
* iv . 启动菜单配置，调整程序运行参数
```
    $ make menuconfig
```
* v . 添加参数`MODS`可仅使选择的模块编译到工程中。
```
    $ make MODS="module1 module2" (run)?
```
* vi. 添加参数`MAINARGS`可添加运行时命令行参数到`main()`函数中。
```
    $ make MAINARGS="sql.db" run
```
* vii. 多线程编译，加快编译速度。如`-j2`双线程编译
```
    $ make -j(\d*)
```

## 三.添加模块说明 ##
`src/`目录下的每一个文件夹为一个独立的模块，脚本将自动检测该***目录下*** 的子文件夹并进行编译链接。
* i . 请在`include`文件夹下创建`$(MODULE_NAME)`子目录，用于存放该模块的头文件（即接口）
* ii . 在该文件夹下定义`interface.h`文件，标识为该模块的调用接口。如若对该模块进行调用，请在代码中进行如下引用
```c++
    #include <$(MODULE_NAME)/interface.h>
```
* iii . 创建`src/$(MODULE_NAME)`子目录，并再次实现模块功能。
* vi . 在`scripts`文件夹下存放对应的文档（可选）。
