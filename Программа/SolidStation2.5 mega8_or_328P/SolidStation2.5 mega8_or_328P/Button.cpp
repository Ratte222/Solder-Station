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
 byte digitalRead(byte pin_)//���������� ���� �������� �� ������� �������� � ��������� �-� ��� �� ��������
 {//� ���������������� ��, ��� �� ����� ������������� ��� ����, ���� ��������� �������� ������ �����
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

 // ����� ���������� ������� �� �������� ��������
 // ��� ������� ������� ������ flagPress= true
 // ��� ������� �������� ������ flagPress= false
 // ��� ��������� ��������� � �������� �� ������ flagClick= true
 void Button::filterAvarage() {
		
	 if ( flagPress != digitalRead(_pin) ) {
		 //  ��������� ������ �������� �������
		 if ( _buttonCount != 0 ) _buttonCount--; // ������� ������������� - 1 � ������������ �� 0
	 }
	 else {
		 // ��������� ������ ����������
		 _buttonCount++;   // +1 � �������� �������������

		 if ( _buttonCount >= _timeButton ) {
			 // ��������� ������� �������� ������ _timeButton
			 flagPress= ! flagPress; // �������� �������� ���������
			 _buttonCount= 0;  // ����� �������� �������������

			 if ( flagPress == true ) flagClick= true; // ������� ����� ������
		 }
	 }
 }

 // ����� ��������� ������ ������ � ������� �������������
 /*void Button::setPinTime(byte pin, byte timeButton)  {

	 _pin = pin;
	 _timeButton = timeButton;
 }*/

 // �������� ������������ ������ Button
 Button::Button(byte pin, byte timeButton) {

	  _pin = pin;
	 _timeButton = timeButton;
 }
