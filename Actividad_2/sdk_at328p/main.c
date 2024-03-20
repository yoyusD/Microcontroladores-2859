/*
 * sdk_atmega328p.c
 *
 * Created: 2/27/2024 6:31:58 PM
 * Author : josel
 */ 

#include <avr/io.h>
#include "ports.h"
#include "leds.h"
#include "i2c.h"
#include "lcd_i2c.h"
#include <util/delay.h>
int main(void)
{
	
	unsigned char Character1[8] = { 0x00, 0x00, 0x0A, 0x15, 0x11, 0x0A, 0x04, 0x00 };  /* Custom char set for alphanumeric LCD Module */
	unsigned char Character2[8] = { 0x00, 0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00 };
	unsigned char Character3[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0A };
	unsigned char Character4[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0A, 0x15 };
	unsigned char Character5[8] = { 0x00, 0x00, 0x00, 0x00, 0x04, 0x0A, 0x15, 0x11 };
	unsigned char Character6[8] = { 0x00, 0x00, 0x00, 0x04, 0x0A, 0x15, 0x11, 0x15 };
	unsigned char Character7[8] = { 0x00, 0x00, 0x04, 0x0A, 0x15, 0x11, 0x15, 0xFF };
	unsigned char Character8[8] = { 0x00, 0x04, 0x0A, 0x15, 0x11, 0x15, 0xFF, 0xFF };
   
	
	init_ports();
	init_i2c();
	lcd_i2c_init();
	//This is the scrolling writing function
	lcd_i2c_long_shift("A partir del desarrollo de circuitos Integrados Digitales programables con una gran cantidad de componentes lógicos y la necesidad de sistema digitales para aplicaciones mas complejas, las herramientas de diseño tradicionales se vuelven cada vez más ineficientes y poco efectivas para lograr desarrollos adecuados, por lo tanto las empresas fabricantes de circuitos integrados desarrollan herramientas más útiles, originándose así los HDL ó Lenguajes de Descripción de hardware. Así");
	
	LCD_Custom_Char(0, Character1);
	LCD_Custom_Char(1, Character2);
	LCD_Custom_Char(2, Character3);
	LCD_Custom_Char(3, Character4);
	LCD_Custom_Char(4, Character5);
	LCD_Custom_Char(5, Character6);
	LCD_Custom_Char(6, Character7);
	LCD_Custom_Char(7, Character8);
	
	figuras(); //the figure in motion

	
	

}


