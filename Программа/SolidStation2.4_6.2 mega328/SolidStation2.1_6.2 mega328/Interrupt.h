/*
 * Interrupt.h
 *
 * Created: 08.08.2018 16:16:19
 *  Author: Artur
 */ 


#ifndef INTERRUPT_H_
#define INTERRUPT_H_
#include "main.h"

#if defined(__AVR_ATmega168__) \
|| defined(__AVR_ATmega328__) \
|| defined(__AVR_ATmega328P__)

#define Int0_LOW EICRA &= ~((1<<ISC01)|(1<<ISC00))
#define Int0_CHANGE EICRA &= ~(1<<ISC01); MCUCR |= (1<<ISC00)
#define Int0_FALLING EICRA |= (1<<ISC01); MCUCR &= ~(1<<ISC00)
#define Int0_RISING EICRA |= ((1<<ISC01)|(1<<ISC00))
#define Int1_LOW EICRA &= ~((1<<ISC11)|(1<<ISC10))
#define Int1_CHANGE EICRA &= ~(1<<ISC11); MCUCR |= (1<<ISC10)
#define Int1_FALLING EICRA |= (1<<ISC11); MCUCR &= ~(1<<ISC10)
#define Int1_RISING EICRA |= ((1<<ISC11)|(1<<ISC10))
#define attachInterrupt0 EIMSK |= (1<<INT0)
#define detachInterrupt0 EIMSK &= ~(1<<INT0)
#define attachInterrupt1 EIMSK |= (1<<INT1)
#define detachInterrupt1 EIMSK &= ~(1<<INT1)

void GetINT0_Vect(void (*int0)());
void GetINT0_Vect(void (*int1)());

#elif defined(__AVR_ATmega8__) \
|| defined(__AVR_ATmega8A__) 

#define Int0_LOW MCUCR &= ~((1<<ISC01)|(1<<ISC00))
#define Int0_CHANGE MCUCR &= ~(1<<ISC01); MCUCR |= (1<<ISC00)
#define Int0_FALLING MCUCR |= (1<<ISC01); MCUCR &= ~(1<<ISC00)
#define Int0_RISING MCUCR |= ((1<<ISC01)|(1<<ISC00))
#define Int1_LOW MCUCR &= ~((1<<ISC11)|(1<<ISC10))
#define Int1_CHANGE MCUCR &= ~(1<<ISC11); MCUCR |= (1<<ISC10)
#define Int1_FALLING MCUCR |= (1<<ISC11); MCUCR &= ~(1<<ISC10)
#define Int1_RISING MCUCR |= ((1<<ISC11)|(1<<ISC10))
#define attachInterrupt0 GICR |= (1<<INT0)
#define detachInterrupt0 GICR &= ~(1<<INT0)
#define attachInterrupt1 GICR |= (1<<INT1)
#define detachInterrupt1 GICR &= ~(1<<INT1)

void GetINT0_Vect(void (*int0)());
void GetINT0_Vect(void (*int1)());

#endif



//void attachInterrupt(byte j, void (*inter)(), byte mod);
//void detachInterrupt(byte j);

#endif /* INTERRUPT_H_ */