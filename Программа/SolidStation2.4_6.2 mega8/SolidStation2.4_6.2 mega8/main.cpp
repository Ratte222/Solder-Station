/*
 * Debug.cpp
 *
 * Created: 18.08.2018 10:00:54
 *  Author: User
 */ 
#include "main.h"
PID_Regulator Dryer(1.5, 2.5, 2, 0.0163);
PID_Regulator Solid(4, 8, 6, 0.0163);
//MENU-------------------------------------------------------------------
//typedef enum{
	//Station_rest = 0,
	//Station_Soldering_Iron_On,
	//Station_Soldering_Dryer_On,
	//Station_Iron_and_Dryer_On,
	//Station_Setting
//}Station_StateTypeDef;
//int abcd = 10;
enum MENU_StateTypeDef {//��������� ��������� ���������
	MENU_STATE_WAIT = 0,
	MENU_STATE_MAIN
};
enum MAIN_MENU_StateTypeDef  {//������������ ��������� ��� ������ � ����
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
//Station_StateTypeDef Station_State = Station_rest;
//Station_StateTypeDef Station_State_Old = Station_rest;
MENU_StateTypeDef menu_state = MENU_STATE_WAIT;
MAIN_MENU_StateTypeDef main_menu_State = MAIN_MENU_STATE_IDLE;
//MENU-------------------------------------------------------------------
int del;//�������� ��� ����
volatile int tic;//�������� ��� ����
//int time1 = 250; //�������� ��� ������ � ��������
//int time2 = 150; //��� ����� ����������� ���
//int time3 = 250; //��� ����� ��� �-� ���������� ����������
//int *pointer;
//byte first_heat_p = 0;//���� ������� ������� ���������
//byte first_heat_f = 0;//���� ������� ������� ����
byte debug_flag = 0;//���� ���� ���� ������ - ��������� ����� ������� � ������� ����� ��������� ������������������ "������"
byte interrupt_flag = 0;//���� ���� ����� ��� ������������ ������� ���������� � ���� �� ��� ����� ������ ���� - ��������� ������
//byte flag_set;//���� ���������
//byte flag_b; //��������������� ����
//byte flag_a; //��������������� ����
byte flag_fan; // ���� == 1 - ��������� ��� ��������� ���� ����� ����������� ����������� � ����
//������ ��������� � �������������� ���� �.�. ���� ���� ������ 0 ����� ���� ����� ��������� ���: 0b00000001
byte error_p = 0;//������ ��� ���������:0 - �� ��������� ����������� ����� EEPROM 1 -  �����������
byte error_f = 0;//������ ��� ���� 0 - ��� ������� ���������� 1 - �����������
//������ ��������� �� ��������
byte power[2]; // ����� ��� ��� ���� �������� � ���
byte mode[2];//�������� ��� ����� ��������. ��� ��������� ���������� ����� ��� ���/����, ��� ���� ������������ ������ ��� ���
int state = 0;//����� ��� ������������� ��������
int delta_t[2]; //����������� ����������� delta_p, delta_f
int factor[2];// �� ������� �������� ��� ��� ��������� ���������� ����������� 
//volatile int old_required_t[2];
int required_t[2]; //�������� ����������� � �������� required_p, required_f
int adc[4];
//volatile int required_adc[2];//�������� ����������� � ���
//volatile int real_adc[2];// ��� � ��������
int real_t[2]; //��������������� ����������� real_p, real_f
//volatile int old_required_t[2]; //������ �������� ����������� old_required_p, old_required_f
//volatile int old_real_t[2]; //������ ��������������� �����������
//volatile byte show_must_t = 0; //���� ������� �������� �� ��, ����� ����� ������� �������� ����������� ������������
//volatile byte setting[2];

#ifdef Stripped_down
//byte addres_eeprom_0 = 0;//delta_t[p]
//byte addres_eeprom_2 = 2;//delta_t[f]
//byte addres_eeprom_4 = 4;//mode[p]
//byte addres_eeprom_5 = 5;//mode[f]
//byte addres_eeprom_6 = 6;//factor[p]
//byte addres_eeprom_8 = 8;//factor[f]
//byte addres_eeprom_10 = 10;//check sum
word addres_eeprom_0_ = 0;//delta_t[p]
word addres_eeprom_2_ = 2;//delta_t[f]
//word addres_eeprom_6_ = 6;//factor[p]
//word addres_eeprom_8_ = 8;//factor[f]
word addres_eeprom_10_ = 10;//check sum
#else
//������ ��� ������ ������, ������ ������� ���� ��������� �� �������
//byte addres_eeprom_0 = 0;//delta_t[p]
//byte addres_eeprom_2 = 2;//delta_t[f]
byte addres_eeprom_4 = 4;//mode[p]
byte addres_eeprom_5 = 5;//mode[f]
//byte addres_eeprom_6 = 6;//factor[p]
//byte addres_eeprom_8 = 8;//factor[f]
//byte addres_eeprom_10 = 10;//check sum
word addres_eeprom_0_ = 0;//delta_t[p]
word addres_eeprom_2_ = 2;//delta_t[f]
word addres_eeprom_6_ = 6;//factor[p]
word addres_eeprom_8_ = 8;//factor[f]
word addres_eeprom_10_ = 10;//check sum
#endif
dword time = 0;//���������� ����� �� ������ ������, 1 ��� = 61
byte array_t[6];// � array_t ������������ ����� �������� �� ������ cifri ������� ����� ����������
//int i, n, m, e;// � - ������� ��� ��� ����� �������� �������� �����������

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
	0b11010011,//y 28 element
	0b1011000//u 29 element
};
const PROGMEM byte mas[6] = { 0b11111101, // ������ ��� �������� ������������� � ������� ���� ������ ���� ����� ������������ � ���� ������ �������
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
const PROGMEM byte array_reason_reset[5][3] = {
	{11, 16, 26},// dr - watchdog system reset
	{14, 0, 26},//bOr -		brown-out reset
	{17, 27, 26},//Etr - external reset
	{24, 0, 26},//POr - power-on reset
	{26, 26, 11} //rr - reason reset
};
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
//const PROGMEM byte array_addr_eep_b[7] = {0,//f_p
	//2,//f_f
	//4,//d_p
	//6,//d_f
	//8,//mode_p
	//9,//mode_f
	//11//check sum
	//};
//const PROGMEM word array_addr_eep_w[5] = {0,//f_p
	//2,//f_f
	//4,//d_p
	//6,//d_f
	//11//check sum
//};

void EmptyProc(void){};//������ ������� � ����� ���� ������ �����

void Time(void)//�������� ����� 61 �.�. = 1 �
{
	time++;
}

//----------------������ � ����������--------------------------------

void OVF(void)//��� �-� ������� ���������� ��� ������������ ������� �������
{
	Mosfet_on;
}

void COMP(void)//� ��� �� ����������� ������� �������. ����� ������� � � �������� ����� ���� ��� �������� �� ��� ����� �� �������� � ��������
{
	Mosfet_off;
}

void Solid_on(void)//�������� ��� ��� ����� ���� ����������� ��������� ����� �������������� �����
{
	if (mode[p])
	{
		//������������� ������ �������� ����� ���� ������ �������� �������
		//Timer1_SetPrescaler(D_Prescaler_no);//244,2 ��� � �������;
		Timer1_Ovf_Ena;//��������� ���������� �� ������������
		Timer1_COMPA_Ena;//��������� ���������� �� ����������  � �����������
		//firstHeatP();
	}
}

void Solid_off(void)//��������� �� � �������� ����� ��������� � ����������(��� ��������)
{
	if (mode[p])
	{
		//Timer1_Stop;//������ ��������� �������
		Timer1_Ovf_Dis;
		Timer1_COMPA_Dis;
		Mosfet_off;
		Solid.Reset_Value();
	}
	else
	{
		Mosfet_off;
	}
	//first_heat_p = 0;//�� ������ ������ � ��� ��������
	power[p] = 0;//�� ������ ������ � ��� ��������
}

//void firstHeatP(void)//������������ ��������� ������ �� ����� ����� ������ ���� ����������� ����� ��� ��� �� ����� �������� �������
//{//� ������������ � ����� �������
	////if(mode[p] == 1)
	////{
		//Mosfet_on;//������ �������� �� �����������
		//first_heat_p = 1;
		////while (1)
		////{
		//if (required_t[p] < (real_t[p] + 7))
		//{
			//Mosfet_off;
			//first_heat_p = 0;
			//if(mode[p] == 1) { Solid_on(); }
			////break;
		//}
		////}
	////}
//}

void Power_p(void)//���� ������� �����������
{
	if (mode[p])//����������� �����
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
		
		//static long PWM;
		//int val = required_t[p] - real_t[p];
		////int var[10] = {50, 30, 20, 5, 1, 1, 5, 40, 50, 50};
		////int element[10] = {-50, -40, -30, -20, 0, 1, 20, 30, 50, 100};
		//byte result = 100;//������ 16-� ������, � ������ ������������ ����� 65535 � ������ �������� �� 100
		////����� ������ - ����������� ������� ������ � � ����� ���������
		////returnValueFromTheTable(var, element, 10, val);
		////abcd = result;
		//if (val < 0)//���� ������ �������� �����������
		//{
			//PWM -= result;
			//if (PWM < 15000)
			//PWM = 15000;
			//CompareReg_1A = PWM;
		//}
		//if (val > 5)//���� ������ �������� �����������
		//{
			//PWM += result;
			//if (PWM > 65000)
			//PWM = 65000;
			//CompareReg_1A = PWM;
		//}
		////CompareReg_2 = required_t[p]>>1;
	}
	else//� ��� ����������� ��������� ���� ���/���� 
	{//����������� ������� �������
		if (required_t[p] > (real_t[p] + 2))
		{
			Mosfet_on;
			// Serial.println("mosf_on");
		}
		if (required_t[p] < (real_t[p] + 2))
		{
			Mosfet_off;
			//  Serial.println("mosf_off");
		}
	}
}
//----------------������ � ����������--------------------------------

//----------------------������ � �����---------------------------------
#define resumeCountTimer2 Timer2_Ovf_Ena
#define stopCountTimer2 Timer2_Ovf_Dis
#define Dryer_On Fan_enable; attachInterrupt0
//void resumeCountTimer1(void)//��������� ������������ ���������� ��� 2-�� �������
//{
	//Timer2_Ovf_Ena;
	////Timer2_COMP_Ena;
	////CompareReg_2 = 127;
//}

//void stopCountTimer1(void)//��������� ������������ ���������� ��� 2-�� �������
//{
	//Timer2_Ovf_Dis;
	////Timer2_COMP_Dis;
//}

//void Dryer_On(void)//��� ������� ����������
//{//��� ����� ���� ����� ����� ���������� � ���� �������� 0-� �����
	//attachInterrupt0;
	////Fan_enable;
//}

void Dryer_Off(void)//�� ��� ����� ������� ���� ��� ����� �� ������� � ��������� �� ������ ��� �� ��������
{
	detachInterrupt0;
	stopCountTimer2;
	Semister_off;
	flag_fan = true;//���� ���� ����� ���� ���������� �� ����� ����������, � ������ ����� ����������� ���� ��������� �� ��������
	//������������ ����� ���������� � ������� �-�
	power[f] = 0;
	//first_heat_f = 0;	
	Dryer.Reset_Value();
}
//void firstHeatF(void)//��� �-� ����� ���� ��� ����������� ������ ����������
//{
		////Semister_on;
		//Fan_enable;
		////first_heat_f = 1;
		////if (required_t[f] < (real_t[f] + 50))
		////{
			////Semister_off;
			////first_heat_f = 0;
			////Dryer_On;
			//////break;
		////}
		//Dryer_On;
//}
//----------------------����������� ����������--------------------------
void timer_interrupt() {       // ���������� ������� ����������� ������ 40 ���
	tic++;                       // �������
	if (tic > del)            // ���� ������� ����� �������� ���
	{
		Semister_on;   // ������� ���
	}
}

void  detect_down();
void  detect_up() {    // ��������� �������� ���������� �� ����������� ���� �����
	tic = 0;                                  // �������� �������
	resumeCountTimer2;                           // ������������� ������
	Int0_RISING;  // ������������� ����������
	//Int0_FALLING;
	GetINT0_Vect(detect_down);
	//abcd = 2;
}

void  detect_down() {  // ��������� �������� ���������� �� ����������� ���� ������
	tic = 0;                                  // �������� �������
	stopCountTimer2;                             // ���������� ������
	Semister_off;                  // �������� ���
	Int0_FALLING;  // ������������� ����������
	//Int0_RISING;
	GetINT0_Vect(detect_up);
	//abcd = 1;
	interrupt_flag = 1;//����� ��� ������������ ������ � �������� ������, ���� �����
}
//----------------------����������� ����������--------------------------
//----------------------������ � �����---------------------------------

//----------���������� � ����������� ���--------------------------------------
void Filter(void)//��� ����������� ��� � ����������� �� ������ ��� �������� ��������
{
  //extern TADC_Channel;
  //word x[2];
  //word r;
  byte i;
  int r = 0;
  static int old_adc[4];
  for (i = 0; i < 4; i++)
  {
  r = ADC_convert(i+1);
  //   x[i] = map(x[i], 0, 800, 0, 500);
  if ((r - old_adc[i]) > Max_step_ADC)// ���� ��� ����������� ������� ������ - �������
  {
	adc[i] = adc[i] + Max_step_ADC;
  }
  else if ((old_adc[i] - r) > Max_step_ADC)// ���� ��� ���������� ������� ������ - �������
  {
	adc[i] = adc[i] - Max_step_ADC;
  }
  else//��� ��������, ����� �������� ��� ������ ����������
  {
	adc[i] = r;
  }
	old_adc[i] = adc[i];
  }
  //word val;
  //static int old_required_t[2];
  //���������� �������� ����������� 
  //for (i = 0; i < 2; i++)
  //{
	//required_adc[i] = ADC_convert(i+1) >> 1;
  //}
	//for (i = 0; i < 2; i++)
	//{
		//r = ADC_convert(i+1) >> 1;
		////   x[i] = map(x[i], 0, 800, 0, 500);
		//if ((r - old_required_t[i]) > Max_step_ADC)// ���� ��� ����������� ������� ������ - �������
		//{
			//required_t[i] = required_t[i] + Max_step_ADC;
		//}
		//else if ((old_required_t[i] - r) > Max_step_ADC)// ���� ��� ���������� ������� ������ - �������
		//{
			//required_t[i] = required_t[i] - Max_step_ADC;
		//}
		//else//��� ��������, ����� �������� ��� ������ ����������
		//{
			//required_t[i] = r;
		//}
		//old_required_t[i] = required_t[i];
	//}
  //
  ////x[0] = ADC_convert(3);
  ////x[1] = ADC_convert(4);
  //static int old_real_adc[2];
  ////��������� �������� � ��������
  //for (i = 0; i < 2; i++)
  //{
	 //r = ADC_convert(i+3);
    ////   x[i] = map(x[i], 0, 800, 0, 500);
    //if ((r - old_real_adc[i]) > Max_step_ADC)// ���� ��� ����������� ������� ������ - ������� 
    //{
      //real_adc[i] = real_adc[i] + Max_step_ADC;
    //}
    //else if ((old_real_adc[i] - r) > Max_step_ADC)// ���� ��� ���������� ������� ������ - �������
    //{
      //real_adc[i] = real_adc[i] - Max_step_ADC;
    //}
    //else//��� ��������, ����� �������� ��� ������ ����������
    //{
      //real_adc[i] = r;
    //}
    //old_real_adc[i] = real_adc[i];
  //}
  
  
}

void Konvert(void)//��� �������������� �������� � �������� � ���������� � �������������, ���� �����, ����� �������� ��� ���� 
{	
	
	Filter();
	required_t[p] = adc[0] >> 1;
	required_t[f] = adc[1] >> 1; 
	//float res = 0;
	//res = (float)real_adc[p] * (float)factor[p];
	//real_t[p] = trunc(res) - delta_t[p];
	////real_t[p] = ((int)((float)real_adc[p] * factor[p]) + delta_t[p]);
	//res = (float)real_adc[f] * (float)factor[f];
	//real_t[f] = trunc(res) - delta_t[f];
	
	//� �������� ����������� �������� ���� ��������� ��� ���� 1
	real_t[p] = (int)((long)(adc[2] * factor[p]) / 100);//long ���� ������ ������������
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
	//real_t[f] = adc[3];
	//int u;
	//������ ���������� ��� ���������
	
	//������ ���������� ��� ���������
	//������ �������� ��� ����
	if (power[f])
	{//��� ������ ����� - ��� ������� ����� ������ ������
		int u = (int)Dryer.PID_Calculation(&required_t[f], &real_t[f]);
		//if (u > 575)
		//{
			//del = 50;
		//} 
		//else if(u <= 575)
		//{
			//del = 625 - u;
		//}
		//else
		//{
			//del = 600;
		//}
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
	//����� ������� �������� ��� ����
}
//----------���������� � ����������� ���--------------------------------------
//---------------------------------����� �� �����-------------------------------

void SPIOutput(void)//�-� ������, ������� ���������� ������ ����� � ��������� ��������
{
	//byte mas = 0b11111101;
	for (byte i = 0; i < 6; i++)//���������(���������) �������� �������� ��� ������
	{
		byte  k = pgm_read_byte(&(cifri[array_t[i]]));
		if ((power[p] == true) && (i == 2))//���� �������� ������� - � ��������� ����� ��������� ����� ��� ��������� ������
		{
			k |= (1<<5);//������� ��� ���������� �� ������� "."
		}
		else if ((power[f] == true) && (i == 5))//���� ��� ������� - � ��������� ����� ��������� ����� ��� ��������� ������
		{
			k |= (1<<5);//������� ��� ���������� �� ������� "."
		}
		// ������ � ������� ������������ � ������
		SPI_Data_Bufer = k;//���������� ���� � ������� SPI � �� ����� �������� ��� ���������� �� ����
		while(SPI_Transmitted);//��� ���� ��������
		// ������ � ������� ������������ � �������
		SPI_Data_Bufer = pgm_read_byte(&(mas[i]));
		//mas = mas - pow(2, i);
		while(SPI_Transmitted);
		Save_Data_in_ShiftRegister;
		//_delay_ms(1);
		_delay_us(100);
		// ������� ���������
		//for(byte n = 0; n < 2; n++)
		//{
			//SPI_Data_Bufer = 0;
			//while(SPI_Transmitted);
		//}
		//Save_Data_in_ShiftRegister;
	}
}

/*
void Output(void)//��� �-� ����� ��� ���������� ������������������ ������ � ��������
{
	//����� � ������� �� 0 �� 9, ���� ������, ������ "-", �������,
	byte cifri[30] = { 0b01011111, //0,O
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
		0b11010011,//y 28 element
		0b1011000//u 29 element
	};
	//byte cifri2[13] = {  0b01111111, //����� � ������� �� 0 �� 9
		//0b01100001,
		//0b10111101,
		//0b11110101,
		//0b11100011,
		//0b11110110,
		//0b11111110,
		//0b01100101,
		//0b11111111,
		//0b11110111,
		//0b11111001,
		//0b10100000,
		//0b00100000
	//};
	byte mas[6] = { 0b11111101, // ������ ��� �������� ������������� � ������� ���� ������ ���� ����� ������������ � ���� ������ �������
		0b11111011,
		0b11110111,
		0b11101111,
		0b11011111,
		0b10111111
	};
	
	//Output(4,0);
	
	//Output(5,0);
	
	for (byte i = 0; i < 6; i++)//���������(���������) �������� �������� ��� ������
	{
		byte m = array_t[i];
		byte  k = cifri[m];
		 // ������ � ������� ������������ � �������
		 latch_2_off;
		 shiftOut(ds, sh, mas[i], 1);
		 latch_2_on;
		 // ������ � ������� ������������ � ������
		 if ((power[p] == true) && (i == 2))//���� �������� ������� - � ��������� ����� ��������� ����� ��� ��������� ������
		 {
			 k = cifri[m];
			 k |= (1<<5);//������� ��� ���������� �� ������� "."
		 }
		 else if ((power[f] == true) && (i == 5))//���� ��� ������� - � ��������� ����� ��������� ����� ��� ��������� ������
		 {
			 k = cifri[m];
			 k |= (1<<5);//������� ��� ���������� �� ������� "."
		 }
		 
		 //if ((power[p] == true) && (i == 2))//���� �������� ������� - � ��������� ����� ��������� ����� ��� ��������� ������
		 //{
			 //latch_1_off;
			 //shiftOut(ds, sh, cifri2[m], 1);
			 //latch_1_on;
			 //_delay_us(500);
		 //}
		 //else
		 //{
			 //latch_1_off;
			 //shiftOut(ds, sh, cifri[m], 1);
			 //latch_1_on;
		 //}
		 //if ((power[f] == true) && (i == 5))//���� ��� ������� - � ��������� ����� ��������� ����� ��� ��������� ������
		 //{
			 //latch_1_off;
			 //shiftOut(ds, sh, cifri2[m], 1);
			 //latch_1_on;
		 //}
		 //else
		 //{
			 //latch_1_off;
			 //shiftOut(ds, sh, cifri[m], 1);
			 //latch_1_on;
		 //}
		  
		  latch_1_off;
		  shiftOut(ds, sh, k, 1);
		  latch_1_on;
		 
		 _delay_ms(1);
		 // ������� ���������
		 latch_1_off;
		 shiftOut(ds, sh, 0, 1);
		 latch_1_on;

		 latch_2_off;
		 shiftOut(ds, sh, 0, 1);
		 latch_2_on;
	}
}
*/
//---------------------------------����� �� �����-------------------------------
//---------------------------------���������� �������, ������� ��������� �� �����----------------
static void checkErrors(void)//��������� ���� �� �������� � ���� � ������� ���� ���� � "1"
{// �� ������� � ������, ������� ������������ ��� ������ ���� ����������� �� ��������� ������ 
	static dword time_error = 0;
	
	if ((time - time_error) > 30)
	{
		if (power[f])//�������� �� ������� ����������
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
					Dryer_Off();
					BitSet(error_f, 0);//��� ������� ����������
				}
				else
				{
					interrupt_flag = 0;
					flag_error = 1;
				}
			}
		}
		
		//if (first_heat_p)
		//{
			////static dword time_heat = 0;
			//static int old_a_p = 0;
			//static byte flag_error = 0;
			////if (time > (time_heat + 61))//�������� ����� �������
			////{
			//if ((adc[2] - old_a_p) > 1)
			//{
				//old_a_p = adc[2];
				////time_heat = time;
				//flag_error = 0;
			//}
			//else
			//{
				//if (flag_error)
				//{
					////Solid_off();
					//error_p[2] = 1;//�������� �� �����������
				//}
				//else
				//{
					//flag_error = 1;
					//old_a_p = adc[2];
					////time_heat = time;
				//}
				//
				////}
			//}
		//}
		//
		//if (first_heat_f)
		//{
			////static dword time_heat = 0;
			//static int old_a_p = 0;
			//static byte flag_error = 0;
			////if (time > (time_heat + 61))//�������� ����� �������
			////{
			//if ((adc[3] - old_a_p) > 1)
			//{
				//old_a_p = adc[3];
				////time_heat = time;
				//flag_error = 0;
			//}
			//else
			//{
				//if (flag_error)
				//{
					////Dryer_Off();
					//error_f[2] = 1;//��� �� �����������
				//}
				//else
				//{
					//flag_error = 1;
					//old_a_p = adc[3];
					////time_heat = time;
				//}
				//
			//}
			////}
		//}
		
		if (power[p])
		{
			//static dword time_work = 0;
			static byte flag_error = 0;
			//if (time > (time_work + 61))//�������� ����� �������
			//{
			if ((real_t[p] - required_t[p]) > 30)//�������� ������ ����������� ������������ �������� �����������
			{
				if (flag_error)
				{
					Solid_off();
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
			//if (time > (time_work + 61))//�������� ����� �������
			//{
			if ((real_t[f] - required_t[f]) > 30)//��� ������ ����������� ������������ �������� �����������
			{
				if (flag_error)
				{
					Dryer_Off();
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
			array_t[2] = i-1;//er � ����� ������
			}
			if(BitTest(error_f, (i-1)))
			{
			array_t[3] = 17;
			array_t[4] = 26;
			array_t[5] = i-1;//er � ����� ������
			}
			i--;
		} while (i);
			
}

void WritrArray(byte i, byte arr, byte n)
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
			case 2:
				array_t[j+i] = pgm_read_byte(&(array_reason_reset[n][j]));
				break;
		}
		
	}
}

byte yesOrNo(byte o)//��� ����� ��� ��������� ������ ���������
{
	if (o)
	{
		WritrArray(3, D_array_maim_menu, state);
	} 
	else
	{
		WritrArray(0, D_array_maim_menu, state);
	}
	
	return 1;
}
void Screen(void)//��� �-� �������� �� ��, ��� ������ ���������� ������ �� ���������
{
	static dword time_; //����������, ������� ������������ ��� ����������� �������� ����������� ��������� �����
	static int old_required[2];//����� ��� ������������ ��� ��������� "�������� �����������"
	//byte flag_count[2] = {0,0};
	//����� ������� ��-�� ������� ���������� ������ ���������
	volatile byte flag_count_p = 0, flag_count_f = 0;//���� �� ������ - �� res_� ��� res_f ����� ������ �� ����� � ������� � ������ array_t
	//word res[2] = {0,0};
	volatile word res_p = 0, res_f = 0;//���� ���������� ����� ���� ����������� �������� �� ����� � ������ �� �����
	int i;
	//volatile byte q, w, e, r, t, y;
	byte flag_p = 0, flag_f = 0;//��� �� ������ - ������ � ������ array_t �� �����
	//byte array_cifra[6];
	if (menu_state == MENU_STATE_MAIN)//���� �� � ���� - ������� ��, ��� �����������
	{
		/*switch((byte)main_menu_State)
		{
			case MAIN_MENU_STATE_DEBUG_WAIT:
				if(state)
				{
					array_t[3] = 28;
					array_t[4] = 17;
					array_t[5] = 5;//Yes
				}
				else
				{
					array_t[3] = 11;
					array_t[4] = 23;
					array_t[5] = 0;//nO
				}
				array_t[0] = 16;
				array_t[1] = 17;
				array_t[2] = 14;//deb - debug
				break;
			case MAIN_MENU_STATE_FACTOR_p_WAIT:
				flag_count[p] = 1;
				//flag_count[f] = 1;
				res[p] = state;
				array_t[3] = 18;
				array_t[4] = 12;
				array_t[5] = 24;//f_p - set factor p
				break;
			case MAIN_MENU_STATE_FACTOR_f_WAIT:
				//flag_count[p] = 1;
				flag_count[f] = 1;
				res[f] = state;
				array_t[0] = 18;
				array_t[1] = 12;
				array_t[2] = 18;//f_f - set factor f
				break;
			case MAIN_MENU_STATE_DEFAULT_FACTOR_p_WAIT:
				//flag_count[p] = 1;
				//flag_count[f] = 1;
				//res[p] = state;
				if(state)
				{
					array_t[0] = 28;
					array_t[1] = 17;
					array_t[2] = 5;//Yes
				}
				else
				{
					array_t[0] = 11;
					array_t[1] = 23;
					array_t[2] = 0;//nO
				}
				array_t[3] = 18;
				array_t[4] = 18;
				array_t[5] = 24;//dfp - default factor p
				break;
			case MAIN_MENU_STATE_DEFAULT_FACTOR_f_WAIT:
				//flag_count[p] = 1;
				//flag_count[f] = 1;
				//res[f] = state;
				if(state)
				{
					array_t[3] = 28;
					array_t[4] = 17;
					array_t[5] = 5;//Yes
				}
				else
				{
					array_t[3] = 11;
					array_t[4] = 23;
					array_t[5] = 0;//nO
				}
				array_t[0] = 16;
				array_t[1] = 18;
				array_t[2] = 18;//dff - default factor f
				break;
			*****************************
			case MAIN_MENU_STATE_DELTA_p_WAIT:
				flag_count[p] = 1;
				//flag_count[f] = 1;
				res[p] = delta_t[p];
				array_t[3] = 16;
				array_t[4] = 12;
				array_t[5] = 24;//d_p - set delta p
				break;
			case MAIN_MENU_STATE_DELTA_f_WAIT:
				//flag_count[p] = 1;
				flag_count[f] = 1;
				res[f] = delta_t[f];
				array_t[0] = 16;
				array_t[1] = 12;
				array_t[2] = 18;//d_f - set delta f
				break;
			case MAIN_MENU_STATE_DEFAULT_DELTA_p_WAIT:
				//flag_count[p] = 1;
				//flag_count[f] = 1;
				//res[p] = state;
				if(state)
				{
					array_t[0] = 28;
					array_t[1] = 17;
					array_t[2] = 5;//Yes
				}
				else
				{
					array_t[0] = 11;
					array_t[1] = 23;
					array_t[2] = 0;//nO
				}
				array_t[3] = 16;
				array_t[4] = 16;
				array_t[5] = 24;//rdp - reset delta p
				break;
			case MAIN_MENU_STATE_DEFAULT_DELTA_f_WAIT:
				//flag_count[p] = 1;
				//flag_count[f] = 1;
				//res[f] = state;
				if(state)
				{
					array_t[3] = 28;
					array_t[4] = 17;
					array_t[5] = 5;//Yes
				}
				else
				{
					array_t[3] = 11;
					array_t[4] = 23;
					array_t[5] = 0;//nO
				}
				array_t[0] = 26;
				array_t[1] = 16;
				array_t[2] = 18;//rdf - reset delta f
				break;
			case MAIN_MENU_STATE_MODE_p_Wait:
				flag_count[p] = 1;
				res[p] = state;
				array_t[3] = 12;
				array_t[4] = 12;
				array_t[5] = 12;
				break;
			case MAIN_MENU_STATE_MODE_f_WAIT:
				flag_count[f] = 1;
				res[f] = state;
				array_t[0] = 12;
				array_t[1] = 12;
				array_t[2] = 12;
				break;
		}*/
		
		switch((byte)main_menu_State)
		{
			case MAIN_MENU_STATE_DEBUG_WAIT:
				flag_f = yesOrNo(f);
				//q = 16;
				//w = 17;
				//e = 14;//deb - debug
				break;
			case MAIN_MENU_STATE_FACTOR_p_WAIT:
				flag_count_p = 1;
				//flag_count[f] = 1;
				res_p = state;
				//r = 18;
				//t = 12;
				//y = 24;//f_p - set factor p
				break;
			case MAIN_MENU_STATE_FACTOR_f_WAIT:
				//flag_count[p] = 1;
				flag_count_f = 1;
				res_f = state;
				//q = 18;
				//w = 12;
				//e = 18;//f_f - set factor f
				break;
			case MAIN_MENU_STATE_DEFAULT_FACTOR_p_WAIT:
				//flag_count[p] = 1;
				//flag_count[f] = 1;
				//res[p] = state;
				flag_p = yesOrNo(p);
				//r = 18;
				//t = 18;
				//y = 24;//dfp - default factor p
				break;
			case MAIN_MENU_STATE_DEFAULT_FACTOR_f_WAIT:
				//flag_count[p] = 1;
				//flag_count[f] = 1;
				//res[f] = state;
				flag_f = yesOrNo(f);
				//q = 16;
				//w = 18;
				//e = 18;//dff - default factor f
				break;
			//*****************************
			case MAIN_MENU_STATE_DELTA_p_WAIT:
				flag_count_p = 1;
				//flag_count[f] = 1;
				res_p = state;
				//r = 16;
				//t = 12;
				//y = 24;//d_p - set delta p
				break;
			case MAIN_MENU_STATE_DELTA_f_WAIT:
				//flag_count[p] = 1;
				flag_count_f = 1;
				res_f = state;
				//q = 16;
				//w = 12;
				//e = 18;//d_f - set delta f
				break;
			case MAIN_MENU_STATE_DEFAULT_DELTA_p_WAIT:
				//flag_count[p] = 1;
				//flag_count[f] = 1;
				//res[p] = state;
				flag_p = yesOrNo(p);
				//r = 26;
				//t = 16;
				//y = 24;//rdp - reset delta p
				break;
			case MAIN_MENU_STATE_DEFAULT_DELTA_f_WAIT:
				//flag_count[p] = 1;
				//flag_count[f] = 1;
				//res[f] = state;
				flag_f = yesOrNo(f);
				//q = 26;
				//w = 16;
				//e = 18;//rdf - reset delta f
				break;
			case MAIN_MENU_STATE_MODE_p_Wait:
				flag_count_p = 1;
				res_p = state;
				//r = 12;
				//t = 12;
				//y = 12;
				break;
			case MAIN_MENU_STATE_MODE_f_WAIT:
				flag_count_f = 1;
				res_f = state;
				//q = 12;
				//w = 12;
				//e = 12;
				break;
		}
		if (flag_count_p || flag_p)
		{
			WritrArray(3, D_array_maim_menu, ((((byte)main_menu_State)>>1)+1));
		}
		if (flag_count_f || flag_f)
		{
			WritrArray(0, D_array_maim_menu, ((((byte)main_menu_State)>>1)+1));
		}
	}
	else if(menu_state == MENU_STATE_WAIT)//���� ���� �� ��������
	{
		int val;
		for (i = 0; i < 2; i++)//���� ������ ������� ��������� �����������. ����� ���������� �������� �������������� ��� 2 ��� ����� ����,
		// ��� �� �������� ����� ���������� ��� ������ ��������
		{
			val = old_required[i] - required_t[i];
			if ((val > 3) || (val < -3))//�������� �� ����� �� ������������� ���������� �� �������� �����������
			{
			time_ = time;
			old_required[i] = required_t[i];
			}
		}
		
		/*if((time - time_) < 122) //���� ��� �� ������ �������� ����� ����� ����, ��� ����������
		// ����������� ��������� �������� ��������
		{
			flag_count[p] = 1;
			flag_count[f] = 1;
			res[p] = required_t[p];
			res[f] = required_t[f];
		}
		else
		{
			if((power[p] == false) && (real_adc[p] > 650))
			{
				flag_count[p] = 0;
				for (i = 0; i < 3; i++)
				{
				array_t[i] = 10;
				}
			}
			else
			{
				flag_count[p] = 1;
				res[p] = real_t[p];	
			}
			if((power[f] == false) && (real_adc[f] > 650))
			{
				flag_count[f] = 0;
				for (i = 3; i < 6; i++)
				{
					array_t[i] = 10;
				}
			}
			else
			{
				flag_count[f] = 1;
				res[f] = real_t[f];
			}
		}*/
		if((time - time_) < 122) //���� ��� �� ������ �������� ����� ����� ����, ��� ����������
		// ����������� ��������� �������� ��������
		{
			flag_count_p = 1;
			flag_count_f = 1;
			res_p = required_t[p];
			res_f = required_t[f];
		}
		else
		{	
				if((power[p] == false) && (adc[2] > 650))//���� �������� �� ��������� ������� "---"
				{
					flag_count_p = 0;
					//q = 10;
					//w = 10;
					//e = 10;
					WritrArray(0, D_array_maim_menu, 12);
				}
				{
					flag_count_p = 1;
					res_p = real_t[p];
				}				
				if((power[f] == false) && (adc[3] > 650))//���� ��� �� ��������� ������� "---"
				{
					flag_count_f = 0;
					//r = 10;
					//t = 10;
					//y = 10;
					WritrArray(3, D_array_maim_menu, 12);
				}
				else
				{
					flag_count_f = 1;
					res_f = real_t[f];
				}
		}
	}
	
	//res_p = del;
	//BitTest(TIMSK2,TOIE2);
	
	//if (!flag_p)
	//{
		//if (flag_count_p)
		//{
			//for (i = 2; i >= 0; i--) // ���������� ���� � �����
			//{
				//array_t[i] = res_p % 10;
				//res_p /= 10;
			//}
		//}
		//else
		//{
			//array_t[0] = q;
			//array_t[1] = w;
			//array_t[2] = e;
		//}
	//}
	////res_f = CompareReg_1A;
	//if (!flag_f)
	//{
		//if (flag_count_f)
		//{
			//for (i = 5; i >= 3; i--)
			//{
				//array_t[i] = res_f % 10;
				//res_f /= 10;
			//}
		//}
		//else
		//{
			//array_t[3] = r;
			//array_t[4] = t;
			//array_t[5] = y;
		//}
	//}
	if (flag_count_p)
	{
		for (i = 2; i >= 0; i--) // ���������� ���� � �����
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
	if (menu_state == MENU_STATE_WAIT)//����������� �������� ������� array_t ���� ���� ������ � ��������� �� ��������� � ����
	{
		checkErrors();
	}
	//res[p] = ButtonCheck(b_p);
  //res_p = abcd;
  //for (i = 2; i >= 0; i--) // ���������� ���� � �����
  //{
	  //array_t[i] = res_p % 10;
	  //res_p /= 10;
  //}
  //res[f] = abcd;
  //res[f] = checkLongPress(b_f);
  //for (i = 5; i >= 3; i--)
  //{
	  //array_t[i] = res[f] % 10;
	  //res[f] /= 10;
  //}
  //Output(6,0);
  //for (byte i = 0; i < 6; i++)//���������(���������) �������� �������� ��� ������
  //{
   ////byte m = array_t[i]; 
   //Output(array_t[i], i);
   //_delay_us(1100);
  //}  
  //Output(7,0);
 }
 
// void Write_array(byte i)
 
void debug(void)//��� �-� ����� ��� �������� "������"
{
	static dword time_deb = 0;
	//time_deb = time;
	static byte i = 0;
	if (i < 12)//������� ����� 0-9,��� ������ "-" � ����������� �� "������ ������"
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
	else//����� ���������� ������, ���� �� ����� ������� - ������� �������������� �����������
	{
		if (time<(time_deb + 1000))//3660 - 1 min
		{
			static dword time_see = 0;
			//time_see = time;
			if (time>(time_see + 61))
			{
				for (byte j = 0; j < 6;j++)
				{
					if (ButtonCheck(j))
					{
						//array_t[0] = j;
						//time_see = time;
						//break;
						WritrArray(0, D_array_deb, j);
						//for(byte count = 0; count < 3; count++)
						//{
							//array_t[count] = pgm_read_byte(&(array_deb[j][count]));
						//}
						//switch(j)
						//{
							//case 0:
							//array_t[0] = 14;
							//array_t[1] = 12;
							//array_t[2] = 24;//b_P
							//time_see = time;
							//break;
							//case 1:
							//array_t[0] = 14;
							//array_t[1] = 12;
							//array_t[2] = 18;//b_F
							//time_see = time;
							//break;
							//case 2:
							//array_t[0] = 14;
							//array_t[1] = 12;
							//array_t[2] = 5;//b_5
							//time_see = time;
							//break;
							//case 3:
							//array_t[0] = 14;
							//array_t[1] = 12;
							//array_t[2] = 29;//b_u
							//time_see = time;
							//break;
							//case 4:
							//array_t[0] = 14;
							//array_t[1] = 12;
							//array_t[2] = 16;//b_d
							//time_see = time;
							//break;
							//case 5:
							//array_t[0] = 8;
							//array_t[1] = 17;
							//array_t[2] = 26;//8Er
							//time_see = time;
							//break;
						//}
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
/*
void debug(void)
{
	static dword time_deb = 0;
	//time_deb = time;
	static byte i = 0;
	if (i < 12)
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
	else
	{
		if (time<(time_deb + 1000))//3660 - 1 min
		{
			static dword time_see = 0;
			//time_see = time;
			if (time>(time_see + 61))
			{
				byte q = 0, w = 0,e = 0;
				for (byte j = 0; j < 6;j++)
				{
					if (ButtonCheck(j))
					{
						//array_t[0] = j;
						//time_see = time;
						//break;
					
						switch(j)
						{
							case 0:
							q = 14;
							w = 12;
							e = 24;//b_P
							time_see = time;
							break;
							case 1:
							q = 14;
							w = 12;
							e = 18;//b_F
							time_see = time;
							break;
							case 2:
							q = 14;
							w = 12;
							e = 5;//b_5
							time_see = time;
							break;
							case 3:
							q = 14;
							w = 12;
							e = 29;//b_u
							time_see = time;
							break;
							case 4:
							q = 14;
							w = 12;
							e = 16;//b_d
							time_see = time;
							break;
							case 5:
							q = 8;
							w = 17;
							e = 26;//8Er
							time_see = time;
							break;
						}
						
					}
				}
				array_t[0] = q;
				array_t[1] = w;
				array_t[2] = e;
				_delay_ms(6);
			}
		}
		else
		{
			debug_flag = 0;
		}
	}
}
*/
//---------------------------------���������� �������, ������� ��������� �� �����-------------------------------
//------------------���� � ��������� ������-------------------------------------
void menuWait(void)//��� ������� ��� ��������� ���� � ������������ ���/���� ��������� � ����
{
	//Output(8,0);
	if (ButtonCheck(b_set))//�������� ������ ���
	{
		//Station_State_Old = Station_State;
		//Station_State = Station_Setting;
		menu_state = MENU_STATE_MAIN;
		main_menu_State = MAIN_MENU_STATE_IDLE;
		power[p] = 0;
		power[f] = 0;
		Solid_off();
		Dryer_Off();
	}
	//Output(9,0);
	if (ButtonCheck(b_p))
	{
		power[p] = !power[p];
		if (power[p])
		{
			Solid_on();
		} 
		else
		{
			Solid_off();
		}
	}
	if (mode[f])// � ���� �� ���� ������� ��������� �� ������. ���� ��� == 1 �� ������ ���������
	{
		if (ButtonCheck(b_f))
		{
			power[f] = !power[f];
			if (power[f])
			{
				Dryer_On;
			} 
			else
			{
				Dryer_Off();
			}
		}
		else
		{
			if (checkState())
			{
				if (power[f])
				{
					Dryer_Off();
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
		if (ButtonCheck(b_f))
		{
			power[f] = !power[f];
			if (power[f])
			{
				Dryer_On;
			}
			else
			{
				Dryer_Off();
			}
		}
	}
	if (ButtonCheck(b_down))
	{
			error_p = 0;
			error_f = 0;
	}
	
	//Output(10,0);
}

int valNonVal(int *pointer)//��� �-� �������� ��� ��������� ������ ��������� � ���������� �������������
{
	int val = *pointer;
	if (ButtonCheck(b_up))
	{
		val = !val;
	}
	if (ButtonCheck(b_down))
	{
		val = !val;
	}
	if (ButtonCheck(b_set))
	{
		//i++;
		//(((byte)main_menu_State)+1);
		main_menu_State=(MAIN_MENU_StateTypeDef)(((byte)main_menu_State)+1);
		// = (byte)main_menu_State + 1;
	}
	return val;
}

int valIncrementDecrement(int *pointer)//��� �-� �������� ��� ��������� ������ ��������� � ���������� �������������
{
	int val = *pointer;
	if (ButtonCheck(b_up))
	{
		val++;
	}
	if (ButtonCheck(b_down))
	{
		val--;
	}
	if (ButtonCheck(b_set))
	{
		//i++;
		//byte(main_menu_State)++;
		main_menu_State=(MAIN_MENU_StateTypeDef)(((byte)main_menu_State)+1);
		//(((byte)main_menu_State)+1);
	}
	return val;
}

void menuMain(void)//���� ������� � ��������
{
	//���� 2 �������� ��������� �������� ����� � ��� ���������(� ����� WAIT) � ���������� ������
	
	
	
	//static byte power[2]; // ����� ��� ��� ���� �������� � ���
	//byte i = 0;
	//int val = 0;
	//static dword t = 0;//��� ���������� ����� ���� ����������������� � ����������������� �� ����������� ����� ������
	//while (/*main_menu_State*/ i < MAIN_MENU_STATE_EXIT)
	//{
		switch ((byte)main_menu_State)//byte ������� ��� ����, ���� ���������� �� ������� - "����� ������������ �� ������������"
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
	for (byte i = 0; i < 10; i++)//������ ����������� �����
	{
		summ += eeprom_read_byte(&i);
	}
	eeprom_write_word(&addres_eeprom_10_, summ);
}
//------------------���� � ��������� ������-------------------------------------

/*
void errors(void)
{
	if (power[f]||first_heat_f)//�������� �� ������� ����������
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
				//Dryer_Off();
				error_f[0] = 1;//��� ������� ����������
			}
			else
			{
				interrupt_flag = 0;
				flag_error = 1;
			}
		}
	}
	
	if (first_heat_p)
	{
		//static dword time_heat = 0;
		static int old_a_p = 0;
		static byte flag_error = 0;
		//if (time > (time_heat + 61))//�������� ����� �������
		//{
			if ((real_adc[p] - old_a_p) > 1)
			{
				old_a_p = real_adc[p];
				//time_heat = time;
				flag_error = 0;
			}
			else
			{
				if (flag_error)
				{
					//Solid_off();
					error_p[1] = 1;//�������� �� �����������
				}
				else
				{
					flag_error = 1;
					old_a_p = real_adc[p];
					//time_heat = time;
				}
				
			//}
		}
	}
	
	if (first_heat_f)
	{
		//static dword time_heat = 0;
		static int old_a_p = 0;
		static byte flag_error = 0;
		//if (time > (time_heat + 61))//�������� ����� �������
		//{
			if ((real_adc[f] - old_a_p) > 1)
			{
				old_a_p = real_adc[f];
				//time_heat = time;
				flag_error = 0;
			}
			else
			{
				if (flag_error)
				{
					//Dryer_Off();
					error_f[1] = 1;//��� �� �����������
				}
				else
				{
					flag_error = 1;
					old_a_p = real_adc[f];
					//time_heat = time;
				}
				
			}
		//}
	}
	
	if (power[p])
	{
		//static dword time_work = 0;
		static byte flag_error = 0;
		//if (time > (time_work + 61))//�������� ����� �������
		//{
			if ((real_t[p] - required_t[p]) > 30)//�������� ������ ����������� ������������ �������� �����������
			{
				if (flag_error)
				{
					Solid_off();
					error_p[2] = 1;
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
		//if (time > (time_work + 61))//�������� ����� �������
		//{
			if ((real_t[f] - required_t[f]) > 30)//��� ������ ����������� ������������ �������� �����������
			{
				if (flag_error)
				{
					Dryer_Off();
					error_f[2] = 1;
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
}
*/

void initAVR()//������������� ��
{
	
	//if (Version == 1)
	//{
		DDRD |= ((1<<DDD1)|(1<<DDD3)|(1<<DDD4));//������
		DDRB |= ((1<<DDB5)|(1<<DDB3)|(1<<DDB2));//������
		DDRD &= ~((1<<DDD0)|(1<<DDD2)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7));//�����, ��������� �� ������ ������
		DDRB &= ~((1<<DDB0)|(1<<DDB1)); //�����, ��������� �� ������ ������
		PORTD |= ((1<<PD0)|(1<<PD5)|(1<<PD6)|(1<<PD7));//���� � ������������ ���������� Page 55 on datasheet
		PORTB |= ((1<<PB0)|(1<<PB1));//���� � ������������ ���������� Page 55 on datasheet
		PORTB &= ~((1<<PB2)|(1<<PB3)|(1<<PB5)); //������ �������
	//}
	//if (Version == 2)
	//{
		//DDRD |= ((1<<DDD1)|(1<<DDD3)|(1<<DDD4));//������
		//DDRB |= ((1<<DDB5)|(1<<DDB3)|(1<<DDB2));//������
		//DDRD &= ~((1<<DDD0)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7));//�����, ��������� �� ������ ������
		//DDRB &= ~((1<<DDB0)|(1<<DDB1)); //�����, ��������� �� ������ ������
		//PORTD |= (1<<PD0);//���� � ������������ ���������� Page 55 on datasheet
		//PORTB &= ~((1<<PORTB2)|(1<<PORTB3)|(1<<PORTB5)); //������ �������
	//}
	//else
	//{
		//DDRD |= ((1<<DDD1)|(1<<DDD3)|(1<<DDD4));//������
		//DDRB |= ((1<<DDB5)|(1<<DDB4)|(1<<DDB3)|(1<<DDB2));//������
		//DDRD &= ~((1<<DDD0)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7));//�����, ��������� �� ������ ������
		//DDRB &= ~((1<<DDB0)|(1<<DDB1)); //�����, ��������� �� ������ ������
		//PORTD |= (1<<PD0);//���� � ������������ ���������� Page 55 on datasheet
		//PORTB &= ~((1<<PORTB2)|(1<<PORTB3)|(1<<PB5)); //������ �������
	//}
	//Semister_off;
	//extern T_Timer_Prescaler;
	//Timer0_init(/*T_Prescaler_1024*/ 5, &Time);//61 ��� � �������
	
	//������������� ��������
	//Timer0_init();
	Timer0_SetPrescaler(D_Prescaler_1024);// 61 ��� � �������
	Timer0_Ovf_Ena;
	GetTimer0_OVF_Vect(Time);//��� ������� �������� �������  ������� ������ ���������� ��� ������ �������
	//� ������ ������ ��� ������������ ������� 0 ��������� �-� Time()
	Timer1_SetPrescaler(D_Prescaler_no);//244,2 ��� � �������;
	GetTimer1_OVF_Vect(OVF);
	GetTimer1_COMPA_Vect(COMP);
	//Timer2_init();
	Timer2_SetPrescaler(D_Prescaler_no);//62500 ��� � �������
	GetTimer2_OVF_Vect(timer_interrupt);
	//GetTimer2_COMPA_Vect(timer_interrupt);
	//GetTimer2_COMPB_Vect(timer_interrupt);
	//������������� ��������
	
	//������������� ������� ����������
	GetINT0_Vect(detect_up);
	Int0_FALLING;
	//Int0_RISING;
	//������������� ������� ����������
	
	//������������� ���
	ADC_Init();
	//������������� ���
	
	//������������� SPI 
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
	//������������� SPI
	
	//initBut();
	/*if(eeprom_read_byte(&addres_eeprom_4) >= 1)
	{
		word value = 0;
		byte value_ = 0;
		eeprom_write_word(&addres_eeprom_0_, value);
		eeprom_write_word(&addres_eeprom_2_, value);
		eeprom_write_byte(&addres_eeprom_4, value_);
	}*/
	delta_t[p] = eeprom_read_word(&addres_eeprom_0_);
	delta_t[f] = eeprom_read_word(&addres_eeprom_2_);
	factor[p] = eeprom_read_word(&addres_eeprom_6_);
	factor[f] = eeprom_read_word(&addres_eeprom_8_);
	mode[p] = eeprom_read_byte(&addres_eeprom_4);
	mode[f] = eeprom_read_byte(&addres_eeprom_5);	
	Solid_off();
	Dryer_Off();
	//word summ = 0;
	//for (byte i = 0; i < 10; i++)//������ ����������� �����
	//{
		//summ += eeprom_read_byte(&i);
	//}
	//if (summ != eeprom_read_word(&addres_eeprom_10_))
	//{
		//BitSet(error_p, 0);
	//}
	wdt_enable(WDTO_2S);//������� ���������� ������ � ����-����� � 2 �������
	sei();//������� ���������� ����������
	
	
}

int main(void)
{
	initAVR();
	//page 41 atmega 8
	//page 79 atmega 328p
	//array_t[0] = 26;
	//array_t[1] = 26;//rr - reason reset
	WritrArray(0, D_array_reason_reset, 4);
	for (byte i = 0; i < 4; i++)
	{
		if (BitTest(MCUSR,i)) 
		{
			array_t[2] = i;//��� � ����� ������ ��������� ������������
			word summ = 0;
			for (byte j = 0; j < 10; j++)//������ ����������� �����
			{
				summ += eeprom_read_byte(&j);
			}
			if (summ != eeprom_read_word(&addres_eeprom_10_))
			{
				BitSet(error_p, 0);
			}
			#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)//���� ������������ �� ATmega168 ��� ATmega328P
			//switch (i)
			//{
				//case 0:
				//array_t[3] = 11;
				//array_t[4] = 16;
				//array_t[5] = 26;// dr - watchdog system reset
				//break;
				//case 1:
				//array_t[3] = 14;
				//array_t[4] = 0;
				//array_t[5] = 26;//bOr -		brown-out reset
				//break;
				//case 2:
				//array_t[3] = 17;
				//array_t[4] = 27;
				//array_t[5] = 26;//Etr - external reset
				//break;
				//case 3:
				//array_t[3] = 24;
				//array_t[4] = 0;
				//array_t[5] = 26;//POr - power-on reset
				//break;
			//}
			WritrArray(3, D_array_reason_reset, i);
			#endif
			break;
		}
	}
	//MCUSR = 0;	
	//byte res_p = TCNT0;
	while(time < 55)
	{
		SPIOutput();
		wdt_reset();//������� ������ ������� ���� ������ ������������ ��
	}
	//Output(0,0);
	static dword time_screen = 0;
	//static dword time_error = 0;
	while (1)
	{
		//Output(2,0);
		if(debug_flag == 0)
		{
			//static dword a;
			//if (time > (a + 61))
			//{
				//abcd = 0;
				//a = time;
			//}
			//if ((time - time_error) > 61)
			//{
				//errors();
				//time_error = time;
			//}
			if ((time-time_screen)>12)//�������� 5 ���� � ���
			{
				Screen();
				time_screen = time;
			}
			static dword time_flag;
			if(time_flag != time)//�������� 61 ��� � �������
			{
				Konvert();
				//checkErrors();
				
				//if (first_heat_p)
				//{
					//firstHeatP();
				//}
				//if (first_heat_f)
				//{
					//firstHeatF();
				//}
				if(power[p])
				{
					Power_p();
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
		wdt_reset();//������� ������ ������� ���� ������ ������������ ��
		 //------------------------������ ��������� ������ ��������� -------------------------------------
		 
		 
		 //------------------����� ��������� ������ ���������-------------------------------------------
	}
    
}

/*
#include <mega328p.h>
#include <delay.h>
float u = 0.0001;//������ ���������� - �����
float e0 = 500.0, e1 = 0.000001;//������� � ���������� ���� ������ 
float Kp = 0.04, Ti = 0.055, Td = 0.00625;//���������� ������������������, ����� ��������������, ����� ����������������
float e = 0.000001;//������� �������� ������
float integral = 0.000001;//������������ ������������
float dif = 0.000001;//���������������� �����������
float SP = 500.0;//�������� ����� - ���������	
const float dt = 0.032768;//����� ����� ���������
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
	e0 = e1;
	e1 = e;
	integral += ((1.0/Ti)*e1*dt);
	dif = Td*(e1 - e0)/dt;
	u = (Kp*e1) + integral + dif;
}
#define FIRST_ADC_INPUT 0
#define LAST_ADC_INPUT 0
unsigned int adc_data[LAST_ADC_INPUT-FIRST_ADC_INPUT+1];
#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))
interrupt [ADC_INT] void adc_isr(void)
{
	static unsigned char input_index=0;
	adc_data[input_index]=ADCW;
	if (++input_index > (LAST_ADC_INPUT-FIRST_ADC_INPUT))
	input_index=0;
	ADMUX=(FIRST_ADC_INPUT | ADC_VREF_TYPE)+input_index;
	delay_us(10);
	ADCSRA|=(1<<ADSC);
}

void main(void)
{
	#pragma optsize-
	CLKPR=(1<<CLKPCE);
	CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
	#ifdef _OPTIMIZE_SIZE_
	#pragma optsize+
	#endif
	DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (1<<DDB1) | (0<<DDB0);
	PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);
	
	TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (0<<WGM00);
	TCCR0B=(0<<WGM02) | (1<<CS02) | (0<<CS01) | (1<<CS00);
	TCNT0=0x00;
	OCR0A=0x00;
	OCR0B=0x00;
	
	TCCR1A=(1<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (1<<WGM11) | (1<<WGM10);
	TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
	TCNT1H=0x00;
	TCNT1L=0x00;
	ICR1H=0x00;
	ICR1L=0x00;
	OCR1AH=0x00;
	OCR1AL=0x00;
	OCR1BH=0x00;
	OCR1BL=0x00;
	
	TIMSK0=(0<<OCIE0B) | (0<<OCIE0A) | (1<<TOIE0);
	
	DIDR0=(0<<ADC5D) | (0<<ADC4D) | (0<<ADC3D) | (0<<ADC2D) | (0<<ADC1D) | (0<<ADC0D);
	ADMUX=FIRST_ADC_INPUT | ADC_VREF_TYPE;
	ADCSRA=(1<<ADEN) | (1<<ADSC) | (0<<ADATE) | (0<<ADIF) | (1<<ADIE) | (0<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	ADCSRB=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);
	#asm("sei")
	
	while (1)
	{
		e = SP - (float)adc_data[0];//��� ������ ������
		if ( u  <  0.0     )                          {OCR1AL = 0x00;               OCR1AH = 0x00;}
		if ((u  >= 0.0     ) && (u <= 255.999 ))      {OCR1AL = (int)u;                           }
		if ((u  >  255.999 ) && (u <= 511.999 ))      {OCR1AL = (int)(u - 255.999); OCR1AH = 0x01;}
		if ((u  >  511.999 ) && (u <= 767.999 ))      {OCR1AL = (int)(u - 511.999); OCR1AH = 0x02;}
		if ((u  >  767.999 ) && (u <= 1023.999))      {OCR1AL = (int)(u - 767.999); OCR1AH = 0x03;}
		if ( u  >  1023.999)                          {OCR1AL = 0xFF;               OCR1AH = 0x03;}
	}
}
*/