/*
 * ADC.c
 *
 * Created: 29.07.2018 14:09:02
 *  Author: Artur
 */ 
 #include "ADC.h"
 #if defined(__AVR_ATmega168__) \
 || defined(__AVR_ATmega328__) \
 || defined(__AVR_ATmega328P__) \
 || defined(__AVR_ATmega8__) \
 || defined(__AVR_ATmega8A__) 
 unsigned int analogRead(uint8_t adc)
 {
	 ADC_SetChannel(adc);//�������� �����(�����) � ������� ����� ������� ���
	 ADCSRA |= (1<<ADSC); //�������� ��������������
	 while((ADCSRA & (1<<ADSC))); //�������� ����������� �� �������-�������� ��������������
	 return (unsigned int) ADC;
 }
 //typedef void (*PEmptyProcADC)(void);
 //void EmptyProcADC(void){};
 //PEmptyProc freeRun = &EmptyProcADC;
 //ISR(ADC_vect)
 //{
	 //freeRun();
 //}
 //void getADCVector(void (*adc_fr)())
 //{
	 //freeRun = adc_fr;
 //}
 
 #elif  defined(__AVR_ATtiny26__)\
 || defined(__AVR_ATtiny26L__)
 
 unsigned int analogRead(uint8_t adc)
 {
	 ADC_SetChannel(adc);//�������� �����(�����) � ������� ����� ������� ���
	 ADCSR |= (1<<ADSC); //�������� ��������������
	 while((ADCSR & (1<<ADSC))); //�������� ����������� �� �������-�������� ��������������
	 return (unsigned int) ADC;
 }
 
 #endif