#include "hw.h"
#include "disp.h"
#include "math.h"
#include "string.h"
#include "helper.h"

void send_usart (unsigned char data);
void send_usart_s (unsigned char * str);
unsigned char read_memory (unsigned int addr);
void write_memory (unsigned int addr, unsigned char data);
unsigned char adc_setup (unsigned char res, unsigned char chnl);
unsigned long adc_get_data (unsigned char res);
unsigned char adc_spi (unsigned char data);
void adc_write_reg (unsigned char addr, unsigned char data);
unsigned char adc_read_reg (unsigned char addr);



unsigned long test;
unsigned char channel,addr,val,str[20];
float x;

void main(void) 
{
hw_init();
disp_init();

adc_write_reg (3,0x02);
adc_write_reg (2,0x10);
adc_write_reg (0,0x51);

disp_clr(0);

for (addr=0;addr<3;addr++)
	{
	disp_set_xy(addr*3,addr);
	disp_puts("abcd1234 ");
	}
/*
x = 233.8878971;
xftoa(x,str,4);
disp_set_xy(0,3);
disp_puts(str);
*/

channel = 0;
addr = 5;

while (1)
	{
	//adc_setup(0,channel);
	if (addr<4)
		{
		adc_write_reg (addr,val);
		addr = 5;
		}
	
	test = adc_get_data(1);
	x = 11.0 * (test * 2.048) / 32767.0;
	xftoa(x,str,4);
	disp_set_xy(0,3);
	disp_puts(str);

//	adc_write_reg (0,0x55);
//	channel = adc_read_reg (0);
	}
}



/*
unsigned char adc_get_drdy (void)
	{
	if (ADC_DO_I==0)
		return 0;
	else
		return 1;
	}
*/
unsigned char adc_spi (unsigned char data)
	{
	unsigned char i,din;
	din = 0;
	for (i=0;i<8;i++)
		{
		if (data&0x80)
			ADC_DI = 1;
		else
			ADC_DI = 0;
		data = data<<1;
		ADC_CLK = 1;
		ADC_DELAY;
		ADC_CLK = 0;
		ADC_DELAY;
		din = din<<1;
		if (ADC_DO_I==1)
			din = din|0x01;
		}
	return din;
	}

volatile unsigned char d1,d2,d3;

void adc_write_reg (unsigned char addr, unsigned char data)
	{
	d1 = ((addr&0x03)<<2)|0x40;
	ADC_CS = 0;
	adc_spi(d1);
	adc_spi(data);
	ADC_CS = 1;
	}

unsigned char adc_read_reg (unsigned char addr)
	{
	d1 = ((addr&0x03)<<2)|0x20;
	ADC_CS = 0;
	adc_spi(d1);
	d1 = adc_spi(0);
	ADC_CS = 1;
	return d1;
	}


unsigned char config;
unsigned char adc_setup (unsigned char res, unsigned char chnl)
	{
	
	}


unsigned long retval;
unsigned long adc_get_data (unsigned char res)
	{
	
	ADC_CS = 0;
	adc_spi(0x08);
	while (ADC_DO_I!=0);
	adc_spi(0x10);
	d1 = adc_spi(0);
	d2 = adc_spi(0);
	ADC_CS = 1;
	retval = d1;
	retval = retval << 8;
	retval = retval | d2;
	return retval;
	}



