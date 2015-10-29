
/****************************************************************************************************/
/**
\file       pwm.c
\brief      contains the driver core functionality.
\author     Team 3
\version    1.0
\date       11/oct/2014
*/
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/
/** Core modules */
#include "pwm.h"
/** driver types Configuration Options */
#include    "pwm_cfg.h"
/** prototypes and main header Configuration Options */
#include "typedefs.h"
/** frecuency values */
#include "pll.h"

//typedef unsigned   char  UINT8; /*unsigned 8 bit definition */ 
static UINT8 Pwm__u8EnableBuffer;

/*****************************************************************************************************/
/**
* \brief    DutyCycle calculation function
* \author   Andres Sanchez, Cesar Gomez, Juan Carlos Morales, Pablo Camacho
* \param    
* \return   
*/
void Pwm_SetDutyCycle( Pwm_ChannelType ChannelNumber, UINT16 DutyCycle )
{
  Pwm_PeriodType xTempPeriod;
  Pwm_ChannelType xTempChannId;
  UINT8 u8Temp;
  UINT8 u8PwmType;
  UINT8 u8TempPercentage;
  volatile UINT32 u32DesiredDuty=0;
  volatile UINT32 u32TempDuty=0;
  volatile UINT32 u32Temp;
  
// XGATE_lock_hw_semaphore_1();
  
//#if PWM_HW_TYPE==PWM_16BIT_HW
  /* 16-bit mode */
  UINT16 * const pvu16BaseDtyReg=(UINT16 * const)&PWMDTY01;
//#elif PWM_HW_TYPE==PWM_08BIT_HW
  /* 8-bit mode */
 // UINT8 * const pvu8BaseDtyReg=(UINT8 * const)&PWMDTY0;
//#else
/* do nothing */
//#endif

/*  xTempPeriod    = astPwmDevices[INTPWM].ptrPwm_ConfigType[ChannelNumber].xPeriodType;
  u8PwmType      = astPwmDevices[INTPWM].ptrPwm_ConfigType[ChannelNumber].u8PwmType;
  xTempChannId   = astPwmDevices[INTPWM].ptrPwm_ConfigType[ChannelNumber].xChannelType;
  u32DesiredDuty = DutyCycle;
  */
  
 // #if PWM_HW_TYPE==PWM_16BIT_HW
  /* 16-bit mode */
 // Pwm_CheckChannelId(&xTempChannId);
 // #endif
  u8Temp = ON<<xTempChannId;
  
  u32Temp = (UINT32)(u32DesiredDuty*PWM_NUMERIC_HUNDRED);
  u32Temp/= PWM_AR_HUNDRED;
  u8TempPercentage = (UINT8)u32Temp;
  
  if( (TYPEA == u8PwmType) || (TYPED == u8PwmType) )
  {
    /* Types A and D begins high */
    PWMPOL|= u8Temp;/*pwm output is high until the duty count is reached, the low order 8bit channel has the pol control*/
  } 
  else
  {
    /* Types B and C begins low */
    u8Temp^=0xFF;  /* invert the mask */
    PWMPOL&=u8Temp;/*pwm output is low until the duty count is reached*/ 
  }

  u32DesiredDuty =(UINT32)(xTempPeriod)*u8TempPercentage;
  u32DesiredDuty/=(UINT32)PWM_NUMERIC_HUNDRED;
  
  if( (TYPEC == u8PwmType) || (TYPED == u8PwmType) )
  {
    u32TempDuty = (UINT16)((UINT32)xTempPeriod - u32DesiredDuty);
  } 
  else
  {
    u32TempDuty = (UINT16)u32DesiredDuty; 
  }  
  
//#if PWM_HW_TYPE==PWM_16BIT_HW /* 16-bit mode */
  
  /* In 16-bit mode only 4 channels can be selected */
  xTempChannId>>=1; /* Converts "logical" channel into "physical" channel */  
  /* Write PWMDTY<xy> register */
  pvu16BaseDtyReg[xTempChannId]=(UINT16)u32TempDuty;
//#elif PWM_HW_TYPE==PWM_08BIT_HW /* 8-bit mode */

  /* Write PWMDTY<x> register */
  //pvu8BaseDtyReg[xTempChannId]=(UINT8)u32TempDuty;
 
//#endif

//XGATE_release_hw_semaphore_1();
}



/*****************************************************************************************************/
/**
* \brief    Set Period an DutyCycle function
* \author   Andres Sanchez, Cesar Gomez, Juan Carlos Morales, Pablo Camacho
* \param    
* \return   
*/
void Pwm_SetPeriodAndDuty( Pwm_ChannelType ChannelNumber, Pwm_PeriodType Period, UINT16 DutyCycle )
{
  
  XGATE_lock_hw_semaphore_1();
  
  Pwm_ChannelType xTempChannId = astPwmDevices[INTPWM].ptrPwm_ConfigType[ChannelNumber].xChannelType;
  
//#if PWM_HW_TYPE==PWM_16BIT_HW /* 16-bit mode */
  UINT16 * const pvu16BasePerReg=(UINT16 * const)&PWMPER01;
  Pwm_CheckChannelId(&xTempChannId);
  
  /* In 16-bit mode only 4 channels can be selected */
  xTempChannId>>=1; /* Converts "logical" channel into "physical" channel */
  /* Write PWMPER<xy> register */
  pvu16BasePerReg[xTempChannId] = Period;
  
//#elif PWM_HW_TYPE==PWM_08BIT_HW /* 8-bit mode */
 // UINT8 * const pvu8BasePerReg=(UINT8 * const)&PWMPER0;
  
  /* Write PWMPER<xy> register */
//  pvu8BasePerReg[xTempChannId] = Period;
//#endif  
  Pwm_SetDutyCycle(ChannelNumber,DutyCycle);
}



//#if PWM_HW_TYPE==PWM_16BIT_HW /* 16-bit mode */
/*****************************************************************************************************/
/**
* \brief    If channel 0, 2, 4 or 6 is selected it's changed to 1, 3, 5 or 7 since in 16-bit mode
*           the chanel pairs (0-1), (2-3), (4-5) and (6-7) are used 
* \author  Andres Sanchez, Cesar Gomez, Juan Carlos Morales, Pablo Camacho
* \param    ChannelNumber Physical channel number
* \return   void
*/
void Pwm_CheckChannelId(Pwm_ChannelType * ChannelNumber)
{
  Pwm_ChannelType xTempChannId = *ChannelNumber;
  
  /* Filter any invalid channel ID */
  xTempChannId&=PWM_CHANNEL_MSK;
  /* Channels 0, 2, 4 or 6 are changed to 1, 3, 5 or 7 */
  xTempChannId|=1;
  *ChannelNumber = xTempChannId;
  
 // XGATE_release_hw_semaphore_1();
}
//#endif

/** Driver function prototypes */
extern void Pwm_SetDutyCycle(Pwm_ChannelType ChannelNumber,UINT16 DutyCycle);
extern void Pwm_SetPeriodAndDuty( Pwm_ChannelType ChannelNumber, Pwm_PeriodType Period, UINT16 DutyCycle );
extern void Pwm_CheckChannelId(Pwm_ChannelType * ChannelNumber);