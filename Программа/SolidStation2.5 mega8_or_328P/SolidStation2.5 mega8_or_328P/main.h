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
#include <avr/interrupt.h>//вкл/выкл глобальные прерывания
#include <avr/eeprom.h>//запись/чтение данных в энергонезависимую память
#include <avr/wdt.h>//сторожевой таймер
#include <util/delay.h>//задержка
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>//запись/чтение данных в флеш-память
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


#define p 0 // паяльник
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

#endif /* MAIN_H_ */