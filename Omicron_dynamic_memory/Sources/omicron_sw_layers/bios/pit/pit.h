/****************************************************************************************************/
/**
\file       pit.h
\brief      Periodic Interrupt Timer initialization and low-level functions and prototypes
\author     Abraham Tezmol
\version    1.0
\date       11/Abr/2010
*/
/****************************************************************************************************/
#ifndef __PIT_H        /*prevent duplicated includes*/
#define __PIT_H

/*****************************************************************************************************
* Include files
*****************************************************************************************************/

/** Core Modules */
/** Configuration Options */
  #include    "configuration.h"
/** S12X derivative information */
  #include    __MCU_DERIVATIVE
/** Variable types and common definitions */
  #include    "typedefs.h"

/** Used Modules */
/** PLL definitions and prototypes*/
  #include    "pll.h"
/* XGATE relevant data types and prototypes */
  #include "xgate_vectors.h"
/** XGATE Intrinsics */
	#include "intrinsics_xgate.h"

/*****************************************************************************************************
* Declaration of module wide TYPES
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of  VARIABLEs - 
*****************************************************************************************************/

/* vfnPIT0_Callback, function to be called upon timeout of PIT channel 0 */
extern tCallbackFunction vfnPIT0_Callback;

/* vfnPIT1_Callback, function to be called upon timeout of PIT channel 1 */
#pragma DATA_SEG SHARED_DATA
	extern tCallbackFunction vfnPIT1_Callback;
#pragma DATA_SEG DEFAULT	

/*****************************************************************************************************
* Definition of module wide MACROS / #DEFINE-CONSTANTS 
*****************************************************************************************************/

/** Periodic Interrupt Timer definitions */
#define    PIT_MICROTIMER_PERIOD            1000000
#define    PIT_TARGET_FREQ                  2000

/** Periodic Interrupt Timer macros */
#define    PIT_MICROTIMER_DIV               ( UINT8 ) ( ( gu32BusFrequency / PIT_MICROTIMER_PERIOD ) - 1 )
#define    PIT_VALUE                        ( ( PIT_MICROTIMER_PERIOD / PIT_TARGET_FREQ ) - 1 )

#define    PIT_MICROTIMER_PERIOD1           200000
#define    PIT_TARGET_FREQ1                 100000

#define    PIT_MICROTIMER1_DIV              ( UINT8 ) ( ( gu32BusFrequency / PIT_MICROTIMER_PERIOD1 ) - 1 )
#define    PIT_VALUE1                       ( ( PIT_MICROTIMER_PERIOD1 / PIT_TARGET_FREQ1 ) - 1 )

/*****************************************************************************************************
* Declaration of module wide FUNCTIONS
*****************************************************************************************************/

/** PIT Low level initialization */
void vfnPIT0_Init(tCallbackFunction  Callback, UINT8 u8Core);

/** PIT Low level Start procedure */
void vfnPIT0_Start(void);

/** PIT Low level initialization */
#pragma CODE_SEG XGATE_CODE
	void vfnPIT1_Init(tCallbackFunction Callback, UINT8 u8Core);
#pragma CODE_SEG DEFAULT

/** PIT Low level Start procedure */
void vfnPIT1_Start(void);

/** PIT Low level Stop procedure */
void vfnPIT_Stop(void);

/** PIT Channel 0 ISR --> Main CPU  */
#pragma CODE_SEG __NEAR_SEG NON_BANKED
	void interrupt  vfnPIT_Channel0_Isr( void  );
#pragma CODE_SEG DEFAULT

/** PIT Channel 1 ISR --> XGATE  */
#pragma CODE_SEG XGATE_CODE
	void interrupt  vfnPIT_Channel1_Isr( void  );
#pragma CODE_SEG DEFAULT

/*******************************************************************************/

#endif /* __PIT_H */