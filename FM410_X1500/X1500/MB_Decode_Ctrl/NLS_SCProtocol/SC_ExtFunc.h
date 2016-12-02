#ifndef __SC_EXTFUNC_H__
#define __SC_EXTFUNC_H__

#ifdef __cplusplus
extern"C" {
#endif

#define  SC_CFG_NSYS               0  //��ǰ����
#define  SC_CFG_SYS                1  //ϵͳ����
#define  SC_CFG_FACT               2  //��������
#define  SC_CFG_USER               3  //�û�����
/************************************************************************/
/****************************************************************************
*@��ƷĬ������
****************************************************************************/
//���س���Ĭ������	Tag:FACDEF
int LoadFactoryDefaultCFG_Set (sc_param_st *pParam);
int LoadFactoryDefaultCFG_Get (sc_param_st *pParam);

//�����û�Ĭ������	Tag:CUSDEF
int LoadUserDefaultCFG_Set(sc_param_st *pParam);
int LoadUserDefaultCFG_Get(sc_param_st *pParam);

//�����û�Ĭ������	Tag:CUSSAV
int SaveUserDefaultCFG_Set(sc_param_st *pParam);
int SaveUserDefaultCFG_Get(sc_param_st *pParam);

//ɾ���û�Ĭ������	Tag:CUSSAV
int DeleteUserDefaultCFG_Set(sc_param_st *pParam);
int DeleteUserDefaultCFG_Get(sc_param_st *pParam);

//�������Ĭ������	Tag:SAVEFY
int SaveFactoryDefaultCFG_Set(sc_param_st *pParam);
int SaveFactoryDefaultCFG_Get(sc_param_st *pParam);

//����ͨ��Ĭ������	Tag:GENDEF
int LoadGeneralDefaultCFG_Set(sc_param_st *pParam);
int LoadGeneralDefaultCFG_Get(sc_param_st *pParam);

/****************************************************************************
*@ͨѶ�ӿ�
****************************************************************************/
//���ڸ�ͨѶ�ӿ����͵��л�����	Tag:INF
int CommunicationInterface_Set(sc_param_st *pParam);
int CommunicationInterface_Get(sc_param_st *pParam);

//����RS232�����ʵ��л�	Tag:232BAD
int RS232Baudrate_Set(sc_param_st *pParam);
int RS232Baudrate_Get(sc_param_st *pParam);

//����RS232����żУ��	Tag:232PAR
int RS232Parity_Set(sc_param_st *pParam);
int RS232Parity_Get(sc_param_st *pParam);

//����RS232ֹͣλ������	Tag:232STP
int RS232StopBits_Set(sc_param_st *pParam);
int RS232StopBits_Get(sc_param_st *pParam);

//����RS232����λ������	Tag:232DAT
int RS232DataBits_Set(sc_param_st *pParam);
int RS232DataBits_Get(sc_param_st *pParam);

//����RS232Ӳ���Զ����ص�ʹ�ܺͽ�ֹ	Tag:232AFL
int RS232HardwereAutoFlowCTL_Set(sc_param_st *pParam);
int RS232HardwereAutoFlowCTL_Get(sc_param_st *pParam);

//����RS232���ص�����	Tag:232FLW
int RS232NoAutoFlowCTL_Set(sc_param_st *pParam);
int RS232NoAutoFlowCTL_Get(sc_param_st *pParam);

//����Keyboard Country Layout���������ԣ�������	Tag:KBWCTY
int KBWCountryLayout_Set(sc_param_st *pParam);
int KBWCountryLayout_Get(sc_param_st *pParam);

//���ڴ���δ֪�ַ�����ʾ��ʹ�ܡ���ֹ	Tag:KBWBUC
int KBWBeepOnUnknownChar_Set(sc_param_st *pParam);
int KBWBeepOnUnknownChar_Get(sc_param_st *pParam);

//���������������1��ʹ�ܡ���ֹ	Tag��KBWALT
int KBWEmulateAltKeypad1_Set(sc_param_st *pParam);
int KBWEmulateAltKeypad1_Get(sc_param_st *pParam);

//���������������2�Ľ�ֹ����������	Tag:KBWALK
int KBWEmulateAltKeypad2_Set(sc_param_st *pParam);
int KBWEmulateAltKeypad2_Get(sc_param_st *pParam);

//���ڹ��ܰ������Ctrl+ASCII��ʹ�ܺͽ�ֹ	Tag:KBWFKM
int KBWCtrlPlusStatus_Set(sc_param_st* pParam);
int KBWCtrlPlusStatus_Get(sc_param_st* pParam);

//�����ڲ����������ʱ������	Tag:KBWDLY
int KBWInterKeyStrokeDelay_Set(sc_param_st *pParam);
int KBWInterKeyStrokeDelay_Get(sc_param_st *pParam);

//���ڼ���Caps Lock����ʹ�ܡ���ֹ	Tag:KBWCAP
int KBWCapsLockStatus_Set(sc_param_st *pParam);
int KBWCapsLockStatus_Get(sc_param_st *pParam);

//���ڼ��̵Ĵ�Сдת�����ܵ�ʹ�ܡ���ֹ	Tag��KBWCAS
int KBWConvertCaseStatus_Set(sc_param_st *pParam);
int KBWConvertCaseStatus_Get(sc_param_st *pParam);

//���������������ּ��̵��������	Tag:KBWNUM
int KBWEmulateNUMPADType_Set(sc_param_st *pParam);
int KBWEmulateNUMPADType_Get(sc_param_st *pParam);

//���������������ּ��̵������"+,-,*"�Ƿ���ò���Numeric Keypad ��ֵ 	Tag:KBWNCH
int KBWEmulateNUMPADOperatorStatus_Set(sc_param_st *pParam);
int KBWEmulateNUMPADOperatorStatus_Get(sc_param_st *pParam);

//�������ÿ���ģʽ�ǿ������߹ر�
int KBWFastMode_Set(sc_param_st *pParam);
int KBWFastMode_Get(sc_param_st *pParam);

/********************************************************************************************************/

//����KBW��ѯ�ٶȵ����úͲ�ѯ
int KBWPollRate_Set(sc_param_st *pParam);
int KBWPollRate_Get(sc_param_st *pParam);

//����KBW CODE PAGE���úͲ�ѯ
int KBWCodePage_Set(sc_param_st *pParam);
int KBWCodePage_Get(sc_param_st *pParam);

//��������PS2����PCʱ�������Ƿ����Ӽ���
int PS2KeyboardConect_Set(sc_param_st *pParam);
int PS2KeyboardConect_Get(sc_param_st *pParam);

//��������PS2��CLKƵ��
int PS2KeyboardClock_Set(sc_param_st *pParam);
int PS2KeyboardClock_Get(sc_param_st *pParam);

/****************************************************************************
*@����ǹ���������
****************************************************************************/
//���ڻָ������������� Tag:BASFDF
int LoadFactoryCFGOfPedestal_Set(sc_param_st *pParam);

//���ڵ�����������Ϣ Tag:BASCLP
int ClearConfigInfoOfPedestal_Set(sc_param_st *pParam);

//���ڲ�ѯ����ɨ�������ȫ����Ϣ	Tag WLSQAL
int AllInfoInWirelessScanner_Get(sc_param_st *pParam);

//���ڲ�ѯ����ϵͳ��Ϣ	Tag BASQSY
int SysInfoOfPedestal_Get(sc_param_st *pParam);

//���ڲ�ѯ�����̼��汾��	Tag BASQFW
int FirmwareVerOfPedestal_Get(sc_param_st *pParam);

//���ڲ�ѯ������Ʒ�ͺ�	Tag BASQHN
int ProductTypeOfPedestal_Get(sc_param_st *pParam);

//���ڲ�ѯ������Ʒ���к�	Tag BASQSN
int SerialNumOfPedestal_Get(sc_param_st *pParam);

//���ڲ�ѯ������Ʒ��������	Tag BASQDA
int DateOfPedestal_Get(sc_param_st *pParam);

//����Batch Mode������	Tag��WLSBAT
int BatchModesOfWirelessScanner_Set(sc_param_st *pParam);
int BatchModesOfWirelessScanner_Get(sc_param_st *pParam);

//���ڲ�ѯ�û�FLASH����������	Tag:WLSQFC
int AmountOfCodesInUserFlash_Get(sc_param_st *pParam);

//��������û�FLASH������		Tag:WLSCLF
int ClearCodesInUserFlash_Set(sc_param_st *pParam);

//���ڿ������رպ˶Թ���		Tag��WLSCHK
int CheckingFunctionEnable_Set(sc_param_st *pParam);
int CheckingFunctionEnable_Get(sc_param_st *pParam);

//�������ú˶Գ�ʱʱ��			Tag��WLSCHT
int TimeoutOfCheckingFunction_Set(sc_param_st *pParam);
int TimeoutOfCheckingFunction_Get(sc_param_st *pParam);

//�������ô���ʱ���Խ����Զ��ػ� Tag��WLSAPO
int IdleTimeForAutoPowerdown_Set(sc_param_st *pParam);
int IdleTimeForAutoPowerdown_Get(sc_param_st *pParam);

//��������Zigbee�����ŵ�	Tag��WLSZCH
int SelectChannelOfZigbee_Set(sc_param_st *pParam);
int SelectChannelOfZigbee_Get(sc_param_st *pParam);

//���ڵ�������				Tag��BASRES
int ResetPedestal_Set(sc_param_st *pParam);

//�����������ߴ���Ӧ��ʱʱ�� 	Tag��WLSTTM
int TimeoutForACKInWirelessTrmt_Set(sc_param_st *pParam);
int TimeoutForACKInWirelessTrmt_Get(sc_param_st *pParam);

/****************************************************************************
*@�����ơ���׼���������
****************************************************************************/
//���������ƵĿ�����	Tag��ILLSCN
int IlluminationEnable_Set(sc_param_st *pParam);
int IlluminationEnable_Get(sc_param_st *pParam);

//���������Ƶ���ɫ��� 	Tag��ILLCLR
int ColorTypeOfIllumination_Set(sc_param_st *pParam);
int ColorTypeOfIllumination_Get(sc_param_st *pParam);

//���������Ƶ�λ��		Tag��ILLPST
int PositionOfIllumination_Set(sc_param_st *pParam);
int PositionOfIllumination_Get(sc_param_st *pParam);

//������׼�ƵĿ�����	Tag��AMLENA
int AimerEnable_Set(sc_param_st *pParam);
int AimerEnable_Get(sc_param_st *pParam);

/****************************************************************************
*@���ڴ���ָ��
****************************************************************************/
//�������ô��ڴ���ָ��	Tag:SCNTRG
int	Trigger2DecoBySerialCommand_Set(sc_param_st *pParam);
int	Trigger2DecoBySerialCommand_Get(sc_param_st *pParam);

/****************************************************************************
*@Setting in Sense Mode
****************************************************************************/
//����ѡ��Sense Mode�¶���ɹ���Ĺ���״̬��After A Good Read��	Tag:SENAGR
int StatusAfterGoodReadInSenseMode_Set(sc_param_st *pParam);
int StatusAfterGoodReadInSenseMode_Get(sc_param_st *pParam);

//����ѡ��Sense Mode�����״̬ʱ�����ƵĿ���	Tag:SENILL
int IlluStatusInSensingForSenseMode_Set(sc_param_st *pParam);
int IlluStatusInSensingForSenseMode_Get(sc_param_st *pParam);

/****************************************************************************
*@Exposure Imaging
****************************************************************************/
//���þ�ͷ���ع�����		Tag:EXPLVL
int ExposureTypes_Set(sc_param_st *pParam);
int ExposureTypes_Get(sc_param_st *pParam);

/****************************************************************************
*@Decode Area
****************************************************************************/
//���ý�������	Tag��CADENA
int DecoArea_Set(sc_param_st *pParam);
int DecoArea_Get(sc_param_st *pParam);

//������������Ķ��˷�Χ	Tag��CADTOP
int TopRangeOfCentralArea_Set(sc_param_st *pParam);
int TopRangeOfCentralArea_Get(sc_param_st *pParam);

//������������ĵ׶˷�Χ	Tag��CADBOT
int BottomRangeOfCentralArea_Set(sc_param_st *pParam);
int BottomRangeOfCentralArea_Get(sc_param_st *pParam);

//���������������˷�Χ	Tag��CADLEF
int LeftRangeOfCentralArea_Set(sc_param_st *pParam);
int LeftRangeOfCentralArea_Get(sc_param_st *pParam);

//��������������Ҷ˷�Χ	Tag��CADRIG
int RightRangeOfCentralArea_Set(sc_param_st *pParam);
int RightRangeOfCentralArea_Get(sc_param_st *pParam);

/****************************************************************************
*@��������ѡ��1D��Ʒ��
****************************************************************************/
//����ѡ�� �����м�����������һ����������������м������һ����������� 
//���ߡ��̶�ʶ��2��3��4�����롱
int DecoAreaLevelsFor1D_Set(sc_param_st *pParam);
int DecoAreaLevelsFor1D_Get(sc_param_st *pParam);

/****************************************************************************
*@���������ã�1D��Ʒ��
****************************************************************************/
//����ѡ��ÿ�����������̶����ʱ��	Tag��CDAINV
int IntervalTimeOfOutputingFor1D_Set(sc_param_st *pParam);
int IntervalTimeOfOutputingFor1D_Get(sc_param_st *pParam);

/****************************************************************************
*@Image Mirror(��ģ�����Ʒ���ṩ)							Tag:MIRROR
****************************************************************************/
//����ѡ��ÿ�����������̶����ʱ��	Tag��CDAINV
int ImageMirrorTypes_Set(sc_param_st *pParam);
int ImageMirrorTypes_Get(sc_param_st *pParam);


//�������˳����������ù��� SETUP
int StartExitSetup_Set(sc_param_st *pParam);
int StartExitSetup_Get(sc_param_st *pParam);

//�����͹ر����������ù��ܣ������ȼ���
int OpenCloseSetup_Set(sc_param_st *pParam);
int OpenCloseSetup_Get(sc_param_st *pParam);

//������ʶ�����ܣ������ȼ���
int EnableSettingCode_Set(sc_param_st *pParam);
int EnableSettingCode_Get(sc_param_st *pParam);

//������ʶ�����ܲ�ѯ�������ȼ���
int QuerySettingCode_Set(sc_param_st *pParam);
int QuerySettingCode_Get(sc_param_st *pParam);

//���������ݷ��͹���
int TrsmtSettingCode_Set(sc_param_st *pParam);
int TrsmtSettingCode_Get(sc_param_st *pParam);

//������������
int PowerOnVoice_Set(sc_param_st *pParam);
int PowerOnVoice_Get(sc_param_st *pParam);

//�ɹ�ʶ����ʾ��ʹ������
int GoodReadBeep_Set(sc_param_st *pParam);
int GoodReadBeep_Get(sc_param_st *pParam);

//�ɹ�ʶ����ʾ����������
int GoodReadBeepVolume_Set(sc_param_st *pParam);
int GoodReadBeepVolume_Get(sc_param_st *pParam);

//�ɹ�ʶ����ʾ����������
int GoodReadBeepDuration_Set(sc_param_st *pParam);
int GoodReadBeepDuration_Get(sc_param_st *pParam);

//�ɹ�ʶ����ʾ��Ƶ������
int GoodReadBeepFreq_Set(sc_param_st *pParam);
int GoodReadBeepFreq_Get(sc_param_st *pParam);

//�ɹ���ʾ���Ĵ�������
int GoodReadBeepNumber_Set(sc_param_st *pParam);
int GoodReadBeepNumber_Get(sc_param_st *pParam);

//�ɹ���ʾ���ļ��ʱ������
int GoodReadBeepIntval_Set(sc_param_st *pParam);
int GoodReadBeepIntval_Get(sc_param_st *pParam);

//ʧ����ʾ����ʹ������
int BadReadBeepEnable_Set(sc_param_st *pParam);
int BadReadBeepEnable_Get(sc_param_st *pParam);

//ʧ��ʶ����ʾ����������
int BadReadBeepVolume_Set(sc_param_st *pParam);
int BadReadBeepVolume_Get(sc_param_st *pParam);

//ʧ��ʶ����ʾ����������
int BadReadBeepDuration_Set(sc_param_st *pParam);
int BadReadBeepDuration_Get(sc_param_st *pParam);

//ʧ��ʶ����ʾ��Ƶ������
int BadReadBeepFreq_Set(sc_param_st *pParam);
int BadReadBeepFreq_Get(sc_param_st *pParam);

//ʧ����ʾ���Ĵ�������
int BadReadBeepNumber_Set(sc_param_st *pParam);
int BadReadBeepNumber_Get(sc_param_st *pParam);

//ʧ����ʾ���ļ��ʱ������
int BadReadBeepIntval_Set(sc_param_st *pParam);
int BadReadBeepIntval_Get(sc_param_st *pParam);

//�������Ź���ʹ������
int VoiceEnable_Set(sc_param_st *pParam);
int VoiceEnable_Get(sc_param_st *pParam);

//�������ż��ʱ������
int VoiceIntvalTime_Set(sc_param_st *pParam);
int VoiceIntvalTime_Get(sc_param_st *pParam);

//������������   CMD:VOICET
int VoiceTypeSelect_Set(sc_param_st *pParam);
int VoiceTypeSelect_Get(sc_param_st *pParam);

//����ģʽ����   CMD:VOICEM
int VoiceModeSelect_Set(sc_param_st *pParam);
int VoiceModeSelect_Get(sc_param_st *pParam);

//������������   CMD:VOICEV
int VoiceVolume_Set(sc_param_st *pParam);
int VoiceVolume_Get(sc_param_st *pParam);

//�ɹ�ʶ����ʹ������
int GoodReadLedEnable_Set(sc_param_st *pParam);
int GoodReadLedEnable_Get(sc_param_st *pParam);

//�ɹ�ʶ���Ƴ���ʱ������
int GoodReadLedDuration_Set(sc_param_st *pParam);
int GoodReadLedDuration_Get(sc_param_st *pParam);

//�ɹ�ʶ����ʹ������
int GoodReadVibrateEnable_Set(sc_param_st *pParam);
int GoodReadVibrateEnable_Get(sc_param_st *pParam);

//�ɹ�ʶ���𶯳���ʱ������
int GoodReadVibrateDuration_Set(sc_param_st *pParam);
int GoodReadVibrateDuration_Get(sc_param_st *pParam);

//������ʾ��ʹ������
int PowerOnLedEnable_Set(sc_param_st *pParam);
int PowerOnLedEnable_Get(sc_param_st *pParam);

//����ģʽѡ������
int WorkMode_Set(sc_param_st *pParam);
int WorkMode_Get(sc_param_st *pParam);

//���ģ�ⴥ������
int PureSoftTrig_Set(sc_param_st *pParam);
int PureSoftTrig_Get(sc_param_st *pParam);

//�����Ъʱ������
int RestTime_Set(sc_param_st *pParam);
int RestTime_Get(sc_param_st *pParam);

//��Ӧģʽ�£��ȴ�ͼ���ȶ���ʱ������
int ImgStabTimeout_Set(sc_param_st *pParam);
int ImgStabTimeout_Get(sc_param_st *pParam);

//����ʱ������
int ReadTime_Set(sc_param_st *pParam);
int ReadTime_Get(sc_param_st *pParam);

//��ͬ������ʱ��������
int RereadDelayEnable_Set(sc_param_st *pParam);
int RereadDelayEnable_Get(sc_param_st *pParam);

//��ͬ������ʱʱ������
int RereadDelayTime_Set(sc_param_st *pParam);
int RereadDelayTime_Get(sc_param_st *pParam);

//��Ъģʽ�µĶ���ʱ������
int FlashOnTime_Set(sc_param_st *pParam);
int FlashOnTime_Get(sc_param_st *pParam);

//��Ъģʽ�µļ�Ъʱ������
int FlashOffTime_Set(sc_param_st *pParam);
int FlashOffTime_Get(sc_param_st *pParam);

//��Ӧ����������
int Sensitivity_Set(sc_param_st *pParam);
int Sensitivity_Get(sc_param_st *pParam);

//Not Good Readʹ������
int NotGoodReadEnable_Set(sc_param_st *pParam);
int NotGoodReadEnable_Get(sc_param_st *pParam);

//Not Good Read ��Ϣ����
int NotGoodReadMsg_Set(sc_param_st *pParam);
int NotGoodReadMsg_Get(sc_param_st *pParam);
int NotGoodReadLenth_Set(sc_param_st *pParam);
int NotGoodReadLenth_Get(sc_param_st *pParam);

//�ػ�ģʽ
int ModePowerOff_Set(sc_param_st *pParam);
int ModePowerOff_Get(sc_param_st *pParam);
//Idleģʽ
int ModeIdle_Set(sc_param_st *pParam);
int ModeIdle_Get(sc_param_st *pParam);
//Sleepģʽ
int ModeSleep_Set(sc_param_st *pParam);
int ModeSleep_Get(sc_param_st *pParam);
//testģʽ
int ModeTest_Set(sc_param_st *pParam);
int ModeTest_Get(sc_param_st *pParam);
//Rebootģʽ
int ModeReboot_Set(sc_param_st *pParam);
int ModeReboot_Get(sc_param_st *pParam);

//�Զ����߹���ѡ��
int AutoSleepEnable_Set(sc_param_st *pParam);
int AutoSleepEnable_Get(sc_param_st *pParam);

//�Զ����߿���ʱ������
int AutoSleepIdleTime_Set(sc_param_st *pParam);
int AutoSleepIdleTime_Get(sc_param_st *pParam);

//��ȫ��������
int SafeLv_Set(sc_param_st *pParam);
int SafeLv_Get(sc_param_st *pParam);

/*********************************************************************************************/
//////////////////////////////////////////////////////////////////////////
//��ѯϵͳ��Ϣ
int SysInfo_Set(sc_param_st *pParam);
int SysInfo_Get(sc_param_st *pParam);
//��ѯ�̼��汾��
int FirmwareVer_Set(sc_param_st *pParam);
int FirmwareVer_Get(sc_param_st *pParam);
//��ѯ����汾��
int DecoderVer_Get(sc_param_st *pParam);
int DecoderVer_Set(sc_param_st *pParam);
//��ѯӲ����Ϣ
int HardwareVer_Get(sc_param_st *pParam);
int HardwareVer_Set(sc_param_st *pParam);
//��ѯ��Ʒ����
int ProductName_Get(sc_param_st *pParam);
int ProductName_Set(sc_param_st *pParam);
//��ѯ��Ʒ���к�
int ProductPID_Get(sc_param_st *pParam);
int ProductPID_Set(sc_param_st *pParam);
//��ѯ��Ʒ��������
int ProduceDate_Get(sc_param_st *pParam);
int ProduceDate_Set(sc_param_st *pParam);
//��ѯ��ƷOEM�ͺ�
int ProductOEMType_Get(sc_param_st *pParam);
int ProductOEMType_Set(sc_param_st *pParam);
//��ѯ��ƷOEM���кţ�ESN��
int ProductOEMESN_Get(sc_param_st *pParam);
int ProductOEMESN_Set(sc_param_st *pParam);
//��ѯ��ƷOEM��������
int ProduceOEMDate_Get(sc_param_st *pParam);
int ProduceOEMDate_Set(sc_param_st *pParam);

//��������ʹ������
int AllCodeTypeEnable_Set(sc_param_st *pParam);
int AllCodeTypeEnable_Get(sc_param_st *pParam);

//1D����ʹ������
int CodeType1DEnable_Set(sc_param_st *pParam);
int CodeType1DEnable_Get(sc_param_st *pParam);

//���ж�ά����ʹ������	Tag:ALL2DC
int CodeType2DEnable_Set(sc_param_st *pParam);
int CodeType2DEnable_Get(sc_param_st *pParam);

//������������ʹ������	Tag:ALLPST
int CodeTypePostCodeEnable_Set(sc_param_st *pParam);
int CodeTypePostCodeEnable_Get(sc_param_st *pParam);

//1D����ĵ�˫������	Tag:A1DDOU
int DualCodeTypesFor1D_Set(sc_param_st *pParam);
int DualCodeTypesFor1D_Get(sc_param_st *pParam);

//ʶ����������ʹ������	Tag:ALLINV
int InverterOfCodesEnable_Set(sc_param_st *pParam);
int InverterOfCodesEnable_Get(sc_param_st *pParam);
//����EANUPC����������
int AllEanUpcAddon_Set(sc_param_st *pParam);
int AllEanUpcAddon_Get(sc_param_st *pParam);

//����Febraban�ַ�����ʱ��ʹ�ܡ���ֹ	Tag��FEBSEN
int FebrabanInterCharDelayEnable_Set(sc_param_st *pParam);
int FebrabanInterCharDelayEnable_Get(sc_param_st *pParam);

//����Febraban�ַ�����ʱʱ��	Tag:FEBSDT
int FebrabanInterCharDelay_Set(sc_param_st *pParam);
int FebrabanInterCharDelay_Get(sc_param_st *pParam);

//����Febrabanÿ12�ַ�����ʱ��ʹ�ܡ���ֹ	Tag��FEBMEN
int FebrabanInterAreaDelayEnable_Set(sc_param_st *pParam);
int FebrabanInterAreaDelayEnable_Get(sc_param_st *pParam);

//����Febrabanÿ12�ַ�����ʱʱ��	Tag:FEBMDT
int FebrabanInterAreaDelay_Set(sc_param_st *pParam);
int FebrabanInterAreaDelay_Get(sc_param_st *pParam);
/****************************************************************************
*@��ͨCode 128
****************************************************************************/
//����Code 128 Ĭ������	Tag��128DEF
int Code128LoadDefault_Set(sc_param_st *pParam);
int Code128LoadDefault_Get(sc_param_st *pParam);

//Code 128��ʹ�ܡ���ֹ	Tag��128ENA
int Code128Enable_Set(sc_param_st *pParam);
int Code128Enable_Get(sc_param_st *pParam);

//����Code 128��С���ȵ�����	Tag��128MIN
int Code128MinLen_Set(sc_param_st *pParam);
int Code128MinLen_Get(sc_param_st *pParam);

//����Code 128����󳤶�	Tag��128MAX
int Code128MaxLen_Set(sc_param_st *pParam);
int Code128MaxLen_Get(sc_param_st *pParam);

//����Code 128 CODEID�����ʹ�ܡ���ֹ	Tag��CID002
int Code128CodeID_Set(sc_param_st *pParam);
int Code128CodeID_Get(sc_param_st *pParam);

//����Code 128У��λ���͵�ʹ�ܡ���ֹ	Tag��128CHK
int Code128ChkTrsmt_Set(sc_param_st *pParam);
int Code128ChkTrsmt_Get(sc_param_st *pParam);

//����Code 128��FNC1�����ʹ�ܡ���ֹ	Tag��128FNC
int Code128FNC1_Set(sc_param_st *pParam);
int Code128FNC1_Get(sc_param_st *pParam);

/****************************************************************************
*@EAN-8
****************************************************************************/
//����EAN-8��Ĭ������	Tag:EA8DEF
int EAN8LoadDefault_Set(sc_param_st *pParam);
int EAN8LoadDefault_Get(sc_param_st *pParam);

//����EAN-8��ʹ�ܡ���ֹ	Tag:EA8ENA
int EAN8Enable_Set(sc_param_st *pParam);
int EAN8Enable_Get(sc_param_st *pParam);

//����EAN-8У��λ��ʹ�ܡ���ֹ	Tag��EA8CHK
int EAN8ChkTrsmt_Set(sc_param_st *pParam);
int EAN8ChkTrsmt_Get(sc_param_st *pParam);

//����EAN-8 2λ��չ���ʹ�ܡ���ֹ	Tag:EA8AD2
int EAN8AddCode2Enable_Set(sc_param_st *pParam);
int EAN8AddCode2Enable_Get(sc_param_st *pParam);

//����EAN-8 5λ��չ���ʹ�ܡ���ֹ	Tag:EA8AD5
int EAN8AddCode5Enable_Set(sc_param_st *pParam);
int EAN8AddCode5Enable_Get(sc_param_st *pParam);

//����EAN-8��չ���ʹ�ܡ���ֹ	Tag:EA8REQ
int EAN8AddedCodeEnable_Set(sc_param_st *pParam);
int EAN8AddedCodeEnable_Get(sc_param_st *pParam);

//����EAN-8��չ���Ƿ���ӷָ���	Tag:EA8SEP
int EAN8SEPRInAddedCodeEnable_Set(sc_param_st *pParam);
int EAN8SEPRInAddedCodeEnable_Get(sc_param_st *pParam);

//����EAN-8�Ƿ�ת��ΪEAN13��������չΪ13λ	Tag��EA8EXP
int EAN8ExpandedType_Set(sc_param_st *pParam);
int EAN8ExpandedType_Get(sc_param_st *pParam);

//����EAN-8 CODEID�����ʹ�ܡ���ֹ	Tag��CID004
int EAN8CodeID_Set(sc_param_st *pParam);
int EAN8CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@EAN-13
****************************************************************************/
//����EAN13��Ĭ��ͨ������	Tag��E13DEF
int EAN13LoadDefault_Set(sc_param_st *pParam);
int EAN13LoadDefault_Get(sc_param_st *pParam);

//����EAN13��ʹ�ܡ���ֹ	Tag��E13ENA
int EAN13Enable_Set(sc_param_st *pParam);
int EAN13Enable_Get(sc_param_st *pParam);

//����EAN13У��λ��ʹ�ܡ���ֹ	Tag��E13CHK
int EAN13ChkTrsmt_Set(sc_param_st *pParam);
int EAN13ChkTrsmt_Get(sc_param_st *pParam);

//����EAN13 2λ ��չ���ʹ�ܡ���ֹ  Tag:E13AD2
int EAN13AddCode2Enable_Set(sc_param_st *pParam);
int EAN13AddCode2Enable_Get(sc_param_st *pParam);

//����EAN13 5λ ��չ���ʹ�ܡ���ֹ  Tag:E13AD5
int EAN13AddCode5Enable_Set(sc_param_st *pParam);
int EAN13AddCode5Enable_Get(sc_param_st *pParam);

//����EAN13��չ���ʹ�ܡ���ֹ	Tag:E13REQ
int EAN13AddedCodeEnable_Set(sc_param_st *pParam);
int EAN13AddedCodeEnable_Get(sc_param_st *pParam);

//����EAN13��չ���Ƿ���ӷָ���	Tag:E13SEP
int EAN13SEPRInAddedCodeEnable_Set(sc_param_st *pParam);
int EAN13SEPRInAddedCodeEnable_Get(sc_param_st *pParam);

//����EAN13 Beginning with 290 Addenda Tag��E132900�� E132901
int EAN13Beggin290Addenda_Set(sc_param_st *pParam);
int EAN13Beggin290Addenda_Get(sc_param_st *pParam);

//����EAN13 Beginning with 378_379 Addenda Tag��E132780�� E132781
int EAN13Beggin378_379Addenda_Set(sc_param_st *pParam);
int EAN13Beggin378_379Addenda_Get(sc_param_st *pParam);

//����EAN13 Beginning with 414_419 Addenda Tag��E134140�� E134141
int EAN13Beggin414_419Addenda_Set(sc_param_st *pParam);
int EAN13Beggin414_419Addenda_Get(sc_param_st *pParam);

//����EAN13 Beginning with 434_439 Addenda Tag��E134340�� E134341
int EAN13Beggin434_439Addenda_Set(sc_param_st *pParam);
int EAN13Beggin434_439Addenda_Get(sc_param_st *pParam);

//����EAN13 Beginning with 977 Addenda Tag��E139770�� E139771
int EAN13Beggin977Addenda_Set(sc_param_st *pParam);
int EAN13Beggin977Addenda_Get(sc_param_st *pParam);

//����EAN13 Beginning with 978 Addenda Tag��E139780�� E139781
int EAN13Beggin978Addenda_Set(sc_param_st *pParam);
int EAN13Beggin978Addenda_Get(sc_param_st *pParam);

//����EAN13 Beginning with 979 Addenda Tag��E139790�� E139791
int EAN13Beggin979Addenda_Set(sc_param_st *pParam);
int EAN13Beggin979Addenda_Get(sc_param_st *pParam);

//����EAN13 CODEID�����ʹ�ܡ���ֹ	Tag��CID005
int EAN13CodeID_Set(sc_param_st *pParam);
int EAN13CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@UPC-E
****************************************************************************/
//����UPC-E��Ĭ������	Tag:UPEDEF
int UPCELoadDefault_Set(sc_param_st *pParam);
int UPCELoadDefault_Get(sc_param_st *pParam);

//����UPC-E��ʹ�ܡ���ֹ	Tag��UPEENA
int UPCEEnable_Set(sc_param_st *pParam);
int UPCEEnable_Get(sc_param_st *pParam);

//����UPC-E��UPC-E0ʹ�ܡ���ֹ	Tag��UPEEN0
int UPCE0Enable_Set(sc_param_st *pParam);
int UPCE0Enable_Get(sc_param_st *pParam);

//����UPC-E��UPC-E1ʹ�ܡ���ֹ	Tag��UPEEN1
int UPCE1Enable_Set(sc_param_st *pParam);
int UPCE1Enable_Get(sc_param_st *pParam);

//����UPC-EУ��λ��ʹ�ܡ���ֹ	Tag��UPECHK
int UPCEChkTrsmt_Set(sc_param_st *pParam);
int UPCEChkTrsmt_Get(sc_param_st *pParam);

//����UPC-E 2λ��չ���ʹ�ܡ���ֹ Tag:UPEAD2
int UPCEAddCode2Enable_Set(sc_param_st *pParam);
int UPCEAddCode2Enable_Get(sc_param_st *pParam);

//����UPC-E 5λ��չ���ʹ�ܡ���ֹ Tag:UPEAD5
int UPCEAddCode5Enable_Set(sc_param_st *pParam);
int UPCEAddCode5Enable_Get(sc_param_st *pParam);

//����UPC-E��չ���ʹ�ܡ���ֹ	Tag:UPEREQ
int UPCEAddedCodeEnable_Set(sc_param_st *pParam);
int UPCEAddedCodeEnable_Get(sc_param_st *pParam);

//����UPC-E��չ���Ƿ���ӷָ���	Tag:UPESEP
int UPCESEPRInAddedCodeEnable_Set(sc_param_st *pParam);
int UPCESEPRInAddedCodeEnable_Get(sc_param_st *pParam);

//����UPC-E ����ǰ�ĵ�����	Tag��UPEPRE
int UPCETrsmtSysDigitType_Set(sc_param_st *pParam);
int UPCETrsmtSysDigitType_Get(sc_param_st *pParam);

//����UPC-Eʹ�ܡ���ֹת��ΪUPC-A Tag��UPEEXP
int UPCEExpandedType_Set(sc_param_st *pParam);
int UPCEExpandedType_Get(sc_param_st *pParam);

//����UPC-E CODEID�����ʹ�ܡ���ֹ	Tag��CID006
int UPCECodeID_Set(sc_param_st *pParam);
int UPCECodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@UPC-A
****************************************************************************/
//����UPC-A��Ĭ������	Tag:UPADEF
int UPCALoadDefault_Set(sc_param_st *pParam);
int UPCALoadDefault_Get(sc_param_st *pParam);

//����UPC-A��ʹ�ܡ���ֹ	Tag��UPAENA
int UPCAEnable_Set(sc_param_st *pParam);
int UPCAEnable_Get(sc_param_st *pParam);

//����UPC-AУ��λ��ʹ�ܡ���ֹ	Tag��UPACHK
int UPCAChkTrsmt_Set(sc_param_st *pParam);
int UPCAChkTrsmt_Get(sc_param_st *pParam);

//����UPC-A 2λ��չ���ʶ��ʹ�ܡ���ֹ	Tag:UPAAD2
int UPCAAddCode2Enable_Set(sc_param_st *pParam);
int UPCAAddCode2Enable_Get(sc_param_st *pParam);

//����UPC-A 5λ��չ���ʶ��ʹ�ܡ���ֹ	Tag:UPAAD5
int UPCAAddCode5Enable_Set(sc_param_st *pParam);
int UPCAAddCode5Enable_Get(sc_param_st *pParam);

//����UPC-E��չ���ʹ�ܡ���ֹ	Tag:UPAREQ
int UPCAAddedCodeEnable_Set(sc_param_st *pParam);
int UPCAAddedCodeEnable_Get(sc_param_st *pParam);

//����UPC-A��չ���Ƿ���ӷָ���	Tag:UPASEP
int UPCASEPRInAddedCodeEnable_Set(sc_param_st *pParam);
int UPCASEPRInAddedCodeEnable_Get(sc_param_st *pParam);

//����UPC-A ����ǰ�ĵ�����	Tag��UPAPRE
int UPCATrsmtSysDigitType_Set(sc_param_st *pParam);
int UPCATrsmtSysDigitType_Get(sc_param_st *pParam);

//����UPC-A CODEID�����ʹ�ܡ���ֹ	Tag��CID007
int UPCACodeID_Set(sc_param_st *pParam);
int UPCACodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Coupon
****************************************************************************/
//����UPCA/EAN13���Extended Coupon Code��״̬��off,allow,require���֣�Tag:CPNENA
int UPCA_EAN13WithExtendedCoupon_Set(sc_param_st *pParam);
int UPCA_EAN13WithExtendedCoupon_Get(sc_param_st *pParam);

//Coupon GS1 DataBar Output Tag:CPNGS1
int CouponGS1DataBarOutput_Set(sc_param_st *pParam);
int CouponGS1DataBarOutput_Get(sc_param_st *pParam);

/****************************************************************************
*@Interleaved 2 of 5
****************************************************************************/
//����Inter2of5��Ĭ������	Tag:I25DEF
int Inter2of5LoadDefault_Set(sc_param_st *pParam);
int Inter2of5LoadDefault_Get(sc_param_st *pParam);

//����Inter2of5��ʹ�ܡ���ֹ	Tag��I25ENA
int Inter2of5Enable_Set(sc_param_st *pParam);
int Inter2of5Enable_Get(sc_param_st *pParam);

//����Inter2of5��ʶ����С����	Tag��I25MIN
int Inter2of5MinLen_Set(sc_param_st *pParam);
int Inter2of5MinLen_Get(sc_param_st *pParam);

//����Inter2of5��ʶ����󳤶�	Tag��I25MAX
int Inter2of5MaxLen_Set(sc_param_st *pParam);
int Inter2of5MaxLen_Get(sc_param_st *pParam);

//����Inter2of5У��λ��ʹ�ܡ���ֹ	Tag��I25CHK
int Inter2of5ChkTrsmt_Set(sc_param_st *pParam);
int Inter2of5ChkTrsmt_Get(sc_param_st *pParam);

//����ʶ���̶��������빦�ܵ�ʹ�ܡ���ֹ	Tag:I25LEN
int Inter2of5DecoFixedLenEnable_Set(sc_param_st *pParam);
int Inter2of5DecoFixedLenEnable_Get(sc_param_st *pParam);

//����Inter2of5��ӹ̶�����ֵ	Tag:I25LSE
int Inter2of5AddFixedLen_Set(sc_param_st *pParam);
int Inter2of5AddFixedLen_Get(sc_param_st *pParam);

//����Inter2of5ȡ���̶�����ֵ	Tag:I25LCA
int Inter2of5CancelFixedLen_Set(sc_param_st *pParam);
int Inter2of5CancelFixedLen_Get(sc_param_st *pParam);

//����Inter2of5 ��Febraban ʹ��̬��������Ϣ��չ	Tag:I25FBB
int Inter2of5FebrabanEnable_Set(sc_param_st *pParam);
int Inter2of5FebrabanEnable_Get(sc_param_st *pParam);

//����Inter2of5 CODEID�����ʹ�ܡ���ֹ	Tag��CID008
int Inter2of5CodeID_Set(sc_param_st *pParam);
int Inter2of5CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@ITF-14
****************************************************************************/
//����ITF-14 ��Ĭ������	Tag:I14DEF
int ITF14LoadDefault_Set(sc_param_st *pParam);
int ITF14LoadDefault_Get(sc_param_st *pParam);

//����ITF-14 ��ʹ�ܡ���ֹ��У��λ�Ĵ���	Tag��I14ENA
int ITF14Enable_Set(sc_param_st *pParam);
int ITF14Enable_Get(sc_param_st *pParam);

//����ITF-14 CODEID�����ʹ�ܡ���ֹ	Tag��CID009
int ITF14CodeID_Set(sc_param_st *pParam);
int ITF14CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@ITF-6
****************************************************************************/
//����ITF-6 ��Ĭ������	Tag:IT6DEF
int ITF6LoadDefault_Set(sc_param_st *pParam);
int ITF6LoadDefault_Get(sc_param_st *pParam);

//����ITF-6 ��ʹ�ܡ���ֹ��У��λ�Ĵ���	Tag��IT6ENA
int ITF6Enable_Set(sc_param_st *pParam);
int ITF6Enable_Get(sc_param_st *pParam);

//����ITF-6 CODEID�����ʹ�ܡ���ֹ	Tag��CID010
int ITF6CodeID_Set(sc_param_st *pParam);
int ITF6CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Matrix 2 of 5
****************************************************************************/
//����Matrix 2 of 5 ��Ĭ������	Tag:M25DEF
int Matrix2of5LoadDefault_Set(sc_param_st *pParam);
int Matrix2of5LoadDefault_Get(sc_param_st *pParam);

//����Matrix 2 of 5 ��ʹ�ܡ���ֹ	Tag��M25ENA
int Matrix2of5Enable_Set(sc_param_st *pParam);
int Matrix2of5Enable_Get(sc_param_st *pParam);

//����Matrix 2 of 5��ʶ����С����	Tag��M25MIN
int Matrix2of5MinLen_Set(sc_param_st *pParam);
int Matrix2of5MinLen_Get(sc_param_st *pParam);

//����Matrix 2 of 5��ʶ����󳤶�	Tag��M25MAX
int Matrix2of5MaxLen_Set(sc_param_st *pParam);
int Matrix2of5MaxLen_Get(sc_param_st *pParam);

//����Matrix 2 of 5У��λ��ʹ�ܡ���ֹ	Tag��I25CHK
int Matrix2of5ChkTrsmt_Set(sc_param_st *pParam);
int Matrix2of5ChkTrsmt_Get(sc_param_st *pParam);

//����Matrix 2 of 5 CODEID�����ʹ�ܡ���ֹ	Tag��CID011
int Matrix2of5CodeID_Set(sc_param_st *pParam);
int Matrix2of5CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Code39
****************************************************************************/
//����Code39��Ĭ������	Tag:C39DEF
int Code39LoadDefault_Set(sc_param_st *pParam);
int Code39LoadDefault_Get(sc_param_st *pParam);

//����Code39��ʹ�ܡ���ֹ	Tag��C39ENA
int Code39Enable_Set(sc_param_st *pParam);
int Code39Enable_Get(sc_param_st *pParam);

//����Code39��ʶ����С����	Tag��C39MIN
int Code39MinLen_Set(sc_param_st *pParam);
int Code39MinLen_Get(sc_param_st *pParam);

//����Code39��ʶ����󳤶�	Tag��C39MAX
int Code39MaxLen_Set(sc_param_st *pParam);
int Code39MaxLen_Get(sc_param_st *pParam);

//����Code39У��λ��ʹ�ܡ���ֹ	Tag��C39CHK
int Code39ChkTrsmt_Set(sc_param_st *pParam);
int Code39ChkTrsmt_Get(sc_param_st *pParam);

//����Code39�Ĵ�����ʼ������ֹ����ʹ�ܡ���ֹ	Tag��C39TSC
int Code39TrsmtSTXETXEnable_Set(sc_param_st *pParam);
int Code39TrsmtSTXETXEnable_Get(sc_param_st *pParam);

//����Code39 Full ASCII��ʹ�ܡ���ֹ	Tag��C39ASC
int Code39FullASCIIEnable_Set(sc_param_st *pParam);
int Code39FullASCIIEnable_Get(sc_param_st *pParam);

//����Code39��Code 32 Pharmaceutical (PARAF)��ʹ�ܡ���ֹ	Tag��C39E32
int Code39Code32PARAFEnable_Set(sc_param_st *pParam);
int Code39Code32PARAFEnable_Get(sc_param_st *pParam);

//����Code39 �Ƿ����Code32ǰ׺	Tag:C39S32
int Code39AddSTXOfCode32Enable_Set(sc_param_st *pParam);
int Code39AddSTXOfCode32Enable_Get(sc_param_st *pParam);

//����Code39 �Ƿ����Code32��ʼ��/��ֹ��	Tag:C39T32
int Code32TrsmtSTXETXEnable_Set(sc_param_st *pParam);
int Code32TrsmtSTXETXEnable_Get(sc_param_st *pParam);

//����Code39 �Ƿ����Code32У���	Tag:C39C32
int Code32ChkTrsmt_Set(sc_param_st *pParam);
int Code32ChkTrsmt_Get(sc_param_st *pParam);

//����Code39 CODEID�����ʹ�ܡ���ֹ	Tag��CID013
int Code39CodeID_Set(sc_param_st *pParam);
int Code39CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Codabar
****************************************************************************/
//����Codabar��Ĭ������	Tag:CBADEF
int CodabarLoadDefault_Set(sc_param_st *pParam);
int CodabarLoadDefault_Get(sc_param_st *pParam);

//����Codabar��ʹ�ܡ���ֹ	Tag��CBAENA
int CodabarEnable_Set(sc_param_st *pParam);
int CodabarEnable_Get(sc_param_st *pParam);

//����Codabar��ʶ����С����	Tag��CBAMIN
int CodabarMinLen_Set(sc_param_st *pParam);
int CodabarMinLen_Get(sc_param_st *pParam);

//����Codabar��ʶ����󳤶�	Tag��CBAMAX
int CodabarMaxLen_Set(sc_param_st *pParam);
int CodabarMaxLen_Get(sc_param_st *pParam);

//����CodabarУ��λ��ʹ�ܡ���ֹ	Tag��CBACHK
int CodabarChkTrsmt_Set(sc_param_st *pParam);
int CodabarChkTrsmt_Get(sc_param_st *pParam);

//����Codabar������ʼ������ֹ����ʹ�ܡ���ֹ	Tag��CBATSC
int CodabarTrsmtSTXETXEnable_Set(sc_param_st *pParam);
int CodabarTrsmtSTXETXEnable_Get(sc_param_st *pParam);

//����Codabar��ʼ������ֹ���ĸ�ʽ	Tag��CBASCF
int CodabarSTXETXFormats_Set(sc_param_st *pParam);
int CodabarSTXETXFormats_Get(sc_param_st *pParam);

//����Codabar��CLSI�༭��ʹ�ܡ���ֹ	Tag:CBACLS
int CodabarCLSIEdit_Set(sc_param_st *pParam);
int CodabarCLSIEdit_Get(sc_param_st *pParam);

//����Codabar CODEID�����ʹ�ܡ���ֹ	Tag��CID015
int CodabarCodeID_Set(sc_param_st *pParam);
int CodabarCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Code93
****************************************************************************/
//����Code93��Ĭ������	Tag:C93DEF
int Code93LoadDefault_Set(sc_param_st *pParam);
int Code93LoadDefault_Get(sc_param_st *pParam);

//����Code93��ʹ�ܡ���ֹ	Tag��C93ENA
int Code93Enable_Set(sc_param_st *pParam);
int Code93Enable_Get(sc_param_st *pParam);

//����Code93��ʶ����С����	Tag��C93MIN
int Code93MinLen_Set(sc_param_st *pParam);
int Code93MinLen_Get(sc_param_st *pParam);

//����Code93��ʶ����󳤶�	Tag��C93MAX
int Code93MaxLen_Set(sc_param_st *pParam);
int Code93MaxLen_Get(sc_param_st *pParam);

//����Code93У��λ��ʹ�ܡ���ֹ	Tag��C93CHK
int Code93ChkTrsmt_Set(sc_param_st *pParam);
int Code93ChkTrsmt_Get(sc_param_st *pParam);

//����Code93 CODEID�����ʹ�ܡ���ֹ	Tag��CID017
int Code93CodeID_Set(sc_param_st *pParam);
int Code93CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@GS1-128(UCC/EAN-128)
****************************************************************************/
//����GS1-128��Ĭ������	Tag:GS1DEF
int GS1_128LoadDefault_Set(sc_param_st *pParam);
int GS1_128LoadDefault_Get(sc_param_st *pParam);

//����GS1-128��ʹ�ܡ���ֹ	Tag��GS1ENA
int GS1_128Enable_Set(sc_param_st *pParam);
int GS1_128Enable_Get(sc_param_st *pParam);

//����GS1-128��ʶ����С����	Tag��GS1MIN
int GS1_128MinLen_Set(sc_param_st *pParam);
int GS1_128MinLen_Get(sc_param_st *pParam);

//����GS1-128��ʶ����󳤶�	Tag��GS1MAX
int GS1_128MaxLen_Set(sc_param_st *pParam);
int GS1_128MaxLen_Get(sc_param_st *pParam);

//����GS1-128У��λ��ʹ�ܡ���ֹ	Tag��GS1CHK
int GS1_128ChkTrsmt_Set(sc_param_st *pParam);
int GS1_128ChkTrsmt_Get(sc_param_st *pParam);

//����GS1-128��FNC1�����ʹ�ܡ���ֹ	Tag��GS1FNC
int GS1_128OutputFNC1_Set(sc_param_st *pParam);
int GS1_128OutputFNC1_Get(sc_param_st *pParam);

//����GS1-128 CODEID�����ʹ�ܡ���ֹ	Tag��CID003
int GS1_128CodeID_Set(sc_param_st *pParam);
int GS1_128CodeID_Get(sc_param_st *pParam);

int QueryBuffSizeOfInstructions_Set(sc_param_st *pParam);
/****************************************************************************
*@GS1 DataBar(RSS)
****************************************************************************/
//����GS1 DataBar(RSS)��Ĭ������	Tag:RSSDEF
int RSSLoadDefault_Set(sc_param_st *pParam);
int RSSLoadDefault_Get(sc_param_st *pParam);

//����GS1 DataBar(RSS)��ʹ�ܡ���ֹ	Tag:RSSENA
int RSSEnable_Set(sc_param_st *pParam);
int RSSEnable_Get(sc_param_st *pParam);

//����GS1 DataBar(RSS)����AI(01)�ַ���ʹ�ܡ���ֹ Tag:RSSTAI
int RSSTrsmtAIEnable_Set(sc_param_st *pParam);
int RSSTrsmtAIEnable_Get(sc_param_st *pParam);

//����GS1 DataBar(RSS) GS1 DataBar Omnidirectional�ַ���ʹ�ܡ���ֹ Tag:RSSE14
int RSSConvert2RSS14_Set(sc_param_st *pParam);
int RSSConvert2RSS14_Get(sc_param_st *pParam);

//����GS1 DataBar(RSS)��GS1 DataBar Limitedʹ�ܡ���ֹ Tag:RSSENL
int RSSForGS1DatabarLimited_Set(sc_param_st *pParam);
int RSSForGS1DatabarLimited_Get(sc_param_st *pParam);

//����GS1 DataBar(RSS)��GS1 DataBar Limitedʹ�ܡ���ֹ Tag:RSSENE
int RSSForGS1DatabarExpand_Set(sc_param_st *pParam);
int RSSForGS1DatabarExpand_Get(sc_param_st *pParam);

//����GS1 DataBar(RSS) CODEID�����ʹ�ܡ���ֹ	Tag��CID031
int RSSCodeID_Set(sc_param_st *pParam);
int RSSCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@GS1 Composite
****************************************************************************/
//����GS1 Composite��Ĭ������	Tag:CPTDEF
int GS1_CompositeLoadDefault_Set(sc_param_st *pParam);
int GS1_CompositeLoadDefault_Get(sc_param_st *pParam);

//����GS1 Composite��ʹ�ܡ���ֹ	Tag:CPTENA
int GS1_CompositeEnable_Set(sc_param_st *pParam);
int GS1_CompositeEnable_Get(sc_param_st *pParam);

//����GS1 Composite��UPC/EAN�汾��ʹ�ܡ���ֹ Tag:CPTUPC
int GS1_CompositeForUPC_EAN_Set(sc_param_st *pParam);
int GS1_CompositeForUPC_EAN_Get(sc_param_st *pParam);

//����GS1 Composite CODEID�����ʹ�ܡ���ֹ	Tag��CID030
int GS1_CompositeCodeID_Set(sc_param_st *pParam);
int GS1_CompositeCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Code11
****************************************************************************/
//����Code11��Ĭ������		Tag:C11DEF
int Code11LoadDefault_Set(sc_param_st *pParam);
int Code11LoadDefault_Get(sc_param_st *pParam);

//����Code11��ʹ�ܡ���ֹ	Tag��C11ENA
int Code11Enable_Set(sc_param_st *pParam);
int Code11Enable_Get(sc_param_st *pParam);

//����Code11��ʶ����С����	Tag��C11MIN
int Code11MinLen_Set(sc_param_st *pParam);
int Code11MinLen_Get(sc_param_st *pParam);

//����Code11��ʶ����󳤶�	Tag��C11MAX
int Code11MaxLen_Set(sc_param_st *pParam);
int Code11MaxLen_Get(sc_param_st *pParam);

//����Code11У��λ��ʹ�ܡ���ֹ��У��λ��ѡ��	Tag��C11CHK
int Code11ChkTrsmt_Set(sc_param_st *pParam);
int Code11ChkTrsmt_Get(sc_param_st *pParam);

//����Code11У��λ�Ƿ���	Tag��C11TCK
int Code11ChkTrsmtAction_Set(sc_param_st *pParam);
int Code11ChkTrsmtAction_Get(sc_param_st *pParam);


//����Code11 CODEID�����ʹ�ܡ���ֹ	Tag��CID028
int Code11CodeID_Set(sc_param_st *pParam);
int Code11CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@ISBN
****************************************************************************/
//����ISBN��Ĭ������	Tag:ISBDEF
int ISBNLoadDefault_Set(sc_param_st *pParam);
int ISBNLoadDefault_Get(sc_param_st *pParam);

//����ISBN��ʹ�ܡ���ֹ	Tag��ISBENA
int ISBNEnable_Set(sc_param_st *pParam);
int ISBNEnable_Get(sc_param_st *pParam);

//����ISBN�ķ������ָ�����10������13����	Tag��ISBT10
int ISBNLenOfNUM_Set(sc_param_st *pParam);
int ISBNLenOfNUM_Get(sc_param_st *pParam);

//����ISBN 2λ��չ���ʶ��ʹ�ܡ���ֹ	Tag:ISBAD2
int ISBNAddCode2Enable_Set(sc_param_st *pParam);
int ISBNAddCode2Enable_Get(sc_param_st *pParam);

//����ISBN 5λ��չ���ʶ��ʹ�ܡ���ֹ	Tag:ISBAD5
int ISBNAddCode5Enable_Set(sc_param_st *pParam);
int ISBNAddCode5Enable_Get(sc_param_st *pParam);

//����ISBN��չ���ʹ�ܡ���ֹ	Tag:ISBNREQ
int ISBNAddedCodeEnable_Set(sc_param_st *pParam);
int ISBNAddedCodeEnable_Get(sc_param_st *pParam);

//����ISBN��չ���Ƿ���ӷָ���	Tag:ISBSEP
int ISBNSEPRInAddedCodeEnable_Set(sc_param_st *pParam);
int ISBNSEPRInAddedCodeEnable_Get(sc_param_st *pParam);

//����ISBN CODEID�����ʹ�ܡ���ֹ	Tag��CID024
int ISBNCodeID_Set(sc_param_st *pParam);
int ISBNCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Industrial 25
****************************************************************************/
//����Industrial 25��Ĭ������		Tag:L25DEF
int IND25LoadDefault_Set(sc_param_st *pParam);
int IND25LoadDefault_Get(sc_param_st *pParam);

//����Industrial 25��ʹ�ܡ���ֹ	Tag��L25ENA
int IND25Enable_Set(sc_param_st *pParam);
int IND25Enable_Get(sc_param_st *pParam);

//����Industrial 25��ʶ����С����	Tag��L25MIN
int IND25MinLen_Set(sc_param_st *pParam);
int IND25MinLen_Get(sc_param_st *pParam);

//����Industrial 25��ʶ����󳤶�	Tag��L25MAX
int IND25MaxLen_Set(sc_param_st *pParam);
int IND25MaxLen_Get(sc_param_st *pParam);

//����Industrial 25У��λ��ʹ�ܡ���ֹ��У��λ��ѡ��	Tag��L25CHK
int IND25ChkTrsmt_Set(sc_param_st *pParam);
int IND25ChkTrsmt_Get(sc_param_st *pParam);

//����Industrial 25 CODEID�����ʹ�ܡ���ֹ	Tag��CID025
int IND25CodeID_Set(sc_param_st *pParam);
int IND25CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Standard 25
****************************************************************************/
//����Standard 25��Ĭ������		Tag:S25DEF
int STD25LoadDefault_Set(sc_param_st *pParam);
int STD25LoadDefault_Get(sc_param_st *pParam);

//����Standard 25��ʹ�ܡ���ֹ	Tag��S25ENA
int STD25Enable_Set(sc_param_st *pParam);
int STD25Enable_Get(sc_param_st *pParam);

//����Standard 25��ʶ����С����	Tag��S25MIN
int STD25MinLen_Set(sc_param_st *pParam);
int STD25MinLen_Get(sc_param_st *pParam);

//����Standard 25��ʶ����󳤶�	Tag��S25MAX
int STD25MaxLen_Set(sc_param_st *pParam);
int STD25MaxLen_Get(sc_param_st *pParam);

//����Standard 25У��λ��ʹ�ܡ���ֹ��У��λ��ѡ��	Tag��S25CHK
int STD25ChkTrsmt_Set(sc_param_st *pParam);
int STD25ChkTrsmt_Get(sc_param_st *pParam);

//����Standard 25 CODEID�����ʹ�ܡ���ֹ	Tag��CID026
int STD25CodeID_Set(sc_param_st *pParam);
int STD25CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Plessey
****************************************************************************/
//����Plessey ��Ĭ������		Tag:PLYDEF
int PlesseyLoadDefault_Set(sc_param_st *pParam);
int PlesseyLoadDefault_Get(sc_param_st *pParam);

//����Plessey ��ʹ�ܡ���ֹ	Tag��PLYENA
int PlesseyEnable_Set(sc_param_st *pParam);
int PlesseyEnable_Get(sc_param_st *pParam);

//����Plessey ��ʶ����С����	Tag��PLYMIN
int PlesseyMinLen_Set(sc_param_st *pParam);
int PlesseyMinLen_Get(sc_param_st *pParam);

//����Plessey ��ʶ����󳤶�	Tag��PLYMAX
int PlesseyMaxLen_Set(sc_param_st *pParam);
int PlesseyMaxLen_Get(sc_param_st *pParam);

//����Plessey У��λ��ʹ�ܡ���ֹ��У��λ�Ĵ���	Tag��PLYCHK
int PlesseyChkTrsmt_Set(sc_param_st *pParam);
int PlesseyChkTrsmt_Get(sc_param_st *pParam);

//����Plessey CODEID�����ʹ�ܡ���ֹ	Tag��CID027
int PlesseyCodeID_Set(sc_param_st *pParam);
int PlesseyCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@MSI Plessey
****************************************************************************/
//����MSI Plessey ��Ĭ������		Tag:MSIDEF
int MSIPlesseyLoadDefault_Set(sc_param_st *pParam);
int MSIPlesseyLoadDefault_Get(sc_param_st *pParam);

//����MSI Plessey ��ʹ�ܡ���ֹ	Tag��MSIENA
int MSIPlesseyEnable_Set(sc_param_st *pParam);
int MSIPlesseyEnable_Get(sc_param_st *pParam);

//����MSI Plessey ��ʶ����С����	Tag��MSIMIN
int MSIPlesseyMinLen_Set(sc_param_st *pParam);
int MSIPlesseyMinLen_Get(sc_param_st *pParam);

//����MSI Plessey ��ʶ����󳤶�	Tag��MSIMAX
int MSIPlesseyMaxLen_Set(sc_param_st *pParam);
int MSIPlesseyMaxLen_Get(sc_param_st *pParam);

//����MSI Plessey У��λ��ʹ�ܡ���ֹ��У��λ��ѡ��	Tag��MSICHK
int MSIPlesseyChkTrsmt_Set(sc_param_st *pParam);
int MSIPlesseyChkTrsmt_Get(sc_param_st *pParam);

//����MSI Plessey У��λ�Ƿ���	Tag��MSITCK
int MSIPlesseyChkTrsmtAction_Set(sc_param_st *pParam);
int MSIPlesseyChkTrsmtAction_Get(sc_param_st *pParam);

//����MSI Plessey CODEID�����ʹ�ܡ���ֹ	Tag��CID029
int MSIPlesseyCodeID_Set(sc_param_st *pParam);
int MSIPlesseyCodeID_Get(sc_param_st *pParam);


/****************************************************************************
*@ISSN
****************************************************************************/
//����ISSN��Ĭ������	Tag:ISSDEF
int ISSNLoadDefault_Set(sc_param_st *pParam);
int ISSNLoadDefault_Get(sc_param_st *pParam);

//����ISSN��ʹ�ܡ���ֹ	Tag��ISSENA
int ISSNEnable_Set(sc_param_st *pParam);
int ISSNEnable_Get(sc_param_st *pParam);

//����ISSN 2λ��չ���ʶ��ʹ�ܡ���ֹ	Tag:ISSAD2
int ISSNAddCode2Enable_Set(sc_param_st *pParam);
int ISSNAddCode2Enable_Get(sc_param_st *pParam);

//����ISSN 5λ��չ���ʶ��ʹ�ܡ���ֹ	Tag:ISSAD5
int ISSNAddCode5Enable_Set(sc_param_st *pParam);
int ISSNAddCode5Enable_Get(sc_param_st *pParam);

//����ISSN��չ���ʹ�ܡ���ֹ	Tag:ISSREQ
int ISSNAddedCodeEnable_Set(sc_param_st *pParam);
int ISSNAddedCodeEnable_Get(sc_param_st *pParam);

//����ISSN��չ���Ƿ���ӷָ���	Tag:ISSSEP
int ISSNSEPRInAddedCodeEnable_Set(sc_param_st *pParam);
int ISSNSEPRInAddedCodeEnable_Get(sc_param_st *pParam);

//����ISSN CODEID�����ʹ�ܡ���ֹ	Tag��CID023
int ISSNCodeID_Set(sc_param_st *pParam);
int ISSNCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@China Post 25
****************************************************************************/
//����China Post 25��Ĭ������	Tag:CHPDEF
int CP25LoadDefault_Set(sc_param_st *pParam);
int CP25LoadDefault_Get(sc_param_st *pParam);

//����China Post 25��ʹ�ܡ���ֹ	Tag��CHPENA
int CP25Enable_Set(sc_param_st *pParam);
int CP25Enable_Get(sc_param_st *pParam);

//����China Post 25 ��ʶ����С����	Tag��CHPMIN
int CP25MinLen_Set(sc_param_st *pParam);
int CP25MinLen_Get(sc_param_st *pParam);

//����China Post 25 ��ʶ����󳤶�	Tag��CHPMAX
int CP25MaxLen_Set(sc_param_st *pParam);
int CP25MaxLen_Get(sc_param_st *pParam);

//����China Post 25 У��λ��ʹ�ܡ���ֹ	Tag��CHPCHK
int CP25ChkTrsmt_Set(sc_param_st *pParam);
int CP25ChkTrsmt_Get(sc_param_st *pParam);

//����China Post 25 CODEID�����ʹ�ܡ���ֹ	Tag��CID019
int CP25CodeID_Set(sc_param_st *pParam);
int CP25CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@AIM 128
****************************************************************************/
//����AIM 128��Ĭ������	Tag:AIMDEF
int AIM128LoadDefault_Set(sc_param_st *pParam);
int AIM128LoadDefault_Get(sc_param_st *pParam);

//����AIM 128��ʹ�ܡ���ֹ	Tag��AIMENA
int AIM128Enable_Set(sc_param_st *pParam);
int AIM128Enable_Get(sc_param_st *pParam);

//����AIM 128��ʶ����С����	Tag��AIMMIN
int AIM128MinLen_Set(sc_param_st *pParam);
int AIM128MinLen_Get(sc_param_st *pParam);

//����AIM 128��ʶ����󳤶�	Tag��AIMMAX
int AIM128MaxLen_Set(sc_param_st *pParam);
int AIM128MaxLen_Get(sc_param_st *pParam);

//����AIM 128У��λ�ķ���ʹ�ܡ���ֹ	Tag��AIMCHK
int AIM128ChkTrsmt_Set(sc_param_st *pParam);
int AIM128ChkTrsmt_Get(sc_param_st *pParam);

//����AIM 128 FNC1��ʽ��ʹ�ܡ���ֹ	Tag��AIMFNC
int AIM128FNC1_Set(sc_param_st *pParam);
int AIM128FNC1_Get(sc_param_st *pParam);

//����AIM 128 CODEID�����ʹ�ܡ���ֹ	Tag��CID020
int AIM128CodeID_Set(sc_param_st *pParam);
int AIM128CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@ISBT 128
****************************************************************************/
//����ISBT 128��Ĭ������	Tag:IBTDEF
int ISBT128LoadDefault_Set(sc_param_st *pParam);
int ISBT128LoadDefault_Get(sc_param_st *pParam);

//����ISBT 128��ʹ�ܡ���ֹ	Tag��IBTENA
int ISBT128Enable_Set(sc_param_st *pParam);
int ISBT128Enable_Get(sc_param_st *pParam);

//����ISBT 128 CODEID�����ʹ�ܡ���ֹ	Tag��CID021
int ISBT128CodeID_Set(sc_param_st *pParam);
int ISBT128CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@COOP 25
****************************************************************************/
//����COOP 25��Ĭ������	Tag:COPDEF
int COOP25LoadDefault_Set(sc_param_st *pParam);
int COOP25LoadDefault_Get(sc_param_st *pParam);

//����COOP 25��ʹ�ܡ���ֹ	Tag��COPENA
int COOP25Enable_Set(sc_param_st *pParam);
int COOP25Enable_Get(sc_param_st *pParam);

//����COOP 25��ʶ����С����	Tag��COPMIN
int COOP25MinLen_Set(sc_param_st *pParam);
int COOP25MinLen_Get(sc_param_st *pParam);

//����COOP 25��ʶ����󳤶�	Tag��COPMAX
int COOP25MaxLen_Set(sc_param_st *pParam);
int COOP25MaxLen_Get(sc_param_st *pParam);

//����COOP 25У��λ�ķ���ʹ�ܡ���ֹ	Tag��COPCHK
int COOP25ChkTrsmt_Set(sc_param_st *pParam);
int COOP25ChkTrsmt_Get(sc_param_st *pParam);

//����COOP 25 CODEID�����ʹ�ܡ���ֹ	Tag��CID022
int COOP25CodeID_Set(sc_param_st *pParam);
int COOP25CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Deutsche 14
****************************************************************************/
//����Deutsche 14��Ĭ������	Tag:D14DEF
int Deut14LoadDefault_Set(sc_param_st *pParam);
int Deut14LoadDefault_Get(sc_param_st *pParam);

//����Deutsche 14��ʹ�ܡ���ֹ	Tag��D14ENA
int Deut14Enable_Set(sc_param_st *pParam);
int Deut14Enable_Get(sc_param_st *pParam);

//����Deutsche 14У��λ�ķ���ʹ�ܡ���ֹ	Tag��D14CHK
int Deut14ChkTrsmt_Set(sc_param_st *pParam);
int Deut14ChkTrsmt_Get(sc_param_st *pParam);

//����Deutsche 14 CODEID�����ʹ�ܡ���ֹ	Tag��CID128
int Deut14CodeID_Set(sc_param_st *pParam);
int Deut14CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Deutsche 12
****************************************************************************/
//����Deutsche 12��Ĭ������	Tag:D12DEF
int Deut12LoadDefault_Set(sc_param_st *pParam);
int Deut12LoadDefault_Get(sc_param_st *pParam);

//����Deutsche 12��ʹ�ܡ���ֹ	Tag��D12ENA
int Deut12Enable_Set(sc_param_st *pParam);
int Deut12Enable_Get(sc_param_st *pParam);

//����Deutsche 12У��λ�ķ���ʹ�ܡ���ֹ	Tag��D12CHK
int Deut12ChkTrsmt_Set(sc_param_st *pParam);
int Deut12ChkTrsmt_Get(sc_param_st *pParam);

//����Deutsche 12 CODEID�����ʹ�ܡ���ֹ	Tag��CID129
int Deut12CodeID_Set(sc_param_st *pParam);
int Deut12CodeID_Get(sc_param_st *pParam);


/****************************************************************************
*@Telepen
****************************************************************************/
//����Telepen ��Ĭ������	Tag:TLPDEF
int TelepenLoadDefault_Set(sc_param_st *pParam);
int TelepenLoadDefault_Get(sc_param_st *pParam);

//����Telepen ��ʹ�ܡ���ֹ	Tag��TLPENA
int TelepenEnable_Set(sc_param_st *pParam);
int TelepenEnable_Get(sc_param_st *pParam);

//����Telepen ��ʶ����С����	Tag��TLPMIN
int TelepenMinLen_Set(sc_param_st *pParam);
int TelepenMinLen_Get(sc_param_st *pParam);

//����Telepen ��ʶ����󳤶�	Tag��TLPMAX
int TelepenMaxLen_Set(sc_param_st *pParam);
int TelepenMaxLen_Get(sc_param_st *pParam);

//����Telepen У��λ�ķ���ʹ�ܡ���ֹ	Tag��TLPCHK
int TelepenChkTrsmt_Set(sc_param_st *pParam);
int TelepenChkTrsmt_Get(sc_param_st *pParam);

//����Telepen �Ľ��������ϣ�Full ASCII��Numeric Only	Tag��TLPASC
int TelepenFullASCIIWithNumericOnly_Set(sc_param_st *pParam);
int TelepenFullASCIIWithNumericOnly_Get(sc_param_st *pParam);

//����Telepen CODEID�����ʹ�ܡ���ֹ	Tag��CID130
int TelepenCodeID_Set(sc_param_st *pParam);
int TelepenCodeID_Get(sc_param_st *pParam);

/***************************************************************************
*@������
****************************************************************************/
//��պ껺������
int MacroCacheCleared_Set(sc_param_st *pParam);

//����������Ϣ����ķ��ͷ�ʽ
int MacroOutputMode1_Set(sc_param_st *pParam);
int MacroOutputMode2_Set(sc_param_st *pParam);
int MacroOutputMode3_Set(sc_param_st *pParam);
int MacroCacheCleared_Get(sc_param_st *pParam);

//����������Ϣ����ķ��ͷ�ʽ
int MacroOutputMode1_Get(sc_param_st *pParam);
int MacroOutputMode2_Get(sc_param_st *pParam);
int MacroOutputMode3_Get(sc_param_st *pParam);
/****************************************************************************
*@PDF417
****************************************************************************/
//����PDF417 ��Ĭ������	Tag:PDFDEF
int PDF417LoadDefault_Set(sc_param_st *pParam);
int PDF417LoadDefault_Get(sc_param_st *pParam);

//����PDF417 ��ʹ�ܡ���ֹ	Tag��PDFENA
int PDF417Enable_Set(sc_param_st *pParam);
int PDF417Enable_Get(sc_param_st *pParam);

//����PDF417 ��ʶ����С����	Tag��PDFMIN
int PDF417MinLen_Set(sc_param_st *pParam);
int PDF417MinLen_Get(sc_param_st *pParam);

//����PDF417 ��ʶ����󳤶�	Tag��PDFMAX
int PDF417MaxLen_Set(sc_param_st *pParam);
int PDF417MaxLen_Get(sc_param_st *pParam);

//����PDF417 �ĵ�˫�빦��	Tag��PDFDOU
int PDF417DecoMultiCode_Set(sc_param_st *pParam);
int PDF417DecoMultiCode_Get(sc_param_st *pParam);

//����PDF417 �ĵ�˫�������ʽ	Tag��PDFDOF
int PDF417OrderOfMultiCode_Set(sc_param_st *pParam);
int PDF417OrderOfMultiCode_Get(sc_param_st *pParam);

//����PDF417 ������������ʶ����ʹ�ܡ���ֹ	Tag��PDFINV
int PDF417PhaseOfCode_Set(sc_param_st *pParam);
int PDF417PhaseOfCode_Get(sc_param_st *pParam);

//����PDF417 ��ECI���ʹ�ܡ���ֹ	Tag��PDFECI
int PDF417OutputECI_Set(sc_param_st *pParam);
int PDF417OutputECI_Get(sc_param_st *pParam);

//����PDF417 ���ַ����뷽ʽ	Tag��PDFENC
int PDF417EncodingOfChar_Set(sc_param_st *pParam);
int PDF417EncodingOfChar_Get(sc_param_st *pParam);

//����PDF417 CODEID�����ʹ�ܡ���ֹ	Tag��CID032
int PDF417CodeID_Set(sc_param_st *pParam);
int PDF417CodeID_Get(sc_param_st *pParam);


/****************************************************************************
*@QR
****************************************************************************/
//����QR ��Ĭ������	Tag:QRCDEF
int QRLoadDefault_Set(sc_param_st *pParam);
int QRLoadDefault_Get(sc_param_st *pParam);

//����QR ��ʹ�ܡ���ֹ	Tag��QRCENA
int QREnable_Set(sc_param_st *pParam);
int QREnable_Get(sc_param_st *pParam);

//����QR ��ʶ����С����	Tag��QRCMIN
int QRMinLen_Set(sc_param_st *pParam);
int QRMinLen_Get(sc_param_st *pParam);

//����QR ��ʶ����󳤶�	Tag��QRCMAX
int QRMaxLen_Set(sc_param_st *pParam);
int QRMaxLen_Get(sc_param_st *pParam);

//����QR �ĵ�˫�빦��	Tag��QRCDOU
int QRDecoMultiCode_Set(sc_param_st *pParam);
int QRDecoMultiCode_Get(sc_param_st *pParam);

//����QR �ĵ�˫�������ʽ	Tag��QRCDOF
int QROrderOfMultiCode_Set(sc_param_st *pParam);
int QROrderOfMultiCode_Get(sc_param_st *pParam);

//����QR ��Micro QRʶ��ʹ�ܡ���ֹ	Tag��QRCMCR
int QRDecoMicroQR_Set(sc_param_st *pParam);
int QRDecoMicroQR_Get(sc_param_st *pParam);

//����QR ������������ʶ����ʹ�ܡ���ֹ	Tag��QRCINV
int QRPhaseOfCode_Set(sc_param_st *pParam);
int QRPhaseOfCode_Get(sc_param_st *pParam);

//����QR ��ECI���ʹ�ܡ���ֹ	Tag��QRCECI
int QROutputECI_Set(sc_param_st *pParam);
int QROutputECI_Get(sc_param_st *pParam);

//����QR ���ַ����뷽ʽ	Tag��QRCENC
int QREncodingOfChar_Set(sc_param_st *pParam);
int QREncodingOfChar_Get(sc_param_st *pParam);

//����QR CODEID�����ʹ�ܡ���ֹ	Tag��CID033
int QRCodeID_Set(sc_param_st *pParam);
int QRCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Aztec
****************************************************************************/
//����Aztec ��Ĭ������	Tag:AZTDEF
int AztecLoadDefault_Set(sc_param_st *pParam);
int AztecLoadDefault_Get(sc_param_st *pParam);

//����Aztec ��ʹ�ܡ���ֹ	Tag��AZTENA
int AztecEnable_Set(sc_param_st *pParam);
int AztecEnable_Get(sc_param_st *pParam);

//����Aztec ��ʶ����С����	Tag��AZTMIN
int AztecMinLen_Set(sc_param_st *pParam);
int AztecMinLen_Get(sc_param_st *pParam);

//����Aztec ��ʶ����󳤶�	Tag��AZTMAX
int AztecMaxLen_Set(sc_param_st *pParam);
int AztecMaxLen_Get(sc_param_st *pParam);

//����Aztec �Ķ���ͬͼ���������	Tag��AZTMUL
int AztecNumOfMultiCode_Set(sc_param_st *pParam);
int AztecNumOfMultiCode_Get(sc_param_st *pParam);

//����Aztec �Ķ���ͬͼ�Ĵ���ʽTag��AZTMAX
int AztecModeOfHandleMultiCode_Set(sc_param_st *pParam);
int AztecModeOfHandleMultiCode_Get(sc_param_st *pParam);

//����Aztec ��ECI���ʹ�ܡ���ֹ	Tag��AZTECI
int AztecOutputECI_Set(sc_param_st *pParam);
int AztecOutputECI_Get(sc_param_st *pParam);

//����Aztec ���ַ����뷽ʽ	Tag��AZTENC
int AztecEncodingOfChar_Set(sc_param_st *pParam);
int AztecEncodingOfChar_Get(sc_param_st *pParam);

//����Aztec CODEID�����ʹ�ܡ���ֹ	Tag��CID034
int AztecCodeID_Set(sc_param_st *pParam);
int AztecCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Data Matrix
****************************************************************************/
//����DM ��Ĭ������	Tag:DMCDEF
int DMLoadDefault_Set(sc_param_st *pParam);
int DMLoadDefault_Get(sc_param_st *pParam);

//����DM ��ʹ�ܡ���ֹ	Tag��DMCENA
int DMEnable_Set(sc_param_st *pParam);
int DMEnable_Get(sc_param_st *pParam);

//����DM ��ʶ����С����	Tag��DMCMIN
int DMMinLen_Set(sc_param_st *pParam);
int DMMinLen_Get(sc_param_st *pParam);

//����DM ��ʶ����󳤶�	Tag��DMCMAX
int DMMaxLen_Set(sc_param_st *pParam);
int DMMaxLen_Get(sc_param_st *pParam);

//����DM �ĵ�˫�빦��	Tag��DMCDOU
int DMDecoMultiCode_Set(sc_param_st *pParam);
int DMDecoMultiCode_Get(sc_param_st *pParam);

//����DM ��ʶ�������빦��	Tag��DMCREC
int DMDecoRectCode_Set(sc_param_st *pParam);
int DMDecoRectCode_Get(sc_param_st *pParam);

//����DM ������Ƿ������Ϣ���	Tag��DMCADD
int DMAddInfoWordsToDecoResult_Set(sc_param_st *pParam);
int DMAddInfoWordsToDecoResult_Get(sc_param_st *pParam);

//����DM ������������ʶ����ʹ�ܡ���ֹ	Tag��DMCINV
int DMPhaseOfCode_Set(sc_param_st *pParam);
int DMPhaseOfCode_Get(sc_param_st *pParam);

//����DM ��ECI���ʹ�ܡ���ֹ	Tag��DMCECI
int DMOutputECI_Set(sc_param_st *pParam);
int DMOutputECI_Get(sc_param_st *pParam);

//����DM ���ַ����뷽ʽ	Tag��DMCENC
int DMEncodingOfChar_Set(sc_param_st *pParam);
int DMEncodingOfChar_Get(sc_param_st *pParam);

//����DM CODEID�����ʹ�ܡ���ֹ	Tag��CID035
int DMCodeID_Set(sc_param_st *pParam);
int DMCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Maxicode
****************************************************************************/
//����Maxicode ��Ĭ������	Tag:MXCDEF
int MaxiLoadDefault_Set(sc_param_st *pParam);
int MaxiLoadDefault_Get(sc_param_st *pParam);

//����Maxicode ��ʹ�ܡ���ֹ	Tag��MXCENA
int MaxiEnable_Set(sc_param_st *pParam);
int MaxiEnable_Get(sc_param_st *pParam);

//����Maxicode ��ʶ����С����	Tag��MXCMIN
int MaxiMinLen_Set(sc_param_st *pParam);
int MaxiMinLen_Get(sc_param_st *pParam);

//����Maxicode ��ʶ����󳤶�	Tag��MXCMAX
int MaxiMaxLen_Set(sc_param_st *pParam);
int MaxiMaxLen_Get(sc_param_st *pParam);

//����Maxicode CODEID�����ʹ�ܡ���ֹ	Tag��CID036
int MaxiCodeID_Set(sc_param_st *pParam);
int MaxiCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@������ CSC
****************************************************************************/
//����CSC ��Ĭ������	Tag:CSCDEF
int CSCLoadDefault_Set(sc_param_st *pParam);
int CSCLoadDefault_Get(sc_param_st *pParam);

//����CSC ��ʹ�ܡ���ֹ	Tag��CSCENA
int CSCEnable_Set(sc_param_st *pParam);
int CSCEnable_Get(sc_param_st *pParam);

//����CSC ��ʶ����С����	Tag��CSCMIN
int CSCMinLen_Set(sc_param_st *pParam);
int CSCMinLen_Get(sc_param_st *pParam);

//����CSC ��ʶ����󳤶�	Tag��CSCMAX
int CSCMaxLen_Set(sc_param_st *pParam);
int CSCMaxLen_Get(sc_param_st *pParam);

//����CSC �ĵ�˫�빦��	Tag��CSCDOU
int CSCDecoMultiCode_Set(sc_param_st *pParam);
int CSCDecoMultiCode_Get(sc_param_st *pParam);

//����CSC ������������ʶ����ʹ�ܡ���ֹ	Tag��CSCINV
int CSCPhaseOfCode_Set(sc_param_st *pParam);
int CSCPhaseOfCode_Get(sc_param_st *pParam);

//����CSC CODEID�����ʹ�ܡ���ֹ	Tag��CID039
int CSCCodeID_Set(sc_param_st *pParam);
int CSCCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@GM��
****************************************************************************/
//����GM ��Ĭ������	Tag:GMCDEF
int GMLoadDefault_Set(sc_param_st *pParam);
int GMLoadDefault_Get(sc_param_st *pParam);

//����GM ��ʹ�ܡ���ֹ	Tag��GMCENA
int GMEnable_Set(sc_param_st *pParam);
int GMEnable_Get(sc_param_st *pParam);

//����GM ��ʶ����С����	Tag��GMCMIN
int GMMinLen_Set(sc_param_st *pParam);
int GMMinLen_Get(sc_param_st *pParam);

//����GM ��ʶ����󳤶�	Tag��GMCMAX
int GMMaxLen_Set(sc_param_st *pParam);
int GMMaxLen_Get(sc_param_st *pParam);

//����GM CODEID�����ʹ�ܡ���ֹ	Tag��CID041
int GMCodeID_Set(sc_param_st *pParam);
int GMCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@MicroPDF417��
****************************************************************************/
//����MicroPDF417 ��Ĭ������	Tag:MPDDEF
int MicroPDF417LoadDefault_Set(sc_param_st *pParam);
int MicroPDF417LoadDefault_Get(sc_param_st *pParam);

//����MicroPDF417 ��ʹ�ܡ���ֹ	Tag��MPDENA
int MicroPDF417Enable_Set(sc_param_st *pParam);
int MicroPDF417Enable_Get(sc_param_st *pParam);

//����MicroPDF417 ��ʶ����С����	Tag��MPDMIN
int MicroPDF417MinLen_Set(sc_param_st *pParam);
int MicroPDF417MinLen_Get(sc_param_st *pParam);

//����MicroPDF417 ��ʶ����󳤶�	Tag��MPDMAX
int MicroPDF417MaxLen_Set(sc_param_st *pParam);
int MicroPDF417MaxLen_Get(sc_param_st *pParam);

//����MicroPDF417 CODEID�����ʹ�ܡ���ֹ	Tag��CID042
int MicroPDF417CodeID_Set(sc_param_st *pParam);
int MicroPDF417CodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@MicroQR��
****************************************************************************/
//����MicroQR ��Ĭ������	Tag:MQRDEF
int MicroQRLoadDefault_Set(sc_param_st *pParam);
int MicroQRLoadDefault_Get(sc_param_st *pParam);

//����MicroQR ��ʹ�ܡ���ֹ	Tag��MQRENA
int MicroQREnable_Set(sc_param_st *pParam);
int MicroQREnable_Get(sc_param_st *pParam);

//����MicroQR ��ʶ����С����	Tag��MQRMIN
int MicroQRMinLen_Set(sc_param_st *pParam);
int MicroQRMinLen_Get(sc_param_st *pParam);

//����MicroQR ��ʶ����󳤶�	Tag��MQRMAX
int MicroQRMaxLen_Set(sc_param_st *pParam);
int MicroQRMaxLen_Get(sc_param_st *pParam);

//����MicroQR CODEID�����ʹ�ܡ���ֹ	Tag��CID043
int MicroQRCodeID_Set(sc_param_st *pParam);
int MicroQRCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Specific OCR-B
****************************************************************************/
//����Specific OCR-B ��Ĭ������	Tag:SOBDEF
int SpecOCRBLoadDefault_Set(sc_param_st *pParam);
int SpecOCRBLoadDefault_Get(sc_param_st *pParam);

//����Specific OCR-B ��ʹ�ܡ���ֹ	Tag��SOBENA
int SpecOCRBEnable_Set(sc_param_st *pParam);
int SpecOCRBEnable_Get(sc_param_st *pParam);

//����Specific OCR-B ��ʶ����С����	Tag��SOBMIN
int SpecOCRBMinLen_Set(sc_param_st *pParam);
int SpecOCRBMinLen_Get(sc_param_st *pParam);

//����Specific OCR-B ��ʶ����󳤶�	Tag��SOBMAX
int SpecOCRBMaxLen_Set(sc_param_st *pParam);
int SpecOCRBMaxLen_Get(sc_param_st *pParam);

/*Code ID �̶� ��ʱ���ṩ�ӿ�
//����Specific OCR-B CODEID�����ʹ�ܡ���ֹ	Tag��CID043
int SpecOCRBCodeID_Set(sc_param_st *pParam);
int SpecOCRBCodeID_Get(sc_param_st *pParam);
*/

/****************************************************************************
*@Passport OCR
****************************************************************************/
//����Passport OCR ��Ĭ������	Tag:PASDEF
int PassportOCRBLoadDefault_Set(sc_param_st *pParam);
int PassportOCRBLoadDefault_Get(sc_param_st *pParam);

//����Passport OCR ��ʹ�ܡ���ֹ	Tag��PASENA
int PassportOCRBEnable_Set(sc_param_st *pParam);
int PassportOCRBEnable_Get(sc_param_st *pParam);

/*Code ID �̶� ��ʱ���ṩ�ӿ�
//����Passport OCR CODEID�����ʹ�ܡ���ֹ	Tag��
int SpecOCRBCodeID_Set(sc_param_st *pParam);
int SpecOCRBCodeID_Get(sc_param_st *pParam);
*/

/****************************************************************************
*@USPS Postnet
****************************************************************************/
//����USPS Postnet ��Ĭ������	Tag:PNTDEF
int PostnetLoadDefault_Set(sc_param_st *pParam);
int PostnetLoadDefault_Get(sc_param_st *pParam);

//����USPS Postnet ��ʹ�ܡ���ֹ	Tag��PNTENA
int PostnetEnable_Set(sc_param_st *pParam);
int PostnetEnable_Get(sc_param_st *pParam);

//����USPS Postnet У��λ�ķ���ʹ�ܡ���ֹ	Tag��PNTCHK
int PostnetChkTrsmt_Set(sc_param_st *pParam);
int PostnetChkTrsmt_Get(sc_param_st *pParam);

//����USPS Postnet CODEID�����ʹ�ܡ���ֹ	Tag��CID096
int PostnetCodeID_Set(sc_param_st *pParam);
int PostnetCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@USPS Intelligent Mail
****************************************************************************/
//����USPS Intelligent Mail ��Ĭ������	Tag:ILGDEF
int InteMailLoadDefault_Set(sc_param_st *pParam);
int InteMailLoadDefault_Get(sc_param_st *pParam);

//����USPS Intelligent Mail ��ʹ�ܡ���ֹ	Tag��ILGENA
int InteMailEnable_Set(sc_param_st *pParam);
int InteMailEnable_Get(sc_param_st *pParam);

//����USPS Intelligent Mail CODEID�����ʹ�ܡ���ֹ	Tag��CID097
int InteMailCodeID_Set(sc_param_st *pParam);
int InteMailCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Royal Mail
****************************************************************************/
//����Royal Mail ��Ĭ������	Tag:ROYDEF
int RoyalMailLoadDefault_Set(sc_param_st *pParam);
int RoyalMailLoadDefault_Get(sc_param_st *pParam);

//����Royal Mail ��ʹ�ܡ���ֹ	Tag��ROYENA
int RoyalMailEnable_Set(sc_param_st *pParam);
int RoyalMailEnable_Get(sc_param_st *pParam);

//����Royal Mail �����ʹ�ܡ���ֹ	Tag��CID098
int RoyalMailCodeID_Set(sc_param_st *pParam);
int RoyalMailCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@USPS Planet
****************************************************************************/
//����USPS Planet ��Ĭ������	Tag:PLADEF
int PlanetLoadDefault_Set(sc_param_st *pParam);
int PlanetLoadDefault_Get(sc_param_st *pParam);

//����USPS Planet ��ʹ�ܡ���ֹ	Tag��PLAENA
int PlanetEnable_Set(sc_param_st *pParam);
int PlanetEnable_Get(sc_param_st *pParam);

//����USPS Planet У��λ�ķ���ʹ�ܡ���ֹ	Tag��PLACHK
int PlanetChkTrsmt_Set(sc_param_st *pParam);
int PlanetChkTrsmt_Get(sc_param_st *pParam);

//����USPS Planet CODEID�����ʹ�ܡ���ֹ	Tag��CID099
int PlanetCodeID_Set(sc_param_st *pParam);
int PlanetCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@KIX Post
****************************************************************************/
//����KIX Post ��Ĭ������	Tag:KIXDEF
int KIXPostLoadDefault_Set(sc_param_st *pParam);
int KIXPostLoadDefault_Get(sc_param_st *pParam);

//����KIX Post ��ʹ�ܡ���ֹ	Tag��KIXENA
int KIXPostEnable_Set(sc_param_st *pParam);
int KIXPostEnable_Get(sc_param_st *pParam);

//����KIX Post �����ʹ�ܡ���ֹ	Tag��CID100
int KIXPostCodeID_Set(sc_param_st *pParam);
int KIXPostCodeID_Get(sc_param_st *pParam);

/****************************************************************************
*@Australian Postal
****************************************************************************/
//����Australian Postal ��Ĭ������	Tag:APLDEF
int AUSPostalLoadDefault_Set(sc_param_st *pParam);
int AUSPostalLoadDefault_Get(sc_param_st *pParam);

//����Australian Postal ��ʹ�ܡ���ֹ	Tag��APLENA
int AUSPostalEnable_Set(sc_param_st *pParam);
int AUSPostalEnable_Get(sc_param_st *pParam);

//����Australian Postal �����ʹ�ܡ���ֹ	Tag��CID101
int AUSPostalCodeID_Set(sc_param_st *pParam);
int AUSPostalCodeID_Get(sc_param_st *pParam);

//ǰ��׺�ȸ�����Ϣ�ۺ����� APSENA
int AllFixMsg_Set(sc_param_st *pParam);
int AllFixMsg_Get(sc_param_st *pParam);

//ǰ׺˳������  PRESEQ
int PrefixOrder_Set(sc_param_st *pParam);
int PrefixOrder_Get(sc_param_st *pParam);

//ǰ׺ʹ������
int PrefixEnable_Set(sc_param_st *pParam);
int PrefixEnable_Get(sc_param_st *pParam);

//ǰ׺��Ϣ�༭
int PrefixEdit_Set(sc_param_st *pParam);
int PrefixEdit_Get(sc_param_st *pParam);
int PrefixLenth_Set(sc_param_st *pParam);
int PrefixLenth_Get(sc_param_st *pParam);


//AIMIDʹ������
int AIMIDEnable_Set(sc_param_st *pParam);
int AIMIDEnable_Get(sc_param_st *pParam);

//CodeID�ۺ�����
int CodeIDEnable_Set(sc_param_st *pParam);
int CodeIDEnable_Get(sc_param_st *pParam);
int AllCodeIDDefault_Set(sc_param_st *pParam);
int AllCodeIDDefault_Get(sc_param_st *pParam);
int CodeIDLenth_Set(sc_param_st *pParam);
int CodeIDLenth_Get(sc_param_st *pParam);
int CodeIDValueRange_Set(sc_param_st *pParam);
int CodeIDValueRange_Get(sc_param_st *pParam);

//��׺ʹ������
int SufixEnable_Set(sc_param_st *pParam);
int SufixEnable_Get(sc_param_st *pParam);

//��׺��Ϣ�༭
int SufixEdit_Set(sc_param_st *pParam);
int SufixEdit_Get(sc_param_st *pParam);
int SufixLenth_Set(sc_param_st *pParam);
int SufixLenth_Get(sc_param_st *pParam);

//������ʹ������
int TerminatEnable_Set(sc_param_st *pParam);
int TerminatEnable_Get(sc_param_st *pParam);

//��������Ϣ�༭
int TerminatEdit_Set(sc_param_st *pParam);
int TerminatEdit_Get(sc_param_st *pParam);
int TerminatLenth_Set(sc_param_st *pParam);
int TerminatLenth_Get(sc_param_st *pParam);


/****************************************************************************
*@Data Formatting ���ݸ�ʽ�༭
****************************************************************************/
//DataFormatter ��ʹ������		Tag��DFMENA
int DataFormatterEnable_Set(sc_param_st *pParam);
int DataFormatterEnable_Get(sc_param_st *pParam);

//����DataFormatter��Ĭ�����ݸ�ʽ 		Tag��DFMDEF
int LoadDefaultDataFormatType_Set(sc_param_st *pParam);
int LoadDefaultDataFormatType_Get(sc_param_st *pParam);

//����DataFormatter�����ݸ�ʽ�༭ 		Tag��DFMSET
int DataFormatEditorEnable_Set(sc_param_st *pParam);
int DataFormatEditorEnable_Get(sc_param_st *pParam);

//���ĳ��DataFormatter���ݸ�ʽ���� 	Tag��DFMCLR
int ClearDataFormatTypes_Set(sc_param_st *pParam);
int ClearDataFormatTypes_Get(sc_param_st *pParam);

//�������DataFormatter���ݸ�ʽ���� 	Tag��DFMCAL
int ClearAllDataFormatTypes_Set(sc_param_st *pParam);
int ClearAllDataFormatTypes_Get(sc_param_st *pParam);

//����ѡ�е�DataFormatter���ݸ�ʽ 		Tag��DFMUSE
int LoadDataFormatTypes_Set(sc_param_st *pParam);
int LoadDataFormatTypes_Get(sc_param_st *pParam);


//����ѡ�еĵ��ζ���DataFormatter���ݸ�ʽ 		Tag��DFMSIN
int LoadDataFormatTypesForSingleDeco_Set(sc_param_st *pParam);
int LoadDataFormatTypesForSingleDeco_Get(sc_param_st *pParam);

//���ݸ�ʽ��ƥ��Ĵ�����ʾ��		Tag��DFMTON
int BeepOnDataFormatNotMatchEnable_Set(sc_param_st *pParam);
int BeepOnDataFormatNotMatchEnable_Get(sc_param_st *pParam);

//��ѯ��ǰ�����ݸ�ʽ				Tag��DFMQCU
int CurrentDataFormatType_Set(sc_param_st *pParam);
int CurrentDataFormatType_Get(sc_param_st *pParam);

//��ѯ����Ĭ�ϵ����ݸ�ʽ			Tag��DFMQFA
int DefaultFactoryDataFormatType_Set(sc_param_st *pParam);
int DefaultFactoryDataFormatType_Get(sc_param_st *pParam);

//��ѯ����Data Format֧�ֵ���󳤶�	Tag��DFMQSZ
int MaxLenOfDataFormatTypes_Get(sc_param_st *pParam);
int MaxLenOfDataFormatTypes_Set(sc_param_st *pParam);
/****************************************************************************
*@���ս�������
****************************************************************************/
//�������ս�����������		Tag��DLPSET
int EnableAnalyzeDrivingLic_Set(sc_param_st *pParam);
int EnableAnalyzeDrivingLic_Get(sc_param_st *pParam);

//��ֹ���ս������ܣ���ɾ������		Tag��DLPCLR
int DisableAnalyzeDrivingLic_Set(sc_param_st *pParam);

//��ѯ���ս�������֧�ֵ�ָ����󳤶�		Tag��DLPQSZ
int MaxLenOfCommandInAnalDrivingLic_Get(sc_param_st *pParam);

/****************************************************************************
*@���ݴ������
****************************************************************************/
//�������ݴ��	Tag��PACKAG
int PackageForDecoInfo_Set(sc_param_st *pParam);
int PackageForDecoInfo_Get(sc_param_st *pParam);

/****************************************************************************
*@��Ϣ��ȡ����
****************************************************************************/
//��Ϣ��ȡ��ʹ������	Tag��EXTRAC
int CuttingoutDecoInfoEnable_Set(sc_param_st *pParam);
int CuttingoutDecoInfoEnable_Get(sc_param_st *pParam);

//�����Ϣ��ȡ����		Tag:EXTADD
int AddConfigForCuttingoutDecoInfo_Set(sc_param_st *pParam);
int AddConfigForCuttingoutDecoInfo_Get(sc_param_st *pParam);

//���ĳ������Ľ�ȡ����		Tag:EXTCLR
int ClearCuttingoutForCodeTypes_Set(sc_param_st *pParam);
int ClearCuttingoutForCodeTypes_Get(sc_param_st *pParam);

//�������һ��Ľ�ȡ����		Tag:EXTCLO
int ClearLatestCFGOfCuttingout_Set(sc_param_st *pParam);
int ClearLatestCFGOfCuttingout_Get(sc_param_st *pParam);

//������еĽ�ȡ����		Tag:EXTCAL
int ClearAllCFGOfCuttingout_Set(sc_param_st *pParam);
int ClearAllCFGOfCuttingout_Get(sc_param_st *pParam);

/****************************************************************************
*@��ȡ�豸�ֱ���
****************************************************************************/
//�豸�ֱ��ʵĲ�ѯ
int ResolutionRatio_Set(sc_param_st *pParam);
int ResolutionRatio_Get(sc_param_st *pParam);

/****************************************************************************
*@��ȡͼ��
****************************************************************************/
//��ȡͼ��
int FetchingImage_Set(sc_param_st *pParam);
int FetchingImage_Get(sc_param_st *pParam);

/****************************************************************************
*@��Ʒ��Ϣ
****************************************************************************/
//���ò�ƷS/N��		Tag:PSNEDT
int SettingProductSN_Set(sc_param_st *pParam);
int SettingProductSN_Get(sc_param_st *pParam);

//ɾ����ƷS/N��		Tag:PSNDEL
int DeleteProductSN_Set(sc_param_st *pParam);
int DeleteProductSN_Get(sc_param_st *pParam);

//��ѯ��ƷS/N��֧�ֵĳ��� Tag:PSNQSZ
int ProductSNLength_Set(sc_param_st *pParam);
int ProductSNLength_Get(sc_param_st *pParam);

//���ò�ƷESN��		Tag:ESNSET
int SettingProductESN_Set(sc_param_st *pParam);
int SettingProductESN_Get(sc_param_st *pParam);

//ɾ����ƷESN��		Tag:ESNDEL
int DeleteProductESN_Set(sc_param_st *pParam);
int DeleteProductESN_Get(sc_param_st *pParam);

//��ѯ��ƷESN��֧�ֵĳ��� Tag:ESNQSZ
int ProductESNLength_Set(sc_param_st *pParam);
int ProductESNLength_Get(sc_param_st *pParam);

//���ò�Ʒ��������		Tag:DATEDT
int SettingProductDate_Set(sc_param_st *pParam);
int SettingProductDate_Get(sc_param_st *pParam);

//ɾ����Ʒ��������		Tag:DATDEL
int DeleteProductDate_Set(sc_param_st *pParam);
int DeleteProductDate_Get(sc_param_st *pParam);

//��ѯ��Ʒ��������֧�ֵĳ��� Tag:DATQSZ
int ProductDateLength_Set(sc_param_st *pParam);
int ProductDateLength_Get(sc_param_st *pParam);

//���ò�Ʒ����		Tag:PDNEDT
int SettingProductName_Set(sc_param_st *pParam);
int SettingProductName_Get(sc_param_st *pParam);

//ɾ����Ʒ����		Tag:PDNDEL
int DeleteProductName_Set(sc_param_st *pParam);
int DeleteProductName_Get(sc_param_st *pParam);

//��ѯ��Ʒ����֧�ֵĳ��� Tag:PDNQSZ
int ProductNameLength_Set(sc_param_st *pParam);
int ProductNameLength_Get(sc_param_st *pParam);

//Is Download ʹ������
int IsDownloadEnable_Set(sc_param_st *pParam);
int IsDownloadEnable_Get(sc_param_st *pParam);

//ɾ��IS Download����
int DeleteCFGOfIsDownload_Set(sc_param_st *pParam);
int DeleteCFGOfIsDownload_Get(sc_param_st *pParam);

//��������������		Tag:MFNEDT
int SettingManufacturerName_Set(sc_param_st *pParam);
int SettingManufacturerName_Get(sc_param_st *pParam);

//ɾ������������		Tag:MFNDEL
int DeleteManufacturerName_Set(sc_param_st *pParam);
int DeleteManufacturerName_Get(sc_param_st *pParam);

//��ѯ��Ʒ����������֧�ֵĳ��� Tag:MFNQSZ
int ManufacturerNameLength_Set(sc_param_st *pParam);
int ManufacturerNameLength_Get(sc_param_st *pParam);

//���ò�ƷӲ����Ϣ		Tag:HWVEDT
int SettingInfoOfHardware_Set(sc_param_st *pParam);
int SettingInfoOfHardware_Get(sc_param_st *pParam);

//ɾ����ƷӲ����Ϣ		Tag:HWVDEL
int DeleteInfoOfHardware_Set(sc_param_st *pParam);
int DeleteInfoOfHardware_Get(sc_param_st *pParam);

//��ѯ��ƷӲ����Ϣ֧�ֵĳ��� Tag:HWVQSZ
int InfoOfHardwareLength_Set(sc_param_st *pParam);
int InfoOfHardwareLength_Get(sc_param_st *pParam);

//���ò�Ʒ��ʵ����		Tag:PRNEDT
int SettingRealNameOfProduct_Set(sc_param_st *pParam);
int SettingRealNameOfProduct_Get(sc_param_st *pParam);

//ɾ����Ʒ��ʵ����		Tag:PRNDEL
int DeleteRealNameOfProduct_Set(sc_param_st *pParam);
int DeleteRealNameOfProduct_Get(sc_param_st *pParam);

//��ѯ��Ʒ��ʵ����֧�ֵĳ��� Tag:PRNQSZ
int RealNameOfProductLength_Set(sc_param_st *pParam);
int RealNameOfProductLength_Get(sc_param_st *pParam);

//��ѯ��Ʒ��ʵ����		Tag:PRQRY
int QueryRealNameOfProduct_Set(sc_param_st *pParam);
int QueryRealNameOfProduct_Get(sc_param_st *pParam);

/****************************************************************************
*@��������
****************************************************************************/
//�����С�Զ���־��������			Tag:DOFLVL
int DepthOfFieldInProduct_Set(sc_param_st *pParam);
int DepthOfFieldInProduct_Get(sc_param_st *pParam);

//ɾ���������ò���					Tag��DELDOF
int DelDepthOfFieldInProduct_Set(sc_param_st *pParam);
int DelDepthOfFieldInProduct_Get(sc_param_st *pParam);

/****************************************************************************
*@MCU����Ƶ��		
****************************************************************************/
//����MCU������Ƶ��		Tag��HWRFRQ
int RunningFreqOfMCU_Set(sc_param_st *pParam);
int RunningFreqOfMCU_Get(sc_param_st *pParam);

//��������ģʽ		Tag��RUNWEL
int RunningWirelessMode_Set(sc_param_st *pParam);
int RunningWirelessMode_Get(sc_param_st *pParam);

//����USBö��PID--SNʱ�Ƿ�ʹ���豸SN	Tag��PIDUSN
int USBEnumratePIDSNWithSN_Set(sc_param_st *pParam);
int USBEnumratePIDSNWithSN_Get(sc_param_st *pParam);

//USB�ӿ��봮�ڵ��л���ʽ����ָ��Ҫ���л�������Ӧ��	Tag��AUTOUR
int AutoAdaptBetweenSerialCommAndUSB_Set(sc_param_st *pParam);
int AutoAdaptBetweenSerialCommAndUSB_Get(sc_param_st *pParam);

//��ѯ�豸ָ����ܻ����С		Tag��BUFQRY
int QueryBuffSizeOfInstructions_Set(sc_param_st *pParam);
int QueryBuffSizeOfInstructions_Get(sc_param_st *pParam);

//��Ʒ��Ϣ���ò�ѯPSN
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

//����
int QueryCPUID_Set(sc_param_st *pParam);
int QueryCPUID_Get(sc_param_st *pParam);
int Encryption_Set(sc_param_st *pParam);
int Encryption_Get(sc_param_st *pParam);
/************************************************************************/
#ifdef __cplusplus
}
#endif
#endif //__SC_EXTFUNC_H__

