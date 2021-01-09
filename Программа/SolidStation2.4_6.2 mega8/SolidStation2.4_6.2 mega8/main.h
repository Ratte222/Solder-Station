/*
 * main.h
 *
 * Created: 29.07.2018 14:12:23
 *  Author: Artur
 */ 


#ifndef MAIN_H_
#define MAIN_H_
#define F_CPU 16000000UL
#define BitSet(xByte,yBit) (xByte|=(1<<yBit))
#define BitClear(xByte,yBit) (xByte&= ~(1<<yBit))
#define BitToggle(xByte,yBit) (xByte^=(1<<yBit))
//#define BitFlip(xByte,yBit) (xByte^=(1<<yBit))
#define BitTest(xByte,yBit)  (xByte&(1<<yBit))

#include <avr/io.h>
#include <avr/interrupt.h>//вкл/выкл глобальные прерывани€
#include <avr/eeprom.h>//запись/чтение данных в энергонезависимую пам€ть
#include <avr/wdt.h>//сторожевой таймер
#include <util/delay.h>//задержка
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>//запись/чтение данных в флеш-пам€ть
//#include <sfr_defs.h>

typedef uint8_t boolean;
typedef uint8_t byte;
typedef uint16_t word;
typedef signed long integer;
typedef unsigned long dword;
typedef void (*PEmptyProc)(void);

//#define Interrupt_en SREG |= (1<<I);
//#define Interrupt_dis SREG &=~(1<<I);

#include "ADC.h"
#include "Timers.h"
#include "Button.h"
#include "SPI.h"
#include "Interrupt.h"
#include "PID_Regulator.h"
//#include "ShiftOut.h"

#define Version 1//если верси€ == 1 то дл€ кнопок нужно припа€ть подт€гивающий резистор, если 3+ - нет
//версию поставить в 2, когда использую SPI
//#define Stripped_down
#define p 0 // па€льник
#define f 1 //фен
#define Max_step_ADC 1
#define Default_Factor_p 75
#define Default_Factor_f 125
#define Default_Delta_p 15
#define Default_Delta_f 20
#define Mosfet_on PORTD |= (1<<PD4)
#define Mosfet_off PORTD &= ~(1<<PD4)
#define Fan_enable PORTD |= (1<<PD3)
#define Fan_disable PORTD &= ~(1<<PD3)
#define Semister_on PORTD |= (1<<PD1)
#define Semister_off PORTD &= ~(1<<PD1)
#define st2 (1<<PB2)//latch
#define ds  (1<<PB3)//data
#define st1 (1<<PB4)//latch
#define sh  (1<<PB5)//clock
#define latch_1_on PORTB |= st1
#define latch_1_off PORTB &= ~st1
#define latch_2_on PORTB |= st2
#define latch_2_off PORTB &= ~st2
#define b_p 0//button p
#define b_f 1//button f
#define b_set 2//button set...
#define b_up 3
#define b_down 4
#define gerc 5// gercon
#define D_array_deb 0
#define D_array_maim_menu 1
#define D_array_reason_reset 2
#define Save_Data_in_ShiftRegister PORTB |= (1<<PORTB2); PORTB &= ~(1<<PORTB2)

#if defined(__AVR_ATmega8__) \
|| defined(__AVR_ATmega8A__) 
#define CompareReg_2A CompareReg_2
#define SREG _SFR_IO8(0x3F)
#endif

//#define dim  1// управление симистером
//#define zero 2//подключЄн дл€ поиска 0 точки
//#define fan  3// управление вентел€тором фена
//#define mosfet_p  4 //мосфет управл€ющий нагревателем па€льника
//#define st2 10
//#define ds  11
//#define st1 12
//#define sh  13
/*void Filter(void);
void Power_p(void);
void Screen(void);
void Setting(void);
void initAVR();
void timer_interrupt();
void detect_up();
void detect_down();
void shiftOut(byte Datapin, byte Clockpin, byte val, byte option);
*/

/*
byte cifri[30] = { 0b01011111, //0,O
	0b01000001,//1,i
	0b10011101,//2
	0b11010101,//3
	0b11000011,//4 4 element
	0b11010110,//5,s
	0b11011110,//6
	0b01000101,//7
	0b11011111,//8,g
	0b11010111,//9 9 element
	0b10000000,//-
	0b00000000,//пустота 11element
	0b00010000,//_ 12 element
	0b11001111,//A
	0b11011010,//b 14 element
	0b00011110,//C
	0b11011001,//d
	0b10011110,//E 17 element
	0b10001110,//F
	0b11001010,//h 19 element
	0b01010001,//J 20 element
	0b11001011,//H имеентс€ ввиду к
	0b00011010,//L 22 element
	//не знаю как изобразить "ћ"
	0b11001000,//n 23 element
	0b10001111,//P 24 element
	0b11000111,//q 25 element
	0b10001000,//r 26 element
	0b10011010,//t 27 element
	0b11010011,//y 28 element
	0b1011000//u 29 element

};
*/
/*
#if defined(__AVR_ATmega168__) \
|| defined(__AVR_ATmega328__) \
|| defined(__AVR_ATmega328P__)



#elif defined(__AVR_ATmega8__) \
|| defined(__AVR_ATmega8A__) \



#endif
*/
#endif /* MAIN_H_ */