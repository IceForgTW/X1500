#ifndef __SC_FNDEFINE_H__
#define __SC_FNDEFINE_H__

#include "SC_ExtFunc.h"

#ifdef __cplusplus
extern"C" {
#endif
/************************************************************************/
//#ifndef SC_DEBUG 
            #define  SC_FACDEF_SET       LoadFactoryDefaultCFG_Set 
            #define  SC_CUSDEF_SET       LoadUserDefaultCFG_Set
            #define  SC_CUSSAV_SET       SaveUserDefaultCFG_Set
            #define  SC_CUSDEL_SET       DeleteUserDefaultCFG_Set
            #define  SC_SAVEFY_SET       SaveFactoryDefaultCFG_Set
            #define  SC_GENDEF_SET       LoadGeneralDefaultCFG_Set

            #define  SC_FACDEF_GET       LoadFactoryDefaultCFG_Get 
            #define  SC_CUSDEF_GET       LoadUserDefaultCFG_Get
            #define  SC_CUSSAV_GET       SaveUserDefaultCFG_Get
            #define  SC_CUSDEL_GET       DeleteUserDefaultCFG_Get
            #define  SC_SAVEFY_GET       SaveFactoryDefaultCFG_Get
            #define  SC_GENDEF_GET       LoadGeneralDefaultCFG_Get
            
            #define  SC_SETUPE_SET       StartExitSetup_Set 
            #define  SC_SETUPN_SET       OpenCloseSetup_Set 
            #define  SC_SETUPT_SET       TrsmtSettingCode_Set 
            #define  SC_FOBSET_SET       EnableSettingCode_Set 
            #define  SC_FOBQRY_SET      QuerySettingCode_Set 
            #define  SC_SETUPE_GET       StartExitSetup_Get 
            #define  SC_SETUPN_GET       OpenCloseSetup_Get 
            #define  SC_SETUPT_GET       TrsmtSettingCode_Get 
            #define  SC_FOBSET_GET       EnableSettingCode_Get 
            #define  SC_FOBQRY_GET      QuerySettingCode_Get 

            #define  SC_SCNMOD_SET       WorkMode_Set 
            #define  SC_SCNTRG_SET       Trigger2DecoBySerialCommand_Set 
            #define  SC_SCNCTM_SET       PureSoftTrig_Set 
            #define  SC_SCNINV_SET       RestTime_Set 
            
            #define  SC_SCNMOD_GET       WorkMode_Get 
            #define  SC_SCNTRG_GET       Trigger2DecoBySerialCommand_Get
            #define  SC_SCNCTM_GET       PureSoftTrig_Get
            #define  SC_SCNINV_GET       RestTime_Get

            #define  SC_ORTSET_SET       ReadTime_Set 

            #define  SC_ORTSET_GET       ReadTime_Get

            #define  SC_RRDENA_SET       RereadDelayEnable_Set 
            #define  SC_RRDDUR_SET       RereadDelayTime_Set

            #define  SC_RRDENA_GET       RereadDelayEnable_Get
            #define  SC_RRDDUR_GET       RereadDelayTime_Get

			#define  SC_FONDUR_SET       FlashOnTime_Set
			#define  SC_FOFDUR_SET       FlashOffTime_Set

			#define  SC_FONDUR_GET       FlashOnTime_Get
			#define  SC_FOFDUR_GET       FlashOffTime_Get

            #define  SC_INTERF_SET       CommunicationInterface_Set
            #define  SC_INF232_SET       SC_NullSet
            #define  SC_INFDPP_SET       SC_NullSet
            #define  SC_INFUCM_SET       SC_NullSet
            #define  SC_INFPS2_SET       SC_NullSet
            #define  SC_INFKBW_SET       SC_NullSet
            #define  SC_INFPOS_SET       SC_NullSet
            #define  SC_INFIPH_SET       SC_NullSet
            #define  SC_INFIPT_SET       SC_NullSet
            #define  SC_INFCDC_SET       SC_NullSet
            #define  SC_INFBSP_SET       SC_NullSet
            #define  SC_INFBHD_SET       SC_NullSet

            #define  SC_INTERF_GET       CommunicationInterface_Get
            #define  SC_INF232_GET       SC_NullSet
            #define  SC_INFDPP_GET       SC_NullSet
            #define  SC_INFUCM_GET       SC_NullSet
            #define  SC_INFPS2_GET       SC_NullSet
            #define  SC_INFKBW_GET       SC_NullSet
            #define  SC_INFPOS_GET       SC_NullSet
            #define  SC_INFIPH_GET       SC_NullSet
            #define  SC_INFIPT_GET       SC_NullSet
            #define  SC_INFCDC_GET       SC_NullSet
            #define  SC_INFBSP_GET       SC_NullSet
            #define  SC_INFBHD_GET       SC_NullSet

            #define  SC_232BAD_SET       RS232Baudrate_Set 
            #define  SC_232PAR_SET       RS232Parity_Set
            #define  SC_232STP_SET       RS232StopBits_Set 
            #define  SC_232DAT_SET       RS232DataBits_Set 
            #define  SC_232AFL_SET       RS232HardwereAutoFlowCTL_Set 
            #define  SC_232FLW_SET       RS232NoAutoFlowCTL_Set 
            
            #define  SC_232BAD_GET       RS232Baudrate_Get 
            #define  SC_232PAR_GET       RS232Parity_Get 
            #define  SC_232STP_GET       RS232StopBits_Get 
            #define  SC_232DAT_GET       RS232DataBits_Get 
            #define  SC_232AFL_GET       RS232HardwereAutoFlowCTL_Get 
            #define  SC_232FLW_GET       RS232NoAutoFlowCTL_Get 

            #define  SC_KBWCTY_SET       KBWCountryLayout_Set
            #define  SC_KBWBUC_SET       KBWBeepOnUnknownChar_Set
            #define  SC_KBWALT_SET       KBWEmulateAltKeypad1_Set
            #define  SC_KBWALK_SET       KBWEmulateAltKeypad2_Set
            #define  SC_KBWFKM_SET       KBWCtrlPlusStatus_Set
            #define  SC_KBWDLY_SET       KBWInterKeyStrokeDelay_Set
            #define  SC_KBWCAP_SET       KBWCapsLockStatus_Set
            #define  SC_KBWCAS_SET       KBWConvertCaseStatus_Set
            #define  SC_KBWNUM_SET       KBWEmulateNUMPADType_Set
            #define  SC_KBWNCH_SET       KBWEmulateNUMPADOperatorStatus_Set
            #define  SC_KBWFAS_SET       KBWFastMode_Set
            #define  SC_KBWPOR_SET       KBWPollRate_Set
            #define  SC_KBWCPG_SET       KBWCodePage_Set

            #define  SC_KBWCTY_GET       KBWCountryLayout_Get
            #define  SC_KBWBUC_GET       KBWBeepOnUnknownChar_Get
            #define  SC_KBWALT_GET       KBWEmulateAltKeypad1_Get
            #define  SC_KBWALK_GET       KBWEmulateAltKeypad2_Get
            #define  SC_KBWFKM_GET       KBWCtrlPlusStatus_Get
            #define  SC_KBWDLY_GET       KBWInterKeyStrokeDelay_Get
            #define  SC_KBWCAP_GET       KBWCapsLockStatus_Get
            #define  SC_KBWCAS_GET       KBWConvertCaseStatus_Get
            #define  SC_KBWNUM_GET       KBWEmulateNUMPADType_Get
            #define  SC_KBWNCH_GET       KBWEmulateNUMPADOperatorStatus_Get
            #define  SC_KBWFAS_GET       KBWFastMode_Get
            #define  SC_KBWPOR_GET       KBWPollRate_Get
            #define  SC_KBWCPG_GET       KBWCodePage_Get

            #define  SC_PS2SLV_SET       PS2KeyboardConect_Set
            #define  SC_PS2CLK_SET       PS2KeyboardClock_Set

            #define  SC_PS2SLV_GET       PS2KeyboardConect_Get
            #define  SC_PS2CLK_GET       PS2KeyboardClock_Get

            #define  SC_PWBENA_SET       PowerOnVoice_Set

            #define  SC_PWBENA_GET       PowerOnVoice_Get

            #define  SC_GRBENA_SET       GoodReadBeep_Set
            #define  SC_GRBVOL_SET       GoodReadBeepVolume_Set
            #define  SC_GRBDUR_SET       GoodReadBeepDuration_Set
            #define  SC_GRBFRQ_SET       GoodReadBeepFreq_Set
            #define  SC_GRBNUM_SET       GoodReadBeepNumber_Set
            #define  SC_GRBITV_SET       GoodReadBeepIntval_Set

            #define  SC_GRBENA_GET       GoodReadBeep_Get
            #define  SC_GRBVOL_GET       GoodReadBeepVolume_Get
            #define  SC_GRBDUR_GET       GoodReadBeepDuration_Get
            #define  SC_GRBFRQ_GET       GoodReadBeepFreq_Get
            #define  SC_GRBNUM_GET       GoodReadBeepNumber_Get
            #define  SC_GRBITV_GET       GoodReadBeepIntval_Get

            #define  SC_BRBENA_SET       BadReadBeepEnable_Set
            #define  SC_BRBVOL_SET       BadReadBeepVolume_Set
            #define  SC_BRBDUR_SET       BadReadBeepDuration_Set
            #define  SC_BRBFRQ_SET       BadReadBeepFreq_Set
            #define  SC_BRBNUM_SET       BadReadBeepNumber_Set
            #define  SC_BRBITV_SET       BadReadBeepIntval_Set

            #define  SC_BRBENA_GET       BadReadBeepEnable_Get
            #define  SC_BRBVOL_GET       BadReadBeepVolume_Get
            #define  SC_BRBDUR_GET       BadReadBeepDuration_Get
            #define  SC_BRBFRQ_GET       BadReadBeepFreq_Get
            #define  SC_BRBNUM_GET       BadReadBeepNumber_Get
            #define  SC_BRBITV_GET       BadReadBeepIntval_Get

            #define  SC_VOICEP_SET       VoiceEnable_Set
            #define  SC_VOICEI_SET       VoiceIntvalTime_Set
            #define  SC_VOICET_SET       VoiceTypeSelect_Set
            #define  SC_VOICEM_SET       VoiceModeSelect_Set
            #define  SC_VOICEV_SET       VoiceVolume_Set

            #define  SC_VOICEP_GET       VoiceEnable_Get
            #define  SC_VOICEI_GET       VoiceIntvalTime_Get
            #define  SC_VOICET_GET       VoiceTypeSelect_Get
            #define  SC_VOICEM_GET       VoiceModeSelect_Get
            #define  SC_VOICEV_GET       VoiceVolume_Get

            #define  SC_GRLENA_SET       GoodReadLedEnable_Set
            #define  SC_GRLDUR_SET       GoodReadLedDuration_Set

            #define  SC_GRLENA_GET       GoodReadLedEnable_Get
            #define  SC_GRLDUR_GET       GoodReadLedDuration_Get

            #define  SC_GRVENA_SET       GoodReadVibrateEnable_Set
            #define  SC_GRVDUR_SET       GoodReadVibrateDuration_Set

            #define  SC_GRVENA_GET       GoodReadVibrateEnable_Get
            #define  SC_GRVDUR_GET       GoodReadVibrateDuration_Get

            #define  SC_POLENA_SET       PowerOnLedEnable_Set
            
            #define  SC_POLENA_GET       PowerOnLedEnable_Get

            #define  SC_ILLSCN_SET       IlluminationEnable_Set
            #define  SC_ILLCLR_SET       ColorTypeOfIllumination_Set
            #define  SC_ILLPST_SET       PositionOfIllumination_Set
            
            #define  SC_ILLSCN_GET       IlluminationEnable_Get
            #define  SC_ILLCLR_GET       ColorTypeOfIllumination_Get
            #define  SC_ILLPST_GET       PositionOfIllumination_Get

            #define  SC_AMLENA_SET       AimerEnable_Set

            #define  SC_AMLENA_GET       AimerEnable_Get

            #define  SC_SENLVL_SET       Sensitivity_Set
            #define  SC_SENAGR_SET       StatusAfterGoodReadInSenseMode_Set
            #define  SC_SENIST_SET       ImgStabTimeout_Set
            #define  SC_SENILL_SET       IlluStatusInSensingForSenseMode_Set

            #define  SC_SENLVL_GET       Sensitivity_Get
            #define  SC_SENAGR_GET       StatusAfterGoodReadInSenseMode_Get
            #define  SC_SENIST_GET       ImgStabTimeout_Get
            #define  SC_SENILL_GET       IlluStatusInSensingForSenseMode_Get

            #define  SC_EXPLVL_SET       ExposureTypes_Set

            #define  SC_EXPLVL_GET       ExposureTypes_Get

            #define  SC_CADENA_SET       DecoArea_Set
            #define  SC_CADTOP_SET       TopRangeOfCentralArea_Set
            #define  SC_CADBOT_SET       BottomRangeOfCentralArea_Set
            #define  SC_CADLEF_SET       LeftRangeOfCentralArea_Set
            #define  SC_CADRIG_SET       RightRangeOfCentralArea_Set

            #define  SC_CADENA_GET       DecoArea_Get
            #define  SC_CADTOP_GET       TopRangeOfCentralArea_Get
            #define  SC_CADBOT_GET       BottomRangeOfCentralArea_Get
            #define  SC_CADLEF_GET       LeftRangeOfCentralArea_Get
            #define  SC_CADRIG_GET       RightRangeOfCentralArea_Get

            #define  SC_CDALVL_SET       DecoAreaLevelsFor1D_Set
            #define  SC_CDAINV_SET       IntervalTimeOfOutputingFor1D_Set

            #define  SC_CDALVL_GET       DecoAreaLevelsFor1D_Get
            #define  SC_CDAINV_GET       IntervalTimeOfOutputingFor1D_Get

            #define  SC_NGRENA_SET       NotGoodReadEnable_Set
            #define  SC_NGRSET_SET       NotGoodReadMsg_Set
            #define  SC_NGRQRY_SET       NotGoodReadLenth_Set

            #define  SC_NGRENA_GET       NotGoodReadEnable_Get
            #define  SC_NGRSET_GET       NotGoodReadMsg_Get
            #define  SC_NGRQRY_GET       NotGoodReadLenth_Get

            #define  SC_MIRROR_SET       ImageMirrorTypes_Set

            #define  SC_MIRROR_GET       ImageMirrorTypes_Get

            #define  SC_PWROFF_SET       ModePowerOff_Set
            #define  SC_PWRIDL_SET       ModeIdle_Set
            #define  SC_PWRSLP_SET       ModeSleep_Set
            #define  SC_PWRDSP_SET       SC_NullSet

			#define  SC_PWROFF_GET       ModePowerOff_Get
			#define  SC_PWRIDL_GET       ModeIdle_Get
			#define  SC_PWRSLP_GET       ModeSleep_Get
			#define  SC_PWRDSP_GET       SC_NullGet

            #define  SC_ATIDLE_SET       AutoSleepEnable_Set
            #define  SC_ATIDUR_SET       AutoSleepIdleTime_Set

            #define  SC_ATIDLE_GET       AutoSleepEnable_Get
            #define  SC_ATIDUR_GET       AutoSleepIdleTime_Get

            #define  SC_TESTMD_SET       ModeTest_Set

			#define  SC_TESTMD_GET       ModeTest_Get

            #define  SC_REBOOT_SET       ModeReboot_Set

			#define  SC_REBOOT_GET       ModeReboot_Get

            #define  SC_SAFLVL_SET       SafeLv_Set

            #define  SC_SAFLVL_GET       SafeLv_Get

            #define  SC_ALLENA_SET       AllCodeTypeEnable_Set
            #define  SC_ALL1DC_SET       CodeType1DEnable_Set
            #define  SC_ALL2DC_SET       CodeType2DEnable_Set
            #define  SC_ALLPST_SET       CodeTypePostCodeEnable_Set
            #define  SC_ALLINV_SET       InverterOfCodesEnable_Set
            #define  SC_ALLADD_SET       AllEanUpcAddon_Set

            #define  SC_ALLENA_GET       AllCodeTypeEnable_Get
            #define  SC_ALL1DC_GET       CodeType1DEnable_Get
            #define  SC_ALL2DC_GET       CodeType2DEnable_Get
            #define  SC_ALLPST_GET       CodeTypePostCodeEnable_Get
            #define  SC_ALLINV_GET       InverterOfCodesEnable_Get
            #define  SC_ALLADD_GET       AllEanUpcAddon_Get

            #define  SC_A1DDOU_SET       DualCodeTypesFor1D_Set

			#define  SC_A1DDOU_GET       DualCodeTypesFor1D_Get

			#define  SC_FEBSEN_SET	     FebrabanInterCharDelayEnable_Set
			#define  SC_FEBSDT_SET	     FebrabanInterCharDelay_Set
			#define  SC_FEBMEN_SET	     FebrabanInterAreaDelayEnable_Set
			#define  SC_FEBMDT_SET	     FebrabanInterAreaDelay_Set

			#define  SC_FEBSEN_GET	     FebrabanInterCharDelayEnable_Get
			#define  SC_FEBSDT_GET	     FebrabanInterCharDelay_Get
			#define  SC_FEBMEN_GET	     FebrabanInterAreaDelayEnable_Get
			#define  SC_FEBMDT_GET	     FebrabanInterAreaDelay_Get

            #define  SC_APSENA_SET       AllFixMsg_Set

            #define  SC_APSENA_GET       AllFixMsg_Get

            #define  SC_PRESEQ_SET       PrefixOrder_Set

            #define  SC_PRESEQ_GET       PrefixOrder_Get

            #define  SC_CPRENA_SET       PrefixEnable_Set
            #define  SC_CPRSET_SET       PrefixEdit_Set
            #define  SC_CPRQSZ_SET       PrefixLenth_Set

            #define  SC_CPRENA_GET       PrefixEnable_Get
            #define  SC_CPRSET_GET       PrefixEdit_Get
            #define  SC_CPRQSZ_GET       PrefixLenth_Get

            #define  SC_AIDENA_SET       AIMIDEnable_Set

            #define  SC_AIDENA_GET       AIMIDEnable_Get

            #define  SC_CIDENA_SET       CodeIDEnable_Set
            #define  SC_CIDDEF_SET       AllCodeIDDefault_Set
            #define  SC_CIDQSZ_SET       CodeIDLenth_Set
            #define  SC_CIDQVL_SET       CodeIDValueRange_Set
            #define  SC_CID002_SET       Code128CodeID_Set
            #define  SC_CID003_SET       GS1_128CodeID_Set
            #define  SC_CID004_SET       EAN8CodeID_Set
            #define  SC_CID005_SET       EAN13CodeID_Set
            #define  SC_CID006_SET       UPCECodeID_Set
            #define  SC_CID007_SET       UPCACodeID_Set
            #define  SC_CID008_SET       Inter2of5CodeID_Set
            #define  SC_CID009_SET       ITF14CodeID_Set
            #define  SC_CID010_SET       ITF6CodeID_Set
            #define  SC_CID011_SET       Matrix2of5CodeID_Set
            #define  SC_CID013_SET       Code39CodeID_Set
            #define  SC_CID015_SET       CodabarCodeID_Set
            #define  SC_CID017_SET       Code93CodeID_Set
            #define  SC_CID019_SET       CP25CodeID_Set
            #define  SC_CID020_SET       AIM128CodeID_Set
            #define  SC_CID021_SET       ISBT128CodeID_Set
            #define  SC_CID022_SET       COOP25CodeID_Set
            #define  SC_CID023_SET       ISSNCodeID_Set
            #define  SC_CID024_SET       ISBNCodeID_Set
            #define  SC_CID025_SET       IND25CodeID_Set
            #define  SC_CID026_SET       STD25CodeID_Set
            #define  SC_CID027_SET       PlesseyCodeID_Set
            #define  SC_CID028_SET       Code11CodeID_Set
            #define  SC_CID029_SET       MSIPlesseyCodeID_Set
            #define  SC_CID030_SET       GS1_CompositeCodeID_Set
            #define  SC_CID031_SET       RSSCodeID_Set
            #define  SC_CID128_SET       Deut14CodeID_Set
            #define  SC_CID129_SET       Deut12CodeID_Set
            #define  SC_CID130_SET       TelepenCodeID_Set
            #define  SC_CID032_SET       PDF417CodeID_Set
            #define  SC_CID033_SET       QRCodeID_Set
            #define  SC_CID034_SET       AztecCodeID_Set
            #define  SC_CID035_SET       DMCodeID_Set
            #define  SC_CID036_SET       MaxiCodeID_Set
            #define  SC_CID039_SET       CSCCodeID_Set
            #define  SC_CID041_SET       GMCodeID_Set
            #define  SC_CID042_SET       MicroPDF417CodeID_Set
            #define  SC_CID043_SET       MicroQRCodeID_Set
            #define  SC_CID096_SET       PostnetCodeID_Set
            #define  SC_CID097_SET       InteMailCodeID_Set
            #define  SC_CID098_SET       RoyalMailCodeID_Set
            #define  SC_CID099_SET       PlanetCodeID_Set
            #define  SC_CID100_SET       KIXPostCodeID_Set
            #define  SC_CID101_SET       AUSPostalCodeID_Set

            #define  SC_CIDENA_GET       CodeIDEnable_Get
            #define  SC_CIDDEF_GET       AllCodeIDDefault_Get
            #define  SC_CIDQSZ_GET       CodeIDLenth_Get
            #define  SC_CIDQVL_GET       CodeIDValueRange_Get
            #define  SC_CID002_GET       Code128CodeID_Get
            #define  SC_CID003_GET       GS1_128CodeID_Get
            #define  SC_CID004_GET       EAN8CodeID_Get
            #define  SC_CID005_GET       EAN13CodeID_Get
            #define  SC_CID006_GET       UPCECodeID_Get
            #define  SC_CID007_GET       UPCACodeID_Get
            #define  SC_CID008_GET       Inter2of5CodeID_Get
            #define  SC_CID009_GET       ITF14CodeID_Get
            #define  SC_CID010_GET       ITF6CodeID_Get
            #define  SC_CID011_GET       Matrix2of5CodeID_Get
            #define  SC_CID013_GET       Code39CodeID_Get
            #define  SC_CID015_GET       CodabarCodeID_Get
            #define  SC_CID017_GET       Code93CodeID_Get
            #define  SC_CID019_GET       CP25CodeID_Get
            #define  SC_CID020_GET       AIM128CodeID_Get
            #define  SC_CID021_GET       ISBT128CodeID_Get
            #define  SC_CID022_GET       COOP25CodeID_Get
            #define  SC_CID023_GET       ISSNCodeID_Get
            #define  SC_CID024_GET       ISBNCodeID_Get
            #define  SC_CID025_GET       IND25CodeID_Get
            #define  SC_CID026_GET       STD25CodeID_Get
            #define  SC_CID027_GET       PlesseyCodeID_Get
            #define  SC_CID028_GET       Code11CodeID_Get
            #define  SC_CID029_GET       MSIPlesseyCodeID_Get
            #define  SC_CID030_GET       GS1_CompositeCodeID_Get
            #define  SC_CID031_GET       RSSCodeID_Get
            #define  SC_CID128_GET       Deut14CodeID_Get
            #define  SC_CID129_GET       Deut12CodeID_Set
            #define  SC_CID130_GET       TelepenCodeID_Get
            #define  SC_CID032_GET       PDF417CodeID_Get
            #define  SC_CID033_GET       QRCodeID_Get
            #define  SC_CID034_GET       AztecCodeID_Get
            #define  SC_CID035_GET       DMCodeID_Get
            #define  SC_CID036_GET       MaxiCodeID_Get
            #define  SC_CID039_GET       CSCCodeID_Get
            #define  SC_CID041_GET       GMCodeID_Get
            #define  SC_CID042_GET       MicroPDF417CodeID_Get
            #define  SC_CID043_GET       MicroQRCodeID_Get
            #define  SC_CID096_GET       PostnetCodeID_Get
            #define  SC_CID097_GET       InteMailCodeID_Get
            #define  SC_CID098_GET       RoyalMailCodeID_Get
            #define  SC_CID099_GET       PlanetCodeID_Get
            #define  SC_CID100_GET       KIXPostCodeID_Get
            #define  SC_CID101_GET       AUSPostalCodeID_Get

            #define  SC_CSUENA_SET       SufixEnable_Set
            #define  SC_CSUSET_SET       SufixEdit_Set
            #define  SC_CSUQSZ_SET       SufixLenth_Set

            #define  SC_CSUENA_GET       SufixEnable_Get
            #define  SC_CSUSET_GET       SufixEdit_Get
            #define  SC_CSUQSZ_GET       SufixLenth_Get
            
            #define  SC_TSUENA_SET       TerminatEnable_Set 
            #define  SC_TSUSET_SET       TerminatEdit_Set 
            #define  SC_TSUQSZ_SET       TerminatLenth_Set 

            #define  SC_TSUENA_GET       TerminatEnable_Get 
            #define  SC_TSUSET_GET       TerminatEdit_Get 
            #define  SC_TSUQSZ_GET       TerminatLenth_Get 
            
            #define  SC_QRYSYS_SET       SysInfo_Set 
			#define  SC_QRYFWV_SET       FirmwareVer_Set 
			#define  SC_QRYDCV_SET       DecoderVer_Set 
			#define  SC_QRYHWV_SET       HardwareVer_Set 
			#define  SC_QRYPDN_SET       ProductName_Set 
			#define  SC_QRYPSN_SET       ProductPID_Set 
			#define  SC_QRYDAT_SET       ProduceDate_Set  
			#define  SC_QRYEPN_SET       ProductOEMType_Set 
			#define  SC_QRYESN_SET       ProductOEMESN_Set 
			#define  SC_QRYEDT_SET       ProduceOEMDate_Set 
			
			#define  SC_QRYSYS_GET       SysInfo_Get 
			#define  SC_QRYFWV_GET       FirmwareVer_Get 
			#define  SC_QRYDCV_GET       DecoderVer_Get 
			#define  SC_QRYHWV_GET       HardwareVer_Get 
			#define  SC_QRYPDN_GET       ProductName_Get 
			#define  SC_QRYPSN_GET       ProductPID_Get 
			#define  SC_QRYDAT_GET       ProduceDate_Get  
			#define  SC_QRYEPN_GET       ProductOEMType_Get 
			#define  SC_QRYESN_GET       ProductOEMESN_Get 
			#define  SC_QRYEDT_GET       ProduceOEMDate_Get 
			
			#define  SC_PRNQRY_GET       QueryRealNameOfProduct_Get                          
                    #define  SC_PRNQRY_SET       QueryRealNameOfProduct_Set 

                    #define  SC_CPDEDT_SET       QueryCPUID_Set
                    #define  SC_CPDEDT_GET       QueryCPUID_Get
                    #define  SC_SECEDT_SET       Encryption_Set
                    #define  SC_SECEDT_GET       Encryption_Get
			
			#define  SC_128DEF_SET       Code128LoadDefault_Set 
			#define  SC_128ENA_SET       Code128Enable_Set 
			#define  SC_128MIN_SET       Code128MinLen_Set 
			#define  SC_128MAX_SET       Code128MaxLen_Set 
			#define  SC_128CHK_SET       Code128ChkTrsmt_Set 
			#define  SC_128FNC_SET       Code128FNC1_Set 

            #define  SC_128DEF_GET       Code128LoadDefault_Get
			#define  SC_128ENA_GET       Code128Enable_Get 
			#define  SC_128MIN_GET       Code128MinLen_Get 
			#define  SC_128MAX_GET       Code128MaxLen_Get 
			#define  SC_128CHK_GET       Code128ChkTrsmt_Get 
			#define  SC_128FNC_GET       Code128FNC1_Get 

			#define  SC_EA8DEF_SET       EAN8LoadDefault_Set 
			#define  SC_EA8ENA_SET       EAN8Enable_Set 
			#define  SC_EA8CHK_SET       EAN8ChkTrsmt_Set 
			#define  SC_EA8AD2_SET       EAN8AddCode2Enable_Set 
			#define  SC_EA8AD5_SET       EAN8AddCode5Enable_Set 
			#define  SC_EA8REQ_SET       EAN8AddedCodeEnable_Set 
			#define  SC_EA8SEP_SET       EAN8SEPRInAddedCodeEnable_Set 
			#define  SC_EA8EXP_SET       EAN8ExpandedType_Set 

            #define  SC_EA8DEF_GET       EAN8LoadDefault_Get 
			#define  SC_EA8ENA_GET       EAN8Enable_Get 
			#define  SC_EA8CHK_GET       EAN8ChkTrsmt_Get 
			#define  SC_EA8AD2_GET       EAN8AddCode2Enable_Get 
			#define  SC_EA8AD5_GET       EAN8AddCode5Enable_Get 
			#define  SC_EA8REQ_GET       EAN8AddedCodeEnable_Get 
			#define  SC_EA8SEP_GET       EAN8SEPRInAddedCodeEnable_Get 
			#define  SC_EA8EXP_GET       EAN8ExpandedType_Get 
			
			#define  SC_E13DEF_SET       EAN13LoadDefault_Set 
			#define  SC_E13ENA_SET       EAN13Enable_Set 
			#define  SC_E13CHK_SET       EAN13ChkTrsmt_Set 
			#define  SC_E13AD2_SET       EAN13AddCode2Enable_Set 
			#define  SC_E13AD5_SET       EAN13AddCode5Enable_Set 
			#define  SC_E13REQ_SET       EAN13AddedCodeEnable_Set 
			#define  SC_E13SEP_SET       EAN13SEPRInAddedCodeEnable_Set 
			#define  SC_E13290_SET       EAN13Beggin290Addenda_Set 
			#define  SC_E13378_SET       EAN13Beggin378_379Addenda_Set 
			#define  SC_E13414_SET       EAN13Beggin414_419Addenda_Set 
			#define  SC_E13434_SET       EAN13Beggin434_439Addenda_Set 
			#define  SC_E13977_SET       EAN13Beggin977Addenda_Set 
			#define  SC_E13978_SET       EAN13Beggin978Addenda_Set 
			#define  SC_E13979_SET       EAN13Beggin979Addenda_Set 

            #define  SC_E13DEF_GET       EAN13LoadDefault_Get 
			#define  SC_E13ENA_GET       EAN13Enable_Get 
			#define  SC_E13CHK_GET       EAN13ChkTrsmt_Get 
			#define  SC_E13AD2_GET       EAN13AddCode2Enable_Get 
			#define  SC_E13AD5_GET       EAN13AddCode5Enable_Get 
			#define  SC_E13REQ_GET       EAN13AddedCodeEnable_Get 
			#define  SC_E13SEP_GET       EAN13SEPRInAddedCodeEnable_Get 
			#define  SC_E13290_GET       EAN13Beggin290Addenda_Get 
			#define  SC_E13378_GET       EAN13Beggin378_379Addenda_Get 
			#define  SC_E13414_GET       EAN13Beggin414_419Addenda_Get 
			#define  SC_E13434_GET       EAN13Beggin434_439Addenda_Get 
			#define  SC_E13977_GET       EAN13Beggin977Addenda_Get 
			#define  SC_E13978_GET       EAN13Beggin978Addenda_Get 
			#define  SC_E13979_GET       EAN13Beggin979Addenda_Get 
			
			#define  SC_UPEDEF_SET       UPCELoadDefault_Set 
			#define  SC_UPEENA_SET       UPCEEnable_Set 
            #define  SC_UPEEN0_SET       UPCE0Enable_Set 
            #define  SC_UPEEN1_SET       UPCE1Enable_Set 
			#define  SC_UPECHK_SET       UPCEChkTrsmt_Set 
			#define  SC_UPEAD2_SET       UPCEAddCode2Enable_Set 
			#define  SC_UPEAD5_SET       UPCEAddCode5Enable_Set 
			#define  SC_UPEREQ_SET       UPCEAddedCodeEnable_Set 
			#define  SC_UPESEP_SET       UPCESEPRInAddedCodeEnable_Set 
			#define  SC_UPEPRE_SET       UPCETrsmtSysDigitType_Set 
			#define  SC_UPEEXP_SET       UPCEExpandedType_Set 

            #define  SC_UPEDEF_GET       UPCELoadDefault_Get
			#define  SC_UPEENA_GET       UPCEEnable_Get 
            #define  SC_UPEEN0_GET       UPCE0Enable_Get
            #define  SC_UPEEN1_GET       UPCE1Enable_Get
            #define  SC_UPECHK_GET       UPCEChkTrsmt_Get 
			#define  SC_UPEAD2_GET       UPCEAddCode2Enable_Get 
			#define  SC_UPEAD5_GET       UPCEAddCode5Enable_Get 
			#define  SC_UPEREQ_GET       UPCEAddedCodeEnable_Get 
			#define  SC_UPESEP_GET       UPCESEPRInAddedCodeEnable_Get 
			#define  SC_UPEPRE_GET       UPCETrsmtSysDigitType_Get 
			#define  SC_UPEEXP_GET       UPCEExpandedType_Get 
			
			#define  SC_UPADEF_SET       UPCALoadDefault_Set 
			#define  SC_UPAENA_SET       UPCAEnable_Set 
			#define  SC_UPACHK_SET       UPCAChkTrsmt_Set 
			#define  SC_UPAAD2_SET       UPCAAddCode2Enable_Set 
			#define  SC_UPAAD5_SET       UPCAAddCode5Enable_Set 
			#define  SC_UPAREQ_SET       UPCAAddedCodeEnable_Set 
			#define  SC_UPASEP_SET       UPCASEPRInAddedCodeEnable_Set 
			#define  SC_UPAPRE_SET       UPCATrsmtSysDigitType_Set 

            #define  SC_UPADEF_GET       UPCALoadDefault_Get 
			#define  SC_UPAENA_GET       UPCAEnable_Get 
			#define  SC_UPACHK_GET       UPCAChkTrsmt_Get 
			#define  SC_UPAAD2_GET       UPCAAddCode2Enable_Get 
			#define  SC_UPAAD5_GET       UPCAAddCode5Enable_Get 
			#define  SC_UPAREQ_GET       UPCAAddedCodeEnable_Get 
			#define  SC_UPASEP_GET       UPCASEPRInAddedCodeEnable_Get 
			#define  SC_UPAPRE_GET       UPCATrsmtSysDigitType_Get 

            #define  SC_CPNENA_SET       UPCA_EAN13WithExtendedCoupon_Set
            #define  SC_CPNGS1_SET       CouponGS1DataBarOutput_Set

            #define  SC_CPNENA_GET       UPCA_EAN13WithExtendedCoupon_Get
            #define  SC_CPNGS1_GET       CouponGS1DataBarOutput_Get
			
			#define  SC_I25DEF_SET       Inter2of5LoadDefault_Set 
			#define  SC_I25ENA_SET       Inter2of5Enable_Set 
			#define  SC_I25MIN_SET       Inter2of5MinLen_Set 
			#define  SC_I25MAX_SET       Inter2of5MaxLen_Set 
			#define  SC_I25CHK_SET       Inter2of5ChkTrsmt_Set 
			#define  SC_I25LEN_SET       Inter2of5DecoFixedLenEnable_Set 
			#define  SC_I25LSE_SET       Inter2of5AddFixedLen_Set 
			#define  SC_I25LCA_SET       Inter2of5CancelFixedLen_Set 
			#define  SC_I25FBB_SET       Inter2of5FebrabanEnable_Set 
            
            #define  SC_I25DEF_GET       Inter2of5LoadDefault_Get 
			#define  SC_I25ENA_GET       Inter2of5Enable_Get 
			#define  SC_I25MIN_GET       Inter2of5MinLen_Get 
			#define  SC_I25MAX_GET       Inter2of5MaxLen_Get 
			#define  SC_I25CHK_GET       Inter2of5ChkTrsmt_Get 
			#define  SC_I25LEN_GET       Inter2of5DecoFixedLenEnable_Get 
			#define  SC_I25LSE_GET       Inter2of5AddFixedLen_Get 
			#define  SC_I25LCA_GET       Inter2of5CancelFixedLen_Get 
			#define  SC_I25FBB_GET       Inter2of5FebrabanEnable_Get 
			
			#define  SC_I14DEF_SET       ITF14LoadDefault_Set 
			#define  SC_I14ENA_SET       ITF14Enable_Set 

            #define  SC_I14DEF_GET       ITF14LoadDefault_Get 
			#define  SC_I14ENA_GET       ITF14Enable_Get 
			
			#define  SC_IT6DEF_SET       ITF6LoadDefault_Set 
			#define  SC_IT6ENA_SET       ITF6Enable_Set 

            #define  SC_IT6DEF_GET       ITF6LoadDefault_Get 
			#define  SC_IT6ENA_GET       ITF6Enable_Get 
			
			#define  SC_M25DEF_SET       Matrix2of5LoadDefault_Set 
			#define  SC_M25ENA_SET       Matrix2of5Enable_Set 
			#define  SC_M25MIN_SET       Matrix2of5MinLen_Set 
			#define  SC_M25MAX_SET       Matrix2of5MaxLen_Set 
			#define  SC_M25CHK_SET       Matrix2of5ChkTrsmt_Set 

            #define  SC_M25DEF_GET       Matrix2of5LoadDefault_Get 
			#define  SC_M25ENA_GET       Matrix2of5Enable_Get 
			#define  SC_M25MIN_GET       Matrix2of5MinLen_Get 
			#define  SC_M25MAX_GET       Matrix2of5MaxLen_Get 
			#define  SC_M25CHK_GET       Matrix2of5ChkTrsmt_Get 

            #define  SC_C39DEF_SET       Code39LoadDefault_Set 
			#define  SC_C39ENA_SET       Code39Enable_Set 
			#define  SC_C39MIN_SET       Code39MinLen_Set 
			#define  SC_C39MAX_SET       Code39MaxLen_Set 
			#define  SC_C39CHK_SET       Code39ChkTrsmt_Set 
            #define  SC_C39TSC_SET       Code39TrsmtSTXETXEnable_Set 
			#define  SC_C39ASC_SET       Code39FullASCIIEnable_Set 
            #define  SC_C39E32_SET       Code39Code32PARAFEnable_Set 
			#define  SC_C39S32_SET       Code39AddSTXOfCode32Enable_Set 
			#define  SC_C39T32_SET       Code32TrsmtSTXETXEnable_Set 
			#define  SC_C39C32_SET       Code32ChkTrsmt_Set 

            #define  SC_C39DEF_GET       Code39LoadDefault_Get 
			#define  SC_C39ENA_GET       Code39Enable_Get 
			#define  SC_C39MIN_GET       Code39MinLen_Get 
			#define  SC_C39MAX_GET       Code39MaxLen_Get 
			#define  SC_C39CHK_GET       Code39ChkTrsmt_Get 
            #define  SC_C39TSC_GET       Code39TrsmtSTXETXEnable_Get 
			#define  SC_C39ASC_GET       Code39FullASCIIEnable_Get 
            #define  SC_C39E32_GET       Code39Code32PARAFEnable_Get 
			#define  SC_C39S32_GET       Code39AddSTXOfCode32Enable_Get 
			#define  SC_C39T32_GET       Code32TrsmtSTXETXEnable_Get 
			#define  SC_C39C32_GET       Code32ChkTrsmt_Get

            #define  SC_CBADEF_SET       CodabarLoadDefault_Set 
			#define  SC_CBAENA_SET       CodabarEnable_Set 
			#define  SC_CBAMIN_SET       CodabarMinLen_Set 
			#define  SC_CBAMAX_SET       CodabarMaxLen_Set 
			#define  SC_CBACHK_SET       CodabarChkTrsmt_Set 
            #define  SC_CBATSC_SET       CodabarTrsmtSTXETXEnable_Set 
			#define  SC_CBASCF_SET       CodabarSTXETXFormats_Set 
            #define  SC_CBACLS_SET       CodabarCLSIEdit_Set 

            #define  SC_CBADEF_GET       CodabarLoadDefault_Get
			#define  SC_CBAENA_GET       CodabarEnable_Get 
			#define  SC_CBAMIN_GET       CodabarMinLen_Get 
			#define  SC_CBAMAX_GET       CodabarMaxLen_Get 
			#define  SC_CBACHK_GET       CodabarChkTrsmt_Get 
            #define  SC_CBATSC_GET       CodabarTrsmtSTXETXEnable_Get 
			#define  SC_CBASCF_GET       CodabarSTXETXFormats_Get 
            #define  SC_CBACLS_GET       CodabarCLSIEdit_Get 

            #define  SC_C93DEF_SET       Code93LoadDefault_Set 
			#define  SC_C93ENA_SET       Code93Enable_Set 
			#define  SC_C93MIN_SET       Code93MinLen_Set 
			#define  SC_C93MAX_SET       Code93MaxLen_Set 
			#define  SC_C93CHK_SET       Code93ChkTrsmt_Set 

            #define  SC_C93DEF_GET       Code93LoadDefault_Get 
			#define  SC_C93ENA_GET       Code93Enable_Get 
			#define  SC_C93MIN_GET       Code93MinLen_Get 
			#define  SC_C93MAX_GET       Code93MaxLen_Get 
			#define  SC_C93CHK_GET       Code93ChkTrsmt_Get 

            #define  SC_GS1DEF_SET       GS1_128LoadDefault_Set 
			#define  SC_GS1ENA_SET       GS1_128Enable_Set 
			#define  SC_GS1MIN_SET       GS1_128MinLen_Set 
			#define  SC_GS1MAX_SET       GS1_128MaxLen_Set 
			#define  SC_GS1CHK_SET       GS1_128ChkTrsmt_Set 
			#define  SC_GS1FNC_SET       GS1_128OutputFNC1_Set 

            #define  SC_GS1DEF_GET       GS1_128LoadDefault_Get 
			#define  SC_GS1ENA_GET       GS1_128Enable_Get 
			#define  SC_GS1MIN_GET       GS1_128MinLen_Get 
			#define  SC_GS1MAX_GET       GS1_128MaxLen_Get 
			#define  SC_GS1CHK_GET       GS1_128ChkTrsmt_Get 
			#define  SC_GS1FNC_GET       GS1_128OutputFNC1_Get 

            #define  SC_RSSDEF_SET       RSSLoadDefault_Set 
			#define  SC_RSSENA_SET       RSSEnable_Set 
            #define  SC_RSSTAI_SET       RSSTrsmtAIEnable_Set 
            #define  SC_RSSE14_SET       RSSConvert2RSS14_Set 
            #define  SC_RSSENL_SET       RSSForGS1DatabarLimited_Set 
            #define  SC_RSSENE_SET       RSSForGS1DatabarExpand_Set 

            #define  SC_RSSDEF_GET       RSSLoadDefault_Get
			#define  SC_RSSENA_GET       RSSEnable_Get 
            #define  SC_RSSTAI_GET       RSSTrsmtAIEnable_Get 
            #define  SC_RSSE14_GET       RSSConvert2RSS14_Get 
            #define  SC_RSSENL_GET       RSSForGS1DatabarLimited_Get 
            #define  SC_RSSENE_GET       RSSForGS1DatabarExpand_Get 

            #define  SC_CPTDEF_SET       GS1_CompositeLoadDefault_Set 
			#define  SC_CPTENA_SET       GS1_CompositeEnable_Set 
            #define  SC_CPTUPC_SET       GS1_CompositeForUPC_EAN_Set 

            #define  SC_CPTDEF_GET       GS1_CompositeLoadDefault_Get 
			#define  SC_CPTENA_GET       GS1_CompositeEnable_Get 
            #define  SC_CPTUPC_GET       GS1_CompositeForUPC_EAN_Get

            #define  SC_C11DEF_SET       Code11LoadDefault_Set 
			#define  SC_C11ENA_SET       Code11Enable_Set 
			#define  SC_C11MIN_SET       Code11MinLen_Set 
			#define  SC_C11MAX_SET       Code11MaxLen_Set 
			#define  SC_C11CHK_SET       Code11ChkTrsmt_Set 
            #define  SC_C11TCK_SET       Code11ChkTrsmtAction_Set 

            #define  SC_C11DEF_GET       Code11LoadDefault_Get 
			#define  SC_C11ENA_GET       Code11Enable_Get 
			#define  SC_C11MIN_GET       Code11MinLen_Get 
			#define  SC_C11MAX_GET       Code11MaxLen_Get 
			#define  SC_C11CHK_GET       Code11ChkTrsmt_Get 
            #define  SC_C11TCK_GET       Code11ChkTrsmtAction_Get 

            #define  SC_ISBDEF_SET       ISBNLoadDefault_Set 
			#define  SC_ISBENA_SET       ISBNEnable_Set 
            #define  SC_ISBT10_SET       ISBNLenOfNUM_Set  
			#define  SC_ISBAD2_SET       ISBNAddCode2Enable_Set 
			#define  SC_ISBAD5_SET       ISBNAddCode5Enable_Set 
			#define  SC_ISBREQ_SET       ISBNAddedCodeEnable_Set 
			#define  SC_ISBSEP_SET       ISBNSEPRInAddedCodeEnable_Set 

            #define  SC_ISBDEF_GET       ISBNLoadDefault_Get
			#define  SC_ISBENA_GET       ISBNEnable_Get 
            #define  SC_ISBT10_GET       ISBNLenOfNUM_Get 
			#define  SC_ISBAD2_GET       ISBNAddCode2Enable_Get 
			#define  SC_ISBAD5_GET       ISBNAddCode5Enable_Get 
			#define  SC_ISBREQ_GET       ISBNAddedCodeEnable_Get 
			#define  SC_ISBSEP_GET       ISBNSEPRInAddedCodeEnable_Get 

            #define  SC_L25DEF_SET       IND25LoadDefault_Set 
			#define  SC_L25ENA_SET       IND25Enable_Set 
			#define  SC_L25MIN_SET       IND25MinLen_Set 
			#define  SC_L25MAX_SET       IND25MaxLen_Set 
			#define  SC_L25CHK_SET       IND25ChkTrsmt_Set 

            #define  SC_L25DEF_GET       IND25LoadDefault_Get 
			#define  SC_L25ENA_GET       IND25Enable_Get 
			#define  SC_L25MIN_GET       IND25MinLen_Get 
			#define  SC_L25MAX_GET       IND25MaxLen_Get 
			#define  SC_L25CHK_GET       IND25ChkTrsmt_Get 

            #define  SC_S25DEF_SET       STD25LoadDefault_Set 
			#define  SC_S25ENA_SET       STD25Enable_Set 
			#define  SC_S25MIN_SET       STD25MinLen_Set 
			#define  SC_S25MAX_SET       STD25MaxLen_Set 
			#define  SC_S25CHK_SET       STD25ChkTrsmt_Set 

            #define  SC_S25DEF_GET       STD25LoadDefault_Get
			#define  SC_S25ENA_GET       STD25Enable_Get 
			#define  SC_S25MIN_GET       STD25MinLen_Get 
			#define  SC_S25MAX_GET       STD25MaxLen_Get 
			#define  SC_S25CHK_GET       STD25ChkTrsmt_Get 

            #define  SC_PLYDEF_SET       PlesseyLoadDefault_Set 
			#define  SC_PLYENA_SET       PlesseyEnable_Set 
			#define  SC_PLYMIN_SET       PlesseyMinLen_Set 
			#define  SC_PLYMAX_SET       PlesseyMaxLen_Set 
			#define  SC_PLYCHK_SET       PlesseyChkTrsmt_Set 

            #define  SC_PLYDEF_GET       PlesseyLoadDefault_Get 
			#define  SC_PLYENA_GET       PlesseyEnable_Get 
			#define  SC_PLYMIN_GET       PlesseyMinLen_Get 
			#define  SC_PLYMAX_GET       PlesseyMaxLen_Get 
			#define  SC_PLYCHK_GET       PlesseyChkTrsmt_Get 

            #define  SC_MSIDEF_SET       MSIPlesseyLoadDefault_Set 
			#define  SC_MSIENA_SET       MSIPlesseyEnable_Set 
			#define  SC_MSIMIN_SET       MSIPlesseyMinLen_Set 
			#define  SC_MSIMAX_SET       MSIPlesseyMaxLen_Set 
			#define  SC_MSICHK_SET       MSIPlesseyChkTrsmt_Set 
            #define  SC_MSITCK_SET       MSIPlesseyChkTrsmtAction_Set 

            #define  SC_MSIDEF_GET       MSIPlesseyLoadDefault_Get 
			#define  SC_MSIENA_GET       MSIPlesseyEnable_Get 
			#define  SC_MSIMIN_GET       MSIPlesseyMinLen_Get 
			#define  SC_MSIMAX_GET       MSIPlesseyMaxLen_Get 
			#define  SC_MSICHK_GET       MSIPlesseyChkTrsmt_Get 
            #define  SC_MSITCK_GET       MSIPlesseyChkTrsmtAction_Get 

            #define  SC_ISSDEF_SET       ISSNLoadDefault_Set 
			#define  SC_ISSENA_SET       ISSNEnable_Set 
			#define  SC_ISSAD2_SET       ISSNAddCode2Enable_Set 
			#define  SC_ISSAD5_SET       ISSNAddCode5Enable_Set 
			#define  SC_ISSREQ_SET       ISSNAddedCodeEnable_Set 
			#define  SC_ISSSEP_SET       ISSNSEPRInAddedCodeEnable_Set 

            #define  SC_ISSDEF_GET       ISSNLoadDefault_Get
			#define  SC_ISSENA_GET       ISSNEnable_Get 
			#define  SC_ISSAD2_GET       ISSNAddCode2Enable_Get 
			#define  SC_ISSAD5_GET       ISSNAddCode5Enable_Get 
			#define  SC_ISSREQ_GET       ISSNAddedCodeEnable_Get 
			#define  SC_ISSSEP_GET       ISSNSEPRInAddedCodeEnable_Get 

            #define  SC_CHPDEF_SET       CP25LoadDefault_Set 
			#define  SC_CHPENA_SET       CP25Enable_Set 
			#define  SC_CHPMIN_SET       CP25MinLen_Set 
			#define  SC_CHPMAX_SET       CP25MaxLen_Set 
			#define  SC_CHPCHK_SET       CP25ChkTrsmt_Set 

            #define  SC_CHPDEF_GET       CP25LoadDefault_Get 
			#define  SC_CHPENA_GET       CP25Enable_Get 
			#define  SC_CHPMIN_GET       CP25MinLen_Get 
			#define  SC_CHPMAX_GET       CP25MaxLen_Get 
			#define  SC_CHPCHK_GET       CP25ChkTrsmt_Get 

            #define  SC_AIMDEF_SET       AIM128LoadDefault_Set 
			#define  SC_AIMENA_SET       AIM128Enable_Set 
			#define  SC_AIMMIN_SET       AIM128MinLen_Set 
			#define  SC_AIMMAX_SET       AIM128MaxLen_Set 
			#define  SC_AIMCHK_SET       AIM128ChkTrsmt_Set 
			#define  SC_AIMFNC_SET       AIM128FNC1_Set 

            #define  SC_AIMDEF_GET       AIM128LoadDefault_Get 
			#define  SC_AIMENA_GET       AIM128Enable_Get 
			#define  SC_AIMMIN_GET       AIM128MinLen_Get 
			#define  SC_AIMMAX_GET       AIM128MaxLen_Get 
			#define  SC_AIMCHK_GET       AIM128ChkTrsmt_Get 
			#define  SC_AIMFNC_GET       AIM128FNC1_Get 

            #define  SC_IBTDEF_SET       ISBT128LoadDefault_Set 
			#define  SC_IBTENA_SET       ISBT128Enable_Set 

            #define  SC_IBTDEF_GET       ISBT128LoadDefault_Get 
            #define  SC_IBTENA_GET       ISBT128Enable_Get 

            #define  SC_COPDEF_SET       COOP25LoadDefault_Set 
			#define  SC_COPENA_SET       COOP25Enable_Set 
			#define  SC_COPMIN_SET       COOP25MinLen_Set 
			#define  SC_COPMAX_SET       COOP25MaxLen_Set 
			#define  SC_COPCHK_SET       COOP25ChkTrsmt_Set 

            #define  SC_COPDEF_GET       COOP25LoadDefault_Get
			#define  SC_COPENA_GET       COOP25Enable_Get 
			#define  SC_COPMIN_GET       COOP25MinLen_Get 
			#define  SC_COPMAX_GET       COOP25MaxLen_Get 
			#define  SC_COPCHK_GET       COOP25ChkTrsmt_Get 

            #define  SC_D14DEF_SET       Deut14LoadDefault_Set 
			#define  SC_D14ENA_SET       Deut14Enable_Set 

            #define  SC_D14DEF_GET       Deut14LoadDefault_Get 
            #define  SC_D14ENA_GET       Deut14Enable_Get 

            #define  SC_D12DEF_SET       Deut12LoadDefault_Set 
			#define  SC_D12ENA_SET       Deut12Enable_Set 

            #define  SC_D12DEF_GET       Deut12LoadDefault_Get
            #define  SC_D12ENA_GET       Deut12Enable_Get 

            #define  SC_TLPDEF_SET       TelepenLoadDefault_Set 
			#define  SC_TLPENA_SET       TelepenEnable_Set 
			#define  SC_TLPMIN_SET       TelepenMinLen_Set 
			#define  SC_TLPMAX_SET       TelepenMaxLen_Set 
			#define  SC_TLPCHK_SET       TelepenChkTrsmt_Set 
			#define  SC_TLPASC_SET       TelepenFullASCIIWithNumericOnly_Set 

            #define  SC_TLPDEF_GET       TelepenLoadDefault_Get 
			#define  SC_TLPENA_GET       TelepenEnable_Get 
			#define  SC_TLPMIN_GET       TelepenMinLen_Get 
			#define  SC_TLPMAX_GET       TelepenMaxLen_Get 
			#define  SC_TLPCHK_GET       TelepenChkTrsmt_Get 
			#define  SC_TLPASC_GET       TelepenFullASCIIWithNumericOnly_Get 

            #define  SC_MACROC_SET       MacroCacheCleared_Set
            #define  SC_MACRO1_SET       MacroOutputMode1_Set
            #define  SC_MACRO2_SET       MacroOutputMode2_Set
            #define  SC_MACRO3_SET       MacroOutputMode3_Set
            
            #define  SC_MACROC_GET       MacroCacheCleared_Get
            #define  SC_MACRO1_GET       MacroOutputMode1_Get
            #define  SC_MACRO2_GET       MacroOutputMode2_Get
            #define  SC_MACRO3_GET       MacroOutputMode3_Get

            #define  SC_PDFDEF_SET       PDF417LoadDefault_Set
            #define  SC_PDFENA_SET       PDF417Enable_Set
            #define  SC_PDFMIN_SET       PDF417MinLen_Set
            #define  SC_PDFMAX_SET       PDF417MaxLen_Set
            #define  SC_PDFDOU_SET       PDF417DecoMultiCode_Set
            #define  SC_PDFDOF_SET       PDF417OrderOfMultiCode_Set
            #define  SC_PDFINV_SET       PDF417PhaseOfCode_Set
            #define  SC_PDFECI_SET       PDF417OutputECI_Set
            #define  SC_PDFENC_SET       PDF417EncodingOfChar_Set

            #define  SC_PDFDEF_GET       PDF417LoadDefault_Get
            #define  SC_PDFENA_GET       PDF417Enable_Get
            #define  SC_PDFMIN_GET       PDF417MinLen_Get
            #define  SC_PDFMAX_GET       PDF417MaxLen_Get
            #define  SC_PDFDOU_GET       PDF417DecoMultiCode_Get
            #define  SC_PDFDOF_GET       PDF417OrderOfMultiCode_Get
            #define  SC_PDFINV_GET       PDF417PhaseOfCode_Get
            #define  SC_PDFECI_GET       PDF417OutputECI_Get
            #define  SC_PDFENC_GET       PDF417EncodingOfChar_Get

            #define  SC_QRCDEF_SET       QRLoadDefault_Set
            #define  SC_QRCENA_SET       QREnable_Set
            #define  SC_QRCMIN_SET       QRMinLen_Set
            #define  SC_QRCMAX_SET       QRMaxLen_Set
            #define  SC_QRCDOU_SET       QRDecoMultiCode_Set
            #define  SC_QRCDOF_SET       QROrderOfMultiCode_Set
            #define  SC_QRCMCR_SET       QRDecoMicroQR_Set
            #define  SC_QRCINV_SET       QRPhaseOfCode_Set
            #define  SC_QRCECI_SET       QROutputECI_Set
            #define  SC_QRCENC_SET       QREncodingOfChar_Set

            #define  SC_QRCDEF_GET       QRLoadDefault_Get
            #define  SC_QRCENA_GET       QREnable_Get
            #define  SC_QRCMIN_GET       QRMinLen_Get
            #define  SC_QRCMAX_GET       QRMaxLen_Get
            #define  SC_QRCDOU_GET       QRDecoMultiCode_Get
            #define  SC_QRCDOF_GET       QROrderOfMultiCode_Get
            #define  SC_QRCMCR_GET       QRDecoMicroQR_Get
            #define  SC_QRCINV_GET       QRPhaseOfCode_Get
            #define  SC_QRCECI_GET       QROutputECI_Get
            #define  SC_QRCENC_GET       QREncodingOfChar_Get

            #define  SC_AZTDEF_SET       AztecLoadDefault_Set
            #define  SC_AZTENA_SET       AztecEnable_Set
            #define  SC_AZTMIN_SET       AztecMinLen_Set
            #define  SC_AZTMAX_SET       AztecMaxLen_Set
            #define  SC_AZTMUL_SET       AztecNumOfMultiCode_Set
            #define  SC_AZTMOD_SET       AztecModeOfHandleMultiCode_Set
            #define  SC_AZTECI_SET       AztecOutputECI_Set
            #define  SC_AZTENC_SET       AztecEncodingOfChar_Set

            #define  SC_AZTDEF_GET       AztecLoadDefault_Get
            #define  SC_AZTENA_GET       AztecEnable_Get
            #define  SC_AZTMIN_GET       AztecMinLen_Get
            #define  SC_AZTMAX_GET       AztecMaxLen_Get
            #define  SC_AZTMUL_GET       AztecNumOfMultiCode_Get
            #define  SC_AZTMOD_GET       AztecModeOfHandleMultiCode_Get
            #define  SC_AZTECI_GET       AztecOutputECI_Get
            #define  SC_AZTENC_GET       AztecEncodingOfChar_Get

            #define  SC_DMCDEF_SET       DMLoadDefault_Set
            #define  SC_DMCENA_SET       DMEnable_Set
            #define  SC_DMCMIN_SET       DMMinLen_Set
            #define  SC_DMCMAX_SET       DMMaxLen_Set
            #define  SC_DMCDOU_SET       DMDecoMultiCode_Set
            #define  SC_DMCDOF_SET       SC_NullSet
            #define  SC_DMCREC_SET       DMDecoRectCode_Set
            #define  SC_DMCADD_SET       DMAddInfoWordsToDecoResult_Set
            #define  SC_DMCINV_SET       DMPhaseOfCode_Set
            #define  SC_DMCECI_SET       DMOutputECI_Set
            #define  SC_DMCENC_SET       DMEncodingOfChar_Set

            #define  SC_DMCDEF_GET       DMLoadDefault_Get
            #define  SC_DMCENA_GET       DMEnable_Get
            #define  SC_DMCMIN_GET       DMMinLen_Get
            #define  SC_DMCMAX_GET       DMMaxLen_Get
            #define  SC_DMCDOU_GET       DMDecoMultiCode_Get
            #define  SC_DMCDOF_GET       SC_NullGet
            #define  SC_DMCREC_GET       DMDecoRectCode_Get
            #define  SC_DMCADD_GET       DMAddInfoWordsToDecoResult_Get
            #define  SC_DMCINV_GET       DMPhaseOfCode_Get
            #define  SC_DMCECI_GET       DMOutputECI_Get
            #define  SC_DMCENC_GET       DMEncodingOfChar_Get

            #define  SC_MXCDEF_SET       MaxiLoadDefault_Set
            #define  SC_MXCENA_SET       MaxiEnable_Set
            #define  SC_MXCMIN_SET       MaxiMinLen_Set
            #define  SC_MXCMAX_SET       MaxiMaxLen_Set

            #define  SC_MXCDEF_GET       MaxiLoadDefault_Get
            #define  SC_MXCENA_GET       MaxiEnable_Get
            #define  SC_MXCMIN_GET       MaxiMinLen_Get
            #define  SC_MXCMAX_GET       MaxiMaxLen_Get

            #define  SC_CSCDEF_SET       CSCLoadDefault_Set
            #define  SC_CSCENA_SET       CSCEnable_Set
            #define  SC_CSCMIN_SET       CSCMinLen_Set
            #define  SC_CSCMAX_SET       CSCMaxLen_Set
            #define  SC_CSCDOU_SET       CSCDecoMultiCode_Set
            #define  SC_CSCINV_SET       CSCPhaseOfCode_Set

            #define  SC_CSCDEF_GET       CSCLoadDefault_Get
            #define  SC_CSCENA_GET       CSCEnable_Get
            #define  SC_CSCMIN_GET       CSCMinLen_Get
            #define  SC_CSCMAX_GET       CSCMaxLen_Get
            #define  SC_CSCDOU_GET       CSCDecoMultiCode_Get
            #define  SC_CSCINV_GET       CSCPhaseOfCode_Get

            #define  SC_GMCDEF_SET       GMLoadDefault_Set
            #define  SC_GMCENA_SET       GMEnable_Set
            #define  SC_GMCMIN_SET       GMMinLen_Set
            #define  SC_GMCMAX_SET       GMMaxLen_Set

            #define  SC_GMCDEF_GET       GMLoadDefault_Get
            #define  SC_GMCENA_GET       GMEnable_Get
            #define  SC_GMCMIN_GET       GMMinLen_Get
            #define  SC_GMCMAX_GET       GMMaxLen_Get

            #define  SC_MPDDEF_SET       MicroPDF417LoadDefault_Set
            #define  SC_MPDENA_SET       MicroPDF417Enable_Set
            #define  SC_MPDMIN_SET       MicroPDF417MinLen_Set
            #define  SC_MPDMAX_SET       MicroPDF417MaxLen_Set

            #define  SC_MPDDEF_GET       MicroPDF417LoadDefault_Get
            #define  SC_MPDENA_GET       MicroPDF417Enable_Get
            #define  SC_MPDMIN_GET       MicroPDF417MinLen_Get
            #define  SC_MPDMAX_GET       MicroPDF417MaxLen_Get

            #define  SC_MQRDEF_SET       MicroQRLoadDefault_Set
            #define  SC_MQRENA_SET       MicroQREnable_Set
            #define  SC_MQRMIN_SET       MicroQRMinLen_Set
            #define  SC_MQRMAX_SET       MicroQRMaxLen_Set

            #define  SC_MQRDEF_GET       MicroQRLoadDefault_Get
            #define  SC_MQRENA_GET       MicroQREnable_Get
            #define  SC_MQRMIN_GET       MicroQRMinLen_Get
            #define  SC_MQRMAX_GET       MicroQRMaxLen_Get

            #define  SC_SOBDEF_SET       SpecOCRBLoadDefault_Set
            #define  SC_SOBENA_SET       SpecOCRBEnable_Set
            #define  SC_SOBMIN_SET       SpecOCRBMinLen_Set
            #define  SC_SOBMAX_SET       SpecOCRBMaxLen_Set

            #define  SC_SOBDEF_GET       SpecOCRBLoadDefault_Get
            #define  SC_SOBENA_GET       SpecOCRBEnable_Get
            #define  SC_SOBMIN_GET       SpecOCRBMinLen_Get
            #define  SC_SOBMAX_GET       SpecOCRBMaxLen_Get

            #define  SC_PASDEF_SET       PassportOCRBLoadDefault_Set
            #define  SC_PASENA_SET       PassportOCRBEnable_Set

            #define  SC_PASDEF_GET       PassportOCRBLoadDefault_Get
            #define  SC_PASENA_GET       PassportOCRBEnable_Get

			#define  SC_PNTDEF_SET       PostnetLoadDefault_Set
			#define  SC_PNTENA_SET       PostnetEnable_Set
			#define  SC_PNTCHK_SET       PostnetChkTrsmt_Set

			#define  SC_PNTDEF_GET       PostnetLoadDefault_Get
			#define  SC_PNTENA_GET       PostnetEnable_Get
			#define  SC_PNTCHK_GET       PostnetChkTrsmt_Get

            #define  SC_ILGDEF_SET       InteMailLoadDefault_Set
            #define  SC_ILGENA_SET       InteMailEnable_Set

            #define  SC_ILGDEF_GET       InteMailLoadDefault_Get
            #define  SC_ILGENA_GET       InteMailEnable_Get

			#define  SC_ROYDEF_SET       RoyalMailLoadDefault_Set
			#define  SC_ROYENA_SET       RoyalMailEnable_Set

			#define  SC_ROYDEF_GET       RoyalMailLoadDefault_Get
			#define  SC_ROYENA_GET       RoyalMailEnable_Get

			#define  SC_PLADEF_SET       PlanetLoadDefault_Set
			#define  SC_PLAENA_SET       PlanetEnable_Set
			#define  SC_PLACHK_SET       PlanetChkTrsmt_Set

			#define  SC_PLADEF_GET       PlanetLoadDefault_Get
			#define  SC_PLAENA_GET       PlanetEnable_Get
			#define  SC_PLACHK_GET       PlanetChkTrsmt_Get

			#define  SC_KIXDEF_SET       KIXPostLoadDefault_Set
			#define  SC_KIXENA_SET       KIXPostEnable_Set

			#define  SC_KIXDEF_GET       KIXPostLoadDefault_Get
			#define  SC_KIXENA_GET       KIXPostEnable_Get

			#define  SC_APLDEF_SET       AUSPostalLoadDefault_Set
			#define  SC_APLENA_SET       AUSPostalEnable_Set

			#define  SC_APLDEF_GET       AUSPostalLoadDefault_Get
			#define  SC_APLENA_GET       AUSPostalEnable_Get

            #define  SC_DFMDEF_SET       LoadDefaultDataFormatType_Set 
            #define  SC_DFMENA_SET       DataFormatterEnable_Set 
            #define  SC_DFMSET_SET       DataFormatEditorEnable_Set 
            #define  SC_DFMCLR_SET       ClearDataFormatTypes_Set 
            #define  SC_DFMCAL_SET       ClearAllDataFormatTypes_Set 
            #define  SC_DFMUSE_SET       LoadDataFormatTypes_Set 
            #define  SC_DFMSIN_SET       LoadDataFormatTypesForSingleDeco_Set 
            #define  SC_DFMTON_SET       BeepOnDataFormatNotMatchEnable_Set 
            #define  SC_DFMQCU_SET       CurrentDataFormatType_Set 
            #define  SC_DFMQFA_SET       DefaultFactoryDataFormatType_Set 
            #define  SC_DFMQSZ_SET       MaxLenOfDataFormatTypes_Set 

            #define  SC_DFMDEF_GET       LoadDefaultDataFormatType_Get 
            #define  SC_DFMENA_GET       DataFormatterEnable_Get 
            #define  SC_DFMSET_GET       DataFormatEditorEnable_Get 
            #define  SC_DFMCLR_GET       ClearDataFormatTypes_Get 
            #define  SC_DFMCAL_GET       ClearAllDataFormatTypes_Get 
            #define  SC_DFMUSE_GET       LoadDataFormatTypes_Get 
            #define  SC_DFMSIN_GET       LoadDataFormatTypesForSingleDeco_Get 
            #define  SC_DFMTON_GET       BeepOnDataFormatNotMatchEnable_Get 
            #define  SC_DFMQCU_GET       CurrentDataFormatType_Get 
            #define  SC_DFMQFA_GET       DefaultFactoryDataFormatType_Get 
            #define  SC_DFMQSZ_GET       MaxLenOfDataFormatTypes_Get 

            #define  SC_DLPSET_SET       EnableAnalyzeDrivingLic_Set 
			#define  SC_DLPCLR_SET       DisableAnalyzeDrivingLic_Set 
            #define  SC_DLPQSZ_SET       MaxLenOfCommandInAnalDrivingLic_Get 

            #define  SC_DLPSET_GET       EnableAnalyzeDrivingLic_Get 
			#define  SC_DLPCLR_GET       SC_NullGet 
            #define  SC_DLPQSZ_GET       SC_NullGet 

            #define  SC_PACKAG_SET       PackageForDecoInfo_Set

            #define  SC_PACKAG_GET       PackageForDecoInfo_Get

            #define  SC_EXTRAC_SET       CuttingoutDecoInfoEnable_Set
            #define  SC_EXTADD_SET       AddConfigForCuttingoutDecoInfo_Set
            #define  SC_EXTCLR_SET       ClearCuttingoutForCodeTypes_Set
            #define  SC_EXTCLO_SET       ClearLatestCFGOfCuttingout_Set
            #define  SC_EXTCAL_SET       ClearAllCFGOfCuttingout_Set

            #define  SC_EXTRAC_GET       CuttingoutDecoInfoEnable_Get
            #define  SC_EXTADD_GET       AddConfigForCuttingoutDecoInfo_Get
            #define  SC_EXTCLR_GET       ClearCuttingoutForCodeTypes_Get
            #define  SC_EXTCLO_GET       ClearLatestCFGOfCuttingout_Get
            #define  SC_EXTCAL_GET       ClearAllCFGOfCuttingout_Set

            #define  SC_IMGGWH_SET       ResolutionRatio_Set
            #define  SC_IMGGET_SET       FetchingImage_Set

            #define  SC_IMGGWH_GET       ResolutionRatio_Get
            #define  SC_IMGGET_GET       FetchingImage_Get

            #define  SC_PSNEDT_SET       SettingProductSN_Set
            #define  SC_PSNDEL_SET       DeleteProductSN_Set
            #define  SC_PSNQSZ_SET       ProductSNLength_Set

            #define  SC_PSNEDT_GET       SettingProductSN_Get
            #define  SC_PSNDEL_GET       DeleteProductSN_Get
	        #define  SC_PSNQSZ_GET       ProductSNLength_Get

            #define  SC_ESNSET_SET       SettingProductESN_Set
            #define  SC_ESNCLR_SET       DeleteProductESN_Set
            #define  SC_ESNQSZ_SET       ProductESNLength_Set

            #define  SC_ESNSET_GET       SettingProductESN_Get
            #define  SC_ESNCLR_GET       DeleteProductESN_Get
            #define  SC_ESNQSZ_GET       ProductESNLength_Get

            #define  SC_DATEDT_SET       SettingProductDate_Set
            #define  SC_DATDEL_SET       DeleteProductDate_Set
            #define  SC_DATQSZ_SET       ProductDateLength_Set

            #define  SC_DATEDT_GET       SettingProductDate_Get
            #define  SC_DATDEL_GET       DeleteProductDate_Get
            #define  SC_DATQSZ_GET       ProductDateLength_Get

            #define  SC_PDNEDT_SET       SettingProductName_Set
            #define  SC_PDNDEL_SET       DeleteProductName_Set
            #define  SC_PDNQSZ_SET       ProductNameLength_Set

            #define  SC_PDNEDT_GET       SettingProductName_Get
            #define  SC_PDNDEL_GET       DeleteProductName_Get
            #define  SC_PDNQSZ_GET       ProductNameLength_Get

            #define  SC_MFNEDT_SET       SettingManufacturerName_Set
            #define  SC_MFNDEL_SET       DeleteManufacturerName_Set
            #define  SC_MFNQSZ_SET       ManufacturerNameLength_Set

            #define  SC_MFNEDT_GET       SettingManufacturerName_Get
            #define  SC_MFNDEL_GET       DeleteManufacturerName_Get
            #define  SC_MFNQSZ_GET       ManufacturerNameLength_Get

            #define  SC_HWVEDT_SET       SettingInfoOfHardware_Set
            #define  SC_HWVDEL_SET       DeleteInfoOfHardware_Set
            #define  SC_HWVQSZ_SET       InfoOfHardwareLength_Set

            #define  SC_HWVEDT_GET       SettingInfoOfHardware_Get
            #define  SC_HWVDEL_GET       DeleteInfoOfHardware_Get
            #define  SC_HWVQSZ_GET       InfoOfHardwareLength_Get

            #define  SC_PRNEDT_SET       SettingRealNameOfProduct_Set
            #define  SC_PRNDEL_SET       DeleteRealNameOfProduct_Set
            #define  SC_PRNQSZ_SET       RealNameOfProductLength_Set

            #define  SC_PRNEDT_GET       SettingRealNameOfProduct_Get
            #define  SC_PRNDEL_GET       DeleteRealNameOfProduct_Get
            #define  SC_PRNQSZ_GET       RealNameOfProductLength_Get

            #define  SC_DWNENA_SET       IsDownloadEnable_Set
            #define  SC_DWNDEL_SET       DeleteCFGOfIsDownload_Set

            #define  SC_DWNENA_GET       IsDownloadEnable_Get
            #define  SC_DWNDEL_GET       DeleteCFGOfIsDownload_Get

            #define  SC_DOFLVL_SET       DepthOfFieldInProduct_Set
            #define  SC_DELDOF_SET       DelDepthOfFieldInProduct_Set

			#define  SC_DOFLVL_GET       DepthOfFieldInProduct_Get
			#define  SC_DELDOF_GET       DelDepthOfFieldInProduct_Get

			#define  SC_HWRFRQ_SET       RunningFreqOfMCU_Set

			#define  SC_HWRFRQ_GET       RunningFreqOfMCU_Get

			#define  SC_RUNWEL_SET       RunningWirelessMode_Set

			#define  SC_RUNWEL_GET       RunningWirelessMode_Get

			#define  SC_PIDUSN_SET       USBEnumratePIDSNWithSN_Set

			#define  SC_PIDUSN_GET       USBEnumratePIDSNWithSN_Get

			#define  SC_AUTOUR_SET       AutoAdaptBetweenSerialCommAndUSB_Set

			#define  SC_AUTOUR_GET       AutoAdaptBetweenSerialCommAndUSB_Get

			#define  SC_BUFQRY_SET       QueryBuffSizeOfInstructions_Set

			#define  SC_BUFQRY_GET       QueryBuffSizeOfInstructions_Get
/************************************************************************/
#ifdef __cplusplus
}
#endif
#endif //__SC_FNDEFINE_H__
