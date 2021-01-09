/*
 * Timers.h
 *
 * Created: 24.08.2018 16:30:59
 *  Author: User
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_
#include "main.h"

#define D_Prescaler_no 0b1
#define D_Prescaler_8 0b10
#define D_Prescaler_64 0b11
#define D_Prescaler_256 0b100
#define D_Prescaler_1024 0b101
#define D_falling_edge = 0b110
#define D_rising_edge = 0b111

#if defined(__AVR_ATmega168__) \
|| defined(__AVR_ATmega328__) \
|| defined(__AVR_ATmega328P__)

#define Timer0_Ovf_Ena BitSet(TIMSK0,TOIE0)
#define Timer0_Ovf_Dis BitClear(TIMSK0,TOIE0)
#define Timer0_Stop TCCR0B &= ~((1<<CS00)|(1<<CS01)|(1<<CS02))
#define Timer0_Restart TCNT0 = 0
#define Timer0_SetPrescaler(val) TCCR0B &= ~((1<<CS00)|(1<<CS01)|(1<<CS02)); TCCR0B |= val
//void Timer0_init(void);
void GetTimer0_OVF_Vect(void (*isr00)());
//#define GetTimer0_OVF_Vect(void (*isr0)()) TIMER0_OVF = isr0


#define RESOLUTION 65536    // Timer1 is 16 bit
#define Timer1_Ovf_Ena BitSet(TIMSK1,TOIE1)
#define Timer1_Ovf_Dis BitClear(TIMSK1,TOIE1)
#define Timer1_COMPA_Ena BitSet(TIMSK1,OCIE1A)
#define Timer1_COMPA_Dis BitClear(TIMSK1,OCIE1A)
#define Timer1_COMPB_Ena BitSet(TIMSK1,OCIE1B)
#define Timer1_COMPB_Dis BitClear(TIMSK1,OCIE1B)
#define Timer1_Stop TCCR1B &= ~((1<<CS10)|(1<<CS11)|(1<<CS12))
#define Timer1_Restart TCNT1 = 0
#define Timer1_SetPrescaler(val) TCCR1B &= ~((1<<CS10)|(1<<CS11)|(1<<CS12)); TCCR1B |= val
#define CompareReg_1A OCR1A
#define CompareReg_1B OCR1B
//void Timer1_init(void);
void GetTimer1_OVF_Vect(void (*isr10)());
void GetTimer1_COMPA_Vect(void (*isr11)());
void GetTimer1_COMPB_Vect(void (*isr12)());

#define Timer2_Ovf_Ena BitSet(TIMSK2,TOIE2)
#define Timer2_Ovf_Dis BitClear(TIMSK2,TOIE2)
#define Timer2_COMPA_Ena BitSet(TIMSK2,OCIE2A)
#define Timer2_COMPA_Dis BitClear(TIMSK2,OCIE2A)
#define Timer2_COMPB_Ena BitSet(TIMSK2,OCIE2B)
#define Timer2_COMPB_Dis BitClear(TIMSK2,OCIE2B)
#define Timer2_Stop TCCR2B &= ~((1<<CS20)|(1<<CS21)|(1<<CS22))
#define Timer2_Restart TCNT2 = 0
#define Timer2_SetPrescaler(val) TCCR2B &= ~((1<<CS20)|(1<<CS21)|(1<<CS22)); TCCR2B |= val
#define Timer2_Waveform_Generation_Mode(val) TCCR2B &= ~(0b01001000); TCCR2B |= val //Режим генерации формы волны
#define CompareReg_2A OCR2A
#define CompareReg_2B OCR2B
//void Timer2_init(void);
void GetTimer2_OVF_Vect(void (*isr20)());
void GetTimer2_COMPA_Vect(void (*isr21)());
void GetTimer2_COMPB_Vect(void (*isr22)());

#elif defined(__AVR_ATmega8__) \
|| defined(__AVR_ATmega8A__) 

#define Timer0_Ovf_Ena BitSet(TIMSK,TOIE0)
#define Timer0_Ovf_Dis BitClear(TIMSK,TOIE0)
#define Timer0_Stop TCCR0 &= ~((1<<CS00)|(1<<CS01)|(1<<CS02))
#define Timer0_Restart TCNT0 = 0
#define Timer0_SetPrescaler(val) TCCR0 &= ~((1<<CS00)|(1<<CS01)|(1<<CS02)); TCCR0 |= val
void GetTimer0_OVF_Vect(void (*isr00)());
//#define GetTimer0_OVF_Vect(void (*isr0)()) TIMER0_OVF = isr0


#define RESOLUTION 65536    // Timer1 is 16 bit
#define Timer1_Ovf_Ena BitSet(TIMSK,TOIE1)
#define Timer1_Ovf_Dis BitClear(TIMSK,TOIE1)
#define Timer1_COMPA_Ena BitSet(TIMSK,OCIE1A)
#define Timer1_COMPA_Dis BitClear(TIMSK,OCIE1A)
#define Timer1_COMPB_Ena BitSet(TIMSK,OCIE1B)
#define Timer1_COMPB_Dis BitClear(TIMSK,OCIE1B)
#define Timer1_Stop TCCR1B &= ~((1<<CS10)|(1<<CS11)|(1<<CS12))
#define Timer1_Restart TCNT1 = 0
#define Timer1_SetPrescaler(val) TCCR1B &= ~((1<<CS10)|(1<<CS11)|(1<<CS12)); TCCR1B |= val
#define CompareReg_1A OCR1A
#define CompareReg_1B OCR1B
void GetTimer1_OVF_Vect(void (*isr10)());
void GetTimer1_COMPA_Vect(void (*isr11)());
void GetTimer1_COMPB_Vect(void (*isr12)());

#define Timer2_Ovf_Ena BitSet(TIMSK,TOIE2)
#define Timer2_Ovf_Dis BitClear(TIMSK,TOIE2)
#define Timer2_COMP_Ena BitSet(TIMSK,OCIE2)
#define Timer2_COMP_Dis BitClear(TIMSK,OCIE2)
#define Timer2_Stop TCCR2 &= ~((1<<CS20)|(1<<CS21)|(1<<CS22))
#define Timer2_Restart TCNT2 = 0
#define Timer2_SetPrescaler(val) TCCR2 &= ~((1<<CS20)|(1<<CS21)|(1<<CS22)); TCCR2 |= val
#define Timer2_Waveform_Generation_Mode(val) TCCR2 &= ~(0b01001000); TCCR2 |= val //Режим генерации формы волны
#define CompareReg_2 OCR2
void GetTimer2_OVF_Vect(void (*isr20)());
void GetTimer2_COMP_Vect(void (*isr21)());

#endif



//void Timer0_Ovf_Ena(void);
//void Timer0_Ovf_Dis(void);
//void Timer0_restart(void);
//void Timer0_init(word value, void (*isr)());
//void Timer2_init(word value, void (*isr)());
//void Timer2_Compare_interrupt_Ena()


#endif /* TIMERS_H_ */