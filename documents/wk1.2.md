# 1 Linux。

## 1.1 Linux 系统是宏内核还是微内核？  

Linux 是宏内核（Monolithic Kernel），将大部分操作系统服务（如内存管理、进程调度、文件系统等）都集成在内核空间中，直接调用，性能相对较高。

## 1.2 Linux 中的用户模式和内核模式是什么含义？    

用户模式和内核模式是操作系统的两种不同运行模式

-用户模式（User Mode）：用户模式，也被称为用户空间，是 Linux 系统中应用程序运行的模式。在用户模式下，应用程序与用户交互，接收用户输入，并执行相应的操作。用户模式运行的应用程序受到操作系统的限制相对较少，可以自由地使用系统资源。但是，用户模式下的应用程序不能直接访问系统硬件资源和系统数据，需要通过系统调用接口向操作系统请求服务。  

-内核模式（Kernel Mode）：内核模式，也被称为核心空间，是 Linux 系统中操作系统运行的模式。在内核模式下，操作系统处理系统级别的任务，例如管理硬件资源、调度进程、分配内存等。内核模式运行的操作系统具有更高的权限，可以访问系统中的所有资源。内核模式下的操作系统还可以通过驱动程序接口向硬件设备请求服务。  

## 1.3 用户进程间通信主要有哪几种方式？  

管道（pipe）、命名管道（FIFO）、消息队列（message queue）、共享内存（shared memory）、信号量（semaphore）、信号（signal）、套接字（socket）  

# 2 ARM  

## 2.1 Nand Flash 和 NOR Flash 的区别  

-NAND Flash：容量大、写入速度快、适用于大量数据的存储（如 SD 卡、SSD）、使用复杂的I/O口来串行地存取数据  、每个块的最大擦写次数是一百万次、擦除单元更小，相应的擦除电路更少

-NOR Flash：启动速度快、读速度稍快、带有SRAM接口，有足够的地址引脚来寻址、擦写次数是十万次  

## 2.2 CPU、MCU、SOC 之间的区别  

-CPU（中央处理器）：是一台计算机的运算核心和控制核心，处理数据和指令的核心单元。CPU由运算器、控制器和寄存器及实现它们之间联系的数据、控制及状态的总线构成。  

-MCU（微控制器）：集成 CPU、内存和外设，适用于控制类任务。它本质上仍是一个完整的单片机，有处理器，有各种接口，所有的开发都是基于已经存在的系统架构，应用者要做的就是开发软件程序和加外部设备。而像ARM（Cortex-A系列）直接放代码是运行不了的，因为它本质上只是增强版的CPU，必须添加相应的RAM和ROM。
  
-SOC（系统级芯片）：指的是片上系统，MCU只是芯片级的芯片，而SoC是系统级的芯片，它既MCU那样有内置RAM、ROM同时又像MPU那样强大，不单单是放简单的代码，可以放系统级的代码，也就是说可以运行操作系统（以Linux OS为主）  

## 2.3 什么是交叉编译？  

在一个平台上生成能在另一个平台上运行的程序代码。常用于嵌入式开发中如在 x86 PC 上生成 arm 架构的程序。

## 2.4 什么是冯诺伊曼结构和哈弗结构？
  
-哈佛结构：是一种将程序指令存储和数据存储分开的存储器结构。哈佛结构是一种并行体系结构，它的主要特点是将程序和数据存储在不同的存储空间中，即程序存储器和数据存储器是两个独立的存储器，每个存储器独立编址、独立访问  

-冯·诺依曼结构：是一种将程序指令存储器和数据存储器合并在一起的存储器结构。该结构隐约指导了将储存装置与中央处理器分开的概念，因此依该结构设计出的计算机又称储存程式型电脑

![](https://github.com/beckyhuilin/Linux-project/blob/main/image/%E5%86%AF%E8%AF%BA%E4%BC%8A%E6%9B%BC%26%E5%93%88%E5%BC%97.png)

## 2.4 ARM分哪些异常等级？  

ARM 架构中定义了多个 异常等级（Exception Levels, 简称 EL），用于处理器在不同权限和执行环境下的运行状态，主要在 ARMv8 及以上的 64 位架构（AArch64） 中提出并广泛使用  

EL0：最低权限。普通的用户程序  

EL1：中等权限。操作系统内核，通常被描述为具有特权的程序  

EL2：高权限。管理员，切换操作系统时可以进入该状态。比如需要从Win切到Linux，需要先进入EL2  Hypervisor模式，然后再切到EL1下的Linux  

EL3：最高权限。底层硬件, 包括 Secure Monitor  

![](https://github.com/beckyhuilin/Linux-project/blob/main/image/%E5%BC%82%E5%B8%B8%E7%AD%89%E7%BA%A7.png)






