
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
	0b00000000,//пустота 11element
	0b00010000,//_ 12 element
	0b11001111,//A
	0b11011010,//b 14 element
	0b00011110,//C
	0b11011001,//d
	0b10011110,//E 17 element
	0b10001110,//F
	0b11001010,//h 19 element
	0b01010001,//J 20 element
	0b11001011,//H имеентся ввиду к
	0b00011010,//L 22 element
	//не знаю как изобразить "М"
	0b11001000,//n 23 element
	0b10001111,//P 24 element
	0b11000111,//q 25 element
	0b10001000,//r 26 element
	0b10011010,//t 27 element
	0b11010011//У 28 element

};


#include <TimerOne.h>
#include <EEPROM.h>
int time1 = 250; //задержка при работе с кнопками
int time2 = 150; //как часто считываются данные с потенциометров
int time3 = 250; //как часто вкл ф-я управления паяльником
int time4 = 150; //как часто считываются данные с термопар
byte  but_set = 7, but_up = 8, but_down = 9, but_p = 5, but_f = 6, st1 = 12, sh = 13, st2 = 10, ds = 11; // кнопки настройка, больше, меньше, вкл./выкл. паяльник, вкл./выкл. фен
byte mosfet_p = 4; //мосфет управляющий нагревателем паяльника
byte first_heat = 0;//флаг первого нагрева паяльника
byte flag_set;//флаг настройки
byte flag_b; //вспомогательный флаг
byte flag_a; //вспомогательный флаг
byte power_p; //флаги вкл или выкл паяльник и фен
byte power_f; // флаги вкл или выкл паяльник и фен 
			  //для какого из инструментов будет меняться калибровачная переменная (дельта)
int delta_tp; // погрешность температуры паяльника
int delta_tf; //погрешность температуры фена
int real_tp; //желаемая температура паяльники и фена
int real_tf; //желаемая температура паяльники и фена
int read_tp; //откалиброванная пемпература паяльнка и фена
int read_tf;//откалиброванная пемпература паяльнка и фена
byte show_must_t; //флаг который отвечает за то, когда нужно вывести заданную температуру нагревателей
int old_tp;
int old_tf;
int tp;//считываемые показания с термопар
int tf; //считываемые показания с термопар
byte addr_dp = 0, addr_df = 1; //адреса погрешности паяльника и фена
int array_cifra[6], array_t[6];// в array_t записывается номер элемента из масива cifri который будет выводиться
unsigned long control_t;// таймер отвечающий за частоту обновления данных с потенциометра для выставления желаемой ттемператуты
unsigned long read_t;// частота считывания с термопар
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

	Timer1.initialize(10);
	Timer1.attachInterrupt(timerIsr);

	delta_tp = EEPROM.read(addr_dp);
	delta_tf = EEPROM.read(addr_dp);

}

void loop()
{


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
	// конец калибровки-----------------------------
	//считывание желаемой температуры 

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
	//конец считывание желаемой температуры 
	//считываем значение с термопар

	if (millis() - read_t > time4)
	{
		read_tp = analogRead(A3);
		read_tf = analogRead(A4);
		delayMicroseconds(100);
		read_tp += analogRead(A3);
		read_tf += analogRead(A4);
		read_t = millis();
	}
	// конец считывания значений с термопар
	// кнопки вкл/выкл раяльник и фен
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
	//управление паяльником
	if (power_p == 1)
		Power_p();
}
//управление паяльником
void Power_p()
{
	if (millis() - time_p > time3)
	{
		char temp = 40;
		real_tp += delta_tp;
		if (first_heat == 0) //должно нагреть и не перегреть паяльник при влкючении
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
//конец управление паяльником
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


void timerIsr()
{

	int a, b, c, d;//вспомагательные переменные, с,d отвечает за то, какие елементы массива заполнятся цифрами, а какие служебными символами
				   //расчитываем реальную температуру паяльника и фена
	real_tp = map(read_tp, 0, 1023, 0, 512);
	real_tf = map(read_tf, 0, 1023, 0, 512);
	delta_tp /= 2;
	delta_tf /= 2;
	real_tp += delta_tp;
	real_tf += delta_tf;
	if (flag_set == 0) // вывод значений температуры
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
	for (; i >= 0; ) // вписывание цифр в масив
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

	if (flag_set == 1) // вывод погрешности температуры при калибровке
	{
		a = delta_tp;
		b = delta_tf;
		if (flag_b == 1) // если калибруется паяльник вывести знак "дельта" перед значением погрешности паяльника
		{
			array_t[0] = 10;
			c = 1;
			d = 3;
		}
		if (flag_b == 0) // если калибруется фен вывести знак "дельта" перед значением погрешности фена
		{
			array_t[3] = 10;
			c = 0;
			d = 4;
		}
		i = 2;
		for (; i >= c; ) // вписывание цифр и символов в масив
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
		/* switch(array_t[i]) //выбор елемента который будет выводиться
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
		// запись в регистр подключенный к катодам
		digitalWrite(st2, 0);
		shiftOut(ds, sh, MSBFIRST, mas[i]);
		digitalWrite(st2, 1);
		// запись в регистр подключенный к анодам
		digitalWrite(st1, 0);
		shiftOut(ds, sh, MSBFIRST, cifri[m]);
		digitalWrite(st1, 1);

		delay(1);
		// очистка регистров
		digitalWrite(st1, 0);
		shiftOut(ds, sh, MSBFIRST, 0);
		digitalWrite(st1, 1);

		digitalWrite(st2, 0);
		shiftOut(ds, sh, MSBFIRST, 0);
		digitalWrite(st2, 1);
		i++;
	}
}
