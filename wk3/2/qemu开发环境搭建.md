# Qemu
## 1 qemu安装
### 1.1 git clone
    创建qemu文件夹目录，在此目录下
    git clone https://github.com/qemu/qemu.git

### 1.2 安装必要的库
     sudo apt-get install libglib2.0-dev

     sudo apt-get install libpixman-1-dev

     sudo apt-get install libfdt-dev

     sudo apt-get install ninja-build

### 1.3 运行配置指令
    1)进入新克隆的qemu目录，新建build文件夹，输入  
    ../configure  
   （提示ERROR: python venv creation failed），则更新python如下：

    sudo apt update
    sudo apt install python3-venv 

    2)再次输入../configure
   （提示found no usable tomli, please install it），则输入如下：
  
    sudo apt install python3-pip
    pip3 install tomli  

    3)再次../configure
   （提示安装ERROR: Program 'flex' not found or not executable），则输入如下：  

    sudo apt install flex 

    4)再次../configure
   （提示ERROR: Program 'bison' not found or not executable），则输入如下：  
 
    sudo apt install bison  

    5)再次../configure
    成功

### 1.4 编译qemu 
    make -j16
### 1.5 安装qemu 
    sudo make install
### 1.6 查看版本
    qemu-system-arm -version
   显示如下
![](https://github.com/beckyhuilin/Linux-project/blob/0673c1bd1997e9cd28b554c7dfd250853bf4537c/image/qemu_version.png)   

## 2 kernel  

### 2.1 安装kernel  

    wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.12.tar.xz  

    tar -xvf linux-6.12.tar.xz  

    cd linux-6.12

### 2.2 编译

    1）
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- vexpress_defconfig
   （显示gcc版本太老错误，所以升级gcc）则输入以下：  

    sudo add-apt-repository ppa:ubuntu-toolchain-r/test
    sudo apt update
    sudo apt install gcc-12 g++-12

    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 100
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 100

    gcc --version  

    2）再输入  
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- vexpress_defconfig
   （还是报错，显示gcc版本老），则更新arm-linux-gnueabihf-gcc如下：

    输入arm-linux-gnueabihf-gcc --version
    sudo apt update
    sudo apt install gcc-12-arm-linux-gnueabihf

    然后执行
    sudo update-alternatives --install /usr/bin/arm-linux-gnueabihf-gcc arm-linux-gnueabihf-gcc /usr/bin/arm-linux-gnueabihf-gcc-12 100

    sudo update-alternatives --config arm-linux-gnueabihf-gcc  

    3）重开shell，再输入
      
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- vexpress_defconfig  

    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage dtbs -j$(nproc)

## 3 制作根文件  

### 3.1 下载busybox

    wget http://busybox.net/downloads/busybox-1.36.0.tar.bz2
    tar -xjf busybox-1.36.0.tar.bz2
    mv busybox-1.36.0 busybox
    cd busybox

    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- defconfig     （生成默认配置）
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-               （编译源码）
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- install       （安装到 _install/）

### 3.2 形成根目录结构

- 创建rootfs目录  
  
      mkdir -p rootfs/{dev,etc/init.d,lib}  
- 把busybox中的文件复制到rootfs根目录下  
   
      sudo cp -r busybox/_install/* rootfs/   
 
- 把交叉编译工具链中的库文件复制到rootfs根目录的lib文件夹下  
  
      sudo cp -p /usr/arm-linux-gnueabihf/lib/* rootfs/lib/  
 
- 创建4个tty端终设备  
  
      sudo mknod rootfs/dev/tty1 c 4 1

      sudo mknod rootfs/dev/tty2 c 4 2

      sudo mknod rootfs/dev/tty3 c 4 3

      sudo mknod rootfs/dev/tty4 c 4 4

### 3.3 制作根文件系统镜像  

   1）输入  

    dd if=/dev/zero of=a9rootfs.ext3 bs=1M count=32  

    mkfs.ext3 a9rootfs.ext3  

    sudo mkdir tmpfs  

    sudo mount -t ext3 a9rootfs.ext3 tmpfs/ -o loop

    sudo cp -r rootfs/*  tmpfs/
   （提示cp: 写入 ... 时出错：设备上没有空间）

   2）回到镜像文件所在目录  

    cd ~/project/wk3/2/qemu
    sudo umount tmpfs
    rm a9rootfs.ext3

    创建更大的 ext3 文件镜像
    dd if=/dev/zero of=a9rootfs.ext3 bs=1M count=512
    mkfs.ext3 a9rootfs.ext3

    重新挂载并复制 rootfs
    sudo mkdir tmpfs
    sudo mount -t ext3 a9rootfs.ext3 tmpfs/ -o loop
    sudo cp -r rootfs/* tmpfs/
    sudo umount tmpfs

### 3.4 启动qemu 
 
1）输入  

qemu-system-arm \
  -M vexpress-a9 \
  -m 512M \
  -kernel ./linux-6.12/arch/arm/boot/zImage \
  -dtb ./linux-6.12/arch/arm/boot/dts/vexpress-v2p-ca9.dtb \
  -nographic \
  -append "root=/dev/mmcblk0 console=ttyAMA0" \
  -sd ./a9rootfs.ext3

(显示qemu-system-arm: No machine specified, and there is no default  
Use -machine help to list supported machines)

2）改成  

    qemu-system-arm -M vexpress-a9 -m 512M -kernel kernel/linux-6.12/arch/arm/boot/zImage -dtb kernel/linux-6.12/arch/arm/boot/dts/vexpress-v2p-ca9.dtb -nographic -append "root=/dev/mmcblk0 console=ttyAMA0" -drive file=./a9rootfs.ext3,format=raw,if=sd

（显示Couldn't open dtb file kernel/linux-6.12/arch/arm/boot/dts/vexpress-v2p-ca9.dtb）

3）检查  

    ls kernel/linux-6.12/arch/arm/boot/dts/vexpress-v2p-ca9.dtb
（显示没有）则进入到6linux-6.12文件夹

    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- dtbs
(显示没有文件) 

4）查找文件位置

    find kernel/linux-6.12/arch/arm/boot/dts/vexpress-v2p-ca9.dtb
(显示存在，但是在 arch/arm/boot/dts/arm/vexpress-v2p-ca9.dtb）

5）修改后重新输入  

    qemu-system-arm -M vexpress-a9 -m 512M -kernel kernel/linux-6.12/arch/arm/boot/zImage -dtb kernel/linux-6.12/arch/arm/boot/dts/arm/vexpress-v2p-ca9.dtb -nographic -append "root=/dev/mmcblk0 console=ttyAMA0" -drive file=./a9rootfs.ext3,format=raw,if=sd
(显示file=./a9rootfs.ext3找不到)

6）查找位置  
  
    find ~/project -name "a9rootfs.ext3"
（得到/home/becky/project/wk3/2/qemu/a9rootfs.ext320124199804063245）

7）修改后在qemu目录下重新输入  

    qemu-system-arm -M vexpress-a9 -m 512M -kernel kernel/linux-6.12/arch/arm/boot/zImage -dtb kernel/linux-6.12/arch/arm/boot/dts/arm/vexpress-v2p-ca9.dtb -nographic -append "root=/dev/mmcblk0 console=ttyAMA0" -drive file=/home/becky/project/wk3/2/qemu/a9rootfs.ext3,format=raw,if=sd

（显示的是：QEMU 已经成功启动了 Linux 内核，也已经挂载了根文件系统，但 BusyBox 或 init 脚本缺失导致系统“卡在了 init 之后”）

8）修改   
 
    mkdir -p etc/init.d
    nano etc/init.d/rcS  

    输入
    #!/bin/sh
    mount -t proc none /proc
    mount -t sysfs none /sys
    echo -e "\nWelcome to minimal BusyBox system on QEMU\n"
    /bin/sh

    chmod +x etc/init.d/rcS

    重新打包镜像
    sudo cp -r busybox/_install/* rootfs/
    dd if=/dev/zero of=a9rootfs.ext3 bs=1M count=512
    mkfs.ext3 a9rootfs.ext3
    sudo mkdir -p tmpfs
    sudo mount -t ext3 a9rootfs.ext3 tmpfs/ -o loop
    sudo cp -r rootfs/* tmpfs/
    sudo umount tmpfs


    然后再输入
    qemu-system-arm -M vexpress-a9 -m 512M -kernel kernel/linux-6.12/arch/arm/boot/zImage -dtb kernel/linux-6.12/arch/arm/boot/dts/arm/vexpress-v2p-ca9.dtb -nographic -append "root=/dev/mmcblk0 console=ttyAMA0" -drive file=/home/becky/project/wk3/2/qemu/a9rootfs.ext3,format=raw,if=sd

（运行大部分成功，但出现
mount: mounting none on /proc failed: No such file or directory
can't access tty; job control turned off）

9）重新回到_install文件夹

   mkdir -p proc sys
   在qemu文件下，重复镜像操作后再次启动


## 4 验证 uname 显示的内核版本  
    
    uname -a
显示如下
![](https://github.com/beckyhuilin/Linux-project/blob/0673c1bd1997e9cd28b554c7dfd250853bf4537c/image/uname.png)  

## 5 共享目录  

### 5.1 设置
1）在ubuntu中输入  

    mkdir -p ~/shared

    启动qemu
    qemu-system-arm -M vexpress-a9 -m 512M -kernel kernel/linux-6.12/arch/arm/boot/zImage -dtb kernel/linux-6.12/arch/arm/boot/dts/arm/vexpress-v2p-ca9.dtb -nographic -append "root=/dev/mmcblk0 console=ttyAMA0" -drive file=/home/becky/project/wk3/2/qemu/a9rootfs.ext3,format=raw,if=sd -fsdev local,id=fsdev0,path=/home/becky/shared,security_model=none -device virtio-9p-device,fsdev=fsdev0,mount_tag=hostshare

    输入mkdir /mnt/host
   （显示mkdir: can't create directory '/mnt/host': No such file or directory）  

2）在ubuntu中建立文件夹  

    mkdir -p busybox/_install/mnt  

    在qemu文件下，重新镜像打包后执行

    qemu-system-arm -M vexpress-a9 -m 512M -kernel kernel/linux-6.12/arch/arm/boot/zImage -dtb kernel/linux-6.12/arch/arm/boot/dts/arm/vexpress-v2p-ca9.dtb -nographic -append "root=/dev/mmcblk0 console=ttyAMA0" -drive file=/home/becky/project/wk3/2/qemu/a9rootfs.ext3,format=raw,if=sd -fsdev local,id=fsdev0,path=/home/becky/shared,security_model=none -device virtio-9p-device,fsdev=fsdev0,mount_tag=hostshare

    mkdir /mnt/host
（显示mkdir: can't create directory '/mnt/host': Read-only file system）

3）修改后重新执行  

    qemu-system-arm -M vexpress-a9 -m 512M -kernel kernel/linux-6.12/arch/arm/boot/zImage -dtb kernel/linux-6.12/arch/arm/boot/dts/arm/vexpress-v2p-ca9.dtb -nographic -append "root=/dev/mmcblk0 rw console=ttyAMA0" -drive file=/home/becky/project/wk3/2/qemu/a9rootfs.ext3,format=raw,if=sd -fsdev local,id=fsdev0,path=/home/becky/shared,security_model=none -device virtio-9p-device,fsdev=fsdev0,mount_tag=hostshare

    mkdir -p /mnt/host
    mount -t 9p -o trans=virtio hostshare /mnt/host



### 5.2 测试

1）在ubuntu执行  

    echo "hello from host" > ~/shared/test.txt

2）在qemu执行  

    cat /mnt/host/test.txt
成功，显示如下：  
![](https://github.com/beckyhuilin/Linux-project/blob/0673c1bd1997e9cd28b554c7dfd250853bf4537c/image/share_test.png)







