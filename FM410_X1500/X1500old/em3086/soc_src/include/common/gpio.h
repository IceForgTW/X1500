#ifndef __GPIO_H__
#define __GPIO_H__

unsigned int gpio_intr_init(GPIO_INTR_TRIGGER_TYPE gitt, unsigned int pin);
void gpio_set_dir(unsigned int pin, bool output, bool level_or_pull);
int gpio_get_pin(unsigned int pin);
void gpio_set_pin(unsigned int pin);
void gpio_clear_pin(unsigned int pin);
void gpio_disable_pull(unsigned int pin);
void gpio_as_input(unsigned int pin);
void gpio_as_output(unsigned int pin);
int gpio_get_earphone_status();

#endif
