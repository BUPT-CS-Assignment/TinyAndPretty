# **TinyAndPretty 自动化配置手册**
![avatar](https://badgen.net/badge/Language/C++11/orange)
[![avatar](https://badgen.net/github/last-commit/micromatch/micromatch)](https://github.com/NoUITeam/TinyAndPretty)
[![avatar](https://badgen.net/github/license/micromatch/micromatch)](https://github.com/NoUITeam/TinyAndPretty)

## 一.目录文件说明 ##
```
.
├── include             //存放头文件（接口声明）
│   ├── common.h
│   └── module1
│       └── head.h
├── Makefile
├── README.md
├── scripts             //存放脚本及文档
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
* iii . 清除`build`编译文件夹。
```
    $make clean
```
* iv . 添加参数`MODE`可切换为`DEBUG`模式，该模式下工程入口为`test-main.cpp`。
```
    $ make MODE=debug (run)?
```
* v . 添加参数`MODS`可仅使选择的模块编译到工程中。可与`MODE`指令组合使用。
```
    $ make MODS="module1 module2" (run)?
```
* vi. 添加参数`MAINARGS`可添加运行时命令行参数到`main()`函数中。
```
    $ make MAINARGS="sql.db" run
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
