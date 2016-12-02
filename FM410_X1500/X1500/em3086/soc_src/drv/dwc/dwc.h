/*
 * dwc.h
 *
 *  Created on: Mar 14, 2013
 *      Author: jyin
 */

#ifndef DWC_H_
#define DWC_H_

#include <os_api.h>

#define DWC_FORCE_SPEED_FULL 1

#if DWC_FORCE_SPEED_FULL
#define ENDPOINT_PACKET_SIZE        512
#define CONTROL_MAX_PACKET_SIZE     64
#else
#define ENDPOINT_PACKET_SIZE        64
#define CONTROL_MAX_PACKET_SIZE     64
#endif

#define EP_IDLE			0
#define EP_TRANSFERED	1
#define EP_TRANSFERING	2

#define EP_SETUP	   	0
#define EP_DATA 		1
#define EP_STATUS  		2
#define EP_SETUP_PHASEDONE 3

#define DWC_OTG_EP_TYPE_CONTROL	0
#define DWC_OTG_EP_TYPE_ISOC	1
#define DWC_OTG_EP_TYPE_BULK	2
#define DWC_OTG_EP_TYPE_INTR	3

typedef struct dwc_send_buffer {                           
    unsigned char *buf;                   
    unsigned int nSize;                   
    struct dwc_send_buffer    *pNext;               
} DWC_Send_Buf;

typedef struct dwc_ep_t {

	unsigned char num; 	/* ep number used for register address lookup */

	unsigned ep_state:2;
	unsigned is_in:1; 	/* ep dir 1 = out */

	unsigned active:1; /* ep active */
	unsigned type:2;	/* ep type */

	unsigned maxpacket:11; /* max packet bytes */

	unsigned int ctrl_req_addr; /* ctrl_req_addr */

	void * xfer_buff;	/* pointer to transfer buffer */

	unsigned xfer_len:19;	/* number of bytes to transfer */

	unsigned xfer_count:19;	/* number of bytes transfered */

	int      EpNumTotalBytes;
	int      EpBytesProcessed;

	void *CallBackParameter;
	void *CallBackBuffer;

} dwc_ep;

typedef union hwcfg1_data {
	unsigned int d32;

	struct {
		unsigned ep_dir0:2;
		unsigned ep_dir1:2;
		unsigned ep_dir2:2;
		unsigned ep_dir3:2;
		unsigned ep_dir4:2;
		unsigned ep_dir5:2;
		unsigned ep_dir6:2;
		unsigned ep_dir7:2;
		unsigned ep_dir8:2;
		unsigned ep_dir9:2;
		unsigned ep_dir10:2;
		unsigned ep_dir11:2;
		unsigned ep_dir12:2;
		unsigned ep_dir13:2;
		unsigned ep_dir14:2;
		unsigned ep_dir15:2;
	} b;
#define DWC_HWCFG1_DIR_BIDIR	0x0
#define DWC_HWCFG1_DIR_IN	0x1
#define DWC_HWCFG1_DIR_OUT	0x2
} hwcfg1_data_t;

/**
 * This union represents the bit fields in the User HW Config2
 * Register.  Read the register into the <i>d32</i> element then read
 * out the bits using the <i>b</i>it elements.
 */
typedef union hwcfg2_data {
	/** raw register data */
	unsigned int d32;
	/** register bits */
	struct {
		/* GHWCFG2 */
		unsigned op_mode:3;
#define DWC_HWCFG2_OP_MODE_HNP_SRP_CAPABLE_OTG 0
#define DWC_HWCFG2_OP_MODE_SRP_ONLY_CAPABLE_OTG 1
#define DWC_HWCFG2_OP_MODE_NO_HNP_SRP_CAPABLE_OTG 2
#define DWC_HWCFG2_OP_MODE_SRP_CAPABLE_DEVICE 3
#define DWC_HWCFG2_OP_MODE_NO_SRP_CAPABLE_DEVICE 4
#define DWC_HWCFG2_OP_MODE_SRP_CAPABLE_HOST 5
#define DWC_HWCFG2_OP_MODE_NO_SRP_CAPABLE_HOST 6

		unsigned architecture:2;
		unsigned point2point:1;
		unsigned hs_phy_type:2;
#define DWC_HWCFG2_HS_PHY_TYPE_NOT_SUPPORTED 0
#define DWC_HWCFG2_HS_PHY_TYPE_UTMI 1
#define DWC_HWCFG2_HS_PHY_TYPE_ULPI 2
#define DWC_HWCFG2_HS_PHY_TYPE_UTMI_ULPI 3

		unsigned fs_phy_type:2;
		unsigned num_dev_ep:4;
		unsigned num_host_chan:4;
		unsigned perio_ep_supported:1;
		unsigned dynamic_fifo:1;
		unsigned multi_proc_int:1;
		unsigned reserved21:1;
		unsigned nonperio_tx_q_depth:2;
		unsigned host_perio_tx_q_depth:2;
		unsigned dev_token_q_depth:5;
		unsigned otg_enable_ic_usb:1;
	} b;
} hwcfg2_data_t;


/**
 * This union represents the bit fields in the User HW Config3
 * Register.  Read the register into the <i>d32</i> element then read
 * out the bits using the <i>b</i>it elements.
 */
typedef union hwcfg3_data {
	/** raw register data */
	unsigned int d32;
	/** register bits */
	struct {
		/* GHWCFG3 */
		unsigned xfer_size_cntr_width:4;
		unsigned packet_size_cntr_width:3;
		unsigned otg_func:1;
		unsigned i2c:1;
		unsigned vendor_ctrl_if:1;
		unsigned optional_features:1;
		unsigned synch_reset_type:1;
		unsigned adp_supp:1;
		unsigned otg_enable_hsic:1;
		unsigned bc_support:1;
		unsigned otg_lpm_en:1;
		unsigned dfifo_depth:16;
	} b;
} hwcfg3_data_t;


/**
 * This union represents the bit fields in the User HW Config4
 * Register.  Read the register into the <i>d32</i> element then read
 * out the bits using the <i>b</i>it elements.
 */
#define USB_CABLE_CONNECT	1
#define USB_CABLE_SUSPEND	2
#define USB_CABLE_DISCONNECT 0
 
typedef union hwcfg4_data {
	/** raw register data */
	unsigned int d32;
	/** register bits */
	struct {
		unsigned num_dev_perio_in_ep:4;
		unsigned power_optimiz:1;
		unsigned min_ahb_freq:1;
		unsigned part_power_down:1;
		unsigned reserved:7;
		unsigned utmi_phy_data_width:2;
		unsigned num_dev_mode_ctrl_ep:4;
		unsigned iddig_filt_en:1;
		unsigned vbus_valid_filt_en:1;
		unsigned a_valid_filt_en:1;
		unsigned b_valid_filt_en:1;
		unsigned session_end_filt_en:1;
		unsigned ded_fifo_en:1;
		unsigned num_in_eps:4;
		unsigned desc_dma:1;
		unsigned desc_dma_dyn:1;
	} b;
} hwcfg4_data_t;

typedef union dwc_state {
	unsigned char d8;

	struct {
		unsigned event:1;
		unsigned state:7;
	}b;

} dwc_st;


#define USB_SPEED_HIGH	0
#define USB_SPEED_FULL	1
#define USB_SPEED_LOW	2

typedef struct dwc_cfg_if_t {
	hwcfg1_data_t hwcfg1;
	hwcfg2_data_t hwcfg2;
	hwcfg3_data_t hwcfg3;
	hwcfg4_data_t hwcfg4;
	unsigned int ctrl_request[5];

	dwc_st status;
	unsigned char speed;
	unsigned char is_dma;

	dwc_ep *dep[32];

	OS_EVENT *dwc_sem;

	DWC_Send_Buf *tx_list[16];
} dwc_cfg_if;

void udc_Send(unsigned int epNum,  const unsigned char *buf, int nSize);
void HW_SendPKT(unsigned int epnum,  const unsigned char *buf, int size);
void HW_GetPKT(unsigned int epnum,  const unsigned char *buf, int size);
void *GetEp(int x);
void SetEp(PIPE *pep, int num);
int dwc_set_address(unsigned arg);
int dwc_configure_epn(unsigned int type);


#endif /* DWC_H_ */
