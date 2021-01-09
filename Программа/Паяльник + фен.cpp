#define tp 0
#define tf 1
#define Max_step_ADC 15
#define Solid_on digitalWrite(mosfet_p, 1)
#define Solid_off digitalWrite(mosfet_p, 0)
#include <TimerOne.h>
void Filter(void);
void Power_p(void);
//����������� ��� ����
byte zero = 2;//��������� ��� ������ 0 �����
byte dim = 3;// ���������� ����������
			 //byte fun = 3;// ���������� ������������ ����
byte half_sin_flag; //����, ������� ��������� �������� ���������� ������ ���� ���
int del = 0;//�������� ��� ����
volatile int tic;//�������� ��� ����
unsigned long t_sinus;// ����������� ����� �� ������ ������ ���� ����� ��� ���� ���������� � �������� ������
					  //����� ����������� ��� ����(�� ����)
int time1 = 250; //�������� ��� ������ � ��������
int time2 = 150; //��� ����� ����������� ���
int time3 = 250; //��� ����� ��� �-� ���������� ����������
				 //int time4 = 150; //��� ����� ����������� ������ � ��������
byte  but_set = 7, but_up = 8, but_down = 9, but_p = 5, but_f = 6, st1 = 12, sh = 13, st2 = 10, ds = 11; // ������ ���������, ������, ������, ���./����. ��������, ���./����. ���
byte mosfet_p = 4; //������ ����������� ������������ ���������
byte first_heat_p = 0;//���� ������� ������� ���������
byte first_heat_f = 0;//���� ������� ������� ���� 
byte flag_set;//���� ���������
byte flag_b; //��������������� ����
byte flag_a; //��������������� ����
byte power_p; //����� ��� ��� ���� �������� � ���
byte power_f; // ����� ��� ��� ���� �������� � ��� 
volatile int delta_t[2]; //����������� ����������� delta_tp, delta_tf
volatile int required_t[2]; //�������� ����������� required_tp, required_tf
volatile int real_t[2]; //��������������� ����������� real_tp, real_tf
volatile int old_required_t[2]; //������ �������� ����������� old_required_tp, old_required_tf
volatile int old_real_t[2]; //������ ��������������� �����������
volatile byte show_must_t; //���� ������� �������� �� ��, ����� ����� ������� �������� ����������� ������������
						   //byte addr_dp = 0, addr_df = 1; //������ ����������� ��������� � ����
int array_cifra[6], array_t[6];// � array_t ������������ ����� �������� �� ������ cifri ������� ����� ����������
unsigned long control_t;// ������ ���������� �� ������� ���������� ���
						//unsigned long read_t;// ������� ���������� � ��������
unsigned long time_p;// ������� ���������� ����������
unsigned long but_pow;// ����� ����� ������ ���/���� ��������� � ���� ��������� ����� �� �������
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
	//������ ���������� ��� ���������
	pinMode(A1, INPUT);
	pinMode(A3, INPUT);
	pinMode(mosfet_p, OUTPUT);
	//����� ���������� ��� ���������
	//������ ���������� ���������� �������
	pinMode(st1, OUTPUT);
	pinMode(st2, OUTPUT);
	pinMode(sh, OUTPUT);
	pinMode(ds, OUTPUT);
	//����� ���������� ���������� �������
	//������ ������
	pinMode(but_set, INPUT);
	pinMode(but_up, INPUT);
	pinMode(but_down, INPUT);
	pinMode(but_p, INPUT);
	pinMode(but_f, INPUT);
	//����� ������  
	//������ ���������� ��� ����
	pinMode(A2, INPUT);
	pinMode(A4, INPUT);
	pinMode(zero, INPUT);
	pinMode(dim, OUTPUT);
	//  pinMode(fun, OUTPUT);
	//attachInterrupt(0, zero_p, FALLING);
	//����� ���������� ��� ����
	attachInterrupt(0, detect_up, FALLING);  // ��������� ������������ ���������� interrupt0 �� pin 2 �� ������ �������
											 //delta_tp = EEPROM.read(addr_dp);
											 //delta_tf = EEPROM.read(addr_dp);
	Timer1.initialize(40);
	Timer1.attachInterrupt(timer_interrupt);
	Timer1.stop();
	Serial.begin(9600);
}

void loop()
{

	/*
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
	// ����� ����������-----------------------------*/
	//���������� �������� ����������� 
	
	if (millis() - control_t > time2)
	{
		Filter();
		Power_p();
	}

}

void Filter()
{
	//���������� �������� ����������� 
	int x[2];
	x[tp] = analogRead(A1);
	x[tf] = analogRead(A2);
	for (i = 0; i < 2; i++)
	{
		x[i] = map(x[i], 0, 1024, 0, 512);
		val = x[i] - old_required_t[i];
		if (val > 4 || val < 4)
		{
			required_t[i] = x[i];
			old_required_t[i] = x[i];
			show_must_t = 1;
			e = 10;
		}
	}

	//��������� �������� � ��������
	x[0] = analogRead(A3);
	x[1] = analogRead(A4);
	for (i = 0; i < 2; i++)
	{
		//   x[i] = map(x[i], 0, 800, 0, 500);
		if ((x[i] - old_real_t[i]) > Max_step_ADC)// ���� ��� ����������� ������� ������ - ������� 
		{
			real_t[i] += Max_step_ADC;

		}
		else if ((old_real_t[i] - x[i]) > Max_step_ADC)// ���� ��� ���������� ������� ������ - �������
		{
			real_t[i] -= Max_step_ADC;

		}
		else
		{
			real_t[i] = x[i];

		}
		old_real_t[i] = real_t[i];
	}
	//������ �������� ��� ����
	if (required_t[tf] > (real_t[tf] + 5))
	{
		del -= 10;
		if (del < 0)
			del = 0;
	}
	if (required_t[tf] < (real_t[tf] + 5))
	{
		del += 10;
		if (del > 10000)
			del = 10000;
	}
	//����� ������� �������� ��� ����


	control_t = millis();
	/*Serial.print("real_tp");
	Serial.println(real_t[tp]);
	Serial.print("required_tp");
	Serial.println(required_t[tp]);*/
	Serial.print("real_tf");
	Serial.println(real_t[tf]);
	Serial.print("required_tf");
	Serial.println(required_t[tf]);
	Serial.print("del");
	Serial.println(del);
	Serial.print("half_sin_flag ");
	Serial.println(half_sin_flag);
}

//���������� ����������
void Power_p()
{
	//if (millis() - time_p > time3)
	//{
	char temp = 15;

	//required_tp += delta_tp;
	if (first_heat_p == 0) //������ ������� � �� ��������� �������� ��� ���������
	{
		if (required_t[tp] < (real_t[tp] - temp))
		{
			Solid_on;
			Serial.println("mosf_on");
		}
		if (required_t[tp] > (real_t[tp] - temp))
		{
			Solid_off;
			first_heat_p = 1;
			Serial.println("mosf_off");
		}
	}
	if (required_t[tp] > (real_t[tp] + 5))
	{
		Solid_on;
		Serial.println("mosf_on");
	}
	if (required_t[tp] < real_t[tp] + 5)
	{
		Solid_off;
		Serial.println("mosf_off");
	}
	//time_p = millis();
	//}
}
//����� ���������� ����������
/*//������ ���������� �����
void Power_f()
{

if (required_t[tf] > (real_t[tf] + 5))
{
del -= 10;
if ((del < 0) && (half_sin_flag == 1))
{
del = 5000;
half_sin_flag = 0;
}
else { del = 0; }
}
if (required_t[tf] < (real_t[tf] + 5))
{
del += 10;
if ((del > 5000) && (half_sin_flag == 1))
{
half_sin_flag = 1;
del = 0;
}
else
del = 5000;
}
//  if (half_sin_flag == 1)
//    del = del - (millis() - t_sinus);
delayMicroseconds(del);
digitalWrite(dim, 1);
Serial.println("fan");
delay(1);
digitalWrite(dim, 0);
//Serial.print("del");
//Serial.println(del);
}
void zero_p()
{
if (half_sin_flag)
{
attachInterrupt(0, half_sinus, LOW);
t_sinus = millis();
}
else
Power_f();
}
void half_sinus()
{
attachInterrupt(0, zero_p, FALLING);
//half_sin_flag = 1;
Power_f();
}
//����� ���������� �����
*/
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
//----------------------����������� ����������--------------------------
void timer_interrupt() {       // ���������� ������� ����������� ������ 40 ���
	tic++;                       // �������
	if (tic > del)            // ���� ������� ����� �������� ���
	{
		digitalWrite(dim, 1);   // ������� ���

	}
}

void  detect_up() {    // ��������� �������� ���������� �� ����������� ���� �����
	tic = 0;                                  // �������� �������
	Timer1.detachInterrupt();                           // ������������� ������
	Timer1.attachInterrupt(timer_interrupt);
	attachInterrupt(0, detect_down, RISING);  // ������������� ����������
}

void  detect_down() {  // ��������� �������� ���������� �� ����������� ���� ������
	tic = 0;                                  // �������� �������
	Timer1.stop();                           // ���������� ������
	digitalWrite(dim, 0);                  // �������� ���
	attachInterrupt(0, detect_up, FALLING);   // ������������� ����������
}
//----------------------����������� ����������--------------------------