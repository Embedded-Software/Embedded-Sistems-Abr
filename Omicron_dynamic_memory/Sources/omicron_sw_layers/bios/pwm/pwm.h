
/****************************************************************************************************/
/**
\file       pwm.h
\brief      pwm signal generation
\author     Andres Sanchez, Cesar Gomez, Juan Carlos Morales, Pablo Camacho
\version    2.0
\date       23/oct/20145
*/
/****************************************************************************************************/

#ifndef __PWM_H        /*prevent duplicated includes*/
#define __PWM_H

/*****************************************************************************************************
* Include files
*****************************************************************************************************/
/** Variable types used by the pwm driver */
#include    "pwm_cfg.h"   
/** Configuration Options */
	#include 	"configuration.h"
/** S12X derivative information */
	#include 	__MCU_DERIVATIVE
/** Variable types */
	#include 	"typedefs.h"
/** Periodic Interrupt Timer routines prototypes */
	#include  "pit.h"	
/** XGATE definitions */
  #include  "xgate_config.h"	

  
#define MAXNUMBERCHANNELS ((UINT8)0x08)/*this is limited by the hardware and the cpnfiguration on PWMCTL */
#define PWM_CHANNEL_MSK   (MAXNUMBERCHANNELS-1)
/*types for calculate the preecaler clock A and B of the pwm this values shall be writed in PWMPRCLK register  */
#define BusClockPeriod    ((UINT8)0x00)
#define ClockPwm2MHZ      ((UINT8)0x11)
#define ClockPwm1MHZ      ((UINT8)0x22)
#define ClockPwm500KHZ    ((UINT8)0x33)
#define ClockPwm250KHZ    ((UINT8)0x44)
#define ClockPwm125KHZ    ((UINT8)0x55)
#define ClockPwm62KHZ     ((UINT8)0x66)  /*62.5KHZ*/
#define ClockPwm31KHZ     ((UINT8)0x77)  /*31.25KHZ*/

#define defaultPeriod     ((UINT16)0xFFFF) /*default value for period*/
#define defaultDutyCycle  ((UINT16)0x4000) /*default value for duty cycle*/


#define PWM_08BIT_HW  (0)
#define PWM_16BIT_HW  (1)
#define PWM_SIZE(x,y)   (sizeof(sx)/sizeof(y))

#define PWM_AR_HUNDRED      ((UINT32)0x8000)
#define PWM_NUMERIC_HUNDRED (100u)

#define PWM_SEMAPHORE	0x01

/** Driver function prototypes */
///extern void Pwm_Init(void);
//extern void Pwm_DeInit(void);
extern void vfnPwm_Init( void );
extern void vfnPwm_Start( void );
extern void Pwm_SetDutyCycle(Pwm_ChannelType ChannelNumber, UINT16 DutyCycle);
extern void Pwm_SetPeriodAndDuty( Pwm_ChannelType ChannelNumber, Pwm_PeriodType Period, UINT16 DutyCycle );
extern void Pwm_CheckChannelId(Pwm_ChannelType * ChannelNumber);

/*PWM - Initialization */
#pragma CODE_SEG XGATE_CODE
	void interrupt vfnPwm_Init_XGATE_Isr(void);
#pragma CODE_SEG DEFAULT	

#endif
    
  