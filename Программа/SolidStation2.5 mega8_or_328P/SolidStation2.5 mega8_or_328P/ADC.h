/*
 * ADC.h
 *
 * Created: 29.07.2018 14:08:41
 *  Author: Artur
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef ARDUINO_INCLUDE
#include <Arduino.h>
#endif
//#include "main.h"

#define D_ADC_Prescaler_2 0b1
#define D_ADC_Prescaler_4 0b10
#define D_ADC_Prescaler_8 0b11
#define D_ADC_Prescaler_16 0b100
#define D_ADC_Prescaler_32 0b101
#define D_ADC_Prescaler_64 0b110
#define D_ADC_Prescaler_128 0b111
#define D_ADC_Channel_0 0b0
#define D_ADC_Channel_1 0b1
#define D_ADC_Channel_2 0b10
#define D_ADC_Channel_3 0b11
#define D_ADC_Channel_4 0b100
#define D_ADC_Channel_5 0b101
#define D_ADC_Channel_6 0b110
#define D_ADC_Channel_7 0b111
//#define ADC_FreeRunning_Enable ADCSRA |= (1<<ADFR)
//#define ADC_FreeRunning_Disable ADCSRA &= ~(1<<ADFR)
#if defined(__AVR_ATmega168__) \
|| defined(__AVR_ATmega328__) \
|| defined(__AVR_ATmega328P__) \
|| defined(__AVR_ATmega8__) \
|| defined(__AVR_ATmega8A__)
#define ADC_Interrupt_Enable ADCSRA |= (1<<ADIE)
#define ADC_Interrupt_Disable ADCSRA &= ~(1<<ADIE)
#define ADC_Enable ADCSRA |= (1<<ADEN)
#define ADC_Disable ADCSRA &= ~(1<<ADEN)
#define ADC_Start ADCSRA |= (1<<ADSC)
#define ADC_Stop ADCSRA &= ~(1<<ADSC)
#define AREF ADMUX &= ~((1<<REFS1)|(1<<REFS0))
#define AVCC ADMUX |= (1<<REFS0); ADMUX &= ~(1<<REFS1)
//#define AVCC ADMUX |= (1<<REFS1)
#define A2V56 ADMUX |= ((1<<REFS1)|(1<<REFS0))
#define ADC_Prescaler(Division_Factor) ADCSRA &= ~((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)); ADCSRA |= Division_Factor
#define ADC_SetChannel(Channel) ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0)); ADMUX |= Channel
//void ADC_Init(void);
unsigned int analogRead (uint8_t adc);
/*
#elif defined(__AVR_ATmega8__) \
|| defined(__AVR_ATmega8A__) 

#define ADC_Interrupt_Enable ADCSR |= (1<<ADIE)
#define ADC_Interrupt_Disable ADCSR &= ~(1<<ADIE)
#define ADC_Enable ADCSR |= (1<<ADEN)
#define ADC_Disable ADCSR &= ~(1<<ADEN)
#define ADC_Start ADCSR |= (1<<ADSC)
#define ADC_Stop ADCSR &= ~(1<<ADSC)
#define AREF ADMUX &= ~((1<<REFS1)|(1<<REFS0))
#define AVCC ADMUX |= (1<<REFS0); ADMUX &= ~(1<<REFS1)
//#define AVCC ADMUX |= (1<<REFS1)
#define A2V56 ADMUX |= ((1<<REFS1)|(1<<REFS0))
#define ADC_Prescaler(Division_Factor) ADCSR &= ~((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)); ADCSR |= Division_Factor
#define ADC_SetChannel(Channel) ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0)); ADMUX |= Channel
unsigned int analogRead (uint8_t adc);
*/
#elif  defined(__AVR_ATtiny26__)\
|| defined(__AVR_ATtiny26L__)

#define ADC_Interrupt_Enable ADCSR |= (1<<ADIE)
#define ADC_Interrupt_Disable ADCSR &= ~(1<<ADIE)
#define ADC_Enable ADCSR |= (1<<ADEN)
#define ADC_Disable ADCSR &= ~(1<<ADEN)
#define ADC_Start ADCSR |= (1<<ADSC)
#define ADC_Stop ADCSR &= ~(1<<ADSC)
#define AVCC ADMUX &= ~((1<<REFS1)|(1<<REFS0))
#define AREF ADMUX |= (1<<REFS0); ADMUX &= ~(1<<REFS1)
//#define AVCC ADMUX |= (1<<REFS1)
#define A2V56 ADMUX |= ((1<<REFS1)|(1<<REFS0))
#define ADC_Prescaler(Division_Factor) ADCSR &= ~((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)); ADCSR |= Division_Factor
#define ADC_SetChannel(Channel) ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0)); ADMUX |= Channel
unsigned int analogRead (uint8_t adc);

#endif

#endif /* ADC_H_ */