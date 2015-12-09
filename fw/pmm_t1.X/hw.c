#include "hw.h"
#include <stdint.h> 

#ifdef  COMPILER_SDCC
#include "pic16f1829.h"
static __code uint16_t __at (_CONFIG1) configword1 = _FOSC_INTOSC & _WDTE_OFF & _PWRTE_ON & _MCLRE_ON & _BOREN_OFF & _CLKOUTEN_OFF & _FCMEN_ON;
static __code uint16_t __at (_CONFIG2) configword2 = _PLLEN_OFF & _STVREN_ON & _BORV_LO;
void dly_ms(unsigned int i)
	{
	i++;
	}

#elif defined (COMPILER_XC8)
//********************************************************************************************************************
//********************************************************************************************************************
//huge block of code related to XC8 compiler. I was too lazy to make too spaghetii of #ifdef statemenrs for every single
//statement which is different, so I just put it here
//********************************************************************************************************************
//********************************************************************************************************************
#include <xc.h>
// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = ON      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

void dly_ms(unsigned int i)
{
unsigned int j;
for (j=0;j<i;j++)
	__delay_ms(1);
}
#endif


//********************************************************************************************************************
//********************************************************************************************************************
//generic pieces of code for both compilers
//********************************************************************************************************************
//********************************************************************************************************************

void hw_init (void)
	{
	OSCCON = 0x60;
	
	ANSELA = 0;
	ANSELB = 0x80;
	ANSELC = 0x80;

	DISP_PWR_T = 0;
	DISP_PWR = 0;

	PAN_EN_T = 0;
	PAN_EN = 1;

	ADC_CS_T = 0;
	ADC_DO_T = 1;
	ADC_DI_T = 0;
	ADC_CLK_T = 0;
	ADC_CS = 1;
	ADC_CLK = 0;
	
	MUX1_T = 0;
	MUX2_T = 0;
	MUX3_T = 0;
	MUX1 = 0;
	MUX2 = 1;
	MUX3 = 1;
	
	SSP1CON1 = 0x28;
	SSP1STAT = 0x00;
	SSP1ADD = SSP_BRG;
	dly_ms(100);
	}

void iic_start (void)
{
SSP_SEN = 1;
while (SSP_IF==0);
SSP_IF = 0;
}

void iic_stop (void)
{
SSP_PEN = 1;
while (SSP_IF==0);
SSP_IF = 0;
}

void iic_rstart (void)
{
SSP_RSEN = 1;
while (SSP_IF==0);
SSP_IF = 0;
}
void iic_write (unsigned char data)
{
SSP_BUF = data;
while (SSP_IF==0);
SSP_IF = 0;
}

unsigned char iic_read (unsigned char ack)
{
unsigned char temp;
SSP_RCEN = 1;
while (SSP_IF==0);
SSP_IF = 0;
temp = SSP_BUF;
if (ack==0) SSP_ACKDT = 0;
	else SSP_ACKDT = 1;
SSP_ACKEN = 1;
while (SSP_IF==0);
SSP_IF = 0;
return temp;
}

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
/*
unsigned char adc_read_reg (unsigned char addr)
	{
	d1 = ((addr&0x03)<<2)|0x20;
	ADC_CS = 0;
	adc_spi(d1);
	d1 = adc_spi(0);
	ADC_CS = 1;
	return d1;
	}
*/

unsigned int retval;
unsigned int adc_get_data (unsigned char channel, unsigned char gain)
	{
    if (channel!=0)                         //channel is 0 or 1
        channel = 0x50;						//zero is luckily also combination for AN0/AN1
											//0x50 is base for AN2/AN3
    channel = channel | (gain<<1) | 0x01;	//now add gain bits and disable PGA
	adc_write_reg (0,channel);
	ADC_CS = 0;
	adc_spi(0x08);							//start conversion and wait
	while (ADC_DO_I!=0);
	adc_spi(0x10);	
	d1 = adc_spi(0);
	d2 = adc_spi(0);
	ADC_CS = 1;
	retval = d1;							//move two bytes into single 16b word
	retval = retval << 8;					
	retval = retval | d2;
	return retval;		
	}

/*
unsigned int get_adc (unsigned char chnl)
	{
	ADCON0 = (chnl<<2)|0x01;
	ADCON1 = 0xF0;
	PIR1bits.ADIF = 0;
	GO_nDONE = 1;
	while (GO_nDONE==1);
	PIR1bits.ADIF = 0;
	return ADRES;
	}
*/
void meter_res_range (unsigned char range)
	{
	if (range==1)	//low res range
		{
		MUX3 = 0;
		MUX2 = 1;
		MUX1 = 1;
		return;
		}
	if (range==2)	//hi res range
		{
		MUX3 = 1;
		MUX2 = 0;
		MUX1 = 0;
		return;
		}
	else			//for range=0 or fail safe
		{			//volts measurement, both current sources off
		MUX2 = 1;
		MUX3 = 1;
		}
	
	}

