#ifndef __SC_EXTFUNC_H__
#define __SC_EXTFUNC_H__

#ifdef __cplusplus
extern"C" {
#endif

#define  SC_CFG_NSYS               0  //当前运行
#define  SC_CFG_SYS                1  //系统配置
#define  SC_CFG_FACT               2  //出厂配置
#define  SC_CFG_USER               3  //用户配置
/************************************************************************/
/****************************************************************************
*@产品默认设置
****************************************************************************/
//加载出厂默认设置	Tag:FACDEF
int LoadFactoryDefaultCFG_Set (sc_param_st *pParam);
int LoadFactoryDefaultCFG_Get (sc_param_st *pParam);

//加载用户默认设置	Tag:CUSDEF
int LoadUserDefaultCFG_Set(sc_param_st *pParam);
int LoadUserDefaultCFG_Get(sc_param_st *pParam);

//保存用户默认设置	Tag:CUSSAV
int SaveUserDefaultCFG_Set(sc_param_st *pParam);
int SaveUserDefaultCFG_Get(sc_param_st *pParam);

//删除用户默认设置	Tag:CUSSAV
int DeleteUserDefaultCFG_Set(sc_param_st *pParam);
int DeleteUserDefaultCFG_Get(sc_param_st *pParam);

//保存出厂默认设置	Tag:SAVEFY
int SaveFactoryDefaultCFG_Set(sc_param_st *pParam);
int SaveFactoryDefaultCFG_Get(sc_param_st *pParam);

//加载通用默认设置	Tag:GENDEF
int LoadGeneralDefaultCFG_Set(sc_param_st *pParam);
int LoadGeneralDefaultCFG_Get(sc_param_st *pParam);

/****************************************************************************
*@通讯接口
****************************************************************************/
//用于各通讯接口类型的切换设置	Tag:INF
int CommunicationInterface_Set(sc_param_st *pParam);
int CommunicationInterface_Get(sc_param_st *pParam);

//用于RS232波特率的切换	Tag:232BAD
int RS232Baudrate_Set(sc_param_st *pParam);
int RS232Baudrate_Get(sc_param_st *pParam);

//用于RS232的奇偶校验	Tag:232PAR
int RS232Parity_Set(sc_param_st *pParam);
int RS232Parity_Get(sc_param_st *pParam);

//用于RS232停止位的设置	Tag:232STP
int RS232StopBits_Set(sc_param_st *pParam);
int RS232StopBits_Get(sc_param_st *pParam);

//用于RS232数据位的设置	Tag:232DAT
int RS232DataBits_Set(sc_param_st *pParam);
int RS232DataBits_Get(sc_param_st *pParam);

//用于RS232硬件自动流控的使能和禁止	Tag:232AFL
int RS232HardwereAutoFlowCTL_Set(sc_param_st *pParam);
int RS232HardwereAutoFlowCTL_Get(sc_param_st *pParam);

//用于RS232流控的设置	Tag:232FLW
int RS232NoAutoFlowCTL_Set(sc_param_st *pParam);
int RS232NoAutoFlowCTL_Get(sc_param_st *pParam);

//用于Keyboard Country Layout（键盘语言）的设置	Tag:KBWCTY
int KBWCountryLayout_Set(sc_param_st *pParam);
int KBWCountryLayout_Get(sc_param_st *pParam);

//用于传输未知字符的提示音使能、禁止	Tag:KBWBUC
int KBWBeepOnUnknownChar_Set(sc_param_st *pParam);
int KBWBeepOnUnknownChar_Get(sc_param_st *pParam);

//用于虚拟键盘类型1的使能、禁止	Tag：KBWALT
int KBWEmulateAltKeypad1_Set(sc_param_st *pParam);
int KBWEmulateAltKeypad1_Get(sc_param_st *pParam);

//用于虚拟键盘类型2的禁止、类型设置	Tag:KBWALK
int KBWEmulateAltKeypad2_Set(sc_param_st *pParam);
int KBWEmulateAltKeypad2_Get(sc_param_st *pParam);

//用于功能按键组合Ctrl+ASCII的使能和禁止	Tag:KBWFKM
int KBWCtrlPlusStatus_Set(sc_param_st* pParam);
int KBWCtrlPlusStatus_Get(sc_param_st* pParam);

//用于内部键盘输出延时的设置	Tag:KBWDLY
int KBWInterKeyStrokeDelay_Set(sc_param_st *pParam);
int KBWInterKeyStrokeDelay_Get(sc_param_st *pParam);

//用于键盘Caps Lock键的使能、禁止	Tag:KBWCAP
int KBWCapsLockStatus_Set(sc_param_st *pParam);
int KBWCapsLockStatus_Get(sc_param_st *pParam);

//用于键盘的大小写转换功能的使能、禁止	Tag：KBWCAS
int KBWConvertCaseStatus_Set(sc_param_st *pParam);
int KBWConvertCaseStatus_Get(sc_param_st *pParam);

//用于设置虚拟数字键盘的输出类型	Tag:KBWNUM
int KBWEmulateNUMPADType_Set(sc_param_st *pParam);
int KBWEmulateNUMPADType_Get(sc_param_st *pParam);

//用于设置虚拟数字键盘的运算符"+,-,*"是否采用采用Numeric Keypad 键值 	Tag:KBWNCH
int KBWEmulateNUMPADOperatorStatus_Set(sc_param_st *pParam);
int KBWEmulateNUMPADOperatorStatus_Get(sc_param_st *pParam);

//用于设置快速模式是开启或者关闭
int KBWFastMode_Set(sc_param_st *pParam);
int KBWFastMode_Get(sc_param_st *pParam);

/********************************************************************************************************/

//用于KBW轮询速度的设置和查询
int KBWPollRate_Set(sc_param_st *pParam);
int KBWPollRate_Get(sc_param_st *pParam);

//用于KBW CODE PAGE设置和查询
int KBWCodePage_Set(sc_param_st *pParam);
int KBWCodePage_Get(sc_param_st *pParam);

//用于设置PS2连接PC时，从线是否连接键盘
int PS2KeyboardConect_Set(sc_param_st *pParam);
int PS2KeyboardConect_Get(sc_param_st *pParam);

//用于设置PS2的CLK频率
int PS2KeyboardClock_Set(sc_param_st *pParam);
int PS2KeyboardClock_Get(sc_param_st *pParam);

/****************************************************************************
*@无线枪相关设置码
****************************************************************************/
//用于恢复底座出厂设置 Tag:BASFDF
int LoadFactoryCFGOfPedestal_Set(sc_param_st *pParam);

//用于底座清空配对信息 Tag:BASCLP
int ClearConfigInfoOfPedestal_Set(sc_param_st *pParam);

//用于查询无线扫描器相关全部信息	Tag WLSQAL
int AllInfoInWirelessScanner_Get(sc_param_st *pParam);

//用于查询底座系统信息	Tag BASQSY
int SysInfoOfPedestal_Get(sc_param_st *pParam);

//用于查询底座固件版本号	Tag BASQFW
int FirmwareVerOfPedestal_Get(sc_param_st *pParam);

//用于查询底座产品型号	Tag BASQHN
int ProductTypeOfPedestal_Get(sc_param_st *pParam);

//用于查询底座产品序列号	Tag BASQSN
int SerialNumOfPedestal_Get(sc_param_st *pParam);

//用于查询底座产品生产日期	Tag BASQDA
int DateOfPedestal_Get(sc_param_st *pParam);

//用于Batch Mode的设置	Tag：WLSBAT
int BatchModesOfWirelessScanner_Set(sc_param_st *pParam);
int BatchModesOfWirelessScanner_Get(sc_param_st *pParam);

//用于查询用户FLASH内条码数量	Tag:WLSQFC
int AmountOfCodesInUserFlash_Get(sc_param_st *pParam);

//用于清除用户FLASH内条码		Tag:WLSCLF
int ClearCodesInUserFlash_Set(sc_param_st *pParam);

//用于开启、关闭核对功能		Tag：WLSCHK
int CheckingFunctionEnable_Set(sc_param_st *pParam);
int CheckingFunctionEnable_Get(sc_param_st *pParam);

//用于设置核对超时时间			Tag：WLSCHT
int TimeoutOfCheckingFunction_Set(sc_param_st *pParam);
int TimeoutOfCheckingFunction_Get(sc_param_st *pParam);

//用于设置待机时间以进入自动关机 Tag：WLSAPO
int IdleTimeForAutoPowerdown_Set(sc_param_st *pParam);
int IdleTimeForAutoPowerdown_Get(sc_param_st *pParam);

//用于设置Zigbee无线信道	Tag：WLSZCH
int SelectChannelOfZigbee_Set(sc_param_st *pParam);
int SelectChannelOfZigbee_Get(sc_param_st *pParam);

//用于底座重启				Tag：BASRES
int ResetPedestal_Set(sc_param_st *pParam);

//用于设置无线传输应答超时时间 	Tag：WLSTTM
int TimeoutForACKInWirelessTrmt_Set(sc_param_st *pParam);
int TimeoutForACKInWirelessTrmt_Get(sc_param_st *pParam);

/****************************************************************************
*@照明灯、瞄准灯相关设置
****************************************************************************/
//设置照明灯的开、关	Tag：ILLSCN
int IlluminationEnable_Set(sc_param_st *pParam);
int IlluminationEnable_Get(sc_param_st *pParam);

//设置照明灯的颜色类别 	Tag：ILLCLR
int ColorTypeOfIllumination_Set(sc_param_st *pParam);
int ColorTypeOfIllumination_Get(sc_param_st *pParam);

//设置照明灯的位置		Tag：ILLPST
int PositionOfIllumination_Set(sc_param_st *pParam);
int PositionOfIllumination_Get(sc_param_st *pParam);

//设置瞄准灯的开、关	Tag：AMLENA
int AimerEnable_Set(sc_param_st *pParam);
int AimerEnable_Get(sc_param_st *pParam);

/****************************************************************************
*@串口触发指令
****************************************************************************/
//用于设置串口触发指令	Tag:SCNTRG
int	Trigger2DecoBySerialCommand_Set(sc_param_st *pParam);
int	Trigger2DecoBySerialCommand_Get(sc_param_st *pParam);

/****************************************************************************
*@Setting in Sense Mode
****************************************************************************/
//用于选择Sense Mode下读码成功后的工作状态（After A Good Read）	Tag:SENAGR
int StatusAfterGoodReadInSenseMode_Set(sc_param_st *pParam);
int StatusAfterGoodReadInSenseMode_Get(sc_param_st *pParam);

//用于选择Sense Mode下侦测状态时照明灯的开关	Tag:SENILL
int IlluStatusInSensingForSenseMode_Set(sc_param_st *pParam);
int IlluStatusInSensingForSenseMode_Get(sc_param_st *pParam);

/****************************************************************************
*@Exposure Imaging
****************************************************************************/
//设置镜头的曝光类型		Tag:EXPLVL
int ExposureTypes_Set(sc_param_st *pParam);
int ExposureTypes_Get(sc_param_st *pParam);

/****************************************************************************
*@Decode Area
****************************************************************************/
//设置解码区域	Tag：CADENA
int DecoArea_Set(sc_param_st *pParam);
int DecoArea_Get(sc_param_st *pParam);

//设置中心区域的顶端范围	Tag：CADTOP
int TopRangeOfCentralArea_Set(sc_param_st *pParam);
int TopRangeOfCentralArea_Get(sc_param_st *pParam);

//设置中心区域的底端范围	Tag：CADBOT
int BottomRangeOfCentralArea_Set(sc_param_st *pParam);
int BottomRangeOfCentralArea_Get(sc_param_st *pParam);

//设置中心区域的左端范围	Tag：CADLEF
int LeftRangeOfCentralArea_Set(sc_param_st *pParam);
int LeftRangeOfCentralArea_Get(sc_param_st *pParam);

//设置中心区域的右端范围	Tag：CADRIG
int RightRangeOfCentralArea_Set(sc_param_st *pParam);
int RightRangeOfCentralArea_Get(sc_param_st *pParam);

/****************************************************************************
*@解码区域选择（1D产品）
****************************************************************************/
//用于选择 “离中间区域的最近的一个条码输出”，“中间区域的一个条码输出” 
//或者“固定识读2、3、4个条码”
int DecoAreaLevelsFor1D_Set(sc_param_st *pParam);
int DecoAreaLevelsFor1D_Get(sc_param_st *pParam);

/****************************************************************************
*@输出间隔设置（1D产品）
****************************************************************************/
//用于选择每个条码的输出固定间隔时间	Tag：CDAINV
int IntervalTimeOfOutputingFor1D_Set(sc_param_st *pParam);
int IntervalTimeOfOutputingFor1D_Get(sc_param_st *pParam);

/****************************************************************************
*@Image Mirror(仅模组类产品才提供)							Tag:MIRROR
****************************************************************************/
//用于选择每个条码的输出固定间隔时间	Tag：CDAINV
int ImageMirrorTypes_Set(sc_param_st *pParam);
int ImageMirrorTypes_Get(sc_param_st *pParam);


//启动和退出设置码设置功能 SETUP
int StartExitSetup_Set(sc_param_st *pParam);
int StartExitSetup_Get(sc_param_st *pParam);

//开启和关闭设置码设置功能（低优先级别）
int OpenCloseSetup_Set(sc_param_st *pParam);
int OpenCloseSetup_Get(sc_param_st *pParam);

//设置码识读功能（高优先级别）
int EnableSettingCode_Set(sc_param_st *pParam);
int EnableSettingCode_Get(sc_param_st *pParam);

//设置码识读功能查询（高优先级别）
int QuerySettingCode_Set(sc_param_st *pParam);
int QuerySettingCode_Get(sc_param_st *pParam);

//设置码内容发送功能
int TrsmtSettingCode_Set(sc_param_st *pParam);
int TrsmtSettingCode_Get(sc_param_st *pParam);

//开机声音设置
int PowerOnVoice_Set(sc_param_st *pParam);
int PowerOnVoice_Get(sc_param_st *pParam);

//成功识读提示音使能设置
int GoodReadBeep_Set(sc_param_st *pParam);
int GoodReadBeep_Get(sc_param_st *pParam);

//成功识读提示音音量设置
int GoodReadBeepVolume_Set(sc_param_st *pParam);
int GoodReadBeepVolume_Get(sc_param_st *pParam);

//成功识读提示音音长设置
int GoodReadBeepDuration_Set(sc_param_st *pParam);
int GoodReadBeepDuration_Get(sc_param_st *pParam);

//成功识读提示音频率设置
int GoodReadBeepFreq_Set(sc_param_st *pParam);
int GoodReadBeepFreq_Get(sc_param_st *pParam);

//成功提示音的次数设置
int GoodReadBeepNumber_Set(sc_param_st *pParam);
int GoodReadBeepNumber_Get(sc_param_st *pParam);

//成功提示音的间隔时间设置
int GoodReadBeepIntval_Set(sc_param_st *pParam);
int GoodReadBeepIntval_Get(sc_param_st *pParam);

//失败提示音的使能设置
int BadReadBeepEnable_Set(sc_param_st *pParam);
int BadReadBeepEnable_Get(sc_param_st *pParam);

//失败识读提示音音量设置
int BadReadBeepVolume_Set(sc_param_st *pParam);
int BadReadBeepVolume_Get(sc_param_st *pParam);

//失败识读提示音音长设置
int BadReadBeepDuration_Set(sc_param_st *pParam);
int BadReadBeepDuration_Get(sc_param_st *pParam);

//失败识读提示音频率设置
int BadReadBeepFreq_Set(sc_param_st *pParam);
int BadReadBeepFreq_Get(sc_param_st *pParam);

//失败提示音的次数设置
int BadReadBeepNumber_Set(sc_param_st *pParam);
int BadReadBeepNumber_Get(sc_param_st *pParam);

//失败提示音的间隔时间设置
int BadReadBeepIntval_Set(sc_param_st *pParam);
int BadReadBeepIntval_Get(sc_param_st *pParam);

//语音播放功能使能设置
int VoiceEnable_Set(sc_param_st *pParam);
int VoiceEnable_Get(sc_param_st *pParam);

//语音播放间隔时间设置
int VoiceIntvalTime_Set(sc_param_st *pParam);
int VoiceIntvalTime_Get(sc_param_st *pParam);

//语音类型设置   CMD:VOICET
int VoiceTypeSelect_Set(sc_param_st *pParam);
int VoiceTypeSelect_Get(sc_param_st *pParam);

//语音模式设置   CMD:VOICEM
int VoiceModeSelect_Set(sc_param_st *pParam);
int VoiceModeSelect_Get(sc_param_st *pParam);

//语音音量设置   CMD:VOICEV
int VoiceVolume_Set(sc_param_st *pParam);
int VoiceVolume_Get(sc_param_st *pParam);

//成功识读灯使能设置
int GoodReadLedEnable_Set(sc_param_st *pParam);
int GoodReadLedEnable_Get(sc_param_st *pParam);

//成功识读灯持续时间设置
int GoodReadLedDuration_Set(sc_param_st *pParam);
int GoodReadLedDuration_Get(sc_param_st *pParam);

//成功识读震动使能设置
int GoodReadVibrateEnable_Set(sc_param_st *pParam);
int GoodReadVibrateEnable_Get(sc_param_st *pParam);

//成功识读震动持续时间设置
int GoodReadVibrateDuration_Set(sc_param_st *pParam);
int GoodReadVibrateDuration_Get(sc_param_st *pParam);

//开机提示灯使能设置
int PowerOnLedEnable_Set(sc_param_st *pParam);
int PowerOnLedEnable_Get(sc_param_st *pParam);

//工作模式选择设置
int WorkMode_Set(sc_param_st *pParam);
int WorkMode_Get(sc_param_st *pParam);

//软件模拟触发设置
int PureSoftTrig_Set(sc_param_st *pParam);
int PureSoftTrig_Get(sc_param_st *pParam);

//读码间歇时长设置
int RestTime_Set(sc_param_st *pParam);
int RestTime_Get(sc_param_st *pParam);

//感应模式下，等待图像稳定的时间设置
int ImgStabTimeout_Set(sc_param_st *pParam);
int ImgStabTimeout_Get(sc_param_st *pParam);

//读码时长设置
int ReadTime_Set(sc_param_st *pParam);
int ReadTime_Get(sc_param_st *pParam);

//相同条码延时开关设置
int RereadDelayEnable_Set(sc_param_st *pParam);
int RereadDelayEnable_Get(sc_param_st *pParam);

//相同条码延时时间设置
int RereadDelayTime_Set(sc_param_st *pParam);
int RereadDelayTime_Get(sc_param_st *pParam);

//间歇模式下的读码时间设置
int FlashOnTime_Set(sc_param_st *pParam);
int FlashOnTime_Get(sc_param_st *pParam);

//间歇模式下的间歇时间设置
int FlashOffTime_Set(sc_param_st *pParam);
int FlashOffTime_Get(sc_param_st *pParam);

//感应灵敏度设置
int Sensitivity_Set(sc_param_st *pParam);
int Sensitivity_Get(sc_param_st *pParam);

//Not Good Read使能设置
int NotGoodReadEnable_Set(sc_param_st *pParam);
int NotGoodReadEnable_Get(sc_param_st *pParam);

//Not Good Read 信息设置
int NotGoodReadMsg_Set(sc_param_st *pParam);
int NotGoodReadMsg_Get(sc_param_st *pParam);
int NotGoodReadLenth_Set(sc_param_st *pParam);
int NotGoodReadLenth_Get(sc_param_st *pParam);

//关机模式
int ModePowerOff_Set(sc_param_st *pParam);
int ModePowerOff_Get(sc_param_st *pParam);
//Idle模式
int ModeIdle_Set(sc_param_st *pParam);
int ModeIdle_Get(sc_param_st *pParam);
//Sleep模式
int ModeSleep_Set(sc_param_st *pParam);
int ModeSleep_Get(sc_param_st *pParam);
//test模式
int ModeTest_Set(sc_param_st *pParam);
int ModeTest_Get(sc_param_st *pParam);
//Reboot模式
int ModeReboot_Set(sc_param_st *pParam);
int ModeReboot_Get(sc_param_st *pParam);

//自动休眠功能选择
int AutoSleepEnable_Set(sc_param_st *pParam);
int AutoSleepEnable_Get(sc_param_st *pParam);

//自动休眠空闲时间设置
int AutoSleepIdleTime_Set(sc_param_st *pParam);
int AutoSleepIdleTime_Get(sc_param_st *pParam);

//安全级别设置
int SafeLv_Set(sc_param_st *pParam);
int SafeLv_Get(sc_param_st *pParam);

/*********************************************************************************************/
//////////////////////////////////////////////////////////////////////////
//查询系统信息
int SysInfo_Set(sc_param_st *pParam);
int SysInfo_Get(sc_param_st *pParam);
//查询固件版本号
int FirmwareVer_Set(sc_param_st *pParam);
int FirmwareVer_Get(sc_param_st *pParam);
//查询解码版本号
int DecoderVer_Get(sc_param_st *pParam);
int DecoderVer_Set(sc_param_st *pParam);
//查询硬件信息
int HardwareVer_Get(sc_param_st *pParam);
int HardwareVer_Set(sc_param_st *pParam);
//查询产品名称
int ProductName_Get(sc_param_st *pParam);
int ProductName_Set(sc_param_st *pParam);
//查询产品序列号
int ProductPID_Get(sc_param_st *pParam);
int ProductPID_Set(sc_param_st *pParam);
//查询产品生产日期
int ProduceDate_Get(sc_param_st *pParam);
int ProduceDate_Set(sc_param_st *pParam);
//查询产品OEM型号
int ProductOEMType_Get(sc_param_st *pParam);
int ProductOEMType_Set(sc_param_st *pParam);
//查询产品OEM序列号（ESN）
int ProductOEMESN_Get(sc_param_st *pParam);
int ProductOEMESN_Set(sc_param_st *pParam);
//查询产品OEM生产日期
int ProduceOEMDate_Get(sc_param_st *pParam);
int ProduceOEMDate_Set(sc_param_st *pParam);

//所有码制使能设置
int AllCodeTypeEnable_Set(sc_param_st *pParam);
int AllCodeTypeEnable_Get(sc_param_st *pParam);

//1D码制使能设置
int CodeType1DEnable_Set(sc_param_st *pParam);
int CodeType1DEnable_Get(sc_param_st *pParam);

//所有二维条码使能设置	Tag:ALL2DC
int CodeType2DEnable_Set(sc_param_st *pParam);
int CodeType2DEnable_Get(sc_param_st *pParam);

//所有邮政条码使能设置	Tag:ALLPST
int CodeTypePostCodeEnable_Set(sc_param_st *pParam);
int CodeTypePostCodeEnable_Get(sc_param_st *pParam);

//1D条码的单双码设置	Tag:A1DDOU
int DualCodeTypesFor1D_Set(sc_param_st *pParam);
int DualCodeTypesFor1D_Get(sc_param_st *pParam);

//识读反相条码使能设置	Tag:ALLINV
int InverterOfCodesEnable_Set(sc_param_st *pParam);
int InverterOfCodesEnable_Get(sc_param_st *pParam);
//所有EANUPC附加码设置
int AllEanUpcAddon_Set(sc_param_st *pParam);
int AllEanUpcAddon_Get(sc_param_st *pParam);

//设置Febraban字符间延时的使能、禁止	Tag：FEBSEN
int FebrabanInterCharDelayEnable_Set(sc_param_st *pParam);
int FebrabanInterCharDelayEnable_Get(sc_param_st *pParam);

//设置Febraban字符间延时时间	Tag:FEBSDT
int FebrabanInterCharDelay_Set(sc_param_st *pParam);
int FebrabanInterCharDelay_Get(sc_param_st *pParam);

//设置Febraban每12字符间延时的使能、禁止	Tag：FEBMEN
int FebrabanInterAreaDelayEnable_Set(sc_param_st *pParam);
int FebrabanInterAreaDelayEnable_Get(sc_param_st *pParam);

//设置Febraban每12字符间延时时间	Tag:FEBMDT
int FebrabanInterAreaDelay_Set(sc_param_st *pParam);
int FebrabanInterAreaDelay_Get(sc_param_st *pParam);
/****************************************************************************
*@普通Code 128
****************************************************************************/
//加载Code 128 默认设置	Tag：128DEF
int Code128LoadDefault_Set(sc_param_st *pParam);
int Code128LoadDefault_Get(sc_param_st *pParam);

//Code 128的使能、禁止	Tag：128ENA
int Code128Enable_Set(sc_param_st *pParam);
int Code128Enable_Get(sc_param_st *pParam);

//设置Code 128最小长度的设置	Tag：128MIN
int Code128MinLen_Set(sc_param_st *pParam);
int Code128MinLen_Get(sc_param_st *pParam);

//设置Code 128的最大长度	Tag：128MAX
int Code128MaxLen_Set(sc_param_st *pParam);
int Code128MaxLen_Get(sc_param_st *pParam);

//设置Code 128 CODEID的输出使能、禁止	Tag：CID002
int Code128CodeID_Set(sc_param_st *pParam);
int Code128CodeID_Get(sc_param_st *pParam);

//设置Code 128校验位发送的使能、禁止	Tag：128CHK
int Code128ChkTrsmt_Set(sc_param_st *pParam);
int Code128ChkTrsmt_Get(sc_param_st *pParam);

//设置Code 128的FNC1输出的使能、禁止	Tag：128FNC
int Code128FNC1_Set(sc_param_st *pParam);
int Code128FNC1_Get(sc_param_st *pParam);

/****************************************************************************
*@EAN-8
****************************************************************************/
//加载EAN-8的默认设置	Tag:EA8DEF
int EAN8LoadDefault_Set(sc_param_st *pParam);
int EAN8LoadDefault_Get(sc_param_st *pParam);

//设置EAN-8的使能、禁止	Tag:EA8ENA
int EAN8Enable_Set(sc_param_st *pParam);
int EAN8Enable_Get(sc_param_st *pParam);

//设置EAN-8校验位的使能、禁止	Tag：EA8CHK
int EAN8ChkTrsmt_Set(sc_param_st *pParam);
int EAN8ChkTrsmt_Get(sc_param_st *pParam);

//设置EAN-8 2位扩展码的使能、禁止	Tag:EA8AD2
int EAN8AddCode2Enable_Set(sc_param_st *pParam);
int EAN8AddCode2Enable_Get(sc_param_st *pParam);

//设置EAN-8 5位扩展码的使能、禁止	Tag:EA8AD5
int EAN8AddCode5Enable_Set(sc_param_st *pParam);
int EAN8AddCode5Enable_Get(sc_param_st *pParam);

//设置EAN-8拓展码的使能、禁止	Tag:EA8REQ
int EAN8AddedCodeEnable_Set(sc_param_st *pParam);
int EAN8AddedCodeEnable_Get(sc_param_st *pParam);

//设置EAN-8扩展码是否添加分隔符	Tag:EA8SEP
int EAN8SEPRInAddedCodeEnable_Set(sc_param_st *pParam);
int EAN8SEPRInAddedCodeEnable_Get(sc_param_st *pParam);

//设置EAN-8是否转换为EAN13或内容扩展为13位	Tag：EA8EXP
int EAN8ExpandedType_Set(sc_param_st *pParam);
int EAN8ExpandedType_Get(sc_param_st *pParam);

//设置EAN-8 CODEID的输出使能、禁止	Tag：CID004
int EAN8CodeID_Set(sc_param_st *pParam);
int EAN8CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@EAN-13
****************************************************************************/
//加载EAN13的默认通用设置	Tag：E13DEF
int EAN13LoadDefault_Set(sc_param_st *pParam);
int EAN13LoadDefault_Get(sc_param_st *pParam);

//设置EAN13的使能、禁止	Tag：E13ENA
int EAN13Enable_Set(sc_param_st *pParam);
int EAN13Enable_Get(sc_param_st *pParam);

//设置EAN13校验位的使能、禁止	Tag：E13CHK
int EAN13ChkTrsmt_Set(sc_param_st *pParam);
int EAN13ChkTrsmt_Get(sc_param_st *pParam);

//设置EAN13 2位 扩展码的使能、禁止  Tag:E13AD2
int EAN13AddCode2Enable_Set(sc_param_st *pParam);
int EAN13AddCode2Enable_Get(sc_param_st *pParam);

//设置EAN13 5位 扩展码的使能、禁止  Tag:E13AD5
int EAN13AddCode5Enable_Set(sc_param_st *pParam);
int EAN13AddCode5Enable_Get(sc_param_st *pParam);

//设置EAN13拓展码的使能、禁止	Tag:E13REQ
int EAN13AddedCodeEnable_Set(sc_param_st *pParam);
int EAN13AddedCodeEnable_Get(sc_param_st *pParam);

//设置EAN13扩展码是否添加分隔符	Tag:E13SEP
int EAN13SEPRInAddedCodeEnable_Set(sc_param_st *pParam);
int EAN13SEPRInAddedCodeEnable_Get(sc_param_st *pParam);

//设置EAN13 Beginning with 290 Addenda Tag：E132900、 E132901
int EAN13Beggin290Addenda_Set(sc_param_st *pParam);
int EAN13Beggin290Addenda_Get(sc_param_st *pParam);

//设置EAN13 Beginning with 378_379 Addenda Tag：E132780、 E132781
int EAN13Beggin378_379Addenda_Set(sc_param_st *pParam);
int EAN13Beggin378_379Addenda_Get(sc_param_st *pParam);

//设置EAN13 Beginning with 414_419 Addenda Tag：E134140、 E134141
int EAN13Beggin414_419Addenda_Set(sc_param_st *pParam);
int EAN13Beggin414_419Addenda_Get(sc_param_st *pParam);

//设置EAN13 Beginning with 434_439 Addenda Tag：E134340、 E134341
int EAN13Beggin434_439Addenda_Set(sc_param_st *pParam);
int EAN13Beggin434_439Addenda_Get(sc_param_st *pParam);

//设置EAN13 Beginning with 977 Addenda Tag：E139770、 E139771
int EAN13Beggin977Addenda_Set(sc_param_st *pParam);
int EAN13Beggin977Addenda_Get(sc_param_st *pParam);

//设置EAN13 Beginning with 978 Addenda Tag：E139780、 E139781
int EAN13Beggin978Addenda_Set(sc_param_st *pParam);
int EAN13Beggin978Addenda_Get(sc_param_st *pParam);

//设置EAN13 Beginning with 979 Addenda Tag：E139790、 E139791
int EAN13Beggin979Addenda_Set(sc_param_st *pParam);
int EAN13Beggin979Addenda_Get(sc_param_st *pParam);

//设置EAN13 CODEID的输出使能、禁止	Tag：CID005
int EAN13CodeID_Set(sc_param_st *pParam);
int EAN13CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@UPC-E
****************************************************************************/
//加载UPC-E的默认设置	Tag:UPEDEF
int UPCELoadDefault_Set(sc_param_st *pParam);
int UPCELoadDefault_Get(sc_param_st *pParam);

//设置UPC-E的使能、禁止	Tag：UPEENA
int UPCEEnable_Set(sc_param_st *pParam);
int UPCEEnable_Get(sc_param_st *pParam);

//设置UPC-E的UPC-E0使能、禁止	Tag：UPEEN0
int UPCE0Enable_Set(sc_param_st *pParam);
int UPCE0Enable_Get(sc_param_st *pParam);

//设置UPC-E的UPC-E1使能、禁止	Tag：UPEEN1
int UPCE1Enable_Set(sc_param_st *pParam);
int UPCE1Enable_Get(sc_param_st *pParam);

//设置UPC-E校验位的使能、禁止	Tag：UPECHK
int UPCEChkTrsmt_Set(sc_param_st *pParam);
int UPCEChkTrsmt_Get(sc_param_st *pParam);

//设置UPC-E 2位扩展码的使能、禁止 Tag:UPEAD2
int UPCEAddCode2Enable_Set(sc_param_st *pParam);
int UPCEAddCode2Enable_Get(sc_param_st *pParam);

//设置UPC-E 5位扩展码的使能、禁止 Tag:UPEAD5
int UPCEAddCode5Enable_Set(sc_param_st *pParam);
int UPCEAddCode5Enable_Get(sc_param_st *pParam);

//设置UPC-E拓展码的使能、禁止	Tag:UPEREQ
int UPCEAddedCodeEnable_Set(sc_param_st *pParam);
int UPCEAddedCodeEnable_Get(sc_param_st *pParam);

//设置UPC-E扩展码是否添加分隔符	Tag:UPESEP
int UPCESEPRInAddedCodeEnable_Set(sc_param_st *pParam);
int UPCESEPRInAddedCodeEnable_Get(sc_param_st *pParam);

//设置UPC-E 发送前文的类型	Tag：UPEPRE
int UPCETrsmtSysDigitType_Set(sc_param_st *pParam);
int UPCETrsmtSysDigitType_Get(sc_param_st *pParam);

//设置UPC-E使能、禁止转换为UPC-A Tag：UPEEXP
int UPCEExpandedType_Set(sc_param_st *pParam);
int UPCEExpandedType_Get(sc_param_st *pParam);

//设置UPC-E CODEID的输出使能、禁止	Tag：CID006
int UPCECodeID_Set(sc_param_st *pParam);
int UPCECodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@UPC-A
****************************************************************************/
//加载UPC-A的默认设置	Tag:UPADEF
int UPCALoadDefault_Set(sc_param_st *pParam);
int UPCALoadDefault_Get(sc_param_st *pParam);

//设置UPC-A的使能、禁止	Tag：UPAENA
int UPCAEnable_Set(sc_param_st *pParam);
int UPCAEnable_Get(sc_param_st *pParam);

//设置UPC-A校验位的使能、禁止	Tag：UPACHK
int UPCAChkTrsmt_Set(sc_param_st *pParam);
int UPCAChkTrsmt_Get(sc_param_st *pParam);

//设置UPC-A 2位扩展码的识读使能、禁止	Tag:UPAAD2
int UPCAAddCode2Enable_Set(sc_param_st *pParam);
int UPCAAddCode2Enable_Get(sc_param_st *pParam);

//设置UPC-A 5位扩展码的识读使能、禁止	Tag:UPAAD5
int UPCAAddCode5Enable_Set(sc_param_st *pParam);
int UPCAAddCode5Enable_Get(sc_param_st *pParam);

//设置UPC-E拓展码的使能、禁止	Tag:UPAREQ
int UPCAAddedCodeEnable_Set(sc_param_st *pParam);
int UPCAAddedCodeEnable_Get(sc_param_st *pParam);

//设置UPC-A扩展码是否添加分隔符	Tag:UPASEP
int UPCASEPRInAddedCodeEnable_Set(sc_param_st *pParam);
int UPCASEPRInAddedCodeEnable_Get(sc_param_st *pParam);

//设置UPC-A 发送前文的类型	Tag：UPAPRE
int UPCATrsmtSysDigitType_Set(sc_param_st *pParam);
int UPCATrsmtSysDigitType_Get(sc_param_st *pParam);

//设置UPC-A CODEID的输出使能、禁止	Tag：CID007
int UPCACodeID_Set(sc_param_st *pParam);
int UPCACodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Coupon
****************************************************************************/
//设置UPCA/EAN13输出Extended Coupon Code的状态（off,allow,require三种）Tag:CPNENA
int UPCA_EAN13WithExtendedCoupon_Set(sc_param_st *pParam);
int UPCA_EAN13WithExtendedCoupon_Get(sc_param_st *pParam);

//Coupon GS1 DataBar Output Tag:CPNGS1
int CouponGS1DataBarOutput_Set(sc_param_st *pParam);
int CouponGS1DataBarOutput_Get(sc_param_st *pParam);

/****************************************************************************
*@Interleaved 2 of 5
****************************************************************************/
//加载Inter2of5的默认设置	Tag:I25DEF
int Inter2of5LoadDefault_Set(sc_param_st *pParam);
int Inter2of5LoadDefault_Get(sc_param_st *pParam);

//设置Inter2of5的使能、禁止	Tag：I25ENA
int Inter2of5Enable_Set(sc_param_st *pParam);
int Inter2of5Enable_Get(sc_param_st *pParam);

//设置Inter2of5的识读最小长度	Tag：I25MIN
int Inter2of5MinLen_Set(sc_param_st *pParam);
int Inter2of5MinLen_Get(sc_param_st *pParam);

//设置Inter2of5的识读最大长度	Tag：I25MAX
int Inter2of5MaxLen_Set(sc_param_st *pParam);
int Inter2of5MaxLen_Get(sc_param_st *pParam);

//设置Inter2of5校验位的使能、禁止	Tag：I25CHK
int Inter2of5ChkTrsmt_Set(sc_param_st *pParam);
int Inter2of5ChkTrsmt_Get(sc_param_st *pParam);

//设置识读固定长度条码功能的使能、禁止	Tag:I25LEN
int Inter2of5DecoFixedLenEnable_Set(sc_param_st *pParam);
int Inter2of5DecoFixedLenEnable_Get(sc_param_st *pParam);

//设置Inter2of5添加固定长度值	Tag:I25LSE
int Inter2of5AddFixedLen_Set(sc_param_st *pParam);
int Inter2of5AddFixedLen_Get(sc_param_st *pParam);

//设置Inter2of5取消固定长度值	Tag:I25LCA
int Inter2of5CancelFixedLen_Set(sc_param_st *pParam);
int Inter2of5CancelFixedLen_Get(sc_param_st *pParam);

//设置Inter2of5 的Febraban 使能态，进行信息扩展	Tag:I25FBB
int Inter2of5FebrabanEnable_Set(sc_param_st *pParam);
int Inter2of5FebrabanEnable_Get(sc_param_st *pParam);

//设置Inter2of5 CODEID的输出使能、禁止	Tag：CID008
int Inter2of5CodeID_Set(sc_param_st *pParam);
int Inter2of5CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@ITF-14
****************************************************************************/
//加载ITF-14 的默认设置	Tag:I14DEF
int ITF14LoadDefault_Set(sc_param_st *pParam);
int ITF14LoadDefault_Get(sc_param_st *pParam);

//设置ITF-14 的使能、禁止及校验位的传送	Tag：I14ENA
int ITF14Enable_Set(sc_param_st *pParam);
int ITF14Enable_Get(sc_param_st *pParam);

//设置ITF-14 CODEID的输出使能、禁止	Tag：CID009
int ITF14CodeID_Set(sc_param_st *pParam);
int ITF14CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@ITF-6
****************************************************************************/
//加载ITF-6 的默认设置	Tag:IT6DEF
int ITF6LoadDefault_Set(sc_param_st *pParam);
int ITF6LoadDefault_Get(sc_param_st *pParam);

//设置ITF-6 的使能、禁止及校验位的传送	Tag：IT6ENA
int ITF6Enable_Set(sc_param_st *pParam);
int ITF6Enable_Get(sc_param_st *pParam);

//设置ITF-6 CODEID的输出使能、禁止	Tag：CID010
int ITF6CodeID_Set(sc_param_st *pParam);
int ITF6CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Matrix 2 of 5
****************************************************************************/
//加载Matrix 2 of 5 的默认设置	Tag:M25DEF
int Matrix2of5LoadDefault_Set(sc_param_st *pParam);
int Matrix2of5LoadDefault_Get(sc_param_st *pParam);

//设置Matrix 2 of 5 的使能、禁止	Tag：M25ENA
int Matrix2of5Enable_Set(sc_param_st *pParam);
int Matrix2of5Enable_Get(sc_param_st *pParam);

//设置Matrix 2 of 5的识读最小长度	Tag：M25MIN
int Matrix2of5MinLen_Set(sc_param_st *pParam);
int Matrix2of5MinLen_Get(sc_param_st *pParam);

//设置Matrix 2 of 5的识读最大长度	Tag：M25MAX
int Matrix2of5MaxLen_Set(sc_param_st *pParam);
int Matrix2of5MaxLen_Get(sc_param_st *pParam);

//设置Matrix 2 of 5校验位的使能、禁止	Tag：I25CHK
int Matrix2of5ChkTrsmt_Set(sc_param_st *pParam);
int Matrix2of5ChkTrsmt_Get(sc_param_st *pParam);

//设置Matrix 2 of 5 CODEID的输出使能、禁止	Tag：CID011
int Matrix2of5CodeID_Set(sc_param_st *pParam);
int Matrix2of5CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Code39
****************************************************************************/
//加载Code39的默认设置	Tag:C39DEF
int Code39LoadDefault_Set(sc_param_st *pParam);
int Code39LoadDefault_Get(sc_param_st *pParam);

//设置Code39的使能、禁止	Tag：C39ENA
int Code39Enable_Set(sc_param_st *pParam);
int Code39Enable_Get(sc_param_st *pParam);

//设置Code39的识读最小长度	Tag：C39MIN
int Code39MinLen_Set(sc_param_st *pParam);
int Code39MinLen_Get(sc_param_st *pParam);

//设置Code39的识读最大长度	Tag：C39MAX
int Code39MaxLen_Set(sc_param_st *pParam);
int Code39MaxLen_Get(sc_param_st *pParam);

//设置Code39校验位的使能、禁止	Tag：C39CHK
int Code39ChkTrsmt_Set(sc_param_st *pParam);
int Code39ChkTrsmt_Get(sc_param_st *pParam);

//设置Code39的传送起始符与终止符的使能、禁止	Tag：C39TSC
int Code39TrsmtSTXETXEnable_Set(sc_param_st *pParam);
int Code39TrsmtSTXETXEnable_Get(sc_param_st *pParam);

//设置Code39 Full ASCII的使能、禁止	Tag：C39ASC
int Code39FullASCIIEnable_Set(sc_param_st *pParam);
int Code39FullASCIIEnable_Get(sc_param_st *pParam);

//设置Code39的Code 32 Pharmaceutical (PARAF)的使能、禁止	Tag：C39E32
int Code39Code32PARAFEnable_Set(sc_param_st *pParam);
int Code39Code32PARAFEnable_Get(sc_param_st *pParam);

//设置Code39 是否添加Code32前缀	Tag:C39S32
int Code39AddSTXOfCode32Enable_Set(sc_param_st *pParam);
int Code39AddSTXOfCode32Enable_Get(sc_param_st *pParam);

//设置Code39 是否添加Code32起始符/终止符	Tag:C39T32
int Code32TrsmtSTXETXEnable_Set(sc_param_st *pParam);
int Code32TrsmtSTXETXEnable_Get(sc_param_st *pParam);

//设置Code39 是否添加Code32校验符	Tag:C39C32
int Code32ChkTrsmt_Set(sc_param_st *pParam);
int Code32ChkTrsmt_Get(sc_param_st *pParam);

//设置Code39 CODEID的输出使能、禁止	Tag：CID013
int Code39CodeID_Set(sc_param_st *pParam);
int Code39CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Codabar
****************************************************************************/
//加载Codabar的默认设置	Tag:CBADEF
int CodabarLoadDefault_Set(sc_param_st *pParam);
int CodabarLoadDefault_Get(sc_param_st *pParam);

//设置Codabar的使能、禁止	Tag：CBAENA
int CodabarEnable_Set(sc_param_st *pParam);
int CodabarEnable_Get(sc_param_st *pParam);

//设置Codabar的识读最小长度	Tag：CBAMIN
int CodabarMinLen_Set(sc_param_st *pParam);
int CodabarMinLen_Get(sc_param_st *pParam);

//设置Codabar的识读最大长度	Tag：CBAMAX
int CodabarMaxLen_Set(sc_param_st *pParam);
int CodabarMaxLen_Get(sc_param_st *pParam);

//设置Codabar校验位的使能、禁止	Tag：CBACHK
int CodabarChkTrsmt_Set(sc_param_st *pParam);
int CodabarChkTrsmt_Get(sc_param_st *pParam);

//设置Codabar传送起始符与终止符的使能、禁止	Tag：CBATSC
int CodabarTrsmtSTXETXEnable_Set(sc_param_st *pParam);
int CodabarTrsmtSTXETXEnable_Get(sc_param_st *pParam);

//设置Codabar起始符与终止符的格式	Tag：CBASCF
int CodabarSTXETXFormats_Set(sc_param_st *pParam);
int CodabarSTXETXFormats_Get(sc_param_st *pParam);

//设置Codabar的CLSI编辑的使能、禁止	Tag:CBACLS
int CodabarCLSIEdit_Set(sc_param_st *pParam);
int CodabarCLSIEdit_Get(sc_param_st *pParam);

//设置Codabar CODEID的输出使能、禁止	Tag：CID015
int CodabarCodeID_Set(sc_param_st *pParam);
int CodabarCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Code93
****************************************************************************/
//加载Code93的默认设置	Tag:C93DEF
int Code93LoadDefault_Set(sc_param_st *pParam);
int Code93LoadDefault_Get(sc_param_st *pParam);

//设置Code93的使能、禁止	Tag：C93ENA
int Code93Enable_Set(sc_param_st *pParam);
int Code93Enable_Get(sc_param_st *pParam);

//设置Code93的识读最小长度	Tag：C93MIN
int Code93MinLen_Set(sc_param_st *pParam);
int Code93MinLen_Get(sc_param_st *pParam);

//设置Code93的识读最大长度	Tag：C93MAX
int Code93MaxLen_Set(sc_param_st *pParam);
int Code93MaxLen_Get(sc_param_st *pParam);

//设置Code93校验位的使能、禁止	Tag：C93CHK
int Code93ChkTrsmt_Set(sc_param_st *pParam);
int Code93ChkTrsmt_Get(sc_param_st *pParam);

//设置Code93 CODEID的输出使能、禁止	Tag：CID017
int Code93CodeID_Set(sc_param_st *pParam);
int Code93CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@GS1-128(UCC/EAN-128)
****************************************************************************/
//加载GS1-128的默认设置	Tag:GS1DEF
int GS1_128LoadDefault_Set(sc_param_st *pParam);
int GS1_128LoadDefault_Get(sc_param_st *pParam);

//设置GS1-128的使能、禁止	Tag：GS1ENA
int GS1_128Enable_Set(sc_param_st *pParam);
int GS1_128Enable_Get(sc_param_st *pParam);

//设置GS1-128的识读最小长度	Tag：GS1MIN
int GS1_128MinLen_Set(sc_param_st *pParam);
int GS1_128MinLen_Get(sc_param_st *pParam);

//设置GS1-128的识读最大长度	Tag：GS1MAX
int GS1_128MaxLen_Set(sc_param_st *pParam);
int GS1_128MaxLen_Get(sc_param_st *pParam);

//设置GS1-128校验位的使能、禁止	Tag：GS1CHK
int GS1_128ChkTrsmt_Set(sc_param_st *pParam);
int GS1_128ChkTrsmt_Get(sc_param_st *pParam);

//设置GS1-128的FNC1输出的使能、禁止	Tag：GS1FNC
int GS1_128OutputFNC1_Set(sc_param_st *pParam);
int GS1_128OutputFNC1_Get(sc_param_st *pParam);

//设置GS1-128 CODEID的输出使能、禁止	Tag：CID003
int GS1_128CodeID_Set(sc_param_st *pParam);
int GS1_128CodeID_Get(sc_param_st *pParam);

int QueryBuffSizeOfInstructions_Set(sc_param_st *pParam);
/****************************************************************************
*@GS1 DataBar(RSS)
****************************************************************************/
//加载GS1 DataBar(RSS)的默认设置	Tag:RSSDEF
int RSSLoadDefault_Set(sc_param_st *pParam);
int RSSLoadDefault_Get(sc_param_st *pParam);

//设置GS1 DataBar(RSS)的使能、禁止	Tag:RSSENA
int RSSEnable_Set(sc_param_st *pParam);
int RSSEnable_Get(sc_param_st *pParam);

//设置GS1 DataBar(RSS)传送AI(01)字符的使能、禁止 Tag:RSSTAI
int RSSTrsmtAIEnable_Set(sc_param_st *pParam);
int RSSTrsmtAIEnable_Get(sc_param_st *pParam);

//设置GS1 DataBar(RSS) GS1 DataBar Omnidirectional字符的使能、禁止 Tag:RSSE14
int RSSConvert2RSS14_Set(sc_param_st *pParam);
int RSSConvert2RSS14_Get(sc_param_st *pParam);

//设置GS1 DataBar(RSS)的GS1 DataBar Limited使能、禁止 Tag:RSSENL
int RSSForGS1DatabarLimited_Set(sc_param_st *pParam);
int RSSForGS1DatabarLimited_Get(sc_param_st *pParam);

//设置GS1 DataBar(RSS)的GS1 DataBar Limited使能、禁止 Tag:RSSENE
int RSSForGS1DatabarExpand_Set(sc_param_st *pParam);
int RSSForGS1DatabarExpand_Get(sc_param_st *pParam);

//设置GS1 DataBar(RSS) CODEID的输出使能、禁止	Tag：CID031
int RSSCodeID_Set(sc_param_st *pParam);
int RSSCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@GS1 Composite
****************************************************************************/
//加载GS1 Composite的默认设置	Tag:CPTDEF
int GS1_CompositeLoadDefault_Set(sc_param_st *pParam);
int GS1_CompositeLoadDefault_Get(sc_param_st *pParam);

//设置GS1 Composite的使能、禁止	Tag:CPTENA
int GS1_CompositeEnable_Set(sc_param_st *pParam);
int GS1_CompositeEnable_Get(sc_param_st *pParam);

//设置GS1 Composite的UPC/EAN版本的使能、禁止 Tag:CPTUPC
int GS1_CompositeForUPC_EAN_Set(sc_param_st *pParam);
int GS1_CompositeForUPC_EAN_Get(sc_param_st *pParam);

//设置GS1 Composite CODEID的输出使能、禁止	Tag：CID030
int GS1_CompositeCodeID_Set(sc_param_st *pParam);
int GS1_CompositeCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Code11
****************************************************************************/
//加载Code11的默认设置		Tag:C11DEF
int Code11LoadDefault_Set(sc_param_st *pParam);
int Code11LoadDefault_Get(sc_param_st *pParam);

//设置Code11的使能、禁止	Tag：C11ENA
int Code11Enable_Set(sc_param_st *pParam);
int Code11Enable_Get(sc_param_st *pParam);

//设置Code11的识读最小长度	Tag：C11MIN
int Code11MinLen_Set(sc_param_st *pParam);
int Code11MinLen_Get(sc_param_st *pParam);

//设置Code11的识读最大长度	Tag：C11MAX
int Code11MaxLen_Set(sc_param_st *pParam);
int Code11MaxLen_Get(sc_param_st *pParam);

//设置Code11校验位的使能、禁止及校验位的选择	Tag：C11CHK
int Code11ChkTrsmt_Set(sc_param_st *pParam);
int Code11ChkTrsmt_Get(sc_param_st *pParam);

//设置Code11校验位是否传送	Tag：C11TCK
int Code11ChkTrsmtAction_Set(sc_param_st *pParam);
int Code11ChkTrsmtAction_Get(sc_param_st *pParam);


//设置Code11 CODEID的输出使能、禁止	Tag：CID028
int Code11CodeID_Set(sc_param_st *pParam);
int Code11CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@ISBN
****************************************************************************/
//加载ISBN的默认设置	Tag:ISBDEF
int ISBNLoadDefault_Set(sc_param_st *pParam);
int ISBNLoadDefault_Get(sc_param_st *pParam);

//设置ISBN的使能、禁止	Tag：ISBENA
int ISBNEnable_Set(sc_param_st *pParam);
int ISBNEnable_Get(sc_param_st *pParam);

//设置ISBN的发送数字个数（10个或者13个）	Tag：ISBT10
int ISBNLenOfNUM_Set(sc_param_st *pParam);
int ISBNLenOfNUM_Get(sc_param_st *pParam);

//设置ISBN 2位扩展码的识读使能、禁止	Tag:ISBAD2
int ISBNAddCode2Enable_Set(sc_param_st *pParam);
int ISBNAddCode2Enable_Get(sc_param_st *pParam);

//设置ISBN 5位扩展码的识读使能、禁止	Tag:ISBAD5
int ISBNAddCode5Enable_Set(sc_param_st *pParam);
int ISBNAddCode5Enable_Get(sc_param_st *pParam);

//设置ISBN拓展码的使能、禁止	Tag:ISBNREQ
int ISBNAddedCodeEnable_Set(sc_param_st *pParam);
int ISBNAddedCodeEnable_Get(sc_param_st *pParam);

//设置ISBN扩展码是否添加分隔符	Tag:ISBSEP
int ISBNSEPRInAddedCodeEnable_Set(sc_param_st *pParam);
int ISBNSEPRInAddedCodeEnable_Get(sc_param_st *pParam);

//设置ISBN CODEID的输出使能、禁止	Tag：CID024
int ISBNCodeID_Set(sc_param_st *pParam);
int ISBNCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Industrial 25
****************************************************************************/
//加载Industrial 25的默认设置		Tag:L25DEF
int IND25LoadDefault_Set(sc_param_st *pParam);
int IND25LoadDefault_Get(sc_param_st *pParam);

//设置Industrial 25的使能、禁止	Tag：L25ENA
int IND25Enable_Set(sc_param_st *pParam);
int IND25Enable_Get(sc_param_st *pParam);

//设置Industrial 25的识读最小长度	Tag：L25MIN
int IND25MinLen_Set(sc_param_st *pParam);
int IND25MinLen_Get(sc_param_st *pParam);

//设置Industrial 25的识读最大长度	Tag：L25MAX
int IND25MaxLen_Set(sc_param_st *pParam);
int IND25MaxLen_Get(sc_param_st *pParam);

//设置Industrial 25校验位的使能、禁止及校验位的选择	Tag：L25CHK
int IND25ChkTrsmt_Set(sc_param_st *pParam);
int IND25ChkTrsmt_Get(sc_param_st *pParam);

//设置Industrial 25 CODEID的输出使能、禁止	Tag：CID025
int IND25CodeID_Set(sc_param_st *pParam);
int IND25CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Standard 25
****************************************************************************/
//加载Standard 25的默认设置		Tag:S25DEF
int STD25LoadDefault_Set(sc_param_st *pParam);
int STD25LoadDefault_Get(sc_param_st *pParam);

//设置Standard 25的使能、禁止	Tag：S25ENA
int STD25Enable_Set(sc_param_st *pParam);
int STD25Enable_Get(sc_param_st *pParam);

//设置Standard 25的识读最小长度	Tag：S25MIN
int STD25MinLen_Set(sc_param_st *pParam);
int STD25MinLen_Get(sc_param_st *pParam);

//设置Standard 25的识读最大长度	Tag：S25MAX
int STD25MaxLen_Set(sc_param_st *pParam);
int STD25MaxLen_Get(sc_param_st *pParam);

//设置Standard 25校验位的使能、禁止及校验位的选择	Tag：S25CHK
int STD25ChkTrsmt_Set(sc_param_st *pParam);
int STD25ChkTrsmt_Get(sc_param_st *pParam);

//设置Standard 25 CODEID的输出使能、禁止	Tag：CID026
int STD25CodeID_Set(sc_param_st *pParam);
int STD25CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Plessey
****************************************************************************/
//加载Plessey 的默认设置		Tag:PLYDEF
int PlesseyLoadDefault_Set(sc_param_st *pParam);
int PlesseyLoadDefault_Get(sc_param_st *pParam);

//设置Plessey 的使能、禁止	Tag：PLYENA
int PlesseyEnable_Set(sc_param_st *pParam);
int PlesseyEnable_Get(sc_param_st *pParam);

//设置Plessey 的识读最小长度	Tag：PLYMIN
int PlesseyMinLen_Set(sc_param_st *pParam);
int PlesseyMinLen_Get(sc_param_st *pParam);

//设置Plessey 的识读最大长度	Tag：PLYMAX
int PlesseyMaxLen_Set(sc_param_st *pParam);
int PlesseyMaxLen_Get(sc_param_st *pParam);

//设置Plessey 校验位的使能、禁止及校验位的传送	Tag：PLYCHK
int PlesseyChkTrsmt_Set(sc_param_st *pParam);
int PlesseyChkTrsmt_Get(sc_param_st *pParam);

//设置Plessey CODEID的输出使能、禁止	Tag：CID027
int PlesseyCodeID_Set(sc_param_st *pParam);
int PlesseyCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@MSI Plessey
****************************************************************************/
//加载MSI Plessey 的默认设置		Tag:MSIDEF
int MSIPlesseyLoadDefault_Set(sc_param_st *pParam);
int MSIPlesseyLoadDefault_Get(sc_param_st *pParam);

//设置MSI Plessey 的使能、禁止	Tag：MSIENA
int MSIPlesseyEnable_Set(sc_param_st *pParam);
int MSIPlesseyEnable_Get(sc_param_st *pParam);

//设置MSI Plessey 的识读最小长度	Tag：MSIMIN
int MSIPlesseyMinLen_Set(sc_param_st *pParam);
int MSIPlesseyMinLen_Get(sc_param_st *pParam);

//设置MSI Plessey 的识读最大长度	Tag：MSIMAX
int MSIPlesseyMaxLen_Set(sc_param_st *pParam);
int MSIPlesseyMaxLen_Get(sc_param_st *pParam);

//设置MSI Plessey 校验位的使能、禁止及校验位的选择	Tag：MSICHK
int MSIPlesseyChkTrsmt_Set(sc_param_st *pParam);
int MSIPlesseyChkTrsmt_Get(sc_param_st *pParam);

//设置MSI Plessey 校验位是否发送	Tag：MSITCK
int MSIPlesseyChkTrsmtAction_Set(sc_param_st *pParam);
int MSIPlesseyChkTrsmtAction_Get(sc_param_st *pParam);

//设置MSI Plessey CODEID的输出使能、禁止	Tag：CID029
int MSIPlesseyCodeID_Set(sc_param_st *pParam);
int MSIPlesseyCodeID_Get(sc_param_st *pParam);


/****************************************************************************
*@ISSN
****************************************************************************/
//加载ISSN的默认设置	Tag:ISSDEF
int ISSNLoadDefault_Set(sc_param_st *pParam);
int ISSNLoadDefault_Get(sc_param_st *pParam);

//设置ISSN的使能、禁止	Tag：ISSENA
int ISSNEnable_Set(sc_param_st *pParam);
int ISSNEnable_Get(sc_param_st *pParam);

//设置ISSN 2位扩展码的识读使能、禁止	Tag:ISSAD2
int ISSNAddCode2Enable_Set(sc_param_st *pParam);
int ISSNAddCode2Enable_Get(sc_param_st *pParam);

//设置ISSN 5位扩展码的识读使能、禁止	Tag:ISSAD5
int ISSNAddCode5Enable_Set(sc_param_st *pParam);
int ISSNAddCode5Enable_Get(sc_param_st *pParam);

//设置ISSN拓展码的使能、禁止	Tag:ISSREQ
int ISSNAddedCodeEnable_Set(sc_param_st *pParam);
int ISSNAddedCodeEnable_Get(sc_param_st *pParam);

//设置ISSN扩展码是否添加分隔符	Tag:ISSSEP
int ISSNSEPRInAddedCodeEnable_Set(sc_param_st *pParam);
int ISSNSEPRInAddedCodeEnable_Get(sc_param_st *pParam);

//设置ISSN CODEID的输出使能、禁止	Tag：CID023
int ISSNCodeID_Set(sc_param_st *pParam);
int ISSNCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@China Post 25
****************************************************************************/
//加载China Post 25的默认设置	Tag:CHPDEF
int CP25LoadDefault_Set(sc_param_st *pParam);
int CP25LoadDefault_Get(sc_param_st *pParam);

//设置China Post 25的使能、禁止	Tag：CHPENA
int CP25Enable_Set(sc_param_st *pParam);
int CP25Enable_Get(sc_param_st *pParam);

//设置China Post 25 的识读最小长度	Tag：CHPMIN
int CP25MinLen_Set(sc_param_st *pParam);
int CP25MinLen_Get(sc_param_st *pParam);

//设置China Post 25 的识读最大长度	Tag：CHPMAX
int CP25MaxLen_Set(sc_param_st *pParam);
int CP25MaxLen_Get(sc_param_st *pParam);

//设置China Post 25 校验位的使能、禁止	Tag：CHPCHK
int CP25ChkTrsmt_Set(sc_param_st *pParam);
int CP25ChkTrsmt_Get(sc_param_st *pParam);

//设置China Post 25 CODEID的输出使能、禁止	Tag：CID019
int CP25CodeID_Set(sc_param_st *pParam);
int CP25CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@AIM 128
****************************************************************************/
//加载AIM 128的默认设置	Tag:AIMDEF
int AIM128LoadDefault_Set(sc_param_st *pParam);
int AIM128LoadDefault_Get(sc_param_st *pParam);

//设置AIM 128的使能、禁止	Tag：AIMENA
int AIM128Enable_Set(sc_param_st *pParam);
int AIM128Enable_Get(sc_param_st *pParam);

//设置AIM 128的识读最小长度	Tag：AIMMIN
int AIM128MinLen_Set(sc_param_st *pParam);
int AIM128MinLen_Get(sc_param_st *pParam);

//设置AIM 128的识读最大长度	Tag：AIMMAX
int AIM128MaxLen_Set(sc_param_st *pParam);
int AIM128MaxLen_Get(sc_param_st *pParam);

//设置AIM 128校验位的发送使能、禁止	Tag：AIMCHK
int AIM128ChkTrsmt_Set(sc_param_st *pParam);
int AIM128ChkTrsmt_Get(sc_param_st *pParam);

//设置AIM 128 FNC1格式的使能、禁止	Tag：AIMFNC
int AIM128FNC1_Set(sc_param_st *pParam);
int AIM128FNC1_Get(sc_param_st *pParam);

//设置AIM 128 CODEID的输出使能、禁止	Tag：CID020
int AIM128CodeID_Set(sc_param_st *pParam);
int AIM128CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@ISBT 128
****************************************************************************/
//加载ISBT 128的默认设置	Tag:IBTDEF
int ISBT128LoadDefault_Set(sc_param_st *pParam);
int ISBT128LoadDefault_Get(sc_param_st *pParam);

//设置ISBT 128的使能、禁止	Tag：IBTENA
int ISBT128Enable_Set(sc_param_st *pParam);
int ISBT128Enable_Get(sc_param_st *pParam);

//设置ISBT 128 CODEID的输出使能、禁止	Tag：CID021
int ISBT128CodeID_Set(sc_param_st *pParam);
int ISBT128CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@COOP 25
****************************************************************************/
//加载COOP 25的默认设置	Tag:COPDEF
int COOP25LoadDefault_Set(sc_param_st *pParam);
int COOP25LoadDefault_Get(sc_param_st *pParam);

//设置COOP 25的使能、禁止	Tag：COPENA
int COOP25Enable_Set(sc_param_st *pParam);
int COOP25Enable_Get(sc_param_st *pParam);

//设置COOP 25的识读最小长度	Tag：COPMIN
int COOP25MinLen_Set(sc_param_st *pParam);
int COOP25MinLen_Get(sc_param_st *pParam);

//设置COOP 25的识读最大长度	Tag：COPMAX
int COOP25MaxLen_Set(sc_param_st *pParam);
int COOP25MaxLen_Get(sc_param_st *pParam);

//设置COOP 25校验位的发送使能、禁止	Tag：COPCHK
int COOP25ChkTrsmt_Set(sc_param_st *pParam);
int COOP25ChkTrsmt_Get(sc_param_st *pParam);

//设置COOP 25 CODEID的输出使能、禁止	Tag：CID022
int COOP25CodeID_Set(sc_param_st *pParam);
int COOP25CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Deutsche 14
****************************************************************************/
//加载Deutsche 14的默认设置	Tag:D14DEF
int Deut14LoadDefault_Set(sc_param_st *pParam);
int Deut14LoadDefault_Get(sc_param_st *pParam);

//设置Deutsche 14的使能、禁止	Tag：D14ENA
int Deut14Enable_Set(sc_param_st *pParam);
int Deut14Enable_Get(sc_param_st *pParam);

//设置Deutsche 14校验位的发送使能、禁止	Tag：D14CHK
int Deut14ChkTrsmt_Set(sc_param_st *pParam);
int Deut14ChkTrsmt_Get(sc_param_st *pParam);

//设置Deutsche 14 CODEID的输出使能、禁止	Tag：CID128
int Deut14CodeID_Set(sc_param_st *pParam);
int Deut14CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Deutsche 12
****************************************************************************/
//加载Deutsche 12的默认设置	Tag:D12DEF
int Deut12LoadDefault_Set(sc_param_st *pParam);
int Deut12LoadDefault_Get(sc_param_st *pParam);

//设置Deutsche 12的使能、禁止	Tag：D12ENA
int Deut12Enable_Set(sc_param_st *pParam);
int Deut12Enable_Get(sc_param_st *pParam);

//设置Deutsche 12校验位的发送使能、禁止	Tag：D12CHK
int Deut12ChkTrsmt_Set(sc_param_st *pParam);
int Deut12ChkTrsmt_Get(sc_param_st *pParam);

//设置Deutsche 12 CODEID的输出使能、禁止	Tag：CID129
int Deut12CodeID_Set(sc_param_st *pParam);
int Deut12CodeID_Get(sc_param_st *pParam);


/****************************************************************************
*@Telepen
****************************************************************************/
//加载Telepen 的默认设置	Tag:TLPDEF
int TelepenLoadDefault_Set(sc_param_st *pParam);
int TelepenLoadDefault_Get(sc_param_st *pParam);

//设置Telepen 的使能、禁止	Tag：TLPENA
int TelepenEnable_Set(sc_param_st *pParam);
int TelepenEnable_Get(sc_param_st *pParam);

//设置Telepen 的识读最小长度	Tag：TLPMIN
int TelepenMinLen_Set(sc_param_st *pParam);
int TelepenMinLen_Get(sc_param_st *pParam);

//设置Telepen 的识读最大长度	Tag：TLPMAX
int TelepenMaxLen_Set(sc_param_st *pParam);
int TelepenMaxLen_Get(sc_param_st *pParam);

//设置Telepen 校验位的发送使能、禁止	Tag：TLPCHK
int TelepenChkTrsmt_Set(sc_param_st *pParam);
int TelepenChkTrsmt_Get(sc_param_st *pParam);

//设置Telepen 的解译输出组合：Full ASCII与Numeric Only	Tag：TLPASC
int TelepenFullASCIIWithNumericOnly_Set(sc_param_st *pParam);
int TelepenFullASCIIWithNumericOnly_Get(sc_param_st *pParam);

//设置Telepen CODEID的输出使能、禁止	Tag：CID130
int TelepenCodeID_Set(sc_param_st *pParam);
int TelepenCodeID_Get(sc_param_st *pParam);

/***************************************************************************
*@宏条码
****************************************************************************/
//清空宏缓存数据
int MacroCacheCleared_Set(sc_param_st *pParam);

//设置条码信息输出的发送方式
int MacroOutputMode1_Set(sc_param_st *pParam);
int MacroOutputMode2_Set(sc_param_st *pParam);
int MacroOutputMode3_Set(sc_param_st *pParam);
int MacroCacheCleared_Get(sc_param_st *pParam);

//设置条码信息输出的发送方式
int MacroOutputMode1_Get(sc_param_st *pParam);
int MacroOutputMode2_Get(sc_param_st *pParam);
int MacroOutputMode3_Get(sc_param_st *pParam);
/****************************************************************************
*@PDF417
****************************************************************************/
//加载PDF417 的默认设置	Tag:PDFDEF
int PDF417LoadDefault_Set(sc_param_st *pParam);
int PDF417LoadDefault_Get(sc_param_st *pParam);

//设置PDF417 的使能、禁止	Tag：PDFENA
int PDF417Enable_Set(sc_param_st *pParam);
int PDF417Enable_Get(sc_param_st *pParam);

//设置PDF417 的识读最小长度	Tag：PDFMIN
int PDF417MinLen_Set(sc_param_st *pParam);
int PDF417MinLen_Get(sc_param_st *pParam);

//设置PDF417 的识读最大长度	Tag：PDFMAX
int PDF417MaxLen_Set(sc_param_st *pParam);
int PDF417MaxLen_Get(sc_param_st *pParam);

//设置PDF417 的单双码功能	Tag：PDFDOU
int PDF417DecoMultiCode_Set(sc_param_st *pParam);
int PDF417DecoMultiCode_Get(sc_param_st *pParam);

//设置PDF417 的单双码输出方式	Tag：PDFDOF
int PDF417OrderOfMultiCode_Set(sc_param_st *pParam);
int PDF417OrderOfMultiCode_Get(sc_param_st *pParam);

//设置PDF417 的正反相条码识读的使能、禁止	Tag：PDFINV
int PDF417PhaseOfCode_Set(sc_param_st *pParam);
int PDF417PhaseOfCode_Get(sc_param_st *pParam);

//设置PDF417 的ECI输出使能、禁止	Tag：PDFECI
int PDF417OutputECI_Set(sc_param_st *pParam);
int PDF417OutputECI_Get(sc_param_st *pParam);

//设置PDF417 的字符编码方式	Tag：PDFENC
int PDF417EncodingOfChar_Set(sc_param_st *pParam);
int PDF417EncodingOfChar_Get(sc_param_st *pParam);

//设置PDF417 CODEID的输出使能、禁止	Tag：CID032
int PDF417CodeID_Set(sc_param_st *pParam);
int PDF417CodeID_Get(sc_param_st *pParam);


/****************************************************************************
*@QR
****************************************************************************/
//加载QR 的默认设置	Tag:QRCDEF
int QRLoadDefault_Set(sc_param_st *pParam);
int QRLoadDefault_Get(sc_param_st *pParam);

//设置QR 的使能、禁止	Tag：QRCENA
int QREnable_Set(sc_param_st *pParam);
int QREnable_Get(sc_param_st *pParam);

//设置QR 的识读最小长度	Tag：QRCMIN
int QRMinLen_Set(sc_param_st *pParam);
int QRMinLen_Get(sc_param_st *pParam);

//设置QR 的识读最大长度	Tag：QRCMAX
int QRMaxLen_Set(sc_param_st *pParam);
int QRMaxLen_Get(sc_param_st *pParam);

//设置QR 的单双码功能	Tag：QRCDOU
int QRDecoMultiCode_Set(sc_param_st *pParam);
int QRDecoMultiCode_Get(sc_param_st *pParam);

//设置QR 的单双码输出方式	Tag：QRCDOF
int QROrderOfMultiCode_Set(sc_param_st *pParam);
int QROrderOfMultiCode_Get(sc_param_st *pParam);

//设置QR 的Micro QR识读使能、禁止	Tag：QRCMCR
int QRDecoMicroQR_Set(sc_param_st *pParam);
int QRDecoMicroQR_Get(sc_param_st *pParam);

//设置QR 的正反相条码识读的使能、禁止	Tag：QRCINV
int QRPhaseOfCode_Set(sc_param_st *pParam);
int QRPhaseOfCode_Get(sc_param_st *pParam);

//设置QR 的ECI输出使能、禁止	Tag：QRCECI
int QROutputECI_Set(sc_param_st *pParam);
int QROutputECI_Get(sc_param_st *pParam);

//设置QR 的字符编码方式	Tag：QRCENC
int QREncodingOfChar_Set(sc_param_st *pParam);
int QREncodingOfChar_Get(sc_param_st *pParam);

//设置QR CODEID的输出使能、禁止	Tag：CID033
int QRCodeID_Set(sc_param_st *pParam);
int QRCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Aztec
****************************************************************************/
//加载Aztec 的默认设置	Tag:AZTDEF
int AztecLoadDefault_Set(sc_param_st *pParam);
int AztecLoadDefault_Get(sc_param_st *pParam);

//设置Aztec 的使能、禁止	Tag：AZTENA
int AztecEnable_Set(sc_param_st *pParam);
int AztecEnable_Get(sc_param_st *pParam);

//设置Aztec 的识读最小长度	Tag：AZTMIN
int AztecMinLen_Set(sc_param_st *pParam);
int AztecMinLen_Get(sc_param_st *pParam);

//设置Aztec 的识读最大长度	Tag：AZTMAX
int AztecMaxLen_Set(sc_param_st *pParam);
int AztecMaxLen_Get(sc_param_st *pParam);

//设置Aztec 的多码同图的条码个数	Tag：AZTMUL
int AztecNumOfMultiCode_Set(sc_param_st *pParam);
int AztecNumOfMultiCode_Get(sc_param_st *pParam);

//设置Aztec 的多码同图的处理方式Tag：AZTMAX
int AztecModeOfHandleMultiCode_Set(sc_param_st *pParam);
int AztecModeOfHandleMultiCode_Get(sc_param_st *pParam);

//设置Aztec 的ECI输出使能、禁止	Tag：AZTECI
int AztecOutputECI_Set(sc_param_st *pParam);
int AztecOutputECI_Get(sc_param_st *pParam);

//设置Aztec 的字符编码方式	Tag：AZTENC
int AztecEncodingOfChar_Set(sc_param_st *pParam);
int AztecEncodingOfChar_Get(sc_param_st *pParam);

//设置Aztec CODEID的输出使能、禁止	Tag：CID034
int AztecCodeID_Set(sc_param_st *pParam);
int AztecCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Data Matrix
****************************************************************************/
//加载DM 的默认设置	Tag:DMCDEF
int DMLoadDefault_Set(sc_param_st *pParam);
int DMLoadDefault_Get(sc_param_st *pParam);

//设置DM 的使能、禁止	Tag：DMCENA
int DMEnable_Set(sc_param_st *pParam);
int DMEnable_Get(sc_param_st *pParam);

//设置DM 的识读最小长度	Tag：DMCMIN
int DMMinLen_Set(sc_param_st *pParam);
int DMMinLen_Get(sc_param_st *pParam);

//设置DM 的识读最大长度	Tag：DMCMAX
int DMMaxLen_Set(sc_param_st *pParam);
int DMMaxLen_Get(sc_param_st *pParam);

//设置DM 的单双码功能	Tag：DMCDOU
int DMDecoMultiCode_Set(sc_param_st *pParam);
int DMDecoMultiCode_Get(sc_param_st *pParam);

//设置DM 的识读矩形码功能	Tag：DMCREC
int DMDecoRectCode_Set(sc_param_st *pParam);
int DMDecoRectCode_Get(sc_param_st *pParam);

//设置DM 解码后是否添加信息码词	Tag：DMCADD
int DMAddInfoWordsToDecoResult_Set(sc_param_st *pParam);
int DMAddInfoWordsToDecoResult_Get(sc_param_st *pParam);

//设置DM 的正反相条码识读的使能、禁止	Tag：DMCINV
int DMPhaseOfCode_Set(sc_param_st *pParam);
int DMPhaseOfCode_Get(sc_param_st *pParam);

//设置DM 的ECI输出使能、禁止	Tag：DMCECI
int DMOutputECI_Set(sc_param_st *pParam);
int DMOutputECI_Get(sc_param_st *pParam);

//设置DM 的字符编码方式	Tag：DMCENC
int DMEncodingOfChar_Set(sc_param_st *pParam);
int DMEncodingOfChar_Get(sc_param_st *pParam);

//设置DM CODEID的输出使能、禁止	Tag：CID035
int DMCodeID_Set(sc_param_st *pParam);
int DMCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Maxicode
****************************************************************************/
//加载Maxicode 的默认设置	Tag:MXCDEF
int MaxiLoadDefault_Set(sc_param_st *pParam);
int MaxiLoadDefault_Get(sc_param_st *pParam);

//设置Maxicode 的使能、禁止	Tag：MXCENA
int MaxiEnable_Set(sc_param_st *pParam);
int MaxiEnable_Get(sc_param_st *pParam);

//设置Maxicode 的识读最小长度	Tag：MXCMIN
int MaxiMinLen_Set(sc_param_st *pParam);
int MaxiMinLen_Get(sc_param_st *pParam);

//设置Maxicode 的识读最大长度	Tag：MXCMAX
int MaxiMaxLen_Set(sc_param_st *pParam);
int MaxiMaxLen_Get(sc_param_st *pParam);

//设置Maxicode CODEID的输出使能、禁止	Tag：CID036
int MaxiCodeID_Set(sc_param_st *pParam);
int MaxiCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@汉信码 CSC
****************************************************************************/
//加载CSC 的默认设置	Tag:CSCDEF
int CSCLoadDefault_Set(sc_param_st *pParam);
int CSCLoadDefault_Get(sc_param_st *pParam);

//设置CSC 的使能、禁止	Tag：CSCENA
int CSCEnable_Set(sc_param_st *pParam);
int CSCEnable_Get(sc_param_st *pParam);

//设置CSC 的识读最小长度	Tag：CSCMIN
int CSCMinLen_Set(sc_param_st *pParam);
int CSCMinLen_Get(sc_param_st *pParam);

//设置CSC 的识读最大长度	Tag：CSCMAX
int CSCMaxLen_Set(sc_param_st *pParam);
int CSCMaxLen_Get(sc_param_st *pParam);

//设置CSC 的单双码功能	Tag：CSCDOU
int CSCDecoMultiCode_Set(sc_param_st *pParam);
int CSCDecoMultiCode_Get(sc_param_st *pParam);

//设置CSC 的正反相条码识读的使能、禁止	Tag：CSCINV
int CSCPhaseOfCode_Set(sc_param_st *pParam);
int CSCPhaseOfCode_Get(sc_param_st *pParam);

//设置CSC CODEID的输出使能、禁止	Tag：CID039
int CSCCodeID_Set(sc_param_st *pParam);
int CSCCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@GM码
****************************************************************************/
//加载GM 的默认设置	Tag:GMCDEF
int GMLoadDefault_Set(sc_param_st *pParam);
int GMLoadDefault_Get(sc_param_st *pParam);

//设置GM 的使能、禁止	Tag：GMCENA
int GMEnable_Set(sc_param_st *pParam);
int GMEnable_Get(sc_param_st *pParam);

//设置GM 的识读最小长度	Tag：GMCMIN
int GMMinLen_Set(sc_param_st *pParam);
int GMMinLen_Get(sc_param_st *pParam);

//设置GM 的识读最大长度	Tag：GMCMAX
int GMMaxLen_Set(sc_param_st *pParam);
int GMMaxLen_Get(sc_param_st *pParam);

//设置GM CODEID的输出使能、禁止	Tag：CID041
int GMCodeID_Set(sc_param_st *pParam);
int GMCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@MicroPDF417码
****************************************************************************/
//加载MicroPDF417 的默认设置	Tag:MPDDEF
int MicroPDF417LoadDefault_Set(sc_param_st *pParam);
int MicroPDF417LoadDefault_Get(sc_param_st *pParam);

//设置MicroPDF417 的使能、禁止	Tag：MPDENA
int MicroPDF417Enable_Set(sc_param_st *pParam);
int MicroPDF417Enable_Get(sc_param_st *pParam);

//设置MicroPDF417 的识读最小长度	Tag：MPDMIN
int MicroPDF417MinLen_Set(sc_param_st *pParam);
int MicroPDF417MinLen_Get(sc_param_st *pParam);

//设置MicroPDF417 的识读最大长度	Tag：MPDMAX
int MicroPDF417MaxLen_Set(sc_param_st *pParam);
int MicroPDF417MaxLen_Get(sc_param_st *pParam);

//设置MicroPDF417 CODEID的输出使能、禁止	Tag：CID042
int MicroPDF417CodeID_Set(sc_param_st *pParam);
int MicroPDF417CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@MicroQR码
****************************************************************************/
//加载MicroQR 的默认设置	Tag:MQRDEF
int MicroQRLoadDefault_Set(sc_param_st *pParam);
int MicroQRLoadDefault_Get(sc_param_st *pParam);

//设置MicroQR 的使能、禁止	Tag：MQRENA
int MicroQREnable_Set(sc_param_st *pParam);
int MicroQREnable_Get(sc_param_st *pParam);

//设置MicroQR 的识读最小长度	Tag：MQRMIN
int MicroQRMinLen_Set(sc_param_st *pParam);
int MicroQRMinLen_Get(sc_param_st *pParam);

//设置MicroQR 的识读最大长度	Tag：MQRMAX
int MicroQRMaxLen_Set(sc_param_st *pParam);
int MicroQRMaxLen_Get(sc_param_st *pParam);

//设置MicroQR CODEID的输出使能、禁止	Tag：CID043
int MicroQRCodeID_Set(sc_param_st *pParam);
int MicroQRCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Specific OCR-B
****************************************************************************/
//加载Specific OCR-B 的默认设置	Tag:SOBDEF
int SpecOCRBLoadDefault_Set(sc_param_st *pParam);
int SpecOCRBLoadDefault_Get(sc_param_st *pParam);

//设置Specific OCR-B 的使能、禁止	Tag：SOBENA
int SpecOCRBEnable_Set(sc_param_st *pParam);
int SpecOCRBEnable_Get(sc_param_st *pParam);

//设置Specific OCR-B 的识读最小长度	Tag：SOBMIN
int SpecOCRBMinLen_Set(sc_param_st *pParam);
int SpecOCRBMinLen_Get(sc_param_st *pParam);

//设置Specific OCR-B 的识读最大长度	Tag：SOBMAX
int SpecOCRBMaxLen_Set(sc_param_st *pParam);
int SpecOCRBMaxLen_Get(sc_param_st *pParam);

/*Code ID 固定 暂时不提供接口
//设置Specific OCR-B CODEID的输出使能、禁止	Tag：CID043
int SpecOCRBCodeID_Set(sc_param_st *pParam);
int SpecOCRBCodeID_Get(sc_param_st *pParam);
*/

/****************************************************************************
*@Passport OCR
****************************************************************************/
//加载Passport OCR 的默认设置	Tag:PASDEF
int PassportOCRBLoadDefault_Set(sc_param_st *pParam);
int PassportOCRBLoadDefault_Get(sc_param_st *pParam);

//设置Passport OCR 的使能、禁止	Tag：PASENA
int PassportOCRBEnable_Set(sc_param_st *pParam);
int PassportOCRBEnable_Get(sc_param_st *pParam);

/*Code ID 固定 暂时不提供接口
//设置Passport OCR CODEID的输出使能、禁止	Tag：
int SpecOCRBCodeID_Set(sc_param_st *pParam);
int SpecOCRBCodeID_Get(sc_param_st *pParam);
*/

/****************************************************************************
*@USPS Postnet
****************************************************************************/
//加载USPS Postnet 的默认设置	Tag:PNTDEF
int PostnetLoadDefault_Set(sc_param_st *pParam);
int PostnetLoadDefault_Get(sc_param_st *pParam);

//设置USPS Postnet 的使能、禁止	Tag：PNTENA
int PostnetEnable_Set(sc_param_st *pParam);
int PostnetEnable_Get(sc_param_st *pParam);

//设置USPS Postnet 校验位的发送使能、禁止	Tag：PNTCHK
int PostnetChkTrsmt_Set(sc_param_st *pParam);
int PostnetChkTrsmt_Get(sc_param_st *pParam);

//设置USPS Postnet CODEID的输出使能、禁止	Tag：CID096
int PostnetCodeID_Set(sc_param_st *pParam);
int PostnetCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@USPS Intelligent Mail
****************************************************************************/
//加载USPS Intelligent Mail 的默认设置	Tag:ILGDEF
int InteMailLoadDefault_Set(sc_param_st *pParam);
int InteMailLoadDefault_Get(sc_param_st *pParam);

//设置USPS Intelligent Mail 的使能、禁止	Tag：ILGENA
int InteMailEnable_Set(sc_param_st *pParam);
int InteMailEnable_Get(sc_param_st *pParam);

//设置USPS Intelligent Mail CODEID的输出使能、禁止	Tag：CID097
int InteMailCodeID_Set(sc_param_st *pParam);
int InteMailCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Royal Mail
****************************************************************************/
//加载Royal Mail 的默认设置	Tag:ROYDEF
int RoyalMailLoadDefault_Set(sc_param_st *pParam);
int RoyalMailLoadDefault_Get(sc_param_st *pParam);

//设置Royal Mail 的使能、禁止	Tag：ROYENA
int RoyalMailEnable_Set(sc_param_st *pParam);
int RoyalMailEnable_Get(sc_param_st *pParam);

//设置Royal Mail 的输出使能、禁止	Tag：CID098
int RoyalMailCodeID_Set(sc_param_st *pParam);
int RoyalMailCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@USPS Planet
****************************************************************************/
//加载USPS Planet 的默认设置	Tag:PLADEF
int PlanetLoadDefault_Set(sc_param_st *pParam);
int PlanetLoadDefault_Get(sc_param_st *pParam);

//设置USPS Planet 的使能、禁止	Tag：PLAENA
int PlanetEnable_Set(sc_param_st *pParam);
int PlanetEnable_Get(sc_param_st *pParam);

//设置USPS Planet 校验位的发送使能、禁止	Tag：PLACHK
int PlanetChkTrsmt_Set(sc_param_st *pParam);
int PlanetChkTrsmt_Get(sc_param_st *pParam);

//设置USPS Planet CODEID的输出使能、禁止	Tag：CID099
int PlanetCodeID_Set(sc_param_st *pParam);
int PlanetCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@KIX Post
****************************************************************************/
//加载KIX Post 的默认设置	Tag:KIXDEF
int KIXPostLoadDefault_Set(sc_param_st *pParam);
int KIXPostLoadDefault_Get(sc_param_st *pParam);

//设置KIX Post 的使能、禁止	Tag：KIXENA
int KIXPostEnable_Set(sc_param_st *pParam);
int KIXPostEnable_Get(sc_param_st *pParam);

//设置KIX Post 的输出使能、禁止	Tag：CID100
int KIXPostCodeID_Set(sc_param_st *pParam);
int KIXPostCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Australian Postal
****************************************************************************/
//加载Australian Postal 的默认设置	Tag:APLDEF
int AUSPostalLoadDefault_Set(sc_param_st *pParam);
int AUSPostalLoadDefault_Get(sc_param_st *pParam);

//设置Australian Postal 的使能、禁止	Tag：APLENA
int AUSPostalEnable_Set(sc_param_st *pParam);
int AUSPostalEnable_Get(sc_param_st *pParam);

//设置Australian Postal 的输出使能、禁止	Tag：CID101
int AUSPostalCodeID_Set(sc_param_st *pParam);
int AUSPostalCodeID_Get(sc_param_st *pParam);

//前后缀等附加信息综合设置 APSENA
int AllFixMsg_Set(sc_param_st *pParam);
int AllFixMsg_Get(sc_param_st *pParam);

//前缀顺序设置  PRESEQ
int PrefixOrder_Set(sc_param_st *pParam);
int PrefixOrder_Get(sc_param_st *pParam);

//前缀使能设置
int PrefixEnable_Set(sc_param_st *pParam);
int PrefixEnable_Get(sc_param_st *pParam);

//前缀信息编辑
int PrefixEdit_Set(sc_param_st *pParam);
int PrefixEdit_Get(sc_param_st *pParam);
int PrefixLenth_Set(sc_param_st *pParam);
int PrefixLenth_Get(sc_param_st *pParam);


//AIMID使能设置
int AIMIDEnable_Set(sc_param_st *pParam);
int AIMIDEnable_Get(sc_param_st *pParam);

//CodeID综合设置
int CodeIDEnable_Set(sc_param_st *pParam);
int CodeIDEnable_Get(sc_param_st *pParam);
int AllCodeIDDefault_Set(sc_param_st *pParam);
int AllCodeIDDefault_Get(sc_param_st *pParam);
int CodeIDLenth_Set(sc_param_st *pParam);
int CodeIDLenth_Get(sc_param_st *pParam);
int CodeIDValueRange_Set(sc_param_st *pParam);
int CodeIDValueRange_Get(sc_param_st *pParam);

//后缀使能设置
int SufixEnable_Set(sc_param_st *pParam);
int SufixEnable_Get(sc_param_st *pParam);

//后缀信息编辑
int SufixEdit_Set(sc_param_st *pParam);
int SufixEdit_Get(sc_param_st *pParam);
int SufixLenth_Set(sc_param_st *pParam);
int SufixLenth_Get(sc_param_st *pParam);

//结束符使能设置
int TerminatEnable_Set(sc_param_st *pParam);
int TerminatEnable_Get(sc_param_st *pParam);

//结束符信息编辑
int TerminatEdit_Set(sc_param_st *pParam);
int TerminatEdit_Get(sc_param_st *pParam);
int TerminatLenth_Set(sc_param_st *pParam);
int TerminatLenth_Get(sc_param_st *pParam);


/****************************************************************************
*@Data Formatting 数据格式编辑
****************************************************************************/
//DataFormatter 的使能设置		Tag：DFMENA
int DataFormatterEnable_Set(sc_param_st *pParam);
int DataFormatterEnable_Get(sc_param_st *pParam);

//加载DataFormatter的默认数据格式 		Tag：DFMDEF
int LoadDefaultDataFormatType_Set(sc_param_st *pParam);
int LoadDefaultDataFormatType_Get(sc_param_st *pParam);

//进入DataFormatter的数据格式编辑 		Tag：DFMSET
int DataFormatEditorEnable_Set(sc_param_st *pParam);
int DataFormatEditorEnable_Get(sc_param_st *pParam);

//清除某个DataFormatter数据格式类型 	Tag：DFMCLR
int ClearDataFormatTypes_Set(sc_param_st *pParam);
int ClearDataFormatTypes_Get(sc_param_st *pParam);

//清除所有DataFormatter数据格式类型 	Tag：DFMCAL
int ClearAllDataFormatTypes_Set(sc_param_st *pParam);
int ClearAllDataFormatTypes_Get(sc_param_st *pParam);

//加载选中的DataFormatter数据格式 		Tag：DFMUSE
int LoadDataFormatTypes_Set(sc_param_st *pParam);
int LoadDataFormatTypes_Get(sc_param_st *pParam);


//加载选中的单次读码DataFormatter数据格式 		Tag：DFMSIN
int LoadDataFormatTypesForSingleDeco_Set(sc_param_st *pParam);
int LoadDataFormatTypesForSingleDeco_Get(sc_param_st *pParam);

//数据格式不匹配的错误提示音		Tag：DFMTON
int BeepOnDataFormatNotMatchEnable_Set(sc_param_st *pParam);
int BeepOnDataFormatNotMatchEnable_Get(sc_param_st *pParam);

//查询当前的数据格式				Tag：DFMQCU
int CurrentDataFormatType_Set(sc_param_st *pParam);
int CurrentDataFormatType_Get(sc_param_st *pParam);

//查询出厂默认的数据格式			Tag：DFMQFA
int DefaultFactoryDataFormatType_Set(sc_param_st *pParam);
int DefaultFactoryDataFormatType_Get(sc_param_st *pParam);

//查询单条Data Format支持的最大长度	Tag：DFMQSZ
int MaxLenOfDataFormatTypes_Get(sc_param_st *pParam);
int MaxLenOfDataFormatTypes_Set(sc_param_st *pParam);
/****************************************************************************
*@驾照解析功能
****************************************************************************/
//启动驾照解析功能设置		Tag：DLPSET
int EnableAnalyzeDrivingLic_Set(sc_param_st *pParam);
int EnableAnalyzeDrivingLic_Get(sc_param_st *pParam);

//禁止驾照解析功能，并删除设置		Tag：DLPCLR
int DisableAnalyzeDrivingLic_Set(sc_param_st *pParam);

//查询驾照解析功能支持的指令最大长度		Tag：DLPQSZ
int MaxLenOfCommandInAnalDrivingLic_Get(sc_param_st *pParam);

/****************************************************************************
*@数据打包设置
****************************************************************************/
//设置数据打包	Tag：PACKAG
int PackageForDecoInfo_Set(sc_param_st *pParam);
int PackageForDecoInfo_Get(sc_param_st *pParam);

/****************************************************************************
*@信息截取设置
****************************************************************************/
//信息截取的使能设置	Tag：EXTRAC
int CuttingoutDecoInfoEnable_Set(sc_param_st *pParam);
int CuttingoutDecoInfoEnable_Get(sc_param_st *pParam);

//添加信息截取设置		Tag:EXTADD
int AddConfigForCuttingoutDecoInfo_Set(sc_param_st *pParam);
int AddConfigForCuttingoutDecoInfo_Get(sc_param_st *pParam);

//清除某种条码的截取设置		Tag:EXTCLR
int ClearCuttingoutForCodeTypes_Set(sc_param_st *pParam);
int ClearCuttingoutForCodeTypes_Get(sc_param_st *pParam);

//清除最新一组的截取设置		Tag:EXTCLO
int ClearLatestCFGOfCuttingout_Set(sc_param_st *pParam);
int ClearLatestCFGOfCuttingout_Get(sc_param_st *pParam);

//清除所有的截取设置		Tag:EXTCAL
int ClearAllCFGOfCuttingout_Set(sc_param_st *pParam);
int ClearAllCFGOfCuttingout_Get(sc_param_st *pParam);

/****************************************************************************
*@获取设备分辨率
****************************************************************************/
//设备分辨率的查询
int ResolutionRatio_Set(sc_param_st *pParam);
int ResolutionRatio_Get(sc_param_st *pParam);

/****************************************************************************
*@获取图像
****************************************************************************/
//获取图像
int FetchingImage_Set(sc_param_st *pParam);
int FetchingImage_Get(sc_param_st *pParam);

/****************************************************************************
*@产品信息
****************************************************************************/
//设置产品S/N号		Tag:PSNEDT
int SettingProductSN_Set(sc_param_st *pParam);
int SettingProductSN_Get(sc_param_st *pParam);

//删除产品S/N号		Tag:PSNDEL
int DeleteProductSN_Set(sc_param_st *pParam);
int DeleteProductSN_Get(sc_param_st *pParam);

//查询产品S/N号支持的长度 Tag:PSNQSZ
int ProductSNLength_Set(sc_param_st *pParam);
int ProductSNLength_Get(sc_param_st *pParam);

//设置产品ESN号		Tag:ESNSET
int SettingProductESN_Set(sc_param_st *pParam);
int SettingProductESN_Get(sc_param_st *pParam);

//删除产品ESN号		Tag:ESNDEL
int DeleteProductESN_Set(sc_param_st *pParam);
int DeleteProductESN_Get(sc_param_st *pParam);

//查询产品ESN号支持的长度 Tag:ESNQSZ
int ProductESNLength_Set(sc_param_st *pParam);
int ProductESNLength_Get(sc_param_st *pParam);

//设置产品生产日期		Tag:DATEDT
int SettingProductDate_Set(sc_param_st *pParam);
int SettingProductDate_Get(sc_param_st *pParam);

//删除产品生产日期		Tag:DATDEL
int DeleteProductDate_Set(sc_param_st *pParam);
int DeleteProductDate_Get(sc_param_st *pParam);

//查询产品生产日期支持的长度 Tag:DATQSZ
int ProductDateLength_Set(sc_param_st *pParam);
int ProductDateLength_Get(sc_param_st *pParam);

//设置产品名称		Tag:PDNEDT
int SettingProductName_Set(sc_param_st *pParam);
int SettingProductName_Get(sc_param_st *pParam);

//删除产品名称		Tag:PDNDEL
int DeleteProductName_Set(sc_param_st *pParam);
int DeleteProductName_Get(sc_param_st *pParam);

//查询产品名称支持的长度 Tag:PDNQSZ
int ProductNameLength_Set(sc_param_st *pParam);
int ProductNameLength_Get(sc_param_st *pParam);

//Is Download 使能设置
int IsDownloadEnable_Set(sc_param_st *pParam);
int IsDownloadEnable_Get(sc_param_st *pParam);

//删除IS Download设置
int DeleteCFGOfIsDownload_Set(sc_param_st *pParam);
int DeleteCFGOfIsDownload_Get(sc_param_st *pParam);

//设置制造商名称		Tag:MFNEDT
int SettingManufacturerName_Set(sc_param_st *pParam);
int SettingManufacturerName_Get(sc_param_st *pParam);

//删除制造商名称		Tag:MFNDEL
int DeleteManufacturerName_Set(sc_param_st *pParam);
int DeleteManufacturerName_Get(sc_param_st *pParam);

//查询产品制造商名称支持的长度 Tag:MFNQSZ
int ManufacturerNameLength_Set(sc_param_st *pParam);
int ManufacturerNameLength_Get(sc_param_st *pParam);

//设置产品硬件信息		Tag:HWVEDT
int SettingInfoOfHardware_Set(sc_param_st *pParam);
int SettingInfoOfHardware_Get(sc_param_st *pParam);

//删除产品硬件信息		Tag:HWVDEL
int DeleteInfoOfHardware_Set(sc_param_st *pParam);
int DeleteInfoOfHardware_Get(sc_param_st *pParam);

//查询产品硬件信息支持的长度 Tag:HWVQSZ
int InfoOfHardwareLength_Set(sc_param_st *pParam);
int InfoOfHardwareLength_Get(sc_param_st *pParam);

//设置产品真实名称		Tag:PRNEDT
int SettingRealNameOfProduct_Set(sc_param_st *pParam);
int SettingRealNameOfProduct_Get(sc_param_st *pParam);

//删除产品真实名称		Tag:PRNDEL
int DeleteRealNameOfProduct_Set(sc_param_st *pParam);
int DeleteRealNameOfProduct_Get(sc_param_st *pParam);

//查询产品真实名称支持的长度 Tag:PRNQSZ
int RealNameOfProductLength_Set(sc_param_st *pParam);
int RealNameOfProductLength_Get(sc_param_st *pParam);

//查询产品真实名称		Tag:PRQRY
int QueryRealNameOfProduct_Set(sc_param_st *pParam);
int QueryRealNameOfProduct_Get(sc_param_st *pParam);

/****************************************************************************
*@景深设置
****************************************************************************/
//近、中、远三种景深的设置			Tag:DOFLVL
int DepthOfFieldInProduct_Set(sc_param_st *pParam);
int DepthOfFieldInProduct_Get(sc_param_st *pParam);

//删除景深配置参数					Tag：DELDOF
int DelDepthOfFieldInProduct_Set(sc_param_st *pParam);
int DelDepthOfFieldInProduct_Get(sc_param_st *pParam);

/****************************************************************************
*@MCU运行频率		
****************************************************************************/
//设置MCU的运行频率		Tag：HWRFRQ
int RunningFreqOfMCU_Set(sc_param_st *pParam);
int RunningFreqOfMCU_Get(sc_param_st *pParam);

//运行无线模式		Tag：RUNWEL
int RunningWirelessMode_Set(sc_param_st *pParam);
int RunningWirelessMode_Get(sc_param_st *pParam);

//设置USB枚举PID--SN时是否使用设备SN	Tag：PIDUSN
int USBEnumratePIDSNWithSN_Set(sc_param_st *pParam);
int USBEnumratePIDSNWithSN_Get(sc_param_st *pParam);

//USB接口与串口的切换方式（按指定要求切换或自适应）	Tag：AUTOUR
int AutoAdaptBetweenSerialCommAndUSB_Set(sc_param_st *pParam);
int AutoAdaptBetweenSerialCommAndUSB_Get(sc_param_st *pParam);

//查询设备指令接受缓存大小		Tag：BUFQRY
int QueryBuffSizeOfInstructions_Set(sc_param_st *pParam);
int QueryBuffSizeOfInstructions_Get(sc_param_st *pParam);

//产品信息设置查询PSN
int ProductSNLength_Set(sc_param_st *pParam);
//int ProductSNLength_Get(sc_param_st *pParam);

//int SettingProductESN_Set(sc_param_st *pParam);
//int SettingProductESN_Get(sc_param_st *pParam);
//int DeleteProductESN_Set(sc_param_st *pParam);

int ProductESNLength_Set(sc_param_st *pParam);
//int ProductESNLength_Get(sc_param_st *pParam);
//int SettingProductDate_Set(sc_param_st *pParam);
//int SettingProductDate_Get(sc_param_st *pParam);
//int DeleteProductDate_Set(sc_param_st *pParam);
int ProductDateLength_Set(sc_param_st *pParam);
//int ProductDateLength_Get(sc_param_st *pParam);
//int SettingProductName_Set(sc_param_st *pParam);
//int SettingProductName_Get(sc_param_st *pParam);
//int DeleteProductName_Set(sc_param_st *pParam);
int ProductNameLength_Set(sc_param_st *pParam);
//int ProductNameLength_Get(sc_param_st *pParam);
//int SettingManufacturerName_Set(sc_param_st *pParam);
//int SettingManufacturerName_Get(sc_param_st *pParam);
//int DeleteManufacturerName_Set(sc_param_st *pParam);
int DeleteManufacturerName_Get(sc_param_st *pParam);
int ManufacturerNameLength_Set(sc_param_st *pParam);
//int ManufacturerNameLength_Get(sc_param_st *pParam);

int InfoOfHardwareLength_Set(sc_param_st *pParam);
int SettingRealNameOfProduct_Get(sc_param_st *pParam);
int RealNameOfProductLength_Set(sc_param_st *pParam);
int EnableSettingQRY_Set(sc_param_st *pParam);
int EnableSettingQRY_Get(sc_param_st *pParam);

//加密
int QueryCPUID_Set(sc_param_st *pParam);
int QueryCPUID_Get(sc_param_st *pParam);
int Encryption_Set(sc_param_st *pParam);
int Encryption_Get(sc_param_st *pParam);
/************************************************************************/
#ifdef __cplusplus
}
#endif
#endif //__SC_EXTFUNC_H__

