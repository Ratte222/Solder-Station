/*
 * Interrupt.c
 *
 * Created: 08.08.2018 16:16:46
 *  Author: Artur
 */
 
 #include "interrupt.h"
extern void EmptyProc(void);
 
 #if defined(__AVR_ATmega168__) \
 || defined(__AVR_ATmega328__) \
 || defined(__AVR_ATmega328P__)
 
 PEmptyProc Interrupt0 = &EmptyProc;
 PEmptyProc Interrupt1 = &EmptyProc;
 ISR(INT0_vect)
 {
	 Interrupt0();
 }
 ISR(INT1_vect)
 {
	 Interrupt1();
 }
 
 void GetINT0_Vect(void (*int0)())
 {
	 Interrupt0 = int0;
 }
 
 void GetINT1_Vect(void (*int1)())
 {
	 Interrupt1 = int1;
 }
 
 #elif defined(__AVR_ATmega8__) \
 || defined(__AVR_ATmega8A__) 
 
 PEmptyProc Interrupt0 = &EmptyProc;
 PEmptyProc Interrupt1 = &EmptyProc;
 ISR(INT0_vect)
 {
	 Interrupt0();
 }
 ISR(INT1_vect)
 {
	 Interrupt1();
 }
 
 void GetINT0_Vect(void (*int0)())
 {
	 Interrupt0 = int0;
 }
 
 void GetINT1_Vect(void (*int1)())
 {
	 Interrupt1 = int1;
 }
 
 #endif