#include "hw.h"
#include "disp.h"
#include "math.h"
#include "string.h"
#include "stdio.h"
#include "helper.h"


void print_meas_val (float val, unsigned char bpoint, unsigned char apoint, unsigned char x, unsigned char y, unsigned char * suffix, unsigned char cursory);
float calc_quad_corr (float input, float * cf);

//volatile variables shared with interrupt code
volatile unsigned char batt_adc, charge_state, joystick_state;

int adc_A, adc_B;
unsigned char range,main_state,set_x,set_y,i,edit_state,edit_point,clear_flag,calculate_flag;
float voltage_A, voltage_B, current_A, resistance_A, resistance_X, batt_volt;
unsigned char t_str[10];
float qcorr_A[3];
float cal_coef[3];

float resistance_P = 11000.00;

void meas_main (void);

void meas_main (void)
{
	meter_res_range(range);
	adc_A = adc_get_data(0,0);
	adc_B = adc_get_data(1,0);
	voltage_A = 1.0 * (adc_A * 2.048) / 32767.0;
	voltage_B = 11.0 * (adc_B * 2.048) / 32767.0;
    voltage_B = voltage_B + calc_quad_corr (voltage_B,qcorr_A);
}

float add_constants[8] = {0.0, 100.000, 10.000, 1.000, 0.0, 0.100, 0.010, 0.001};


void main(void) 
{
hw_init();
disp_init();

adc_write_reg (3,0x02);
adc_write_reg (2,0x10);
//adc_write_reg (0,0x51);

qcorr_A[0] = CAL_COEF_A;
qcorr_A[1] = CAL_COEF_B;
qcorr_A[2] = CAL_COEF_C;

cal_coef[0] = 0.0;
cal_coef[1] = 61.93;
cal_coef[2] = -99.9;

edit_state = 0;
edit_point = 0;
main_state=0;
clear_flag = 1;
calculate_flag = 0;

while (1)
	{
    if (clear_flag)
        {
        clear_flag = 0;
        disp_clr(0);
        for (i=0;i<10;i++) t_str[i] = ' ';
        //memset(t_str,' ',10);
        }
	disp_set_xy(0,0);
	if (main_state==0)
		{
        range = 0;
        meas_main();
		if (calculate_flag) disp_puts("Volts C");
		else	disp_puts("Volts");
		if (calculate_flag) voltage_B = calc_quad_corr (voltage_B,cal_coef);
		print_meas_val(voltage_B,2,3,0,2," V",255);
        if (joystick_state!=0)
            {
            if (joystick_state==JOY_R)
                {
                main_state = 1;
                clear_flag = 1;
                }
            if (joystick_state==JOY_L)
                {
                main_state = 3;
                clear_flag = 1;
                }
            if (joystick_state==JOY_M)
                {
				if (calculate_flag) calculate_flag = 0;
				else calculate_flag = 1;
                }
            joystick_state = 0;
            }
        }
    
	if (main_state==1)
		{
        if (range==0) range = 1;
        meas_main();
		if (calculate_flag) disp_puts("Ohm C");
		else	disp_puts("Ohm");
		if (range==1) 
            resistance_A = 6.80;
        if (range==2) 
            resistance_A = 470.0;
		current_A = voltage_A / resistance_A;
        resistance_X = 5000.0;
		if (adc_A>300)
			{
			resistance_X = voltage_B / current_A;
			resistance_X = (resistance_X*resistance_P) / (resistance_P-resistance_X);
			if (calculate_flag) 
				{
				resistance_X = calc_quad_corr (resistance_X,cal_coef);
				print_meas_val(resistance_X,1,2,0,2,"",255);
				}
			else
				{
				if (resistance_X<4999.0)
					print_meas_val(resistance_X,1,2,0,2," k ",255);
				else
					{
				    disp_set_xy(0,2);
			        disp_puts("----");
					}
				}
			}
        
        if (resistance_X<100.0)
            range = 1;
        if (resistance_X>200.0)
            range = 2;
        if (joystick_state!=0)
            {
            if (joystick_state==JOY_R)
                {
                main_state = 2;
                edit_state = 0;
                set_y = 255;
                clear_flag = 1;
                }
            if (joystick_state==JOY_L)
                {
                main_state = 0;
                clear_flag = 1;
                }
            if (joystick_state==JOY_M)
                {
				if (calculate_flag) calculate_flag = 0;
				else calculate_flag = 1;
                }
            joystick_state = 0;
            }        
		}
    
	if (main_state==2)
		{
        disp_set_xy(0,0);
        disp_puts("edit");
        disp_set_xy(0,1);
        disp_char_print('0' + edit_point);
        print_meas_val(cal_coef[edit_point],3,3,0,2," ",set_y);
        if (joystick_state!=0)
            {
            if (joystick_state==JOY_U)
                {
                if (edit_state) 
                    cal_coef[edit_point]= cal_coef[edit_point] + add_constants[set_y];
                else 
                    {   
                    if (edit_point<3) edit_point++;
                    }
                }
            if (joystick_state==JOY_D)
                {
                if (edit_state) 
                    cal_coef[edit_point]= cal_coef[edit_point] - add_constants[set_y];
                else 
                    {
                    if (edit_point>0) edit_point--;
                    }
                }
            if (joystick_state==JOY_R)
                {
                if (edit_state) 
                    {
                    if (set_y<10) set_y++;
                    }
                else
                    {
                    main_state = 3;
                    clear_flag = 1;
                    }
                }
            if (joystick_state==JOY_L)
                {
                if (edit_state) 
                    {
                    if (set_y>0) set_y--;
                    }
                else    
                    {
                    main_state = 1;
                    clear_flag = 1;
                    }
                }
            if (joystick_state==JOY_M)
                {
                if (edit_state==0) 
                    {
                    edit_state = 1;
                    set_y = 0;
                    }
                else    
                    {
                    edit_state = 0;
                    set_y = 255;
                    }
                }
            joystick_state = 0;
            }         
		}


	if (main_state==3)
		{
        disp_set_xy(0,0);
		disp_puts("Off  ");
		disp_set_xy(0,2);
		batt_adc = (batt_adc/2 - 70) / 3;
		intToStr(batt_adc,t_str,2);		
		disp_puts(t_str);
        if (joystick_state!=0)
            {
			clear_flag = 1;
            if (joystick_state==JOY_R)
                {
                main_state = 0;
                }
            if (joystick_state==JOY_L)
                {
                main_state = 2;
                }
            if (joystick_state==JOY_M)
                {
                shdn();
                }
            joystick_state = 0;
            }
		}

    
    
	/*	
	if (range==1) resistance_A = 6.80;
	if (range==2) resistance_A = 470.00;
	meter_res_range(range);
	adc_A = adc_get_data(0,0);
	voltage_A = 1.0 * (adc_A * 2.048) / 32767.0;
	current_A = voltage_A / resistance_A;
	print_meas_val(voltage_A,2,3,0,1," V ");
	print_meas_val(current_A,2,3,0,2," mA");
	adc_B = adc_get_data(1,0);
	voltage_B = 11.0 * (adc_B * 2.048) / 32767.0;
	voltage_B = voltage_B + calc_quad_corr (voltage_B,qcorr_A);
	print_meas_val(voltage_B,2,3,0,0," V ");
	print_meas_val(resistance_X,1,2,0,3," kO ");
*/
 	/*
	if (adc_A>300)
		{
		resistance_X = voltage_B / current_A;
		print_meas_val(resistance_X,1,2,0,3," kO ");
		}
	 */

	}
}




float calc_quad_corr (float input, float * cf)
	{
	return ((cf[0] * input * input) + (cf[1]*input) + (cf[2])); //when in doubt, use parentheses
	}

void print_meas_val (float val, unsigned char bpoint, unsigned char apoint, unsigned char x, unsigned char y, unsigned char * suffix, unsigned char cursory)
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
	while (*suffix!=0)                  //this is actually strcpy replacement
		t_str[i++] = *suffix++;
	disp_set_xy(x,y);
    if (cursory!=255)
        t_str[cursory] = t_str[cursory] | 0x80;
	disp_puts(t_str);
	}



