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

#ifdef SERIAL_COMM_CHANNEL0
/* Global SCI communication status */
UINT8  u8SCI0_Comm_Status;
/* Length of received frame */
UINT8  u8SCI0_RxLength;
/* Reception array */
tu8ptr_far pu8SCI0_RxData;
/* Pointer to current location on Reception array */
tu8ptr_far pu8SCI0_Receive_Data_ptr;
/* Pointer to actual location being read from the input buffer */
tu8ptr_far pu8SCI0_Read_Receive_Data_ptr;
/* Length of received frame */
UINT8  u8SCI0_TxLength;
/* Transmision array */
tu8ptr_far pu8SCI0_TxData;
/* Pointer to actual location being read from the output buffer */
tu8ptr_far pu8SCI0_Read_Transmit_Data_ptr;
#endif

#ifdef SERIAL_COMM_CHANNEL1
/* Global SCI communication status */
UINT8  u8SCI1_Comm_Status;
/* Reception array */
tu8ptr_far  pu8SCI1_RxData;
/* Length of received frame */
UINT8  u8SCI1_RxLength;
/* Pointer to current location on Reception array */
tu8ptr_far pu8SCI1_Receive_Data_ptr;
/* Pointer to actual location being read from the input buffer */
UINT8 * pu8SCI1_Read_Receive_Data_ptr;
/* Transmision array */
tu8ptr_far  pu8SCI1_TxData;
/* Length of received frame */
UINT8  u8SCI1_TxLength;
/* Pointer to actual location being read from the output buffer */
tu8ptr_far pu8SCI1_Read_Transmit_Data_ptr;
#endif

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
* \brief    SCI port initialization
* \author   Abraham Tezmol
* \param    UINT8 u8SCIPort
* \return   void
*/

void vfnSCI_Init( UINT8  u8SCIPort )
{
    switch( u8SCIPort )
    {
            #ifdef SERIAL_COMM_CHANNEL0
        case ( SCI_PORT0 ):
        /* Configuration of SCI control register 1         */
            SCI0CR1 = 0x00u;
        /*      0b00000000
                ||||||||____ Even parity bit, even parity
                |||||||_____ Parity function bit, parity disabled 
                ||||||______ Idle character bit count begins after start bit 
                |||||_______ Idle line wakeup
                ||||________ One start bit, eight data bits, one stop bit
                |||_________ Rx input internally connected to Tx output
                ||__________ SCI enabled in wait mode
                |___________ Loop Select bit, normal operation enabled
        */
            /* Configuration of SCI control register 2 */
            SCI0CR2 = 0x20u;
        /*       0b00101100
                   ||||||||____ SendBrakeBit
                   |||||||_____ Receiver Wakeup bit
                   ||||||______ Receiver Enable bit
                   |||||_______ Transmiter Enable Bit
                   ||||________ Idle Line Interrupt Enable Bit
                   |||_________ Receiver full interrupt enable bit
                   ||__________ Transmission complete interrupt enable bit
                   |___________ Transmitter Interrupt Enable Bit
        */
            /* Request memory for reception array */
            pu8SCI0_RxData = MemAlloc_Reserve((UINT16)SCI0_RX_MAX_SIZE);
            /* Request memory for transmision array */
            pu8SCI0_TxData = MemAlloc_Reserve((UINT16)SCI0_TX_MAX_SIZE);
            
            break;
            #endif

            #ifdef SERIAL_COMM_CHANNEL1
        case ( SCI_PORT1 ):
        /*Configuration of SCI control register 1         */
            SCI1CR1 = 0x00u;
        /*      0b00000000
                ||||||||____ Even parity bit, even parity
                |||||||_____ Parity function bit, parity disabled 
                ||||||______ Idle character bit count begins after start bit 
                |||||_______ Idle line wakeup
                ||||________ One start bit, eight data bits, one stop bit
                |||_________ Rx input internally connected to Tx output
                ||__________ SCI enabled in wait mode
                |___________ Loop Select bit, normal operation enabled
        */
            /* Configuration of SCI control register 2 */
            SCI1CR2 = 0x20u;
        /*       0b00100000
                   ||||||||____ SendBrakeBit
                   |||||||_____ Receiver Wakeup bit
                   ||||||______ Receiver Enable bit - Disabled
                   |||||_______ Transmiter Enable Bit - Disabled
                   ||||________ Idle Line Interrupt Enable Bit
                   |||_________ Receiver full interrupt enable bit
                   ||__________ Transmission complete interrupt enable bit
                   |___________ Transmitter Interrupt Enable Bit
        */
            /* Request memory for reception array */
            pu8SCI1_RxData = MemAlloc_Reserve((UINT16)SCI1_RX_MAX_SIZE);
            /* Request memory for transmision array */
            pu8SCI1_TxData = MemAlloc_Reserve((UINT16)SCI1_TX_MAX_SIZE);
            break;
            #endif

        default:
            break;
    }
}

/******************************************************************************************************************/
/**
* \brief    SCI port baud rate configuration
* \author   Abraham Tezmol
* \param    UINT8   u8SCIPort. SCI port number
*           UINT16  U16SCIBaudRate. SCI baud rate
* \return   void
*/
void vfnSCI_Configure( UINT8  u8SCIPort, UINT16  u16SCIBaudRate )
{
    switch( u8SCIPort )
    {
        #ifdef SERIAL_COMM_CHANNEL0
        case ( SCI_PORT0 ):
            /* Configure targer baud rate */
            SCI0BD = ( u16SCIBaudRate & 0x1fffu );
            /* Reset auxiliary variables for input buffer management */
            vfnSCI_ClearRx( SCI_PORT0 );
            /* Reset auxiliary variables for output buffer management */
            vfnSCI_ClearTx( SCI_PORT0 );
            break;
            #endif

            #ifdef SERIAL_COMM_CHANNEL1
        case ( SCI_PORT1 ):
            /* Configure targer baud rate */
            SCI1BD = ( u16SCIBaudRate & 0x1fffu );
            /* Reset auxiliary variables for input buffer management */
            vfnSCI_ClearRx( SCI_PORT1 );
            /* Reset auxiliary variables for output buffer management */
            vfnSCI_ClearTx( SCI_PORT1 );
            break;
            #endif

        default:
            break;
    }
}

/******************************************************************************************************************/
/**
* \brief    Turn SCI port ON. Enable transmission and reception.
* \author   Abraham Tezmol
* \param    UINT8   u8SCIPort. SCI port number
* \return   void
*/
void vfnSCI_ON( UINT8  u8SCIPort )
{
    switch( u8SCIPort )
    {
        #ifdef SERIAL_COMM_CHANNEL0
        case ( SCI_PORT0 ):
        /* Enable Reception and Transmission bits on control register 2 */
            SCI0CR2 |= 0x0Cu;
        /* Update SCI Communication status */
            u8SCI0_Comm_Status = SCI_COMM_ON;
            break;
            #endif

            #ifdef SERIAL_COMM_CHANNEL1
        case ( SCI_PORT1 ):
        /*Enable Reception and Transmission bits on control register 2 */
            SCI1CR2 |= 0x0Cu;
        /* Update SCI Communication status */
            u8SCI1_Comm_Status = SCI_COMM_ON;
            break;
            #endif

        default:
            break;
    }
}

/******************************************************************************************************************/
/**
* \brief    Turn SCI port OFF. Disable transmission and reception.
* \author   Abraham Tezmol
* \param    UINT8   u8SCIPort. SCI port number
* \return   void
*/
void vfnSCI_OFF( UINT8  u8SCIPort )
{
    switch( u8SCIPort )
    {
        #ifdef SERIAL_COMM_CHANNEL0
        case ( SCI_PORT0 ):
            /*Disable Reception and Transmission              */ 
            SCI0CR2 &= 0xF3u;
            /* Update SCI Communication status */
            u8SCI0_Comm_Status = SCI_COMM_OFF;
            break;
            #endif

            #ifdef SERIAL_COMM_CHANNEL1
        case ( SCI_PORT1 ):
            /*Disable Reception and Transmission              */ 
            SCI1CR2 &= 0xF3u;
            /* Update SCI Communication status */
            u8SCI1_Comm_Status = SCI_COMM_OFF;
            break;
            #endif

        default:
            break;
    }
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
UINT8 u8SCI_ReadRx( UINT8  u8SCIPort )
{
    UINT8 u8ReturnValue;

    switch( u8SCIPort )
    {
        #ifdef SERIAL_COMM_CHANNEL0
        case ( SCI_PORT0 ):
            u8SCI0_RxLength--;
            u8ReturnValue = *pu8SCI0_Read_Receive_Data_ptr;
            pu8SCI0_Read_Receive_Data_ptr++;
            break;
            #endif

            #ifdef SERIAL_COMM_CHANNEL1
        case ( SCI_PORT1 ):
            u8SCI1_RxLength--;
            u8ReturnValue = *pu8SCI1_Read_Receive_Data_ptr;
            pu8SCI1_Read_Receive_Data_ptr++;
            break;
            #endif

        default:
            u8ReturnValue = 0x00u;
            break;
    }
    return ( u8ReturnValue );
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
UINT8 u8SCI_GetStatus( UINT8  u8SCIPort )
{
    UINT8 u8ReturnValue;

    switch( u8SCIPort )
    {
        #ifdef SERIAL_COMM_CHANNEL0
        case ( SCI_PORT0 ):
            u8ReturnValue = u8SCI0_Comm_Status;
            break;
            #endif

            #ifdef SERIAL_COMM_CHANNEL1
        case ( SCI_PORT1 ):
            u8ReturnValue = u8SCI1_Comm_Status;
            break;
            #endif

        default:
            u8ReturnValue = 0x00u;
            break;
    }
    return ( u8ReturnValue );
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
void vfnSCI_WriteTx( UINT8  u8SCIPort, UINT8  u8TxDataByte )
{
    switch( u8SCIPort )
    {
        #ifdef SERIAL_COMM_CHANNEL0
        case ( SCI_PORT0 ):
            if( SCI0SR1_TDRE == 1u )
            {
                SCI0DRL = u8TxDataByte;
            }
            else
            {
                /* Update SCI Communication status */
                u8SCI0_Comm_Status = SCI_COMM_TX_BUSY;
            }
            break;
            #endif

            #ifdef SERIAL_COMM_CHANNEL1
        case ( SCI_PORT1 ):
            if( SCI1SR1_TDRE == 1u )
            {
                SCI1DRL = ( UINT8 )u8TxDataByte;
            }
            else
            {
                /* Update SCI Communication status */
                u8SCI1_Comm_Status = SCI_COMM_TX_BUSY;
            }
            break;
            #endif

        default:
            break;
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
void vfnSCI_WriteBufferTx( UINT8  u8SCIPort, UINT8 * pu8TxDataBuffer, UINT8  u8BufferLenght )
{
    UINT8 u8Index;

    switch( u8SCIPort )
    {
        #ifdef SERIAL_COMM_CHANNEL0
        case ( SCI_PORT0 ):
            if( u8SCI0_Comm_Status == SCI_COMM_ON )
            {
                for( u8Index = 0u;
                     u8Index < u8BufferLenght;
                     u8Index++ )
                {
                    pu8SCI0_TxData[ u8Index ] = *pu8TxDataBuffer;
                    pu8TxDataBuffer++;
                }
                u8SCI0_TxLength    = u8BufferLenght;
                /* Update SCI Communication status */
                u8SCI0_Comm_Status = SCI_COMM_TX_BUSY;
                /* Enable interrupt-driven transmission scheme */
                SCI0CR2_TIE        = SET;
            }
            break;
            #endif

            #ifdef SERIAL_COMM_CHANNEL1
        case ( SCI_PORT1 ):
            if( u8SCI1_Comm_Status == SCI_COMM_ON )
            {
                for( u8Index = 0; 
                     u8Index < u8BufferLenght;
                     u8Index++ )
                {
                    *pu8SCI1_TxData[ u8Index ] = *pu8TxDataBuffer;
                    pu8TxDataBuffer++;
                }
                u8SCI1_TxLength    = u8BufferLenght;
                /* Update SCI Communication status */
                u8SCI1_Comm_Status = SCI_COMM_TX_BUSY;
                /* Enable interrupt-driven transmission scheme */
                SCI1CR2_TIE        = SET;
            }
            break;
            #endif

        default:
            break;
    }
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
UINT8 u8SCI_CheckRx( UINT8  u8SCIPort )
{
    UINT8 u8ReturnValue;

    switch( u8SCIPort )
    {
        #ifdef SERIAL_COMM_CHANNEL0
        case ( SCI_PORT0 ):
            u8ReturnValue = u8SCI0_RxLength;
            break;
            #endif

            #ifdef SERIAL_COMM_CHANNEL1
        case ( SCI_PORT1 ):
            u8ReturnValue = u8SCI1_RxLength;
            break;
            #endif

        default:
            u8ReturnValue = 0x00u;
            break;
    }
    return ( u8ReturnValue );
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
void vfnSCI_ClearRx( UINT8  u8SCIPort )
{
    switch( u8SCIPort )
    {
 /* Reset auxiliary variables for input buffer management */
            #ifdef SERIAL_COMM_CHANNEL0
        case ( SCI_PORT0 ):
            pu8SCI0_Receive_Data_ptr      = pu8SCI0_RxData;
            pu8SCI0_Read_Receive_Data_ptr = pu8SCI0_Receive_Data_ptr;
            u8SCI0_RxLength               = 0u;
            break;
            #endif

 /* Reset auxiliary variables for input buffer management */
            #ifdef SERIAL_COMM_CHANNEL1
        case ( SCI_PORT1 ):
            pu8SCI1_Receive_Data_ptr      = pu8SCI1_RxData;
            pu8SCI1_Read_Receive_Data_ptr = pu8SCI1_Receive_Data_ptr;
            u8SCI1_RxLength               = 0u;
            break;
            #endif

        default:
            break;
    }
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
void vfnSCI_ClearTx( UINT8  u8SCIPort )
{
    switch( u8SCIPort )
    {
 /* Reset auxiliary variables for output buffer management */
        #ifdef SERIAL_COMM_CHANNEL0
        case ( SCI_PORT0 ):
            /* inhibit interrupt-driven Tx scheme */
            SCI0CR2_TIE                    = CLEAR;
            pu8SCI0_Read_Transmit_Data_ptr = pu8SCI0_TxData;
            u8SCI0_TxLength                = 0u;
            if( u8SCI0_Comm_Status == SCI_COMM_TX_BUSY )
            {
                u8SCI0_Comm_Status = SCI_COMM_ON;
            }
            break;
            #endif

 /* Reset auxiliary variables for output buffer management */
            #ifdef SERIAL_COMM_CHANNEL1
        case ( SCI_PORT1 ):
            /* inhibit interrupt-driven Tx scheme */
            SCI1CR2_TIE                    = CLEAR;
            pu8SCI1_Read_Transmit_Data_ptr = pu8SCI1_TxData;
            u8SCI1_TxLength                = 0u;
            if( u8SCI0_Comm_Status == SCI_COMM_TX_BUSY )
            {
                u8SCI1_Comm_Status = SCI_COMM_ON;
            }
            break;
            #endif

        default:
            break;
    }
}

/****************************************************************************************************/
/**
* \brief    SCI ISR, serves Tx and Rx events
* \author   Abraham Tezmol
* \param    void
* \return   void
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#ifdef SERIAL_COMM_CHANNEL0
void interrupt  vfnSCI0_Tx_Rx_Isr( void  )
{
    if( SCI0SR1_RDRF == 1u )
    {
        if( SCI0SR1_OR == CLEAR)                                /*    Check for buffer overrun condition          */ 
        {
            /* Note: The following line violates MISRA-C 2004 Rule 6.2          */
            /* Rationale: Header file provided by compiler toolset defines      */
            /* SCI0DRL as part of a Union, which is by prohibited by rule       */
            /* 18.4. Therefore, SCI0DRL can not be properly translated as       */
            /* "numeric" value.                                                 */
            *pu8SCI0_Receive_Data_ptr = SCI0DRL;                /*      Copy data from input buffer               */ 
            u8SCI0_RxLength++;                                  /*      Update input data bytes counter           */ 
            pu8SCI0_Receive_Data_ptr++;                         /*      Update input buffer pointer               */ 
        }
        else
        {
            /* Update SCI Communication status */
            u8SCI0_Comm_Status = SCI_COMM_RX_OVERRUN;
        }
    }
    else
    {
        /* If Tx shift register is ready for Tx */
        if( ( SCI0SR1_TDRE == SET ) && ( SCI0CR2_TIE == SET ) )
        {
            /* Check for pending elements to transmit */
            if( u8SCI0_TxLength > 0u )
            {
                /* Send out data byte */
                SCI0DRL = *pu8SCI0_Read_Transmit_Data_ptr;
                /* If pending elements to tx, point to next element*/
                u8SCI0_TxLength--;
                pu8SCI0_Read_Transmit_Data_ptr++;
            }
            else
            {   /* Last element transmitted, inhibit interrupt-driven Tx scheme */ 
                SCI0CR2_TIE                    = CLEAR;
                /* Update SCI Communication status */
                u8SCI0_Comm_Status             = SCI_COMM_ON;
                pu8SCI0_Read_Transmit_Data_ptr = pu8SCI0_TxData;
            }
        }
    }
}
#endif
#pragma CODE_SEG DEFAULT


/****************************************************************************************************/
/**
* \brief    SCI ISR, serves Tx and Rx events
* \author   Abraham Tezmol
* \param    void
* \return   void
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#ifdef SERIAL_COMM_CHANNEL1
void interrupt  vfnSCI1_Tx_Rx_Isr( void  )
{
    if( SCI1SR1_RDRF == 1u )
    {
        if( SCI1SR1_OR == CLEAR)                                /*    Check for buffer overrun condition          */ 
        {
            *pu8SCI1_Receive_Data_ptr = SCI1DRL;                /*      Copy data from input buffer               */ 
            u8SCI1_RxLength++;                                  /*      Update input data bytes counter           */ 
            pu8SCI1_Receive_Data_ptr++;                         /*      Update input buffer pointer               */ 
        }
        else
        {
            /* Update SCI Communication status */
            u8SCI1_Comm_Status = SCI_COMM_RX_OVERRUN;
        }
    }
    else
    {
        /* If Tx shift register is ready for Tx */
        if( ( SCI1SR1_TDRE == SET ) && ( SCI1CR2_TIE == SET ) )
        {
            /* Check for pending elements to transmit */
            if( u8SCI1_TxLength > 0 )
            {
                /* Send out data byte */
                SCI1DRL = *pu8SCI1_Read_Transmit_Data_ptr;
                /* If pending elements to tx, point to next element*/
                u8SCI1_TxLength--;
                pu8SCI1_Read_Transmit_Data_ptr++;
            }
            else
            {   /* Last element transmitted, inhibit interrupt-driven Tx scheme */ 
                SCI1CR2_TIE                    = CLEAR;
                /* Update SCI Communication status */
                u8SCI1_Comm_Status             = SCI_COMM_ON;
                pu8SCI1_Read_Transmit_Data_ptr = pu8SCI1_TxData;
            }
        }
    }
}
#endif
#pragma CODE_SEG DEFAULT