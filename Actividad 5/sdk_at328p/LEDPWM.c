/*
 * LEDPWM.c
 *
 * Created: 07/05/2024 10:21:52 a. m.
 *  Author: guado
 */ 
#include <avr/io.h>

void pwm_led(int led_color,int duty_cycle){ //duty_cycle RANGE = 0 - 255
	TCCR0B |= (1<<CS00)|(1<<CS01); //prescaler Fosc/64
	TCCR2B |= (1<<CS20)|(1<<CS21); //prescaler Fosc/64
	
	if(led_color == 1) //RED
	{
		TCCR0A |= (1<<WGM01)|(1<<WGM00)|(1<<COM0A1);//fast pwm, non inverted
		if(duty_cycle > 255){
			duty_cycle = 255; 
		}
		OCR0A=duty_cycle; //PIND6
	}
	
	else if(led_color == 2){ //GREEN
		TCCR0A |= (1<<WGM01)|(1<<WGM00)|(1<<COM0B1);//fast pwm, non inverted
		if(duty_cycle > 255){
			duty_cycle = 255;
		}
		OCR0B=duty_cycle; //PIND5
	}
	
	else if (led_color ==3) //BLUE
	{
		TCCR2A |= (1<<WGM21)|(1<<WGM20)|(1<<COM2B1);//fast pwm, non inverted
		if(duty_cycle > 255){
			duty_cycle = 255;
		}
		OCR2B=duty_cycle; //PIND3
	}
	
	
}



