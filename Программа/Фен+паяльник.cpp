/*int time3 = 250; //��� ����� ��� �-� ���������� ����������
int time4 = 150; //��� ����� ����������� ������ � ��������
byte  but_set = 7, but_up = 8, but_down = 9, but_p = 5, but_f = 6, st1 = 12, sh = 13, st2 = 10, ds = 11; // ������ ���������, ������, ������, ���./����. ��������, ���./����. ���
byte mosfet_p = 4; //������ ����������� ������������ ���������
byte first_heat_p = 0;//���� ������� ������� ���������
byte power_p; //����� ��� ��� ���� �������� � ���
int delta_tp; // ����������� ����������� ���������
int real_tp; //�������� ����������� ��������� � ����
int read_tp; //��������������� ����������� �������� � ����
int old_tp;
int tp;//����������� ��������� � ��������
unsigned long control_t;// ������ ���������� �� ������� ���������� ������ � ������������� ��� ����������� �������� ������������
unsigned long read_t;// ������� ���������� � ��������
unsigned long time_p;// ������� ���������� ����������
unsigned long but_pow;// ����� ����� ������ ���/���� ��������� � ���� ��������� ����� �� �������
int i, n, m, val, e;// � - ������� ��� ��� ����� �������� �������� �����������*/
//����������� ��� ����
byte zero = 2;//��������� ��� ������ 0 �����
byte dim = 1;// ���������� ����������
byte fun = 3;// ���������� ������������ ����
byte hslf_sin_flag; //����, ������� ��������� �������� ���������� ������ ���� ���
unsigned long t_sinus;// ����������� ����� �� ������ ������ ���� ����� ��� ���� ���������� � �������� ������
//����� ����������� ��� ����
int time1 = 250; //�������� ��� ������ � ��������
int time2 = 150; //��� ����� ����������� ������ � ��������������
int time3 = 250; //��� ����� ��� �-� ���������� ����������
int time4 = 150; //��� ����� ����������� ������ � ��������
byte  but_set = 7, but_up = 8, but_down = 9, but_p = 5, but_f = 6, st1 = 12, sh = 13, st2 = 10, ds = 11; // ������ ���������, ������, ������, ���./����. ��������, ���./����. ���
byte mosfet_p = 4; //������ ����������� ������������ ���������
byte first_heat_p = 0;//���� ������� ������� ���������
byte first_heat_f = 0;//���� ������� ������� ���� !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
byte flag_set;//���� ���������
byte flag_b; //��������������� ����
byte flag_a; //��������������� ����
byte power_p; //����� ��� ��� ���� �������� � ���
byte power_f; // ����� ��� ��� ���� �������� � ��� 
			  //��� ������ �� ������������ ����� �������� ������������� ���������� (������)
int delta_tp; // ����������� ����������� ���������
int delta_tf; //����������� ����������� ����
int real_tp; // ����������� ��������� �������� � ���������
int real_tf; //����������� ���� �������� � ���������
int read_tp; //��������������� ����������� �������� � ����
int read_tf;//��������������� ����������� �������� � ����
byte show_must_t; //���� ������� �������� �� ��, ����� ����� ������� �������� ����������� ������������
int old_tp; //����������� �������� �������� ����������� ���������
int old_tf;//����������� �������� �������� ����������� ���� 
int tp;//����������� ��������� � �������������
int tf; //����������� ��������� � �������������
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


	pinMode(zero, INPUT);
	pinMode(dim, OUTPUT);
	pinMode(fun, OUTPUT);
	attachInterrupt(0, zero_p, FALLING);
	//Timer1.initialize(10);
	//Timer1.attachInterrupt(timerIsr);

	//delta_tp = EEPROM.read(addr_dp);
	//delta_tf = EEPROM.read(addr_dp);

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
		tp = analogRead(A1);
		tf = analogRead(A2);
		real_tp = map(read_tp, 0, 1023, 100, 500);
		real_tf = map(read_tf, 0, 1023, 100, 500);
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
		Serial.print("old_tp");
		Serial.println(old_tp);
		Serial.print("old_tf");
		Serial.println(old_tf);
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
		//������� ���������� ���������������������� ���� � �� ������ ���!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		real_tp = map(read_tp, 0, 700, 0, 500);
		real_tf = map(read_tf, 0, 700, 0, 500);
		delta_tp /= 2;
		delta_tf /= 2;
	//	real_tp += delta_tp;
	//	real_tf += delta_tf;

		Serial.print("real_tp");
		Serial.println(real_tp);
		Serial.print("real_tf");
		Serial.println(real_tf);
	}
	// ����� ���������� �������� � ��������
	/*	// ������ ���/���� �������� � ���
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
	}*/
	//���������� ����������
	//if (power_p == 1)
	Power_p();
}
//���������� ����������
void Power_p()
{
	if (millis() - time_p > time3)
	{
		char temp = 40;
		//real_tp += delta_tp;
		if (first_heat_p == 0) //������ ������� � �� ��������� �������� ��� ���������
		{
			if (real_tp < old_tp - temp)
				digitalWrite(mosfet_p, 1);
			if (real_tp > old_tp - temp)
			{
				digitalWrite(mosfet_p, 0);
				first_heat_p = 1;
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
//������ ���������� �����
void Power_f()
{
	int del = 0;
	if (real_tf < old_tf - 10)
	{
		del--;
		if (del < 0)
			del = 0;
	}
	if (real_tf > old_tf + 10)
	{
		del--;
		if (del < 0)
			del = 0;
	}
	if (hslf_sin_flag == 1)
		del = del - (millis() - t_sinus);
	delayMicroseconds(del);
	digitalWrite(dim, 1);
	delayMicroseconds(500);
}
void zero_p()
{
	if (old_tf < 512)
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
	hslf_sin_flag = 1;
	Power_f();
}
//����� ����� ��� ����



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