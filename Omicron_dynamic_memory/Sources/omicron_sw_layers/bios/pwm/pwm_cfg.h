/****************************************************************************************************/
/**
\file       pwm_cfg.h
\brief      Includes driver internal type declarations.
\author     Team 3
\version    1.0
\date       11/oct/2014
*/
/****************************************************************************************************/

#ifndef __PWM_CFG_H        /*prevent duplicated includes*/
#define __PWM_CFG_H

/** Variable types and common definitions */
  #include    "typedefs.h"
/** Core modules */
/** Omicron Configuration Options */
  #include    "configuration.h"
/** MCU derivative information */
  #include    __MCU_DERIVATIVE

#define Pwm_ChannelType UINT8
#define Pwm_PeriodType  UINT8

#define TYPEA ((UINT8)0x01)
#define TYPEB ((UINT8)0x08)
#define TYPEC ((UINT8)0x03)
#define TYPED ((UINT8)0x04)

#define EIGHT ((UINT8)8u)

typedef enum
{
  CHANNELPWM0=0, /*channel 0 and 1 */
  CHANNELPWM1,   /*channel 2 and 3 */
  CHANNELPWM2,   /*channel 4 and 5 */
  CHANNELPWM3,   /*channel 6 and 7 */
  CHANNELPWM4,   /*channel 0 and 1 */
  CHANNELPWM5,   /*channel 2 and 3 */
  CHANNELPWM6,   /*channel 4 and 5 */
  CHANNELPWM7,   /*channel 6 and 7 */
  MAX_PWM_CHANNELS
}tenPwmChanIds;

typedef enum
{
  INTPWM=0,
  MAX_DEVICES
}tenDevices;


typedef struct pwm_configType 
{
  Pwm_ChannelType xChannelType; /* Physical Channel ID*/
  UINT8           u8PwmType;
  UINT8           u8IdleState; /*depends on the type of pwm type (A,B,C,D) */
  Pwm_PeriodType  xPeriodType; /*it shall be multiplied by SA o SB period to know the channel period, it shall be a value between  0x0001 and 0xFFFF uS */
  UINT16          u16DutyCycle; 
}Pwm_ConfigChannel;

typedef struct pwm_configdevice 
{
  UINT8                   deviceId;
  UINT8                   numberChannels;
  UINT8                   pwmsclB; /*frecuency for channels 2,3,6,7 must be a value between 0x00 and 0xFF*/
  UINT8                   pwmsclA; /*frecuency for channels 0,1,4,5 must be a value between 0x00 and 0xFF */
  const Pwm_ConfigChannel *ptrPwm_ConfigType;
}Pwm_ConfigDevice;


extern const Pwm_ConfigChannel Pwn_config[];


#endif
