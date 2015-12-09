#include "hw.h"
#include "disp.h"
#include "math.h"
#include "string.h"
#include "helper.h"

void print_meas_val (float val, unsigned char bpoint, unsigned char apoint, unsigned char x, unsigned char y, unsigned char * suffix);
float calc_quad_corr (float input, float * cf);

int adc_A, adc_B;
unsigned char channel,addr,val,range,gain_b;
float voltage_A, voltage_B, current_A, resistance_A, resistance_X;
unsigned char t_str[10];
unsigned long counter;
//float qcorr_AA,qcorr_AB,qcorr_AC;
float qcorr_A[3];

void main(void) 
{
hw_init();
disp_init();

adc_write_reg (3,0x02);
adc_write_reg (2,0x10);
//adc_write_reg (0,0x51);
disp_clr(0);
disp_set_xy(0,0);
disp_puts("uProgMeter");
counter = 0;
range = 1;
gain_b = 0;

qcorr_A[0] = -0.000594;
qcorr_A[1] = -0.008438;
qcorr_A[2] = 0.0317;
while (1)
	{
	if (range==1) resistance_A = 6.80;
	if (range==2) resistance_A = 470.00;
	meter_res_range(range);
	adc_A = adc_get_data(0,0);
	voltage_A = 1.0 * (adc_A * 2.048) / 32767.0;
	current_A = voltage_A / resistance_A;
	print_meas_val(voltage_A,2,3,0,1," V ");
	print_meas_val(current_A,2,3,0,2," mA");
	adc_B = adc_get_data(1,gain_b);
	voltage_B = 11.0 * (adc_B * 2.048) / 32767.0;
	voltage_B = voltage_B + calc_quad_corr (voltage_B,qcorr_A);
	print_meas_val(voltage_B,2,3,0,0," V ");
	if (adc_A>300)
		{
		resistance_X = voltage_B / current_A;
		print_meas_val(resistance_X,1,2,0,3," kO ");
		}
	}
}

float calc_quad_corr (float input, float * cf)
	{
	return ((cf[0] * input * input) + (cf[1]*input) + (cf[2])); //when in doubt, use parentheses
	}

void print_meas_val (float val, unsigned char bpoint, unsigned char apoint, unsigned char x, unsigned char y, unsigned char * suffix)
	{
	unsigned char i;
	if (val<0.0) 
		{
		t_str[0] = '-';
		val = -val;
		}
	else
		t_str[0] = ' ';
	i = xftoa(val,t_str+1,bpoint,apoint) + 1;
	while (*suffix!=0)
		t_str[i++] = *suffix++;
	disp_set_xy(x,y);
	disp_puts(t_str);
	}



