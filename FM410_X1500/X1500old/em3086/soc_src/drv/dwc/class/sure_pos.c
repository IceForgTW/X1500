//#include <os_api.h>
#include <udcbus.h>
#include <usb.h>
#include "dwc.h"
#include <sysdefs.h>

#include "../../misc/generate.h"
#include "../../watchdog/watchdog.h"
#include "../compat.h"

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

#define SUREPOS_IN_EP	1
#define SUREPOS_OUT_EP	2

#define SUREPOS_DATA_BUFFMAX    (1024*32)
#define IBM_SurePOS_FRAME_SIZE	64  /* report buffer size */

PIPE surepos_pipe[] = {
	              {0,ENDPOINT_TYPE_CONTROL,64},
				  {(0x80|SUREPOS_IN_EP),ENDPOINT_TYPE_INTERRUPT,64},
                  {SUREPOS_OUT_EP,ENDPOINT_TYPE_INTERRUPT,64}
               };


static volatile BOOL g_surepos_active = FALSE;
static BOOL g_surepos_isOpen = FALSE;

static USB_DeviceDescriptor surepos_devDesc = 
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
	DRIVER_PRODUCT_NUM_IBM_SUREPOS_Tabletop,
	DRIVER_VERSION_NUM_IBM_SUREPOS,
	STRING_MANUFACTURER, 	//iManufacturer;
	STRING_PRODUCT,         //iProduct;
	STRING_SN,
	0x01
};

static u8 surepos_report_descriptor[]=
{
	0x06, 0x45, 0xff,
	0x0a, 0x00, 0x4a,
	0xa1, 0x01, 
	0x0a, 0x01, 0x4a,
	0x75, 0x08, 
	0x95, 0x0b, 
	0x15, 0x00, 
	0x26, 0xff, 0x00,
	0x91, 0x02, 
	0x0a, 0x02, 0x4a,
	0x95, 0x40, 
	0x81, 0x02,
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
} __attribute__ ((packed)) surepos_confDesc = {
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
		STRING_IBM_SUREPOS_TT    /* Interface Description String Index*/
	},


    /* HID descriptor */
	{
		sizeof(hid_kbw_t),
        USB_HID_DESCRIPTOR,
        0x0110,
        0x00,
        0x01,
        0x22,
        sizeof(surepos_report_descriptor)
	},
	
    /*Endpoint 1 Descriptor*/
	{
		sizeof(USB_EndPointDescriptor),
		ENDPOINT_DESCRIPTOR,
		(1 << 7) | SUREPOS_IN_EP,// endpoint 1 is IN endpoint
		ENDPOINT_TYPE_INTERRUPT, /* interrupt */
		64, /* IN EP FIFO size */
		0x02
	},
	
    /*Endpoint 2 Descriptor*/
	{
		sizeof(USB_EndPointDescriptor),
		ENDPOINT_DESCRIPTOR,
		SUREPOS_OUT_EP,              /* endpoint 2 is OUT endpoint*/
		ENDPOINT_TYPE_INTERRUPT, /* interrupt */
		64,                      /* OUT EP FIFO size */
		0x01
	}

};


static USB_DeviceQualifierDescriptor surepos_devQualifyDesc =
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

static u32 surepos_get_state = 0;
static unsigned char surepos_RcvBuf[SUREPOS_DATA_BUFFMAX];
static int g_surepos_ReadPoint = 0;
static int g_surepos_WritePoint = 0;
OS_EVENT *surepos_read_mbox =  NULL;

static unsigned int surepos_detect(unsigned int handle,unsigned int stat)
{

	return 1;	
}

static unsigned int surepos_reset(unsigned int handle)
{

	g_surepos_active = FALSE;
	return 1;
	
}


static unsigned int surepos_suspend(unsigned int handle)
{
	return 1;
}


static unsigned int surepos_set_speed(unsigned stat)
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

static  void surepos_get_dev_report_descriptor(unsigned int handle,USB_DeviceRequest *dreq)
{

    PUDC_BUS pBus = (PUDC_BUS)handle;
	
	pBus->StartTransfer(handle,surepos_pipe[0].ep, surepos_report_descriptor, sizeof(surepos_report_descriptor));
}


static  void surepos_get_dev_hid_descriptor(unsigned int handle,USB_DeviceRequest *dreq)
{
    PUDC_BUS pBus = (PUDC_BUS)handle;
	
	pBus->StartTransfer(handle,surepos_pipe[0].ep, (u8 *)(&surepos_confDesc.hid), sizeof(hid_kbw_t));
}


static  void surepos_get_dev_descriptor(unsigned int handle,USB_DeviceRequest *dreq)
{
    PUDC_BUS pBus = (PUDC_BUS)handle;
    unsigned short size = dreq->wLength;

	if(size < sizeof(surepos_devDesc))
	{
		surepos_devDesc.bLength = size;
		pBus->StartTransfer(handle,surepos_pipe[0].ep,(unsigned char *)&surepos_devDesc, size);
	}
	
	else
	{
		surepos_devDesc.bLength = sizeof(surepos_devDesc);
		pBus->StartTransfer(handle,surepos_pipe[0].ep,(unsigned char *)&surepos_devDesc, sizeof(surepos_devDesc));
	}
}

static  void surepos_get_dev_configuration(unsigned int handle,USB_DeviceRequest *dreq)
{
	PUDC_BUS pBus = (PUDC_BUS)handle;
	switch (dreq->wLength) {
	case 9:
		pBus->StartTransfer(handle,surepos_pipe[0].ep, (unsigned char *)&surepos_confDesc, 9);
		break;
	case 8:
		pBus->StartTransfer(handle,surepos_pipe[0].ep, (unsigned char *)&surepos_confDesc, 8);
		break;
	default:
		pBus->StartTransfer(handle,surepos_pipe[0].ep, (unsigned char *)&surepos_confDesc, sizeof(surepos_confDesc));
		break;
	}
}

static  void surepos_get_dev_qualify_descitptor(unsigned int handle,USB_DeviceRequest *dreq)
{
    PUDC_BUS pBus = (PUDC_BUS)handle;
    pBus->StartTransfer(handle,surepos_pipe[0].ep, (unsigned char *)&surepos_devQualifyDesc, sizeof(surepos_devQualifyDesc));	
}


static  void surepos_get_dev_other_speed(unsigned int handle,USB_DeviceRequest *dreq)
{
}


static inline void surepos_get_dev_descriptor_string(unsigned int handle,USB_DeviceRequest *dreq)
{
	int len = dreq->wLength;
	PUDC_BUS pBus = (PUDC_BUS)handle;
	unsigned char buf[256];

	//printf("send_dev_desc_string type %d \r\n",dreq->wValue & 0xff);

	len = usb_gadget_get_string(dreq->wValue & 0xff, buf);	
	if (len<=0)
	{
		printf("usb_gadget_get_string error: %d\n", len);
		return;
	}

	if (len>dreq->wLength)
		len = dreq->wLength;
	
	pBus->StartTransfer(handle,surepos_pipe[0].ep, buf, len);

}


/*  */
unsigned int surepos_setup_handle(unsigned int handle,unsigned int stat,unsigned char *bufaddr,unsigned int len)
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
        pBus->StartTransfer(handle, surepos_pipe[0].ep, NULL, 0);	
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
				surepos_get_dev_descriptor(handle,dreq);
				break;
			case CONFIGURATION_DESCRIPTOR:
				//printf("\n\nGet Config!\n");
				surepos_get_dev_configuration(handle,dreq);
				break;
			case STRING_DESCRIPTOR:
				//printf("\n\nGet string!\n");
				surepos_get_dev_descriptor_string(handle,dreq);
				break;
			case DEVICE_QUALIFIER_DESCRIPTOR:
				//printf("\n\nGet qualify!\n");
				surepos_get_dev_qualify_descitptor(handle,dreq);
				break;
            case OTHER_SPEED_CONFIGURATION_DESCRIPTOR:
                //printf("\n\nGet other speed!\n");
                surepos_get_dev_other_speed(handle,dreq);
                break;
            case USB_REPORT_DESCRIPTOR:
                surepos_get_dev_report_descriptor(handle, dreq);
                break;
            case USB_HID_DESCRIPTOR:
                surepos_get_dev_hid_descriptor(handle, dreq);
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

    			pBus->StartTransfer(handle,surepos_pipe[0].ep,(u8*)&surepos_get_state,2);			
    			break;
    		case 81:	/* interface */
    		case 82:	/* ep */
    			pBus->StartTransfer(handle,surepos_pipe[0].ep,(u8*)&surepos_get_state,2);			
    			break;
    		}

    		break;
       
    	case CLEAR_FEATURE:
    		//printf("CLEAR_FEATURE!\n");
    		break;
    		
    	case SET_CONFIGURATION:
            //printf("SET_CONFIGURATION!\n");
    		pBus->SetConfig(handle);
			g_surepos_active= TRUE;
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

static unsigned int surepos_handle_receive(unsigned handle,unsigned char *buf,unsigned int len)
{
	unsigned int count;
	unsigned char *pbuf;
	unsigned char err;
    int RecvLen=0;
    
 	//printf("receive: %d, w: %d\n", len, g_surepos_WritePoint);
    // 	printf("r: \t%s\n", buf);
	count = g_surepos_WritePoint + len;
    pbuf = buf;
    
	if (count>=SUREPOS_DATA_BUFFMAX)
	{
		count = count % SUREPOS_DATA_BUFFMAX;
		memcpy((surepos_RcvBuf+g_surepos_WritePoint), pbuf, SUREPOS_DATA_BUFFMAX-g_surepos_WritePoint);	
		memcpy(surepos_RcvBuf, pbuf+SUREPOS_DATA_BUFFMAX-g_surepos_WritePoint, count);	
	}
	else
		memcpy(surepos_RcvBuf+g_surepos_WritePoint, pbuf, RecvLen);	
	
	g_surepos_WritePoint = count%SUREPOS_DATA_BUFFMAX;
	
	err = os_MailBoxPost(surepos_read_mbox, NULL);
	if (err)
		printf("udp receive surepost error: %d", err);
		
	return 0;
}

static unsigned int surepos_handle_send(unsigned handle,unsigned char *buf,unsigned int len)
{

// 	printf("send: %d\n", len);
	return 0;
}


static unsigned int surepos_notify(unsigned int handle,unsigned int stat,unsigned char *bufaddr,unsigned int len)
{
	unsigned int ret = 0;
	
	//printf("Bus notify stat :%x \n",stat);
	
	if((stat < 0x200) && (stat >=0x100))
	   return surepos_detect(handle,stat);
	if(stat == UDC_RESET)
		return surepos_reset(handle);
	if(stat == UDC_SUSPEND)
		return surepos_suspend(handle);
	if((stat >= UDC_FULLSPEED) && (stat <= UDC_HIGHSPEED))
		return surepos_set_speed(stat);
	if(stat == UDC_SETUP_PKG_FINISH)
	{
		//printf("handle setup\n");
		return surepos_setup_handle(handle,stat,bufaddr,len);
	}
	if(stat == UDC_PROTAL_RECEIVE_FINISH)
	{
        ret = surepos_handle_receive(handle,bufaddr,len);
		return ret;	
    }
	if(stat == UDC_PROTAL_SEND_FINISH)
	{
		ret = surepos_handle_send(handle,bufaddr,len);
		return ret;
	}
	return 0;
}

static int	surepos_buf_count(void)
{
	return ((g_surepos_WritePoint - g_surepos_ReadPoint + SUREPOS_DATA_BUFFMAX)%SUREPOS_DATA_BUFFMAX);
}


int surepos_open(int type)
{
	if (g_surepos_isOpen == TRUE)
		return 1;	//already open

	if ((type != IBM_SurePOS_TYPE_TableTop) && (type != IBM_SurePOS_TYPE_HandHeld))
		return -2;		

	if(type == IBM_SurePOS_TYPE_TableTop)
	{
	    surepos_devDesc.idProduct = DRIVER_PRODUCT_NUM_IBM_SUREPOS_Tabletop;
        surepos_confDesc.interface_descritor.iInterface = STRING_IBM_SUREPOS_TT; 
        surepos_report_descriptor[5] = 0x4a;
        
	}
    else
    {
        surepos_devDesc.idProduct = DRIVER_PRODUCT_NUM_IBM_SUREPOS_Handheld;
        surepos_confDesc.interface_descritor.iInterface = STRING_IBM_SUREPOS_HH;
        surepos_report_descriptor[5] = 0x4b;
    }

	g_surepos_active = FALSE;	
	g_surepos_WritePoint = 0;
	g_surepos_ReadPoint = 0;	

	surepos_read_mbox = os_MailBoxCreate(0);
	if(surepos_read_mbox == NULL)
		return -1;

    SetEp(surepos_pipe, 3);
	if (CreateDevice(surepos_notify) == 0)
		return -2;

	g_surepos_isOpen = TRUE;
	return 0;
	
}

int surepos_close(void)
{
	unsigned char err;
	
	if (g_surepos_isOpen == FALSE)
		return 1;	//already close

    os_MailBoxDelete(surepos_read_mbox ,1, &err);
    if(err != 0)
    {
        printf("close surepos mailbox err:%d\n",err);
    }
    CloseDevice();
	
	g_surepos_isOpen = FALSE;

	return 0;
}

int surepos_write (unsigned char *buff, unsigned int size)
{
    int Remainlen=0;
	u8 SendBuf[64]={0};
    int len;
    int i;
    int packets ;

    packets = (size/IBM_SurePOS_FRAME_SIZE) + ((size%IBM_SurePOS_FRAME_SIZE)?1:0);
    //printf("surepos write:%d %d %d\n",size, packets, size/MAX_SUREPOS_LEN);
	if (buff == NULL)
		return -1;
	
	if (g_surepos_isOpen == FALSE)
		return -2;
	
	if (!g_surepos_active)
		return -3;

    for(i=0; i<packets; i++)
    {
        memset(SendBuf, 0, 64);
		len =  MIN((size-Remainlen), IBM_SurePOS_FRAME_SIZE);
		memcpy(SendBuf, &buff[Remainlen], len);

		udc_Send(SUREPOS_IN_EP, SendBuf, IBM_SurePOS_FRAME_SIZE);
		Remainlen += len;

    }

	return size;
}

int surepos_read(void* buf, unsigned int count, int timeout)
{

	unsigned int remain = count;
	unsigned int readcnt;
	unsigned int total = 0;
	unsigned int i, j;
	unsigned char err;
	unsigned char *p = (unsigned char*)buf;
	
	if (g_surepos_isOpen == FALSE)
		return 0;
	
	if(buf == NULL)
		return 0;
	
	if (!g_surepos_active)
		return 0;

	if(surepos_buf_count() == 0){
		if(timeout == TIMEOUT_NOWAIT) return 0;
		ResetWatchDog();
		os_MailBoxPend(surepos_read_mbox, (timeout+9)/10, &err);
		if (err == OS_TIMEOUT) return 0;
	}

	while (1)
	{
		i = surepos_buf_count();
		readcnt = (remain > i) ? i : remain;

		if ((g_surepos_ReadPoint+readcnt) > SUREPOS_DATA_BUFFMAX)
		{
			j = SUREPOS_DATA_BUFFMAX-g_surepos_ReadPoint;
			memcpy(p, surepos_RcvBuf+g_surepos_ReadPoint, j);
			memcpy(p+j, surepos_RcvBuf, readcnt-j);
			g_surepos_ReadPoint = (g_surepos_ReadPoint+readcnt)%SUREPOS_DATA_BUFFMAX;
		}
		else
		{
			memcpy(p, surepos_RcvBuf+g_surepos_ReadPoint, readcnt);
			g_surepos_ReadPoint += readcnt;
		}

		p += readcnt;
		remain -= readcnt;
		total += readcnt;

		if((0 == remain) || (timeout == TIMEOUT_NOWAIT))	//read enough
			break;

		os_MailBoxPend(surepos_read_mbox, (timeout+9)/10, &err);
		if (err == OS_TIMEOUT) 	break;

	}

	return total;
}


BOOL surepos_active(void)
{
	return g_surepos_active;
}

