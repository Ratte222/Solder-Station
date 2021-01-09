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
	Button(byte pin, byte timeButton);  // �����������
	boolean flagPress;    // ������� ������ � ������� ��������� (������ � ������)
	boolean flagClick;    // ������� ����� ������ (�����)
	//void  scanState();    // ����� �������� �������� ����������� ��������� �������
	void  filterAvarage(); // ����� ���������� ������� �� �������� ��������
	//void setPinTime(byte pin, byte timeButton); // ����� ��������� ������ ������ � ������� (�����) �������������
	//byte digitalRead(byte pin_);
	private:
	byte  _buttonCount;    // ������� ������� ����������
	byte _timeButton;      // ����� ����������
	byte _pin;             // ����� ������
	
	//byte pin_;
};

//byte digitalRead(byte pin_);
byte checkState(void);
byte ButtonCheck(byte but);
#endif /* BUTTON_H_ */