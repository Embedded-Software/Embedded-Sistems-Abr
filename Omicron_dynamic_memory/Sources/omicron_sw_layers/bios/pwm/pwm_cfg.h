#include "configuration.h"
#include "typedefs.h"
#include  "pit.h"

#define PWM_SEMAPHORE	0x02

#define SET_SEM(x)  (XGSEM = 0x0101 << (x))
#define TST_SEM(x)  (XGSEM & 0x0001 << (x))
#define REL_SEM(x)  (XGSEM = 0x0100 << (x))

/* Constantes para seleccionar mediante una */
/* mascara los bits del puerto de salida*/
#define D0    1
#define D1    2
#define D2    4
#define D3    8
#define D4    16
#define D5    32
#define D6    64
#define D7    128

typedef enum 
{
    PWM_CH0,
    PWM_CH1,
    PWM_NULL
}tPWMChannel_Number;

typedef struct 
{
  UINT8     PWM_CHANNEL;
  UINT8     FRECUENCIA;
  UINT8     CICLO_TRABAJO;
  UINT8     PIN;
  UINT8     *PUERTO;
  UINT8     *DDR;
} tPWMChannel_Config;


typedef struct 
{
  UINT8     u8Number_of_PWM_Channels;
  tPWMChannel_Config * ptrPWMchannels_config;
} tPWMDriver_Config;


#pragma DATA_SEG SHARED_DATA

extern tPWMChannel_Config PWMmultchannel_confarray[];
extern tPWMDriver_Config PWMmultdriver_confarray;

#pragma DATA_SEG DEFAULT