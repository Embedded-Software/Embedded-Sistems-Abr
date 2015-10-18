/*******************************************************************************/
/**
\file       cam_crank.h
\brief      Definitions and function prototypes for CAM CRANK signal generation
\author     Abraham Tezmol
\version    0.1
\date       04/04/2010
*/
/*******************************************************************************/

#ifndef __CAM_CRANK_H        /*prevent duplicated includes*/
#define __CAM_CRANK_H

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
  #include    "xgate_config.h"	

/****************************************************************************************************
* Declaration of module wide TYPEs 
*****************************************************************************************************/

typedef struct {
	UINT8 * u8Port;
 	UINT8 * u8ConfigProfile;
 	UINT16	u16PITCounter;
	UINT8   u8SignalType;
	UINT8 	u8ArrayIndex;  
    UINT8 	u8BitMask;
    UINT8   u8DirConfig;
    UINT8   u8Direction;  
    UINT8   u8ProfileSize;   
} tCamCrankChDescr;

typedef struct {
	UINT8 	      u8ConfigSize;  
	tCamCrankChDescr 	*StrChDesPtr;	
} tCamCrankControl;

/*****************************************************************************************************
* Definition of module wide VARIABLEs 
*****************************************************************************************************/


#pragma DATA_SEG SHARED_DATA
	extern tCamCrankChDescr cam_crank_channels[];                         
#pragma DATA_SEG DEFAULT

/****************************************************************************************************
* Declaration of module wide FUNCTIONs 
****************************************************************************************************/

/****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs 
*****************************************************************************************************/

#define CAM_SIGNAL                      0
#define CRANK_SIGNAL                    1

#define DIMMING_PROFILE_DOWN			0u
#define DIMMING_PROFILE_UP				1u
#define DIMMING_IDLE_COUNTS				300u

#define RPM_VALUE                       1000u
#define TEETH_NUM                       60u

#define UNIDIRECTIONAL                  0u
#define BIDIRECTIONAL                   1u

#define FORWARD                         0u
#define REVERSE                         1u
#define NONE                            2u

/* For a low time of 90us if PIT_TARGET_FREQ1 = 200000 */
#define REVERSE_DELAY_IN_PIT_COUNTS     9u
/* For a low time of 45us if PIT_TARGET_FREQ1 = 200000 */
#define FORWARD_DELAY_IN_PIT_COUNTS     4u  

#define CAM_CRANK_SEMAPHORE	0x01

/****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/

/****************************************************************************************************
* Code of module wide FUNCTIONS
*****************************************************************************************************/ 

#pragma CODE_SEG XGATE_CODE

/* Read Cam Crank ISR callback */	
		void vfnCamCrank_Callback( void );

#pragma CODE_SEG DEFAULT

/* Cam Crank - Initialization */
void vfnCamCrank_Init( void );

#pragma DATA_SEG SHARED_DATA
  extern UINT8 u8CrankConfig[];                           
#pragma DATA_SEG DEFAULT

#pragma DATA_SEG SHARED_DATA
  extern UINT8 u8CamConfig[];                           
#pragma DATA_SEG DEFAULT

#pragma DATA_SEG SHARED_DATA
  extern UINT16 u16RPMValue;
#pragma DATA_SEG DEFAULT

#pragma DATA_SEG SHARED_DATA
  extern UINT8 u8NumberOfTeeth;
#pragma DATA_SEG DEFAULT

#pragma DATA_SEG SHARED_DATA
  extern UINT16 u16PITTimeOut;
#pragma DATA_SEG DEFAULT

/* Emulated PWM - Initialization */
#pragma CODE_SEG XGATE_CODE
	void interrupt vfnCamCrank_Init_XGATE_Isr(void);
#pragma CODE_SEG DEFAULT	

/* Cam Crank - Start */
void vfnCamCrank_Start ( void );

#endif /* __CAM_CRANK_H */

/*******************************************************************************/