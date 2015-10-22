#include "serial_cfg.h"

const tSCIchannel_config SCI_channel_cfg[] =
{
    {   
        (UINT32)9600,		            /* Baudaje para funcionamiento del SCI */
        (tCallbackFunction)NULL,  	    /* Funci�n para transmisi�n */
        (tCallbackFunction)NULL,	    /* Funci�n para recepci�n  */
        SCI_CH0,         	            /* Canal de SCI */	
        (UINT8)ENABLE,                  /* Habilitaci�n de transmisi�n */ 
        (UINT8)ENABLE,                  /* Habilitaci�n de recepci�n */
        (UINT8)ENABLE,                  /* Habilitaci�n del uso de interrupci�n para transmisi�n */
        (UINT8)ENABLE,                  /* Habilitaci�n de interrupci�n para recepci�n */
        (UINT8)SCI_TX_BUFFER_SIZE,  /* Tama�o m�ximo del buffer de transmisi�n */
        (UINT8)SCI_RX_BUFFER_SIZE   /* Tama�o m�ximo del buffer de recepci�n */
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
        sizeof(SCI_channel_cfg)/sizeof(SCI_channel_cfg[0]), /* C�lculo del n�mero de canales */
        &SCI_channel_cfg[0]                                 /* Apuntador al primer elemento
		                                                       del arreglo de estructuras
															   de configuraci�n para cada canal*/
    }
};

