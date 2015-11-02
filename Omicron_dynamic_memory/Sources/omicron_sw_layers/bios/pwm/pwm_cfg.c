
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


//const Pwm_ConfigDevice astPwmDevices[]=
//const Pwm_DriverConfig  Pwm_Config[] =
//{ 
    
 {   // INTPWM,                                                       // deviceId
        //PWM_SIZE(astPwmChannels,Pwm_ConfigChannel),                   // numberChannels 
       //PWM_SIZE(Pwm_ChannelConfig,Pwm_ConfigChannel),
/////       sizeof(Pwm_ChannelConfig)/sizeof(Pwm_ChannelConfig[0]), 
       //0x01,                                                        // pwmsclB 
       //0x01,                                                        // pwmsclA
  /////     &Pwm_ChannelConfig[0]                                           // ptrPwm_ConfigType   
 //}
//};
