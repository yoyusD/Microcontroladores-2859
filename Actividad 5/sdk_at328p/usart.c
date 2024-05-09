/*
 * usart.c
 *
 *  Author: jlb
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "ssd_i2c.h"
#include "LEDPWM.h"
#include <string.h>



//global variables

volatile char usart_received_char;
volatile int reception_state = 0; 
volatile int reception_state1 = 0;
volatile int reception_state2 = 0;
volatile int reception_state3 = 0;
volatile int reception_state4 = 0;
volatile int reception_state5 = 0;
volatile int reception_state0 = 0;
volatile int state = 0;
volatile char a,b,c; //1 byte variables 
volatile int ai,bi,ci,di; //Integer variables
ISR(USART_RX_vect)
{
	
	usart_received_char = UDR0;
	//-------------CONTROL PWM RGB-----------///////////////////////////////////
	switch (usart_received_char)
	{
		case 'R': // Waiting 'R'
	    reception_state = 1;
		break;
		case 'G': // Waiting 'G'
		reception_state = 2;
		break;
		case 'B': // Waiting 'B'
		reception_state = 3;
		break;
		default:
		break;
	}

	if((usart_received_char >= 48)&&(usart_received_char <= 57)){
		if(state == 0){
			 a = usart_received_char;
			 ssd_num_write(a, reception_state,state);
			 ai = a - 48; //a integer  
			 state = 1;
		}else if(state == 1){
			b = usart_received_char;
			ssd_num_write(b, reception_state,state);
			bi = b - 48; //b integer
			state = 2;
		}else if(state == 2){
			c = usart_received_char;
			ci = c - 48; //c integer
			ssd_num_write(c, reception_state,state);
			di = ai + (bi*10) + (ci*100); //integer result 
			pwm_led(reception_state,di);
			state = 0;
			reception_state = 0;
			write_lcd_state(7);
		}
 
	}
	else if(((usart_received_char >= 90)&&(usart_received_char <= 160))||(usart_received_char <= 0)){
	}

////////////// PWM COMMANDS FOR RGB LED LIGHTING //////////////////////////////
		
		//-------------RED-----------///
		switch (usart_received_char)
		{
			case 'R': // Waiting 'R'
			reception_state0 = 1;
			break;
			case ('E') : // Waiting 'E'
			if(reception_state0 == 1){
				reception_state0 = 2;
			}
			break;
			case ('D'): // Waiting 'D'
			if(reception_state0 == 2){
				reception_state0 = 0;
				pwm_led(1,255);
				pwm_led(2,0);
				pwm_led(3,0);
				ssd_num_write_data("255",1);
				ssd_num_write_data("000",2);
				ssd_num_write_data("000",3);
				write_lcd_state(7);
				write_lcd_state(1);
			}
			break;
			default:
			break;
		}
		//--------------BLUE-----------////
		switch (usart_received_char)
		{
			
			case 'B': // Waiting 'B'
			reception_state1 = 1;
			break;
			case ('L') : // Waiting 'L'
			if(reception_state1 == 1){
				reception_state1 = 2;
			}
			break;
			case ('U'): // Waiting 'U'
			if(reception_state1 == 2){
				reception_state1 = 3; 
			}
			break;
			case ('E') : // Waiting 'E'
			if(reception_state1 == 3){
				reception_state1 = 0;
				pwm_led(3,240);
				pwm_led(2,0);
				pwm_led(1,0);
				ssd_num_write_data("000",1);
				ssd_num_write_data("000",2);
				ssd_num_write_data("240",3);
				write_lcd_state(7);
				write_lcd_state(3);
			}
			break;
			default:
			break;
		}
		//------------GREEN---------
		switch (usart_received_char)
		{
			case 'G': // Waiting 'G'
			reception_state2 = 1;
			break;
			case ('R') : //Waiting 'R'
			if(reception_state2 == 1){
				reception_state2 = 2;
			}
			break;
			case ('E'): // Waiting 'E'
			if(reception_state2 == 2){
				reception_state2 = 3; 
				} else{
				reception_state2 = 4;
			}
			break;
			case ('N') : // Waiting 'N'
			if(reception_state2 == 4){
				reception_state2 = 0;
				pwm_led(2,220);
				pwm_led(1,0);
				pwm_led(3,0);
				ssd_num_write_data("000",1);
				ssd_num_write_data("220",2);
				ssd_num_write_data("000",3);
				write_lcd_state(7);
				write_lcd_state(2);
			}
			break;
			default:
			break;
		}
		//---------------PURPLE---------------
		
		switch (usart_received_char)
		{
			case ('P'): // Waiting 'P'
			reception_state4 = 1;
			break;
			case ('U') : // Waiting 'U'
			if(reception_state4 == 1){
				reception_state4 = 2;
			}
			break;
			case ('R'): // Waiting 'R'
			if(reception_state4 == 2){
				reception_state4 = 3; // Activar el pin B1
			}
			break;
			case ('E') : // Waiting 'E'
			if(reception_state4 == 3){
				reception_state4 = 0;
				pwm_led(1,255);
				pwm_led(2,0);
				pwm_led(3,215);
				ssd_num_write_data("255",1);
				ssd_num_write_data("000",2);
				ssd_num_write_data("215",3);
				write_lcd_state(7);
				write_lcd_state(4);
			}
			break;
			default:
			break;
		}
		
		//-------------------------YELLOW-----------
		switch (usart_received_char)
		{
			case ('Y'): // Waiting 'Y'
			reception_state3 = 1;
			break;
			case ('E') : // Waiting 'E'
			if(reception_state3 == 1){
				reception_state3 = 2;
			}
			break;
			case ('L'): // Waiting 'L'
			if(reception_state3 == 2){
				reception_state3 = 3; // Activar el pin B1
				} else if(reception_state3 == 3){
				reception_state3 = 4;
			}
			break;
			case ('W') : // Waiting 'W'
			if(reception_state3 == 4){
				reception_state3 = 0;
				pwm_led(1,200);
				pwm_led(2,150);
				pwm_led(3,0);
				ssd_num_write_data("200",1);
				ssd_num_write_data("150",2);
				ssd_num_write_data("000",3);
				write_lcd_state(7);
				write_lcd_state(5);
			}
			break;
			default:
			break;
		}
		//------------------CELESTE-----------------------
		switch (usart_received_char)
		{
			case ('C'): // Waiting 'C'
			reception_state5 = 1;
			break;
			case ('E') : // Waiting 'E'
			if(reception_state5 == 1){
				reception_state5 = 2;
			}
			break;
			case ('L'): // Waiting 'L'
			if(reception_state5 == 2){
				reception_state5 = 3; 
			} 
			break;
			case ('S') : // Waiting 'E'
			if(reception_state5 == 3){
				reception_state5 = 0;
				pwm_led(3,235);
				pwm_led(2,195);
				pwm_led(1,0);
				ssd_num_write_data("000",1);
				ssd_num_write_data("195",2);
				ssd_num_write_data("235",3);
				write_lcd_state(7);
				write_lcd_state(6);
			}
			break;
			default:
			break;
		}
	
		if ( usart_received_char == 'F'){
			for(int co = 0; co<5; co++){
				pwm_led(3,40);
				_delay_ms(500);
				pwm_led(2,255);
				_delay_ms(500);
				pwm_led(1,100);
				_delay_ms(500);
				pwm_led(3,255);
				_delay_ms(500);
				pwm_led(2,100);
				_delay_ms(500);
				pwm_led(1,20);
				_delay_ms(500);
			}
		}
	
	


}
		 
		
	

unsigned char usart_receive( void )
{
  while (!(UCSR0A & (1 << RXC0))); // wait for data (RXC=1)
  return UDR0;
}

void usart_transmit( unsigned char data )
{
  while (!(UCSR0A & (1 << UDRE0)));  // wait for transmitter buffer empty(UDRE=1)
  UDR0 = data; // when buffer empty, write data to UDR
}

void usart_transmit_string( char s[] )
{	
	int i = 0;
  while (i < 64)
  {
	  if (s[i] == '\0') break;
	  usart_transmit(s[i++]);
  }
}

void init_usart(unsigned int baudrate){
	UCSR0C &= (~(1<<UMSEL00) & ~(1<<UMSEL01)); // bit UMSEL = 0 asyncronous mode	
	UCSR0A = (1<<U2X0); // bit U2X = 1 double speed
	//Baudrate:fosc=16Mhz,U2Xn=1,BaudRate=9600, then UBRR= 207 (DS pag.199)
	UBRR0H = (unsigned char) (baudrate>>8); // write(MSB)
	UBRR0L = (unsigned char) (baudrate);	//write(LSB)
	UCSR0C = ((1<<UCSZ00) | (1<<UCSZ01)); //8 bits data lenght
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);	//enable transmitter & receiver
	//UCSRC = (1<<USBS);	//2 stop bits
	UCSR0B |= (1<<RXCIE0);	//enable reception complete (RXCIE0=1) interrupts
}