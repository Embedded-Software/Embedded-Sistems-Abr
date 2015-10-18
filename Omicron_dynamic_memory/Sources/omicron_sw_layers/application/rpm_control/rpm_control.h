/****************************************************************************************************/
/**
\file       rpm_control.h
\brief      RPM control functions using available push buttons on DEMOS12XEP100 board
\author     Abraham Tezmol
\version    1.0
\date       08/Dec/2011
*/
/****************************************************************************************************/

#ifndef __RPM_CONTROL_H        /*prevent duplicated includes*/
#define __RPM_CONTROL_H

/*****************************************************************************************************
* Include files
*****************************************************************************************************/

/** Configuration Options */
#include    "configuration.h"
/** MCU derivative information */
#include    __MCU_DERIVATIVE
/** Variable types and common definitions */
#include    "typedefs.h"
/** Global XGATE specific mnemonics and definitions */
#include    "xgate_config.h"
/** Global Input/output definitions */
#include    "io.h"
/** Global CAM/Crank variables definition */
#include    "cam_crank.h"

/*****************************************************************************************************
* Definition of module wide VARIABLEs
*****************************************************************************************************/

extern UINT8 u8TimeoutFlag;

/*****************************************************************************************************
* Declaration of module wide TYPEs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of global wide MACROs / #DEFINE-CONSTANTs
*****************************************************************************************************/

#define POSITIVE_RPM_MAX        8000u
#define NEGATIVE_RPM_MIN        3000u
#define RPM_PushButton_Delta    5u

#define SECONDS_PER_MIN    60u
  
#define NUM_PIT_EVENTS  (UINT32)( (SECONDS_PER_MIN) * (PIT_TARGET_FREQ1) )
/*****************************************************************************************************
* Declaration of project wide FUNCTIONS
*****************************************************************************************************/

void vfnGetRPMButtons(void);
void vfnSetPITCounts(void);

#endif /* __RPM_CONTROL_H */