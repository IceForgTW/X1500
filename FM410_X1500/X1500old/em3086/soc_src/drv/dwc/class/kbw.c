#include <os_api.h>
#include <udcbus.h>
#include <usb.h>
#include "dwc.h"
#include <sysdefs.h>

#include "../../misc/generate.h"
#include "../../watchdog/watchdog.h"
#include "../compat.h"

#include "hid_sendkey.h"

#if 0
#define dprintf printf
#else
#define dprintf(x...)
#endif
#ifndef NULL
#define NULL
#endif

#define UNUSED(x)   (void)x;


#define  KEYBOARD_BUFF_SIZE    (8)   /* report buffer size */
#define KBW_REQ_DATA_SIZE     (1)
//extern int req_num;
PIPE kbw_pipe[] = {
	              {0,ENDPOINT_TYPE_CONTROL,64},
				  {(0x80|KBW_IN_EP),ENDPOINT_TYPE_INTERRUPT,8}
               };

volatile int req_num = 0;
static volatile BOOL g_kbw_active = FALSE;

static BOOL g_kbw_isOpen = FALSE;

static USB_DeviceDescriptor kbw_devDesc = 
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
	USB_FUNCTION_HID_KEYBOARD_PRODUCT_ID,
	DRIVER_VERSION_NUM_HID_KBW,
	STRING_MANUFACTURER, 	//iManufacturer;
	STRING_PRODUCT,   //iProduct;
	STRING_SN,
	0x01
};

static u8 kbw_report_descriptor[]=
{
	0x05, 0x01, // USAGE_PAGE(generic desktop)
	0x09, 0x06, // USAGE (Keyboard)
	0xa1, 0x01, // COLLECTION (Application)
	0x05, 0x07, // USAGE_PAGE(keyboard/Keypad(0x07))
	0x19, 0xe0, // USAGE_MINIMN(Keyboard LeftControl)
	0x29, 0xe7, // USAGE_MAXINUM(Keyboard Right GUI)
	0x15, 0x00, // LOGICAL_MINIMUN(0)
	0x25, 0x01, // LOGICAL_MAXIMUN(1)	
	0x75, 0x01, // REPORT_SIZE(1)	
	0x95, 0x08, // REPORT_COUNT(8)	
	0x81, 0x02, // INPUT(Data, Var, Abs)
	0x95, 0x01, // REPORT_COUNT(1)
	0x75, 0x08, // REPORT_SIZE(8)
	
	0x81, 0x03, // INPUT(Cnst, Var, Abs)
	0x95, 0x05, // REPORT_COUNT(5)
	0x75, 0x01, // REPORT_SIZE(1)
	0x05, 0x08, // USAGE_PAGE(LEDs)
	0x19, 0x01, // USAGE_MINIMUM(Num Lock)
	0x29, 0x05, // USAGE_MAXIMUM(Kana)
	0x91, 0x02, // OUTPUT(Data, Var, Abs)
	0x95, 0x01, // REPORT_COUNT(1)
	0x75, 0x03, // REPORT_SIZE(3)
	0x91, 0x03, // OUTPUT(Cnst, Var, Abs)
	
	0x95, 0x06, // REPORT_COUNT(6)
	0x75, 0x08, // REPORT_SIZE(8)
	0x15, 0x00, // LOGICAL_MININUM(0)
	0x25, 0x65, // LOGICAL_MAXIMUN(1)
	0x05, 0x07, // USAGE_PAGE(keyboard/Keypad)
	0x19, 0x00, // USAGE_MINIMUM(Reserved(no event indicated))
	0x29, 0x65, // USAGE_MAXINUM(keyboard Application)	
	0x81, 0x00, // INPUT(Data, Ary, Abs)
	0xc0		// END_COLLECTION
};


#define	CONFIG_DESCRIPTOR_LEN	(sizeof(USB_ConfigDescriptor) + \
				 sizeof(USB_InterfaceDescriptor) + \
				 sizeof(hid_kbw_t) +\
				 sizeof(USB_EndPointDescriptor) )

static struct {
	USB_ConfigDescriptor    configuration_descriptor;
	USB_InterfaceDescriptor interface_descritor;
    hid_kbw_t   hid;
	USB_EndPointDescriptor  ep_in;
} __attribute__ ((packed)) kbw_confDesc = {
	{
		sizeof(USB_ConfigDescriptor),
		CONFIGURATION_DESCRIPTOR,
		CONFIG_DESCRIPTOR_LEN,  /*  Total length of the Configuration descriptor */ 
		0x01,                   /*  NumInterfaces */
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
		0x03,   /* Communication Interface Class */
		0x01,   /* Abstract Control Model */
		0x01,   /* Interface Protocol*/
		STRING_HID_KBD    /* Interface Description String Index*/
	},


    /* HID descriptor */
	{
		sizeof(hid_kbw_t),
        USB_HID_DESCRIPTOR,
        0x0110,
        0x00,
        0x01,
        0x22,
        sizeof(kbw_report_descriptor)
	},
	
    /*Endpoint 1 Descriptor*/
	{
		sizeof(USB_EndPointDescriptor),
		ENDPOINT_DESCRIPTOR,
		(1 << 7) | KBW_IN_EP,// endpoint 1 is IN endpoint
		ENDPOINT_TYPE_INTERRUPT, /* interrupt */
		8, /* IN EP FIFO size */
		0x01
	},
};


static USB_DeviceQualifierDescriptor kbw_devQualifyDesc =
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

static u32 kbw_get_state = 0;
static u8 g_kbw_request_params[2];          /* for get/set idle and*/
static u8 kbw_rpt_buf[KEYBOARD_BUFF_SIZE];      /* Key Press Report Buffer */

static u8 g_kbw_sysKeyboardStatus;

static unsigned int kbw_detect(unsigned int handle,unsigned int stat)
{

	return 1;	
}

static unsigned int kbw_reset(unsigned int handle)
{

	g_kbw_active = FALSE;
	return 1;
	
}


static unsigned int kbw_suspend(unsigned int handle)
{
	return 1;
}


static unsigned int kbw_set_speed(unsigned stat)
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

static  void kbw_get_dev_report_descriptor(unsigned int handle,USB_DeviceRequest *dreq)
{

    PUDC_BUS pBus = (PUDC_BUS)handle;
	
	pBus->StartTransfer(handle,kbw_pipe[0].ep, kbw_report_descriptor, sizeof(kbw_report_descriptor));
}


static  void kbw_get_dev_hid_descriptor(unsigned int handle,USB_DeviceRequest *dreq)
{
    PUDC_BUS pBus = (PUDC_BUS)handle;
	
	pBus->StartTransfer(handle,kbw_pipe[0].ep, (u8 *)(&kbw_confDesc.hid), sizeof(hid_kbw_t));
}


static  void kbw_get_dev_descriptor(unsigned int handle,USB_DeviceRequest *dreq)
{
    PUDC_BUS pBus = (PUDC_BUS)handle;
    unsigned short size = dreq->wLength;

	if(size < sizeof(kbw_devDesc))
	{
		kbw_devDesc.bLength = size;
		pBus->StartTransfer(handle,kbw_pipe[0].ep,(unsigned char *)&kbw_devDesc, size);
	}
	
	else
	{
		kbw_devDesc.bLength = sizeof(kbw_devDesc);
		pBus->StartTransfer(handle,kbw_pipe[0].ep,(unsigned char *)&kbw_devDesc, sizeof(kbw_devDesc));
	}
}

static  void kbw_get_dev_configuration(unsigned int handle,USB_DeviceRequest *dreq)
{
	PUDC_BUS pBus = (PUDC_BUS)handle;
	switch (dreq->wLength) {
	case 9:
		pBus->StartTransfer(handle,kbw_pipe[0].ep, (unsigned char *)&kbw_confDesc, 9);
		break;
	case 8:
		pBus->StartTransfer(handle,kbw_pipe[0].ep, (unsigned char *)&kbw_confDesc, 8);
		break;
	default:
		pBus->StartTransfer(handle,kbw_pipe[0].ep, (unsigned char *)&kbw_confDesc, sizeof(kbw_confDesc));
		break;
	}
}

static  void kbw_get_dev_qualify_descitptor(unsigned int handle,USB_DeviceRequest *dreq)
{
    PUDC_BUS pBus = (PUDC_BUS)handle;
    pBus->StartTransfer(handle,kbw_pipe[0].ep, (unsigned char *)&kbw_devQualifyDesc, sizeof(kbw_devQualifyDesc));	
}


static  void kbw_get_dev_other_speed(unsigned int handle,USB_DeviceRequest *dreq)
{
}


static inline void kbw_get_dev_descriptor_string(unsigned int handle,USB_DeviceRequest *dreq)
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
	
	pBus->StartTransfer(handle,kbw_pipe[0].ep, buf, len);

}


/*  */
unsigned int kbw_setup_handle(unsigned int handle,unsigned int stat,unsigned char *bufaddr,unsigned int len)
{
	PUDC_BUS pBus = (PUDC_BUS)handle;
	USB_DeviceRequest *dreq = (USB_DeviceRequest *)bufaddr;
    u8 *data;

    
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
        u8 index = (u8)((dreq->bRequest - 2) & USB_HID_REQUEST_TYPE_MASK);
                                             /* index == 0 for get/set idle,
                                                index == 1 for get/set protocol
                                             */
        u16 value = dreq->wValue;
                                             
        switch(dreq->bRequest)
        {

            case USB_HID_GET_REPORT_REQUEST :
                pBus->StartTransfer(handle,kbw_pipe[0].ep, kbw_rpt_buf, KEYBOARD_BUFF_SIZE);
                break;

            case USB_HID_SET_REPORT_REQUEST :
                data = bufaddr + 8;
                
                for(index = 0; index < REPORT_SIZE; index++)
                {   /* copy the data sent by host in the buffer */
                  kbw_rpt_buf[index] = (*data + index);
                }
                g_kbw_sysKeyboardStatus = kbw_rpt_buf[0];
                pBus->StartTransfer(handle, kbw_pipe[0].ep, NULL,  0);
                break;

            case USB_HID_GET_IDLE_REQUEST :
                /* point to the current idle rate */
                pBus->StartTransfer(handle,kbw_pipe[0].ep, &g_kbw_request_params[index], KBW_REQ_DATA_SIZE);
                break;

            case USB_HID_SET_IDLE_REQUEST :
                /* set the idle rate sent by the host */
                if(index <2)
                {
                    g_kbw_request_params[index] =(u8)((value & MSB_MASK) >> HIGH_BYTE_SHIFT);
                }
                pBus->StartTransfer(handle, kbw_pipe[0].ep, NULL,  0);
                break;

            case USB_HID_GET_PROTOCOL_REQUEST :
                /* point to the current protocol code
                   0 = Boot Protocol
                   1 = Report Protocol*/
                pBus->StartTransfer(handle,kbw_pipe[0].ep, &g_kbw_request_params[index], KBW_REQ_DATA_SIZE);	
                break;

            case USB_HID_SET_PROTOCOL_REQUEST :
                /* set the protocol sent by the host
                     0 = Boot Protocol
                     1 = Report Protocol*/
                if(index < 2)
                {
                    g_kbw_request_params[index] = (u8)(value);
                }
                pBus->StartTransfer(handle, kbw_pipe[0].ep, NULL,  0);
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
				kbw_get_dev_descriptor(handle,dreq);
				break;
			case CONFIGURATION_DESCRIPTOR:
				//printf("\n\nGet Config!\n");
				kbw_get_dev_configuration(handle,dreq);
				break;
			case STRING_DESCRIPTOR:
				//printf("\n\nGet string!\n");
				kbw_get_dev_descriptor_string(handle,dreq);
				break;
			case DEVICE_QUALIFIER_DESCRIPTOR:
				//printf("\n\nGet qualify!\n");
				kbw_get_dev_qualify_descitptor(handle,dreq);
				break;
            case OTHER_SPEED_CONFIGURATION_DESCRIPTOR:
                //printf("\n\nGet other speed!\n");
                kbw_get_dev_other_speed(handle,dreq);
                break;
            case USB_REPORT_DESCRIPTOR:
                kbw_get_dev_report_descriptor(handle, dreq);
                break;
            case USB_HID_DESCRIPTOR:
                kbw_get_dev_hid_descriptor(handle, dreq);
                break;

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

    			pBus->StartTransfer(handle,kbw_pipe[0].ep,(u8*)&kbw_get_state,2);			
    			break;
    		case 81:	/* interface */
    		case 82:	/* ep */
    			pBus->StartTransfer(handle,kbw_pipe[0].ep,(u8*)&kbw_get_state,2);			
    			break;
    		}

    		break;
       
    	case CLEAR_FEATURE:
    		//printf("CLEAR_FEATURE!\n");
    		break;
    		
    	case SET_CONFIGURATION:
            //printf("SET_CONFIGURATION!\n");
    		pBus->SetConfig(handle);
			g_kbw_active= TRUE;
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

int get_hid_kbd_req_num(void)
{
	return req_num;
}

static unsigned int kbw_handle_send(unsigned handle,unsigned char *buf,unsigned int len)
{
    unsigned int sr;
    static unsigned int i=0;
//printf("s=%d\n", i++);
    sr = spin_lock_irqsave();
    req_num--;  	
    if(req_num < 1) 
		req_num = 0;
    //printf("S");
    spin_unlock_irqrestore(sr);
// 	printf("send: %d\n", len);
	return 0;
}


static unsigned int kbw_notify(unsigned int handle,unsigned int stat,unsigned char *bufaddr,unsigned int len)
{
	unsigned int ret = 0;
	
	//printf("Bus notify stat :%x \n",stat);
	
	if((stat < 0x200) && (stat >=0x100))
	   return kbw_detect(handle,stat);
	if(stat == UDC_RESET)
		return kbw_reset(handle);
	if(stat == UDC_SUSPEND)
		return kbw_suspend(handle);
	if((stat >= UDC_FULLSPEED) && (stat <= UDC_HIGHSPEED))
		return kbw_set_speed(stat);
	if(stat == UDC_SETUP_PKG_FINISH)
	{
		//printf("handle setup\n");
		return kbw_setup_handle(handle,stat,bufaddr,len);
	}
	if(stat == UDC_PROTAL_RECEIVE_FINISH)
	{
		return 1;
	}
	if(stat == UDC_PROTAL_SEND_FINISH)
	{
		ret = kbw_handle_send(handle,bufaddr,len);
		return ret;
	}
	return 0;
}


int kbw_open(void)
{
	if (g_kbw_isOpen == TRUE)
		return 1;	//already open
	
	g_kbw_active = FALSE;	

    SetEp(kbw_pipe, 2);
	if (CreateDevice(kbw_notify) == 0)
		return -2;

	g_kbw_isOpen = TRUE;
	return 0;
	
}

int kbw_close(void)
{
	if (g_kbw_isOpen == FALSE)
		return 1;	//already close

    CloseDevice();
	
	g_kbw_isOpen = FALSE;

	return 0;
}

int kbw_write (unsigned char * buff, unsigned int size,int nCharEncoding)
{
	if (buff == NULL)
		return -1;
	
	if (g_kbw_isOpen == FALSE)
		return -2;
	
	
	if (!g_kbw_active)
		return -3;
	
	send_key_str(buff, size,0);
	return size;
}


// 获取当前kbw设备是否枚举成功
BOOL kbw_active(void)
{
	return g_kbw_active;
}

// 获取当前键盘的状态，以获得上位机的caps lock、num loack、scroll lock的状态
int UKB_GetKbwStatus(void)
{
	return g_kbw_sysKeyboardStatus;
}

// 设置字符间隔，如果有更改则需要重新枚举设备
int UKB_KbwInterval(int control)
{
    BOOL reset = FALSE;
    
    if(kbw_confDesc.ep_in.bInterval != control)
    {
        kbw_confDesc.ep_in.bInterval = control;
        reset = TRUE;
    }
    
    if((g_kbw_active == TRUE) && (reset == TRUE))
    {
        kbw_close();
        os_TimeDelay(100);
        kbw_open();
    }
    return 0;
}

