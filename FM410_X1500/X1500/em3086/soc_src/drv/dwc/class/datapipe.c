#include <os_api.h>
#include <udcbus.h>
#include <usb.h>
#include "dwc.h"
#include <sysdefs.h>

#include "generate.h"
#include "watchdog.h"
#include "compat.h"

#include <string.h>

#if 0
#define dprintf printf
#else
#define dprintf(x...)
#endif
#ifndef NULL
#define NULL
#endif

#define UDP_IN_EP	1
#define UDP_OUT_EP	2
#define UDP_IN_EP_OTHER		3
#define UDP_OUT_EP_OTHER	4


#if DWC_FORCE_SPEED_FULL
PIPE udp_pipe[3] = {
	              {0,ENDPOINT_TYPE_CONTROL,64},
				  {(0x80|UDP_IN_EP),ENDPOINT_TYPE_BULK,64},
				  {UDP_OUT_EP,ENDPOINT_TYPE_BULK,64}
               };
#else

PIPE udp_pipe[5] = {
	              {0,ENDPOINT_TYPE_CONTROL,64},
				  {(0x80|UDP_IN_EP),ENDPOINT_TYPE_BULK,512},
				  {UDP_OUT_EP,ENDPOINT_TYPE_BULK,512},
				  {(0x80|UDP_IN_EP_OTHER),ENDPOINT_TYPE_BULK,64},
				  {UDP_OUT_EP_OTHER,ENDPOINT_TYPE_BULK,64}
               };

#endif

#define DATA_BUFFMAX  (1024*32)

static unsigned char RcvBuf[DATA_BUFFMAX];
static int ReadPoint = 0;
static int WritePoint = 0;

static volatile BOOL datapipe_active = FALSE;

static BOOL isOpen = FALSE;
OS_EVENT *udp_read_mbox =  NULL;



static USB_DeviceDescriptor udp_devDesc = 
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
	USB_FUNCTION_DATAPIPE_PRODUCT_ID,
	DRIVER_VERSION_NUM_DATAPIPE,
	STRING_MANUFACTURER, 	//iManufacturer;
	STRING_PRODUCT,   //iProduct;
	STRING_SN,
	0x01
};

#define	CONFIG_DESCRIPTOR_LEN	(sizeof(USB_ConfigDescriptor) + \
				 sizeof(USB_InterfaceDescriptor) + \
				 sizeof(USB_EndPointDescriptor) * 2)

static struct {
	USB_ConfigDescriptor    configuration_descriptor;
	USB_InterfaceDescriptor interface_descritor;
	USB_EndPointDescriptor  endpoint_descriptor[2];
} __attribute__ ((packed)) udp_confDesc = {
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
	{
		sizeof(USB_InterfaceDescriptor),
		INTERFACE_DESCRIPTOR,
		0x00,   /* bInterfaceNumber */
		0x00,   /* bAlternateSetting */
		0x02,	/* ep number */
		0xff,   /* Interface Class */
		0,      /* Interface Subclass*/
		0,      /* Interface Protocol*/
		STRING_DATAPIPE    /* Interface Description String Index*/
	},
	{
		{
			sizeof(USB_EndPointDescriptor),
			ENDPOINT_DESCRIPTOR,
			(1 << 7) | UDP_IN_EP,// endpoint 1 is IN endpoint
			2, /* bulk */
			/* Transfer Type: Bulk;
			 * Synchronization Type: No Synchronization;
			 * Usage Type: Data endpoint
			 */
			#if DWC_FORCE_SPEED_FULL
			64, /* IN EP FIFO size */
			#else
            512,
            #endif
			0
		},
		{
			sizeof(USB_EndPointDescriptor),
			ENDPOINT_DESCRIPTOR,
			(0 << 7) | UDP_OUT_EP,// endpoint 2 is OUT endpoint
			2, /* bulk */
			/* Transfer Type: Bulk;
			 * Synchronization Type: No Synchronization;
			 * Usage Type: Data endpoint
			 */
			#if DWC_FORCE_SPEED_FULL
			64, /* OUT EP FIFO size */
			#else
            512,
            #endif
			0
		}
	}
};


static USB_DeviceQualifierDescriptor udp_devQualifyDesc =
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


static struct {
	USB_ConfigDescriptor    USB_OtherSpeedConfigDescriptor;
	USB_InterfaceDescriptor interface_descritor;
	USB_EndPointDescriptor  endpoint_descriptor[2];
} __attribute__ ((packed)) udp_other_speed_confDesc = {
	{
		sizeof(USB_OtherSpeedConfigDescriptor),
		OTHER_SPEED_CONFIGURATION_DESCRIPTOR,
		CONFIG_DESCRIPTOR_LEN,  /*  Total length of the Configuration descriptor */ 
		0x01,                   /*  NumInterfaces */
		0x01,                   /*  Configuration Value */
		0x00,                   /* Configuration Description String Index */  
		0x80,	// Self Powered, no remote wakeup
		0xfa	// Maximum power consumption 500 mA
	},
	{
		sizeof(USB_InterfaceDescriptor),
		INTERFACE_DESCRIPTOR,
		0x00,   /* bInterfaceNumber */
		0x00,   /* bAlternateSetting */
		0x02,	/* ep number */
		0xff,   /* Interface Class */
		0,      /* Interface Subclass*/
		0,      /* Interface Protocol*/
		STRING_DATAPIPE    /* Interface Description String Index*/
	},
	{
		{
			sizeof(USB_EndPointDescriptor),
			ENDPOINT_DESCRIPTOR,
			(1 << 7) | UDP_IN_EP_OTHER,// endpoint 2 is IN endpoint
			2, /* bulk */
			/* Transfer Type: Bulk;
			 * Synchronization Type: No Synchronization;
			 * Usage Type: Data endpoint
			 */
			64, /* IN EP FIFO size */
			0
		},
		{
			sizeof(USB_EndPointDescriptor),
			ENDPOINT_DESCRIPTOR,
			(0 << 7) | UDP_OUT_EP_OTHER,// endpoint 5 is OUT endpoint
			2, /* bulk */
			/* Transfer Type: Bulk;
			 * Synchronization Type: No Synchronization;
			 * Usage Type: Data endpoint
			 */
			64, /* OUT EP FIFO size */

			0
		}
	}
};




static u32 mass_get_state = 0;

static unsigned int udp_detect(unsigned int handle,unsigned int stat)
{

	return 1;	
}

static unsigned int udp_reset(unsigned int handle)
{

	datapipe_active = FALSE;
	return 1;
	
}


static unsigned int udp_suspend(unsigned int handle)
{
	return 1;
}


static unsigned int udp_set_speed(unsigned stat)
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


static  void udp_get_dev_descriptor(unsigned int handle,USB_DeviceRequest *dreq)
{
    PUDC_BUS pBus = (PUDC_BUS)handle;
    unsigned short size = dreq->wLength;
	
	if(size < sizeof(udp_devDesc))
	{
		udp_devDesc.bLength = size;
		pBus->StartTransfer(handle,udp_pipe[0].ep,(unsigned char *)&udp_devDesc, size);
	}
	
	else
	{
		udp_devDesc.bLength = sizeof(udp_devDesc);
		pBus->StartTransfer(handle,udp_pipe[0].ep,(unsigned char *)&udp_devDesc, sizeof(udp_devDesc));
	}
}

static  void udp_get_dev_configuration(unsigned int handle,USB_DeviceRequest *dreq)
{
	PUDC_BUS pBus = (PUDC_BUS)handle;
	switch (dreq->wLength) {
	case 9:
		pBus->StartTransfer(handle,udp_pipe[0].ep, (unsigned char *)&udp_confDesc, 9);
		break;
	case 8:
		pBus->StartTransfer(handle,udp_pipe[0].ep, (unsigned char *)&udp_confDesc, 8);
		break;
	default:
		pBus->StartTransfer(handle,udp_pipe[0].ep, (unsigned char *)&udp_confDesc, sizeof(udp_confDesc));
		break;
	}
}

static  void udp_get_dev_qualify_descitptor(unsigned int handle,USB_DeviceRequest *dreq)
{
    PUDC_BUS pBus = (PUDC_BUS)handle;
    pBus->StartTransfer(handle,udp_pipe[0].ep, (unsigned char *)&udp_devQualifyDesc, sizeof(udp_devQualifyDesc));	
}


static  void udp_get_dev_other_speed(unsigned int handle,USB_DeviceRequest *dreq)
{
    PUDC_BUS pBus = (PUDC_BUS)handle;
	int len;
	len = MIN(sizeof(udp_other_speed_confDesc), dreq->wLength);

    pBus->StartTransfer(handle,udp_pipe[0].ep, (unsigned char *)&udp_other_speed_confDesc, len);	
}


static inline void udp_get_dev_descriptor_string(unsigned int handle,USB_DeviceRequest *dreq)
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
	
	pBus->StartTransfer(handle,udp_pipe[0].ep, buf, len);

}


/*  */
unsigned int udp_setup_handle(unsigned int handle,unsigned int stat,unsigned char *bufaddr,unsigned int len)
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
				udp_get_dev_descriptor(handle,dreq);
				break;
			case CONFIGURATION_DESCRIPTOR:
				//printf("\n\nGet Config!\n");
				udp_get_dev_configuration(handle,dreq);
				break;
			case STRING_DESCRIPTOR:
				//printf("\n\nGet string!\n");
				udp_get_dev_descriptor_string(handle,dreq);
				break;
			case DEVICE_QUALIFIER_DESCRIPTOR:
				//printf("\n\nGet qualify!\n");
				udp_get_dev_qualify_descitptor(handle,dreq);
				break;
            case OTHER_SPEED_CONFIGURATION_DESCRIPTOR:
                //printf("\n\nGet other speed!\n");
                udp_get_dev_other_speed(handle,dreq);
			}
		break;
        
    	case SET_ADDRESS:
    		//printf("\n\nSet Address");
    		pBus->SetAddress(handle,dreq->wValue);
    		break;
    		
    	case GET_STATUS:
    		switch (dreq->bmRequestType) {
    		case 80:	/* device */

    			pBus->StartTransfer(handle,udp_pipe[0].ep,(u8*)&mass_get_state,2);			
    			break;
    		case 81:	/* interface */
    		case 82:	/* ep */
    			pBus->StartTransfer(handle,udp_pipe[0].ep,(u8*)&mass_get_state,2);			
    			break;
    		}

    		break;
       
    	case CLEAR_FEATURE:
    		//printf("CLEAR_FEATURE!\r\n");
    		break;
    		
    	case SET_CONFIGURATION:

    		pBus->SetConfig(handle);
			datapipe_active= TRUE;
    		break;
    		
    	case SET_INTERFACE:
    		//printf("SET_INTERFACE!\r\n");
    		break;
    	case SET_FEATURE:
    		//printf("SET_FEATURE!\r\n");
    		break;
    	default:	
            //printf("protal isn't surporst\r\n");
    		break;
    	}
	}
	return 0;
}


static unsigned int udp_handle_receive(unsigned handle,unsigned char *buf,unsigned int len)
{
	unsigned int count;
	unsigned char *pbuf;
	unsigned char err;
	
// 	printf("receive: %d, w: %d\n", len, WritePoint);
// 	printf("r: \t%s\n", buf);

	count = WritePoint + len;
// 	pbuf = HW_GetPKTBuf(UDP_OUT_EP, len);
	pbuf = buf;

	if (count>=DATA_BUFFMAX)
	{
		count = count % DATA_BUFFMAX;
		memcpy((RcvBuf+WritePoint), pbuf, DATA_BUFFMAX-WritePoint);	
		memcpy(RcvBuf, pbuf+DATA_BUFFMAX-WritePoint, count);	
	}
	else
		memcpy(RcvBuf+WritePoint, pbuf, len);	
	
	WritePoint = count%DATA_BUFFMAX;
	
	err = os_MailBoxPost(udp_read_mbox, NULL);
	if (err)
		printf("udp receive post error: %d", err);
		
	return 0;
}



static unsigned int udp_handle_send(unsigned handle,unsigned char *buf,unsigned int len)
{

	//printf("send: %d\n", len);
	return 0;
}


static unsigned int udp_notify(unsigned int handle,unsigned int stat,unsigned char *bufaddr,unsigned int len)
{
	unsigned int ret = 0;
	
	//printf("Bus notify stat :%x \n",stat);
	
	if((stat < 0x200) && (stat >=0x100))
	   return udp_detect(handle,stat);
	if(stat == UDC_RESET)
		return udp_reset(handle);
	if(stat == UDC_SUSPEND)
		return udp_suspend(handle);
	if((stat >= UDC_FULLSPEED) && (stat <= UDC_HIGHSPEED))
		return udp_set_speed(stat);
	if(stat == UDC_SETUP_PKG_FINISH)
	{
		//printf("handle setup\n");
		return udp_setup_handle(handle,stat,bufaddr,len);
	}
	if(stat == UDC_PROTAL_RECEIVE_FINISH)
	{
		ret = udp_handle_receive(handle,bufaddr,len);
		return ret;
	}
	if(stat == UDC_PROTAL_SEND_FINISH)
	{
		ret = udp_handle_send(handle,bufaddr,len);
		return ret;
	}
	return 0;
}


static int	BufCount(void)
{
	return ((WritePoint - ReadPoint + DATA_BUFFMAX)%DATA_BUFFMAX);
}


int udp_open(int devmode)
{
	if (isOpen == TRUE)
		return 1;	//already open
	
	if(devmode == UDP_OPEN_MODE_NOR) //datapipe
		udp_devDesc.idProduct = USB_FUNCTION_DATAPIPE_PRODUCT_ID;
	else	//datapipe to serial
		udp_devDesc.idProduct = USB_FUNCTION_SER_PRODUCT_ID;

	datapipe_active = FALSE;	
	WritePoint = 0;
	ReadPoint = 0;	

	udp_read_mbox = os_MailBoxCreate(0);
	if(udp_read_mbox == NULL)
		return -1;
    SetEp(udp_pipe, 3);
	if (CreateDevice(udp_notify) == 0)
		return -2;

	isOpen = TRUE;
	return 0;
	
}

int udp_close(void)
{

    unsigned char err;
    
	if (isOpen == FALSE)
		return 1;	//already close

    os_MailBoxDelete(udp_read_mbox ,1, &err);
    if(err != 0)
    {
        printf("close datapipe mailbox err:%d\n",err);
    }


    CloseDevice();
	
	isOpen = FALSE;

	return 0;
}



int udp_write (unsigned char * buff, unsigned int size)
{
	if (buff == NULL)
		return -1;
	
	if (isOpen == FALSE)
		return -2;
	
	
	if (!datapipe_active)
		return -3;
	
	
	udc_Send(UDP_IN_EP, buff, size);

	return size;
}


int udp_read(void* buf, unsigned int count, int timeout)
{

	unsigned int remain = count;
	unsigned int readcnt;
	unsigned int total = 0;
	unsigned int i, j;
	unsigned char err;
	unsigned char *p = (unsigned char*)buf;
	
	if (isOpen == FALSE)
		return 0;
	
	if(buf == NULL)
		return 0;
	
	if (!datapipe_active)
		return 0;

	if(BufCount() == 0){
		if(timeout == TIMEOUT_NOWAIT) return 0;
		ResetWatchDog();
		os_MailBoxPend(udp_read_mbox, (timeout+9)/10, &err);
		if (err == OS_TIMEOUT) return 0;
	}

	while (1)
	{
		i = BufCount();
		readcnt = (remain > i) ? i : remain;

		if ((ReadPoint+readcnt)>DATA_BUFFMAX)
		{
			j = DATA_BUFFMAX-ReadPoint;
			memcpy(p, RcvBuf+ReadPoint, j);
			memcpy(p+j, RcvBuf, readcnt-j);
			ReadPoint = (ReadPoint+readcnt)%DATA_BUFFMAX;
		}
		else
		{
			memcpy(p, RcvBuf+ReadPoint, readcnt);
			ReadPoint += readcnt;
		}

		p += readcnt;
		remain -= readcnt;
		total += readcnt;

		if((0 == remain) || (timeout == TIMEOUT_NOWAIT))	//read enough
			break;

		os_MailBoxPend(udp_read_mbox, (timeout+9)/10, &err);
		if (err == OS_TIMEOUT) 	break;

	}

	return total;
}

BOOL udp_active(void)
{
	return datapipe_active;
}

