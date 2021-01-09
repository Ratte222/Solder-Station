#define tp 0
#define tf 1
#define Max_step_ADC 15
#define Solid_on digitalWrite(mosfet_p, 1)
#define Solid_off digitalWrite(mosfet_p, 0)
#include <TimerOne.h>
void Filter(void);
void Power_p(void);
//Преременные для фена
byte zero = 2;//подключён для поиска 0 точки
byte dim = 3;// управление симистером
			 //byte fun = 3;// управление вентелятором фена
byte half_sin_flag; //флаг, который позволяел включать прерывания только один раз
int del = 0;//задержка для фена
volatile int tic;//задержка для фена
unsigned long t_sinus;// отсчитывает время от начала синуса если будет ещё одно прерывание в середине синуса
					  //Конец преременных для фена(не всех)
int time1 = 250; //задержка при работе с кнопками
int time2 = 150; //как часто считывается АЦП
int time3 = 250; //как часто вкл ф-я управления паяльником
				 //int time4 = 150; //как часто считываются данные с термопар
byte  but_set = 7, but_up = 8, but_down = 9, but_p = 5, but_f = 6, st1 = 12, sh = 13, st2 = 10, ds = 11; // кнопки настройка, больше, меньше, вкл./выкл. паяльник, вкл./выкл. фен
byte mosfet_p = 4; //мосфет управляющий нагревателем паяльника
byte first_heat_p = 0;//флаг первого нагрева паяльника
byte first_heat_f = 0;//флаг первого нагрева фена 
byte flag_set;//флаг настройки
byte flag_b; //вспомогательный флаг
byte flag_a; //вспомогательный флаг
byte power_p; //флаги вкл или выкл паяльник и фен
byte power_f; // флаги вкл или выкл паяльник и фен 
volatile int delta_t[2]; //погрешность температуры delta_tp, delta_tf
volatile int required_t[2]; //желаемая температура required_tp, required_tf
volatile int real_t[2]; //откалиброванная пемпература real_tp, real_tf
volatile int old_required_t[2]; //старая желаемая температура old_required_tp, old_required_tf
volatile int old_real_t[2]; //старая откалиброванная пемпература
volatile byte show_must_t; //флаг который отвечает за то, когда нужно вывести заданную температуру нагревателей
						   //byte addr_dp = 0, addr_df = 1; //адреса погрешности паяльника и фена
int array_cifra[6], array_t[6];// в array_t записывается номер элемента из масива cifri который будет выводиться
unsigned long control_t;// таймер отвечающий за частоту считываний АЦП
						//unsigned long read_t;// частота считывания с термопар
unsigned long time_p;// частота управления паяльником
unsigned long but_pow;// какое время кнопри вкл/выкл паяльника и фена выключены после их нажатия
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
	//начало переменных для паяльника
	pinMode(A1, INPUT);
	pinMode(A3, INPUT);
	pinMode(mosfet_p, OUTPUT);
	//конец переменных для паяльника
	//начало переменных сегментных экранов
	pinMode(st1, OUTPUT);
	pinMode(st2, OUTPUT);
	pinMode(sh, OUTPUT);
	pinMode(ds, OUTPUT);
	//конец переменных сегментных экранов
	//начало кнопок
	pinMode(but_set, INPUT);
	pinMode(but_up, INPUT);
	pinMode(but_down, INPUT);
	pinMode(but_p, INPUT);
	pinMode(but_f, INPUT);
	//конец кнопок  
	//начало переменных для фена
	pinMode(A2, INPUT);
	pinMode(A4, INPUT);
	pinMode(zero, INPUT);
	pinMode(dim, OUTPUT);
	//  pinMode(fun, OUTPUT);
	//attachInterrupt(0, zero_p, FALLING);
	//конец переменных для фена
	attachInterrupt(0, detect_up, FALLING);  // настроить срабатывание прерывания interrupt0 на pin 2 на низкий уровень
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
		Power_p();
	}

}

void Filter()
{
	//считывание желаемой температуры 
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

	//считываем значение с термопар
	x[0] = analogRead(A3);
	x[1] = analogRead(A4);
	for (i = 0; i < 2; i++)
	{
		//   x[i] = map(x[i], 0, 800, 0, 500);
		if ((x[i] - old_real_t[i]) > Max_step_ADC)// если адц увеличилось слишком сильно - срезать 
		{
			real_t[i] += Max_step_ADC;

		}
		else if ((old_real_t[i] - x[i]) > Max_step_ADC)// если адц уменшилось слишком сильно - срезать
		{
			real_t[i] -= Max_step_ADC;

		}
		else
		{
			real_t[i] = x[i];

		}
		old_real_t[i] = real_t[i];
	}
	//расчёт задержки для фена
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
	//конец расчёта задержки для фена


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

//управление паяльником
void Power_p()
{
	//if (millis() - time_p > time3)
	//{
	char temp = 15;

	//required_tp += delta_tp;
	if (first_heat_p == 0) //должно нагреть и не перегреть паяльник при влкючении
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
//конец укпавления паяльником
/*//Начало управления феном
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
//Конец управления феном
*/
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
//----------------------ОБРАБОТЧИКИ ПРЕРЫВАНИЙ--------------------------
void timer_interrupt() {       // прерывания таймера срабатывают каждые 40 мкс
	tic++;                       // счетчик
	if (tic > del)            // если настало время включать ток
	{
		digitalWrite(dim, 1);   // врубить ток

	}
}

void  detect_up() {    // обработка внешнего прерывания на пересекание нуля снизу
	tic = 0;                                  // обнулить счетчик
	Timer1.detachInterrupt();                           // перезапустить таймер
	Timer1.attachInterrupt(timer_interrupt);
	attachInterrupt(0, detect_down, RISING);  // перенастроить прерывание
}

void  detect_down() {  // обработка внешнего прерывания на пересекание нуля сверху
	tic = 0;                                  // обнулить счетчик
	Timer1.stop();                           // остановить таймер
	digitalWrite(dim, 0);                  // вырубить ток
	attachInterrupt(0, detect_up, FALLING);   // перенастроить прерывание
}
//----------------------ОБРАБОТЧИКИ ПРЕРЫВАНИЙ--------------------------