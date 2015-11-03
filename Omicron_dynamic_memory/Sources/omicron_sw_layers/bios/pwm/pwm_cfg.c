
#include    "pwm_cfg.h"
#include    "pwm.h"
#include    "typedefs.h"
/** XGATE definitions */
#include  "xgate_config.h"

/******************************************************************************
PWM Configuration Channel Structure
Array PWM_Channels consists of record of structure type PWM_ConfigChannel
For each required PWM channel, identify the relevant port address and pin,
the PWM period and duty cycle, and the starting position

* \brief    If channel 0, 2, 4 or 6 is selected it's changed to 1, 3, 5 or 7 since in 16-bit mode
*           the chanel pairs (0-1), (2-3), (4-5) and (6-7) are used 
* \author  Andres Sanchez, Cesar Gomez, Juan Carlos Morales, Pablo Camacho
* \param    ChannelNumber Physical channel number
* \return   void
*******************************************************************************/

//#define PWM_HW_TYPE PWM_08BIT_HW



//#pragma DATA_SEG SHARED_DATA 


//const Pwm_ConfigChannel Pwm_ChannelConfig[] =
//const Pwm_ConfigChannel astPwmChannels[]= 
//{ 
/* Pwm_ChannelType  pwmType       idle_State        Pwm_PeriodType     dutyCycle (0x0000...0x8000) */
/*                                                                                0%        100%*/
  //{ CHANNELPWM7,     TYPED,       idleTypeD,        ((Pwm_PeriodType)0x7F),           0x1000/*12.5%*/       },
  //{ CHANNELPWM5,     TYPEB,       idleTypeB,        ((Pwm_PeriodType)0x7F),           0x4000/*50%*/         },
  //{ CHANNELPWM3,     TYPEC,       idleTypeC,        ((Pwm_PeriodType)0x7F),           0x2000/*25%*/         },  
  //{ CHANNELPWM1,     TYPEA,       idleTypeA,        ((Pwm_PeriodType)0x7F),           0x6000/*75%*/         },
  //{ CHANNELPWM2,     TYPED,       idleTypeD,        ((Pwm_PeriodType)0xFF),           0x1000/*12.5%*/       },
  //{ CHANNELPWM4,     TYPEB,       idleTypeB,        ((Pwm_PeriodType)0xFF),           0x4000/*50%*/         },
  //{ CHANNELPWM6,     TYPEC,       idleTypeC,        ((Pwm_PeriodType)0xFF),           0x2000/*25%*/         },  
  //{ CHANNELPWM0,     TYPEA,       idleTypeA,        ((Pwm_PeriodType)0xFF),           0x6000/*75%*/         },
//};

//const Pwm_DriverConfig  Pwm_Config[] =
//{ 
    
 ////{   // INTPWM,                                                       // deviceId
        //PWM_SIZE(astPwmChannels,Pwm_ConfigChannel),                   // numberChannels 
       //PWM_SIZE(Pwm_ChannelConfig,Pwm_ConfigChannel),
 ////      sizeof(Pwm_ChannelConfig)/sizeof(Pwm_ChannelConfig[0]), 
       //0x01,                                                        // pwmsclB 
       //0x01,                                                        // pwmsclA
 ////      &Pwm_ChannelConfig[0]                                           // ptrPwm_ConfigType   
 //}
//};

//#pragma DATA_SEG DEFAULT