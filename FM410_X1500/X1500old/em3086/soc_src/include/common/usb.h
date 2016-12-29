/********************** BEGIN LICENSE BLOCK ************************************
 *
 * JZ4740  mobile_tv  Project  V1.0.0
 * INGENIC CONFIDENTIAL--NOT FOR DISTRIBUTION IN SOURCE CODE FORM
 * Copyright (c) Ingenic Semiconductor Co. Ltd 2005. All rights reserved.
 * 
 * This file, and the files included with this file, is distributed and made 
 * available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER 
 * EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS ALL SUCH WARRANTIES, 
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS 
 * FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. 
 * 
 * http://www.ingenic.cn 
 *
 ********************** END LICENSE BLOCK **************************************
 *
 *  Author:  <dsqiu@ingenic.cn>  <jgao@ingenic.cn> 
 *
 *  Create:   2008-06-26, by dsqiu
 *            
 *  Maintain: 2008-06-26, by jgao
 *            
 *
 *******************************************************************************
 */
 
 

#ifndef __USB_H
#define	__USB_H

#ifndef u8
#define u8	unsigned char
#endif

#ifndef u16
#define u16	unsigned short
#endif

#ifndef u32
#define u32	unsigned int
#endif

#ifndef s8
#define s8	char
#endif

#ifndef s16
#define s16	short
#endif

#ifndef s32
#define s32	int
#endif

extern int usbdebug;
/*
 * Standard requests
 */
#define USB_REQ_GET_STATUS		0x00
#define USB_REQ_CLEAR_FEATURE		0x01
/* 0x02 is reserved */
#define USB_REQ_SET_FEATURE		0x03
/* 0x04 is reserved */
#define USB_REQ_SET_ADDRESS		0x05
#define USB_REQ_GET_DESCRIPTOR		0x06
#define USB_REQ_SET_DESCRIPTOR		0x07
#define USB_REQ_GET_CONFIGURATION	0x08
#define USB_REQ_SET_CONFIGURATION	0x09
#define USB_REQ_GET_INTERFACE		0x0A
#define USB_REQ_SET_INTERFACE		0x0B
#define USB_REQ_SYNCH_FRAME		0x0C

#define USB_TYPE_STANDARD		(0x00 << 5)
#define USB_TYPE_CLASS			(0x01 << 5)
#define USB_TYPE_VENDOR			(0x02 << 5)
#define USB_TYPE_RESERVED		(0x03 << 5)

#define USB_RECIP_DEVICE		0x00
#define USB_RECIP_INTERFACE		0x01
#define USB_RECIP_ENDPOINT		0x02
#define USB_RECIP_OTHER			0x03


/* Descriptor types ... USB 2.0 spec table 9.5 */
#define USB_DT_DEVICE			0x01
#define USB_DT_CONFIG			0x02
#define USB_DT_STRING			0x03
#define USB_DT_INTERFACE		0x04
#define USB_DT_ENDPOINT			0x05
#define USB_DT_DEVICE_QUALIFIER		0x06
#define USB_DT_OTHER_SPEED_CONFIG	0x07
#define USB_DT_INTERFACE_POWER		0x08
/* these are from a minor usb 2.0 revision (ECN) */
#define USB_DT_OTG			0x09
#define USB_DT_DEBUG			0x0a
#define USB_DT_INTERFACE_ASSOCIATION	0x0b


#define USB_DT_DEVICE_SIZE		18
#define USB_DT_CONFIG_SIZE		9


/* identification values and masks to identify request types  */
#define USB_REQUEST_CLASS_MASK              (0x60)
#define USB_REQUEST_CLASS_STRD              (0x00)
#define USB_REQUEST_CLASS_CLASS             (0x20)
#define USB_REQUEST_CLASS_VENDOR            (0x40)


#define USB_FUNCTION_VENDOR_ID					(0x1eab)        /* Vendor  ID */
#define USB_FUNCTION_DATAPIPE_PRODUCT_ID		(0x0001)        /* DATAPIPE Product ID */
#define USB_FUNCTION_SER_PRODUCT_ID				(0x0002)        /* BulkToSer over datapipe Product ID */
#define USB_FUNCTION_CDC_PRODUCT_ID				(0x0c06)        /* CDC SERIAL Product ID */
#define USB_FUNCTION_HID_KEYBOARD_PRODUCT_ID	(0x0003)        /* HID KBW Product ID */
#define USB_FUNCTION_HID_POS_PRODUCT_ID			(0x0010)        /* HID POS Product ID */
#define DRIVER_PRODUCT_NUM_IBM_SUREPOS_Tabletop	(0x0020)		/* USB HID IBM SurePOS Tabletop */
#define DRIVER_PRODUCT_NUM_IBM_SUREPOS_Handheld	(0x0021)		/* USB HID IBM SurePOS Handheld */


#define STRING_MANUFACTURER			1
#define STRING_PRODUCT				2
#define STRING_SN_DATAPIPE			3
#define STRING_SN					4
#define STRING_HID_POS				5
#define STRING_DATAPIPE				6
#define STRING_HID_KBD				7
#define STRING_CDC_ACM_COMM			8
#define STRING_CDC_ACM_DATA			9
#define STRING_IBM_SUREPOS_TT		10
#define STRING_IBM_SUREPOS_HH		11
#define STRING_IBM_SUREPOS_FLASH	12


//////////////////////////////////////////////////////////////////////////
//	USB Driver version define
#define DRIVER_VERSION_NUM_DATAPIPE		0x0103
#define DRIVER_VERSION_NUM_HID_KBW		0x0103
#define DRIVER_VERSION_NUM_HID_POS		0x0101
#define DRIVER_VERSION_NUM_IBM_SUREPOS	0x0000
#define DRIVER_VERSION_NUM_CDC			0x0001


#define UDP_OPEN_MODE_SER			1
#define UDP_OPEN_MODE_NOR			0


#define USB_MODE_UDP        0
#define USB_MODE_CDC        1
#define USB_MODE_KBW        2
#define USB_MODE_POS        2
/**
 * struct usb_string - wraps a C string and its USB id
 * @id:the (nonzero) ID for this string
 * @s:the string, in UTF-8 encoding
 *
 * If you're using usb_gadget_get_string(), use this to wrap a string
 * together with its ID.
 */
struct usb_string {
	unsigned char	id;
	const char		*s;
};

/**
 * struct usb_gadget_strings - a set of USB strings in a given language
 * @language:identifies the strings' language (0x0409 for en-us)
 * @strings:array of strings with their ids
 *
 * If you're using usb_gadget_get_string(), use this to wrap all the
 * strings for a given language.
 */
struct usb_gadget_strings {
	unsigned short		language;	/* 0x0409 for en-us */
	struct usb_string	*strings;
};


typedef enum 
{
	ENDPOINT_TYPE_CONTROL,
	/* Typically used to configure a device when attached to the host.
	 * It may also be used for other device specific purposes, including
	 * control of other pipes on the device.
	 */
	ENDPOINT_TYPE_ISOCHRONOUS,
	/* Typically used for applications which need guaranteed speed.
	 * Isochronous transfer is fast but with possible data loss. A typical
	 * use is audio data which requires a constant data rate.
	 */
	ENDPOINT_TYPE_BULK,
	/* Typically used by devices that generate or consume data in relatively
	 * large and bursty quantities. Bulk transfer has wide dynamic latitude
	 * in transmission constraints. It can use all remaining available bandwidth,
	 * but with no guarantees on bandwidth or latency. Since the USB bus is
	 * normally not very busy, there is typically 90% or more of the bandwidth
	 * available for USB transfers.
	 */
	ENDPOINT_TYPE_INTERRUPT
	/* Typically used by devices that need guaranteed quick responses
	 * (bounded latency).
	 */
}USB_ENDPOINT_TYPE;


enum USB_STANDARD_REQUEST_CODE {
	GET_STATUS,
	CLEAR_FEATURE,
	SET_FEATURE = 3,
	SET_ADDRESS = 5,
	GET_DESCRIPTOR,
	SET_DESCRIPTOR,
	GET_CONFIGURATION,
	SET_CONFIGURATION,
	GET_INTERFACE,
	SET_INTERFACE,
	SYNCH_FRAME
};


enum USB_DESCRIPTOR_TYPE {
	DEVICE_DESCRIPTOR = 1,
	CONFIGURATION_DESCRIPTOR,
	STRING_DESCRIPTOR,
	INTERFACE_DESCRIPTOR,
	ENDPOINT_DESCRIPTOR,
	DEVICE_QUALIFIER_DESCRIPTOR,
	OTHER_SPEED_CONFIGURATION_DESCRIPTOR,
	INTERFACE_POWER1_DESCRIPTOR,
	USB_HID_DESCRIPTOR = 0x21,
	USB_REPORT_DESCRIPTOR = 0x22,
	USB_CS_INTERFACE =0x24,
    USB_CS_ENDPOINT=0x25
};


/* Descriptor SubType in Communications Class Functional Descriptors */
enum USB_COMM_CLASS_SUBTYPE{
    HEADER_FUNC_DESC             =  (0x00),
    CALL_MANAGEMENT_FUNC_DESC   =  (0x01),
    ABSTRACT_CONTROL_FUNC_DESC  =  (0x02),
    DIRECT_LINE_FUNC_DESC        =  (0x03),
    TELEPHONE_RINGER_FUNC_DESC  =  (0x04),
    TELEPHONE_REPORT_FUNC_DESC  =  (0x05),
    UNION_FUNC_DESC              =  (0x06),
    COUNTRY_SELECT_FUNC_DESC    =  (0x07),
    TELEPHONE_MODES_FUNC_DESC   =  (0x08),
    USB_TERMINAL_FUNC_DESC      =  (0x09),
    NETWORK_CHANNEL_FUNC_DESC   =  (0x0A),
    PROTOCOL_UNIT_FUNC_DESC     =  (0x0B),
    EXTENSION_UNIT_FUNC_DESC    =  (0x0C),
    MULTI_CHANNEL_FUNC_DESC     =  (0x0D),
    CAPI_CONTROL_FUNC_DESC      =  (0x0E),
    ETHERNET_NETWORKING_FUNC_DESC= (0x0F),
    ATM_NETWORKING_FUNC_DESC     = (0x10),
    WIRELESS_CONTROL_FUNC_DESC   = (0x11),
    MOBILE_DIRECT_LINE_FUNC_DESC = (0x12),
    MDLM_DETAIL_FUNC_DESC        = (0x13),
    DEVICE_MANAGEMENT_FUNC_DESC  = (0x14),
    OBEX_FUNC_DESC               = (0x15),
    COMMAND_SET_FUNC_DESC        = (0x16),
    COMMAND_SET_DETAIL_FUNC_DESC = (0x17),
    TELEPHONE_CONTROL_FUNC_DESC  = (0x18),
    OBEX_SERVICE_ID_FUNC_DESC    = (0x19)
};


/* Implementation Specific Macros */
#define LINE_CODING_SIZE              (0x07)
#define COMM_FEATURE_DATA_SIZE        (0x02)

#define LINE_CODE_DTERATE_IFACE0      (115200) /*e.g 9600 is 0x00002580 */
#define LINE_CODE_CHARFORMAT_IFACE0   (0x00)   /* 1 stop bit */
#define LINE_CODE_PARITYTYPE_IFACE0   (0x00)   /* No Parity */
#define LINE_CODE_DATABITS_IFACE0     (0x08)   /* Data Bits Format */

#define LINE_CODE_DTERATE_IFACE1      (9600)   /*e.g. 115200 is 0x0001C200*/
#define LINE_CODE_CHARFORMAT_IFACE1   (0x00)   /* 1 stop bit */
#define LINE_CODE_PARITYTYPE_IFACE1   (0x00)   /* No Parity */
#define LINE_CODE_DATABITS_IFACE1     (0x08)   /* Data Bits Format */

#define STATUS_ABSTRACT_STATE_IFACE0  (0x0000) /* Disable Multiplexing
                                                  ENDP in this interface will
                                                 continue to accept/offer data*/
#define STATUS_ABSTRACT_STATE_IFACE1  (0x0000) /* Disable Multiplexing
                                                  ENDP in this interface will
                                                 continue to accept/offer data*/
#define COUNTRY_SETTING_IFACE0        (0x0000) /* Country Code in the format as
                                                  defined in [ISO3166] */
#define COUNTRY_SETTING_IFACE1        (0x0000) /* Country Code in the format as
                                                  defined in [ISO3166] */

/* Class specific request Codes */
#define SEND_ENCAPSULATED_COMMAND       (0x00)
#define GET_ENCAPSULATED_RESPONSE       (0x01)
#define SET_COMM_FEATURE                (0x02)
#define GET_COMM_FEATURE                (0x03)
#define CLEAR_COMM_FEATURE              (0x04)
#define SET_AUX_LINE_STATE              (0x10)
#define SET_HOOK_STATE                  (0x11)
#define PULSE_SETUP                     (0x12)
#define SEND_PULSE                      (0x13)
#define SET_PULSE_TIME                  (0x14)
#define RING_AUX_JACK                   (0x15)
#define SET_LINE_CODING                 (0x20)
#define GET_LINE_CODING                 (0x21)
#define SET_CONTROL_LINE_STATE          (0x22)
#define SEND_BREAK                      (0x23)
#define SET_RINGER_PARAMS               (0x30)
#define GET_RINGER_PARAMS               (0x31)
#define SET_OPERATION_PARAM             (0x32)
#define GET_OPERATION_PARAM             (0x33)
#define SET_LINE_PARAMS                 (0x34)
#define GET_LINE_PARAMS                 (0x35)
#define DIAL_DIGITS                     (0x36)
#define SET_UNIT_PARAMETER              (0x37)
#define GET_UNIT_PARAMETER              (0x38)
#define CLEAR_UNIT_PARAMETER            (0x39)
#define GET_PROFILE                     (0x3A)
#define SET_ETHERNET_MULTICAST_FILTERS  (0x40)
#define SET_ETHERNET_POW_PATTER_FILTER  (0x41)
#define GET_ETHERNET_POW_PATTER_FILTER  (0x42)
#define SET_ETHERNET_PACKET_FILTER      (0x43)
#define GET_ETHERNET_STATISTIC          (0x44)
#define SET_ATM_DATA_FORMAT             (0x50)
#define GET_ATM_DEVICE_STATISTICS       (0x51)
#define SET_ATM_DEFAULT_VC              (0x52)
#define GET_ATM_VC_STATISTICS           (0x53)
#define MDLM_SPECIFIC_REQUESTS_MASK     (0x7F)

#define UART_BITMAP_SIZE           (0x02)
#define ABSTRACT_STATE_FEATURE     (0x01)
#define COUNTRY_SETTING_FEATURE    (0x02)
#define CARRIER_ACTIVATION_CHECK   (0x02)
#define DTE_PRESENCE_CHECK         (0x01)

/* other macros */
#define NOTIF_PACKET_SIZE             (0x08)
#define NOTIF_REQUEST_TYPE            (0xA1)

/* Class Specific Notification Codes */
#define NETWORK_CONNECTION_NOTIF        (0x00)
#define RESPONSE_AVAIL_NOTIF            (0x01)
#define AUX_JACK_HOOK_STATE_NOTIF       (0x08)
#define RING_DETECT_NOTIF               (0x09)
#define SERIAL_STATE_NOTIF              (0x20)
#define CALL_STATE_CHANGE_NOTIF         (0x28)
#define LINE_STATE_CHANGE_NOTIF         (0x29)
#define CONNECTION_SPEED_CHANGE_NOTIF   (0x2A)
#define MDLM_SPECIFIC_NOTIF_MASK        (0x5F)


enum USB_FEATURE_SELECTOR {
	ENDPOINT_HALT,
	DEVICE_REMOTE_WAKEUP,
	TEST_MODE
};


enum USB_CLASS_CODE {
	CLASS_DEVICE,
	CLASS_AUDIO,
	CLASS_COMM_AND_CDC_CONTROL,
	CLASS_HID,
	CLASS_PHYSICAL = 0x05,
	CLASS_STILL_IMAGING,
	CLASS_PRINTER,
	CLASS_MASS_STORAGE,
	CLASS_HUB,
	CLASS_CDC_DATA,
	CLASS_SMART_CARD,
	CLASS_CONTENT_SECURITY = 0x0d,
	CLASS_VIDEO,
	CLASS_DIAGNOSTIC_DEVICE = 0xdc,
	CLASS_WIRELESS_CONTROLLER = 0xe0,
	CLASS_MISCELLANEOUS = 0xef,
	CLASS_APP_SPECIFIC = 0xfe,
	CLASS_VENDOR_SPECIFIC = 0xff
};


 /* class specific requests */
#define USB_HID_GET_REPORT_REQUEST      (0x01)
#define USB_HID_GET_IDLE_REQUEST        (0x02)
#define USB_HID_GET_PROTOCOL_REQUEST    (0x03)
#define USB_HID_SET_REPORT_REQUEST      (0x09)
#define USB_HID_SET_IDLE_REQUEST        (0x0A)
#define USB_HID_SET_PROTOCOL_REQUEST    (0x0B)

/* for class specific requests */
#define HIGH_BYTE_SHIFT                 (8)
#define MSB_MASK                        (0xFF00)
#define USB_HID_REQUEST_DIR_MASK        (0x08)
#define USB_HID_REQUEST_TYPE_MASK       (0x01)
#define REPORT_SIZE                     (4)
#define CLASS_REQ_DATA_SIZE             (0x01)


typedef struct _BITMAP_UART
{
    u8 bRxCarrier   : 1;    /* Receive Carrier Activation Flag */
    u8 bTxCarrier   : 1;    /* Transmit Carrier Activation Flag */
    u8 bBreak       : 1;    /* Break Flag */
    u8 bRingSignal  : 1;    /* Ring Signal Flag */
    u8 bFraming     : 1;    /* Frame Flag */
    u8 bParity      : 1;    /* Parity Flag */
    u8 bOverRun     : 1;    /* OverRun Flag */
    u8 reserved1    : 1;    /* Reserved */
}BITMAP_UART;


typedef union _UART_BITMAP
{
    u8 _byte;
    BITMAP_UART Bitmap_Uart;
}UART_BITMAP;   /* UART STATE BITMAP */


typedef struct {
	u8 bmRequestType;
	u8 bRequest;
	u16 wValue;
	u16 wIndex;
	u16 wLength;
} __attribute__ ((packed)) USB_DeviceRequest;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u16 bcdUSB;
	u8 bDeviceClass;
	u8 bDeviceSubClass;
	u8 bDeviceProtocol;
	u8 bMaxPacketSize0;
	u16 idVendor;
	u16 idProduct;
	u16 bcdDevice;
	u8 iManufacturer;
	u8 iProduct;
	u8 iSerialNumber;
	u8 bNumConfigurations;
} __attribute__ ((packed)) USB_DeviceDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u16 bcdUSB;
	u8 bDeviceClass;
	u8 bDeviceSubClass;
	u8 bDeviceProtocol;
	u8 bMaxPacketSize0;
	u8 bNumConfigurations;
	u8 bReserved;
} __attribute__ ((packed)) USB_DeviceQualifierDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u16 wTotalLength;
	u8 bNumInterfaces;
	u8 bConfigurationValue;
	u8 iConfiguration;
	u8 bmAttributes;
	u8 MaxPower;
} __attribute__ ((packed)) USB_ConfigDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u16 wTotalLength;
	u8 bNumInterfaces;
	u8 bConfigurationValue;
	u8 iConfiguration;
	u8 bmAttributes;
	u8 bMaxPower;
} __attribute__ ((packed)) USB_OtherSpeedConfigDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u8 bInterfaceNumber;
	u8 bAlternateSetting;
	u8 bNumEndpoints;
	u8 bInterfaceClass;
	u8 bInterfaceSubClass;
	u8 bInterfaceProtocol;
	u8 iInterface;
} __attribute__ ((packed)) USB_InterfaceDescriptor;


typedef struct {
	u8 bLegth;
	u8 bDescriptorType;
	u8 bEndpointAddress;
	u8 bmAttributes;
	u16 wMaxPacketSize;
	u8 bInterval;
} __attribute__ ((packed)) USB_EndPointDescriptor;


typedef struct {
	u8 bLength;
	u8 bDescriptorType;
	u16 SomeDesriptor[1];
} __attribute__ ((packed)) USB_StringDescriptor;


/* header Functioal Descripter */
typedef struct {
	u8 bLegth;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u16 bcdCDC;
} __attribute__ ((packed)) head_functional_t;

/* Call Management Functional */
typedef struct{
	u8 bLegth;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u8 bmCapabilities;
	u8 bDataInterface;
} __attribute__ ((packed)) call_management_functional_t;

/*Abstract Control Management Functional */
typedef struct{
	u8 bLegth;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u8 bmCapabilities;
} __attribute__ ((packed)) abstract_control_management_functional_t;

/*Union Functional */
typedef struct{
	u8 bLegth;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u8 MasterInterface;
	u8 SlaveInterface;
} __attribute__ ((packed)) union_functional_t;



/* HID KBW ÃèÊö·û */
typedef struct{
	u8 bLegth;
	u8 bDescriptorType;
	u16 bcdHID;
	u8 bCountyCode;
	u8 bNumReport;
	u8 bReportType;
	u16 bReportLength;
} __attribute__ ((packed))hid_kbw_t;

int cdc_open(void);
int cdc_close(void);
int cdc_write (unsigned char * buff, unsigned int size);
int cdc_read(void* buf, unsigned int count, int timeout);
int cdc_active(void);

int pos_open(void);
int pos_close(void);
int pos_write (unsigned char * buff, unsigned int size);
int pos_read(void* buf, unsigned int count, int timeout);
int pos_active(void);

int udp_open(int type);
int udp_close(void);
int udp_write (unsigned char * buff, unsigned int size);
int udp_read(void* buf, unsigned int count, int timeout);
int udp_active(void);

int kbw_open(void);
int kbw_close(void);
int kbw_write (unsigned char * buff, unsigned int size,int nCharEncoding);
int kbw_active(void);

int surepos_open(int type);
int surepos_close(void);
int surepos_write (unsigned char * buff, unsigned int size);
int surepos_read(void* buf, unsigned int count, int timeout);
int surepos_active(void);

#endif // !defined(__USB_H)

