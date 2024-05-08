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
#include "usart.h"
#include "timers.h"
#include "LEDPWM.h"
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
    init_ports();
	
	init_i2c();
	ssd_i2c_init();
	clear_screen();
	init_usart(207);
	write_led();
	sei(); /*enable general interrupts*/

	 
	while (1)
    {
	
    }
}

