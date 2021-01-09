/*
 * Debug.cpp
 *
 * Created: 18.08.2018 10:00:54
 *  Author: User
 */ 
#include "main.h"
PID_Regulator Dryer(1.5, 2.5, 2, 0.0163);
PID_Regulator Solid(4, 8, 6, 0.0163);
Button but_p(5, 15);//PD5 кнопки настройка, вкл./выкл. паяльник, вкл./выкл. фен
Button but_f(6, 15);//PD
Button but_set(7, 15);//PD7
Button but_up(8, 10);//PB0 больше, меньше,
Button but_down(9, 10);//PB1
Button gercon(0,10);
//MENU-------------------------------------------------------------------

enum MENU_StateTypeDef {//возможные состояния программы
	MENU_STATE_WAIT = 0,
	MENU_STATE_MAIN
};
enum MAIN_MENU_StateTypeDef  {//всевозможные состояния при работе с меню
	MAIN_MENU_STATE_IDLE = 1,
	MAIN_MENU_STATE_DEBUG_WAIT,
	MAIN_MENU_STATE_DEBUG,
	MAIN_MENU_STATE_FACTOR_p_WAIT,
	MAIN_MENU_STATE_FACTOR_p,
	MAIN_MENU_STATE_FACTOR_f_WAIT,
	MAIN_MENU_STATE_FACTOR_f,
	MAIN_MENU_STATE_DEFAULT_FACTOR_p_WAIT,
	MAIN_MENU_STATE_DEFAULT_FACTOR_p,
	MAIN_MENU_STATE_DEFAULT_FACTOR_f_WAIT,
	MAIN_MENU_STATE_DEFAULT_FACTOR_f,
	MAIN_MENU_STATE_DELTA_p_WAIT,
	MAIN_MENU_STATE_DELTA_p,
	MAIN_MENU_STATE_DELTA_f_WAIT,
	MAIN_MENU_STATE_DELTA_f,
	MAIN_MENU_STATE_DEFAULT_DELTA_p_WAIT,
	MAIN_MENU_STATE_DEFAULT_DELTA_p,
	MAIN_MENU_STATE_DEFAULT_DELTA_f_WAIT,
	MAIN_MENU_STATE_DEFAULT_DELTA_f,
	MAIN_MENU_STATE_MODE_p_Wait,
	MAIN_MENU_STATE_MODE_p,
	MAIN_MENU_STATE_MODE_f_WAIT,
	MAIN_MENU_STATE_MODE_f,
	MAIN_MENU_STATE_EXIT
};

MENU_StateTypeDef menu_state = MENU_STATE_WAIT;
MAIN_MENU_StateTypeDef main_menu_State = MAIN_MENU_STATE_IDLE;
//MENU-------------------------------------------------------------------
int del;//задержка для фена
volatile int tic;//задержка для фена

byte debug_flag = 0;//если этот флаг поднят - включится режим отладки в котором можно проверить работоспособбность "железа"
byte interrupt_flag = 0;//этот флаг нужен для отслеживания внешних прерываний и если ех нет когда должны быть - пробивать ошибку

byte flag_fan; // если == 1 - программа ждёт остывания вена перед выключением вентилятора в фене
//ошибки заносятся в соотвецтвующие биты т.е. если есть ошибка 0 тогда байт будет выглядеть так: 0b00000001
byte error_p = 0;//ошибки для паяльника:0 - не совпадает контрольная сумма EEPROM 1 -  перегревает
byte error_f = 0;//ошибки для фена 0 - нет внешних прерываний 1 - перегревает
//ошибки выводятся по важности
byte power[2]; // флаги вкл или выкл паяльник и фен
byte mode[2];//выбирает как нужно работать. для паяльника управление шимом или вкл/выкл, для фена игнорировать геркон или нет
int state = 0;//нужен при использовании настроек
int delta_t[2]; //погрешность температуры delta_p, delta_f
int factor[2];// на сколько умножать адц для получение корректной температуры 

int required_t[2]; //желаемая температура в градусах required_p, required_f
int adc[4];

int real_t[2]; //откалиброванная пемпература real_p, real_f


#ifdef Stripped_down

word addres_eeprom_0_ = 0;//delta_t[p]
word addres_eeprom_2_ = 2;//delta_t[f]
word addres_eeprom_10_ = 10;//check sum

#else

byte addres_eeprom_4 = 4;//mode[p]
byte addres_eeprom_5 = 5;//mode[f]
word addres_eeprom_0_ = 0;//delta_t[p]
word addres_eeprom_2_ = 2;//delta_t[f]
word addres_eeprom_6_ = 6;//factor[p]
word addres_eeprom_8_ = 8;//factor[f]
word addres_eeprom_10_ = 10;//check sum

#endif

dword time = 0;//показывает время от начала работы, 1 сек = 61
byte array_t[6];// в array_t записывается номер элемента из масива cifri который будет выводиться

const PROGMEM byte cifri[30] = { 0b01011111, //0,O
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
	0b11010011,//y 28 element
	0b1011000//u 29 element
};
const PROGMEM byte mas[6] = { 0b11111101, // массив для регистра подключенного к катодам чтоб только одна цифра отабражалась в один момент времени
	0b11111011,
	0b11110111,
	0b11101111,
	0b11011111,
	0b10111111
};
const PROGMEM byte array_deb[6][3] = {
	{14, 12, 24},//b_p
	{14, 12, 18},//b_F
	{14, 12, 5},//b_5
	{14, 12, 29},//b_u
	{14, 12, 16},//b_d
	{8, 17, 26}//8Er - ger
};
#if defined(__AVR_ATmega168__) \
|| defined(__AVR_ATmega328__) \
|| defined(__AVR_ATmega328P__)
const PROGMEM byte array_reason_reset[5][3] = {
	{11, 16, 26},// dr - watchdog system reset
	{14, 0, 26},//bOr -		brown-out reset
	{17, 27, 26},//Etr - external reset
	{24, 0, 26},//POr - power-on reset
	{26, 26, 11} //rr - reason reset
};
#endif

const PROGMEM byte array_maim_menu[13][3] = {
	{11, 23, 0},//nO
	{28, 17, 5},//Yes
	{16, 17, 14},//deb - debug
	{18, 12, 24},//f_p - set factor p
	{18, 12, 18},//f_f - set factor f
	{16, 18, 24},//dfp - default factor p
	{16, 18, 18},//dff - default factor f
	{16, 12, 24},//d_p - set delta p
	{16, 12, 18},//d_f - set delta f
	{16, 16, 24},//ddp - default delta p
	{16, 16, 18},//ddf - default delta f
	{12, 12, 12},// ___
	{10, 10, 10}//---
};
void EmptyProc(void){};//пустая функция и такое тоже иногда нужно

void Time(void)//реальное время 61 у.е. = 1 с
{
	time++;
}

 byte buttonCheck(byte but)//провераят нажимание кнопки
 {
	 switch (but)
	 {
		 case 0:
		 return but_p.isClick();
		 break;
		 case 1:
		 return but_f.isClick();
		 break;
		 case 2:
		 return but_set.isClick();
		 break;
		 case 3:
		 return but_up.isClick();
		 break;
		 case 4:
		 return but_down.isClick();
		 break;
	 }
	 return 0;
 }

//----------------Работа с паяльником--------------------------------

void OVF(void)//это ф-я которая вызывается при переполнении первого таймера
{
	Mosfet_on;
}

void COMP(void)//а эта по компаратору первого таймера. Можно конечто и в векторах сразу было это написать но так вроде бы красивее и понятнее
{
	Mosfet_off;
}

void solidOn(void)//включает все что нужно если регулировка паяльника будет осуществляться ШИМом
{
	if (mode[p])
	{
		//устанавливает нужный делитель после чего таймер начинает считать
		Timer1_Ovf_Ena;//разрешает прерывания по переполнению
		Timer1_COMPA_Ena;//разрешает прерывания по совпадению  в компараторе
	}
}

void solidOff(void)//выключает всё и опускает флаги связанные с паятьником(для удобства)
{
	if (mode[p])
	{
		Timer1_Ovf_Dis;
		Timer1_COMPA_Dis;
		Mosfet_off;
		Solid.Reset_Value();
	}
	else
	{
		Mosfet_off;
	}
	power[p] = 0;//на всякий случай и для удобства
}

void powerP(void)//сама функция регулировка
{
	if (mode[p])//регулировка ШИМом
	{
		long PWM = Solid.PID_Calculation(&required_t[p], &real_t[p]);
		PWM = PWM << 7;
		if (PWM > 65535)
		{
			CompareReg_1A = 65535;
		} 
		else if(PWM >= 8000)
		{
			CompareReg_1A = PWM;
		}
		else
		{
			CompareReg_1A = 8000;
		}
	}
	else//а тут нагреватель паяльника тупо вкл/выкл 
	{//температура плавает сильнее
		if (required_t[p] > (real_t[p] + 2))
		{
			Mosfet_on;
		}
		if (required_t[p] < (real_t[p] + 2))
		{
			Mosfet_off;
		}
	}
}
//----------------Работа с паяльником--------------------------------

//----------------------Работа с феном---------------------------------
#define resumeCountTimer2 Timer2_Ovf_Ena
#define stopCountTimer2 Timer2_Ovf_Dis
#define Dryer_On Fan_enable; attachInterrupt0

void dryerOff(void)//всё что нужно сделать чтоб фен точно не работал и программа не думала что он работает
{
	detachInterrupt0;
	stopCountTimer2;
	Semister_off;
	flag_fan = true;//этот флаг нужен чтоб вентилятор не сразу выключился, а только когда температура фена опустится до заданной
	//отслеживание этого происходит в главной ф-и
	power[f] = 0;	
	Dryer.Reset_Value();
}
//----------------------ОБРАБОТЧИКИ ПРЕРЫВАНИЙ--------------------------
void timerInterrupt() {       // прерывания таймера срабатывают каждые 40 мкс
	tic++;                       // счетчик
	if (tic > del)            // если настало время включать ток
	{
		Semister_on;   // врубить ток
	}
}

void  detectDown();
void  detectUp() {    // обработка внешнего прерывания на пересекание нуля снизу
	tic = 0;                                  // обнулить счетчик
	resumeCountTimer2;                           // перезапустить таймер
	Int0_RISING;  // перенастроить прерывание
	GetINT0_Vect(detectDown);
}

void  detectDown() {  // обработка внешнего прерывания на пересекание нуля сверху
	tic = 0;                                  // обнулить счетчик
	stopCountTimer2;                             // остановить таймер
	Semister_off;                  // вырубить ток
	Int0_FALLING;  // перенастроить прерывание
	GetINT0_Vect(detectUp);
	interrupt_flag = 1;//нужно для отслеживания работы и пробития ошибок, если нужно
}
//----------------------ОБРАБОТЧИКИ ПРЕРЫВАНИЙ--------------------------
//----------------------Работа с феном---------------------------------

//----------Считывание и конвертация АЦП--------------------------------------
void filter(void)//тут считывается адц и фильтруется но только для значений термопар
{
  byte i;
  int r = 0;
  static int old_adc[4];
  for (i = 0; i < 4; i++)
  {
  r = ADC_convert(i+1);
  //   x[i] = map(x[i], 0, 800, 0, 500);
  if ((r - old_adc[i]) > Max_step_ADC)// если адц увеличилось слишком сильно - срезать
  {
	adc[i] = adc[i] + Max_step_ADC;
  }
  else if ((old_adc[i] - r) > Max_step_ADC)// если адц уменшилось слишком сильно - срезать
  {
	adc[i] = adc[i] - Max_step_ADC;
  }
  else//адц отличное, сразу присвоим его нужной переменной
  {
	adc[i] = r;
  }
	old_adc[i] = adc[i];
  }
}

void konvert(void)//тут конвертируется значение с термопар в напряжение и расчитывается, если нужно, время задержки для фена 
{	
	filter();
	required_t[p] = adc[0] >> 1;
	required_t[f] = adc[1] >> 1; 
	real_t[p] = (int)((long)(adc[2] * factor[p]) / 100);//long чтоб небыло переполнения
	if (delta_t[p] > real_t[p])
	{
		real_t[p] = 0;
	}
	else
	{
		real_t[p] -= delta_t[p];	
	}	 
	real_t[f] = (int)((long)(adc[3] * factor[f]) / 100);
	real_t[f] += delta_t[f];
	if (power[f])
	{//чем больше число - тем большую часть синуса срежет
		int u = (int)Dryer.PID_Calculation(&required_t[f], &real_t[f]);
		if (u < 50)
		{
			del = 575;
		} 
		else if (u <= 575)
		{
			del = 625 - u;
		}
		else
		{
			del = 50;
		}
	
	}
	//конец расчёта задержки для фена
}
//----------Считывание и конвертация АЦП--------------------------------------
//---------------------------------Вывод на экран-------------------------------

void SPIOutput(void)//ф-я вывода, которая отправляет нужные байты в сдвиговые регистры
{
	for (byte i = 0; i < 6; i++)//поциферно(символьно) передает значения для вывода
	{
		byte  k = pgm_read_byte(&(cifri[array_t[i]]));
		if ((power[p] == true) && (i == 2))//если паяльник включён - у последней цифри включится точка для индикации работы
		{
			k |= (1<<5);//ставить бит отвечающий за сегмент "."
		}
		else if ((power[f] == true) && (i == 5))//если фен включён - у последней цифри включится точка для индикации работы
		{
			k |= (1<<5);//ставить бит отвечающий за сегмент "."
		}
		// запись в регистр подключенный к анодам
		SPI_Data_Bufer = k;//закидывает байт в регистр SPI и он сразу начинает его передавать по шине
		while(SPI_Transmitted);//ждём пока передаст
		// запись в регистр подключенный к катодам
		SPI_Data_Bufer = pgm_read_byte(&(mas[i]));
		while(SPI_Transmitted);
		Save_Data_in_ShiftRegister;
		_delay_us(100);
	}
}
//---------------------------------Вывод на экран-------------------------------
//---------------------------------Заполнение массива, который выведется на экран----------------
static void checkErrors(void)//проверяем есть ли проблемы и если в массиве есть биты в "1"
{// он запишет в массив, который используется для выбора бита отвечающего за выводимый символ 
	static dword time_error = 0;
	
	if ((time - time_error) > 30)
	{
		if (power[f])//проверка на внешние прерывания
		{
			static byte flag_error;
			if (interrupt_flag)
			{
				interrupt_flag = 0;
				flag_error = 0;
			}
			else
			{
				if (flag_error)
				{
					dryerOff();
					BitSet(error_f, 0);//нет внешних прерываний
				}
				else
				{
					interrupt_flag = 0;
					flag_error = 1;
				}
			}
		}
		
		if (power[p])
		{
			//static dword time_work = 0;
			static byte flag_error = 0;
			//if (time > (time_work + 61))//проверим через секунду
			//{
			if ((real_t[p] - required_t[p]) > 30)//паяльник сильно перегревает относительно желаемой температуры
			{
				if (flag_error)
				{
					solidOff();
					BitSet(error_p, 1);
				}
				else
				{
					//time_work = time;
					flag_error = 1;
				}
			}
			else
			{
				//time_work = time;
				flag_error = 0;
			}
			//}
		}
		
		if (power[f])
		{
			//static dword time_work = 0;
			static byte flag_error = 0;
			//if (time > (time_work + 61))//проверим через секунду
			//{
			if ((real_t[f] - required_t[f]) > 30)//фен сильно перегревает относительно желаемой температуры
			{
				if (flag_error)
				{
					dryerOff();
					BitSet(error_f, 1);
				}
				else
				{
					//time_work = time;
					flag_error = 1;
				}
			}
			else
			{
				//time_work = time;
				flag_error = 0;
			}
			//}
		}
		
		time_error = time;
	}
	byte i = 8;
		do 
		{
			if(BitTest(error_p, (i-1)))
			{
			array_t[0] = 17;
			array_t[1] = 26;
			array_t[2] = i-1;//er и номер ошибки
			}
			if(BitTest(error_f, (i-1)))
			{
			array_t[3] = 17;
			array_t[4] = 26;
			array_t[5] = i-1;//er и номер ошибки
			}
			i--;
		} while (i);
			
}

void writeArray(byte i, byte arr, byte n)
{
	for (byte j = 0; j < 3; j++)
	{
		switch (arr)
		{
			case 0:
				array_t[j+i] = pgm_read_byte(&(array_deb[n][j]));
				break;
			case 1:
				array_t[j+i] = pgm_read_byte(&(array_maim_menu[n][j]));
				break;
			#if defined(__AVR_ATmega168__) \
			|| defined(__AVR_ATmega328__) \
			|| defined(__AVR_ATmega328P__)
			case 2:
				array_t[j+i] = pgm_read_byte(&(array_reason_reset[n][j]));
				break;
			#endif
		}
		
	}
}

byte yesOrNo(byte o)//это нужно для уменшения объема программы
{
	if (o)
	{
		writeArray(3, D_array_maim_menu, state);
	} 
	else
	{
		writeArray(0, D_array_maim_menu, state);
	}
	
	return 1;
}
void screen(void)//эта ф-я отвечает за то, что должны отобразить каждый из сегментов
{
	static dword time_; //переменная, которая используется для отображения желаемон температуры некоторое время
	static int old_required[2];//нужна для реагирование при изменении "желаемой температуры"
	//убрал массивы из-за попыток сокращения объёма программы
	volatile byte flag_count_p = 0, flag_count_f = 0;//если он поднят - то res_р или res_f будет разбит на цифры и записан в массив array_t
	//word res[2] = {0,0};
	volatile word res_p = 0, res_f = 0;//сюда записывать числа длля последущего розбития на цифри и вывода на экран
	int i;
	byte flag_p = 0, flag_f = 0;//ели он поднят - запись в массив array_t не будет
	if (menu_state == MENU_STATE_MAIN)//если мы в меню - мыводим то, что редактирует
	{		
		switch((byte)main_menu_State)
		{
			case MAIN_MENU_STATE_DEBUG_WAIT:
				flag_f = yesOrNo(f);
				break;
			case MAIN_MENU_STATE_FACTOR_p_WAIT:
				flag_count_p = 1;
				res_p = state;
				break;
			case MAIN_MENU_STATE_FACTOR_f_WAIT:
				flag_count_f = 1;
				res_f = state;
				break;
			case MAIN_MENU_STATE_DEFAULT_FACTOR_p_WAIT:
				flag_p = yesOrNo(p);
				break;
			case MAIN_MENU_STATE_DEFAULT_FACTOR_f_WAIT:
				flag_f = yesOrNo(f);
				break;
			//*****************************
			case MAIN_MENU_STATE_DELTA_p_WAIT:
				flag_count_p = 1;
				res_p = state;
				break;
			case MAIN_MENU_STATE_DELTA_f_WAIT:
				flag_count_f = 1;
				res_f = state;
				break;
			case MAIN_MENU_STATE_DEFAULT_DELTA_p_WAIT:
				flag_p = yesOrNo(p);
				break;
			case MAIN_MENU_STATE_DEFAULT_DELTA_f_WAIT:
				flag_f = yesOrNo(f);
				break;
			case MAIN_MENU_STATE_MODE_p_Wait:
				flag_count_p = 1;
				res_p = state;
				break;
			case MAIN_MENU_STATE_MODE_f_WAIT:
				flag_count_f = 1;
				res_f = state;
				break;
		}
		if (flag_count_p || flag_p)
		{
			writeArray(3, D_array_maim_menu, ((((byte)main_menu_State)>>1)+1));
		}
		if (flag_count_f || flag_f)
		{
			writeArray(0, D_array_maim_menu, ((((byte)main_menu_State)>>1)+1));
		}
	}
	else if(menu_state == MENU_STATE_WAIT)//если меню не работает
	{
		int val;
		for (i = 0; i < 2; i++)//если начали крутить регулятор температуры. будет показывать значение потенциометров ещё 2 сек после того,
		// как их значение будет неизменным или меньше пределов
		{
			val = old_required[i] - required_t[i];
			if ((val > 3) || (val < -3))//проверим не крули ли потенциометры отвечающие за желаемую температуру
			{
			time_ = time;
			old_required[i] = required_t[i];
			}
		}
		
		if((time - time_) < 122) //если ещё не прошло заданное время полсе того, как регуляторы
		// температуры перестали изменять значения
		{
			flag_count_p = 1;
			flag_count_f = 1;
			res_p = required_t[p];
			res_f = required_t[f];
		}
		else
		{	
				if((power[p] == false) && (adc[2] > 650))//если паяльник не подключен выведем "---"
				{
					flag_count_p = 0;
					writeArray(0, D_array_maim_menu, 12);
				}
				{
					flag_count_p = 1;
					res_p = real_t[p];
				}				
				if((power[f] == false) && (adc[3] > 650))//если фен не подключен выведем "---"
				{
					flag_count_f = 0;
					writeArray(3, D_array_maim_menu, 12);
				}
				else
				{
					flag_count_f = 1;
					res_f = real_t[f];
				}
		}
	}
	if (flag_count_p)
	{
		for (i = 2; i >= 0; i--) // вписывание цифр в масив
		{
			array_t[i] = res_p % 10;
			res_p /= 10;
		}
	}
	if (flag_count_f)
	{
		for (i = 5; i >= 3; i--)
		{
			array_t[i] = res_f % 10;
			res_f /= 10;
		}
	}
	if (menu_state == MENU_STATE_WAIT)//перезапишет значения массива array_t если есть ошибки и программа не находится в меню
	{
		checkErrors();
	}
 }
 
void debug(void)//эта ф-я нужна для проверки "железа"
{
	static dword time_deb = 0;
	//time_deb = time;
	static byte i = 0;
	if (i < 12)//выведет цифры 0-9,спц символ "-" и остановится на "пустом экране"
	{
		if (time>(time_deb + 61))
		{
			for (byte j = 0; j < 6; j++)
			{
				array_t[j] = i;
			}
			time_deb = time;
			i++;
		}
	}
	else//можно понажимать кнопки, если он видит нажатие - выводит соотвецтвующее уведомление
	{
		if (time<(time_deb + 1000))//3660 - 1 min
		{
			static dword time_see = 0;
			if (time>(time_see + 61))
			{
				for (byte j = 0; j < 6;j++)
				{
					if (buttonCheck(j))
					{
						writeArray(0, D_array_deb, j);
						break;
					}
					
				}
				_delay_ms(6);
			}
		}
		else
		{
			debug_flag = 0;
		}
	}
}

//---------------------------------Заполнение массива, который выведется на экран-------------------------------
//------------------Меню и обработка кнопок-------------------------------------
void menuWait(void)//эта функция ждёт включения меню и обрабатывает вкл/выкл паяльника и фена
{
	//Output(8,0);
	if (buttonCheck(b_set))//проверка кнопки сет
	{
		menu_state = MENU_STATE_MAIN;
		main_menu_State = MAIN_MENU_STATE_IDLE;
		power[p] = 0;
		power[f] = 0;
		solidOff();
		dryerOff();
	}
	//Output(9,0);
	if (buttonCheck(b_p))
	{
		power[p] = !power[p];
		if (power[p])
		{
			solidOn();
		} 
		else
		{
			solidOff();
		}
	}
	if (mode[f])// у фена от мода зависит подключёт ли геркон. если мод == 1 то геркон подключен
	{
		if (buttonCheck(b_f))
		{
			power[f] = !power[f];
			if (power[f])
			{
				Dryer_On;
			} 
			else
			{
				dryerOff();
			}
		}
		else
		{
			if (gercon.flagPress)
			{
				if (power[f])
				{
					dryerOff();
				}
			} 
			else
			{
				if (power[f] == 0)
				{
					power[f] = 1;
					//firstHeatF();
					Dryer_On;
				}
			}
		}
	} 
	else
	{
		if (buttonCheck(b_f))
		{
			power[f] = !power[f];
			if (power[f])
			{
				Dryer_On;
			}
			else
			{
				dryerOff();
			}
		}
	}
	if (buttonCheck(b_down))
	{
			error_p = 0;
			error_f = 0;
	}
}

int valNonVal(int *pointer)//эта ф-я написана бля уменшения объёма программы и увеличения читабельности
{
	int val = *pointer;
	if (buttonCheck(b_up))
	{
		val = !val;
	}
	if (buttonCheck(b_down))
	{
		val = !val;
	}
	if (buttonCheck(b_set))
	{
		main_menu_State=(MAIN_MENU_StateTypeDef)(((byte)main_menu_State)+1);
	}
	return val;
}

int valIncrementDecrement(int *pointer)//эта ф-я написана бля уменшения объёма программы и увеличения читабельности
{
	int val = *pointer;
	if (buttonCheck(b_up))
	{
		val++;
	}
	if (buttonCheck(b_down))
	{
		val--;
	}
	if (buttonCheck(b_set))
	{
		main_menu_State=(MAIN_MENU_StateTypeDef)(((byte)main_menu_State)+1);
	}
	return val;
}

void menuMain(void)//меню большое и старшное
{
	//есть 2 варианта состояний ожидание ввода и его обработка(в конце WAIT) и применение выбора
		switch ((byte)main_menu_State)//byte написан для того, чтоб компилятор не говорил - "часть перечесления не используется"
		{
			case MAIN_MENU_STATE_IDLE:
				//val = eeprom_read_word(&addres_eeprom_0_);
				//i++;
				state = 0;
				main_menu_State = MAIN_MENU_STATE_DEBUG_WAIT;
				break;	
			case MAIN_MENU_STATE_DEBUG_WAIT:
				state = valNonVal(&state);
				break;
			case MAIN_MENU_STATE_DEBUG:
				if(state == 1)
					{
						debug_flag = 1;
						main_menu_State = MAIN_MENU_STATE_EXIT;
					}
				else
				{
					state = eeprom_read_word(&addres_eeprom_6_);
					//state = eeprom_read_word(pgm_read_word(&(array_addr_eep_w[3])));
					main_menu_State = MAIN_MENU_STATE_FACTOR_p_WAIT;
				}
				break;
			case MAIN_MENU_STATE_FACTOR_p_WAIT:
				state = valIncrementDecrement(&state);
				break;
			case MAIN_MENU_STATE_FACTOR_p:
				//val = delta_t[p];
				if(state != eeprom_read_word(&addres_eeprom_6_))
				{
					//delta_t[p] = val;
					eeprom_write_word(&addres_eeprom_6_, state);
					//val = state;
					factor[p] = state;
				}
				//val = eeprom_read_word(&addres_eeprom_2_);
				//i++;
				state = eeprom_read_word(&addres_eeprom_8_);
				main_menu_State = MAIN_MENU_STATE_FACTOR_f_WAIT;
				break;
			case MAIN_MENU_STATE_FACTOR_f_WAIT:
				state = valIncrementDecrement(&state);
				break;
			case MAIN_MENU_STATE_FACTOR_f:
				if(state != eeprom_read_word(&addres_eeprom_8_))
				{
					//delta_t[f] = val;
					eeprom_write_word(&addres_eeprom_8_, state);
					//val = state;
					factor[f] = state;
				}
				//val = 0;
				//i++;
				state = 0;
				main_menu_State = MAIN_MENU_STATE_DEFAULT_FACTOR_p_WAIT;
				break;
			case MAIN_MENU_STATE_DEFAULT_FACTOR_p_WAIT:
				state = valNonVal(&state);
				break;
			case MAIN_MENU_STATE_DEFAULT_FACTOR_p:
				if(state == 1)
				{
					factor[p] = Default_Factor_p;
					eeprom_write_word(&addres_eeprom_6_, factor[p]);
				}
				state = 0;
				//i++;
				main_menu_State = MAIN_MENU_STATE_DEFAULT_FACTOR_f_WAIT;
				break;
			case MAIN_MENU_STATE_DEFAULT_FACTOR_f_WAIT:
				state = valNonVal(&state);
				break;
			case MAIN_MENU_STATE_DEFAULT_FACTOR_f:
				if(state == 1)
				{
					factor[f] = Default_Factor_f;
					eeprom_write_word(&addres_eeprom_8_, factor[f]);
				}
				state = delta_t[p];
				//i++;
				main_menu_State = MAIN_MENU_STATE_DELTA_p_WAIT;
				break;
			case MAIN_MENU_STATE_DELTA_p_WAIT:
				state = valIncrementDecrement(&state);
				break;
			case MAIN_MENU_STATE_DELTA_p:			
				//val = delta_t[p];
				if(state != delta_t[p])
				{
					delta_t[p] = state;
					eeprom_write_word(&addres_eeprom_0_, state);
				}
				//val = eeprom_read_word(&addres_eeprom_2_);
				//i++;
				state = delta_t[f];
				main_menu_State = MAIN_MENU_STATE_DELTA_f_WAIT;
				break;
			case MAIN_MENU_STATE_DELTA_f_WAIT:
				state = valIncrementDecrement(&state);
				break;
			case MAIN_MENU_STATE_DELTA_f:
				if(delta_t[f] != state)
				{
					delta_t[f] = state;
					eeprom_write_word(&addres_eeprom_2_, state);
				}
				//val = 0;
				//i++;
				state = 0;
				main_menu_State = MAIN_MENU_STATE_DEFAULT_DELTA_p_WAIT;
				break;
			case MAIN_MENU_STATE_DEFAULT_DELTA_p_WAIT:
				state = valNonVal(&state);
				break;
			case MAIN_MENU_STATE_DEFAULT_DELTA_p:
				if(state == 1)
				{
					delta_t[p] = Default_Delta_p;
					eeprom_write_word(&addres_eeprom_0_, delta_t[p]);
				}
				state = 0;
				//i++;
				main_menu_State = MAIN_MENU_STATE_DEFAULT_DELTA_f_WAIT;
				break;
			case MAIN_MENU_STATE_DEFAULT_DELTA_f_WAIT:
				state = valNonVal(&state);		
				break;
			case MAIN_MENU_STATE_DEFAULT_DELTA_f:
				if(state == 1)
				{ 
					delta_t[f] = Default_Delta_f;
					eeprom_write_word(&addres_eeprom_2_, delta_t[f]);
				}
				state = eeprom_read_byte(&addres_eeprom_4);
				//i++;
				main_menu_State = MAIN_MENU_STATE_MODE_p_Wait;
				break;
			case MAIN_MENU_STATE_MODE_p_Wait:
				state = valNonVal(&state);
				break;
			case MAIN_MENU_STATE_MODE_p:
				if(eeprom_read_byte(&addres_eeprom_4) != state)
				{
					eeprom_write_byte(&addres_eeprom_4, state);
					mode[p] = state;
				}
				//i++;
				state = eeprom_read_byte(&addres_eeprom_5);
				main_menu_State = MAIN_MENU_STATE_MODE_f_WAIT;
				break;
			case MAIN_MENU_STATE_MODE_f_WAIT:
				state = valNonVal(&state);
				break;
			case MAIN_MENU_STATE_MODE_f:
				if(eeprom_read_byte(&addres_eeprom_5) != state)
				{
					eeprom_write_byte(&addres_eeprom_5, state);
					mode[p] = state;
				}
				//i++;
				state = 0;
				main_menu_State = MAIN_MENU_STATE_EXIT;
				break;
			case MAIN_MENU_STATE_EXIT:
				//Station_State = Station_State_Old;
				menu_state = MENU_STATE_WAIT;
				main_menu_State = MAIN_MENU_STATE_IDLE;
				//i++;
				//main_menu_State = i;
				break;
		}
	//}
	word summ = 0;
	for (byte i = 0; i < 10; i++)//расчёт котнрольной суммы
	{
		summ += eeprom_read_byte(&i);
	}
	eeprom_write_word(&addres_eeprom_10_, summ);
}
//------------------Меню и обработка кнопок-------------------------------------

void initAVR()//инициализация мк
{
		DDRD |= ((1<<DDD1)|(1<<DDD3)|(1<<DDD4));//выходы
		DDRB |= ((1<<DDB5)|(1<<DDB3)|(1<<DDB2));//выходы
		DDRD &= ~((1<<DDD0)|(1<<DDD2)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7));//входы, определит на всякий случай
		DDRB &= ~((1<<DDB0)|(1<<DDB1)); //входы, определит на всякий случай
		PORTD |= ((1<<PD0)|(1<<PD5)|(1<<PD6)|(1<<PD7));//вход c подтягиающим резистором Page 55 on datasheet
		PORTB |= ((1<<PB0)|(1<<PB1));//вход и подтягиающим резистором Page 55 on datasheet
		PORTB &= ~((1<<PB2)|(1<<PB3)|(1<<PB5)); //низкий уровень
	//инициализация таймеров
	Timer0_SetPrescaler(D_Prescaler_1024);// 61 раз в секунду
	Timer0_Ovf_Ena;
	GetTimer0_OVF_Vect(Time);//эта функция присвает функцию  которая должна выполнятся при данном событии
	//в данном случае при переполнении таймера 0 вызовется ф-я Time()
	Timer1_SetPrescaler(D_Prescaler_no);//244,2 раз в секунду;
	GetTimer1_OVF_Vect(OVF);
	GetTimer1_COMPA_Vect(COMP);
	//Timer2_init();
	Timer2_SetPrescaler(D_Prescaler_no);//62500 раз в секунду
	GetTimer2_OVF_Vect(timerInterrupt);
	//GetTimer2_COMPA_Vect(timerInterrupt);
	//GetTimer2_COMPB_Vect(timerInterrupt);
	//инициализация таймеров
	
	//инициализация внешних прерываний
	GetINT0_Vect(detectUp);
	Int0_FALLING;
	//Int0_RISING;
	//инициализация внешних прерываний
	
	//инициализация АДЦ
	ADC_Enable; // Разрешение использования АЦП
	AREF;//выбираем опорное напряжение
	ADC_Prescaler(D_ADC_Prescaler_128);
	//инициализация АДЦ
	
	//инициализация SPI 
	SPI_Ena_Master;	
	SPI_First_bit_HIGH;
	SPI_Cloc_Rate_Not2x;
	byte i = 0;
	for(; i < 2; i++)
	{	
		SPI_Data_Bufer = 0;
		while(SPI_Transmitted);
	}
	Save_Data_in_ShiftRegister;
	//инициализация SPI
	delta_t[p] = eeprom_read_word(&addres_eeprom_0_);
	delta_t[f] = eeprom_read_word(&addres_eeprom_2_);
	factor[p] = eeprom_read_word(&addres_eeprom_6_);
	factor[f] = eeprom_read_word(&addres_eeprom_8_);
	mode[p] = eeprom_read_byte(&addres_eeprom_4);
	mode[f] = eeprom_read_byte(&addres_eeprom_5);	
	solidOff();
	dryerOff();
	wdt_enable(WDTO_2S);//включим сторожевой таймер с тайм-аутом в 2 секунды
	sei();//включим глобальные прерывания
	
	
}

int main(void)
{
	initAVR();
	//page 41 atmega 8
	//page 79 atmega 328p
	//array_t[0] = 26;
	//array_t[1] = 26;//rr - reason reset
	writeArray(0, D_array_reason_reset, 4);
	for (byte i = 0; i < 4; i++)
	{
		if (BitTest(MCUSR,i)) 
		{
			array_t[2] = i;//так я узнаю почему произошла перезагрузка
			word summ = 0;
			for (byte j = 0; j < 10; j++)//расчёт котнрольной суммы
			{
				summ += eeprom_read_byte(&j);
			}
			if (summ != eeprom_read_word(&addres_eeprom_10_))
			{
				BitSet(error_p, 0);
			}
			#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)//если используется мк ATmega168 или ATmega328P
			writeArray(3, D_array_reason_reset, i);
			#endif
			break;
		}
	}
	while(time < 55)
	{
		SPIOutput();
		wdt_reset();//сбросим счёчик таймера чтоб небыло перезагрузки мк
	}
	static dword time_screen = 0;
	while (1)
	{
		if(debug_flag == 0)
		{
			if ((time-time_screen)>12)//примерно 5 раза в сек
			{
				screen();
				time_screen = time;
			}
			static dword time_flag;
			if(time_flag != time)//примерно 61 раз в секунду
			{
				konvert();
				if(power[p])
				{
					powerP();
				}
				switch(menu_state)
				{
					case MENU_STATE_WAIT:
					menuWait();
					break;
					case MENU_STATE_MAIN:
					menuMain();
					break;
				}
				if ((flag_fan == true) && (adc[3] < 2))
				{
				Fan_disable;
				flag_fan = 0;
				}	
				time_flag = time;
			}
		}
		else
		{
			debug();
		}
		SPIOutput();
		wdt_reset();//сбросим счёчик таймера чтоб небыло перезагрузки мк
		 //------------------------Начало обработки кнопок включения -------------------------------------
		 
		 
		 //------------------Конец обработки кнопок включения-------------------------------------------
	}
    
}