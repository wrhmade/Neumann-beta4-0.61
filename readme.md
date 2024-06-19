感谢你抽出百忙体验Neumann!
Neumann（原名WBoxOS）操作系统是基于HaribetoOS（也称为自制操作系统30天中的纸娃娃系统）的图形操作系统，支持8位颜色。在VMware中启动仅需2秒（256MB内存，内存越大，启动速度越慢）。这个项目由原71GN Deep Space中的WRH（现位于W24 Studio）从2023年12月开始编写。这是一个开源项目，任何人都可以下载这个源代码的副本来编译、运行。
编译方法（不支持Linux/macOS编译，使用此操作系统的用户需要Windows虚拟机）：
1.打开src文件夹，然后打开!cons_nt.bat（Windows NT系列操作系统，最低Windows NT 4.0，包括但不限于Windows 2000/XP/Vista/7/8/8.1/10/11）或!cons_9x.bat（Windows 95/98/ME操作系统），打开命令行窗口；
2.输入make full编译操作系统；如果编译成功，您将得到bootfd.img（beta 2及以下版本为haribote.img），这是操作系统的软盘映像，可以放置在虚拟机中运行，也可以写入软盘在真实机器上运行（注意：这个机器从没有实机测试过，仅供学习用途）。
使用此操作系统的教程：按Shift+F2键调出控制台（通常在启动后会自动出现）在控制台中输入命令，如shutdown、mem（内存使用情况）、dir（磁盘文件列表），以运行系统的内置命令，或运行应用程序（只需输入文件名）；
想要查询所有的命令，请在控制台中使用help命令。

Thank you for taking the time to experience Neumann!
Neumann (formerly known as WBoxOS) operating system is a graphics operating system based on HaribetoOS (also known as the paper doll system in 30 days of self-made operating system), supporting 8-bit colors. Starting in VMware only takes 2 seconds (256MB of memory, the larger the memory, the slower the startup speed). This project was written by WRH (now located in W24 Studio) from the original 71GN Deep Space in December 2023. This is an open source project, and anyone can download a copy of this source code to compile and run.
Compilation method (does not support Linux/macOS compilation, users using this operating system require a Windows virtual machine):
1. Open the src folder and then open it! Consn.bat (Windows NT series operating system, minimum Windows NT 4.0, including but not limited to Windows 2000/XP/Vista/7/8/8.1/10/11) or! Cons_9x.bat (Windows 95/98/ME operating system), open a command line window;
2. Enter make full to compile the operating system; If the compilation is successful, you will receive bootfd.img (haribote. img for beta 2 and below), which is the floppy disk image of the operating system. It can be placed on a virtual machine to run, or written to a floppy disk to run on a real machine (note: this machine has never been tested on a real machine and is for learning purposes only).
Tutorial on using this operating system: Press Shift+F2 to bring up the console (usually appearing automatically after startup), enter commands in the console, such as shutdown Mem (memory usage) Dir (disk file list) to run the system's built-in commands or run applications (simply enter the file name);
To query all commands, please use the help command in the console.