/*
 * Timers.cpp
 *
 * Created: 24.08.2018 16:31:16
 *  Author: User
 */ 
#include "Timers.h"
extern void EmptyProc(void);

#if defined(__AVR_ATmega168__) \
|| defined(__AVR_ATmega328__) \
|| defined(__AVR_ATmega328P__)

PEmptyProc TIMER0_OVF = &EmptyProc;
PEmptyProc TIMER1_OVF = &EmptyProc;
PEmptyProc TIMER1_CompareA = &EmptyProc;
PEmptyProc TIMER1_CompareB = &EmptyProc;
PEmptyProc TIMER2_OVF = &EmptyProc;
PEmptyProc TIMER2_CompareA = &EmptyProc;
PEmptyProc TIMER2_CompareB = &EmptyProc;
ISR(TIMER0_OVF_vect)
{
	TIMER0_OVF();
	//time++;
}
ISR(TIMER1_OVF_vect)
{
	TIMER1_OVF();
}
ISR(TIMER1_COMPA_vect)
{
	TIMER1_CompareA();
}
ISR(TIMER1_COMPB_vect)
{
	TIMER1_CompareB();
}
ISR(TIMER2_OVF_vect)
{
	TIMER2_OVF();
}
ISR(TIMER2_COMPA_vect)
{
	TIMER2_CompareA();
}
ISR(TIMER2_COMPB_vect)
{
	TIMER2_CompareB();
}

void GetTimer0_OVF_Vect(void (*isr00)())
{
	TIMER0_OVF = isr00;
}

void GetTimer1_OVF_Vect(void (*isr10)())
{
	TIMER1_OVF = isr10;
}

void GetTimer1_COMPA_Vect(void (*isr11)())
{
	TIMER1_CompareA = isr11;
}

void GetTimer1_COMPB_Vect(void (*isr12)())
{
	TIMER1_CompareB = isr12;
}

void GetTimer2_OVF_Vect(void (*isr20)())
{
	TIMER2_OVF = isr20;
}

void GetTimer2_COMPA_Vect(void (*isr21)())
{
	TIMER2_CompareA = isr21;
}

void GetTimer2_COMPB_Vect(void (*isr22)())
{
	TIMER2_CompareB = isr22;
}

#elif defined(__AVR_ATmega8__) \
|| defined(__AVR_ATmega8A__) 

PEmptyProc TIMER0_OVF = &EmptyProc;
PEmptyProc TIMER2_OVF = &EmptyProc;
PEmptyProc TIMER2_Compare = &EmptyProc;
PEmptyProc TIMER1_OVF = &EmptyProc;
PEmptyProc TIMER1_CompareA = &EmptyProc;
PEmptyProc TIMER1_CompareB = &EmptyProc;
ISR(TIMER0_OVF_vect)
{
	TIMER0_OVF();
	//time++;
}
ISR(TIMER1_OVF_vect)
{
	TIMER1_OVF();
}
ISR(TIMER1_COMPA_vect)
{
	TIMER1_CompareA();
}
ISR(TIMER1_COMPB_vect)
{
	TIMER1_CompareB();
}
ISR(TIMER2_OVF_vect)
{
	TIMER2_OVF();
}
ISR(TIMER2_COMP_vect)
{
	TIMER2_Compare();
}

void GetTimer0_OVF_Vect(void (*isr00)())
{
	TIMER0_OVF = isr00;
}

void GetTimer1_OVF_Vect(void (*isr10)())
{
	TIMER1_OVF = isr10;
}

void GetTimer1_COMPA_Vect(void (*isr11)())
{
	TIMER1_CompareA = isr11;
}

void GetTimer1_COMPB_Vect(void (*isr12)())
{
	TIMER1_CompareB = isr12;
}

void GetTimer2_OVF_Vect(void (*isr20)())
{
	TIMER2_OVF = isr20;
}

void GetTimer2_COMP_Vect(void (*isr21)())
{
	TIMER2_Compare = isr21;
}

#endif


