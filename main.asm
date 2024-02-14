;
; AssemblerApplication2.asm
;
; Created: 10/02/2024 07:48:01 p. m.
; Author : Alexis Horacio López Fragoso
;


; Replace with your application code

.cseg
.org 0x00


ldi r16,high(RAMEND)
out SPH,r16
ldi r16,low(RAMEND)
out SPL,r16

ldi r16,0b00100000 ;PB5 output, the other pins as inputs
out DDRB,r16

ldi r16,0
sts TCCR1A,r16
ldi r16, 0b00000100 ;Prescale 256
sts TCCR1B,r16

;fosc = 16M/256 = 62,500 Hz

;1Hz
;counter = 500m*62,500 = 31,250
;so 31250/31250 = 1. Requires 31,250 = 0x7A12

;2.5Hz
;counter = 200m*62,500 = 12,500
;so 12,500/12,500 = 1. Requires 12,500 = 0x30D4

;50 Hz
;counter = 10m*62,500 = 625
;so 625/625 = 1. Requires 625, for more accurate 624 = 0x0270

;10k Hz
;counter = 50u*62,500 = 3.125
;so 3.125/3 = 1.04. Requires 3 = 0x0003

start:
;here I use the pins PB0 and PB1 as inputs
; PB0 |	PB1	| OUT	|
;	0 |	 0	| 1HZ	|	
;	0 |	 1	| 2.5HZ	|
;	1 |	 0	| 50HZ  |
;	1 |	 1	| 10KHZ |
	in r16,PINB
	cpi r16,0b00000000
	breq ONEHZ

	in r16,PINB
	cpi r16,0b00000001
	breq TWOFIVEHZ

	in r16,PINB
	cpi r16,0b00000010
	breq FIFTYHZ

	in r16,PINB
	cpi r16,0b00000011
	breq TENKHZ

	rjmp start


ONEHZ:
	sbi PORTB,PB5
	rcall delay1
	cbi PORTB,PB5
	rcall delay1
	rjmp start
delay1: 
	ldi r16,0
	ldi r17,0
	sts	TCNT1H,r17
	sts TCNT1L,r16
ciclo1:
	lds r16,TCNT1L
	cpi r16,0x12
	brne ciclo1
	lds r17,TCNT1H
	cpi r17,0x7A
	brne ciclo1
	ret 



TWOFIVEHZ:
	sbi PORTB,PB5
	rcall delay2
	cbi PORTB,PB5
	rcall delay2
	rjmp start
delay2: 
	ldi r16,0
	ldi r17,0
	sts	TCNT1H,r17
	sts TCNT1L,r16
ciclo2:
	lds r16,TCNT1L
	cpi r16,0xD4
	brne ciclo2
	lds r17,TCNT1H
	cpi r17,0x30
	brne ciclo2
	ret 

FIFTYHZ:
	sbi PORTB,PB5
	rcall delay3
	cbi PORTB,PB5
	rcall delay3
	rjmp start
delay3: 
	ldi r16,0
	ldi r17,0
	sts	TCNT1H,r17
	sts TCNT1L,r16
ciclo3:
	lds r16,TCNT1L
	cpi r16,0x70
	brne ciclo3
	lds r17,TCNT1H
	cpi r17,0x02
	brne ciclo3
	ret 

TENKHZ:
	sbi PORTB,PB5
	rcall delay4
	cbi PORTB,PB5
	rcall delay4
	rjmp start
delay4: 
	ldi r16,0
	ldi r17,0
	sts	TCNT1H,r17
	sts TCNT1L,r16
ciclo4:
	lds r16,TCNT1L
	cpi r16,0x03
	brne ciclo4
	lds r17,TCNT1H
	cpi r17,0x00
	brne ciclo4
	ret 


