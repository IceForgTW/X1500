#include <stdio.h>

#define DWC_PR(r)							\
	printf("DWC2: REG["#r"]=%x\n", *((volatile unsigned int *)((r) | 0xb3500000)))

#define DWC_RR(_r) 	r##_r = (*((volatile unsigned int *)((_r) | 0xb3500000)))
#define DWC_P(v)	printf("DWC2: REG["#v"] = %x\n", r##v)


__attribute__((unused)) void dwc2_core_dump_regs(void) {
	unsigned int r0x000, r0x004, r0x008, r0x00c, r0x010, r0x014, r0x018, r0x01c;
	unsigned int r0x020, r0x024, r0x028, r0x02c, r0x030, r0x034, r0x038, r0x03c;
	unsigned int r0x040, r0x044, r0x048, r0x04c, r0x050, r0x054, r0x058, r0x05c;
	unsigned int r0x060;

	DWC_RR(0x000); DWC_RR(0x004); DWC_RR(0x008); DWC_RR(0x00c);
	DWC_RR(0x010); DWC_RR(0x014); DWC_RR(0x018); DWC_RR(0x01c);
	DWC_RR(0x020); DWC_RR(0x024); DWC_RR(0x028); DWC_RR(0x02c);
	DWC_RR(0x030); DWC_RR(0x034); DWC_RR(0x038); DWC_RR(0x03c);
	DWC_RR(0x040); DWC_RR(0x044); DWC_RR(0x048); DWC_RR(0x04c);
	DWC_RR(0x050); DWC_RR(0x054); DWC_RR(0x058); DWC_RR(0x05c);
	DWC_RR(0x060);

	/*--------------------------*/
	printf(" ----=== %s() start ===---\n", __func__);
	DWC_P(0x000); DWC_P(0x004); DWC_P(0x008); DWC_P(0x00c);
	DWC_P(0x010); DWC_P(0x014); DWC_P(0x018); DWC_P(0x01c);
	DWC_P(0x020); DWC_P(0x024); DWC_P(0x028); DWC_P(0x02c);
	DWC_P(0x030); DWC_P(0x034); DWC_P(0x038); DWC_P(0x03c);
	DWC_P(0x040); DWC_P(0x044); DWC_P(0x048); DWC_P(0x04c);
	DWC_P(0x050); DWC_P(0x054); DWC_P(0x058); DWC_P(0x05c);
	DWC_P(0x060);
	printf(" ----=== %s() end ===---\n", __func__);

	/*--------------------------*/
}

__attribute__((unused)) void dwc2_cpm_dump_regs(void) {
	printf(" ----=== %s() start ===---\n", __func__);
	printf("DWC2: CPM 0x10000050 %x\n", *(volatile unsigned int *)(0xb0000050));
	printf("DWC2: CPM 0x1000003c %x\n", *(volatile unsigned int *)(0xb000003c));
	printf("DWC2: CPM 0x10000048 %x\n", *(volatile unsigned int *)(0xb0000048));
	printf(" ----=== %s() end ===---\n", __func__);
}

__attribute__((unused)) void dwc2_ep0_dump_regs(void) {
	unsigned int r0x800, r0x804, r0x808, r0x80c, r0x810, r0x814, r0x818, r0x81c;
	unsigned int r0x900, r0x908, r0x910, r0x914;
	unsigned int r0xb00, r0xb08, r0xb10, r0xb14;

	DWC_RR(0x900);
	DWC_RR(0x908);
	DWC_RR(0x910);
	DWC_RR(0x914);

	DWC_RR(0xb00);
	DWC_RR(0xb08);
	DWC_RR(0xb10);
	DWC_RR(0xb14);

	DWC_RR(0x800);
	DWC_RR(0x804);
	DWC_RR(0x808);
	DWC_RR(0x80c);
	DWC_RR(0x810);
	DWC_RR(0x814);
	DWC_RR(0x818);
	DWC_RR(0x81c);

	/*--------------------------*/

	printf(" ----=== %s() start ===---\n", __func__);
	DWC_P(0x800);
	DWC_P(0x804);
	DWC_P(0x808);
	DWC_P(0x80c);
	DWC_P(0x810);
	DWC_P(0x814);
	DWC_P(0x818);
	DWC_P(0x81c);

	DWC_P(0x900);
	DWC_P(0x908);
	DWC_P(0x910);
	DWC_P(0x914);

	DWC_P(0xb00);
	DWC_P(0xb08);
	DWC_P(0xb10);
	DWC_P(0xb14);
	printf(" ----=== %s() end ===---\n", __func__);


	/*--------------------------*/
}


__attribute__((unused)) void dwc2_dump_ep_regs(
	int epnum, const char *func, int line) {
	unsigned int r0x800, r0x804, r0x808, r0x80c, r0x810, r0x814, r0x818, r0x81c;
	unsigned int r0x900 = 0, r0x908 = 0, r0x910 = 0, r0x914 = 0, r0xb00 = 0, r0xb08 = 0, r0xb10 = 0, r0xb14 = 0;
	unsigned int r0x920 = 0, r0x928 = 0, r0x930 = 0, r0x934 = 0, r0xb20 = 0, r0xb28 = 0, r0xb30 = 0, r0xb34 = 0;
	unsigned int r0x940 = 0, r0x948 = 0, r0x950 = 0, r0x954 = 0, r0xb40 = 0, r0xb48 = 0, r0xb50 = 0, r0xb54 = 0;
	unsigned int r0x960 = 0, r0x968 = 0, r0x970 = 0, r0x974 = 0, r0xb60 = 0, r0xb68 = 0, r0xb70 = 0, r0xb74 = 0;

	DWC_RR(0x800);
	DWC_RR(0x804);
	DWC_RR(0x808);
	DWC_RR(0x80c);
	DWC_RR(0x810);
	DWC_RR(0x814);
	DWC_RR(0x818);
	DWC_RR(0x81c);

	switch (epnum) {
	case -1:
	case 0:
		DWC_RR(0x900);
		DWC_RR(0x908);
		DWC_RR(0x910);
		DWC_RR(0x914);
		DWC_RR(0xb00);
		DWC_RR(0xb08);
		DWC_RR(0xb10);
		DWC_RR(0xb14);

		if (epnum != -1)
			break;
	case 1:
		DWC_RR(0x920);
		DWC_RR(0x928);
		DWC_RR(0x930);
		DWC_RR(0x934);
		DWC_RR(0xb20);
		DWC_RR(0xb28);
		DWC_RR(0xb30);
		DWC_RR(0xb34);

		if (epnum != -1)
			break;
	case 2:
		DWC_RR(0x940);
		DWC_RR(0x948);
		DWC_RR(0x950);
		DWC_RR(0x954);
		DWC_RR(0xb40);
		DWC_RR(0xb48);
		DWC_RR(0xb50);
		DWC_RR(0xb54);

		if (epnum != -1)
			break;
	case 3:
		DWC_RR(0x960);
		DWC_RR(0x968);
		DWC_RR(0x970);
		DWC_RR(0x974);
		DWC_RR(0xb60);
		DWC_RR(0xb68);
		DWC_RR(0xb70);
		DWC_RR(0xb74);

		if (epnum != -1)
			break;
	default:
		return;
	}

	printf(" ----=== %s() ===---\n", __func__);
	printf("=====%s:%d ep%d regs=====\n", func, line, epnum);
	DWC_P(0x800);
	DWC_P(0x804);
	DWC_P(0x808);
	DWC_P(0x80c);
	DWC_P(0x810);
	DWC_P(0x814);
	DWC_P(0x818);
	DWC_P(0x81c);

	switch (epnum) {
	case -1:
	case 0:
		DWC_P(0x900);
		DWC_P(0x908);
		DWC_P(0x910);
		DWC_P(0x914);
		DWC_P(0xb00);
		DWC_P(0xb08);
		DWC_P(0xb10);
		DWC_P(0xb14);

		if (epnum != -1)
			return;
	case 1:
		DWC_P(0x920);
		DWC_P(0x928);
		DWC_P(0x930);
		DWC_P(0x934);
		DWC_P(0xb20);
		DWC_P(0xb28);
		DWC_P(0xb30);
		DWC_P(0xb34);

		if (epnum != -1)
			return;
	case 2:
		DWC_P(0x940);
		DWC_P(0x948);
		DWC_P(0x950);
		DWC_P(0x954);
		DWC_P(0xb40);
		DWC_P(0xb48);
		DWC_P(0xb50);
		DWC_P(0xb54);

		if (epnum != -1)
			return;
	case 3:
		DWC_P(0x960);
		DWC_P(0x968);
		DWC_P(0x970);
		DWC_P(0x974);
		DWC_P(0xb60);
		DWC_P(0xb68);
		DWC_P(0xb70);
		DWC_P(0xb74);

		if (epnum != -1)
			return;
	default:
		return;
	}

}



