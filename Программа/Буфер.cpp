
byte cifri[29] = { 0b01011111, //0,O
	0b01000001,//1,i
	0b10011101,//2
	0b11010101,//3
	0b11000011,//4 4 element
	0b11010110,//5,s
	0b11011110,//6
	0b01000101,//7
	0b11011111,//8,g
	0b11010111,//9 9 element
	0b10000000,//-
	0b00000000,//������� 11element
	0b00010000,//_ 12 element
	0b11001111,//A
	0b11011010,//b 14 element
	0b00011110,//C
	0b11011001,//d
	0b10011110,//E 17 element
	0b10001110,//F
	0b11001010,//h 19 element
	0b01010001,//J 20 element
	0b11001011,//H �������� ����� �
	0b00011010,//L 22 element
	//�� ���� ��� ���������� "�"
	0b11001000,//n 23 element
	0b10001111,//P 24 element
	0b11000111,//q 25 element
	0b10001000,//r 26 element
	0b10011010,//t 27 element
	0b11010011//� 28 element

};


#include <TimerOne.h>
#include <EEPROM.h>
int time1 = 250; //�������� ��� ������ � ��������
int time2 = 150; //��� ����� ����������� ������ � ��������������
int time3 = 250; //��� ����� ��� �-� ���������� ����������
int time4 = 150; //��� ����� ����������� ������ � ��������
byte  but_set = 7, but_up = 8, but_down = 9, but_p = 5, but_f = 6, st1 = 12, sh = 13, st2 = 10, ds = 11; // ������ ���������, ������, ������, ���./����. ��������, ���./����. ���
byte mosfet_p = 4; //������ ����������� ������������ ���������
byte first_heat = 0;//���� ������� ������� ���������
byte flag_set;//���� ���������
byte flag_b; //��������������� ����
byte flag_a; //��������������� ����
byte power_p; //����� ��� ��� ���� �������� � ���
byte power_f; // ����� ��� ��� ���� �������� � ��� 
			  //��� ������ �� ������������ ����� �������� ������������� ���������� (������)
int delta_tp; // ����������� ����������� ���������
int delta_tf; //����������� ����������� ����
int real_tp; //�������� ����������� ��������� � ����
int real_tf; //�������� ����������� ��������� � ����
int read_tp; //��������������� ����������� �������� � ����
int read_tf;//��������������� ����������� �������� � ����
byte show_must_t; //���� ������� �������� �� ��, ����� ����� ������� �������� ����������� ������������
int old_tp;
int old_tf;
int tp;//����������� ��������� � ��������
int tf; //����������� ��������� � ��������
byte addr_dp = 0, addr_df = 1; //������ ����������� ��������� � ����
int array_cifra[6], array_t[6];// � array_t ������������ ����� �������� �� ������ cifri ������� ����� ����������
unsigned long control_t;// ������ ���������� �� ������� ���������� ������ � ������������� ��� ����������� �������� ������������
unsigned long read_t;// ������� ���������� � ��������
unsigned long time_p;// ������� ���������� ����������
unsigned long but_pow;// ����� ����� ������ ���/���� ��������� � ���� ��������� ����� �� �������

/*
int arr0[8] = {0, 1, 0, 1, 1, 1, 1, 1};
int arr1[8] = {0, 1, 0, 0, 0, 0, 0, 1};
int arr2[8] = {1, 0, 0, 1, 1, 1, 0, 1};
int arr3[8] = {1, 1, 0, 1, 0, 1, 0, 1};
int arr4[8] = {1, 1, 0, 0, 0, 0, 1, 1};
int arr5[8] = {1, 1, 0, 1, 0, 1, 1, 0};
int arr6[8] = {1, 1, 0, 1, 1, 1, 0, 1};
int arr7[8] = {0, 1, 0, 0, 0, 1, 0, 1};
int arr8[8] = {1, 1, 0, 1, 1, 1, 1, 1};
int arr9[8] = {1, 1, 0, 1, 0, 1, 1, 1};
int cifri[10][8] = {{0, 1, 0, 1, 1, 1, 1, 1}, //����� � ������� �� 0 �� 9
{0, 1, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 1, 1, 1, 0, 1},
{1, 1, 0, 1, 0, 1, 0, 1},
{1, 1, 0, 0, 0, 0, 1, 1},
{1, 1, 0, 1, 0, 1, 1, 0},
{1, 1, 0, 1, 1, 1, 0, 1},
{0, 1, 0, 0, 0, 1, 0, 1},
{1, 1, 0, 1, 1, 1, 1, 1},
{1, 1, 0, 1, 0, 1, 1, 1}
};
int mas[6][8] =     {{1, 1, 1, 1, 1, 1, 0, 1}, // ����� ��� ��������� ������������� � ������� ���� ������ ���� ����� ������������ � ���� ������ �������
{1, 1, 1, 1, 1, 0, 1, 1},
{1, 1, 1, 1, 0, 1, 1, 1},
{1, 1, 1, 0, 1, 1, 1, 1},
{1, 1, 0, 1, 1, 1, 1, 1},
{1, 0, 1, 1, 1, 1, 1, 1},
};
*/
int cifri[12] = { 0b01011111, //����� � ������� �� 0 �� 9, ���� ������, ������ "�� ����������"
0b01000001,
0b10011101,
0b11010101,
0b11000011,
0b11010110,
0b11011110,
0b01000101,
0b11011111,
0b11010111,
0b11111001,
0b10000000
};
int mas[6] = { 0b11111101, // ����� ��� ��������� ������������� � ������� ���� ������ ���� ����� ������������ � ���� ������ �������
0b11111011,
0b11110111,
0b11101111,
0b11011111,
0b10111111
};

int i, n, m, val, e;// � - ������� ��� ��� ����� �������� �������� �����������

void setup() {
	// put your setup code here, to run once:
	pinMode(but_set, INPUT);
	pinMode(but_up, INPUT);
	pinMode(but_down, INPUT);
	pinMode(A1, INPUT);
	pinMode(A2, INPUT);
	pinMode(A3, INPUT);
	pinMode(A4, INPUT);
	pinMode(st1, OUTPUT);
	pinMode(st2, OUTPUT);
	pinMode(sh, OUTPUT);
	pinMode(ds, OUTPUT);
	pinMode(mosfet_p, OUTPUT);

	Timer1.initialize(10);
	Timer1.attachInterrupt(timerIsr);

	delta_tp = EEPROM.read(addr_dp);
	delta_tf = EEPROM.read(addr_dp);

}

void loop()
{


	// ������ � ����� ����������, �������� ��� ����� �����������, � ���������-------
	if ((!digitalRead(but_set) == 1) && (Wait_MinTime_Pressing(but_set) == 1))
	{
		flag_set = 1;
		flag_a = 1;
	}
	while (flag_set == 1)
	{
		if ((!digitalRead(but_set)) && (Wait_MinTime_Pressing(but_set)))
		{
			// ������ ������ �����
			switch (flag_a)
			{
			case 0: {
				delay(time1);
				flag_b = 1;
			}
					break;
			case 1: {
				delay(time1);
				flag_b = 0;
			}
					break;

			case 2: {
				flag_b = 0;
				flag_set = 0;
				EEPROM.write(addr_dp, delta_tp);
				EEPROM.write(addr_df, delta_tf);
				delay(time1);
			}
			}// switch

			flag_a = (flag_a + 1) & 2;     // �������� ����� �������������, � ����� ��� ���������� ������ 2 �������������� � 0
		}
		// ��������� �������� �����������
		if ((!digitalRead(but_up)) && (Wait_MinTime_Pressing(but_up)))
		{
			switch (flag_b)
			{
			case 0: {
				delta_tp += 2; // 2 ������� - ��� 1 ������
				if (delta_tp > 198)// �������� ����������� �� ����� ���� ������ 99 
					delta_tp = 198;
				delay(time1);
			}
					break;
			case 1: {
				delta_tf += 2;
				if (delta_tf > 198)
					delta_tf = 198;
				delay(time1);
			}
					break;
			}
		}
		//��������� �������� �����������
		if ((!digitalRead(but_down)) && (Wait_MinTime_Pressing(but_down)))
		{
			switch (flag_b)
			{
			case 0: {
				delta_tp -= 2;
				if (delta_tp < 0)
					delta_tp = 0;
				delay(time1);
			}
					break;
			case 1: {
				delta_tf -= 2;
				if (delta_tf < 0)
					delta_tf = 0;
				delay(time1);
			}
					break;
			}
		}
	}
	// ����� ����������-----------------------------
	//���������� �������� ����������� 

	if (millis() - control_t > time2)
	{
		tp = analogRead(A1);
		tf = analogRead(A2);
		val = tp - old_tp;
		if (val > 4 || val < 4)
		{
			old_tp = tp;
			show_must_t = 1;
			e = 10;
		}
		val = tf - old_tf;
		if (val > 4 || val < 4)
		{
			old_tf = tf;
			show_must_t = 1;
			e = 10;
		}
		control_t = millis();
	}
	//����� ���������� �������� ����������� 
	//��������� �������� � ��������

	if (millis() - read_t > time4)
	{
		read_tp = analogRead(A3);
		read_tf = analogRead(A4);
		delayMicroseconds(100);
		read_tp += analogRead(A3);
		read_tf += analogRead(A4);
		read_t = millis();
	}
	// ����� ���������� �������� � ��������
	// ������ ���/���� �������� � ���
	if (millis() - but_pow < 1000)
	{
		if ((!digitalRead(but_p)) && (Wait_MinTime_Pressing(but_p)))
		{
			power_p = (power_p + 1) & 1;
			show_must_t = 1;
			e = 10;
			//   delay(time1);
		}
		if ((!digitalRead(but_f)) && (Wait_MinTime_Pressing(but_f)))
		{
			power_f = (power_f + 1) & 1;
			show_must_t = 1;
			e = 10;
			// delay(time1);
		}
		but_pow = millis();
	}
	//���������� ����������
	if (power_p == 1)
		Power_p();
}
//���������� ����������
void Power_p()
{
	if (millis() - time_p > time3)
	{
		char temp = 40;
		real_tp += delta_tp;
		if (first_heat == 0) //������ ������� � �� ��������� �������� ��� ���������
		{
			if (real_tp < old_tp - temp)
				digitalWrite(mosfet_p, 1);
			if (real_tp > old_tp - temp)
			{
				digitalWrite(mosfet_p, 0);
				first_heat = 1;
			}
		}
		if (real_tp < old_tp - 10)
			digitalWrite(mosfet_p, 1);
		if (real_tp > old_tp + 10)
			digitalWrite(mosfet_p, 0);
		time_p = millis();
	}
}
//����� ���������� ����������
int Wait_MinTime_Pressing(byte but) {
	// ���� ������ ������ ������ 50������� - ��� �� ������
	//int flag = 1;  
	unsigned long time = millis() + 50;
	while (millis() < time)
	{
		/*������ ��������� ������ ��� ������ 50����*/
		if (digitalRead(but) == 1) { return 0; }
		delay(10);
	}
	return digitalRead(but);
}


void timerIsr()
{

	int a, b, c, d;//��������������� ����������, �,d �������� �� ��, ����� �������� ������� ���������� �������, � ����� ���������� ���������
				   //����������� �������� ����������� ��������� � ����
	real_tp = map(read_tp, 0, 1023, 0, 512);
	real_tf = map(read_tf, 0, 1023, 0, 512);
	delta_tp /= 2;
	delta_tf /= 2;
	real_tp += delta_tp;
	real_tf += delta_tf;
	if (flag_set == 0) // ����� �������� �����������
	{
		a = real_tp;
		b = real_tf;
	}
	if (show_must_t == 1 && e > 0)
	{
		a = old_tp;
		b = old_tf;
		e--;
		if (e == 0)
			show_must_t = 0;
	}
	i = 2;
	for (; i >= 0; ) // ���������� ���� � �����
	{
		array_t[i] = a % 10;
		a /= 10;
		i--;
	}
	i = 5;
	for (; i >= 3; )
	{
		array_t[i] = b % 10;
		b /= 10;
		i--;
	}
	i = 0;
	n = 0;

	if (flag_set == 1) // ����� ����������� ����������� ��� ����������
	{
		a = delta_tp;
		b = delta_tf;
		if (flag_b == 1) // ���� ����������� �������� ������� ���� "������" ����� ��������� ����������� ���������
		{
			array_t[0] = 10;
			c = 1;
			d = 3;
		}
		if (flag_b == 0) // ���� ����������� ��� ������� ���� "������" ����� ��������� ����������� ����
		{
			array_t[3] = 10;
			c = 0;
			d = 4;
		}
		i = 2;
		for (; i >= c; ) // ���������� ���� � �������� � �����
		{
			array_t[i] = a % 10;
			a /= 10;
			i--;
		}
		i = 5;
		for (; i >= d; )
		{
			array_t[i] = b % 10;
			b /= 10;
			i--;
		}
		i = 0;
		n = 0;
	}
	for (; i <= 5;)
	{
		/* switch(array_t[i]) //����� �������� ������� ����� ����������
		{
		case 0 :
		m = 0;
		break;
		case 1:
		m = 1;
		break;
		case 2:
		m = 2;
		break;
		case 3:
		m = 3;
		break;
		case 4:
		m = 4;
		break;
		case 5:
		m = 5;
		break;
		case 6:
		m = 6;
		break;
		case 7:
		m = 7;
		break;
		case 8:
		m = 8;
		break;
		case 9:
		m = 9;
		break;
		case 10:
		m = 10;
		break;
		}*/
		m = array_t[i];
		// ������ � ������� ������������ � �������
		digitalWrite(st2, 0);
		shiftOut(ds, sh, MSBFIRST, mas[i]);
		digitalWrite(st2, 1);
		// ������ � ������� ������������ � ������
		digitalWrite(st1, 0);
		shiftOut(ds, sh, MSBFIRST, cifri[m]);
		digitalWrite(st1, 1);

		delay(1);
		// ������� ���������
		digitalWrite(st1, 0);
		shiftOut(ds, sh, MSBFIRST, 0);
		digitalWrite(st1, 1);

		digitalWrite(st2, 0);
		shiftOut(ds, sh, MSBFIRST, 0);
		digitalWrite(st2, 1);
		i++;
	}
}
