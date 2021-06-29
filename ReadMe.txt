Tetrix.sln         为VC2005项目文件
Tetrix_2003.sln    为VC2003项目文件
STM32文件夹下面为STM32固件代码，IAR4.42编译


Tetrix.sln         Solution file for VC2005
Tetrix_2003.sln    Solution file for VC2003
.\STM32            Source code for the STM32 firmware, compile by IAR4.42 for ARM


此项目是 stm32f103 模拟USB摄像头，画面是内部的缓冲，充当屏幕显示。内部实现了俄罗斯方块小游戏，显示在摄像头画面上。

以下摘抄自https://bbs.21ic.com/icview-163992-1-1.html

Google Code下载: http://lxyppc-tetrix.googlecode.com/files/TetrisSrc.zip
编译环境IAR 4.42
工程配置说明：
STM3210E-EVAL        红牛开发板使用
STM3210B-EVAL        万利199开发板使用
STM3210B-HEX          生成万利开发板Hex文件
SIM                           程序仿真
RAM_DEBUG              RAM中调试程序

红牛开发板原理图  红牛电路图(黑白).pdf (101.01 KB, 下载次数: 1093)
万利开发板原理图http://www.manley.com.cn/web/admin_ml32/pic/down/STM3210B-LK1_UM.pdf
开发板是万利的那个带有ST-Link2的199开发板  STM3210B-LK1
上面有一块STM32F103VBT6，这个片子有128K的Flash，20K的RAM
开发板上面可以用到的资源
·1 个LCD 显示,通过跳线选择连接LCD
·四个LED 发光管
·一个五方向输入摇杆
·两个GPIO 按键
可惜那个LCD是米字的LCD，如果是点阵的话，就可以在它上面画图了
不过不要紧，我之前做了一个OLed显示的小东西http://blog.ednchina.com/lxyppc/725361/message.aspx
在调试它的UI的时候我把开发板虚拟成了一个USB设备，并将数据以摄像头的格式发送上来，这样我就可以在电脑上直接调试了。
受此思路影响，只需要将游戏图像数据转换成摄像头数据发送上来，这样就解决了没有显示屏的问题，实际上把电脑显示器当成了显示屏。
最后我将这块开发板虚拟成了一个USB摄像头和一个USB鼠标，不玩游戏的时候可做鼠标用。游戏的时候画面通过摄像头传到电脑上来。
未来计划：
拆解一个USB的游戏手柄，将里面的主控芯片换成STM32，在实现手柄所有功能的同时，虚拟出一个摄像头设备让游戏手柄变成“游戏”手柄。
现在已经成功虚拟出了一个摄像头和一个鼠标设备，剩下来要做的只需要考虑在硬件上怎样改造手柄。

Hex文件下载  Tetris.zip (54 KB, 下载次数: 161)
直接下载到万利199元的开发板中，然后重新插拔一次USB线。
会发现一个USB摄像头，和一个USB鼠标。未打开摄像头时可当鼠标使用，打开摄像头后即可开始游戏。关闭摄像头游戏自动暂停。
资源使用情况
  9 568 bytes of CODE  memory
  4 855 bytes of DATA  memory
34 646 bytes of CONST memory
游戏说明：
当USB摄像头没有打开时:五方向输入摇杆控制鼠标的上下左右，KEY2为鼠标左键，KEY3为鼠标右键
当USB摄像头打开时:左右键移动方块，向上键旋转方块，向下键让方块快速下落，KEY2为开始/暂停
工程源代码在Google Code上，编译环境IAR4.42
http://code.google.com/p/lxyppc-tetrix/
SVN地址:
http://lxyppc-tetrix.googlecode.com/svn/trunk
游戏“快照”




