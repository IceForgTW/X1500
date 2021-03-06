#ifndef __UIMG_MODULE_SDK_VERSION_H__
#define __UIMG_MODULE_SDK_VERSION_H__


//模组开发包的版本信息
#define MODULE_SDK_VERSION		"1.03.042"



#endif//__UIMG_MODULE_SDK_VERSION_H__

//1.02.020-2008-11-14
//1.修改了在批处理设置中对信息截取设置处理的错误导致了设置无法成功的问题
//2.整理了批处理设置数据转换的代码，合并实现代码相同项，使程序更简洁

//1.02.019-2008-08-04
//1.加入了DM码的矩形码识别设置和及PAD后码词添加设置

//1.02.018-2008-08-01
//1.加入了USB的S/N存储和加载
//2.把PS/2参数整合到KBW中

//1.02.017-2008-07-21
//加入了HID-POS接口设置

//1.02.016-2008-07-16
//汉信码的英文名称统一为：Chinese Sensible Code

//1.02.015-2008-07-15
//修改了系统相关参数查询中汉信码的名称

//1.02.014-2008-07-15
//将版本休息进行重新排序. 硬件信息/应用信息/UIMG信息/MLIB信息

//1.02.013-2008-07-08
//(1)修改了1.02.011版本中，一维条码最大最小长度限制不超过127，程序中写成31了

//1.02.012-2008-07-03
//(1)加入了ManufactureID设置，在系统相关信息查询中可以查询到ManufactureID
//(2)S/N，ESN，Date几个设置加入了长度限制1到20字节

//1.02.011-2008-06-26
//(1)一维条码最大最小长度设置不能超过127，二维条码最大最小长度设置不能超过65535，在程序中加入了这些限制
//(2)加入了二维条码参数设置，并修改了一维条码参数设置的错误
//(3)加入了对QuickSet二维条码参数设置与查询的支持

//1.02.010-2008-05-30
//配合3.01.009进行AIM ID前缀的功能更改

//1.02.009-2008-05-19
//加入PS2/HID-USB接口设置
//更改了KBW的参数配置

//1.02.008-2008-02-21
//加入倒计时器用于定时休眠唤醒
//将连续识读模式设计为固定时间间隔休眠300ms并自动唤醒
//将自动感应模式设计为在检测判断的每次循环中固定休眠200ms并自动唤醒.

//1.02.007-2007-01-22
//将对图像处理API查询调用接口部分放置于图像处理库中

//1.02.006-2008-01-21
//更改了对UIMG版本信息的调用。使其统一在图像处理库中

//1.02.005-2008-01-08
//增加对一些图像处理API的查询调用接口

//1.02.004-2007-12-29
//添加了Aztec的多码同图设置，另外PDF417、QR及DataMatrix的设置加上了宏定义限制

//1.02.003-2007-12-21
//发送图像时，先保留看门狗的复位时间，并将其禁用，待图像发送完成后恢复看门狗。
//防止其在发送图像时自动重新启动

//1.02.002-2007-12-17
//将解码后的信息发送做成回调函数及默认处理两种方式
//当回调函数为空时,采用默认处理,否则用回调函数进行处理

//1.02.001 只发过2套
//将解码后持续按键的处理移到App中处理。

//1.02.000
//2007.11.12  加入汉信码/AZTEC 及禁止/使能/最大最小长度设置.
//更改了POS_FULL39的值,由13变成14,与Decoder统一

//1.01.Spc	特别裁剪版本,提供给大APP的EM3000使用
//1.02.Pre	
//2007.10.18 增加汉信码识读 

//1.01.020
//2007.09.30 改为读码成功后，非常亮模式下，照明灯是关闭的
//1.01.019
//2007.09.29 改为只在识读成功后的闪烁绿色灯光进行提示
//1.01.018
//2007.09.20 加大了版本信息获取的帧缓存空间
//1.01.017
//2007.09.20 更改了蓝牙设置不能设置10个字符以上的ID的错误
//1.01.016
//2007.09.17 将通讯口的切换与关闭分离，提供了检测是否打开及关闭的调用接口
//1.01.015
//2007.09.13 严格了对关键字比对时的长度限制.改进了对镜像的设置。
//1.01.014
//2007.09.13 增大了信息查询时的返回帧的最大长度,达到512
//1.01.013
//2007.09.13 加入了查询关键字指令
//1.01.012
//2007.09.12 加入了设置镜像的关键字设置,使得有该设置为on的设备始终为镜像
//			 通过设置~YesFFP~后设置Mirr=On来开启Mirr=Off来关闭.
						
//1.01.011
//2007.09.12 修正Y_commXXX中对超时判断比给定值大了10倍的问题.
//1.01.010
//2007.09.12 更改V_commAutoCheckCmd中IsEscCtrl调用后未加break的错误.
//1.01.009
//2007.09.10 加入二次开发中，用户默认保存与导入的调用。
//1.01.008
//2007.09.07 加入了图像地址及按键检测的替代功能设置方式
//1.01.007
//2007.09.06 对利用数据通讯进行设置时的信息读取超时限制统一加长
//1.01.006
//2007.09.05 改正应答对OCR是否使能的查询的表达。
//1.01.005
//2007.09.05 将Aztec设置与控制功能加入
//1.01.004
//2007.09.05 将PowerOnDisplay设了标志，使它仅被调用一次，利于将其放在内核启动中调用。
//1.01.003
//2007.09.05 将ChkStringParam字符串指令中的用户命令及LIC控制等命令删除。
//1.01.002
//2007.08.29 将对焦灯的Sensor Mode失效，变成Normal Mode
//1.01.001
//2007.08.27 完成对BlueTooth的设置支持

//1.01.000 2007.08.21 发布
//更改1.00.007上错误后制定版本
//1.00.007 2007.08.20
//2007.08.17 增加了对1D双码/DM双码/QR双码的支持
//2007.08.17 增加了对RowMirror设置的支持
//2007.08.16 增加了BlueTooth支持

//1.00.006 2007.08.15
//2007.08.15 增加了用户自定义解码的禁止使能设置
//2007.08.15 更改了自动感应模式的处理，并增加了该模式下的按键触发
//2007.08.14 增加了~PowerOff~指令处理
//2007.08.14 增加USB Serial支持,增加PowerOff指令处理支持

//1.00.005 2007-08-13
//2007.08.13 添加了可增加用户自定义加入版本信息的功能.  可以同时查询到app的版本信息
//2007.08.13 添加U_appMsgAddTerminal功能,将对信息编辑与添加终止字符进行区分

//1.00.004 2007-08-07
//2007.08.07 将通讯接口自动切换做了完善。
//2007.08.03 OV7141的调整方法恢复为原来的方法

//1.00.003 2007-08-02
//2007.08.02 增加了发送开机信息的接口函数
//2007.08.01 将通讯接口设为,若与设定类型不一致,则间歇尝试继续打开.
//2007.07.31 改进了MT9V022中的亮度计算函数及调整方法,改进了OV7141的调整方法.
//MT9v022里计算图像的亮度函数可能有问题，待查。
//2007.07.30 改进了通讯口初始化处理中的处理顺序
//2007.07.19 将发送图像时对USB接口的特殊处理,进行1:1时特别限定.
//2007.07.19 将对焦灯常亮模式时,拍图时刻将对焦灯关闭,随后恢复.

//Ver: 1.00.002
//2007.07.18 对DataMatrix/PDF417/QRCode/增加了对长度的限制
//2007.07.18 加入了QR Code Model 1支持
//2007.07.18 修改了CodeZ使能与禁止相反的错误
//2007.07.18 修改了OCR-B使能与禁止相反的错误
//2007.07.17 修改了传输图像的函数,减小了缓冲区
//2007.07.17 修改了通讯接口选择部分,对已经打开的接口,若欲切换成相同接口,
//			 参数无变化,则不先关闭已打开的接口

//Ver: 1.00.001 2007-06-28
//修正了识读设备码后，系统启动即从ESC0变成ESC1模式的问题。
//修正了不能使用批设置码的问题。
//修正了不能通过设置码批设置设置ESN、DATE、SN的问题。

//Ver: 1.00.000
//以样品形式释出

