/*
 * PID_Regulator.h
 *
 * Created: 11.10.2018 15:28:08
 *  Author: Администратор
 */ 


#ifndef PID_REGULATOR_H_
#define PID_REGULATOR_H_

#include "main.h"

class PID_Regulator
{
public:

PID_Regulator(float _Kp, float _Ki, float _Kd, float _dt);//котструктор
/*
_Kp - пропрорционатьный коэффициент
_Ki - интегральный коэффициент
_Kd - диффиринциальный коэффициент
_dt - в данном случаи время между вызовами ф-и PID_Calculation
увеличение пропорционального коэффициента увеличивает быстродействие и снижает запас устойчивости;
с уменьшением интегральной составляющей ошибка регулирования с течением времени уменьшается быстрее;
уменьшение постоянной интегрирования уменьшает запас устойчивости;
увеличение дифференциальной составляющей увеличивает запас устойчивости и быстродействие.
*/
long PID_Calculation(int *required, int *now);
void Reset_Value(void);//естьсмысл сбрасывать, если будет происходить несколько отдельных запусков с нуля
//например вкл паяльник, попользовался, выкл, (1) он остыл и опять вкл. там где 1 нужно вызвать ф-ю Reset_Value
//float u = 0.0001;//сигнал управления - выход
//float e0 = 0.000001, e1 = 0.000001;//текущий и предыдущий такт ошибки
//const float Kp = 3, Ki = 2.5, Kd = 1.5;//коефициетн пропорциональности, время интегрирования, время дифференцироваия
//float e = 0.000001;//текущее значение ошибки
//static float integral = 0.000001;//интегральная составляющая
//float dif = 0.000001;//дифференциальная оставляющая
//float SP = 500.0;//обратная связь - термопара
//const float dt = 0.0163;//время между расчетами
//e = (float)required_t[f] - (float)real_t[f];//вот расчет ошибки
//e0 = e1;
//e1 = e;
//integral += ((1.0/Ki)*e1*dt);
//dif = Kd*(e1 - e0)/dt;
//u = (Kp*e1) + integral + dif;


protected:

private:
float Kp, Ki, Kd;//коефициетн пропорциональности, время интегрирования, время дифференцироваия
float dt;//время между расчетами
float e0, e1;//текущий и предыдущий такт ошибки
float e;//текущее значение ошибки
float integral;//интегральная составляющая
float dif;//дифференциальная оставляющая
};


#endif /* PID REGULATOR_H_ */


/*
пример с ПИД-регулятором
#include <mega328p.h>
#include <delay.h>
float u = 0.0001;//сигнал управления - выход
float e0 = 500.0, e1 = 0.000001;//текущий и предыдущий такт ошибки 
float Kp = 0.04, Ki = 0.055, Kd = 0.00625;//коефициетн пропорциональности, время интегрирования, время дифференцироваия
float e = 0.000001;//текущее значение ошибки
float integral = 0.000001;//интегральная составляющая
float dif = 0.000001;//дифференциальная оставляющая
float SP = 500.0;//обратная связь - термопара	
const float dt = 0.032768;//время между расчетами
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
	e0 = e1;
	e1 = e;
	integral += ((1.0/Ki)*e1*dt);
	dif = Kd*(e1 - e0)/dt;
	u = (Kp*e1) + integral + dif;
}
#define FIRST_ADC_INPUT 0
#define LAST_ADC_INPUT 0
unsigned int adc_data[LAST_ADC_INPUT-FIRST_ADC_INPUT+1];
#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))
interrupt [ADC_INT] void adc_isr(void)
{
	static unsigned char input_index=0;
	adc_data[input_index]=ADCW;
	if (++input_index > (LAST_ADC_INPUT-FIRST_ADC_INPUT))
	input_index=0;
	ADMUX=(FIRST_ADC_INPUT | ADC_VREF_TYPE)+input_index;
	delay_us(10);
	ADCSRA|=(1<<ADSC);
}

void main(void)
{
	#pragma optsize-
	CLKPR=(1<<CLKPCE);
	CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
	#ifdef _OPTIMIZE_SIZE_
	#pragma optsize+
	#endif
	DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (1<<DDB1) | (0<<DDB0);
	PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);
	
	TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (0<<WGM00);
	TCCR0B=(0<<WGM02) | (1<<CS02) | (0<<CS01) | (1<<CS00);
	TCNT0=0x00;
	OCR0A=0x00;
	OCR0B=0x00;
	
	TCCR1A=(1<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (1<<WGM11) | (1<<WGM10);
	TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
	TCNT1H=0x00;
	TCNT1L=0x00;
	ICR1H=0x00;
	ICR1L=0x00;
	OCR1AH=0x00;
	OCR1AL=0x00;
	OCR1BH=0x00;
	OCR1BL=0x00;
	
	TIMSK0=(0<<OCIE0B) | (0<<OCIE0A) | (1<<TOIE0);
	
	DIDR0=(0<<ADC5D) | (0<<ADC4D) | (0<<ADC3D) | (0<<ADC2D) | (0<<ADC1D) | (0<<ADC0D);
	ADMUX=FIRST_ADC_INPUT | ADC_VREF_TYPE;
	ADCSRA=(1<<ADEN) | (1<<ADSC) | (0<<ADATE) | (0<<ADIF) | (1<<ADIE) | (0<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	ADCSRB=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);
	#asm("sei")
	
	while (1)
	{
		e = SP - (float)adc_data[0];//вот расчет ошибки
		if ( u  <  0.0     )                          {OCR1AL = 0x00;               OCR1AH = 0x00;}
		if ((u  >= 0.0     ) && (u <= 255.999 ))      {OCR1AL = (int)u;                           }
		if ((u  >  255.999 ) && (u <= 511.999 ))      {OCR1AL = (int)(u - 255.999); OCR1AH = 0x01;}
		if ((u  >  511.999 ) && (u <= 767.999 ))      {OCR1AL = (int)(u - 511.999); OCR1AH = 0x02;}
		if ((u  >  767.999 ) && (u <= 1023.999))      {OCR1AL = (int)(u - 767.999); OCR1AH = 0x03;}
		if ( u  >  1023.999)                          {OCR1AL = 0xFF;               OCR1AH = 0x03;}
	}
}
*/