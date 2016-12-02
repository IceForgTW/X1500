/********************************************************
*
*Author:  <sftan@ingenic.cn>
*
*********************************************************/

#ifndef __JZ4775CODEC_H__
#define __JZ4775CODEC_H__

#define CODEC_SR         0 /* status register */
#define CODEC_AICR_DAC   1  /* DAC audio interface control register*/
#define CODEC_AICR_ADC   2  /*ADC audio interface control register*/
#define CODEC_CR_LO      3  /* differential line-out Control Register*/
#define CODEC_CR_HP      4	/* HeadPhone Control Register*/
#define CODEC_CR_DAC     6	/* DAC Control Register*/
#define CODEC_CR_MIC     7  /* Microphone Control Register*/
#define CODEC_CR_LI      8  /* Control Register for line inputs*/
#define CODEC_CR_ADC     9  /* ADC Control Register */
#define CODEC_CR_MIX     10  /* Control Register for digital mixer*/
#define CODEC_CR_VIC     11  /* Control Register for the codec*/
#define CODEC_CCR        12 /* Clock Control Register */
#define CODEC_FCR_DAC    13 /* DAC Frequency Control Register*/
#define CODEC_FCR_ADC    14 /* ADC Frequency Control Register*/
#define CODEC_ICR        15 /* Interrupt Control Register*/
#define CODEC_IMR        16 /* Interrupt Mask Register*/
#define CODEC_IFR        17 /* Interrupt Flag Register*/
#define CODEC_GCR_HPL    18 /* Left channel headphone Control Gain Register*/
#define CODEC_GCR_HPR    19 /* Right channel headphone Control Gain Register*/
#define CODEC_GCR_LIBYL  20 /* Left channel bypass line Control Gain Register*/
#define CODEC_GCR_LIBYR  21 /* Right channel bypass line Control Gain Register*/
#define CODEC_GCR_DACL   22 /* Left channel DAC Gain Control Register*/
#define CODEC_GCR_DACR   23 /* Right channel DAC Gain Control Register*/
#define CODEC_GCR_MIC1   24 /* Microphone 1 Gain Control Register*/
#define CODEC_GCR_MIC2   25 /* Microphone 2 Gain Control Register*/
#define CODEC_GCR_ADCL   26 /* Left ADC Gain Control Register*/
#define CODEC_GCR_ADCR   27 /* Right ADC Gain Control Register*/
#define CODEC_GCR_MIXADC 29 /* ADC Digital Mixer Control Register*/
#define CODEC_GCR_MIXDAC 30 /* DAC Digital Mixer Control Register*/
#define CODEC_AGC1       31 /* Automatic Gain Control 1*/
#define CODEC_AGC2       32 /* Automatic Gain Control 2*/
#define CODEC_AGC3       33 /* Automatic Gain Control 3*/
#define CODEC_AGC4       34 /* Automatic Gain Control 4*/
#define CODEC_AGC5       35 /* Automatic Gain Control 5*/

/*SR  status register */

#define PON_ACK  		( 1<<7 )
#define IRQ_ACK  		( 1<<6 )   
#define JACK     		( 1<<5 )   

/*AICR_DAC: audio interface control register*/
#define DAC_ADWL(n) 	(n << 6)
#define DAC_SERIAL  	(1 << 1)
#define DAC_I2S     	(1 << 0)

/*AICR_ADC :audio interface control register*/
#define ADC_ADWL(n) 	(n << 6)
#define ADC_SERIAL  	(1 << 1)
#define ADC_I2S     	(1 << 0)

/*CR_LO : differential line-out Control Register*/
#define LO_MUTE			(1 << 7)
#define SB_LO			(1 << 4)
#define LO_SEL(n)		(n << 0)

/*CR_HP :    HeadPhone Control Register*/
#define HP_MUTE     	(1 << 7)
#define LOAD			(1 << 6)
#define SB_HP			(1 << 4)
#define SB_HPCM     	(1 << 3)
#define HP_SEL(n)		(n << 0)

/*CR_DAC :   DAC Control Register for DAC 3*/
#define DAC_MUTE       (1 << 7)
#define DAC_MONO	   (1 << 6)
#define DAC_LEFT_ONLY  (1 << 5)
#define SB_DAC		   (1 << 4)
#define DAC_LRSWAP	   (1 << 3)

/*CR_MIC:    Microphone Control Register*/
#define MIC_STEREO		(1 << 7)
#define MICIDFF     	(1 << 6)
#define SB_MIC2			(1 << 5)
#define SB_MIC1			(1 << 4)
#define MICBIAS_V0  	(1 << 1)
#define SB_MICBIAS  	(1 << 0)

/*CR_LI:     Control Register for line inputs*/
#define SB_LIBY    		(1 << 4)
#define SB_LIN     		(1 << 0)

/*CR_ADC:    ADC Control Register*/
#define DMIC_SEL   		(1 << 7)
#define ADC_MONO   		(1 << 6)
#define ADC_LEFT_ONLY 	(1 << 5)
#define SB_ADC			(1 << 4)
#define ADC_LRSWAP		(1 << 3)
#define IN_SEL(n)		(n << 0)

/*CR_MIX:   Control Register for digital mixer*/
#define MIX_REC(n)		(n << 2)
#define DAC_MIX(n)		(n << 0)

/*CR_VIC:    Control Register for the codec*/
#define SB_SLEEP 		(1 << 1)
#define SB				(1 << 0)

/*CCR :      Clock Control Register*/
#define DMIC_CLKON		(1 << 7)
#define CRYSTAL(n)		(n << 0)

/*FCR_DAC:   DAC Frequency Control Register*/
#define DAC_FREQ(n)		(n << 0)

/*FCR_ADC :  ADC Frequency Control Register*/
#define ADC_HPF			(1 << 6)
#define ADC_FREQ(n)		(n << 0)

/*ICR :      Interrupt Control Register*/
#define INT_FORM(n)		(n << 6)

/*IMR :      Interrupt Mask Register*/
#define SCLR_MASK		(1 << 6)
#define JACK_MASK 		(1 << 5)
#define SCMC_MASK 		(1 << 4)
#define RUP_MASK		(1 << 3)
#define RDO_MASK 		(1 << 2)
#define GUP_MASK		(1 << 1)
#define GDO_MASK 		(1 << 0)

/*IFR :      Interrupt Flag Register*/
#define SCLR 			(1 << 6)
#define JACK_EVENT 		(1 << 5)
#define SCMC 			(1 << 4)
#define RUP				(1 << 3)
#define RDO				(1 << 2)
#define GUP				(1 << 1)
#define GDO				(1 << 0)

/*GCR_HPL:  Left channel headphone Control Gain Register*/
#define LRGO			(1 << 7)
#define GOL(n)			(n << 0)

/*GCR_HPR:   Right channel headphone Control Gain Register*/
#define GOR(n)			(n << 0)

/*GCR_LIBYL: Left channel bypass line Control Gain Register*/
#define LRGI 			(1 << 7)
#define GIL(n)			(n << 0)

/*GCR_LIBYR:   Right channel bypass line Control Gain Register*/
#define GIR(n)			(n << 0)

/*GCR_DACL : Left channel DAC Gain Control Register*/
#define RLGOD 			(1 << 7)
#define GODL(n)			(n << 0)

/*GCR_DACR:  Right channel DAC Gain Control Register*/
#define GODR(n)			(n << 0)

/*GCR_MIC1:  Microphone 1 Gain Control Register*/
#define GIM1(n)			(n << 0)

/*GCR_MIC2:  Microphone 2 Gain Control Register*/
#define GIM2(n)			(n << 0)

/*GCR_ADCL:  Left ADC Gain Control Register*/
#define LRGID 			(1 << 7)
#define GIDL(n)			(n << 0)

/*GCR_ADCR:  Right ADC Gain Control Register*/
#define GIDR(n)			(n << 0)

/*GCR_MIXADC: ADC Digital Mixer Control Register*/
#define GIMIX(n)		(n << 0)

/*GCR_MIXDAC: DAC Digital Mixer Control Register*/
#define GOMIX(n)		(n << 0)

/*AGC1 :      Automatic Gain Control 1*/
#define AGC_EN 			(1 << 7)
#define AGC_STEREO		(1 << 6)
#define TARGET(n)		(n << 2)

/*AGC2 :      Automatic Gain Control  2*/
#define NG_EN 			(1 << 7)
#define NG_THR(n)		(n << 4)
#define HOLD(n)			(n << 0)

/*AGC3  :    Automatic Gain Control  3*/
#define ATK(n)			(n << 4)
#define DCY(n)			(n << 0)

/*AGC4 :      Automatic Gain Control  4*/
#define AGC_MAX(n)		(n << 0)

/*AGC5  :     Automatic Gain Control  5*/
#define AGC_MIN(n)		(n << 0)

/* Audio Sample Format */
#define	AFMT_U8			8
#define AFMT_S16_LE		16

/* PCM ioctl command */
#define PCM_SET_SAMPLE_RATE	  0
#define PCM_SET_CHANNEL		    1
#define PCM_SET_FORMAT		    2
#define PCM_SET_VOL		        3
#define PCM_GET_VOL		        4
#define PCM_GET_SPACE         5
#define PCM_SET_HP_VOL		    6
#define PCM_GET_HP_VOL		    7
#define PCM_SET_PAUSE		      8
#define PCM_SET_PLAY		      9
#define PCM_RESET		          10
#define PCM_SET_REPLAY		    11
#define PCM_SET_RECORD		    12
#define PCM_SET_MUTE		      13
#define PCM_SET_EQPLAY        14
#define PCM_SET_VOLFUNCTION   15
#define PCM_GET_SAMPLE_MAX    16
#define PCM_SET_RECORD_FM	    17 
#define PCM_REINIT            18
#define PCM_DATA_FINISH       19
#define PCM_POWER_ON          20
#define PCM_POWER_OFF         21

#define PCM_BYPASS_LH         22 /* bypass line to headphone*/
#define PCM_BYPASS_LL         23 /* bypass line to line out */


#define PCM_SIDETONE_MONO_MIC1_HP       26 /* stereo mic to 16Ohm headphone out mic1 to right channel */
#define PCM_SIDETONE_MONO_MIC2_HP       27 /* stereo mic to BTL out mic2 to right channel */

#define PCM_SIDETONE_STER_MIC1_HP       28 /* stereo mic to 16Ohm headphone out mic1 to right channel */
#define PCM_SIDETONE_STER_MIC2_HP       29 /* stereo mic to BTL out mic2 to right channel */

#define PCM_SIDETONE_MIC1_LO			30
#define PCM_SIDETONE_MIC2_LO			31
#define PCM_SIDETONE_MIC1_MIC2_LO		32 

/* replay set macro*/
#define DAC_TO_HP     1
#define DAC_TO_LOUT   2


/* record set macro*/
#define MONO_MIC1_IN  1
#define MONO_MIC2_IN  2 
#define STEREO_MIC1_IN 3
#define STEREO_MIC2_IN 4 
#define LINE_IN       5


#endif 
