/*
 * ssd_i2c.c
 *
 * Created: 24/03/2024 08:52:08 p. m.
 *  Author: guado
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "ssd_i2c.h"



void ssd_i2c_init (void){
	init_i2c();				//****init I2C****
	lcd_i2c_reset();		// Reset LCD.
	ssd_i2c_cmd(0xAE);		// Set display OFF
	ssd_i2c_cmd(0xD4);		// Set Display Clock Divide Ratio / OSC Frequency
	ssd_i2c_cmd(0x80);		// Display Clock Divide Ratio / OSC Frequency 
	ssd_i2c_cmd(0xA8);		// Set Multiplex Ratio
	ssd_i2c_cmd(0x3F);		// Multiplex Ratio for 128x64 (64-1)
	ssd_i2c_cmd(0xD3);		 // Set Display Offset
	ssd_i2c_cmd(0x00);		// Display Offset
	ssd_i2c_cmd(0x40);		 // Set Display Start Line
	ssd_i2c_cmd(0x8D);		 // Set Charge Pump
	ssd_i2c_cmd(0x14);		// Charge Pump (0x10 External, 0x14 Internal DC/DC)
	ssd_i2c_cmd(0xA1);		// Set Segment Re-Map
	ssd_i2c_cmd(0xC8);		// Set Com Output Scan Direction
	ssd_i2c_cmd(0xDA);		// Set COM Hardware Configuration
	ssd_i2c_cmd(0x12);		// COM Hardware Configuration
	ssd_i2c_cmd(0x81);		// Set Contrast
	ssd_i2c_cmd(0x0F);		// Contrast
	ssd_i2c_cmd(0xD9);		// Set Pre-Charge Period
	ssd_i2c_cmd(0xF1);		// Set Pre-Charge Period (0x22 External, 0xF1 Internal)
	ssd_i2c_cmd(0xDB);		// Set VCOMH Deselect Level
	ssd_i2c_cmd(0x40);		// VCOMH Deselect Level
	ssd_i2c_cmd(0xA4);		// Set all pixels OFF
	ssd_i2c_cmd(0xA6);		// Set display not inverted
	ssd_i2c_cmd(0xAF);		// Set display On
}

void ssd_i2c_cmd (char cmd){
	
	start();	//START--> I2C
	write_i2c(SSD1306_ADDRW);
	write_i2c(0x00);
	//I2C address writing 
	write_i2c(cmd);
	
	stop();	//STOP--> I2C
	_delay_ms(2);
	_delay_ms(2);
}

void ssd_write(uint8_t data){
	
	start();	//START--> I2C
	write_i2c(SSD1306_ADDRW);
	//I2C address writing
	write_i2c(0x40); //Set Display RAM line 
	write_i2c(data);
	stop();
	_delay_ms(2);
	_delay_ms(2);
}

void set_cursor(uint8_t x, uint8_t y){
	if((x<=0x07u) & (y<127)){
		
		ssd_i2c_cmd(0x21); //Set column address range 0-127d
		ssd_i2c_cmd(y);
		ssd_i2c_cmd(127);
		
		ssd_i2c_cmd(0x22); //Set page address, range 0-7d
		ssd_i2c_cmd(x);
		ssd_i2c_cmd(7);
		
		start();
		write_i2c(SSD1306_ADDRW);
		write_i2c(0x40); //Set Display RAM line 
	}
}
const unsigned char volt[]={
0x1F, 0x20, 0x40, 0x20, 0x1F,0x00}; // V
void graphic(void){
	const unsigned char up[] = {0x80,0x80,0x40,0x60,0xF8,0x60,0x40,0x80};
		ssd_write(0x00); //Space
		ssd_write(0x00);
	for( int r = 0; r < 21; r++){
		ssd_write(volt[r]);
	}
	set_cursor(0,10); //Arrow Graph 
	for( int s = 0; s < 8; s++){
		ssd_write(up[s]);
	}
	
	for(uint8_t i=1;i<8;i++){ // Y axis 
		set_cursor(i,13);
		for(int c = 0; c<2; c++){
			ssd_write(0xFF);
		}
	}
	
	for(int u = 1; u<90; u++){ // X axis 
			ssd_write(0x40);
		}
	ssd_write(0x48); // Second Arrow
	ssd_write(0x70);
	ssd_write(0xE0);
	ssd_write(0x40);
	ssd_write(0x00);
	ssd_write(0x00);
	
	static unsigned char LF[] ={
		0x48, 0x54, 0x54, 0x54, 0x20,0x00,0x00,   // sec
	    0x38, 0x54, 0x54, 0x54, 0x18,0x00,0x00,
		0x38, 0x44, 0x44, 0x44, 0x20,0x00,0x00
	};
	for( int r = 0; r < 21; r++){
		ssd_write(LF[r]);
	}
	
	write_adc();
}

void write_adc(void){
	int i;
	const unsigned char adc[] = { //ADC0 =
		0x7C, 0x12, 0x11, 0x12, 0x7C,0x00,0x00,   // A
		0x7F, 0x41, 0x41, 0x22, 0x1C,0x00,0x00,   // D
		0x3E, 0x41, 0x41, 0x41, 0x22,0x00,0x00   // C
	};
	const unsigned char num0[] = {
		0x3E, 0x51, 0x49, 0x45, 0x3E,0x00,0x00,  // 0
		0x14, 0x14, 0x14, 0x14, 0x14,0x00,0x00   // =
	};
	const unsigned char num1[] = {
		0x00, 0x42, 0x7F, 0x40, 0x00,0x00,0x00,   // 1
		0x14, 0x14, 0x14, 0x14, 0x14,0x00,0x00    // =
	};
	const unsigned char num2[] = {
		0x42, 0x61, 0x51, 0x49, 0x46,0x00,0x00,  // 2
		0x14, 0x14, 0x14, 0x14, 0x14,0x00,0x00   // =
	};
	//type in the screen : ADC0
	set_cursor(0,64);
	ssd_write(0x7F);
	for( int r = 0; r < 21; r++){
		ssd_write(adc[r]);
	}
	for( i = 0; i < 14 ; i++){
		ssd_write(num0[i]);
	}
	//type in the screen : ADC1
	set_cursor(1,64);
	ssd_write(0x7F);
	for( int r = 0; r < 21; r++){
		ssd_write(adc[r]);
	}
	for( i = 0; i < 14 ; i++){
		ssd_write(num1[i]);
	}
	//type in the screen : ADC2
	set_cursor(2,64);
	ssd_write(0x7F);
	for( int r = 0; r < 21; r++){
		ssd_write(adc[r]);
	}
	for( i = 0; i < 14 ; i++){
		ssd_write(num2[i]);
	}
	
}


//Numbers to display on the screen
const unsigned char n0[]={
0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00}; //0
const unsigned char n1[]={
0x00, 0x42, 0x7F, 0x40, 0x00, 0x00}; //1 
const unsigned char n2[]={
0x42, 0x61, 0x51, 0x49, 0x46, 0x00}; //2
const unsigned char n3[]={
0x21, 0x41, 0x45, 0x4B, 0x31, 0x00}; //3 
const unsigned char n4[]={
0x18, 0x14, 0x12, 0x7F, 0x10, 0x00}; //4
const unsigned char n5[]={
0x27, 0x45, 0x45, 0x45, 0x39, 0x00}; //5
const unsigned char n6[]={
0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00}; //6
const unsigned char n7[]={
0x01, 0x71, 0x09, 0x05, 0x03, 0x00}; //7 
const unsigned char n8[]={
0x36, 0x49, 0x49, 0x49, 0x36, 0x00}; //8
const unsigned char n9[]={
0x06, 0x49, 0x49, 0x29, 0x1E, 0x00}; //9
const unsigned char dot[]={
0x00, 0x60, 0x60, 0x00, 0x00, 0x00}; //.

   


void ssd_num_write(char *dato, char adc_select){
	char i = 0; 
	// This function simply changes the writing position of the voltage to be displayed on each adc
	if(adc_select==1){
		set_cursor(0,99);
		ssd_write(0x00);
	}else if(adc_select==2){
		set_cursor(1,99);
		ssd_write(0x00);
	}else if(adc_select==3){
		set_cursor(2,99);
		ssd_write(0x00);
	}
	
	for(i=0; i<4; i++){
		switch(dato[i]){
			case '0':
			for( char r = 0; r < 6; r++){
				ssd_write(n0[r]);
			}
					break;
			case '1':
			for( char r = 0; r < 6; r++){
				ssd_write(n1[r]);
			}
					break;
			case '2':
			for( char r = 0; r < 6; r++){
				ssd_write(n2[r]);
			}
					break;
			case '3':
			for( char r = 0; r < 6; r++){
				ssd_write(n3[r]);
			}
					break;
			case '4':
			for( char r = 0; r < 6; r++){
				ssd_write(n4[r]);
			}
					break;
			case '5':
			for( char r = 0; r < 6; r++){
				ssd_write(n5[r]);
			}
					break;
			case '6':
			for( char r = 0; r < 6; r++){
				ssd_write(n6[r]);
			}
					break;
			case '7':
			for( char r = 0; r < 6; r++){
				ssd_write(n7[r]);
			}
					break;
			case '8':
			for( char r = 0; r < 6; r++){
				ssd_write(n8[r]);
			}
					break;
			case '9':
			for( char r = 0; r < 6; r++){
				ssd_write(n9[r]);
			}
					break;
			case '.':
			for( char r = 0; r < 6; r++){
				ssd_write(dot[r]);
			}
					break;
		}
	}
	for( char r = 0; r < 6; r++){
	ssd_write(volt[r]);}
}



void clear_graph(char status){
	if(status == 1){
		int i,j;
		for(i=0; i<3; i++){
			set_cursor(6-i,15);
			for(j=0;j<90;j++){
				ssd_write(0x00);
			}
		}	
	}
}

void graph_volt(float voltage, char loc, char lvl){ //El loc me lo define el contador que se encuentra en adc.c 
	loc = loc + 15; 
	set_cursor(3+lvl,loc);
	ssd_write(0x00);
	if( voltage > 0.0 && voltage < 0.6 ){
		ssd_write(0x80);
	}
	else if( voltage > 0.6 && voltage < 1.2){
		ssd_write(0x40);
	}
	else if( voltage > 1.2 && voltage < 1.8){
		ssd_write(0x20);
	}
	else if( voltage > 1.8 && voltage < 2.5){
		ssd_write(0x10);
	}
	else if( voltage > 2.5 && voltage < 3.1){
		ssd_write(0x08);
	}
	else if( voltage > 3.1 && voltage < 3.7){
		ssd_write(0x04);
	}
	else if( voltage > 3.7 && voltage < 4.3){
		ssd_write(0x02);
	}
	else if( voltage > 4. && voltage < 6){
		ssd_write(0x01);
	}

}