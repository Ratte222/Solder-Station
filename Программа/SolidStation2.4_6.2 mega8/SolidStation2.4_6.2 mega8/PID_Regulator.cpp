/*
 * PID_Regulator.cpp
 *
 * Created: 11.10.2018 15:28:23
 *  Author: Администратор
 */ 
#include "PID_Regulator.h"

//PID_Regulator Dryer(3, 2.5, 1.5, 0.0163);

void PID_Regulator::Reset_Value(void)
{
	e0 = 0.000001;
	e1 = 0.000001;//текущий и предыдущий такт ошибки
	e = 0.000001;//текущее значение ошибки
	integral = 0.000001;//интегральная составляющая
	dif = 0.000001;//дифференциальная оставляющая
}

long PID_Regulator::PID_Calculation(int *required, int *now)
{
	e = (float)(*required - *now);//вот расчет ошибки
	e0 = e1;
	e1 = e;
	integral += ((1.0/Ki)*e1*dt);
	dif = Kd*(e1 - e0)/dt;
	return (long)((Kp*e1) + integral + dif);
}

PID_Regulator::PID_Regulator(float _Kp, float _Ki, float _Kd, float _dt)
{
	Kp = _Kp;
	Ki = _Ki;
	Kd = _Kd;
	dt = _dt;
	Reset_Value();
}