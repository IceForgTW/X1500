
#include "usb.h"
#include <udcbus.h>
#include "dwc.h"
#include <sysdefs.h>


#undef printf
#include <string.h>
#include <stdio.h>
#ifdef USE_MIDWARE

unsigned char BAT_GetDCDetect(void)
{
}

//----------------------------------------------------------------------------

unsigned int MidCheckBatState(void)
{

}
#endif
//----------------------------------------------------------------------------
int UdcGetDcdetecPin(){
	#ifdef DC_DETE_PIN
	return 1;
	#else
	return 0;
	#endif
}
int udc_detect_status(void)
{
	return 0;
}

int udc_detect_cable_status(void)
{
	return 0;
}


void udc_disable_device_directly_global(void)
{
	;
}


void enable_device(unsigned int handle)
{
	;
}

void disable_device(unsigned int handle)
{
	;
}


void init_endpoint_suppost(unsigned int handle,unsigned char *ep, USB_ENDPOINT_TYPE ep_type,
		unsigned short *ep_max_pkg, unsigned char *buf)
{
	dwc_ep * pep=NULL;
	if(ep_type == ENDPOINT_TYPE_CONTROL)
	{
		*ep = 0;
		*ep_max_pkg = 64;
	}

	if(ep_type == ENDPOINT_TYPE_BULK)
	{
		if(*ep & 0x80)
		{
			pep = (dwc_ep *)GetEp(1); //in ep
			pep->xfer_buff = (unsigned char *)buf;
		}
		else
		{
			pep = (dwc_ep *)GetEp(2); //out ep
			pep->xfer_buff = (unsigned char *)buf;
		}
		*ep = pep->num;
		*ep_max_pkg = pep->maxpacket;
	}
	printf("ep = %x ep_type = %x epmax = %x\r\n",*ep, ep_type, pep->maxpacket);
}

void set_address(unsigned int handle,unsigned short value)
{
	dwc_set_address(value);
}

void set_config(unsigned int handle)
{
	dwc_configure_epn(0);
}
extern void HW_GetPKT(unsigned int epnum,  const unsigned char *buf, int size);
extern void HW_SendPKT(unsigned int epnum,  const u8 *buf, int size);

void start_transfer(unsigned int handle,unsigned char ep,unsigned char *buf,unsigned int len)
{
	switch(ep) {
	case 0:
//		printf("%s %d\n", __FUNCTION__, __LINE__);
		HW_SendPKT(0, buf, len);
		break;
	case 0x81:
		HW_SendPKT(1, buf, len);
    	break;
	case 0x2:
		HW_GetPKT(2, buf, len);
    	break;
    case 0x83:
        HW_SendPKT(3, buf, len);
        break;
	}
}




/*****************************************************************************
 * Local Variables 
 *****************************************************************************/

static const char longname [] = "EM3086";

#define DeviceSerialMaxNum		40
char DeviceSerial[DeviceSerialMaxNum] = "11111111111111";
unsigned short DeviceVerdorID = USB_FUNCTION_VENDOR_ID;
#define ManufacturerStringMaxNum		20
char ManufacturerString[ManufacturerStringMaxNum] = "Newland Auto-ID";

/*****************************************************************************
 * Global Functions
 *****************************************************************************/


/* static strings, in iso 8859/1 */
static struct usb_string		strings [] = {
	{ STRING_MANUFACTURER, ManufacturerString, },
	{ STRING_PRODUCT, longname, },
	{ STRING_SN_DATAPIPE, "11111111111111", },/*DeviceSerial*/
	{ STRING_SN, DeviceSerial, },/*DeviceSerial*/
	{ STRING_HID_POS, "USB HID POS", },
	{ STRING_DATAPIPE, "USB Datapipe", },
	{ STRING_HID_KBD, "USB HID Keyboard", },
	{ STRING_IBM_SUREPOS_TT, "IBM SurePos Table-top", },
	{ STRING_IBM_SUREPOS_HH, "IBM SurePos Hand-held", },
	{ STRING_CDC_ACM_COMM, "CDC ACM Comm", },
	{ STRING_CDC_ACM_DATA, "CDC ACM Data", },
	{ (unsigned char)0, (const char*)0 }			/* end of list */
};

struct usb_gadget_strings	usb_stringtab = {
	0x0409,	/* en-us */
	strings,
};


static int utf8_to_utf16le(const char *s, unsigned short *cp, unsigned len)
{
	int	count = 0;
	unsigned char	c;
	unsigned short	uchar;

	/* this insists on correct encodings, though not minimal ones.
	 * BUT it currently rejects legit 4-byte UTF-8 code points,
	 * which need surrogate pairs.  (Unicode 3.1 can use them.)
	 */
	while (len != 0 && (c = (unsigned char) *s++) != 0) {
		if (c & 0x80) {
			// 2-byte sequence:
			// 00000yyyyyxxxxxx = 110yyyyy 10xxxxxx
			if ((c & 0xe0) == 0xc0) {
				uchar = (c & 0x1f) << 6;

				c = (unsigned char) *s++;
				if ((c & 0xc0) != 0xc0)
					goto fail;
				c &= 0x3f;
				uchar |= c;

			// 3-byte sequence (most CJKV characters):
			// zzzzyyyyyyxxxxxx = 1110zzzz 10yyyyyy 10xxxxxx
			} else if ((c & 0xf0) == 0xe0) {
				uchar = (c & 0x0f) << 12;

				c = (unsigned char) *s++;
				if ((c & 0xc0) != 0xc0)
					goto fail;
				c &= 0x3f;
				uchar |= c << 6;

				c = (unsigned char) *s++;
				if ((c & 0xc0) != 0xc0)
					goto fail;
				c &= 0x3f;
				uchar |= c;

				/* no bogus surrogates */
				if (0xd800 <= uchar && uchar <= 0xdfff)
					goto fail;

			// 4-byte sequence (surrogate pairs, currently rare):
			// 11101110wwwwzzzzyy + 110111yyyyxxxxxx
			//     = 11110uuu 10uuzzzz 10yyyyyy 10xxxxxx
			// (uuuuu = wwww + 1)
			// FIXME accept the surrogate code points (only)

			} else
				goto fail;
		} else
			uchar = c;
		//put_unaligned (cpu_to_le16 (uchar), cp++);
		memcpy(cp, &uchar,2);
		cp++;

		count++;
		len--;
	}
	return count;
fail:
	return -1;
}

/**
 * usb_gadget_get_string - fill out a string descriptor 
 * @table: of c strings encoded using UTF-8
 * @id: string id, from low byte of wValue in get string descriptor
 * @buf: at least 256 bytes
 *
 * Finds the UTF-8 string matching the ID, and converts it into a
 * string descriptor in utf16-le.
 * Returns length of descriptor (always even) or negative errno
 *
 * If your driver needs stings in multiple languages, you'll probably
 * "switch (wIndex) { ... }"  in your ep0 string descriptor logic,
 * using this routine after choosing which set of UTF-8 strings to use.
 * Note that US-ASCII is a strict subset of UTF-8; any string bytes with
 * the eighth bit set will be multibyte UTF-8 characters, not ISO-8859/1
 * characters (which are also widely used in C strings).
 */
int
usb_gadget_get_string (int id, unsigned char *buf)
{
	struct usb_string	*s;
	int			len;

	/* descriptor 0 has the language id */
	if (id == 0) {
		buf [0] = 4;
		buf [1] = 0x3;
		buf [2] = (unsigned char) usb_stringtab.language;
		buf [3] = (unsigned char) (usb_stringtab.language >> 8);
		return 4;
	}
	for (s = usb_stringtab.strings; s && s->s; s++)
		if (s->id == id)
			break;

	/* unrecognized: stall. */
	if (!s || !s->s)
		return -1;

	/* string descriptors have length, tag, then UTF16-LE text */
	len = MIN(126, strlen (s->s));
	memset (buf + 2, 0, 2 * len);	/* zero all the bytes */
	len = utf8_to_utf16le(s->s, (unsigned short *)&buf[2], len);
	if (len < 0)
		return -2;
	buf [0] = (len + 1) * 2;
	buf [1] = 0x3;
	return buf [0];
}




//设置设备制造商
int USB_SetDeviceManufacturer(char *manufacturer, int len)
{
	if (manufacturer == NULL)
		return 0;
	if (len<=0)
		return 0;
	if (len>ManufacturerStringMaxNum)
		len = ManufacturerStringMaxNum;
	memset(ManufacturerString, 0, ManufacturerStringMaxNum);
	memcpy(ManufacturerString, manufacturer, len);
	return 1;
}


//设置厂商ID
int USB_SetDeviceVerdorID(unsigned short vid)
{
	if (vid  == 0)
		return 0;
	DeviceVerdorID = vid;
	return 1;
}
//获取厂商ID
unsigned short USB_GetDeviceVerdorID(void)
{
	return DeviceVerdorID;
}

//设置设备序列号
int USB_SetDeviceSerial(char *serialNum, int len)
{
	if (serialNum == NULL)
		return 0;
	if (len<=0)
		return 0;
	if (len>DeviceSerialMaxNum)
		len = DeviceSerialMaxNum;
	memset(DeviceSerial, 0, DeviceSerialMaxNum);
	memcpy(DeviceSerial, serialNum, len);
	return 1;
}
// 获取设备序列号
char *USB_GetDeviceSerial()
{
	return DeviceSerial;
}



