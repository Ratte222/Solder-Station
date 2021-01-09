/*int time3 = 250; //как часто вкл ф-я управления паяльником
int time4 = 150; //как часто считываются данные с термопар
byte  but_set = 7, but_up = 8, but_down = 9, but_p = 5, but_f = 6, st1 = 12, sh = 13, st2 = 10, ds = 11; // кнопки настройка, больше, меньше, вкл./выкл. паяльник, вкл./выкл. фен
byte mosfet_p = 4; //мосфет управляющий нагревателем паяльника
byte first_heat_p = 0;//флаг первого нагрева паяльника
byte power_p; //флаги вкл или выкл паяльник и фен
int delta_tp; // погрешность температуры паяльника
int required_tp; //желаемая температура паяльники и фена
int read_tp; //откалиброванная пемпература паяльнка и фена
int old_required_tp;
int tp;//считываемые показания с термопар
unsigned long control_t;// таймер отвечающий за частоту обновления данных с потенциометра для выставления желаемой ттемператуты
unsigned long read_t;// частота считывания с термопар
unsigned long time_p;// частота управления паяльником
unsigned long but_pow;// какое время кнопри вкл/выкл паяльника и фена выключены после их нажатия
int i, n, m, val, e;// е - сколько ещё раз будет показана желаемая температура*/
#define tp 0
#define tf 1
#define Max_step_ADC 15
#define Solid_on digitalWrite(mosfet_p, 1)
#define Solid_off digitalWrite(mosfet_p, 0)
void Filter(void);
int time1 = 250; //задержка при работе с кнопками
int time2 = 150; //как часто считывается АЦП
int time3 = 250; //как часто вкл ф-я управления паяльником
				 //int time4 = 150; //как часто считываются данные с термопар
byte  but_set = 7, but_up = 8, but_down = 9, but_p = 5, but_f = 6, st1 = 12, sh = 13, st2 = 10, ds = 11; // кнопки настройка, больше, меньше, вкл./выкл. паяльник, вкл./выкл. фен
byte mosfet_p = 4; //мосфет управляющий нагревателем паяльника
byte first_heat_p = 0;//флаг первого нагрева паяльника
byte first_heat_f = 0;//флаг первого нагрева фена !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
byte flag_set;//флаг настройки
byte flag_b; //вспомогательный флаг
byte flag_a; //вспомогательный флаг
byte power_p; //флаги вкл или выкл паяльник и фен
byte power_f; // флаги вкл или выкл паяльник и фен 
			  //для какого из инструментов будет меняться калибровачная переменная (дельта)
			  //int delta_tp; // погрешность температуры паяльника
			  //int delta_tf; //погрешность температуры фена
			  //int required_tp; //желаемая температура паяльники и фена
			  //int required_tf; //желаемая температура паяльники и фена
			  //int read_tp; //откалиброванная пемпература паяльнка и фена
			  //int read_tf;//откалиброванная пемпература паяльнка и фена
int delta_t[2]; //погрешность температуры delta_tp, delta_tf
int required_t[2]; //желаемая температура required_tp, required_tf
int real_t[2]; //откалиброванная пемпература real_tp, real_tf
int old_required_t[2]; //старая желаемая температура old_required_tp, old_required_tf
int old_real_t[2]; //старая откалиброванная пемпература
byte show_must_t; //флаг который отвечает за то, когда нужно вывести заданную температуру нагревателей
				  //int old_required_tp;
				  //int old_required_tf;
				  //int tp;//считываемые показания с термопар
				  //int tf; //считываемые показания с термопар
				  //byte addr_dp = 0, addr_df = 1; //адреса погрешности паяльника и фена
int array_cifra[6], array_t[6];// в array_t записывается номер элемента из масива cifri который будет выводиться
unsigned long control_t;// таймер отвечающий за частоту считываний АЦП
						//unsigned long read_t;// частота считывания с термопар
unsigned long time_p;// частота управления паяльником
unsigned long but_pow;// какое время кнопри вкл/выкл паяльника и фена выключены после их нажатия

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
					  int cifri[10][8] = {{0, 1, 0, 1, 1, 1, 1, 1}, //масив с цифрами от 0 до 9
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
					  int mas[6][8] =     {{1, 1, 1, 1, 1, 1, 0, 1}, // масив для регисткра подключенного к катодам чтоб только одна цифра отабражалась в один момент времени
					  {1, 1, 1, 1, 1, 0, 1, 1},
					  {1, 1, 1, 1, 0, 1, 1, 1},
					  {1, 1, 1, 0, 1, 1, 1, 1},
					  {1, 1, 0, 1, 1, 1, 1, 1},
					  {1, 0, 1, 1, 1, 1, 1, 1},
					  };
					  */
int cifri[12] = { 0b01011111, //масив с цифрами от 0 до 9, знак дельта, символ "не полключено"
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
int mas[6] = { 0b11111101, // масив для регисткра подключенного к катодам чтоб только одна цифра отабражалась в один момент времени
0b11111011,
0b11110111,
0b11101111,
0b11011111,
0b10111111
};

int i, n, m, val, e;// е - сколько ещё раз будет показана желаемая температура

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

	//Timer1.initialize(10);
	//Timer1.attachInterrupt(timerIsr);

	//delta_tp = EEPROM.read(addr_dp);
	//delta_tf = EEPROM.read(addr_dp);

	Serial.begin(9600);
}

void loop()
{

	/*
	// входим в режим калибровки, вибираем что будем калибровать, и калибруем-------
	if ((!digitalRead(but_set) == 1) && (Wait_MinTime_Pressing(but_set) == 1))
	{
	flag_set = 1;
	flag_a = 1;
	}
	while (flag_set == 1)
	{
	if ((!digitalRead(but_set)) && (Wait_MinTime_Pressing(but_set)))
	{
	// кнопка нажата точно
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

	flag_a = (flag_a + 1) & 2;     // значение флага увеличивается, а когда оно становится больше 2 приравнивается к 0
	}
	// увеличить значение погрешности
	if ((!digitalRead(but_up)) && (Wait_MinTime_Pressing(but_up)))
	{
	switch (flag_b)
	{
	case 0: {
	delta_tp += 2; // 2 единици - это 1 градус
	if (delta_tp > 198)// значение погрешности не может быть больше 99
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
	//уменьшить значение погрешности
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
	// конец калибровки-----------------------------*/
	//считывание желаемой температуры 

	if (millis() - control_t > time2)
	{
		Filter();
		/*tp = analogRead(A1);
		tf = analogRead(A2);
		val = tp - old_required_tp;
		if (val > 4 || val < 4)
		{
		old_required_tp = tp;
		show_must_t = 1;
		e = 10;
		}
		val = tf - old_required_tf;
		if (val > 4 || val < 4)
		{
		old_required_tf = tf;
		show_must_t = 1;
		e = 10;
		}
		control_t = millis();
		Serial.print("old_required_tp");
		Serial.println(old_required_tp);*/
	}
	//конец считывание желаемой температуры 
	//считываем значение с термопар

	//if (millis() - read_t > time4)
	//{
	//  read_tp = analogRead(A3);
	//  read_tf = analogRead(A4);
	//  delayMicroseconds(100);
	//  read_tp += analogRead(A3);
	//  read_tf += analogRead(A4);
	//  read_t = millis();
	//  //Добавил вычисление среднеарифмметического сюда и не только это!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//  required_tp = map(read_tp, 0, 2047, 0, 512);
	//  required_tf = map(read_tf, 0, 2047, 0, 512);
	//  delta_tp /= 2;
	//  delta_tf /= 2;
	//  required_tp += delta_tp;
	//  required_tf += delta_tf;

	//  Serial.print("required_tp");
	//  Serial.println(required_tp);
	//}
	// конец считывания значений с термопар
	/*  // кнопки вкл/выкл раяльник и фен
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
	//управление паяльником
	//if (power_p == 1)
	Power_p();
}

void Filter()
{
	//считывание желаемой температуры 
	int x[2];
	required_t[tp] = analogRead(A1);
	required_t[tf] = analogRead(A2);
	for (i = 0; i < 2; i++)
	{
		val = required_t[i] - old_required_t[i];
		if (val > 4 || val < 4)
		{
			old_required_t[i] = required_t[i];
			show_must_t = 1;
			e = 10;
		}
	}

	//считываем значение с термопар
	x[0] = analogRead(A3);
	x[1] = analogRead(A4);
	for (i = 0; i < 2; i++)
	{
		if ((x[i] - old_real_t[i]) > Max_step_ADC)// если адц увеличилось слишком сильно - срезать 
		{
			real_t[i] += Max_step_ADC;
			old_real_t[i] += Max_step_ADC;
		}
		else if ((old_real_t[i] - x[i]) > Max_step_ADC)// если адц уменшилось слишком сильно - срезать
		{
			real_t[i] -= Max_step_ADC;
			old_real_t[i] -= Max_step_ADC;
		}
		else
		{
			real_t[i] = x[i];
			old_real_t[i] = x[i];
		}
	}
	control_t = millis();
	Serial.print("real_tp");
	Serial.println(real_t[tp]);
	Serial.print("real_tf");
	Serial.println(real_t[tf]);
	Serial.print("x tp");
	Serial.println(x[tp]);
	Serial.print("x tf");
	Serial.println(x[tf]);
	Serial.print("required_tp");
	Serial.println(required_t[tp]);
	Serial.print("required_tf");
	Serial.println(required_t[tf]);
}

//управление паяльником
void Power_p()
{
	if (millis() - time_p > time3)
	{
		char temp = 15;
		//required_tp += delta_tp;
		if (first_heat_p == 0) //должно нагреть и не перегреть паяльник при влкючении
		{
			if (required_t[tp] < (real_t[tp] - temp))
				Solid_on;
			if (required_t[tp] > (real_t[tp] - temp))
			{
				Solid_off;
				first_heat_p = 1;
			}
		}
		if (required_t[tp] < (real_t[tp] - 5))
			Solid_on;
		if (required_t[tp] > (real_t[tp] + 5))
			Solid_off;
		time_p = millis();
	}
}
int Wait_MinTime_Pressing(byte but) {
	// Если кнопка нажата меньше 50милиСек - она не нажата
	//int flag = 1;  
	unsigned long time = millis() + 50;
	while (millis() < time)
	{
		/*кнопку отпустили раньше чем прошло 50мСек*/
		if (digitalRead(but) == 1) { return 0; }
		delay(10);
	}
	return digitalRead(but);
}