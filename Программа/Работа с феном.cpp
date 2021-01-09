
byte zero = 2;//подключён для поиска 0 точки
byte dim = 1;// управление симистером
byte fun = 3;// управление вентелятором фена
byte hslf_sin_flag; //флаг, который позволяел включать прерывания только один раз
unsigned long t_sinus;

void setup()
{
	pinMode(zero, INPUT);
	pinMode(dim, OUTPUT);
	pinMode(fun, OUTPUT);
	attachInterrupt(0, zero_p, FALLING);
}
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