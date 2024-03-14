/*
 * lcd_i2c.c
 *
 * Created: 6/29/2021 7:38:17 PM
 *  Author: jlb
 */
#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "lcd_i2c.h"
#include <stdio.h>
#include <stdbool.h>


//first column address
char Columna_1 [2] = {0 , 64};	

/*	4 bits mode (pag.46 44780 datasheet) */
void lcd_i2c_reset(void)
{
	//unsigned char data;
	start();	//START--> I2C
	write_i2c(PCF8574_ADDR);
	
	write_i2c(0xFF); //wait for 15ms (at least)
	_delay_ms(20);
	
	write_i2c(0x30+LCD_EN); //write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 & E=1
	write_i2c(0x30);		//write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 & E=0
	_delay_ms(10);			//wait for 4.1ms (ate least)
	
	write_i2c(0x30+LCD_EN); //write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 & E=1
	write_i2c(0x30);		//write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 & E=0
	_delay_ms(1);			//wait for 100 us (at least)
	
	write_i2c(0x30+LCD_EN); //write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 & E=1
	write_i2c(0x30);		//write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 & E=0
	_delay_ms(1);			//wait for 100 us (at least)
	
	write_i2c(0x20+LCD_EN); //write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=0 & E=1
	write_i2c(0x20);		//write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=0 & E=0
	_delay_ms(1);			//wait for 100 us (at least)
	
	stop();	//STOP--> I2C
}

/*	write command to LCD	*/
void lcd_i2c_cmd (char cmd)
{
	unsigned char data;
	start();	//START--> I2C
	write_i2c(PCF8574_ADDR);
	//I2C address
	
	data=(cmd & 0xF0)|LCD_EN|LUZ_FONDO;
	write_i2c(data);

	data=(cmd & 0xF0)|LUZ_FONDO;
	write_i2c(data);

	data=((cmd << 4) & 0xF0)|LCD_EN|LUZ_FONDO;
	write_i2c(data);

	data=((cmd << 4) & (0xF0|LUZ_FONDO));
	write_i2c(data);
	
	stop();	//STOP--> I2C
	_delay_ms(2);
	_delay_ms(2);
}

/*	LCD	initialization */
void lcd_i2c_init (void)
{
	init_i2c(); //****init I2C****

	lcd_i2c_reset();		// Reset LCD.
	
	lcd_i2c_cmd(0x2C);		// Function Set: 4 bits mode, 2 lines, 5×10 resolution
	//lcd_i2c_cmd(0x0F);	// Display control: LCD on, cursor on, blinking
	lcd_i2c_cmd(0x0C);		// cursor off
	lcd_i2c_cmd(0x06);		// Entry mode: Increment.
	lcd_i2c_cmd(0x80);		// points to DDRAM.
}

/*	display character	*/
void lcd_i2c_data (unsigned char dat)
{
	unsigned char data;
	start();	//START--> I2C
	write_i2c(PCF8574_ADDR);

	data=((dat & 0xF0)|LCD_EN|LCD_RS|LUZ_FONDO);
	write_i2c(data);

	data=((dat & 0xF0)|LCD_RS|LUZ_FONDO);
	write_i2c(data);

	data=(((dat << 4) & 0xF0)|LCD_EN|LCD_RS|LUZ_FONDO);
	write_i2c(data);

	data=(((dat << 4) & 0xF0)|LCD_RS|LUZ_FONDO);
	write_i2c(data);

	stop();	// STOP--> I2C
	_delay_ms(2);
	_delay_ms(2);
}

/* set cursor on specified column&row	*/
void lcd_i2c_col_row(uint8_t x, uint8_t y)
{
	lcd_i2c_cmd(0x80 + Columna_1[y-1]+(x-1));
}

/*	clear screen	*/
void lcd_i2c_clr(void)
{
	lcd_i2c_cmd(0x01); //clear LCD (0000 0001)
	_delay_ms(2);
}

/*	write character string	*/
void lcd_i2c_write_string(char *a)
{
	unsigned int i;
	for(i=0;a[i]!=0;i++){
		lcd_i2c_data(a[i]);
		
	}
}

/*	Write integer value */
void lcd_i2c_write_int(int value,unsigned int field_length)
{
	char str[5]={0,0,0,0,0};
	int i=4,j=0;
	while(value)
		{
			str[i]=value%10;
			value=value/10;
			i--;
		}
	if(field_length==-1)
	while(str[j]==0) j++;
	else  
		j=5-field_length;
	if(value<0) lcd_i2c_data('-');
	for(i=j;i<5;i++)
		{
			lcd_i2c_data(48+str[i]);
		}
}



void lcd_i2c_long_shift(char *w) {
	lcd_i2c_cmd(0x01); 
	_delay_ms(2);
	unsigned count = 0;
	for(unsigned int s = 0; s < 200; s++) { //infinite loop attempt
		lcd_i2c_data(w[count]); //Write the first character
		count++;
		_delay_ms(150);
		
		switch(s) {  //With switch statement, I can write all 64 characters in the LCD
			case 16 ... 24:
			case 25 ... 32:
			lcd_i2c_cmd(0x18); //shift the screen to the left 
			if(s == 32) {
				lcd_i2c_cmd(0xC0); //Set second row and the first column 
				_delay_ms(500);
				for(unsigned int i = 0; i < 17; i++) {
					_delay_ms(200);
					lcd_i2c_cmd(0x1C); //shift the screen to the right  
				}
			}
			break;
			case 48 ... 65: 
			lcd_i2c_cmd(0x18); //shift the screen to the left 
			if(s == 65) {
				lcd_i2c_cmd(0x80);
				_delay_ms(500);
				for(unsigned int i = 0; i < 17; i++) {
					_delay_ms(200);
					lcd_i2c_cmd(0x1C);  //shift the screen to the right  
				}
			}
			break;
			default:
			break;
		}
		
		if(w[count] == '\0') { 
			break; //exit out of "for" loop
		}
		else if (s == 73) { 
			s = 0; //Reset counter
		}
	}
}



//Otra funcion de escritura
void lcd_i2c_long_writing(char *wd) {
	unsigned int i = 0;
	bool p  = false; 
	for(unsigned int c = 0; c <= 200; c++){
		
		lcd_i2c_data(wd[i]);
		_delay_ms(100);
		i++;
		
		if( c == 15 && !p ){
			lcd_i2c_cmd(0xC0);
			p = true;
		}
		
		if( (c % 16 == 0)){
			lcd_i2c_cmd(0x01);
			for(unsigned s = 0; s<15; s++){
				lcd_i2c_data(wd[i-15+s]);
				_delay_ms(50);
			}
			lcd_i2c_cmd(0xC0);
			for(unsigned s = 0; s<15; s++){
				lcd_i2c_data(wd[i]);
				_delay_ms(100);
				i++;
			}
		}
		if(wd[i] == '\0') {
			break;
		}
	}
}


void LCD_Custom_Char(unsigned char loc, unsigned char *msg)
{
	unsigned char i;
	if(loc<8)
	{
		lcd_i2c_cmd(0x40 + (loc*8));	/* Command 0x40 and onwards forces the device to point CGRAM address */
		for(i=0;i<8;i++)	/* Write 8 byte for generation of 1 character */
		lcd_i2c_data(msg[i]);
	}
	
	lcd_i2c_cmd(0x80);
}

void figuras(void){
	unsigned char i;
	lcd_i2c_cmd(0x00);
	for(i = 0; i<7 ; i++){
		lcd_i2c_data(0x00+i);
		_delay_ms(300);
		_delay_ms(300);
		lcd_i2c_cmd(0x80);
	}
}