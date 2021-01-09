/*
 * Button.cpp
 *
 * Created: 07.08.2018 16:27:35
 *  Author: Artur
 */ 
 /*
Button.h - библиотека для цифровой обработки сигналов контактов кнопок
 и сигналов других компонентов параллельным процессом
 
В параллельном процессе должен регулярно вызываться один из методов:
    void  scanState();    // метод проверки ожидание стабильного состояния сигнала
    void  filterAvarage(); // метод фильтрации сигнала по среднему значению
	
В результате формируются признаки:
	
 для метода scanState():
 - при нажатой кнопке flagPress= true 
 - при отжатой кнопке flagPress= false
 - при нажатии на кнопку flagClick= true

 для метода filterAvarage() :
 - при сигнале низкого уровня flagPress= true 
 - при сигнале высокого уровня flagPress= false
 - при изменении состояния с высокого на низкий flagClick= true

Объект типа Button при создании имеет параметры:
 - номер вывода, к которому подключена кнопка или сигнал
 - время обработки сигнала (умножается на период вызова метода scanState() или filterAvarage()

 Button button1(12, 15);  // создание объекта для кнопки, подключенной к 12 выводу 
 с временем фильтрации 30 мс (при цикле 2 мс)

 Библиотека разработана Калининым Эдуардом
 http://mypractic.ru/urok-8-cifrovaya-filtraciya-signalov-v-programmax-dlya-arduino.html
 
*/
 //#include "Button.h"
/* #ifndef BUTTON_CPP_
 #define BUTTON_CPP_
 #include "main.h"

 class Button {
	 public:
	 Button(byte pin, byte timeButton);  // конструктор
	 boolean flagPress;    // признак кнопка в нажатом состоянии (сигнал в низком)
	 boolean flagClick;    // признак клика кнопки (фронт)
	 void  scanState();    // метод проверки ожидание стабильного состояния сигнала
	 void  filterAvarage(); // метод фильтрации сигнала по среднему значению
	// void setPinTime(byte pin, byte timeButton); // метод установки номера вывода и времени (числа) подтверждения
	 byte digitalRead(byte pin_);
	 private:
	 byte  _buttonCount;    // счетчик времени фильтрации
	 byte _timeButton;      // время фильтрации
	 byte _pin;             // номер вывода
	 //byte pin_;
 };
 */

 #include "Button.h"
 Button but_p(5, 15);//PD5 кнопки настройка, вкл./выкл. паяльник, вкл./выкл. фен
 Button but_f(6, 15);//PD
 Button but_set(7, 15);//PD7
 Button but_up(8, 10);//PB0 больше, меньше,
 Button but_down(9, 10);//PB1
 Button gercon(0,10);
 byte ButtonCheck(byte but)//провераят нажимание кнопки
 {
	 
	 switch (but)
	 {
	 case 0:
	 but_p.filterAvarage();//вызывает метод фильтрации
	 if (but_p.flagClick == true)//если кнопка нажата 
	 {
		 but_p.flagClick = false;//сбосим признак нажатия кнопри 
		 return(1);//и "скажем" что кнопку нажали
	 }
	 else {return(0);}
	 break;
	 case 1:
	 but_f.filterAvarage();
	 if (but_f.flagClick == true)
	 {
		 but_f.flagClick = false;
		 return(1);
	 }
	 else {return(0);}
	 break;
	 case 2:
	 but_set.filterAvarage();
	 if (but_set.flagClick == true)
	 {
		 but_set.flagClick = false;
		 return(1);
	 }
	 else {return(0);}	 
	 break;
	 case 3:
	 but_up.filterAvarage();
	 if (but_up.flagClick == true)
	 {
		 but_up.flagClick = false;
		 return(1);
	 }
	 else {return(0);}
	 break;
	 case 4:
	 but_down.filterAvarage();
	 if (but_down.flagClick == true)
	 {
		 but_down.flagClick = false;
		 return(1);
	 }
	 else {return(0);}
	 break;
	 }	 
 return 0;
 }
 
 byte digitalRead(byte pin_)//библиотека была написана на ардуино прилшось и некоторые ф-и для неё написать
 {//и закомментировать всё, что не будет использоватся для того, чтоб программа занимала меньше места
	 /*if (pin_ == 5)
	 {
	 if (!(PIND&0b00100000)) { return(1); }
	 else{	 return(0);}
	 }
	 if (pin_ == 6)
	 {
		 if (!(PIND&0b01000000)) { return(1); }
		 else{	 return(0);}
	 }
	 if (pin_ == 7)
	 {
		 if (!(PIND&0b10000000)) { return(1); }
		 else{	 return(0);}
	 }
	 if (pin_ == 0)
	 {
		 if (!(PINB&0b00000001)) { return(1); }
		 else{	 return(0);}
	 }
	 if (pin_ == 1)
	 {
		 if (!(PINB&0b00000010)) { return(1); }
		 else{	 return(0);}
	 }*/
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
	}
 return 0;
 }

byte checkState()//не вышо в меине сделать внешний объёкт геркон вот и ф-ю эту написал тут
{
	return 	gercon.flagPress;
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




 // метод проверки ожидание стабильного состояния сигнала
 // при нажатой кнопке flagPress= true
 // при отжатой кнопке flagPress= false
 // при нажатии на кнопку flagClick= true
 //void Button::scanState() {
//
	 //if ( flagPress != digitalRead(_pin) ) {
		 //// признак flagPress = текущему состоянию кнопки
		 //// (инверсия т.к. активное состояние кнопки LOW)
		 //// т.е. состояние кнопки осталось прежним
		 //_buttonCount= 0;  // сброс счетчика подтверждений состояния кнопки
	 //}
	 //else {
		 //// признак flagPress не = текущему состоянию кнопки
		 //// состояние кнопки изменилось
		 //_buttonCount++;   // +1 к счетчику состояния кнопки
//
		 //if ( _buttonCount >= _timeButton ) {
			 //// состояние кнопки не мянялось в течение заданного времени
			 //// состояние кнопки стало устойчивым
			 //flagPress= ! flagPress; // инверсия признака состояния
			 //_buttonCount= 0;  // сброс счетчика подтверждений состояния кнопки
//
			 //if ( flagPress == true ) flagClick= true; // признак фронта кнопки на нажатие
		 //}
	 //}
 //}


 // метод установки номера вывода и времени подтверждения
 /*void Button::setPinTime(byte pin, byte timeButton)  {

	 _pin = pin;
	 _timeButton = timeButton;
	// pinMode(_pin, INPUT);  // определяем вывод как вход
 }*/

 // описание конструктора класса Button
 Button::Button(byte pin, byte timeButton) {

	  _pin = pin;
	 _timeButton = timeButton;
	// pinMode(_pin, INPUT);  // определяем вывод как вход
 }

 
 //#endif /* BUTTON_CPP_ */