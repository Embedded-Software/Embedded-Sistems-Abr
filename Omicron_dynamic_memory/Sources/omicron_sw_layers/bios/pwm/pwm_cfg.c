
#include    "pwm_cfg.h"
#include    "typedefs.h"

/******************************************************************************
PWM Configuration Channel Structure
Array PWM_Channels consists of record of structure type tPWMChDescr
For each required PWM channel, identify the relevant port address and pin,
the PWM period and duty cycle, and the starting position

******************************************************************************/

const Pwm_ConfigChannel astPwmChannels[]= 
{ 
/* Pwm_ChannelType  pwmType       idle_State        Pwm_PeriodType     dutyCycle (0x0000...0x8000) */
/*                                                                                0%        100%*/
  { CHANNELPWM7,     TYPED,       idleTypeD,        ((Pwm_PeriodType)0x7F),           0x1000/*12.5%*/       },
  { CHANNELPWM5,     TYPEB,       idleTypeB,        ((Pwm_PeriodType)0x7F),           0x4000/*50%*/         },
  { CHANNELPWM3,     TYPEC,       idleTypeC,        ((Pwm_PeriodType)0x7F),           0x2000/*25%*/         },  
  { CHANNELPWM1,     TYPEA,       idleTypeA,        ((Pwm_PeriodType)0x7F),           0x6000/*75%*/         },
  { CHANNELPWM2,     TYPED,       idleTypeD,        ((Pwm_PeriodType)0xFF),           0x1000/*12.5%*/       },
  { CHANNELPWM4,     TYPEB,       idleTypeB,        ((Pwm_PeriodType)0xFF),           0x4000/*50%*/         },
  { CHANNELPWM6,     TYPEC,       idleTypeC,        ((Pwm_PeriodType)0xFF),           0x2000/*25%*/         },  
  { CHANNELPWM0,     TYPEA,       idleTypeA,        ((Pwm_PeriodType)0xFF),           0x6000/*75%*/         },
};

const Pwm_ConfigDevice astPwmDevices[]=
{ 
    
 {    INTPWM,                                                       // deviceId
       PWM_SIZE(astPwmChannels,Pwm_ConfigChannel),                  // numberChannels 
       0x01,                                                        // pwmsclB 
       0x01,                                                        // pwmsclA
       &astPwmChannels[0]                                           // ptrPwm_ConfigType   
 }
};
