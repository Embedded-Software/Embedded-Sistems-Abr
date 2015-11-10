#include "pwm_cfg.h"

#pragma DATA_SEG SHARED_DATA 

tPWMChannel_Config PWMmultchannel_confarray[] = 
{
  {
    PWM_CH0,
    (UINT8) 100,
    (UINT8) 10,
    (UINT8) D3,
    (UINT8 *) &PORTA,
    (UINT8 *) &DDRA
  },
  {
    PWM_CH1,
    (UINT8) 100,
    (UINT8) 20,
    (UINT8) D2,
    (UINT8 *) &PORTA,
    (UINT8 *) &DDRA
  }
};

tPWMDriver_Config PWMmultdriver_confarray =
{
    sizeof(PWMmultchannel_confarray)/sizeof(tPWMChannel_Config),
    &PWMmultchannel_confarray[0]
};

#pragma DATA_SEG DEFAULT