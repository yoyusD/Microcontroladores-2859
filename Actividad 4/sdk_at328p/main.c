/*
 * sdk_atmega328p.c
 *
 * Created: 2/27/2024 6:31:58 PM
 * Author : josel
 */ 
#include "ssd_i2c.h"
#include <avr/io.h>
#include "ports.h"
#include "i2c.h"
#include "adc.h"
#include "usart.h"
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
    init_ports();
	
	init_i2c();
	ssd_i2c_init();
	clear_screen();
	write_adc();
	init_usart(207);
	init_adc_withINT();
	sei(); /*enable general interrupts*/
	while (1)
    {
		ADCSRA |= 1<<ADSC;
		_delay_ms(600);
    }
}

