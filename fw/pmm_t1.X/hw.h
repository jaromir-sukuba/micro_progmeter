//this is where compilers switching occurs
#ifdef __XC8 
#define	COMPILER_XC8
#endif
#ifndef __XC8 
#define	COMPILER_SDCC
#endif


#define CAL_COEF_A -0.00040
#define CAL_COEF_B -0.00466
#define CAL_COEF_C  0.0230


#define	_XTAL_FREQ	(2000000*1)
#define	IIC_BAUD	100
#define	SSP_BRG		((_XTAL_FREQ/4000)/IIC_BAUD)-1


#ifdef  COMPILER_SDCC
//********************************************************************************************************************
// a lot of defines fot SDCC compiler
//********************************************************************************************************************
#include "pic16f1829.h"

#define mTIMER_F_CLR TMR0IF = 0
#define mGET_ADC        ((((unsigned int)(ADRESH))<<8)|ADRESL)

#define ADC_DELAY __asm__ ("nop");

#define	DISP_PWR	 LATB5
#define	DISP_PWR_T	 TRISB5
#define	PAN_EN		 LATA2
#define	PAN_EN_T	 TRISA2

#define	ADC_CS		 LATC4
#define	ADC_CS_T	 TRISC4
#define	ADC_CLK		 LATC5
#define	ADC_CLK_T	 TRISC5
#define	ADC_DI		 LATA5
#define	ADC_DI_T	 TRISA5
#define	ADC_DO		 LATA4
#define	ADC_DO_I	 RA4
#define	ADC_DO_T	 TRISA4

#define	MUX2		 LATC0
#define	MUX2_T		 TRISC0
#define	MUX3		 LATC1
#define	MUX3_T		 TRISC1
#define	MUX1		 LATC2
#define	MUX1_T		 TRISC2

#define SSP_SEN     SEN
#define SSP_PEN     PEN
#define SSP_IF      SSP1IF
#define SSP_RSEN    RSEN
#define SSP_BUF     SSPBUF
#define SSP_RCEN    RCEN
#define SSP_ACKDT   ACKDT
#define SSP_ACKEN   ACKEN

#define JOY_BTN_M   RB7

#define ICD_PGD_T     TRISA0
#define ICD_PGC_T     TRISA1

#define mSLEEP      __asm__ ("sleep");
#define mRESET      __asm__ ("reset");

#elif defined (COMPILER_XC8)
//********************************************************************************************************************
// a lot of defines fot XC8 compiler
//********************************************************************************************************************
#include <xc.h>

#define mTIMER_F_CLR INTCONbits.TMR0IF = 0
#define mGET_ADC        ((((unsigned int)(ADRESH))<<8)|ADRESL)

#define ADC_DELAY   __delay_us(1)

#define	DISP_PWR	LATBbits.LATB5
#define	DISP_PWR_T	TRISBbits.TRISB5
#define	PAN_EN		LATAbits.LATA2
#define	PAN_EN_T	TRISAbits.TRISA2

#define	ADC_CS		LATCbits.LATC4
#define	ADC_CS_T	TRISCbits.TRISC4
#define	ADC_CLK		LATCbits.LATC5
#define	ADC_CLK_T	TRISCbits.TRISC5
#define	ADC_DI		LATAbits.LATA5
#define	ADC_DI_T	TRISAbits.TRISA5
#define	ADC_DO		LATAbits.LATA4
#define	ADC_DO_I	PORTAbits.RA4
#define	ADC_DO_T	TRISAbits.TRISA4

#define	MUX2		LATCbits.LATC0
#define	MUX2_T		TRISCbits.TRISC0
#define	MUX3		LATCbits.LATC1
#define	MUX3_T		TRISCbits.TRISC1
#define	MUX1		LATCbits.LATC2
#define	MUX1_T		TRISCbits.TRISC2

#define SSP_SEN     SSP1CON2bits.SEN
#define SSP_PEN     SSP1CON2bits.PEN
#define SSP_IF      SSP1IF
#define SSP_RSEN    SSP1CON2bits.RSEN
#define SSP_BUF     SSPBUF
#define SSP_RCEN    SSP1CON2bits.RCEN
#define SSP_ACKDT   SSP1CON2bits.ACKDT
#define SSP_ACKEN   SSP1CON2bits.ACKEN

#define JOY_BTN_M   PORTBbits.RB7

#define ICD_PGD_T     TRISAbits.TRISA0
#define ICD_PGC_T     TRISAbits.TRISA1

#define mSLEEP      asm("sleep");
#define mRESET      asm("reset");

#endif




//********************************************************************************************************************
//********************************************************************************************************************
//generic defines for both compilers
//********************************************************************************************************************
//********************************************************************************************************************

#define JOY_N   0
#define JOY_U   1
#define JOY_D   2
#define JOY_R   3
#define JOY_L   4
#define JOY_M   5



void hw_init (void);
void iic_start (void);
void iic_start_addr (unsigned char address);
void iic_stop (void);
void iic_rstart (void);
void iic_write (unsigned char data);
unsigned char iic_read (unsigned char ack);
void dly_ms(unsigned int i);
unsigned char adc_setup (unsigned char res, unsigned char chnl);
unsigned int adc_get_data (unsigned char channel, unsigned char gain);
unsigned char adc_spi (unsigned char data);
void adc_write_reg (unsigned char addr, unsigned char data);
unsigned char adc_read_reg (unsigned char addr);
void meter_res_range (unsigned char range);


void shdn (void);

void start_adc (unsigned char chnl);
unsigned int get_adc (void);