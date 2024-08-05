
#include <mcs51/8052.h>

#include "tools.h"

// #include <mcs51/stc89.h>
// #include <mcs51/lint.h>

#define LSA P2_2
#define LSB P2_3
#define LSC P2_4

unsigned long counter = 0;

void delay_ms(unsigned int ms);

unsigned char segment_codes[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

void select_ditgit(unsigned char i)
{
	switch(i)//位选
	{
	case 0: LSC=1;LSB=1;LSA=1;break;
	case 1: LSC=1;LSB=1;LSA=0;break;
	case 2: LSC=1;LSB=0;LSA=1;break;
	case 3: LSC=1;LSB=0;LSA=0;break;
	case 4: LSC=0;LSB=1;LSA=1;break;
	case 5: LSC=0;LSB=1;LSA=0;break;
	case 6: LSC=0;LSB=0;LSA=1;break;
	case 7: LSC=0;LSB=0;LSA=0;break;
	}
}

void display(unsigned int num)
{
	unsigned int offset = 7;
	while (num > 0) {
		select_ditgit(offset--);
		P0 = segment_codes[num % 10];
		num /= 10;
		delay_ms(1);
		P0 = 0x00;
	}
}

void time0() __interrupt(1)
{
	counter++;
	TH0 = 0xFC;
	TL0 = 0x78;
}

void time0_init()
{
	// increase counter every 1ms
	TMOD = 0x01;

	// set initial value
	TH0 = 0xFC;
	TL0 = 0x78;

	// enable timer interrupt
	ET0 = 1;

	// start timer
	TR0 = 1;

	// enable interrupt
	EA = 1;
}

void start_motor()
{
	P2_7 = 0;
}

void stop_motor()
{
	P2_7 = 1;
}

int is_motor_running()
{
	return !P2_7;
}

int main()
{
	// LED
	P2 = 0xFF;		// 关闭所有LED

	// 电机
	start_motor();


	unsigned long on_time_limit = 300;
	unsigned long off_time_limit = 600;

	time0_init();

	int pressed = 0;
	int sleep = 0;

	unsigned long i = 0;

	while(1)
	{
		display(counter / 1000);

		if (!sleep) {// running
			if (counter >= on_time_limit * 1000) {
				stop_motor();
				counter = 0;
				sleep = 1;
				i = counter / 1000 + 5;
			}
		} else { // sleeping
			if (counter >= off_time_limit * 1000) {
				start_motor();
				counter = 0;
				sleep = 0;
			}
		}

		// if (sleep)  {
		// 	if (counter / 1000 == i) {
		// 		if (!is_motor_running()) {
		// 			start_motor();
		// 		}
		// 	} else {
		// 		if (is_motor_running()) {
		// 			stop_motor();
		// 			i = counter / 1000 + 5;
		// 		}
		// 	}
		// }

		P2_0 = !sleep;
		P2_5 = P3_0;
		P2_6 = P3_1;

		if (P3_1 == 0 && counter > 1000) {
			pressed += 1;
			if (pressed > 2) {
				pressed = 0;
				counter = 0;

				if (is_motor_running()) {
					stop_motor();
					sleep = 1;
					i = counter / 1000 + 5;
				} else {
					start_motor();
					sleep = 0;
				}
			}
		} else {
			pressed = 0;
		}
	}		

	return 0;
}