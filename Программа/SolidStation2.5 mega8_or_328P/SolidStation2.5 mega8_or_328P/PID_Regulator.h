﻿/*
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

private:
float Kp, Ki, Kd;//коефициетн пропорциональности, время интегрирования, время дифференцироваия
float dt;//время между расчетами
float e0, e1;//текущий и предыдущий такт ошибки
float e;//текущее значение ошибки
float integral;//интегральная составляющая
float dif;//дифференциальная оставляющая
};


#endif /* PID REGULATOR_H_ */