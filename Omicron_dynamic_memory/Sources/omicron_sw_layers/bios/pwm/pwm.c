#include	"configuration.h"
#include	__MCU_DERIVATIVE
#include	"typedefs.h"
#include 	"pwm.h"
#include	"xgate_config.h"

void vfnPWM_Init( void )
{	
	/* Command SW trigger 1 to execute --> XGATE to initialize appropriate signal mechanism */
	XGATE_SW_TRIGGER( SOFTWARE_TRIGGER_1, SOFTWARE_TRIGGER_ENABLE);
}

void vfnPWM_Start( void )
{
	/* Perform Low level PIT channel 2 start */
	vfnPIT1_Start();
}
