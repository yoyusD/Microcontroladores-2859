/*
 * ssd_i2c.h
 *
 * Created: 24/03/2024 08:52:19 p. m.
 *  Author: guado
 */ 


#ifndef SSD_I2C_H_
#define SSD_I2C_H_
#define SSD1306_ADDRW	0x78		//Dirección Escritura
#define SSD1306_ADDRR	0x79		//Dirección Lectura
#include <avr/io.h>
void ssd_i2c_init (void);
void ssd_i2c_cmd (char cmd);
void ssd_write(uint8_t data);
void set_cursor(uint8_t x, uint8_t y);
void graphic(void);
void write_adc(void);
void ssd_num_write(char dato, char pwn_select,int position);
void graph_volt(float voltage, char loc);
void clear_graph(void);
void clear_screen(void);
void write_lcd_state(char data);
void write_led(void);




#endif /* SSD_I2C_H_ */