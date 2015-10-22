/****************************************************************************************************/
/**
\file       serial.c
\brief      Serial Communication Interface functions
\author     Abraham Tezmol
\version    1.0
\date       26/Jun/2009
*/
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/
/** Own headers */
/* SCI routines prototypes */
#include    "serial.h"

/** Other module headers */

/*****************************************************************************************************
* Definition of module wide VARIABLEs 
*****************************************************************************************************/
/* Toda esta parte sobre la definición de variables se elimina */
/* debido a una parte corresponde a la información en las estructuras */
/* de configuración y otra ya se definió en el archivo de cabecera */

/* #ifdef SERIAL_COMM_CHANNEL0 */
/* Global SCI communication status */
/* UINT8  u8SCI0_Comm_Status; */
/* Length of received frame */
/* UINT8  u8SCI0_RxLength; */
/* Reception array */
/* tu8ptr_far pu8SCI0_RxData; */
/* Pointer to current location on Reception array */
/* tu8ptr_far pu8SCI0_Receive_Data_ptr; */
/* Pointer to actual location being read from the input buffer */
/* tu8ptr_far pu8SCI0_Read_Receive_Data_ptr; */
/* Length of received frame */
/* UINT8  u8SCI0_TxLength; */
/* Transmision array */
/* tu8ptr_far pu8SCI0_TxData; */
/* Pointer to actual location being read from the output buffer */
/* tu8ptr_far pu8SCI0_Read_Transmit_Data_ptr;*/
/* #endif */

/* #ifdef SERIAL_COMM_CHANNEL1 */
/* Global SCI communication status */
/* UINT8  u8SCI1_Comm_Status; */
/* Reception array */
/* tu8ptr_far  pu8SCI1_RxData; */
/* Length of received frame */
/* UINT8  u8SCI1_RxLength; */
/* Pointer to current location on Reception array */
/* tu8ptr_far pu8SCI1_Receive_Data_ptr; */
/* Pointer to actual location being read from the input buffer */
/* UINT8 * pu8SCI1_Read_Receive_Data_ptr; */
/* Transmision array */
/* tu8ptr_far  pu8SCI1_TxData; */
/* Length of received frame */
/* UINT8  u8SCI1_TxLength; */
/* Pointer to actual location being read from the output buffer */
/* tu8ptr_far pu8SCI1_Read_Transmit_Data_ptr; */
/* #endif */

const tSCIdriver_config * sci_cfg;

/* Apuntador a la estructura de estado del canal SCI */ 
tSCIchannel_status * sci_channel_status;


/*******************************************************************************************************************
* Declaration of module wide FUNCTIONs 
*******************************************************************************************************************/

/*******************************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs 
*******************************************************************************************************************/

/*******************************************************************************************************************
* Declaration of module wide TYPEs 
*******************************************************************************************************************/

/*******************************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*******************************************************************************************************************/

/*******************************************************************************************************************
* Code of module wide FUNCTIONS
*******************************************************************************************************************/

/******************************************************************************************************************/
/**
* \brief    SCI port initialization, configuration and on
* \author   Abraham Tezmol
* \param    UINT8 u8SCIPort
* \return   void
*/

void vfnSCI_Init(const tSCIdriver_config * SCIdriver_config)
{
    UINT8  i;
    enum tSCI_Channel  _SCI_Channel;
    UINT8  _SCIxCR1;
    UINT8  _SCIxCR2;
    UINT16 _SCI_baudrate; 
    
    sci_cfg = SCIdriver_config;
    sci_channel_status = (tSCIchannel_status *)MemAlloc_NearReserve(sci_cfg->u8Number_of_SCI_channels * sizeof(tSCIchannel_status));
    _SCIxCR1 =  SCIxCR1_DEFAULT;    
    
    for (i = 0; i < sci_cfg->u8Number_of_SCI_channels; i++)
    {  
        /* Indice del canal */
        _SCI_Channel = sci_cfg->ptr_SCIchannels_config[i].SCI_channel;
        
        /* Configuración del registro de control CR2 */
        /* Se habilitan la recepción, la transmisión y la interrupción de recepción */
        /* Lo que equivale a encender el SCI */
        _SCIxCR2 =  SCIxCR2_DEFAULT | (sci_cfg->ptr_SCIchannels_config[i].SCI_RX_enable << 2) | 
                                          (sci_cfg->ptr_SCIchannels_config[i].SCI_TX_enable << 3) | 
                                          (sci_cfg->ptr_SCIchannels_config[i].SCI_RIE_enable << 5);

        /* Memoria para los buffers de transmisión y recepción */
        sci_channel_status[i].u8SCI_RxData = MemAlloc_NearReserve(sci_cfg->ptr_SCIchannels_config[i].SCI_RX_MAX_BUFFER_SIZE);
        sci_channel_status[i].u8SCI_TxData = MemAlloc_NearReserve(sci_cfg->ptr_SCIchannels_config[i].SCI_TX_MAX_BUFFER_SIZE);
        sci_channel_status[i].SCI_channel = _SCI_Channel;
        
        
        /* Modificación de los registros de control del SCI */
        WRITE_SCIxCR1(_SCI_Channel, _SCIxCR1);
        WRITE_SCIxCR2(_SCI_Channel, _SCIxCR2);
        
        
        /* Configuración de la tasa de transferencia en baudios */
        switch (sci_cfg->ptr_SCIchannels_config[i].SCI_baudrate)
        {   
        case (115200):
            _SCI_baudrate = SCI115200bps;
            break;
        case (57600):
            _SCI_baudrate = SCI57600bps;
            break;
        case (38400):
            _SCI_baudrate = SCI38400bps;
            break;
        case (19200):
            _SCI_baudrate = SCI19200bps;
            break;
        case (9600):
            _SCI_baudrate = SCI9600bps;
            break;
        case (4800):
            _SCI_baudrate = SCI4800bps;
            break;       
        }
        
        /* Escribiendo en el registro de configuración para la tasa de transferencia en baudios */
        WRITE_SCIxBD(_SCI_Channel, _SCI_baudrate & 0x1fffu);
        
         
        /* Limpieza del buffer de recepción */
        vfnSCI_ClearRx(_SCI_Channel);
        /* Limpieza del buffer de transmisión */
        vfnSCI_ClearTx(_SCI_Channel);
        
        /* Actualización del estado del SCI */
         sci_channel_status[i].u8SCI_Comm_Status = SCI_COMM_ON;
    }

}

/******************************************************************************************************************/
/**
* \brief    Turn SCI port OFF. Disable transmission and reception.
* \author   Abraham Tezmol
* \param    UINT8   u8SCIPort. SCI port number
* \return   void
*/
void vfnSCI_OFF(enum tSCI_Channel u8SCIPort)
{
    UINT8  _SCIxCR2;
    
    /* Apaga la recepción y transmisión de información para un canal en específico */
    _SCIxCR2 = READ_SCIxCR2(u8SCIPort);
    _SCIxCR2 = (~_CR2_RE & ~_CR2_TE);
    WRITE_SCIxCR2(u8SCIPort, _SCIxCR2);
    
    /* Actualización del estado del SCI */
    sci_channel_status[u8SCIPort].u8SCI_Comm_Status = SCI_COMM_OFF;
}

/******************************************************************************************************************/
/**
* \brief    Read 1 data byte from input buffer. 
* \author   Abraham Tezmol
* \param    UINT8 u8SCIPort - Port number
            0 -> SCI channel 0
            1 -> SCI channel 1
* \return   UINT8 - read byte from input buffer.
*/
UINT8 u8SCI_ReadRx(enum tSCI_Channel u8SCIPort)
{
    UINT8 ValueRead;

    sci_channel_status[u8SCIPort].u8SCI_RxLength--;
    ValueRead = *(sci_channel_status[u8SCIPort].pu8SCI_Read_Receive_Data_ptr);
    sci_channel_status[u8SCIPort].pu8SCI_Read_Receive_Data_ptr++;    

    return(ValueRead);
}

/******************************************************************************************************************/
/**
* \brief    SCI Low-Level Tramsmit Function
* \author   Abraham Tezmol
* \param    UINT8 u8SCIPort - Port number
            0 -> SCI channel 0
            1 -> SCI channel 1
            UINT8 u8TxDataByte - Data byte to be transmitted
* \return   void
*/
void vfnSCI_WriteTx(enum tSCI_Channel u8SCIPort, UINT8 u8TxDataByte)
{
    UINT8 _SCIxSR1;

    _SCIxSR1 = READ_SCIxSR1(u8SCIPort);
    
    if((_SCIxSR1 & _SR1_TDRE) == _SR1_TDRE)
    {
        WRITE_SCIxDRL(u8SCIPort, u8TxDataByte);
    }
    else
    {
        /* Actualización del estado del SCI */
        sci_channel_status[u8SCIPort].u8SCI_Comm_Status = SCI_COMM_TX_BUSY;
    }
}

/******************************************************************************************************************/
/**
* \brief    Provide global status of SCI channel. 
* \author   Abraham Tezmol
* \param    UINT8 u8SCIPort - Port number
            0 -> SCI channel 0
            1 -> SCI channel 1
* \return   UINT8 - SCI channel status.
*/
UINT8 u8SCI_GetStatus(enum tSCI_Channel u8SCIPort)
{
    UINT8 Status;

    Status = sci_channel_status[u8SCIPort].u8SCI_Comm_Status;

    return (Status);
}

/******************************************************************************************************************/
/**
* \brief    Poll for new data flag on SCI input buffer
* \author   Abraham Tezmol
* \param    UINT8 u8SCIPort - Port number
            0 -> SCI channel 0
            1 -> SCI channel 1
* \return   UINT8 - Number of bytes in the reception buffer
*/
UINT8 u8SCI_CheckRx(enum tSCI_Channel u8SCIPort)
{
    UINT8 StatusRX;

    StatusRX = sci_channel_status[u8SCIPort].u8SCI_RxLength;
   
    return (StatusRX);
}

/******************************************************************************************************************/
/**
* \brief    Discard any incomming data on SCI receive buffer
* \author   Abraham Tezmol
* \param    UINT8 u8SCIPort - Port number
            0 -> SCI channel 0
            1 -> SCI channel 1
* \return   void
*/
void vfnSCI_ClearRx(enum tSCI_Channel u8SCIPort)
{
    /* Inicializa las variables de la estructura de estado del canal para desechar lo que se hubiera recibido */
    sci_channel_status[u8SCIPort].pu8SCI_Receive_Data_ptr = sci_channel_status[u8SCIPort].u8SCI_RxData;
    sci_channel_status[u8SCIPort].pu8SCI_Read_Receive_Data_ptr = sci_channel_status[u8SCIPort].pu8SCI_Receive_Data_ptr;
    sci_channel_status[u8SCIPort].u8SCI_RxLength = 0u;
}


/******************************************************************************************************************/
/**
* \brief    Discard any queued data on SCI transmit buffer
* \author   Abraham Tezmol
* \param    UINT8 u8SCIPort - Port number
            0 -> SCI channel 0
            1 -> SCI channel 1
* \return   void
*/
void vfnSCI_ClearTx(enum tSCI_Channel u8SCIPort)
{
    UINT8 _SCIxCR2;
    
    
    _SCIxCR2 = READ_SCIxCR2(u8SCIPort);
    _SCIxCR2 &= ~_CR2_TIE;     /* Bloquea la ocurrencia de una interrupción para transmisión */
    WRITE_SCIxCR2(u8SCIPort, _SCIxCR2);
    
    /* Inicializa las variables de la estructura de estado del canal para desechar lo que se hubiera colocado antes ahí */
    sci_channel_status[u8SCIPort].pu8SCI_Read_Transmit_Data_ptr = sci_channel_status[u8SCIPort].u8SCI_TxData;
    sci_channel_status[u8SCIPort].u8SCI_TxLength = 0u;
    if(sci_channel_status[u8SCIPort].u8SCI_Comm_Status == SCI_COMM_TX_BUSY)
    {
        sci_channel_status[u8SCIPort].u8SCI_Comm_Status = SCI_COMM_ON;
    }

}

/******************************************************************************************************************/
/**
* \brief    SCI Low-Level Tramsmit Buffer Function - Interrupt driven scheme
* \author   Abraham Tezmol
* \param    UINT8 u8SCIPort - Port number
            0 -> SCI channel 0
            1 -> SCI channel 1
            UINT8* pu8TxDataBuffer - Pointer to data buffer to be transmitted
            UINT8  u8BufferLenght - Number of bytes to be transmitted
* \return   void
*/
void vfnSCI_WriteBufferTx(enum tSCI_Channel  u8SCIPort, UINT8 *  pu8TxDataBuffer, UINT8 u8BufferLenght)
{
    UINT8 i;
    UINT8 _SCIxCR2;

    if(sci_channel_status[u8SCIPort].u8SCI_Comm_Status == SCI_COMM_ON)
    {
        for(i = 0u; i < u8BufferLenght; i++)
        {
            sci_channel_status[u8SCIPort].u8SCI_TxData[i] = *pu8TxDataBuffer;
            pu8TxDataBuffer++;
        }
        
        sci_channel_status[u8SCIPort].u8SCI_TxLength = u8BufferLenght;

        /* Actualización del estado del SCI */
        sci_channel_status[u8SCIPort].u8SCI_Comm_Status = SCI_COMM_TX_BUSY;

        /* Enable interrupt-driven transmission scheme */
        _SCIxCR2 = READ_SCIxCR2(u8SCIPort);
        _SCIxCR2 |= _CR2_TIE;
        WRITE_SCIxCR2(u8SCIPort, _SCIxCR2);   
    }
}



#pragma CODE_SEG __NEAR_SEG NON_BANKED
void TxRx_Handler(enum tSCI_Channel u8SCIPort)
{
    UINT8 _SCIxSR1;
    UINT8 _SCIxCR2;
    UINT8 Dato_Leido;
    
    _SCIxSR1 = READ_SCIxSR1(u8SCIPort);
    
    if((_SCIxSR1 & _SR1_RDRF) == _SR1_RDRF)    // Verifica la bandera de registro de recepción
    {
        Dato_Leido = READ_SCIxDRL(u8SCIPort);
        *sci_channel_status[u8SCIPort].pu8SCI_Receive_Data_ptr = Dato_Leido; /*Obtiene el dato del buffer de entrada */
        vfnSCI_WriteTx(u8SCIPort,Dato_Leido);                                /* Hace eco del dato leido */
        sci_channel_status[u8SCIPort].u8SCI_RxLength++;                                   /* Incrementa el contador de datos recibidos */
        sci_channel_status[u8SCIPort].pu8SCI_Receive_Data_ptr++;                          /* Actualiza el apuntador de datos recibidos */        
    }
    else
    {
        _SCIxCR2 = READ_SCIxCR2(u8SCIPort);
        _SCIxSR1 = READ_SCIxSR1(u8SCIPort);

        /* Verifica si el registro de corrimiento para transmisión */
        /* del SCI está listo para transmitir */        
        if(((_SCIxCR2 & _CR2_TIE) == _CR2_TIE) && ((_SCIxSR1 & _SR1_TDRE) == _SR1_TDRE)) /* Verificación de los registros de control y estado */
        {
            /* Verifica si ya transmitió todos los datos */
            if(sci_channel_status[u8SCIPort].u8SCI_TxLength > 0u)
            {
                /* Escribe el dato */
                WRITE_SCIxDRL(u8SCIPort, *sci_channel_status[u8SCIPort].pu8SCI_Read_Transmit_Data_ptr);
                /* Si quedan pendientes elementos de ser transmitidos incrementa el puntero y decrementa el contador */
                sci_channel_status[u8SCIPort].u8SCI_TxLength--;
                sci_channel_status[u8SCIPort].pu8SCI_Read_Transmit_Data_ptr++;
            }
            else
            {   /* Bloquea la interrupción de transmisión y transmite el último dato */ 
                _SCIxCR2 &= ~_CR2_TIE;
                WRITE_SCIxCR2(u8SCIPort, _SCIxCR2);
                /* Actualización del estado del SCI */
                sci_channel_status[u8SCIPort].u8SCI_Comm_Status = SCI_COMM_ON;
                sci_channel_status[u8SCIPort].pu8SCI_Read_Transmit_Data_ptr = sci_channel_status[u8SCIPort].u8SCI_TxData;
            }
        }
    }
}
#pragma CODE_SEG DEFAULT



/****************************************************************************************************/
/**
* \brief    SCI ISR, serves Tx and Rx events
* \author   Abraham Tezmol
* \param    void
* \return   void
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
void interrupt vfnSCI0_TxRx_Isr(void)
{TxRx_Handler(SCI_CH0);}
#pragma CODE_SEG DEFAULT


/****************************************************************************************************/
/**
* \brief    SCI ISR, serves Tx and Rx events
* \author   Abraham Tezmol
* \param    void
* \return   void
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
void interrupt vfnSCI1_TxRx_Isr(void)
{TxRx_Handler(SCI_CH1);}
#pragma CODE_SEG DEFAULT