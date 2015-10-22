#include "serial_cfg.h"

const tSCIchannel_config SCI_channel_cfg[] =
{
    {   
        (UINT32)9600,		            /* Baudaje para funcionamiento del SCI */
        (tCallbackFunction)NULL,  	    /* Función para transmisión */
        (tCallbackFunction)NULL,	    /* Función para recepción  */
        SCI_CH0,         	            /* Canal de SCI */	
        (UINT8)ENABLE,                  /* Habilitación de transmisión */ 
        (UINT8)ENABLE,                  /* Habilitación de recepción */
        (UINT8)ENABLE,                  /* Habilitación del uso de interrupción para transmisión */
        (UINT8)ENABLE,                  /* Habilitación de interrupción para recepción */
        (UINT8)SCI_TX_BUFFER_SIZE,  /* Tamaño máximo del buffer de transmisión */
        (UINT8)SCI_RX_BUFFER_SIZE   /* Tamaño máximo del buffer de recepción */
    },
    {   
        (UINT32)19200,
        (tCallbackFunction)NULL,
        (tCallbackFunction)NULL,
        SCI_CH1,
        (UINT8)ENABLE,
        (UINT8)ENABLE,
        (UINT8)ENABLE,
        (UINT8)ENABLE,
        (UINT8)SCI_TX_BUFFER_SIZE,
        (UINT8)SCI_RX_BUFFER_SIZE
    },
};


const tSCIdriver_config  SCI_config[] =
{
    {
        sizeof(SCI_channel_cfg)/sizeof(SCI_channel_cfg[0]), /* Cálculo del número de canales */
        &SCI_channel_cfg[0]                                 /* Apuntador al primer elemento
		                                                       del arreglo de estructuras
															   de configuración para cada canal*/
    }
};

