/*
 * Button.cpp
 *
 * Created: 07.08.2018 16:27:35
 *  Author: Artur
 */ 
 /*
Button.h - ���������� ��� �������� ��������� �������� ��������� ������
 � �������� ������ ����������� ������������ ���������
 
� ������������ �������� ������ ��������� ���������� ���� �� �������:
    void  scanState();    // ����� �������� �������� ����������� ��������� �������
    void  filterAvarage(); // ����� ���������� ������� �� �������� ��������
	
� ���������� ����������� ��������:
	
 ��� ������ scanState():
 - ��� ������� ������ flagPress= true 
 - ��� ������� ������ flagPress= false
 - ��� ������� �� ������ flagClick= true

 ��� ������ filterAvarage() :
 - ��� ������� ������� ������ flagPress= true 
 - ��� ������� �������� ������ flagPress= false
 - ��� ��������� ��������� � �������� �� ������ flagClick= true

������ ���� Button ��� �������� ����� ���������:
 - ����� ������, � �������� ���������� ������ ��� ������
 - ����� ��������� ������� (���������� �� ������ ������ ������ scanState() ��� filterAvarage()

 Button button1(12, 15);  // �������� ������� ��� ������, ������������ � 12 ������ 
 � �������� ���������� 30 �� (��� ����� 2 ��)

 ���������� ����������� ��������� ��������
 http://mypractic.ru/urok-8-cifrovaya-filtraciya-signalov-v-programmax-dlya-arduino.html
 
*/
 //#include "Button.h"
/* #ifndef BUTTON_CPP_
 #define BUTTON_CPP_
 #include "main.h"

 class Button {
	 public:
	 Button(byte pin, byte timeButton);  // �����������
	 boolean flagPress;    // ������� ������ � ������� ��������� (������ � ������)
	 boolean flagClick;    // ������� ����� ������ (�����)
	 void  scanState();    // ����� �������� �������� ����������� ��������� �������
	 void  filterAvarage(); // ����� ���������� ������� �� �������� ��������
	// void setPinTime(byte pin, byte timeButton); // ����� ��������� ������ ������ � ������� (�����) �������������
	 byte digitalRead(byte pin_);
	 private:
	 byte  _buttonCount;    // ������� ������� ����������
	 byte _timeButton;      // ����� ����������
	 byte _pin;             // ����� ������
	 //byte pin_;
 };
 */

 #include "Button.h"
 Button but_p(5, 15);//PD5 ������ ���������, ���./����. ��������, ���./����. ���
 Button but_f(6, 15);//PD
 Button but_set(7, 15);//PD7
 Button but_up(8, 10);//PB0 ������, ������,
 Button but_down(9, 10);//PB1
 Button gercon(0,10);
 byte ButtonCheck(byte but)//��������� ��������� ������
 {
	 
	 switch (but)
	 {
	 case 0:
	 but_p.filterAvarage();//�������� ����� ����������
	 if (but_p.flagClick == true)//���� ������ ������ 
	 {
		 but_p.flagClick = false;//������ ������� ������� ������ 
		 return(1);//� "������" ��� ������ ������
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
 
 byte digitalRead(byte pin_)//���������� ���� �������� �� ������� �������� � ��������� �-� ��� �� ��������
 {//� ���������������� ��, ��� �� ����� ������������� ��� ����, ���� ��������� �������� ������ �����
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

byte checkState()//�� ���� � ����� ������� ������� ������ ������ ��� � �-� ��� ������� ���
{
	return 	gercon.flagPress;
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




 // ����� �������� �������� ����������� ��������� �������
 // ��� ������� ������ flagPress= true
 // ��� ������� ������ flagPress= false
 // ��� ������� �� ������ flagClick= true
 //void Button::scanState() {
//
	 //if ( flagPress != digitalRead(_pin) ) {
		 //// ������� flagPress = �������� ��������� ������
		 //// (�������� �.�. �������� ��������� ������ LOW)
		 //// �.�. ��������� ������ �������� �������
		 //_buttonCount= 0;  // ����� �������� ������������� ��������� ������
	 //}
	 //else {
		 //// ������� flagPress �� = �������� ��������� ������
		 //// ��������� ������ ����������
		 //_buttonCount++;   // +1 � �������� ��������� ������
//
		 //if ( _buttonCount >= _timeButton ) {
			 //// ��������� ������ �� �������� � ������� ��������� �������
			 //// ��������� ������ ����� ����������
			 //flagPress= ! flagPress; // �������� �������� ���������
			 //_buttonCount= 0;  // ����� �������� ������������� ��������� ������
//
			 //if ( flagPress == true ) flagClick= true; // ������� ������ ������ �� �������
		 //}
	 //}
 //}


 // ����� ��������� ������ ������ � ������� �������������
 /*void Button::setPinTime(byte pin, byte timeButton)  {

	 _pin = pin;
	 _timeButton = timeButton;
	// pinMode(_pin, INPUT);  // ���������� ����� ��� ����
 }*/

 // �������� ������������ ������ Button
 Button::Button(byte pin, byte timeButton) {

	  _pin = pin;
	 _timeButton = timeButton;
	// pinMode(_pin, INPUT);  // ���������� ����� ��� ����
 }

 
 //#endif /* BUTTON_CPP_ */