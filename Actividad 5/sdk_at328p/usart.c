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
volatile int reception_state = 0; // Estado de recepci�n inicial RED
volatile int reception_state1 = 0;// Estado de recepci�n inicial BLUE 
volatile int reception_state2 = 0;// Estado de recepci�n inicial GREEN
volatile int reception_state3 = 0;
volatile int state = 0;
volatile char a,b,c;
volatile int ai,bi,ci,di; 
ISR(USART_RX_vect)
{
	
	usart_received_char = UDR0;
	//-------------RED-----------
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
		}
		
// 		ssd_num_write("1", 1);
// 		ssd_num_write("1", 2);
// 		ssd_num_write("1", 3);
 
	}
	else if(((usart_received_char >= 90)&&(usart_received_char <= 160))||(usart_received_char <= 0)){
		
// 		ssd_num_write("0", 1);
// 		ssd_num_write("0", 2);
// 		ssd_num_write("0", 3);
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