#include "hw.h"

volatile extern unsigned char batt_adc, charge_state, joystick_state;
unsigned char adc_res,joystick_state_new,joystick_state_old, joystick_state_older;
unsigned char interrupt_state;

#ifdef  COMPILER_SDCC
void Intr(void) __interrupt 0
#elif defined (COMPILER_XC8)
static void interrupt isr(void)
#endif
	{ 
	mTIMER_F_CLR;
	if (interrupt_state==0)
		{
		start_adc(8);
		interrupt_state = 1;
		return;
		}
	if (interrupt_state==1)
		{
		interrupt_state = 2;
		adc_res = ((mGET_ADC)>>2);
		joystick_state_older=joystick_state_old;
		joystick_state_old=joystick_state_new;
		joystick_state_new = JOY_N;
		if (adc_res<210)
			{
			joystick_state_new = JOY_U;
			}
		if (adc_res<150)
			{
			joystick_state_new = JOY_L;
			}
		if (adc_res<100)
			{
			joystick_state_new = JOY_D;
			}
		if (adc_res<40)
			{
			joystick_state_new = JOY_R;
			}
		if (JOY_BTN_M==0)
			joystick_state_new = JOY_M;
		
		//if ((joystick_state_old==joystick_state_new)&(joystick_state_new!=0))
		if ((joystick_state_old==joystick_state_new))
			{
			if ((joystick_state_new!=0))
				if ((joystick_state_older==0))
					joystick_state=joystick_state_new;
			}	 
		return;
		}
	if (interrupt_state==2)
		{
		interrupt_state = 3;
		return;
		}
	if (interrupt_state==3)
		{
		interrupt_state = 4;
		return;
		}
	if (interrupt_state==4)
		{
		interrupt_state = 5;
		return;
		}
	if (interrupt_state==5)
		{
		interrupt_state = 0;
		return;
		}
	if (interrupt_state>5)
		interrupt_state = 0;
	}