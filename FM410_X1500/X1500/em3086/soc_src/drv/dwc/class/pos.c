#include <os_api.h>
#include <udcbus.h>
#include <usb.h>
#include "dwc.h"
#include <sysdefs.h>

#include "generate.h"
#include "watchdog.h"
#include "compat.h"

#include "hid_sendkey.h"
#include <string.h>
#if 0
#define dprintf printf
#else
#define dprintf(x...)
#endif
#ifndef NULL
#define NULL
#endif

#define UNUSED(x)   (void)x;

#define POS_IN_EP	1
#define POS_OUT_EP	2

#define  KEYBOARD_BUFF_SIZE    (8)   /* report buffer size */
#define KBW_REQ_DATA_SIZE     (1)
#define POS_DATA_BUFFMAX    (1024*32)
#define UPOS_BUFF_SIZE    (64)   /* report buffer size */
#define MAX_POS_LEN 56

PIPE pos_pipe[] = {
	              {0,ENDPOINT_TYPE_CONTROL,64},
				  {(0x80|POS_IN_EP),ENDPOINT_TYPE_INTERRUPT,64},
                  {POS_OUT_EP,ENDPOINT_TYPE_INTERRUPT,64}
               };


static volatile BOOL g_pos_active = FALSE;

static BOOL g_pos_isOpen = FALSE;

static USB_DeviceDescriptor pos_devDesc = 
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
	USB_FUNCTION_HID_POS_PRODUCT_ID,
	DRIVER_VERSION_NUM_HID_POS,
	STRING_MANUFACTURER, 	//iManufacturer;
	STRING_PRODUCT,   //iProduct;
	STRING_SN,
	0x01
};

static u8 pos_report_descriptor[]=
{
	0x05, 0x8c, 
	0x09, 0x02, 
	0xa1, 0x01, 
	0x09, 0x07, 
	0xa1, 0x02, 
	0x85, 0x02, 
	0x15, 0x00, 
	0x26, 0xff, 0x00,
	0x75, 0x08, 
	0x95, 0x3f, 
	0x09, 0xfe, 
	0x82, 0x02, 0x01,
	0xc0, 
	
	0x09, 0x06, 
	0xa1, 0x02, 
	0x85, 0x04, 
	0x15, 0x00, 
	0x26, 0xff, 0x00, 
	0x75, 0x08, 
	0x95, 0x3f, 
	0x09, 0x00, 
	0x91, 0x96, 
	0xc0,
	0xc0
};


#define	CONFIG_DESCRIPTOR_LEN	(sizeof(USB_ConfigDescriptor) + \
				 sizeof(USB_InterfaceDescriptor) + \
				 sizeof(hid_kbw_t) +\
				 sizeof(USB_EndPointDescriptor)*2 )

static struct {
	USB_ConfigDescriptor    configuration_descriptor;
	USB_InterfaceDescriptor interface_descritor;
    hid_kbw_t   hid;
	USB_EndPointDescriptor  ep_in;
    USB_EndPointDescriptor  ep_out;
} __attribute__ ((packed)) pos_confDesc = {
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
		0x02,	/* ep number */
		0x03,   /* Communication Interface Class */
		0x00,   /* Abstract Control Model */
		0x00,   /* Interface Protocol*/
		STRING_HID_POS    /* Interface Description String Index*/
	},


    /* HID descriptor */
	{
		sizeof(hid_kbw_t),
        USB_HID_DESCRIPTOR,
        0x0110,
        0x00,
        0x01,
        0x22,
        sizeof(pos_report_descriptor)
	},
	
    /*Endpoint 1 Descriptor*/
	{
		sizeof(USB_EndPointDescriptor),
		ENDPOINT_DESCRIPTOR,
		(1 << 7) | POS_IN_EP,// endpoint 1 is IN endpoint
		ENDPOINT_TYPE_INTERRUPT, /* interrupt */
		64, /* IN EP FIFO size */
		0x0a
	},
	
    /*Endpoint 2 Descriptor*/
	{
		sizeof(USB_EndPointDescriptor),
		ENDPOINT_DESCRIPTOR,
		POS_OUT_EP,              /* endpoint 2 is OUT endpoint*/
		ENDPOINT_TYPE_INTERRUPT, /* interrupt */
		64,                      /* OUT EP FIFO size */
		0x0a
	}

};


static USB_DeviceQualifierDescriptor pos_devQualifyDesc =
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

static u32 pos_get_state = 0;
static unsigned char pos_RcvBuf[POS_DATA_BUFFMAX];
static int g_pos_ReadPoint = 0;
static int g_pos_WritePoint = 0;
OS_EVENT *pos_read_mbox =  NULL;

static unsigned int pos_detect(unsigned int handle,unsigned int stat)
{

	return 1;	
}

static unsigned int pos_reset(unsigned int handle)
{

	g_pos_active = FALSE;
	return 1;
	
}


static unsigned int pos_suspend(unsigned int handle)
{
	return 1;
}


static unsigned int pos_set_speed(unsigned stat)
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

static  void pos_get_dev_report_descriptor(unsigned int handle,USB_DeviceRequest *dreq)
{

    PUDC_BUS pBus = (PUDC_BUS)handle;
	
	pBus->StartTransfer(handle,pos_pipe[0].ep, pos_report_descriptor, sizeof(pos_report_descriptor));
}


static  void pos_get_dev_hid_descriptor(unsigned int handle,USB_DeviceRequest *dreq)
{
    PUDC_BUS pBus = (PUDC_BUS)handle;
	
	pBus->StartTransfer(handle,pos_pipe[0].ep, (u8 *)(&pos_confDesc.hid), sizeof(hid_kbw_t));
}


static  void pos_get_dev_descriptor(unsigned int handle,USB_DeviceRequest *dreq)
{
    PUDC_BUS pBus = (PUDC_BUS)handle;
    unsigned short size = dreq->wLength;

	if(size < sizeof(pos_devDesc))
	{
		pos_devDesc.bLength = size;
		pBus->StartTransfer(handle,pos_pipe[0].ep,(unsigned char *)&pos_devDesc, size);
	}
	
	else
	{
		pos_devDesc.bLength = sizeof(pos_devDesc);
		pBus->StartTransfer(handle,pos_pipe[0].ep,(unsigned char *)&pos_devDesc, sizeof(pos_devDesc));
	}
}

static  void pos_get_dev_configuration(unsigned int handle,USB_DeviceRequest *dreq)
{
	PUDC_BUS pBus = (PUDC_BUS)handle;
	switch (dreq->wLength) {
	case 9:
		pBus->StartTransfer(handle,pos_pipe[0].ep, (unsigned char *)&pos_confDesc, 9);
		break;
	case 8:
		pBus->StartTransfer(handle,pos_pipe[0].ep, (unsigned char *)&pos_confDesc, 8);
		break;
	default:
		pBus->StartTransfer(handle,pos_pipe[0].ep, (unsigned char *)&pos_confDesc, sizeof(pos_confDesc));
		break;
	}
}

static  void pos_get_dev_qualify_descitptor(unsigned int handle,USB_DeviceRequest *dreq)
{
    PUDC_BUS pBus = (PUDC_BUS)handle;
    pBus->StartTransfer(handle,pos_pipe[0].ep, (unsigned char *)&pos_devQualifyDesc, sizeof(pos_devQualifyDesc));	
}


static  void pos_get_dev_other_speed(unsigned int handle,USB_DeviceRequest *dreq)
{
}


static inline void pos_get_dev_descriptor_string(unsigned int handle,USB_DeviceRequest *dreq)
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
	
	pBus->StartTransfer(handle,pos_pipe[0].ep, buf, len);

}


/*  */
unsigned int pos_setup_handle(unsigned int handle,unsigned int stat,unsigned char *bufaddr,unsigned int len)
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
        pBus->StartTransfer(handle, pos_pipe[0].ep, NULL, 0);	
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
				pos_get_dev_descriptor(handle,dreq);
				break;
			case CONFIGURATION_DESCRIPTOR:
				//printf("\n\nGet Config!\n");
				pos_get_dev_configuration(handle,dreq);
				break;
			case STRING_DESCRIPTOR:
				//printf("\n\nGet string!\n");
				pos_get_dev_descriptor_string(handle,dreq);
				break;
			case DEVICE_QUALIFIER_DESCRIPTOR:
				//printf("\n\nGet qualify!\n");
				pos_get_dev_qualify_descitptor(handle,dreq);
				break;
            case OTHER_SPEED_CONFIGURATION_DESCRIPTOR:
                //printf("\n\nGet other speed!\n");
                pos_get_dev_other_speed(handle,dreq);
                break;
            case USB_REPORT_DESCRIPTOR:
                pos_get_dev_report_descriptor(handle, dreq);
                break;
            case USB_HID_DESCRIPTOR:
                pos_get_dev_hid_descriptor(handle, dreq);
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

    			pBus->StartTransfer(handle,pos_pipe[0].ep,(u8*)&pos_get_state,2);			
    			break;
    		case 81:	/* interface */
    		case 82:	/* ep */
    			pBus->StartTransfer(handle,pos_pipe[0].ep,(u8*)&pos_get_state,2);			
    			break;
    		}

    		break;
       
    	case CLEAR_FEATURE:
    		//printf("CLEAR_FEATURE!\n");
    		break;
    		
    	case SET_CONFIGURATION:
            //printf("SET_CONFIGURATION!\n");
    		pBus->SetConfig(handle);
			g_pos_active= TRUE;
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

static unsigned int pos_handle_receive(unsigned handle,unsigned char *buf,unsigned int len)
{
	unsigned int count;
	unsigned char *pbuf;
	unsigned char err;
    u8 TransBuf[64]={0};
    int RecvLen=0;
    
 	//printf("receive: %d, w: %d\n", len, g_pos_WritePoint);
    // 	printf("r: \t%s\n", buf);
    memcpy(TransBuf, buf, len);
    RecvLen = TransBuf[1];

	count = g_pos_WritePoint + RecvLen;
    pbuf = &TransBuf[2];
    
	if (count>=POS_DATA_BUFFMAX)
	{
		count = count % POS_DATA_BUFFMAX;
		memcpy((pos_RcvBuf+g_pos_WritePoint), pbuf, POS_DATA_BUFFMAX-g_pos_WritePoint);	
		memcpy(pos_RcvBuf, pbuf+POS_DATA_BUFFMAX-g_pos_WritePoint, count);	
	}
	else
		memcpy(pos_RcvBuf+g_pos_WritePoint, pbuf, RecvLen);	
	
	g_pos_WritePoint = count%POS_DATA_BUFFMAX;
	
	err = os_MailBoxPost(pos_read_mbox, NULL);
	if (err)
		printf("udp receive post error: %d", err);
		
	return 0;
}

static unsigned int pos_handle_send(unsigned handle,unsigned char *buf,unsigned int len)
{

// 	printf("send: %d\n", len);
	return 0;
}


static unsigned int pos_notify(unsigned int handle,unsigned int stat,unsigned char *bufaddr,unsigned int len)
{
	unsigned int ret = 0;
	
	//printf("Bus notify stat :%x \n",stat);
	
	if((stat < 0x200) && (stat >=0x100))
	   return pos_detect(handle,stat);
	if(stat == UDC_RESET)
		return pos_reset(handle);
	if(stat == UDC_SUSPEND)
		return pos_suspend(handle);
	if((stat >= UDC_FULLSPEED) && (stat <= UDC_HIGHSPEED))
		return pos_set_speed(stat);
	if(stat == UDC_SETUP_PKG_FINISH)
	{
		//printf("handle setup\n");
		return pos_setup_handle(handle,stat,bufaddr,len);
	}
	if(stat == UDC_PROTAL_RECEIVE_FINISH)
	{
        ret = pos_handle_receive(handle,bufaddr,len);
		return ret;	
    }
	if(stat == UDC_PROTAL_SEND_FINISH)
	{
		ret = pos_handle_send(handle,bufaddr,len);
		return ret;
	}
	return 0;
}

static int	pos_buf_count(void)
{
	return ((g_pos_WritePoint - g_pos_ReadPoint + POS_DATA_BUFFMAX)%POS_DATA_BUFFMAX);
}


int pos_open(void)
{
	if (g_pos_isOpen == TRUE)
		return 1;	//already open
	
	pos_devDesc.idProduct = USB_FUNCTION_HID_POS_PRODUCT_ID;

	g_pos_active = FALSE;	
	g_pos_WritePoint = 0;
	g_pos_ReadPoint = 0;	

	pos_read_mbox = os_MailBoxCreate(0);
	if(pos_read_mbox == NULL)
		return -1;

    SetEp(pos_pipe, 3);
	if (CreateDevice(pos_notify) == 0)
		return -2;

	g_pos_isOpen = TRUE;
	return 0;
	
}

int pos_close(void)
{
	unsigned char err;
    
	if (g_pos_isOpen == FALSE)
		return 1;	//already close

    os_MailBoxDelete(pos_read_mbox ,1, &err);
    if(err != 0)
    {
        printf("close pos mailbox err:%d\n",err);
    }


    CloseDevice();
	
	g_pos_isOpen = FALSE;

	return 0;
}

int pos_write (unsigned char *buff, unsigned int size)
{
    int Remainlen=0;
	u8 SendBuf[64]={0};
    int len;
    int i;
    int packets ;

    packets = (size/MAX_POS_LEN) + ((size%MAX_POS_LEN)?1:0);
    //printf("pos write:%d %d %d\n",size, packets, size/MAX_POS_LEN);
	if (buff == NULL)
		return -1;
	
	if (g_pos_isOpen == FALSE)
		return -2;
	
	if (!g_pos_active)
		return -3;

    for(i=0; i<packets; i++)
    {
        memset(SendBuf, 0, 64);
		len =  MIN((size-Remainlen), MAX_POS_LEN);

		SendBuf[0] = 0x02;
		SendBuf[1] = len;
		memcpy(&SendBuf[2], &buff[Remainlen], len);
		if((size- Remainlen) > MAX_POS_LEN)
			SendBuf[63] = 0x01;

		udc_Send(POS_IN_EP, SendBuf, UPOS_BUFF_SIZE);
		Remainlen += len;

    }

	return size;
}

int pos_read(void* buf, unsigned int count, int timeout)
{

	unsigned int remain = count;
	unsigned int readcnt;
	unsigned int total = 0;
	unsigned int i, j;
	unsigned char err;
	unsigned char *p = (unsigned char*)buf;
	
	if (g_pos_isOpen == FALSE)
		return 0;
	
	if(buf == NULL)
		return 0;
	
	if (!g_pos_active)
		return 0;

	if(pos_buf_count() == 0){
		if(timeout == TIMEOUT_NOWAIT) return 0;
		ResetWatchDog();
		os_MailBoxPend(pos_read_mbox, (timeout+9)/10, &err);
		if (err == OS_TIMEOUT) return 0;
	}

	while (1)
	{
		i = pos_buf_count();
		readcnt = (remain > i) ? i : remain;

		if ((g_pos_ReadPoint+readcnt) > POS_DATA_BUFFMAX)
		{
			j = POS_DATA_BUFFMAX-g_pos_ReadPoint;
			memcpy(p, pos_RcvBuf+g_pos_ReadPoint, j);
			memcpy(p+j, pos_RcvBuf, readcnt-j);
			g_pos_ReadPoint = (g_pos_ReadPoint+readcnt)%POS_DATA_BUFFMAX;
		}
		else
		{
			memcpy(p, pos_RcvBuf+g_pos_ReadPoint, readcnt);
			g_pos_ReadPoint += readcnt;
		}

		p += readcnt;
		remain -= readcnt;
		total += readcnt;

		if((0 == remain) || (timeout == TIMEOUT_NOWAIT))	//read enough
			break;

		os_MailBoxPend(pos_read_mbox, (timeout+9)/10, &err);
		if (err == OS_TIMEOUT) 	break;

	}

	return total;
}


BOOL pos_active(void)
{
	return g_pos_active;
}


