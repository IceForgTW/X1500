/*
 * linux/include/asm-mips/mach-jz4770/jz4770efuse.h
 *
 * JZ4770 DDRC register definition.
 *
 * Copyright (C) 2010 Ingenic Semiconductor Co., Ltd.
 */

#ifndef __JZX1000EFUSE_H__
#define __JZX1000EFUSE_H__

#define EFUSE_BASE	0xB3540000

#define EFUSE_CTRL      (EFUSE_BASE + 0x00)
#define EFUSE_CFG       (EFUSE_BASE + 0x04)
#define EFUSE_STATE     (EFUSE_BASE + 0x08)
#define EFUSE_DATA0     (EFUSE_BASE + 0x0c)
#define EFUSE_DATA1     (EFUSE_BASE + 0x10)
#define EFUSE_DATA2     (EFUSE_BASE + 0x14)
#define EFUSE_DATA3     (EFUSE_BASE + 0x18)
#define EFUSE_DATA4     (EFUSE_BASE + 0x1c)
#define EFUSE_DATA5     (EFUSE_BASE + 0x20)
#define EFUSE_DATA6     (EFUSE_BASE + 0x24)
#define EFUSE_DATA7     (EFUSE_BASE + 0x28)

#define REG_EFUSE_CTRL      REG32(EFUSE_CTRL)
#define REG_EFUSE_CFG       REG32(EFUSE_CFG)
#define REG_EFUSE_STATE     REG32(EFUSE_STATE)
#define REG_EFUSE_DATA0     REG32(EFUSE_DATA0)
#define REG_EFUSE_DATA1     REG32(EFUSE_DATA1)
#define REG_EFUSE_DATA2     REG32(EFUSE_DATA2)
#define REG_EFUSE_DATA3     REG32(EFUSE_DATA3)
#define REG_EFUSE_DATA4     REG32(EFUSE_DATA4)
#define REG_EFUSE_DATA5     REG32(EFUSE_DATA5)
#define REG_EFUSE_DATA6     REG32(EFUSE_DATA6)
#define REG_EFUSE_DATA7     REG32(EFUSE_DATA7)
#define REG_EFUSE_DATA(x)   REG32(EFUSE_DATA0 + 4*x)

#define EFUSE_CTRL_LOW_BIT	(0x1 << 0)
#define EFUSE_CTRL_HIGH_BIT	(0x1 << 1)

#define efuse_low_bit_enable()		(REG_EFUSE_CTRL = EFUSE_CTRL_LOW_BIT)
#define efuse_low_bit_disable()		(REG_EFUSE_CTRL &= ~EFUSE_CTRL_LOW_BIT)
#define efuse_high_bit_enable()		(REG_EFUSE_CTRL = EFUSE_CTRL_HIGH_BIT)
#define efuse_high_bit_disable()	(REG_EFUSE_CTRL &= ~EFUSE_CTRL_HIGH_BIT)
#define efuse_full_bit_enable()		(REG_EFUSE_CTRL = (EFUSE_CTRL_LOW_BIT | EFUSE_CTRL_HIGH_BIT))

#define efuse_wait_write_full_ready()						\
do {										\
	while(REG_EFUSE_CTRL & (EFUSE_CTRL_LOW_BIT | EFUSE_CTRL_HIGH_BIT));	\
}while(0)

#define efuse_wait_write_low_ready()			\
do {							\
	while(REG_EFUSE_CTRL & EFUSE_CTRL_LOW_BIT);	\
}while(0)

#define efuse_wait_write_high_ready()			\
do {							\
	while(REG_EFUSE_CTRL & EFUSE_CTRL_HIGH_BIT);	\
}while(0)
#define efuse_low_disable_prog()	(REG_EFUSE_DATA3 |= (1 << 31))
#define efuse_high_disable_prog()	(REG_EFUSE_DATA7 |= (1 << 31))

#endif
