/*****************************************************************************************************
* Include files
*****************************************************************************************************/

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
/** PWM configuration */  
  #include "pwm_cfg.h"

/****************************************************************************************************
* Code of module wide FUNCTIONS
*****************************************************************************************************/ 

/* PWM - Initialization */
void vfnPWM_Init( void );

/* Cam Crank - Start */
void vfnPWM_Start ( void );

/* Emulated PWM - Initialization */
#pragma CODE_SEG XGATE_CODE
void interrupt vfnPWM_Init_XGATE_Isr(void);
#pragma CODE_SEG DEFAULT

#pragma CODE_SEG XGATE_CODE
/* Read PWM ISR callback */	
void vfnPWM_Callback( void );
#pragma CODE_SEG DEFAULT

#pragma CODE_SEG XGATE_CODE
void vfnPWM_OUT_Init(UINT8 *, UINT8 *);
#pragma CODE_SEG DEFAULT

#pragma CODE_SEG XGATE_CODE
void PIN_ON(UINT8 *Puerto, UINT8 BitMask);
#pragma CODE_SEG DEFAULT

#pragma CODE_SEG XGATE_CODE
void PIN_OFF(UINT8 *Puerto, UINT8 BitMask);
#pragma CODE_SEG DEFAULT

#pragma CODE_SEG XGATE_CODE
void PIN_TOGGLE(UINT8 *Puerto, UINT8 BitMask);
#pragma CODE_SEG DEFAULT