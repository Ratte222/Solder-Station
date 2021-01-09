/*
 * Button.cpp
 *
 * Created: 07.08.2018 16:27:35
 *  Author: Artur
 */ 
 
 #include "Button.h"
 byte Button::isClick(void)
 {
	 filterAvarage();
	 if (flagClick == true)
	 {
		flagClick = false;
		return(1);
	 }
	 else {return(0);}
 }
 byte digitalRead(byte pin_)//библиотека была написана на ардуино прилшось и некоторые ф-и для неё написать
 {//и закомментировать всё, что не будет использоватся для того, чтоб программа занимала меньше места
	 switch(pin_)
	 {
		case 0:
			 if (PIND&0b00000001) { return(1); }
			 else{	 return(0);}
			break;
		//case 1:
		//if (PIND&0b00000010) { return(1); }
		//else{	 return(0);}
		//break;
		//case 2:
		//if (PIND&0b00000100) { return(1); }
		//else{	 return(0);}
		//break;
		//case 3:
		//if (PIND&0b00001000) { return(1); }
		//else{	 return(0);}
		//break;
		//case 4:
		//if (PIND&0b00010000) { return(1); }
		//else{	 return(0);}
		//break;
		case 5:
		if (PIND&0b00100000) { return(1); }
		else{	 return(0);}
		break;
		case 6:
		if (PIND&0b01000000) { return(1); }
		else{	 return(0);}
		break;
		case 7:
		if (PIND&0b10000000) { return(1); }
		else{	 return(0);}
		break;
		case 8:
		if (PINB&0b00000001) { return(1); }
		else{	 return(0);}
		break;
		case 9:
		if (PINB&0b00000010) { return(1); }
		else{	 return(0);}
		break;
		//case 10:
		//if (PINB&0b00000100) { return(1); }
		//else{	 return(0);}
		//break;
		//case 11:
		//if (PINB&0b00001000) { return(1); }
		//else{	 return(0);}
		//break;
		//case 12:
		//if (PINB&0b00010000) { return(1); }
		//else{	 return(0);}
		//break;
		//case 13:
		//if (PINB&0b001000000) { return(1); }
		//else{	 return(0);}
		//break;
		//case 14:
		//if (PINC&0b00000001) { return(1); }
		//else{	 return(0);}
		//break;
		//case 15:
		//if (PINC&0b00000010) { return(1); }
		//else{	 return(0);}
		//break;
		//case 16:
		//if (PINC&0b00000100) { return(1); }
		//else{	 return(0);}
		//break;
		//case 17:
		//if (PINC&0b00001000) { return(1); }
		//else{	 return(0);}
		//break;
		//case 18:
		//if (PINC&0b00010000) { return(1); }
		//else{	 return(0);}
		//break;
		//case 19:
		//if (PINC&0b00100000) { return(1); }
		//else{	 return(0);}
		//break;
		//case 20:
		//if (PINC&0b01000000) { return(1); }
		//else{	 return(0);}
		//break;
	}
 return 0;
 }

 // метод фильтрации сигнала по среднему значению
 // при сигнале низкого уровня flagPress= true
 // при сигнале высокого уровня flagPress= false
 // при изменении состояния с высокого на низкий flagClick= true
 void Button::filterAvarage() {
		
	 if ( flagPress != digitalRead(_pin) ) {
		 //  состояние кнопки осталось прежним
		 if ( _buttonCount != 0 ) _buttonCount--; // счетчик подтверждений - 1 с ограничением на 0
	 }
	 else {
		 // состояние кнопки изменилось
		 _buttonCount++;   // +1 к счетчику подтверждений

		 if ( _buttonCount >= _timeButton ) {
			 // состояние сигнала достигло порога _timeButton
			 flagPress= ! flagPress; // инверсия признака состояния
			 _buttonCount= 0;  // сброс счетчика подтверждений

			 if ( flagPress == true ) flagClick= true; // признак клика кнопки
		 }
	 }
 }

 // метод установки номера вывода и времени подтверждения
 /*void Button::setPinTime(byte pin, byte timeButton)  {

	 _pin = pin;
	 _timeButton = timeButton;
 }*/

 // описание конструктора класса Button
 Button::Button(byte pin, byte timeButton) {

	  _pin = pin;
	 _timeButton = timeButton;
 }
