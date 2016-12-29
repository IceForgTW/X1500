

#include <x1000.h>
#include <time-tcu.h>
#include <mipsregs.h>


#undef printf

void usb_cpm_init(void)
{
	unsigned int ref_clk_div = 24 / 24;//24 / 24;
	unsigned int usbpcr1;

	/* select dwc otg */
	REG_CPM_USBPCR1 |= USBPCR1_USB_SEL;

	/* select utmi data bus width of port0 to 16bit/30M */

	REG_CPM_USBPCR1 |= USBPCR1_WORD_IF0;

	usbpcr1 = REG_CPM_USBPCR1;
	usbpcr1 &= ~(0x3 << 24);
	usbpcr1 |= (ref_clk_div << 24);
	REG_CPM_USBPCR1 = usbpcr1;

	/* fil */
	REG_CPM_USBVBFIL = 0;

	/* rdt */
	REG_CPM_USBRDT =  0x96;
	/* rdt - filload_en */


	REG_CPM_USBRDT |=  USBRDT_VBFIL_LD_EN;

	/* TXRISETUNE & TXVREFTUNE. */
	REG_CPM_USBPCR = 0x3f;
	REG_CPM_USBPCR = 0x35;


	/* enable tx pre-emphasis */
	REG_CPM_USBPCR |= USBPCR_TXPREEMPHTUNE;

	/* OTGTUNE adjust */
	REG_CPM_USBPCR = (7 << 14);

	//usb2.0 ÐÞÕýÑÛÍ¼£º
	REG32(0xb000003C) = 0x82003857;
	REG32(0xb0000040) = 0x02000096;
	REG32(0xb0000044) = 0x00000000;
	REG32(0xb0000048) = 0x9DCA3360;

}


void jz47xx_usb_set_device_only_mode(void)
{
	REG_CPM_USBPCR &= ~USBPCR_USB_MODE;
	REG_CPM_USBPCR &= ~USBPCR_OTG_DISABLE;
    
    REG_CPM_USBPCR |= USBPCR_VBUSVLDEXTSEL;
    REG_CPM_USBPCR |= USBPCR_VBUSVLDEXT;

}

void jz47xx_usb_set_dual_mode(void)
{
	;
}




void jz47xx_usb_phy_init(void)
{
	REG_CPM_USBPCR |= USBPCR_POR;

	mdelay(1);

	REG_CPM_USBPCR &= ~USBPCR_POR;

	mdelay(1);
}


void jz47xx_usb_phy_switch(unsigned char is_on)
{
	unsigned int value;
	if (is_on)
	{

		value = REG_CPM_OPCR;
		REG_CPM_OPCR |= OPCR_OTGPHY0_ENABLE;
		mdelay(1);
	}
	else
	{
		value = REG_CPM_OPCR;
		REG_CPM_OPCR &= ~OPCR_OTGPHY0_ENABLE;

		mdelay(1);
	}
}


