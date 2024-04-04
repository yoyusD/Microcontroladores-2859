/*
 * sdk_atmega328p.c
 *
 * Created: 2/27/2024 6:31:58 PM
 * Author : josel
 */ 


#include "ports.h"
#include "leds.h"
#include "i2c.h"
#include "lcd_i2c.h"
#include "ext_int.h"
#include "comp.h"
#include "adc.h"
#include "usart.h"
#include <avr/interrupt.h>
#include "ssd_i2c.h"

int main(void)
{
    init_ports();
	init_i2c();
	init_ext_int();
	init_analog_comp();
	ssd_i2c_init();
	graphic();
	init_adc_withINT();

	/**********************************/
	sei(); /*enable general interrupts*/
	/**********************************/
	while (1)
    {
    }
}

