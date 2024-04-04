/*
 * ports.c
 *
 * Created: 2/27/2024 6:34:52 PM
 *  Author: josel
 */ 
#include <avr/io.h>

void init_ports(void)
{
	DDRB = 0xFF;
	// PD2 input for INT0, PD3 input for INT1
	DDRD &= (0 << PIND2) | (0 << PIND3);
	PORTD |= (1 << PIND2) | (1 << PIND3); //enable Rp
	// PC0 input for ADC0
	// First set ADC0-2 pins in output
	DDRC = (0 << PINC0)|(0 << PINC1)|(0 << PINC2);
	//PORTC = (1 << PINC0)|(1 << PINC1)|(1 << PINC2); //enable Rp
		
}