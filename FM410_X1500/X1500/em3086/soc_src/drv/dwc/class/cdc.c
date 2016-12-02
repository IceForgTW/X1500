#include <os_api.h>
#include <udcbus.h>
#include <usb.h>
#include "dwc.h"
#include <sysdefs.h>

#include "generate.h"
#include "watchdog.h"
#include "compat.h"

#include <string.h>
#include <stdio.h>

#if 0
#define dprintf printf
#else
#define dprintf(x...)
#endif
#ifndef NULL
#define NULL
#endif

#define CDC_IN_EP	1
#define CDC_OUT_EP	2
#define CDC_CTL_EP	3
#define UNUSED(x)   (void)x;

#define USB_MAX_CDC_SUPPORTED_INTERFACES     (2)

#define DATA_BUFFMAX  (1024*32)

#if DWC_FORCE_SPEED_FULL
PIPE cdc_pipe[] = {
	              {0,ENDPOINT_TYPE_CONTROL,64},
				  {(0x80|CDC_IN_EP),ENDPOINT_TYPE_BULK,64},
				  {CDC_OUT_EP,ENDPOINT_TYPE_BULK,64},
                  {(0x80|CDC_CTL_EP),ENDPOINT_TYPE_INTERRUPT,64}
               };
#else

PIPE cdc_pipe[] = {
	              {0,ENDPOINT_TYPE_CONTROL,64},
				  {(0x80|CDC_IN_EP),ENDPOINT_TYPE_BULK,512},
				  {CDC_OUT_EP,ENDPOINT_TYPE_BULK,512},
                  {(0x80|CDC_CTL_EP),ENDPOINT_TYPE_INTERRUPT,512}
               };

#endif

static unsigned char cdc_RcvBuf[DATA_BUFFMAX];
static int g_cdc_ReadPoint = 0;
static int g_cdc_WritePoint = 0;

static volatile BOOL g_cdc_active = FALSE;

static BOOL g_cdc_isOpen = FALSE;
OS_EVENT *cdc_read_mbox =  NULL;



static USB_DeviceDescriptor cdc_devDesc = 
{
	sizeof(USB_DeviceDescriptor),
	DEVICE_DESCRIPTOR,	//1
    #if DWC_FORCE_SPEED_FULL
	0x0110,     //Version 1.10
	#else
    0x0200,     //Version 2.0
    #endif
	0xFF,
	0x00,
	0x00,
	64,	/* Ep0 FIFO size */
	USB_FUNCTION_VENDOR_ID,
	USB_FUNCTION_CDC_PRODUCT_ID,
	DRIVER_VERSION_NUM_CDC,
	STRING_MANUFACTURER, 	//iManufacturer;
	STRING_PRODUCT,   //iProduct;
	STRING_SN,
	0x01
};

#define	CONFIG_DESCRIPTOR_LEN	(sizeof(USB_ConfigDescriptor) + \
				 sizeof(USB_InterfaceDescriptor) + \
				 sizeof(head_functional_t) + \
				 sizeof(call_management_functional_t) + \
				 sizeof(abstract_control_management_functional_t) + \
				 sizeof(union_functional_t) + \
				 sizeof(USB_EndPointDescriptor) + \
				 sizeof(USB_InterfaceDescriptor) + \
				 sizeof(USB_EndPointDescriptor) * 2)

static struct {
	USB_ConfigDescriptor    configuration_descriptor;
	USB_InterfaceDescriptor interface_descritor;
    head_functional_t hf;
    call_management_functional_t cmf;
    abstract_control_management_functional_t acmf;
    union_functional_t uf;
    USB_EndPointDescriptor     ep_ctl;
    USB_InterfaceDescriptor   data_intf;
	USB_EndPointDescriptor  ep_in;
    USB_EndPointDescriptor  ep_out;
} __attribute__ ((packed)) cdc_confDesc = {
	{
		sizeof(USB_ConfigDescriptor),
		CONFIGURATION_DESCRIPTOR,
		CONFIG_DESCRIPTOR_LEN,  /*  Total length of the Configuration descriptor */ 
		0x02,                   /*  NumInterfaces */
		0x01,                   /*  Configuration Value */
		0x00,                   /* Configuration Description String Index */  
		0x80,	// Self Powered, no remote wakeup
		0xfa	// Maximum power consumption 500 mA
	},

	/* Interface descriptor */
	{
		sizeof(USB_InterfaceDescriptor),
		INTERFACE_DESCRIPTOR,
		0x00,   /* bInterfaceNumber */
		0x00,   /* bAlternateSetting */
		0x01,	/* ep number */
		0x02,   /* Communication Interface Class */
		0x02,   /* Abstract Control Model */
		0x01,   /* Interface Protocol*/
		STRING_CDC_ACM_COMM    /* Interface Description String Index*/
	},

	/*Header Functional Descriptor*/
	{
        sizeof(head_functional_t),             /* size of Functional Desc in bytes */
        USB_CS_INTERFACE,  /* descriptor type*/
        HEADER_FUNC_DESC,
        0x0110,  /* USB Class Definitions for CDC spec release number in BCD */
    },

	/*Call Managment Functional Descriptor*/
    {
        sizeof(call_management_functional_t),             /* Size of this descriptor */
        USB_CS_INTERFACE, /* descriptor type*/
        CALL_MANAGEMENT_FUNC_DESC,
        0x01,/*may use 0x03 */  /* device handales call management itself(D0 set)
                  and will process commands multiplexed over the data interface */
        0x01,      /* Indicates multiplexed commands are
                    handled via data interface */
    },

	/*ACM Functional Descriptor*/
    {
        sizeof(abstract_control_management_functional_t),             /* Size of this descriptor */
        USB_CS_INTERFACE, /* descriptor type*/
        ABSTRACT_CONTROL_FUNC_DESC,
        0x06, /*may use 0x0F */ /* Device Supports all commands for ACM - CDC
                                  PSTN SubClass bmCapabilities */
    },

	/*Union Functional Descriptor*/
    {
        sizeof(union_functional_t),             /* size of Functional Desc in bytes */
        USB_CS_INTERFACE,  /* descriptor type*/
        UNION_FUNC_DESC,
        0x00,           /* Interface Number of Control */
        0x01            /* Interface Number of Subordinate (Data Class) Interface */
    },
        
	/*Endpoint 3 Descriptor*/
	{
		sizeof(USB_EndPointDescriptor),
		ENDPOINT_DESCRIPTOR,
		(1 << 7) | CDC_CTL_EP,// endpoint 3 is IN endpoint
		ENDPOINT_TYPE_INTERRUPT, /* control */
		16, /* IN EP FIFO size */
		0
	},

    /*Data class interface descriptor*/
	{
		sizeof(USB_InterfaceDescriptor),
		INTERFACE_DESCRIPTOR,
		0x01,   /* bInterfaceNumber */
		0x00,   /* bAlternateSetting */
		0x02,	/* ep number */
		0x0a,   /* Interface Class CDC */
		0,      /* Abstract Control Model*/
		0,      /* Interface Protocol*/
		STRING_CDC_ACM_DATA    /* Interface Description String Index*/
	},
	
    /*Endpoint 1 Descriptor*/
	{
		sizeof(USB_EndPointDescriptor),
		ENDPOINT_DESCRIPTOR,
		(1 << 7) | CDC_IN_EP,// endpoint 1 is IN endpoint
		ENDPOINT_TYPE_BULK, /* bulk */
		64, /* IN EP FIFO size */
		0
	},

    /*Endpoint 2 Descriptor*/
	{
		sizeof(USB_EndPointDescriptor),
		ENDPOINT_DESCRIPTOR,
		(0 << 7) | CDC_OUT_EP,// endpoint 2 is OUT endpoint
		ENDPOINT_TYPE_BULK, /* bulk */
		64, /* OUT EP FIFO size */
		0
	}
};


static USB_DeviceQualifierDescriptor cdc_devQualifyDesc =
{
	sizeof(USB_DeviceQualifierDescriptor),
	0x06,       /* Type of Descriptor */
	0x0200,     /*  BCD USB version  */ 
	0xff,       /* bDeviceClass */
	0x00,       /* bDeviceSubClass */
	0xff,       /* bDeviceProtocol */
	0x40,       /* bMaxPacketSize0 */
	0x01,       /* bNumConfigurations */
	0x00        /* Reserved : must be zero */ 
};


static u8 g_line_coding[USB_MAX_CDC_SUPPORTED_INTERFACES][LINE_CODING_SIZE] =
{
        { (LINE_CODE_DTERATE_IFACE0>> 0) & 0x000000FF,
          (LINE_CODE_DTERATE_IFACE0>> 8) & 0x000000FF,
          (LINE_CODE_DTERATE_IFACE0>>16) & 0x000000FF,
          (LINE_CODE_DTERATE_IFACE0>>24) & 0x000000FF,
          /*e.g. 0x00,0xC2,0x01,0x00 : 0x0001C200 is 115200 bits per second */
           LINE_CODE_CHARFORMAT_IFACE0,
           LINE_CODE_PARITYTYPE_IFACE0,
           LINE_CODE_DATABITS_IFACE0
        },
        { (LINE_CODE_DTERATE_IFACE1>> 0) & 0x000000FF,
          (LINE_CODE_DTERATE_IFACE1>> 8) & 0x000000FF,
          (LINE_CODE_DTERATE_IFACE1>>16) & 0x000000FF,
          (LINE_CODE_DTERATE_IFACE1>>24) & 0x000000FF,
          /*e.g. 0x00,0xC2,0x01,0x00 : 0x0001C200 is 9600 bits per second */
           LINE_CODE_CHARFORMAT_IFACE1,
           LINE_CODE_PARITYTYPE_IFACE1,
           LINE_CODE_DATABITS_IFACE1
        },
};

static u8 g_abstract_state[USB_MAX_CDC_SUPPORTED_INTERFACES][COMM_FEATURE_DATA_SIZE] =
{
        { (STATUS_ABSTRACT_STATE_IFACE0>>0) & 0x00FF,
          (STATUS_ABSTRACT_STATE_IFACE0>>8) & 0x00FF
        }
};

static u8 g_country_code[USB_MAX_CDC_SUPPORTED_INTERFACES][COMM_FEATURE_DATA_SIZE] =
{
        { (COUNTRY_SETTING_IFACE0>>0) & 0x00FF,
          (COUNTRY_SETTING_IFACE0>>8) & 0x00FF
        }
};

static u8 g_serial_state_buf[NOTIF_PACKET_SIZE+UART_BITMAP_SIZE] =
{
    NOTIF_REQUEST_TYPE,SERIAL_STATE_NOTIF,
    0x00,0x00,/*wValue*/
    0x00,0x00,/*interface - modifies*/
    UART_BITMAP_SIZE,0x00,/* wlength*/
    0x00,0x00 /*data initialized - modifies*/
};/*uart bitmap*/


static u32 cdc_get_state = 0;

static bool g_dte_present = FALSE;
static u8 g_dte_status = 0; /* Status of DATA TERMINAL EQUIPMENT */
static u16 g_break_duration = 0; /* Length of time in milliseconds of the
                                                              break signal */
static u8 g_current_interface = 0;
static UART_BITMAP g_uart_bitmap;
static volatile bool start_transactions = FALSE;


static unsigned int cdc_detect(unsigned int handle,unsigned int stat)
{

	return 1;	
}

static unsigned int cdc_reset(unsigned int handle)
{

	g_cdc_active = FALSE;
	return 1;
	
}


static unsigned int cdc_suspend(unsigned int handle)
{
	return 1;
}


static unsigned int cdc_set_speed(unsigned stat)
{
	switch(stat)
	{
	case UDC_HIGHSPEED:
		printf("UDC HIGHSPEED-----------\r\n");
		break;
		
	case UDC_FULLSPEED:
		printf("UDC FULLSPEED-----------\r\n");
		break;
		
	}
	return 1;
}


static  void cdc_get_dev_descriptor(unsigned int handle,USB_DeviceRequest *dreq)
{
    PUDC_BUS pBus = (PUDC_BUS)handle;
    unsigned short size = dreq->wLength;
	
	if(size < sizeof(cdc_devDesc))
	{
		cdc_devDesc.bLength = size;
		pBus->StartTransfer(handle,cdc_pipe[0].ep,(unsigned char *)&cdc_devDesc, size);
	}
	
	else
	{
		cdc_devDesc.bLength = sizeof(cdc_devDesc);
		pBus->StartTransfer(handle,cdc_pipe[0].ep,(unsigned char *)&cdc_devDesc, sizeof(cdc_devDesc));
	}
}

static  void cdc_get_dev_configuration(unsigned int handle,USB_DeviceRequest *dreq)
{
	PUDC_BUS pBus = (PUDC_BUS)handle;
	switch (dreq->wLength) {
	case 9:
		pBus->StartTransfer(handle,cdc_pipe[0].ep, (unsigned char *)&cdc_confDesc, 9);
		break;
	case 8:
		pBus->StartTransfer(handle,cdc_pipe[0].ep, (unsigned char *)&cdc_confDesc, 8);
		break;
	default:
		pBus->StartTransfer(handle,cdc_pipe[0].ep, (unsigned char *)&cdc_confDesc, sizeof(cdc_confDesc));
		break;
	}
}

static  void cdc_get_dev_qualify_descitptor(unsigned int handle,USB_DeviceRequest *dreq)
{
    PUDC_BUS pBus = (PUDC_BUS)handle;
    pBus->StartTransfer(handle,cdc_pipe[0].ep, (unsigned char *)&cdc_devQualifyDesc, sizeof(cdc_devQualifyDesc));	
}


static  void cdc_get_dev_other_speed(unsigned int handle,USB_DeviceRequest *dreq)
{
}


static inline void cdc_get_dev_descriptor_string(unsigned int handle,USB_DeviceRequest *dreq)
{
	int len = dreq->wLength;
	PUDC_BUS pBus = (PUDC_BUS)handle;
	unsigned char buf[256];

//	printf("send_dev_desc_string size = %d type %d \r\n",size,dreq->wValue & 0xff);

	len = usb_gadget_get_string(dreq->wValue & 0xff, buf);	
	if (len<=0)
	{
		printf("usb_gadget_get_string error: %d\n", len);
		return;
	}

	if (len>dreq->wLength)
		len = dreq->wLength;
	
	pBus->StartTransfer(handle,cdc_pipe[0].ep, buf, len);

}

/**************************************************************************//*!
 *
 * @name  cdc_pstn_send_serial_state
 *
 * @brief  This function is called to send serial state notification
 *
 *
 * @return NONE
 ******************************************************************************
 * Returns the Serial State
 *****************************************************************************/
void cdc_pstn_send_serial_state (int handle)
{
    PUDC_BUS pBus = (PUDC_BUS)handle;

    /* update array for current interface */
    g_serial_state_buf[4] = g_current_interface;

    /* Lower byte of UART BITMAP */
    g_serial_state_buf[NOTIF_PACKET_SIZE+UART_BITMAP_SIZE-2] = g_uart_bitmap._byte;

    // send ack
    pBus->StartTransfer(handle, cdc_pipe[0].ep, NULL,  0);

    // send line state data
    //pBus->StartTransfer(handle, cdc_pipe[3].ep, g_serial_state_buf, sizeof(g_serial_state_buf));
}


/**************************************************************************//*!
 *
 * @name  cdc_pstn_get_line_coding
 *
 * @brief  This function is called in response to GetLineCoding request
 *
 * @param setup_packet    : Pointer to setup packet
 *
 * @return status:
 *          0 : When Successfull
 *          1: When  request for invalid Interface is presented
 ******************************************************************************
 * Calls application to receive Line Coding Information
 *****************************************************************************/
u8 cdc_pstn_get_line_coding (
    unsigned int handle,
    USB_DeviceRequest * setup_packet    /* [IN] Pointer to setup packet */
)
{
    PUDC_BUS pBus = (PUDC_BUS)handle;
    
    g_current_interface = (u8)setup_packet->wIndex ;

    if(g_current_interface < USB_MAX_CDC_SUPPORTED_INTERFACES)
    {
        pBus->StartTransfer(handle,
                            cdc_pipe[0].ep, 
                            (u8 *)g_line_coding[g_current_interface], 
                            sizeof(g_line_coding)/USB_MAX_CDC_SUPPORTED_INTERFACES);

        return 0;
    }

    return 1;
}

/**************************************************************************//*!
 *
 * @name  cdc_pstn_set_line_coding
 *
 * @brief  This function is called in response to SetLineCoding request
 *
 * @param setup_packet    : Pointer to setup packet
 *
 * @return status:
 *      0                  : When Successfull
 *      1 : When  request for invalid
 *                                Interface is presented
 ******************************************************************************
 * Calls Applciation to update Line Coding Information
 *****************************************************************************/
u8 cdc_pstn_set_line_coding (
    unsigned int handle,
    USB_DeviceRequest * setup_packet    /* [OUT] Pointer to setup packet */
)
{
    int count;
    PUDC_BUS pBus = (PUDC_BUS)handle;
    u8 *data = (u8 *)setup_packet + 8;
    
    g_current_interface = (u8)setup_packet->wIndex ;

    if(g_current_interface < USB_MAX_CDC_SUPPORTED_INTERFACES)
    {
        /* set line coding data*/
        for (count = 0; count < LINE_CODING_SIZE; count++)
        {
            g_line_coding[g_current_interface][count] = *(data+ count);
        }

        // ×´Ì¬½×¶Î£ºIN                DATA1(Áã×Ö½Ú) 
        pBus->StartTransfer(handle, cdc_pipe[0].ep, NULL,  0);
        return 0;
    }

    return 1;
}

/**************************************************************************//*!
 *
 * @name  cdc_pstn_set_ctrl_line_state
 *
 * @brief  This function is called in response to Set Control Line State
 *
 * @param controller_ID   : Controller ID
 * @param setup_packet    : Pointer to setup packet
 * @param data            : Pointer to Data
 * @param size            : Pointer to Size of Data
 *
 * @return status:
 *                        USB_OK : Always
 ******************************************************************************
 * Updates Control Line State
 *****************************************************************************/
u8 cdc_pstn_set_ctrl_line_state (
    unsigned int handle,
    USB_DeviceRequest * setup_packet    /* [IN] Pointer to setup packet */
)
{
    g_dte_status = (u8)setup_packet->wValue ;
    g_uart_bitmap._byte = 0x00; /* initialize */
    
    /* activate/deactivate Tx carrier */
    g_uart_bitmap.Bitmap_Uart.bTxCarrier = (g_dte_status & CARRIER_ACTIVATION_CHECK) ? 1 : 0 ;
    
     /* activate carrier and DTE */
    g_uart_bitmap.Bitmap_Uart.bRxCarrier = (g_dte_status & CARRIER_ACTIVATION_CHECK) ? 1 : 0 ;

    /* Indicates to DCE if DTE is present or not */
    g_dte_present = (bool)((g_dte_status & DTE_PRESENCE_CHECK) ? TRUE : FALSE);

    /* Send Notification to Host - Parameter on Device side has changed */
    cdc_pstn_send_serial_state(handle);

    if(g_dte_status & CARRIER_ACTIVATION_CHECK)
    {
        start_transactions = TRUE;
    }
    else
    {
        start_transactions = FALSE;
    }

    return 0;
}

/**************************************************************************//*!
 *
 * @name  cdc_pstn_send_break
 *
 * @brief  This function is called in response to Set Config request
 *
 * @param controller_ID   : Controller ID
 * @param setup_packet    : Pointer to setup packet
 * @param data            : Pointer to Data
 * @param size            : Pointer to Size of Data
 *
 * @return status:
 *                        USB_OK : Always
 ******************************************************************************
 * Updates Break Duration Information from Host
 *****************************************************************************/
u8 cdc_pstn_send_break (
    unsigned int handle,
    USB_DeviceRequest * setup_packet    /* [IN] Pointer to setup packet */
)
{
    g_break_duration = setup_packet->wValue;
    UNUSED(g_break_duration);

    return 0;
}


/**************************************************************************//*!
 *
 * @name  cdc_pstn_get_comm_feature
 *
 * @brief  This function is called in response to GetCommFeature request
 *
 * @param controller_ID   : Controller ID
 * @param setup_packet    : Pointer to setup packet
 * @param data            : Pointer to Data to be send
 * @param size            : Pointer to Size of Data
 *
 * @return status:
 *      USB_OK                  : When Successfull
 *      USBERR_INVALID_REQ_TYPE : When  request for invalid
 *                                Interface is presented
 ******************************************************************************
 * Returns the status of the get comm feature request
 *****************************************************************************/
u8 cdc_pstn_get_comm_feature (
    unsigned int handle,
    USB_DeviceRequest *setup_packet    /* [IN] Pointer to setup packet */
)
{   
    PUDC_BUS pBus = (PUDC_BUS)handle;
    
    g_current_interface = (u8)setup_packet->wIndex ;
    
    if(setup_packet->wValue == ABSTRACT_STATE_FEATURE)
    {
        if(g_current_interface < USB_MAX_CDC_SUPPORTED_INTERFACES)
        {
            pBus->StartTransfer(handle,
                                cdc_pipe[0].ep, 
                                (u8 *)g_abstract_state[g_current_interface], 
                                sizeof(g_abstract_state)/USB_MAX_CDC_SUPPORTED_INTERFACES);
        }

    }
    else if(setup_packet->wValue == COUNTRY_SETTING_FEATURE)
    {
        if(g_current_interface < USB_MAX_CDC_SUPPORTED_INTERFACES)
        {
            pBus->StartTransfer(handle, 
                                cdc_pipe[0].ep, 
                                (u8 *)g_country_code[g_current_interface], 
                                sizeof(g_country_code)/USB_MAX_CDC_SUPPORTED_INTERFACES);            
        }
    }
    else
    {
    }
    return 0;
}

/**************************************************************************//*!
 *
 * @name  cdc_pstn_set_comm_feature
 *
 * @brief  This function is called in response to SetCommFeature request
 *
 * @param setup_packet    : Pointer to setup packet
 *
 * @return status:
 *      USB_OK                  : When Successfull
 *      USBERR_INVALID_REQ_TYPE : When  request for invalid
 *                                Interface is presented
 ******************************************************************************
 * Sets the comm feature specified by Host
 *****************************************************************************/
u8 cdc_pstn_set_comm_feature (
    unsigned int handle,
    USB_DeviceRequest *setup_packet    /* [IN] Pointer to setup packet */
)
{
    int count;
    u8 *data = (U8 *)setup_packet +8;
    
    g_current_interface = (u8)setup_packet->wIndex ;
    if(setup_packet->wValue == ABSTRACT_STATE_FEATURE)
    {
        if(g_current_interface < USB_MAX_CDC_SUPPORTED_INTERFACES)
        {
            /* set line coding data*/
            for (count = 0; count < COMM_FEATURE_DATA_SIZE; count++)
            {
                g_abstract_state[g_current_interface][count] = *(data + count);
            }
            return 0;
        }
    }
    else if(setup_packet->wValue == COUNTRY_SETTING_FEATURE)
    {
        if(g_current_interface < USB_MAX_CDC_SUPPORTED_INTERFACES)
        {
            /* set line coding data*/
            for (count = 0; count < COMM_FEATURE_DATA_SIZE; count++)
            {
                g_country_code[g_current_interface][count] = *(data + count);
            }
            return 0;
        }
    }
    else
    {
    }

    return 1;
}


/*  */
unsigned int cdc_setup_handle(unsigned int handle,unsigned int stat,unsigned char *bufaddr,unsigned int len)
{
	PUDC_BUS pBus = (PUDC_BUS)handle;
	USB_DeviceRequest *dreq = (USB_DeviceRequest *)bufaddr;
	if(len != 8)
		return 0;

    /* vendor specific request  */
	if((dreq->bmRequestType & USB_REQUEST_CLASS_MASK) == USB_REQUEST_CLASS_VENDOR)
	{
		return 0;
	}
    /* call for class/subclass specific requests */
    else if((dreq->bmRequestType & USB_REQUEST_CLASS_MASK) == USB_REQUEST_CLASS_CLASS)
    {

        switch(dreq->bRequest)
        {
            case SEND_ENCAPSULATED_COMMAND :
                /* Add code to transfer Request and Acknowledgement */
                break;
            case GET_ENCAPSULATED_RESPONSE :
                /* 
                   Add code for handling Transfer Response/Requests and
                   Notification 
                */
                break;
            case SET_COMM_FEATURE :
                cdc_pstn_set_comm_feature(handle, dreq);
                break;
            case GET_COMM_FEATURE :
                cdc_pstn_get_comm_feature(handle, dreq);
                break;
            case CLEAR_COMM_FEATURE : /* Verify this implementation */
                cdc_pstn_set_comm_feature(handle, dreq);
              break;
            case GET_LINE_CODING :
                cdc_pstn_get_line_coding(handle, dreq);
                break;
            case SET_LINE_CODING :
                cdc_pstn_set_line_coding(handle, dreq);
                break;
            case SET_CONTROL_LINE_STATE :
                cdc_pstn_set_ctrl_line_state(handle, dreq);
                break;
            case SEND_BREAK :
                cdc_pstn_send_break(handle, dreq);
                break;
            default:  
                break;
        }
        
        return 0;
    }
    /* Standard Request function pointers */
	else if((dreq->bmRequestType & USB_REQUEST_CLASS_MASK) == USB_REQUEST_CLASS_STRD)
	{
    	switch (dreq->bRequest) 
        {
    	case GET_DESCRIPTOR:
			switch(dreq->wValue >> 8) 
			{
			case DEVICE_DESCRIPTOR:
				//printf("\n\nGet Device\n");
				cdc_get_dev_descriptor(handle,dreq);
				break;
			case CONFIGURATION_DESCRIPTOR:
				//printf("\n\nGet Config!\n");
				cdc_get_dev_configuration(handle,dreq);
				break;
			case STRING_DESCRIPTOR:
				//printf("\n\nGet string!\n");
				cdc_get_dev_descriptor_string(handle,dreq);
				break;
			case DEVICE_QUALIFIER_DESCRIPTOR:
				//printf("\n\nGet qualify!\n");
				cdc_get_dev_qualify_descitptor(handle,dreq);
				break;
            case OTHER_SPEED_CONFIGURATION_DESCRIPTOR:
                //printf("\n\nGet other speed!\n");
                cdc_get_dev_other_speed(handle,dreq);
			}
		break;
        
    	case SET_ADDRESS:
    		//printf("Set Address\n");
    		pBus->SetAddress(handle,dreq->wValue);
    		break;
    		
    	case GET_STATUS:
    		//printf("Set STATUS\n");
    		switch (dreq->bmRequestType) {
    		case 80:	/* device */

    			pBus->StartTransfer(handle,cdc_pipe[0].ep,(u8*)&cdc_get_state,2);			
    			break;
    		case 81:	/* interface */
    		case 82:	/* ep */
    			pBus->StartTransfer(handle,cdc_pipe[0].ep,(u8*)&cdc_get_state,2);			
    			break;
    		}

    		break;
       
    	case CLEAR_FEATURE:
    		//printf("CLEAR_FEATURE!\n");
    		break;
    		
    	case SET_CONFIGURATION:
            //printf("SET_CONFIGURATION!\n");
    		pBus->SetConfig(handle);
			g_cdc_active= TRUE;
    		break;
    		
    	case SET_INTERFACE:
    		//printf("SET_INTERFACE!\n");
    		break;
    	case SET_FEATURE:
    		//printf("SET_FEATURE!\n");
    		break;
    	default:	
            //printf("protal isn't surporst\n");
    		break;
    	}
	}
	return 0;
}


static unsigned int cdc_handle_receive(unsigned handle,unsigned char *buf,unsigned int len)
{
	unsigned int count;
	unsigned char *pbuf;
	unsigned char err;
	
 	//printf("receive: %d, w: %d\n", len, g_cdc_WritePoint);
    // 	printf("r: \t%s\n", buf);

	count = g_cdc_WritePoint + len;
    pbuf = buf;
    
	if (count>=DATA_BUFFMAX)
	{
		count = count % DATA_BUFFMAX;
		memcpy((cdc_RcvBuf+g_cdc_WritePoint), pbuf, DATA_BUFFMAX-g_cdc_WritePoint);	
		memcpy(cdc_RcvBuf, pbuf+DATA_BUFFMAX-g_cdc_WritePoint, count);	
	}
	else
		memcpy(cdc_RcvBuf+g_cdc_WritePoint, pbuf, len);	
	
	g_cdc_WritePoint = count%DATA_BUFFMAX;
	
	err = os_MailBoxPost(cdc_read_mbox, NULL);
	if (err)
		printf("udp receive post error: %d", err);
		
	return 0;
}



static unsigned int cdc_handle_send(unsigned handle,unsigned char *buf,unsigned int len)
{

// 	printf("send: %d\n", len);
	return 0;
}


static unsigned int cdc_notify(unsigned int handle,unsigned int stat,unsigned char *bufaddr,unsigned int len)
{
	unsigned int ret = 0;
	
	//printf("Bus notify stat :%x \n",stat);
	
	if((stat < 0x200) && (stat >=0x100))
	   return cdc_detect(handle,stat);
	if(stat == UDC_RESET)
		return cdc_reset(handle);
	if(stat == UDC_SUSPEND)
		return cdc_suspend(handle);
	if((stat >= UDC_FULLSPEED) && (stat <= UDC_HIGHSPEED))
		return cdc_set_speed(stat);
	if(stat == UDC_SETUP_PKG_FINISH)
	{
		//printf("handle setup\n");
		return cdc_setup_handle(handle,stat,bufaddr,len);
	}
	if(stat == UDC_PROTAL_RECEIVE_FINISH)
	{
		ret = cdc_handle_receive(handle,bufaddr,len);
		return ret;
	}
	if(stat == UDC_PROTAL_SEND_FINISH)
	{
		ret = cdc_handle_send(handle,bufaddr,len);
		return ret;
	}
	return 0;
}


static int	cdc_buf_count(void)
{
	return ((g_cdc_WritePoint - g_cdc_ReadPoint + DATA_BUFFMAX)%DATA_BUFFMAX);
}


int cdc_open(void)
{
	if (g_cdc_isOpen == TRUE)
		return 1;	//already open
	
	cdc_devDesc.idProduct = USB_FUNCTION_CDC_PRODUCT_ID;

	g_cdc_active = FALSE;	
	g_cdc_WritePoint = 0;
	g_cdc_ReadPoint = 0;	

	cdc_read_mbox = os_MailBoxCreate(0);
	if(cdc_read_mbox == NULL)
		return -1;
    
    SetEp(cdc_pipe, 4);
	if (CreateDevice(cdc_notify) == 0)
		return -2;

	g_cdc_isOpen = TRUE;
	return 0;
	
}

int cdc_close(void)
{
    unsigned char err;
	
	if (g_cdc_isOpen == FALSE)
		return 1;	//already close
		
    os_MailBoxDelete(cdc_read_mbox ,1, &err);
    if(err != 0)
    {
        printf("close cdc mailbox err:%d\n",err);
    }


    CloseDevice();
	
	g_cdc_isOpen = FALSE;

	return 0;
}





int cdc_write (unsigned char * buff, unsigned int size)
{
	if (buff == NULL)
		return -1;
	
	if (g_cdc_isOpen == FALSE)
		return -2;
	
	
	if (!g_cdc_active)
		return -3;
	
	
	udc_Send(CDC_IN_EP, buff, size);

	return size;
}


int cdc_read(void* buf, unsigned int count, int timeout)
{

	unsigned int remain = count;
	unsigned int readcnt;
	unsigned int total = 0;
	unsigned int i, j;
	unsigned char err;
	unsigned char *p = (unsigned char*)buf;
	
	if (g_cdc_isOpen == FALSE)
		return 0;
	
	if(buf == NULL)
		return 0;
	
	if (!g_cdc_active)
		return 0;

	if(cdc_buf_count() == 0){
		if(timeout == TIMEOUT_NOWAIT) return 0;
		ResetWatchDog();
		os_MailBoxPend(cdc_read_mbox, (timeout+9)/10, &err);
		if (err == OS_TIMEOUT) return 0;
	}

	while (1)
	{
		i = cdc_buf_count();
		readcnt = (remain > i) ? i : remain;

		if ((g_cdc_ReadPoint+readcnt)>DATA_BUFFMAX)
		{
			j = DATA_BUFFMAX-g_cdc_ReadPoint;
			memcpy(p, cdc_RcvBuf+g_cdc_ReadPoint, j);
			memcpy(p+j, cdc_RcvBuf, readcnt-j);
			g_cdc_ReadPoint = (g_cdc_ReadPoint+readcnt)%DATA_BUFFMAX;
		}
		else
		{
			memcpy(p, cdc_RcvBuf+g_cdc_ReadPoint, readcnt);
			g_cdc_ReadPoint += readcnt;
		}

		p += readcnt;
		remain -= readcnt;
		total += readcnt;

		if((0 == remain) || (timeout == TIMEOUT_NOWAIT))	//read enough
			break;

		os_MailBoxPend(cdc_read_mbox, (timeout+9)/10, &err);
		if (err == OS_TIMEOUT) 	break;

	}

	return total;
}

BOOL cdc_active(void)
{
	return g_cdc_active;
}

