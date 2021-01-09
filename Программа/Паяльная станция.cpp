/*��� �������� �� ���������, �� ����� ������� */
#include <Button.h>
#include <EEPROM.h>
#include <TimerOne.h>
#define p 0 // ��������
#define f 1 //���
#define dim  1// ���������� ����������
#define zero 2//��������� ��� ������ 0 �����
#define fan  3// ���������� ������������ ����
#define mosfet_p  4 //������ ����������� ������������ ���������
#define st2 10
#define ds  11 
#define st1 12
#define sh  13
#define Max_step_ADC 15
#define Solid_on digitalWrite(mosfet_p, 1)
#define Solid_off digitalWrite(mosfet_p, 0)
#define Fan_enable digitalWrite(fan, 1)
#define Fan_disable digitalWrite(fan, 0)

void Filter(void);
void Power_p(void);
void Screen(void);
void Setting(void);
//Button button1(12, 15);// �������� ������� ��� ������, ������������ � 12 ������ 
//� �������� ���������� 30 ��(��� ����� 2 ��)
Button but_p(5, 16);// ������ ���������, ������, ������, ���./����. ��������, ���./����. ���
Button but_f(6, 16);
Button but_set(7, 16);
Button but_up(8, 10);
Button but_down(9, 10);
//byte half_sin_flag; //����, ������� ��������� �������� ���������� ������ ���� ���
int del = 0;//�������� ��� ����
volatile int tic;//�������� ��� ����
				 //unsigned long t_sinus;// ����������� ����� �� ������ ������ ���� ����� ��� ���� ���������� � �������� ������
				 //����� ����������� ��� ����(�� ����)
int time1 = 250; //�������� ��� ������ � ��������
int time2 = 150; //��� ����� ����������� ���
int time3 = 250; //��� ����� ��� �-� ���������� ����������
				 //int time4 = 150; //��� ����� ����������� ������ � ��������

				 //byte  but_set = 7, but_up = 8, but_down = 9, but_p = 5, but_f = 6;

int *pointer;



byte first_heat_p = 0;//���� ������� ������� ���������
byte first_heat_f = 0;//���� ������� ������� ���� 
byte flag_set;//���� ���������
byte flag_b; //��������������� ����
byte flag_a; //��������������� ����
byte flag_fan; // ���� == 1 - ��������� ��� ��������� ���� ����� ����������� ����������� � ����
			   //byte power_p; //����� ��� ��� ���� �������� � ���
			   //byte power_f; // ����� ��� ��� ���� �������� � ��� 
volatile byte power[2]; // ����� ��� ��� ���� �������� � ��� 
int delta_t[2]; //����������� ����������� delta_p, delta_f
volatile int required_t[2]; //�������� ����������� required_p, required_f
volatile int real_t[2]; //��������������� ����������� real_p, real_f
volatile int old_required_t[2]; //������ �������� ����������� old_required_p, old_required_f
volatile int old_real_t[2]; //������ ��������������� �����������
volatile byte show_must_t; //���� ������� �������� �� ��, ����� ����� ������� �������� ����������� ������������
						   //byte addr_dp = 0, addr_df = 1; //������ ����������� ��������� � ����
volatile byte setting[2];
int array_cifra[6], array_t[6];// � array_t ������������ ����� �������� �� ������ cifri ������� ����� ����������
							   //unsigned long control_t;// ������ ���������� �� ������� ���������� ���
							   //unsigned long read_t;// ������� ���������� � ��������
							   //unsigned long time_p;// ������� ���������� ����������
							   //unsigned long but_pow;// ����� ����� ������ ���/���� ��������� � ���� ��������� ����� �� �������
							   //float temperatura[100] ={0.000 , }; 
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
int cifri2[12] = { 0b01111111, //����� � ������� �� 0 �� 9
0b01100001,
0b10111101,
0b11110101,
0b11100011,
0b11110110,
0b11111110,
0b01100101,
0b11111111,
0b11110111,
0b11111001,
0b10100000
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
	/* pinMode(5, INPUT);
	pinMode(6, INPUT);
	pinMode(7, INPUT);
	pinMode(8, INPUT);
	pinMode(9, INPUT);*/
	//����� ������  
	//������ ���������� ��� ����
	pinMode(A2, INPUT);
	pinMode(A4, INPUT);
	pinMode(zero, INPUT);
	pinMode(dim, OUTPUT);
	//  pinMode(fan, OUTPUT);
	//attachInterrupt(0, zero_p, FALLING);
	//����� ���������� ��� ����

	//delta_p = EEPROM.read(addr_dp);
	//delta_f = EEPROM.read(addr_dp);
	Timer1.initialize(40);
	Timer1.attachInterrupt(timer_interrupt);
	Timer1.stop();
	//Serial.begin(9600);
	if (EEPROM.read(4) == 255)
	{
		EEPROM.write(0, 0);
		EEPROM.write(2, 0);
		EEPROM.write(4, 0);
	}
	delta_t[p] = EEPROM.read(0);
	delta_t[f] = EEPROM.read(2);

}

void loop()
{
	Filter();
	//------------------------������ ��������� ������ ��������� -------------------------------------
	// but_p.scanState();
	// but_f.scanState();

	but_set.filterAvarage();
	if (but_set.flagClick == true)
	{
		power[p] = false;
		but_set.flagClick = false;
		if ((setting[0] == true) && (setting[1] == 0))
		{
			val = EEPROM.read(0);
			if (delta_t[p] != val)
			{
				val = delta_t[p];
				EEPROM.write(0, val);
			}
			setting[1] = true;
		}
		else if ((setting[0] == true) && (setting[1] == 1))
		{
			val = EEPROM.read(2);
			if (delta_t[f] != val)
			{
				val = delta_t[f];
				EEPROM.write(2, val);
			}
			setting[1] = false;
			setting[0] = false;
		}
		else
		{
			setting[0] = true;
		}
	}
	if (setting[0] == true) { Setting(); }
	if (setting[0] != true)
	{
		but_p.filterAvarage();
		but_f.filterAvarage();
		if (but_p.flagClick == true)
		{
			power[p] = !power[p];
			but_p.flagClick = false;
		}
		if (but_f.flagClick == true)
		{
			power[f] = !power[f];
			if (power[f])
			{
				attachInterrupt(0, detect_up, FALLING);  // ��������� ������������ ���������� interrupt0 �� pin 2 �� ������ �������
				Timer1.start();
				Fan_enable;
			}
			else
			{
				detachInterrupt(0);
				Timer1.stop();
				flag_fan = true;
			}
			but_f.flagClick = false;
		}
		if (power[p] == 1)
		{
			Power_p();
		}
	}
	if ((flag_fan == true) && (real_t[f] < 100))
		Fan_disable;
	//------------------����� ��������� ������ ���������-------------------------------------------
	Screen();
}

void Setting(void)
{
	but_up.filterAvarage();
	but_down.filterAvarage();
	if (but_up.flagClick == true)
	{
		delta_t[setting[1]]++;
		but_up.flagClick = false;
	}
	if (but_down.flagClick == true)
	{
		delta_t[setting[1]]--;
		but_down.flagClick = false;
	}
}
//------------------������ ������ �� �������---------------------------------------------------
void Screen(void)
{
	int a, b, c, d;//��������������� ����������, �,d �������� �� ��, ����� �������� ������� ���������� �������, � ����� ���������� ���������
	if (show_must_t == 1)
	{
		a = required_t[p];
		b = required_t[f];
		e--;
		if (e == 0)
			show_must_t = 0;
	}
	else if (setting[0] == true)
	{
		a = delta_t[p];
		b = delta_t[f];
	}
	else
	{
		a = real_t[p];
		b = real_t[f];
	}
	/*a = required_t[p];
	b = required_t[f];*/
	if ((power[p] == false) && (real_t[p] - delta_t[p] > 40))
	{
		for (i = 2; i >= 0; i--)
		{
			array_t[i] = 11;
		}
	}
	else if ((setting[0] == true) && (setting[1] == false))
	{
		a = delta_t[p];
		for (i = 2; i >= 0; i--)
		{
			array_t[i] = a % 10;
			a /= 10;
		}
		array_t[0] = 10;
	}
	else
	{
		for (i = 2; i >= 0; i--) // ���������� ���� � �����
		{
			array_t[i] = a % 10;
			a /= 10;
		}
	}
	if ((power[f] == false) && (real_t[f] - delta_t[f] > 40))
	{
		for (i = 5; i >= 3; i--)
		{
			array_t[i] = 11;
		}
	}
	else if ((setting[0] == true) && (setting[1] == true))
	{
		b = delta_t[f];
		for (i = 5; i >= 3; i--)
		{
			array_t[i] = b % 10;
			b /= 10;
		}
		array_t[3] = 10;
	}
	else
	{
		for (i = 5; i >= 3; i--)
		{
			array_t[i] = b % 10;
			b /= 10;
		}
	}
	for (i = 0; i <= 5;)
	{
		m = array_t[i];
		// ������ � ������� ������������ � �������
		digitalWrite(st2, 0);
		shiftOut(ds, sh, MSBFIRST, mas[i]);
		digitalWrite(st2, 1);
		// ������ � ������� ������������ � ������
		/*  if ((power[p] == true) && (i == 2))//���� �������� ������� - � ��������� ����� ��������� ����� ��� ��������� ������
		{
		*pointer = &cifri2[m];
		}
		else
		{
		*pointer = &cifri[m];
		}
		if ((power[f] == true) && (i == 5))//���� ��� ������� - � ��������� ����� ��������� ����� ��� ��������� ������
		{
		*pointer = &cifri2[m];
		}
		else
		{
		*pointer = &cifri[m];
		}
		digitalWrite(st1, 0);
		shiftOut(ds, sh, MSBFIRST, *pointer);
		digitalWrite(st1, 1);
		*/
		if ((power[p] == true) && (i == 2))//���� �������� ������� - � ��������� ����� ��������� ����� ��� ��������� ������
		{
			digitalWrite(st1, 0);
			shiftOut(ds, sh, MSBFIRST, cifri2[m]);
			digitalWrite(st1, 1);
		}
		else
		{
			digitalWrite(st1, 0);
			shiftOut(ds, sh, MSBFIRST, cifri[m]);
			digitalWrite(st1, 1);
		}
		if ((power[f] == true) && (i == 5))//���� ��� ������� - � ��������� ����� ��������� ����� ��� ��������� ������
		{
			digitalWrite(st1, 0);
			shiftOut(ds, sh, MSBFIRST, cifri2[m]);
			digitalWrite(st1, 1);
		}
		else
		{
			digitalWrite(st1, 0);
			shiftOut(ds, sh, MSBFIRST, cifri[m]);
			digitalWrite(st1, 1);
		}
		delay(3);
		// ������� ���������
		digitalWrite(st1, 0);
		shiftOut(ds, sh, MSBFIRST, 0);
		digitalWrite(st1, 1);

		digitalWrite(st2, 0);
		shiftOut(ds, sh, MSBFIRST, 0);
		digitalWrite(st2, 1);
		i++;
	}
	// delay(10);

}
//------------------����� ������ �� �������---------------------------------------------------

void Filter(void)
{
	//���������� �������� ����������� 
	int x[2];
	x[p] = analogRead(A1);
	x[f] = analogRead(A2);
	for (i = 0; i < 2; i++)
	{
		x[i] = map(x[i], 0, 1024, 0, 512);
		val = x[i] - old_required_t[i];
		if (val > 2 || val < -2)
		{
			required_t[i] = x[i];
			old_required_t[i] = x[i];
			show_must_t = 1;
			e = 250;
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
			real_t[i] = real_t[i] + Max_step_ADC - delta_t[i];
		}
		else if ((old_real_t[i] - x[i]) > Max_step_ADC)// ���� ��� ���������� ������� ������ - �������
		{
			real_t[i] = real_t[i] - Max_step_ADC - delta_t[i];
		}
		else
		{
			real_t[i] = x[i];

		}
		old_real_t[i] = real_t[i];
		real_t[i] += delta_t[i];
	}
	//������ �������� ��� ����
	if (required_t[f] > (real_t[f] + 5))
	{
		del -= 10;
		if (del < 0)
			del = 0;
	}
	if (required_t[f] < (real_t[f] + 5))
	{
		del += 10;
		if (del > 10000)
			del = 10000;
	}
	//����� ������� �������� ��� ����


	/*control_t = millis();
	Serial.print("real_p");
	Serial.println(real_t[p]);
	Serial.print("required_p");
	Serial.println(required_t[p]);
	Serial.print("real_f");
	Serial.println(real_t[f]);
	Serial.print("required_f");
	Serial.println(required_t[f]);
	Serial.print("del");
	Serial.println(del);
	Serial.print("half_sin_flag ");
	Serial.println(half_sin_flag);*/
}

//���������� ����������
void Power_p()
{
	//if (millis() - time_p > time3)
	//{
	char temp = 15;

	//required_p += delta_p;
	if (first_heat_p == 0) //������ ������� � �� ��������� �������� ��� ���������
	{
		if (required_t[p] < (real_t[p] - temp))
		{
			Solid_on;
			Serial.println("mosf_on");
		}
		if (required_t[p] > (real_t[p] - temp))
		{
			Solid_off;
			first_heat_p = 1;
			Serial.println("mosf_off");
		}
	}
	if (required_t[p] > (real_t[p] + 2))
	{
		Solid_on;
		Serial.println("mosf_on");
	}
	if (required_t[p] < real_t[p] + 2)
	{
		Solid_off;
		Serial.println("mosf_off");
	}
	//time_p = millis();
	//}
}
//����� ���������� ����������
//������ ���������� �����
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
	Timer1.restart();                           // ������������� ������
	attachInterrupt(0, detect_down, RISING);  // ������������� ����������
}

void  detect_down() {  // ��������� �������� ���������� �� ����������� ���� ������
	tic = 0;                                  // �������� �������
	Timer1.stop();                           // ���������� ������
	digitalWrite(dim, 0);                  // �������� ���
	attachInterrupt(0, detect_up, FALLING);   // ������������� ����������
}
//----------------------����������� ����������--------------------------
//����� ���������� �����

/*int Wait_MinTime_Pressing(byte but) {
// ���� ������ ������ ������ 50������� - ��� �� ������
//int flag = 1;
unsigned long time = millis() + 50;
while (millis() < time)
{
//������ ��������� ������ ��� ������ 50����
if (digitalRead(but) == 1) { return 0; }
delay(10);
}
return digitalRead(but);
}
*/