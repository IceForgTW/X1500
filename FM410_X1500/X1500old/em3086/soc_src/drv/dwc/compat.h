#ifndef __COMPAT_H__
#define __COMPAT_H__



int usb_gadget_get_string (int id, unsigned char *buf);
int USB_SetDeviceManufacturer(char *manufacturer, int len);
int USB_SetDeviceVerdorID(unsigned short vid);
unsigned short USB_GetDeviceVerdorID(void);
int USB_SetDeviceSerial(char *serialNum, int len);
// 获取设备序列号
char *USB_GetDeviceSerial();


void enable_device(unsigned int handle);
void disable_device(unsigned int handle);
void init_endpoint_suppost(unsigned int handle,unsigned char *ep,
		USB_ENDPOINT_TYPE ep_type,unsigned short *ep_max_pkg, unsigned char *buf);
void set_address(unsigned int handle,unsigned short value);
void start_transfer(unsigned int handle,unsigned char ep,unsigned char *buf,unsigned int len);

#endif
