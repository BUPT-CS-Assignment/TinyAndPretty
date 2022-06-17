![intro](../utils/pic1.png)
![avatar](https://badgen.net/badge/Language/C++17/orange)
![stars](https://badgen.net/badge/Dev%20Env./Linux/green)
![license](https://badgen.net/badge/License/Apache-2.0/blue)
# **TinyAndPretty 小而美的网络服务框架**
TinyAndPretty(TAP)是使用`Modern C++(11/14/17)`开发的网络服务框架，它秉持简单、易用、可扩展易开发的原则，帮助你快速构建~~可靠~~的``C++``后台服务。

## 分层的微核架构 ##
TAP主体采用[微核架构](http://www.ruanyifeng.com/blog/2016/09/software-architecture.html)`(microkernel architecture)`，又称为"插件架构"`(plug-in architecture)`。TAP Center Manager（TAP中枢管理员）会负责系统整体的初始化、子管理员（plugins）的协调与调度，主要功能和业务逻辑都通过插件实现。
* 良好的功能延伸性`(extensibility)`，需要什么功能，开发一个插件即可.
* 功能之间是隔离的，插件可以独立的加载和卸载，使得它比较容易部署.
* 可定制性高，适应不同的开发需要
* 可以渐进式地开发，逐步增加功能

## UNIX友好的配置方式 ##
移植了`Kconfig`系统，在纯命令行环境下也有对人类友好的配置体验，可视化界面充分展示系统的所有参数细节。

## 纯粹事件驱动模型 ##
依托`Linux`系统高效的`Epoll`功能和`C++17`强大的泛型支持，本框架中的`Event Pool`支持了**事件管道**级别的抽象，将数据挂载和文件描述符监听结合起来，同时配合`timer fd`完成了纳米级定时事件的处理。

这一模型中，`TAP Center Manager`充当了总线功能，通过对插件的预设**模块魔数**进行判断完成调度。`Timer Manager`也被纳入这一框架中，通过`TimeLine`这一数据结构完成虚拟时间和现实时间的转换，省去了线程轮询的过程极大提高了CPU使用率。

## 易用的HTTP/1.1 + COMET ##
本框架支持了`HTTP` GET、POST、**multipart/form-data**、**Static Resource**等常见请求形式，并在此基础上实现了`COMET`长连接服务的支持，使得~~在懂得都懂的情况下的~~服务器获得了主动向客户端推送广播的能力。

同时基于`Modern C++`的现代语言功能，框架推荐支持`std::filesystem` 和 `std::chrono`等在内的现代库，充分使用`std::tuple`、`std::initialize_list`和结构化绑定联动的方式，简化了大量的操作流程，配置`HTTP`服务变得人性化，这一点在实际的线上项目开发中得到了检验。

同时`HTTP`响应服务支持了字符串类型的`HttpResponse`、经由`sendfile()`优化过的`FileResponse`、自主开发的`JsonResponse`。充分考虑多种常见实际业务情况。

## Reactor模型的单进程多线程服务 ##
本项目在启动初期就把超好用的并发服务的作为重要目标之一。为此移植了简洁好用的`Thread Pool`，同时使用了`RAII`风格的`std::unique_lock`/`std::shared_lock`操作`std::shared_mutex`等锁。整体高效使用。


## 未来提升的方向 ##
1. ~~多半咕咕，没有未来~~
2. 分布式支持
3. `Linux`下自有网络协议栈
4. `WebSocket` 和 `HTTP/2` 的支持
5. ~~BUG FREE~~



## 自动化指令说明 ##
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

## 添加模块说明 ##
`src/`目录下的每一个文件夹为一个独立的模块，脚本将自动检测该***目录下*** 的子文件夹并进行编译链接。
* i . 请在`include`文件夹下创建`$(MODULE_NAME)`子目录，用于存放该模块的头文件（即接口）
* ii . 在该文件夹下定义`interface.h`文件，标识为该模块的调用接口。如若对该模块进行调用，请在代码中进行如下引用
```c++
    #include <$(MODULE_NAME)/interface.h>
```
* iii . 在`include/router.conf`中配置路由信息
* iii . 创建`src/$(MODULE_NAME)`子目录，并再次实现模块功能。
* vi . 在`scripts`文件夹下存放对应的文档（可选）。
