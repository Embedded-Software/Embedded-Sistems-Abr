#include "configuration.h"
#include "typedefs.h"

#define SCI_TX_BUFFER_SIZE   64u
#define SCI_RX_BUFFER_SIZE   64u

/* Enumeración con la lista de canales que estarán disponibles */
typedef enum 
{
    SCI_CH0,
    SCI_CH1,
    SCI_NULL
}tSCI_Channel;

/* Estructura de configuración de canal */
typedef struct 
{
 	UINT32		        SCI_baudrate;	/*  */
	tCallbackFunction	SCI_TX_callback;
	tCallbackFunction	SCI_RX_callback;
  enum tSCI_Channel	SCI_channel;	
 	UINT8   	        SCI_TX_enable; 
 	UINT8    	        SCI_RX_enable;
	UINT8			        SCI_TIE_enable; /*  */
	UINT8			        SCI_RIE_enable; /*  */
	UINT8			        SCI_TX_MAX_BUFFER_SIZE;
	UINT8			        SCI_RX_MAX_BUFFER_SIZE;
}tSCIchannel_config;

/* Estructura del driver de configuración */
typedef struct 
{
    	UINT8                       u8Number_of_SCI_channels;
    	const tSCIchannel_config *  ptr_SCIchannels_config;
}tSCIdriver_config;

extern const tSCIdriver_config SCI_config[];