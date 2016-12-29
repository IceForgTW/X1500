#include "SC_Protocol.h"
#include "SC_Function.h"
#include "SC_FnDefine.h"
#include "SC_MapTable.h"


/*查询所有参数执行函数映射表*/
sc_subtagmap_st  SC_QueryAllMapTab[] = 
{
	    {"0",                         SC_QueryAll, 0},       
	    {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Factory Config Sub Tag 执行函数映射表TAG:FAC*/
const sc_subtagmap_st  SC_FactoryCfgMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_FACDEF_GET, SC_FACDEF_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*User Config Sub Tag 执行函数映射表TAG:CUS*/
const sc_subtagmap_st  SC_UserConfigMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_CUSDEF_GET, SC_CUSDEF_SET}, 
        {SC_SUBTAG_INT("SAV"),      SC_CUSSAV_GET, SC_CUSSAV_SET}, 
        {SC_SUBTAG_INT("DEL"),      SC_CUSDEL_GET, SC_CUSDEL_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Genneruc Config Sub Tag 执行函数映射表TAG:GEN*/
const sc_subtagmap_st  SC_GenericCfgMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_GENDEF_GET, SC_GENDEF_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Save Config Sub Tag 执行函数映射表TAG:SAV*/
const sc_subtagmap_st  SC_SaveConfigMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("EFY"),      SC_SAVEFY_GET, SC_SAVEFY_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*System message Sub Tag 执行函数映射表TAG:QRY*/
const sc_subtagmap_st  SC_SysMsgMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("SYS"),      SC_QRYSYS_GET, SC_QRYSYS_SET}, 	 
        {SC_SUBTAG_INT("FWV"),      SC_QRYFWV_GET, SC_QRYFWV_SET}, 
        {SC_SUBTAG_INT("DCV"),      SC_QRYDCV_GET, SC_QRYDCV_SET}, 
        {SC_SUBTAG_INT("HWV"),      SC_QRYHWV_GET, SC_QRYHWV_SET}, 			
        {SC_SUBTAG_INT("PDN"),      SC_QRYPDN_GET, SC_QRYPDN_SET}, 	
        {SC_SUBTAG_INT("PSN"),      SC_QRYPSN_GET, SC_QRYPSN_SET}, 		
        {SC_SUBTAG_INT("DAT"),      SC_QRYDAT_GET, SC_QRYDAT_SET}, 
        {SC_SUBTAG_INT("EPN"),      SC_QRYEPN_GET, SC_QRYEPN_SET}, 			
        {SC_SUBTAG_INT("ESN"),      SC_QRYESN_GET, SC_QRYESN_SET},
        {SC_SUBTAG_INT("EDT"),      SC_QRYEDT_GET, SC_QRYEDT_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Product message Sub Tag 执行函数映射表PRN*/

const sc_subtagmap_st  SC_ProdMsgMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("EDT"),      SC_PRNEDT_GET, SC_PRNEDT_SET}, 
        {SC_SUBTAG_INT("DEL"),      SC_PRNDEL_GET, SC_PRNDEL_SET}, 
        {SC_SUBTAG_INT("QSZ"),      SC_PRNQSZ_GET, SC_PRNQSZ_SET},
        {SC_SUBTAG_INT("QRY"),      SC_PRNQRY_GET, SC_PRNQRY_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

const sc_subtagmap_st  SC_CPUIDMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},            
        {"2"/*SC_TAG_PRIV*/,              0, 0},
        {SC_SUBTAG_INT("QRY"),      SC_CPDEDT_GET, SC_CPDEDT_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

const sc_subtagmap_st  SC_encryptMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {"2"/*SC_TAG_PRIV*/,              0, 0},
        {SC_SUBTAG_INT("SET"),      SC_SECEDT_GET, SC_SECEDT_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

      
/*Code128 Sub Tag 执行函数映射表TAG:128*/

const sc_subtagmap_st  SC_Code128MapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_128DEF_GET, SC_128DEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_128ENA_GET, SC_128ENA_SET}, 
        {SC_SUBTAG_INT("MIN"),      SC_128MIN_GET, SC_128MIN_SET}, 
        {SC_SUBTAG_INT("MAX"),      SC_128MAX_GET, SC_128MAX_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_128CHK_GET, SC_128CHK_SET},
        {SC_SUBTAG_INT("FNC"),      SC_128FNC_GET, SC_128FNC_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*EAN8 Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_EAN8MapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_EA8DEF_GET, SC_EA8DEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_EA8ENA_GET, SC_EA8ENA_SET},  
        {SC_SUBTAG_INT("CHK"),      SC_EA8CHK_GET, SC_EA8CHK_SET},
        {SC_SUBTAG_INT("AD2"),      SC_EA8AD2_GET, SC_EA8AD2_SET}, 
        {SC_SUBTAG_INT("AD5"),      SC_EA8AD5_GET, SC_EA8AD5_SET}, 
        {SC_SUBTAG_INT("REQ"),      SC_EA8REQ_GET, SC_EA8REQ_SET}, 
        {SC_SUBTAG_INT("SEP"),      SC_EA8SEP_GET, SC_EA8SEP_SET},

		//{"2"/*SC_TAG_PRIV*/,               0, 0},
        {SC_SUBTAG_INT("EXP"),      SC_EA8EXP_GET, SC_EA8EXP_SET},
        {"1",               0, 0}
};


/*EAN13 Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_EAN13MapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_E13DEF_GET, SC_E13DEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_E13ENA_GET, SC_E13ENA_SET},  
        {SC_SUBTAG_INT("CHK"),      SC_E13CHK_GET, SC_E13CHK_SET},
        {SC_SUBTAG_INT("AD2"),      SC_E13AD2_GET, SC_E13AD2_SET}, 
        {SC_SUBTAG_INT("AD5"),      SC_E13AD5_GET, SC_E13AD5_SET}, 
        {SC_SUBTAG_INT("REQ"),      SC_E13REQ_GET, SC_E13REQ_SET}, 
        {SC_SUBTAG_INT("SEP"),      SC_E13SEP_GET, SC_E13SEP_SET},
        {SC_SUBTAG_INT("290"),      SC_E13290_GET, SC_E13290_SET},
        {SC_SUBTAG_INT("378"),      SC_E13378_GET, SC_E13378_SET},
        {SC_SUBTAG_INT("414"),      SC_E13414_GET, SC_E13414_SET},
        {SC_SUBTAG_INT("434"),      SC_E13434_GET, SC_E13434_SET},
        {SC_SUBTAG_INT("977"),      SC_E13977_GET, SC_E13977_SET},
        {SC_SUBTAG_INT("978"),      SC_E13978_GET, SC_E13978_SET},
        {SC_SUBTAG_INT("979"),      SC_E13979_GET, SC_E13979_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*UPCE Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_UPCEMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_UPEDEF_GET, SC_UPEDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_UPEENA_GET, SC_UPEENA_SET},  
        {SC_SUBTAG_INT("EN0"),      SC_UPEEN0_GET, SC_UPEEN0_SET},  
        {SC_SUBTAG_INT("EN1"),      SC_UPEEN1_GET, SC_UPEEN1_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_UPECHK_GET, SC_UPECHK_SET},
        {SC_SUBTAG_INT("AD2"),      SC_UPEAD2_GET, SC_UPEAD2_SET}, 
        {SC_SUBTAG_INT("AD5"),      SC_UPEAD5_GET, SC_UPEAD5_SET}, 
        {SC_SUBTAG_INT("REQ"),      SC_UPEREQ_GET, SC_UPEREQ_SET}, 
        {SC_SUBTAG_INT("SEP"),      SC_UPESEP_GET, SC_UPESEP_SET},
        {SC_SUBTAG_INT("PRE"),      SC_UPEPRE_GET, SC_UPEPRE_SET},
		{"2"/*SC_TAG_PRIV*/,              0, 0},
        {SC_SUBTAG_INT("EXP"),      SC_UPEEXP_GET, SC_UPEEXP_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*UPCA Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_UPCAMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_UPADEF_GET, SC_UPADEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_UPAENA_GET, SC_UPAENA_SET},  
        {SC_SUBTAG_INT("CHK"),      SC_UPACHK_GET, SC_UPACHK_SET},
        {SC_SUBTAG_INT("AD2"),      SC_UPAAD2_GET, SC_UPAAD2_SET}, 
        {SC_SUBTAG_INT("AD5"),      SC_UPAAD5_GET, SC_UPAAD5_SET}, 
        {SC_SUBTAG_INT("REQ"),      SC_UPAREQ_GET, SC_UPAREQ_SET}, 
        {SC_SUBTAG_INT("SEP"),      SC_UPASEP_GET, SC_UPASEP_SET},
        {SC_SUBTAG_INT("PRE"),      SC_UPAPRE_GET, SC_UPAPRE_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Coupon Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_CouponMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 	
        {SC_SUBTAG_INT("ENA"),      SC_CPNENA_GET, SC_CPNENA_SET},  
        {SC_SUBTAG_INT("GS1"),      SC_CPNGS1_GET, SC_CPNGS1_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};


/*ITF25 Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_ITF25MapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_I25DEF_GET, SC_I25DEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_I25ENA_GET, SC_I25ENA_SET}, 
        {SC_SUBTAG_INT("MIN"),      SC_I25MIN_GET, SC_I25MIN_SET}, 
        {SC_SUBTAG_INT("MAX"),      SC_I25MAX_GET, SC_I25MAX_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_I25CHK_GET, SC_I25CHK_SET},
		{SC_SUBTAG_INT("FBB"),      SC_I25FBB_GET, SC_I25FBB_SET},

		{"2"/*SC_TAG_PRIV*/,               0, 0},
        {SC_SUBTAG_INT("LEN"),      SC_I25LEN_GET, SC_I25LEN_SET},
        {SC_SUBTAG_INT("LSE"),      SC_I25LSE_GET, SC_I25LSE_SET},
        {SC_SUBTAG_INT("LCA"),      SC_I25LCA_GET, SC_I25LCA_SET},
        {"1",               0, 0}
};
/*Febraban Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_FebrabanMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("SEN"),      SC_FEBSEN_GET, SC_FEBSEN_SET}, 	
	  {SC_SUBTAG_INT("SDT"),      SC_FEBSDT_GET, SC_FEBSDT_SET}, 
	  {SC_SUBTAG_INT("MEN"),      SC_FEBMEN_GET, SC_FEBMEN_SET}, 	
        {SC_SUBTAG_INT("MDT"),      SC_FEBMDT_GET, SC_FEBMDT_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};


/*ITF14 Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_ITF14MapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_I14DEF_GET, SC_I14DEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_I14ENA_GET, SC_I14ENA_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*ITF6 Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_ITF6MapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_IT6DEF_GET, SC_IT6DEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_IT6ENA_GET, SC_IT6ENA_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Matrix 25 Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_Matrix25MapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_M25DEF_GET, SC_M25DEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_M25ENA_GET, SC_M25ENA_SET}, 
        {SC_SUBTAG_INT("MIN"),      SC_M25MIN_GET, SC_M25MIN_SET}, 
        {SC_SUBTAG_INT("MAX"),      SC_M25MAX_GET, SC_M25MAX_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_M25CHK_GET, SC_M25CHK_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Code 39 Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_Code39MapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_C39DEF_GET, SC_C39DEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_C39ENA_GET, SC_C39ENA_SET}, 
        {SC_SUBTAG_INT("MIN"),      SC_C39MIN_GET, SC_C39MIN_SET}, 
        {SC_SUBTAG_INT("MAX"),      SC_C39MAX_GET, SC_C39MAX_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_C39CHK_GET, SC_C39CHK_SET},
        {SC_SUBTAG_INT("TSC"),      SC_C39TSC_GET, SC_C39TSC_SET},
        {SC_SUBTAG_INT("ASC"),      SC_C39ASC_GET, SC_C39ASC_SET},
        {SC_SUBTAG_INT("E32"),      SC_C39E32_GET, SC_C39E32_SET},
        {SC_SUBTAG_INT("S32"),      SC_C39S32_GET, SC_C39S32_SET},
		{SC_SUBTAG_INT("T32"),      SC_C39T32_GET, SC_C39T32_SET},
		{SC_SUBTAG_INT("C32"),      SC_C39C32_GET, SC_C39C32_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Codabar Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_CodabarMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_CBADEF_GET, SC_CBADEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_CBAENA_GET, SC_CBAENA_SET}, 
        {SC_SUBTAG_INT("MIN"),      SC_CBAMIN_GET, SC_CBAMIN_SET}, 
        {SC_SUBTAG_INT("MAX"),      SC_CBAMAX_GET, SC_CBAMAX_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_CBACHK_GET, SC_CBACHK_SET},
        {SC_SUBTAG_INT("TSC"),      SC_CBATSC_GET, SC_CBATSC_SET},
        {SC_SUBTAG_INT("SCF"),      SC_CBASCF_GET, SC_CBASCF_SET},
        {SC_SUBTAG_INT("CLS"),      SC_CBACLS_GET, SC_CBACLS_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Code 93 Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_Code93MapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_C93DEF_GET, SC_C93DEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_C93ENA_GET, SC_C93ENA_SET}, 
        {SC_SUBTAG_INT("MIN"),      SC_C93MIN_GET, SC_C93MIN_SET}, 
        {SC_SUBTAG_INT("MAX"),      SC_C93MAX_GET, SC_C93MAX_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_C93CHK_GET, SC_C93CHK_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};


/*GS1 Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_GS1MapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_GS1DEF_GET, SC_GS1DEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_GS1ENA_GET, SC_GS1ENA_SET}, 
        {SC_SUBTAG_INT("MIN"),      SC_GS1MIN_GET, SC_GS1MIN_SET}, 
        {SC_SUBTAG_INT("MAX"),      SC_GS1MAX_GET, SC_GS1MAX_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_GS1CHK_GET, SC_GS1CHK_SET},
        {SC_SUBTAG_INT("FNC"),      SC_GS1FNC_GET, SC_GS1FNC_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*RSS Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_RSSMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_RSSDEF_GET, SC_RSSDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_RSSENA_GET, SC_RSSENA_SET}, 
        {SC_SUBTAG_INT("TAI"),      SC_RSSTAI_GET, SC_RSSTAI_SET},
        {SC_SUBTAG_INT("E14"),      SC_RSSE14_GET, SC_RSSE14_SET},
        {SC_SUBTAG_INT("ENL"),      SC_RSSENL_GET, SC_RSSENL_SET},
        {SC_SUBTAG_INT("ENE"),      SC_RSSENE_GET, SC_RSSENE_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*GS1 Composite Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_GS1CompositeMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_CPTDEF_GET, SC_CPTDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_CPTENA_GET, SC_CPTENA_SET}, 
        {SC_SUBTAG_INT("UPC"),      SC_CPTUPC_GET, SC_CPTUPC_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Code11 Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_Code11MapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_C11DEF_GET, SC_C11DEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_C11ENA_GET, SC_C11ENA_SET}, 
        {SC_SUBTAG_INT("MIN"),      SC_C11MIN_GET, SC_C11MIN_SET}, 
        {SC_SUBTAG_INT("MAX"),      SC_C11MAX_GET, SC_C11MAX_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_C11CHK_GET, SC_C11CHK_SET},
        {SC_SUBTAG_INT("TCK"),      SC_C11TCK_GET, SC_C11TCK_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*ISBN Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_ISBNMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_ISBDEF_GET, SC_ISBDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_ISBENA_GET, SC_ISBENA_SET},  
        {SC_SUBTAG_INT("T10"),      SC_ISBT10_GET, SC_ISBT10_SET},
        {SC_SUBTAG_INT("AD2"),      SC_ISBAD2_GET, SC_ISBAD2_SET}, 
        {SC_SUBTAG_INT("AD5"),      SC_ISBAD5_GET, SC_ISBAD5_SET}, 
        {SC_SUBTAG_INT("REQ"),      SC_ISBREQ_GET, SC_ISBREQ_SET}, 
        {SC_SUBTAG_INT("SEP"),      SC_ISBSEP_GET, SC_ISBSEP_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*IND25 Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_IND25MapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_L25DEF_GET, SC_L25DEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_L25ENA_GET, SC_L25ENA_SET},  
        {SC_SUBTAG_INT("MIN"),      SC_L25MIN_GET, SC_L25MIN_SET}, 
        {SC_SUBTAG_INT("MAX"),      SC_L25MAX_GET, SC_L25MAX_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_L25CHK_GET, SC_L25CHK_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*STD25 Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_STD25MapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_S25DEF_GET, SC_S25DEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_S25ENA_GET, SC_S25ENA_SET},  
        {SC_SUBTAG_INT("MIN"),      SC_S25MIN_GET, SC_S25MIN_SET}, 
        {SC_SUBTAG_INT("MAX"),      SC_S25MAX_GET, SC_S25MAX_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_S25CHK_GET, SC_S25CHK_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Plessey Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_PlesseyMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_PLYDEF_GET, SC_PLYDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_PLYENA_GET, SC_PLYENA_SET},  
        {SC_SUBTAG_INT("MIN"),      SC_PLYMIN_GET, SC_PLYMIN_SET}, 
        {SC_SUBTAG_INT("MAX"),      SC_PLYMAX_GET, SC_PLYMAX_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_PLYCHK_GET, SC_PLYCHK_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*MSI Plessey Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_MSIMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_MSIDEF_GET, SC_MSIDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_MSIENA_GET, SC_MSIENA_SET},  
        {SC_SUBTAG_INT("MIN"),      SC_MSIMIN_GET, SC_MSIMIN_SET}, 
        {SC_SUBTAG_INT("MAX"),      SC_MSIMAX_GET, SC_MSIMAX_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_MSICHK_GET, SC_MSICHK_SET},
        {SC_SUBTAG_INT("TCK"),      SC_MSITCK_GET, SC_MSITCK_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*ISSN Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_ISSNMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_ISSDEF_GET, SC_ISSDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_ISSENA_GET, SC_ISSENA_SET},  
        {SC_SUBTAG_INT("AD2"),      SC_ISSAD2_GET, SC_ISSAD2_SET}, 
        {SC_SUBTAG_INT("AD5"),      SC_ISSAD5_GET, SC_ISSAD5_SET}, 
        {SC_SUBTAG_INT("REQ"),      SC_ISSREQ_GET, SC_ISSREQ_SET}, 
        {SC_SUBTAG_INT("SEP"),      SC_ISSSEP_GET, SC_ISSSEP_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*China Post 25 Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_ChinaPost25MapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_CHPDEF_GET, SC_CHPDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_CHPENA_GET, SC_CHPENA_SET}, 
        {SC_SUBTAG_INT("MIN"),      SC_CHPMIN_GET, SC_CHPMIN_SET}, 
        {SC_SUBTAG_INT("MAX"),      SC_CHPMAX_GET, SC_CHPMAX_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_CHPCHK_GET, SC_CHPCHK_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*AIM128 Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_AIM128MapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_AIMDEF_GET, SC_AIMDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_AIMENA_GET, SC_AIMENA_SET}, 
        {SC_SUBTAG_INT("MIN"),      SC_AIMMIN_GET, SC_AIMMIN_SET}, 
        {SC_SUBTAG_INT("MAX"),      SC_AIMMAX_GET, SC_AIMMAX_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_AIMCHK_GET, SC_AIMCHK_SET},
        {SC_SUBTAG_INT("FNC"),      SC_AIMFNC_GET, SC_AIMFNC_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*ISBT Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_ISBTMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_IBTDEF_GET, SC_IBTDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_IBTENA_GET, SC_IBTENA_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*COOP25 Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_COOP25MapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_COPDEF_GET, SC_COPDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_COPENA_GET, SC_COPENA_SET}, 
        {SC_SUBTAG_INT("MIN"),      SC_COPMIN_GET, SC_COPMIN_SET}, 
        {SC_SUBTAG_INT("MAX"),      SC_COPMAX_GET, SC_COPMAX_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_COPCHK_GET, SC_COPCHK_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Deutsche14 Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_Deutsche14MapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_D14DEF_GET, SC_D14DEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_D14ENA_GET, SC_D14ENA_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Deutsche12 Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_Deutsche12MapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_D12DEF_GET, SC_D12DEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_D12ENA_GET, SC_D12ENA_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Telepen Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_TelepenMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_TLPDEF_GET, SC_TLPDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_TLPENA_GET, SC_TLPENA_SET}, 
        {SC_SUBTAG_INT("MIN"),      SC_TLPMIN_GET, SC_TLPMIN_SET}, 
        {SC_SUBTAG_INT("MAX"),      SC_TLPMAX_GET, SC_TLPMAX_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_TLPCHK_GET, SC_TLPCHK_SET},
        {SC_SUBTAG_INT("ASC"),      SC_TLPASC_GET, SC_TLPASC_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Macro Barcode Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_MacroBarcodeMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("ROC"),      SC_MACROC_GET, SC_MACROC_SET}, 	
        {SC_SUBTAG_INT("RO1"),      SC_MACRO1_GET, SC_MACRO1_SET},  
        {SC_SUBTAG_INT("RO2"),      SC_MACRO2_GET, SC_MACRO2_SET},
        {SC_SUBTAG_INT("RO3"),      SC_MACRO3_GET, SC_MACRO3_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*PDF Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_PDFCodeMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_PDFDEF_GET, SC_PDFDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_PDFENA_GET, SC_PDFENA_SET},  
        {SC_SUBTAG_INT("MIN"),      SC_PDFMIN_GET, SC_PDFMIN_SET},
        {SC_SUBTAG_INT("MAX"),      SC_PDFMAX_GET, SC_PDFMAX_SET},
        {SC_SUBTAG_INT("DOU"),      SC_PDFDOU_GET, SC_PDFDOU_SET},
        {SC_SUBTAG_INT("INV"),      SC_PDFINV_GET, SC_PDFINV_SET},
        {SC_SUBTAG_INT("ECI"),      SC_PDFECI_GET, SC_PDFECI_SET},
        {SC_SUBTAG_INT("ENC"),      SC_PDFENC_GET, SC_PDFENC_SET},
		{"2"/*SC_TAG_PRIV*/,               0, 0},
		{SC_SUBTAG_INT("DOF"),      SC_PDFDOF_GET, SC_PDFDOF_SET},
		{"1"/*SC_TAG_NULL*/,               0, 0}
};


/*QR Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_QRCodeMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_QRCDEF_GET, SC_QRCDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_QRCENA_GET, SC_QRCENA_SET},  
        {SC_SUBTAG_INT("MIN"),      SC_QRCMIN_GET, SC_QRCMIN_SET},
        {SC_SUBTAG_INT("MAX"),      SC_QRCMAX_GET, SC_QRCMAX_SET},
        {SC_SUBTAG_INT("DOU"),      SC_QRCDOU_GET, SC_QRCDOU_SET},
        {SC_SUBTAG_INT("MCR"),      SC_QRCMCR_GET, SC_QRCMCR_SET},
        {SC_SUBTAG_INT("INV"),      SC_QRCINV_GET, SC_QRCINV_SET},
        {SC_SUBTAG_INT("ECI"),      SC_QRCECI_GET, SC_QRCECI_SET},
        {SC_SUBTAG_INT("ENC"),      SC_QRCENC_GET, SC_QRCENC_SET},
		{"2"/*SC_TAG_PRIV*/,               0, 0},
		{SC_SUBTAG_INT("DOF"),      SC_QRCDOF_GET, SC_QRCDOF_SET},
		{"1",               0, 0}
};

/*Aztec Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_AztecCodeMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_AZTDEF_GET, SC_AZTDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_AZTENA_GET, SC_AZTENA_SET},  
        {SC_SUBTAG_INT("MIN"),      SC_AZTMIN_GET, SC_AZTMIN_SET},
        {SC_SUBTAG_INT("MAX"),      SC_AZTMAX_GET, SC_AZTMAX_SET},
        {SC_SUBTAG_INT("MUL"),      SC_AZTMUL_GET, SC_AZTMUL_SET},
        {SC_SUBTAG_INT("MOD"),      SC_AZTMOD_GET, SC_AZTMOD_SET},
        {SC_SUBTAG_INT("ECI"),      SC_AZTECI_GET, SC_AZTECI_SET},
        {SC_SUBTAG_INT("ENC"),      SC_AZTENC_GET, SC_AZTENC_SET},
        {"1",               0, 0}
};

/*DM Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_DMCodeMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_DMCDEF_GET, SC_DMCDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_DMCENA_GET, SC_DMCENA_SET},  
        {SC_SUBTAG_INT("MIN"),      SC_DMCMIN_GET, SC_DMCMIN_SET},
        {SC_SUBTAG_INT("MAX"),      SC_DMCMAX_GET, SC_DMCMAX_SET},
        {SC_SUBTAG_INT("DOU"),      SC_DMCDOU_GET, SC_DMCDOU_SET},
        {SC_SUBTAG_INT("REC"),      SC_DMCREC_GET, SC_DMCREC_SET},
        {SC_SUBTAG_INT("INV"),      SC_DMCINV_GET, SC_DMCINV_SET},
        {SC_SUBTAG_INT("ECI"),      SC_DMCECI_GET, SC_DMCECI_SET},
        {SC_SUBTAG_INT("ENC"),      SC_DMCENC_GET, SC_DMCENC_SET},
		{"2"/*SC_TAG_PRIV*/,               0, 0},
		{SC_SUBTAG_INT("DOF"),      SC_DMCDOF_GET, SC_DMCDOF_SET},
		{SC_SUBTAG_INT("ADD"),      SC_DMCADD_GET, SC_DMCADD_SET},
		{"1",               0, 0}
};

/*MaxiCode Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_MaxiCodeMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_MXCDEF_GET, SC_MXCDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_MXCENA_GET, SC_MXCENA_SET},  
        {SC_SUBTAG_INT("MIN"),      SC_MXCMIN_GET, SC_MXCMIN_SET},
        {SC_SUBTAG_INT("MAX"),      SC_MXCMAX_GET, SC_MXCMAX_SET},
        {"1",               0, 0}
};

/*CSC Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_CSCodeMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_CSCDEF_GET, SC_CSCDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_CSCENA_GET, SC_CSCENA_SET},  
        {SC_SUBTAG_INT("MIN"),      SC_CSCMIN_GET, SC_CSCMIN_SET},
        {SC_SUBTAG_INT("MAX"),      SC_CSCMAX_GET, SC_CSCMAX_SET},
        {SC_SUBTAG_INT("DOU"),      SC_CSCDOU_GET, SC_CSCDOU_SET},
        {SC_SUBTAG_INT("INV"),      SC_CSCINV_GET, SC_CSCINV_SET},
        {"1",               0, 0}
};


/*GM Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_GMCodeMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_GMCDEF_GET, SC_GMCDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_GMCENA_GET, SC_GMCENA_SET},  
        {SC_SUBTAG_INT("MIN"),      SC_GMCMIN_GET, SC_GMCMIN_SET},
        {SC_SUBTAG_INT("MAX"),      SC_GMCMAX_GET, SC_GMCMAX_SET},
        {"1",               0, 0}
};

/*MicroPDF Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_MicroPDFMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_MPDDEF_GET, SC_MPDDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_MPDENA_GET, SC_MPDENA_SET},  
        {SC_SUBTAG_INT("MIN"),      SC_MPDMIN_GET, SC_MPDMIN_SET},
        {SC_SUBTAG_INT("MAX"),      SC_MPDMAX_GET, SC_MPDMAX_SET},
        {"1",               0, 0}
};

/*MicroQR Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_MicroQRMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_MQRDEF_GET, SC_MQRDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_MQRENA_GET, SC_MQRENA_SET},  
        {SC_SUBTAG_INT("MIN"),      SC_MQRMIN_GET, SC_MQRMIN_SET},
        {SC_SUBTAG_INT("MAX"),      SC_MQRMAX_GET, SC_MQRMAX_SET},
        {"1",               0, 0}
};


/*Specific OCR-B Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_SpecificOCRBMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_SOBDEF_GET, SC_SOBDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_SOBENA_GET, SC_SOBENA_SET},  
		{"2"/*SC_TAG_PRIV*/,               0, 0},
        {SC_SUBTAG_INT("MIN"),      SC_SOBMIN_GET, SC_SOBMIN_SET},
        {SC_SUBTAG_INT("MAX"),      SC_SOBMAX_GET, SC_SOBMAX_SET},
        {"1",               0, 0}
};


/*Passport OCR Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_PassportOCRMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_PASDEF_GET, SC_PASDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_PASENA_GET, SC_PASENA_SET}, 
        { "1",               0, 0}
};

/*USPS Postnet Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_USPSPostnetMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_PNTDEF_GET, SC_PNTDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_PNTENA_GET, SC_PNTENA_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_PNTCHK_GET, SC_PNTCHK_SET}, 
        { "1",               0, 0}
};

/*USPS Intelligent Mail Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_USPSIntelliMailMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_ILGDEF_GET, SC_ILGDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_ILGENA_GET, SC_ILGENA_SET},
        { "1",               0, 0}
};

/*Royal Mail Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_RoyalMailMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_ROYDEF_GET, SC_ROYDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_ROYENA_GET, SC_ROYENA_SET},
        { "1",               0, 0}
};

/*USPS Planet Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_USPSPlanetMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_PLADEF_GET, SC_PLADEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_PLAENA_GET, SC_PLAENA_SET}, 
        {SC_SUBTAG_INT("CHK"),      SC_PLACHK_GET, SC_PLACHK_SET},
        { "1",               0, 0}
};

/*KIX Post Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_KIXPostMapTab[] = 
{
        { "0",                         SC_QueryAll, 0},
        {SC_SUBTAG_INT("DEF"),      SC_KIXDEF_GET, SC_KIXDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_KIXENA_GET, SC_KIXENA_SET},
        { "1",               0, 0}
};

/*Australian Postal Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_AustPostalMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_APLDEF_GET, SC_APLDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_APLENA_GET, SC_APLENA_SET},
        {"1",               0, 0}
};

/*Data Formatting Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_DataFormatMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DEF"),      SC_DFMDEF_GET, SC_DFMDEF_SET}, 	
        {SC_SUBTAG_INT("ENA"),      SC_DFMENA_GET, SC_DFMENA_SET}, 
        {SC_SUBTAG_INT("SET"),      SC_DFMSET_GET, SC_DFMSET_SET}, 
        {SC_SUBTAG_INT("CLR"),      SC_DFMCLR_GET, SC_DFMCLR_SET}, 
        {SC_SUBTAG_INT("CAL"),      SC_DFMCAL_GET, SC_DFMCAL_SET}, 
        {SC_SUBTAG_INT("USE"),      SC_DFMUSE_GET, SC_DFMUSE_SET}, 
        {SC_SUBTAG_INT("SIN"),      SC_DFMSIN_GET, SC_DFMSIN_SET}, 
        {SC_SUBTAG_INT("TON"),      SC_DFMTON_GET, SC_DFMTON_SET}, 
        {SC_SUBTAG_INT("QCU"),      SC_DFMQCU_GET, SC_DFMQCU_SET}, 
        {SC_SUBTAG_INT("QFA"),      SC_DFMQFA_GET, SC_DFMQFA_SET}, 
		{"2"/*SC_TAG_PRIV*/,               0, 0},
        {SC_SUBTAG_INT("QSZ"),      SC_DFMQSZ_GET, SC_DFMQSZ_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Driver Code Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_DriverCodeMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("SET"),      SC_DLPSET_GET, SC_DLPSET_SET}, 	
        {SC_SUBTAG_INT("CLR"),      SC_DLPCLR_GET, SC_DLPCLR_SET}, 
        {SC_SUBTAG_INT("QSZ"),      SC_DLPQSZ_GET, SC_DLPQSZ_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Package Mode Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_PackageModeMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("KAG"),      SC_PACKAG_GET, SC_PACKAG_SET}, 	
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Message Extract Mode Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_MsgExtractMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("RAC"),      SC_EXTRAC_GET, SC_EXTRAC_SET}, 	
        {SC_SUBTAG_INT("ADD"),      SC_EXTADD_GET, SC_EXTADD_SET},
        {SC_SUBTAG_INT("CLR"),      SC_EXTCLR_GET, SC_EXTCLR_SET},
        {SC_SUBTAG_INT("CLO"),      SC_EXTCLO_GET, SC_EXTCLO_SET},
        {SC_SUBTAG_INT("CAL"),      SC_EXTCAL_GET, SC_EXTCAL_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Image Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_ImageMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("GWH"),      SC_IMGGWH_GET, SC_IMGGWH_SET}, 
        {SC_SUBTAG_INT("GET"),      SC_IMGGET_GET, SC_IMGGET_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Setup Sub Tag 执行函数映射表TAG:SET*/
const sc_subtagmap_st  SC_SetupMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("UPE"),      SC_SETUPE_GET, SC_SETUPE_SET}, 	
        {SC_SUBTAG_INT("UPN"),      SC_SETUPN_GET, SC_SETUPN_SET}, 
        {SC_SUBTAG_INT("UPT"),      SC_SETUPT_GET, SC_SETUPT_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Setup Forbid Sub Tag 执行函数映射表TAG:FOB*/
const sc_subtagmap_st  SC_SetForbidMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("SET"),      SC_FOBSET_GET, SC_FOBSET_SET}, 	
        {SC_SUBTAG_INT("QRY"),      SC_FOBQRY_GET, SC_FOBQRY_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Scan Mode Sub Tag 执行函数映射表TAG:SCN*/
const sc_subtagmap_st  SC_ScanModeMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("MOD"),      SC_SCNMOD_GET, SC_SCNMOD_SET}, 	
        {SC_SUBTAG_INT("TRG"),      SC_SCNTRG_GET, SC_SCNTRG_SET}, 	
        {SC_SUBTAG_INT("CTM"),      SC_SCNCTM_GET, SC_SCNCTM_SET}, 
        {SC_SUBTAG_INT("INV"),      SC_SCNINV_GET, SC_SCNINV_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*One Reading Timeout Sub Tag 执行函数映射表TAG:ORT*/
const sc_subtagmap_st  SC_OneReadTmoMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("SET"),      SC_ORTSET_GET, SC_ORTSET_SET}, 	
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Reread Set Sub Tag 执行函数映射表TGA:RRD*/
const sc_subtagmap_st  SC_RereadSetMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("ENA"),      SC_RRDENA_GET, SC_RRDENA_SET}, 	
        {SC_SUBTAG_INT("DUR"),      SC_RRDDUR_GET, SC_RRDDUR_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Flash On/Off Sub Tag 执行函数映射表TAG:FON*/
const sc_subtagmap_st  SC_FlashTimeMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DUR"),      SC_FONDUR_GET, SC_FONDUR_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};
const sc_subtagmap_st  SC_FlashOffTimeMapTab[] = 
{ 
		{"0",                         SC_QueryAll, 0}, 
		{SC_SUBTAG_INT("DUR"),      SC_FOFDUR_GET, SC_FOFDUR_SET},
		{"1"/*SC_TAG_NULL*/,              0}
};

/*Interface Sub Tag 执行函数映射表TAG:INT*/
const sc_subtagmap_st  SC_InterfaceMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("ERF"),      SC_INTERF_GET, SC_INTERF_SET}, 
/*
        {SC_SUBTAG_INT("232"),      SC_INF232_GET, SC_INF232_SET}, 
        {SC_SUBTAG_INT("DPP"),      SC_INFDPP_GET, SC_INFDPP_SET},
        {SC_SUBTAG_INT("UCM"),      SC_INFUCM_GET, SC_INFUCM_SET},
        {SC_SUBTAG_INT("KBW"),      SC_INFKBW_GET, SC_INFKBW_SET},
        {SC_SUBTAG_INT("PS2"),      SC_INFPS2_GET, SC_INFPS2_SET},
        {SC_SUBTAG_INT("POS"),      SC_INFPOS_GET, SC_INFPOS_SET},
        {SC_SUBTAG_INT("IPT"),      SC_INFIPT_GET, SC_INFIPT_SET},
        {SC_SUBTAG_INT("IPH"),      SC_INFIPH_GET, SC_INFIPH_SET},
        {SC_SUBTAG_INT("CDC"),      SC_INFCDC_GET, SC_INFCDC_SET},
        {SC_SUBTAG_INT("BSP"),      SC_INFBSP_GET, SC_INFBSP_SET},
        {SC_SUBTAG_INT("BHD"),      SC_INFBHD_GET, SC_INFBHD_SET},
*/
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*RS232 Sub Tag 执行函数映射表TAG:232*/
const sc_subtagmap_st  SC_RS232MapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("BAD"),      SC_232BAD_GET, SC_232BAD_SET}, 	
        {SC_SUBTAG_INT("PAR"),      SC_232PAR_GET, SC_232PAR_SET}, 	
        {SC_SUBTAG_INT("STP"),      SC_232STP_GET, SC_232STP_SET}, 
        {SC_SUBTAG_INT("DAT"),      SC_232DAT_GET, SC_232DAT_SET}, 
        {SC_SUBTAG_INT("AFL"),      SC_232AFL_GET, SC_232AFL_SET}, 
        {SC_SUBTAG_INT("FLW"),      SC_232FLW_GET, SC_232FLW_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*HID KBW Sub Tag 执行函数映射表TAG:KBW*/
const sc_subtagmap_st  SC_HIDKBWMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("CTY"),      SC_KBWCTY_GET, SC_KBWCTY_SET}, 	
        {SC_SUBTAG_INT("BUC"),      SC_KBWBUC_GET, SC_KBWBUC_SET}, 
        {SC_SUBTAG_INT("ALT"),      SC_KBWALT_GET, SC_KBWALT_SET}, 
        {SC_SUBTAG_INT("ALK"),      SC_KBWALK_GET, SC_KBWALK_SET}, 
        {SC_SUBTAG_INT("FKM"),      SC_KBWFKM_GET, SC_KBWFKM_SET}, 
        {SC_SUBTAG_INT("DLY"),      SC_KBWDLY_GET, SC_KBWDLY_SET}, 
        {SC_SUBTAG_INT("CAP"),      SC_KBWCAP_GET, SC_KBWCAP_SET}, 
        {SC_SUBTAG_INT("CAS"),      SC_KBWCAS_GET, SC_KBWCAS_SET}, 
        {SC_SUBTAG_INT("NUM"),      SC_KBWNUM_GET, SC_KBWNUM_SET}, 
        {SC_SUBTAG_INT("NCH"),      SC_KBWNCH_GET, SC_KBWNCH_SET}, 
        {SC_SUBTAG_INT("FAS"),      SC_KBWFAS_GET, SC_KBWFAS_SET}, 
        {SC_SUBTAG_INT("POR"),      SC_KBWPOR_GET, SC_KBWPOR_SET}, 
        {SC_SUBTAG_INT("CPG"),      SC_KBWCPG_GET, SC_KBWCPG_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*PS2 KBW Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_PS2KBWMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("SLV"),      SC_PS2SLV_GET, SC_PS2SLV_SET}, 	
        {SC_SUBTAG_INT("CLK"),      SC_PS2CLK_GET, SC_PS2CLK_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};


/*Power On Buzzer Sub Tag 执行函数映射表TAG:PWB*/
const sc_subtagmap_st  SC_PwrOnBuzMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("ENA"),      SC_PWBENA_GET, SC_PWBENA_SET}, 	
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Good Read Beep Sub Tag 执行函数映射表TAG:GRB*/
const sc_subtagmap_st  SC_GoodRdBeepMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("ENA"),      SC_GRBENA_GET, SC_GRBENA_SET}, 	
        {SC_SUBTAG_INT("VOL"),      SC_GRBVOL_GET, SC_GRBVOL_SET}, 	
        {SC_SUBTAG_INT("DUR"),      SC_GRBDUR_GET, SC_GRBDUR_SET}, 	
        {SC_SUBTAG_INT("FRQ"),      SC_GRBFRQ_GET, SC_GRBFRQ_SET}, 	
        {SC_SUBTAG_INT("NUM"),      SC_GRBNUM_GET, SC_GRBNUM_SET},
        {SC_SUBTAG_INT("ITV"),      SC_GRBITV_GET, SC_GRBITV_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Bad Read Beep Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_BadRdBeepMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("ENA"),      SC_BRBENA_GET, SC_BRBENA_SET}, 	
        {SC_SUBTAG_INT("VOL"),      SC_BRBVOL_GET, SC_BRBVOL_SET}, 	
        {SC_SUBTAG_INT("DUR"),      SC_BRBDUR_GET, SC_BRBDUR_SET}, 	
        {SC_SUBTAG_INT("FRQ"),      SC_BRBFRQ_GET, SC_BRBFRQ_SET}, 	
        {SC_SUBTAG_INT("NUM"),      SC_BRBNUM_GET, SC_BRBNUM_SET},
        {SC_SUBTAG_INT("ITV"),      SC_BRBITV_GET, SC_BRBITV_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Voice Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_VoiceMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("CEP"),      SC_VOICEP_GET, SC_VOICEP_SET},   
        {SC_SUBTAG_INT("CEI"),      SC_VOICEI_GET, SC_VOICEI_SET},  
        {SC_SUBTAG_INT("CET"),      SC_VOICET_GET, SC_VOICET_SET},
        {SC_SUBTAG_INT("CEM"),      SC_VOICEM_GET, SC_VOICEM_SET},
        {SC_SUBTAG_INT("CEV"),      SC_VOICEV_GET, SC_VOICEV_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Good Read LED Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_GoodRdLedMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("ENA"),      SC_GRLENA_GET, SC_GRLENA_SET}, 		
        {SC_SUBTAG_INT("DUR"),      SC_GRLDUR_GET, SC_GRLDUR_SET}, 	
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Good Read Vibrate Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_GoodRdVibrateMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("ENA"),      SC_GRVENA_GET, SC_GRVENA_SET}, 		
        {SC_SUBTAG_INT("DUR"),      SC_GRVDUR_GET, SC_GRVDUR_SET}, 	
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Power On LED Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_PowerOnLedMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("ENA"),      SC_POLENA_GET, SC_POLENA_SET}, 			
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Illumination Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_IlluminationMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("SCN"),      SC_ILLSCN_GET, SC_ILLSCN_SET}, 	
        {SC_SUBTAG_INT("CLR"),      SC_ILLCLR_GET, SC_ILLCLR_SET}, 
        {SC_SUBTAG_INT("PST"),      SC_ILLPST_GET, SC_ILLPST_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Aimer Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_AimerMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("ENA"),      SC_AMLENA_GET, SC_AMLENA_SET}, 	
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Sense Control Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_SenseCtrlMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("LVL"),      SC_SENLVL_GET, SC_SENLVL_SET}, 	
        {SC_SUBTAG_INT("AGR"),      SC_SENAGR_GET, SC_SENAGR_SET}, 	
        {SC_SUBTAG_INT("IST"),      SC_SENIST_GET, SC_SENIST_SET},
        {SC_SUBTAG_INT("ILL"),      SC_SENILL_GET, SC_SENILL_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Exposure Set Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_ExposureSetMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("LVL"),      SC_EXPLVL_GET, SC_EXPLVL_SET}, 	
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Decode Area Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_DecodeAreaMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("ENA"),      SC_CADENA_GET, SC_CADENA_SET}, 		
        {SC_SUBTAG_INT("TOP"),      SC_CADTOP_GET, SC_CADTOP_SET}, 	
        {SC_SUBTAG_INT("BOT"),      SC_CADBOT_GET, SC_CADBOT_SET}, 	
        {SC_SUBTAG_INT("LEF"),      SC_CADLEF_GET, SC_CADLEF_SET}, 	
        {SC_SUBTAG_INT("RIG"),      SC_CADRIG_GET, SC_CADRIG_SET}, 		
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*1D Decode Area Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_DecAreaParaMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("LVL"),      SC_CDALVL_GET, SC_CDALVL_SET}, 		
        {SC_SUBTAG_INT("INV"),      SC_CDAINV_GET, SC_CDAINV_SET}, 			
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Not Good Read Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_NotGoodReadMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("ENA"),      SC_NGRENA_GET, SC_NGRENA_SET}, 	
        {SC_SUBTAG_INT("SET"),      SC_NGRSET_GET, SC_NGRSET_SET},
	  {"2"/*SC_TAG_PRIV*/,               0, 0},
        {SC_SUBTAG_INT("QRY"),      SC_NGRQRY_GET, SC_NGRQRY_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Mirror Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_MirrorMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("ROR"),      SC_MIRROR_GET, SC_MIRROR_SET}, 	
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Power Mode Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_PowerModeMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("OFF"),      SC_PWROFF_GET, SC_PWROFF_SET}, 	
        {SC_SUBTAG_INT("IDL"),      SC_PWRIDL_GET, SC_PWRIDL_SET},
        {SC_SUBTAG_INT("SLP"),      SC_PWRSLP_GET, SC_PWRSLP_SET},
        {SC_SUBTAG_INT("DSP"),      SC_PWRDSP_GET, SC_PWRDSP_SET},
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Auto Sleep Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_AutoSleepMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DLE"),      SC_ATIDLE_GET, SC_ATIDLE_SET}, 	
        {SC_SUBTAG_INT("DUR"),      SC_ATIDUR_GET, SC_ATIDUR_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Test Mode Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_TestModeMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("TMD"),      SC_TESTMD_GET, SC_TESTMD_SET}, 	 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Reboot Mode Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_RebootModeMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("OOT"),      SC_REBOOT_GET, SC_REBOOT_SET}, 	 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*1D Safe Level Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_SafeLevelMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("LVL"),      SC_SAFLVL_GET, SC_SAFLVL_SET}, 	
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*All Set Sub Tag 执行函数映射表TAG:ALL*/
const sc_subtagmap_st  SC_AllSetMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("ENA"),      SC_ALLENA_GET, SC_ALLENA_SET}, 	
        {SC_SUBTAG_INT("1DC"),      SC_ALL1DC_GET, SC_ALL1DC_SET}, 
        {SC_SUBTAG_INT("2DC"),      SC_ALL2DC_GET, SC_ALL2DC_SET}, 
        {SC_SUBTAG_INT("PST"),      SC_ALLPST_GET, SC_ALLPST_SET}, 
        {SC_SUBTAG_INT("INV"),      SC_ALLINV_GET, SC_ALLINV_SET}, 
        {SC_SUBTAG_INT("ADD"),      SC_ALLADD_GET, SC_ALLADD_SET}, 
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*All 1D Set Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_All1DSetMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("DOU"),      SC_A1DDOU_GET, SC_A1DDOU_SET}, 	
        {"1"/*SC_TAG_NULL*/,               0, 0}
};


/*Message Edit All Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_MsgEditAllMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("ENA"),      SC_APSENA_GET, SC_APSENA_SET}, 	
        {"1",               0, 0}
};


/*Prefix Sequence Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_PrefixSeqMapTab[] = 
{ 
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("SEQ"),      SC_PRESEQ_GET, SC_PRESEQ_SET}, 	
        {"1"/*SC_TAG_NULL*/,               0, 0}
};

/*Prefix Set Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_PrefixSetMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("ENA"),      SC_CPRENA_GET, SC_CPRENA_SET},     
        {SC_SUBTAG_INT("SET"),      SC_CPRSET_GET, SC_CPRSET_SET}, 
		{"2"/*SC_TAG_PRIV*/,               0, 0},
        {SC_SUBTAG_INT("QSZ"),      SC_CPRQSZ_GET, SC_CPRQSZ_SET}, 
        {"1",               0, 0}
};


/*AIMID Set Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_AIMIDSetMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("ENA"),      SC_AIDENA_GET, SC_AIDENA_SET},     
        {"1",               0, 0}
};


/*Code ID Set Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_CodeIDSetMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("ENA"),      SC_CIDENA_GET, SC_CIDENA_SET},     
        {SC_SUBTAG_INT("DEF"),      SC_CIDDEF_GET, SC_CIDDEF_SET}, 
        //{SC_SUBTAG_INT("QSZ"),      SC_CIDQSZ_GET, SC_CIDQSZ_SET},   
        //{SC_SUBTAG_INT("QVL"),      SC_CIDQVL_GET, SC_CIDQVL_SET},  
        {SC_SUBTAG_INT("002"),      SC_CID002_GET, SC_CID002_SET},
        {SC_SUBTAG_INT("003"),      SC_CID003_GET, SC_CID003_SET},
        {SC_SUBTAG_INT("004"),      SC_CID004_GET, SC_CID004_SET},
        {SC_SUBTAG_INT("005"),      SC_CID005_GET, SC_CID005_SET},
        {SC_SUBTAG_INT("006"),      SC_CID006_GET, SC_CID006_SET},
        {SC_SUBTAG_INT("007"),      SC_CID007_GET, SC_CID007_SET},
        {SC_SUBTAG_INT("008"),      SC_CID008_GET, SC_CID008_SET},
        {SC_SUBTAG_INT("009"),      SC_CID009_GET, SC_CID009_SET},
        {SC_SUBTAG_INT("010"),      SC_CID010_GET, SC_CID010_SET},
        {SC_SUBTAG_INT("011"),      SC_CID011_GET, SC_CID011_SET},
        {SC_SUBTAG_INT("013"),      SC_CID013_GET, SC_CID013_SET},
        {SC_SUBTAG_INT("015"),      SC_CID015_GET, SC_CID015_SET},
        {SC_SUBTAG_INT("017"),      SC_CID017_GET, SC_CID017_SET},
        {SC_SUBTAG_INT("019"),      SC_CID019_GET, SC_CID019_SET},
        {SC_SUBTAG_INT("020"),      SC_CID020_GET, SC_CID020_SET},
        {SC_SUBTAG_INT("021"),      SC_CID021_GET, SC_CID021_SET},
        {SC_SUBTAG_INT("022"),      SC_CID022_GET, SC_CID022_SET},
        {SC_SUBTAG_INT("023"),      SC_CID023_GET, SC_CID023_SET},
        {SC_SUBTAG_INT("024"),      SC_CID024_GET, SC_CID024_SET},
        {SC_SUBTAG_INT("025"),      SC_CID025_GET, SC_CID025_SET},
        {SC_SUBTAG_INT("026"),      SC_CID026_GET, SC_CID026_SET},
        {SC_SUBTAG_INT("027"),      SC_CID027_GET, SC_CID027_SET},
        {SC_SUBTAG_INT("028"),      SC_CID028_GET, SC_CID028_SET},
        {SC_SUBTAG_INT("029"),      SC_CID029_GET, SC_CID029_SET},
        {SC_SUBTAG_INT("030"),      SC_CID030_GET, SC_CID030_SET},
        {SC_SUBTAG_INT("031"),      SC_CID031_GET, SC_CID031_SET},
        {SC_SUBTAG_INT("128"),      SC_CID128_GET, SC_CID128_SET},
        {SC_SUBTAG_INT("129"),      SC_CID129_GET, SC_CID129_SET},
        {SC_SUBTAG_INT("130"),      SC_CID130_GET, SC_CID130_SET},
        {SC_SUBTAG_INT("032"),      SC_CID032_GET, SC_CID032_SET},
        {SC_SUBTAG_INT("033"),      SC_CID033_GET, SC_CID033_SET},
        {SC_SUBTAG_INT("034"),      SC_CID034_GET, SC_CID034_SET},
        {SC_SUBTAG_INT("035"),      SC_CID035_GET, SC_CID035_SET},
        {SC_SUBTAG_INT("036"),      SC_CID036_GET, SC_CID036_SET},
        {SC_SUBTAG_INT("039"),      SC_CID039_GET, SC_CID039_SET},
        {SC_SUBTAG_INT("041"),      SC_CID041_GET, SC_CID041_SET},
        {SC_SUBTAG_INT("042"),      SC_CID042_GET, SC_CID042_SET},
        {SC_SUBTAG_INT("043"),      SC_CID043_GET, SC_CID043_SET},
        {SC_SUBTAG_INT("096"),      SC_CID096_GET, SC_CID096_SET},
        {SC_SUBTAG_INT("097"),      SC_CID097_GET, SC_CID097_SET},
        {SC_SUBTAG_INT("098"),      SC_CID098_GET, SC_CID098_SET},
        {SC_SUBTAG_INT("099"),      SC_CID099_GET, SC_CID099_SET},
        {SC_SUBTAG_INT("100"),      SC_CID100_GET, SC_CID100_SET},
        {SC_SUBTAG_INT("101"),      SC_CID101_GET, SC_CID101_SET},
		{"2"/*SC_TAG_PRIV*/,               0, 0},
		{SC_SUBTAG_INT("QSZ"),      SC_CIDQSZ_GET, SC_CIDQSZ_SET},   
		{SC_SUBTAG_INT("QVL"),      SC_CIDQVL_GET, SC_CIDQVL_SET},
        {"1",               0, 0}
};


/*Suffix Set Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_SuffixSetMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("ENA"),      SC_CSUENA_GET, SC_CSUENA_SET},     
        {SC_SUBTAG_INT("SET"),      SC_CSUSET_GET, SC_CSUSET_SET}, 
		{"2"/*SC_TAG_PRIV*/,               0, 0},
        {SC_SUBTAG_INT("QSZ"),      SC_CSUQSZ_GET, SC_CSUQSZ_SET}, 
        {"1",               0, 0}
};

/*Terminator Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_TerminatorMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("ENA"),      SC_TSUENA_GET, SC_TSUENA_SET},
        {SC_SUBTAG_INT("SET"),      SC_TSUSET_GET, SC_TSUSET_SET},  
		{"2"/*SC_TAG_PRIV*/,               0, 0},
        {SC_SUBTAG_INT("QSZ"),      SC_TSUQSZ_GET, SC_TSUQSZ_SET},  
        {"1",               0, 0}
};


/*Batch Set Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_BatchSetMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("CHS"),      SC_Batch_Get, SC_Batch_Set}, 
        {"1",               0, 0}
};

/*PSN Edit Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_PsnEditMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("EDT"),      SC_PSNEDT_GET, SC_PSNEDT_SET}, 
        {SC_SUBTAG_INT("DEL"),      SC_PSNDEL_GET, SC_PSNDEL_SET}, 
		{SC_SUBTAG_INT("QSZ"),      SC_PSNQSZ_GET, SC_PSNQSZ_SET}, 
        {"1",               0, 0}
};

/*ESN Edit Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_EsnEditMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("SET"),      SC_ESNSET_GET, SC_ESNSET_SET}, 
        {SC_SUBTAG_INT("CLR"),      SC_ESNCLR_GET, SC_ESNCLR_SET}, 
        {SC_SUBTAG_INT("QSZ"),      SC_ESNQSZ_GET, SC_ESNQSZ_SET},
        {"1",               0, 0}
};

/*Date Edit Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_DateEditMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("EDT"),      SC_DATEDT_GET, SC_DATEDT_SET}, 
        {SC_SUBTAG_INT("DEL"),      SC_DATDEL_GET, SC_DATDEL_SET}, 
        {SC_SUBTAG_INT("QSZ"),      SC_DATQSZ_GET, SC_DATQSZ_SET},
        {"1",               0, 0}
};

/*Product Name Edit Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_ProductNameEditMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("EDT"),      SC_PDNEDT_GET, SC_PDNEDT_SET}, 
        {SC_SUBTAG_INT("DEL"),      SC_PDNDEL_GET, SC_PDNDEL_SET}, 
        {SC_SUBTAG_INT("QSZ"),      SC_PDNQSZ_GET, SC_PDNQSZ_SET},
        {"1",               0, 0}
};

/*Manufacture Name Edit Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_ManufactureNameEditMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("EDT"),      SC_MFNEDT_GET, SC_MFNEDT_SET}, 
        {SC_SUBTAG_INT("DEL"),      SC_MFNDEL_GET, SC_MFNDEL_SET},
        {SC_SUBTAG_INT("QSZ"),      SC_MFNQSZ_GET, SC_MFNQSZ_SET},
        {"1",               0, 0}
};

/*Hardware version Edit Sub Tag 执行函数映射表HWV*/
const sc_subtagmap_st  SC_HardwareVerEditMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("EDT"),      SC_HWVEDT_GET, SC_HWVEDT_SET}, 
        {SC_SUBTAG_INT("DEL"),      SC_HWVDEL_GET, SC_HWVDEL_SET}, 
        {SC_SUBTAG_INT("QSZ"),      SC_HWVQSZ_GET, SC_HWVQSZ_SET},
        {"1",               0, 0}
};

/*DownLoad Set Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_DownLoadSetMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("ENA"),      SC_DWNENA_GET, SC_DWNENA_SET}, 
        {SC_SUBTAG_INT("DEL"),      SC_DWNDEL_GET, SC_DWNDEL_SET}, 
        {"1",               0, 0}
};

/*Depth of Field Set Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_DepthofFieldSetMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("LVL"),      SC_DOFLVL_GET, SC_DOFLVL_SET},  
        {"1",               0, 0}
};

/*Depth of Field Del Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_DepthofFieldDelMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("DOF"),      SC_DELDOF_GET, SC_DELDOF_SET},  
        {"1",               0, 0}
};

/*Hardware Run Param Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_HardwareRunParamMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("FRQ"),      SC_HWRFRQ_GET, SC_HWRFRQ_SET},  
        {"1",               0, 0}
};

/*Running Mode Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_RunningModeMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("WEL"),      SC_RUNWEL_GET, SC_RUNWEL_SET},  
        {"1",               0, 0}
};

/*USB Enum Mode Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_USBEnumModeMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("USN"),      SC_PIDUSN_GET, SC_PIDUSN_SET},  
        {"1",               0, 0}
};

/*USB Auto Adapt Mode Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_USBAutoAdaptMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("OUR"),      SC_AUTOUR_GET, SC_AUTOUR_SET},  
        {"1",               0, 0}
};

/*USB Auto Adapt Mode Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_BufferQueryMapTab[] = 
{
        {"0",                         SC_QueryAll, 0},          
        {SC_SUBTAG_INT("QRY"),      SC_BUFQRY_GET, SC_BUFQRY_SET},  
        {"1",               0, 0}
};

/*数据码 Sub Tag 执行函数映射表*/
const sc_subtagmap_st  SC_DigitMapTab[] = 
{
        {"0",                         SC_QueryAll, 0}, 
        {SC_SUBTAG_INT("IT0"),      SC_NullGet, SC_Digit_0},          
        {SC_SUBTAG_INT("IT1"),      SC_NullGet, SC_Digit_1},
        {SC_SUBTAG_INT("IT2"),      SC_NullGet, SC_Digit_2},
        {SC_SUBTAG_INT("IT3"),      SC_NullGet, SC_Digit_3},
        {SC_SUBTAG_INT("IT4"),      SC_NullGet, SC_Digit_4},
        {SC_SUBTAG_INT("IT5"),      SC_NullGet, SC_Digit_5},
        {SC_SUBTAG_INT("IT6"),      SC_NullGet, SC_Digit_6},
        {SC_SUBTAG_INT("IT7"),      SC_NullGet, SC_Digit_7},
        {SC_SUBTAG_INT("IT8"),      SC_NullGet, SC_Digit_8},
        {SC_SUBTAG_INT("IT9"),      SC_NullGet, SC_Digit_9},
        {SC_SUBTAG_INT("ITA"),      SC_NullGet, SC_Digit_A},
        {SC_SUBTAG_INT("ITB"),      SC_NullGet, SC_Digit_B},
        {SC_SUBTAG_INT("ITC"),      SC_NullGet, SC_Digit_C},
        {SC_SUBTAG_INT("ITD"),      SC_NullGet, SC_Digit_D},
        {SC_SUBTAG_INT("ITE"),      SC_NullGet, SC_Digit_E},
        {SC_SUBTAG_INT("ITF"),      SC_NullGet, SC_Digit_F},            
        {SC_SUBTAG_INT("SAV"),      SC_NullGet, SC_Digit_Save},         
        {SC_SUBTAG_INT("DEL"),      SC_NullGet, SC_Digit_Delete},      
        {SC_SUBTAG_INT("DAL"),      SC_NullGet, SC_Digit_DelAll},      
        {SC_SUBTAG_INT("CAN"),      SC_NullGet, SC_Digit_Cancel},     

        {"1"/*SC_TAG_NULL*/,              0, 0}
};

/*Main Tag 映射表*/
const sc_maintagmap_st SC_MainTagMapTab[] = 
{
        {"0",                        (void *)SC_QueryAllMapTab},

        //默认设置
        {SC_TAG_INT("FAC"),        (void *)SC_FactoryCfgMapTab},
     
        {SC_TAG_INT("CUS"),        (void *)SC_UserConfigMapTab},

   
        //查询
        {SC_TAG_INT("QRY"),        (void *)SC_SysMsgMapTab}, 	

        //设置码
        {SC_TAG_INT("SET"),        (void *)SC_SetupMapTab},

        //读码模式
        {SC_TAG_INT("SCN"),        (void *)SC_ScanModeMapTab},

        //一次读码时间
       {SC_TAG_INT("ORT"),        (void *)SC_OneReadTmoMapTab},

        //重复识读同一条码
         {SC_TAG_INT("RRD"),        (void *)SC_RereadSetMapTab},

        //Flash On/Off
        {SC_TAG_INT("FON"),        (void *)SC_FlashTimeMapTab},

        //通讯接口
        {SC_TAG_INT("INT"),        (void *)SC_InterfaceMapTab},
        {SC_TAG_INT("232"),        (void *)SC_RS232MapTab},
        {SC_TAG_INT("KBW"),        (void *)SC_HIDKBWMapTab},
        {SC_TAG_INT("PS2"),        (void *)SC_PS2KBWMapTab},

        //开机提示音
        {SC_TAG_INT("PWB"),        (void *)SC_PwrOnBuzMapTab},

        //Good Read Beep
        {SC_TAG_INT("GRB"),        (void *)SC_GoodRdBeepMapTab},

        //Bad Read Beep
        {SC_TAG_INT("BRB"),        (void *)SC_BadRdBeepMapTab},

        //Voice
        {SC_TAG_INT("VOI"),        (void *)SC_VoiceMapTab},

        //Good Read LED
        {SC_TAG_INT("GRL"),        (void *)SC_GoodRdLedMapTab},

        //Good Read Vibrate
        {SC_TAG_INT("GRV"),        (void *)SC_GoodRdVibrateMapTab},

        //Power On LED
       {SC_TAG_INT("POL"),        (void *)SC_PowerOnLedMapTab},

        //Illumination
        {SC_TAG_INT("ILL"),        (void *)SC_IlluminationMapTab},

        //Aimer
        {SC_TAG_INT("AML"),        (void *)SC_AimerMapTab},

        //Sense Control
        {SC_TAG_INT("SEN"),        (void *)SC_SenseCtrlMapTab},

        //Exposure
         {SC_TAG_INT("EXP"),        (void *)SC_ExposureSetMapTab},

        //Decode Area
      {SC_TAG_INT("CAD"),        (void *)SC_DecodeAreaMapTab},

        //Decode Area(1D)
          {SC_TAG_INT("CDA"),        (void *)SC_DecAreaParaMapTab},

        //Not Good Read
        {SC_TAG_INT("NGR"),        (void *)SC_NotGoodReadMapTab},

        //Mirror
        {SC_TAG_INT("MIR"),        (void *)SC_MirrorMapTab},

        //Power Mode
       {SC_TAG_INT("PWR"),        (void *)SC_PowerModeMapTab},

        //Auto Sleep Mode
        {SC_TAG_INT("ATI"),        (void *)SC_AutoSleepMapTab},

        //Test Mode
        //{SC_TAG_INT("TES"),        (void *)SC_TestModeMapTab},

        //Reboot Mode
        //{SC_TAG_INT("REB"),        (void *)SC_RebootModeMapTab},

        //Safe Level(1D)
        {SC_TAG_INT("SAF"),        (void *)SC_SafeLevelMapTab},

        //ALL SET
        {SC_TAG_INT("ALL"),        (void *)SC_AllSetMapTab},
        {SC_TAG_INT("A1D"),        (void *)SC_All1DSetMapTab},
		
        //信息编辑综合设置
        {SC_TAG_INT("APS"),        (void *)SC_MsgEditAllMapTab},

        //Prefix Sequence
        {SC_TAG_INT("PRE"),        (void *)SC_PrefixSeqMapTab},

        //Prefix Set
        {SC_TAG_INT("CPR"),        (void *)SC_PrefixSetMapTab},

        //AIMID Set
        {SC_TAG_INT("AID"),        (void *)SC_AIMIDSetMapTab},

        //Code ID Set
        {SC_TAG_INT("CID"),        (void *)SC_CodeIDSetMapTab},
	
        //Suffix Set
        {SC_TAG_INT("CSU"),        (void *)SC_SuffixSetMapTab},
        
        //Terminator Set
        {SC_TAG_INT("TSU"),        (void *)SC_TerminatorMapTab}, 
     
        //码制
        {SC_TAG_INT("128"),        (void *)SC_Code128MapTab}, 
        {SC_TAG_INT("EA8"),        (void *)SC_EAN8MapTab}, 
        {SC_TAG_INT("E13"),        (void *)SC_EAN13MapTab}, 		
        {SC_TAG_INT("UPE"),        (void *)SC_UPCEMapTab}, 	
        {SC_TAG_INT("UPA"),        (void *)SC_UPCAMapTab}, 	
        {SC_TAG_INT("CPN"),        (void *)SC_CouponMapTab}, 	
        {SC_TAG_INT("I25"),        (void *)SC_ITF25MapTab}, 			
	  {SC_TAG_INT("FEB"),        (void *)SC_FebrabanMapTab}, 
        {SC_TAG_INT("I14"),        (void *)SC_ITF14MapTab}, 
        {SC_TAG_INT("IT6"),        (void *)SC_ITF6MapTab},
        {SC_TAG_INT("M25"),        (void *)SC_Matrix25MapTab}, 
        {SC_TAG_INT("C39"),        (void *)SC_Code39MapTab},
        {SC_TAG_INT("CBA"),        (void *)SC_CodabarMapTab},
        {SC_TAG_INT("C93"),        (void *)SC_Code93MapTab},
        {SC_TAG_INT("GS1"),        (void *)SC_GS1MapTab},
        {SC_TAG_INT("RSS"),        (void *)SC_RSSMapTab},
        {SC_TAG_INT("CPT"),        (void *)SC_GS1CompositeMapTab},
        {SC_TAG_INT("C11"),        (void *)SC_Code11MapTab},
        {SC_TAG_INT("ISB"),        (void *)SC_ISBNMapTab},
        {SC_TAG_INT("L25"),        (void *)SC_IND25MapTab},
        {SC_TAG_INT("S25"),        (void *)SC_STD25MapTab},
        {SC_TAG_INT("PLY"),        (void *)SC_PlesseyMapTab},
        {SC_TAG_INT("MSI"),        (void *)SC_MSIMapTab},
        {SC_TAG_INT("ISS"),        (void *)SC_ISSNMapTab},
        {SC_TAG_INT("CHP"),        (void *)SC_ChinaPost25MapTab},
        {SC_TAG_INT("AIM"),        (void *)SC_AIM128MapTab},
        {SC_TAG_INT("IBT"),        (void *)SC_ISBTMapTab},
        {SC_TAG_INT("COP"),        (void *)SC_COOP25MapTab},
        {SC_TAG_INT("D14"),        (void *)SC_Deutsche14MapTab},
        {SC_TAG_INT("D12"),        (void *)SC_Deutsche12MapTab},
        {SC_TAG_INT("TLP"),        (void *)SC_TelepenMapTab},

        {SC_TAG_INT("MAC"),        (void *)SC_MacroBarcodeMapTab},
        {SC_TAG_INT("PDF"),        (void *)SC_PDFCodeMapTab},
        {SC_TAG_INT("QRC"),        (void *)SC_QRCodeMapTab},
        {SC_TAG_INT("AZT"),        (void *)SC_AztecCodeMapTab},
        {SC_TAG_INT("DMC"),        (void *)SC_DMCodeMapTab},
        {SC_TAG_INT("MXC"),        (void *)SC_MaxiCodeMapTab},
        {SC_TAG_INT("CSC"),        (void *)SC_CSCodeMapTab},
        {SC_TAG_INT("GMC"),        (void *)SC_GMCodeMapTab},
        {SC_TAG_INT("MPD"),        (void *)SC_MicroPDFMapTab},
        {SC_TAG_INT("MQR"),        (void *)SC_MicroQRMapTab},
        
        {SC_TAG_INT("SOB"),        (void *)SC_SpecificOCRBMapTab},
        {SC_TAG_INT("PAS"),        (void *)SC_PassportOCRMapTab},
        
        {SC_TAG_INT("PNT"),        (void *)SC_USPSPostnetMapTab},
        {SC_TAG_INT("ILG"),        (void *)SC_USPSIntelliMailMapTab},
        {SC_TAG_INT("ROY"),        (void *)SC_RoyalMailMapTab},
        {SC_TAG_INT("PLA"),        (void *)SC_USPSPlanetMapTab},
        {SC_TAG_INT("KIX"),        (void *)SC_KIXPostMapTab},
        {SC_TAG_INT("APL"),        (void *)SC_AustPostalMapTab},

        //Data Formatting
        {SC_TAG_INT("DFM"),        (void *)SC_DataFormatMapTab},

        //Driver Code
        {SC_TAG_INT("DLP"),        (void *)SC_DriverCodeMapTab},

        //Package Mode
        {SC_TAG_INT("PAC"),        (void *)SC_PackageModeMapTab},

        //Message Extract
        {SC_TAG_INT("EXT"),        (void *)SC_MsgExtractMapTab},

        //Image
        {SC_TAG_INT("IMG"),        (void *)SC_ImageMapTab},

        //Batch
        {SC_TAG_INT("BAT"),        (void *)SC_BatchSetMapTab}, 

		/***********************************************************
		分割线，以下是不公开的命令
		***********************************************************/
		{"2"/*SC_TAG_PRIV*/,             0},
        //DIGIT
        {SC_TAG_INT("DIG"),        (void *)SC_DigitMapTab},     

		//Test Mode
		{SC_TAG_INT("TES"),        (void *)SC_TestModeMapTab},
		
		//Reboot Mode
		{SC_TAG_INT("REB"),        (void *)SC_RebootModeMapTab},
		
		//Voice
		{SC_TAG_INT("VOI"),        (void *)SC_VoiceMapTab},
		
		//设置码
		{SC_TAG_INT("FOB"),        (void *)SC_SetForbidMapTab},
		
		//默认设置
		{SC_TAG_INT("SAV"),        (void *)SC_SaveConfigMapTab},
		{SC_TAG_INT("GEN"),        (void *)SC_GenericCfgMapTab},
		

        //PSN
        {SC_TAG_INT("PSN"),        (void *)SC_PsnEditMapTab}, 

        //ESN
        {SC_TAG_INT("ESN"),        (void *)SC_EsnEditMapTab}, 

       //DATE
        {SC_TAG_INT("DAT"),        (void *)SC_DateEditMapTab}, 

        //Product Name
        {SC_TAG_INT("PDN"),        (void *)SC_ProductNameEditMapTab}, 

        //Manufacture Name
        {SC_TAG_INT("MFN"),        (void *)SC_ManufactureNameEditMapTab},

        //Hardware Version
        {SC_TAG_INT("HWV"),        (void *)SC_HardwareVerEditMapTab},
		
        {SC_TAG_INT("PRN"),        (void *)SC_ProdMsgMapTab},
        
        {SC_TAG_INT("CPD"),        (void *)SC_CPUIDMapTab},
        
        {SC_TAG_INT("SEC"),        (void *)SC_encryptMapTab},
        //DownLoad Set
        {SC_TAG_INT("DWN"),        (void *)SC_DownLoadSetMapTab},

        //Depth of field
         {SC_TAG_INT("DOF"),        (void *)SC_DepthofFieldSetMapTab},
        {SC_TAG_INT("DEL"),        (void *)SC_DepthofFieldDelMapTab},

        //MCU Frequency
        {SC_TAG_INT("HWR"),        (void *)SC_HardwareRunParamMapTab},

        //Running Mode
        {SC_TAG_INT("RUN"),        (void *)SC_RunningModeMapTab},

        //USB枚举使用PID
        {SC_TAG_INT("PID"),        (void *)SC_USBEnumModeMapTab},

        //USB自适应
        {SC_TAG_INT("AUT"),        (void *)SC_USBAutoAdaptMapTab},

        //Buffer Query
        {SC_TAG_INT("BUF"),        (void *)SC_BufferQueryMapTab},
        
        {"1"/*SC_TAG_NULL*/,             0}
			
};

