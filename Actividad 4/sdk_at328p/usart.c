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

//global variables

volatile char usart_received_char;
volatile int reception_state = 0; // Estado de recepción inicial RED
volatile int reception_state1 = 0;// Estado de recepción inicial BLUE 
volatile int reception_state2 = 0;// Estado de recepción inicial GREEN
volatile int reception_state3 = 0;
volatile int reception_state4 = 0;
ISR(USART_RX_vect)
{
	
	usart_received_char = UDR0;
	//-------------RED-----------
	switch (usart_received_char)
	{
		case 'R': // Esperando 'R'
	    reception_state = 1;
		break;
		case ('E') : // Esperando 'E'
		if(reception_state == 1){
		reception_state = 2;
		}
		break;
		case ('D'): // Esperando 'D'
		if(reception_state == 2){
			reception_state = 0;
		PORTB = (0 << PINB5) | (0 << PINB4) | (1 << PINB3); // Activar el pin B1
		write_lcd_state(1);
		}
		break;
		default:
		break;
	}
	//--------------BLUE-----------
	switch (usart_received_char)
	{
			
		case 'B': // Esperando 'B'
		reception_state1 = 1;
		break;
		case ('L') : // Esperando 'L'
		if(reception_state1 == 1){
			reception_state1 = 2;
		}
		break;
		case ('U'): // Esperando 'U'
		if(reception_state1 == 2){
			reception_state1 = 3; // Activar el pin B1
		}
		break;
		case ('E') : // Esperando 'E'
		if(reception_state1 == 3){
			reception_state1 = 0;
			PORTB = (1 << PINB5) | (0 << PINB4) | (0 << PINB3); // Activar el pin B1
			write_lcd_state(3);
		}
		break;
		default:
		break;
	}
	//------------GREEN---------
	switch (usart_received_char)
	{
		case 'G': // Esperando 'R'
		reception_state2 = 1;
		break;
		case ('R') : // Esperando 'E'
		if(reception_state2 == 1){
			reception_state2 = 2;
		}
		break;
		case ('E'): // Esperando 'D'
		if(reception_state2 == 2){
			reception_state2 = 3; // Activar el pin B1
		} else{
			reception_state2 = 4; 
		}
		break;
		case ('N') : // Esperando 'E'
		if(reception_state2 == 4){
			reception_state2 = 0;
			PORTB = (0 << PINB5) | (1 << PINB4) | (0 << PINB3); // Activar el pin B2
			write_lcd_state(2);
		}
		break;
		default:
		break;
	}
	//---------------PURPLE---------------
	
		switch (usart_received_char)
	{
		case ('P'): // Esperando 'R'
		  reception_state4 = 1;
		break;
		case ('U') : // Esperando 'E'
		if(reception_state4 == 1){
			reception_state4 = 2;
		}
		break;
		case ('R'): // Esperando 'D'
		if(reception_state4 == 2){
			reception_state4 = 3; // Activar el pin B1
		} 
		break;
		case ('E') : // Esperando 'E'
		if(reception_state4 == 3){
			reception_state4 = 0;
			PORTB = (1 << PINB5) | (0 << PINB4) | (1 << PINB3); // Activar el pin B2
			write_lcd_state(4);
		}
		break;
		default:
		break;
	}
		
	 //-------------------------YELLOW-----------
	 switch (usart_received_char)
	 {
		 case ('Y'): // Esperando 'Y'
		 reception_state3 = 1;
		 break;
		 case ('E') : // Esperando 'E'
		 if(reception_state3 == 1){
			 reception_state3 = 2;
		 }
		 break;
		 case ('L'): // Esperando 'D'
		 if(reception_state3 == 2){
			 reception_state3 = 3; // Activar el pin B1
			 } else if(reception_state3 == 3){
			 reception_state3 = 4;
		 }
		 break;
		 case ('W') : // Esperando 'E'
		 if(reception_state3 == 4){
			 reception_state3 = 0;
			 PORTB = (0 << PINB5) | (1 << PINB4) | (1 << PINB3); // Activar el pin B2
			 write_lcd_state(5);
		 }
		 break;
		 default:
		 break;
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

void init_usart(unsigned int baudrate)
{
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