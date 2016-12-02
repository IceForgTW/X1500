/********************** BEGIN LICENSE BLOCK ************************************
 *
 * JZ4755  mobile_tv  Project  V1.0.0
 * INGENIC CONFIDENTIAL--NOT FOR DISTRIBUTION IN SOURCE CODE FORM
 * Copyright Ingenic Semiconductor Co. Ltd 2008. All rights reserved.
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
 *  Create:   2008-06-26, by dsqiu, dsqiu
 *
 *  Maintain: 2008-11-13, by whxu, Add gpio_intr_init
 *
 *  Maintain: 2008-11-19, by whxu, Add gpio_set_dir
 *
 *******************************************************************************
 */

#include <bsp.h>
#include <x1000.h>

//----------------------------------------------------------------------------

unsigned int gpio_intr_init(GPIO_INTR_TRIGGER_TYPE gitt, unsigned int pin)
{
	if(gitt == GITT_F_EDGE)
	{
		__gpio_as_irq_fall_edge(pin);
		goto out;
	}
	else if(gitt == GITT_R_EDGE)
	{
		__gpio_as_irq_rise_edge(pin);
		goto out;
	}
	else if(gitt == GITT_H_LEVEL)
	{
		__gpio_as_irq_high_level(pin);
		goto out;
	}
	else if(gitt == GITT_L_LEVEL)
	{
		__gpio_as_irq_low_level(pin);
		goto out;
	}
	else 
	{
		return 0;
	}

out:
	__gpio_ack_irq(pin);
     return 1;
}


// ---------------------------------------------------------------------------
//
// @func    void | gpio_set_dir | Configure the GPIO direction 
//
// @param
//			@1 unsigned int pin
//				-> The gpio index 
//			@2 bool output
//				-> The gpio direction, 1 is output and 0 is input
//			@3 bool level_or_pull
//				-> When the @2 is 1, the @3 is level, 1 is HIGH, 0 is LOW;
//				   When the @2 is 0, the @3 is PULL, 1 is PULL, 0 is NO-PULL;
//
// @rdesc   void.
//
// ---------------------------------------------------------------------------


void gpio_set_dir(unsigned int pin, bool output, bool level_or_pull)
{
	if(output == 1)
	{
		__gpio_as_output(pin);
		if(level_or_pull == 1)
		{
			__gpio_set_pin(pin);	
		}
		else
		{
			__gpio_clear_pin(pin);	
		}			
	}
	else 
	{
		__gpio_as_input(pin);
		if(level_or_pull ==1)
		{
			__gpio_enable_pull(pin);	
		}
		else
		{
			__gpio_disable_pull(pin);	
		}		
	}
			
}


//----------------------------------------------------------------------------

void gpio_init(void)
{
	
}

//----------------------------------------------------------------------------

int gpio_get_pin(unsigned int pin)
{
	return __gpio_get_pin(pin);
}


//----------------------------------------------------------------------------

void gpio_set_pin(unsigned int pin)
{
	__gpio_set_pin(pin);
}

//----------------------------------------------------------------------------

void gpio_clear_pin(unsigned int pin)
{
	__gpio_clear_pin(pin);
}

//----------------------------------------------------------------------------

void gpio_disable_pull(unsigned int pin)
{
	__gpio_disable_pull(pin);
}

//----------------------------------------------------------------------------

void gpio_as_input(unsigned int pin)
{
	__gpio_as_input(pin);	
}

//----------------------------------------------------------------------------

void gpio_as_output(unsigned int pin)
{
	__gpio_as_output(pin);
}

//----------------------------------------------------------------------------

void gpio_set_mute(int mute)
{

}

//----------------------------------------------------------------------------

int gpio_get_earphone_status()
{
    return 0;
}


//----------------------------------------------------------------------------
