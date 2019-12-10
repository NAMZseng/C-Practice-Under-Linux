# C-Practice-Under-Linux
Linux下的C语言开发学习

## 相关笔记
### gcc编译四个过程
- 预处理
  -  头文件包含、宏替换、条件编译、删除注释
  -  gcc -E
  -  该步骤生成的文件后缀 .i
- 编译
  - 语法检查、将预处理的文件编译成汇编文件
  -  gcc -S
  -  该步骤生成的文件后缀 .s
- 汇编
    -  将汇编文件处理成二进制文件
    - gcc -c
    -  该步骤生成的文件后缀 .o
- 链接
    - 整合二进制文件、相关库函数、启动代码生成可执行文件,main函数是由启动代码调用的，程序是从启动代码开始运行的。
    - 静态链接，指调用ld/collect2链接程序，将所有的.o中的机器指令整合到一起，然后保存到可执行文件中。
    - 动态链接，在编译的时候只留下调用接口（函数第一条指令的地址），当程序真正运行的时候，才去链接执行，动态链接这件事不是在编译时发生的，是在程序动态运行时发生。
    -  比如程序中调用printf函数，这个函数基本都是动态库提供的，程序编译后代码里面是没有printf函数代码的，只有printf这个接口，当程序运行起来后，再去动态链接printf所在的动态库，那么程序就能调用printf函数。
    - Linux默认的动态库搜索路径/usr/lib
    - gcc
    -  该步骤生成的文件后缀 .out

**参考自**
- [https://www.tianmaying.com/tutorial/GCC](https://www.tianmaying.com/tutorial/GCC
- [https://www.cnblogs.com/kelamoyujuzhen/p/9411007.html](https://www.cnblogs.com/kelamoyujuzhen/p/9411007.html)


​	
