# **TinyAndPretty 自动化配置手册**
![avatar](https://badgen.net/badge/Language/C++11/orange)
![stars](https://badgen.net/badge/Dev%20Env./Linux/green)
![license](https://badgen.net/badge/License/Apache-2.0/blue)

## 一.目录文件说明 ##
```
.
├── bin                 //存放输出可执行文件
├── build               //编译过程临时文件夹
├── auto.sh             //自动化编译链接脚本
├── include             //存放头文件（接口声明）
│   ├── common.h
│   └── module1
│       └── head.h
├── CMakeLists.txt      //CMake配置文件
├── README.md
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
* i . 编译配置全部工程. 
    
    ```
        $ ./auto.sh 
    ```
    编译脚本将自动清除 **`build`** 目录下内容, 若保留请自行更改 **`auto.sh`** 内容


* ii . 编译并运行。
    ```
        $ cd bin
        $ ./${PROJECT_NAME}
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
