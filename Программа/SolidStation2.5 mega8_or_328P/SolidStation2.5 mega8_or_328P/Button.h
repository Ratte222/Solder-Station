/*
 * Button.h
 *
 * Created: 07.08.2018 16:26:51
 *  Author: Artur
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_
#include "main.h"
class Button {
	public:
	Button(byte pin, byte timeButton);  // конструктор
	boolean flagPress;    // признак кнопка в нажатом состоянии (сигнал в низком)
	boolean flagClick;    // признак клика кнопки (фронт)
	byte isClick(void);//провераят нажимание кнопки
	void  filterAvarage(); // метод фильтрации сигнала по среднему значению
	//void setPinTime(byte pin, byte timeButton); // метод установки номера вывода и времени (числа) подтверждения
	//byte digitalRead(byte pin_);
	private:
	byte  _buttonCount;    // счетчик времени фильтрации
	byte _timeButton;      // время фильтрации
	byte _pin;             // номер вывода
	
	//byte pin_;
};

#endif /* BUTTON_H_ */