/****************************************************************************************************/
/**
\file       serial.h
\brief      Serial Communication Interface functions
\author     Abraham Tezmol
\version    1.0
\date       26/Jun/2009
*/
/****************************************************************************************************/

#ifndef __SERIAL_H        /*prevent duplicated includes*/
#define __SERIAL_H

/*****************************************************************************************************
* Include files
*****************************************************************************************************/

/** Core modules */
/** Omicron Configuration Options */
  #include    "configuration.h"
/** MCU derivative information */
  #include    __MCU_DERIVATIVE
/** Variable types and common definitions */
  #include    "typedefs.h"

/** Used modules */
/** PLL prototypes and definitions */
  #include    "pll.h"
/** Dynamic Memory allocation services */
  #include    "memory_allocation.h"  

/*****************************************************************************************************
* Declaration of module wide TYPEs 
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs 
*****************************************************************************************************/

#ifdef SERIAL_COMM_CHANNEL0
    #define SCI0_RX_MAX_SIZE    92u
    #define SCI0_TX_MAX_SIZE    16u
#endif


#ifdef SERIAL_COMM_CHANNEL1
    #define SCI1_RX_MAX_SIZE    16u
    #define SCI1_TX_MAX_SIZE    16u
#endif

#ifndef SCI_PORT0
    #define SCI_PORT0           0u
#endif

#ifndef SCI_PORT1    
    #define SCI_PORT1           1u
#endif    

/*-- Communication status defines --------------------------------------------*/

#define SCI_COMM_ON             0u
#define SCI_COMM_OFF            1u
#define SCI_COMM_TX_BUSY        2u
#define SCI_COMM_RX_OVERRUN     3u

/*-- Macros ------------------------------------------------------------------*/
#define SCI115200bps    (UINT16)((UINT32)(gu32BusFrequency) / (UINT32)(1843200u))/* (115200 * 16) */ 
#define SCI57600bps     (UINT16)((UINT32)(gu32BusFrequency) / (UINT32)(921600u)) /* (57600 * 16) */   
#define SCI38400bps     (UINT16)((UINT32)(gu32BusFrequency) / (UINT32)(614400u)) /* (38400 * 16) */
#define SCI19200bps     (UINT16)((UINT32)(gu32BusFrequency) / (UINT32)(307200u)) /* (19200 * 16) */
#define SCI9600bps      (UINT16)((UINT32)(gu32BusFrequency) / (UINT32)(153600u)) /* (9600 * 16)  */
#define SCI4800bps      (UINT16)((UINT32)(gu32BusFrequency) / (UINT32)(76800u))  /* (4800 * 16)  */

/*******************************************************************************/
/* Pointer to current location on Reception array */
extern tu8ptr_far pu8SCI0_Receive_Data_ptr;
/* Pointer to actual location being read from the input buffer */
extern tu8ptr_far pu8SCI0_Read_Receive_Data_ptr;
/* Pointer to actual location being read from the output buffer */
extern tu8ptr_far pu8SCI0_Read_Transmit_Data_ptr;
/* Transmision array */
extern tu8ptr_far pu8SCI0_TxData;
/* Length of Transmited frame */
extern UINT8 u8SCI0_TxLength;

/*****************************************************************************************************
* Declaration of module wide FUNCTIONS
*****************************************************************************************************/

/** SCI Initalization */
void vfnSCI_Init(UINT8 u8SCIPort);
/** SCI Configuration */
void vfnSCI_Configure(UINT8 u8SCIPort, UINT16 u16SCIBaudRate);
/** SCI Port enable (transmission and reception */
void vfnSCI_ON(UINT8 u8SCIPort);
/** SCI Port disable (transmission and reception */
void vfnSCI_OFF(UINT8 u8SCIPort);
/** SCI Low-Level Byte Transmit function **/
void vfnSCI_WriteTx(UINT8 u8SCIPort, UINT8 u8TxDataByte);
/** SCI Byte read from receive buffer **/
UINT8 u8SCI_ReadRx(UINT8 u8SCIPort);
/** SCI Data bytes available from receive buffer **/
UINT8 u8SCI_CheckRx(UINT8 u8SCIPort);
/** Discard any incomming data on SCI receive buffer **/
void vfnSCI_ClearRx(UINT8 u8SCIPort);
/** Discard any queued data on SCI transmit buffer **/
void vfnSCI_ClearTx(UINT8 u8SCIPort);
/** SCI Low-Level Buffer Transmit function - interrupt driven scheme */
void vfnSCI_WriteBufferTx( UINT8  u8SCIPort, UINT8*  pu8TxDataBuffer, UINT8 u8BufferLenght );
/** SCI Status - to be used by upper layers */
UINT8 u8SCI_GetStatus(UINT8 u8SCIPort); 

#pragma CODE_SEG __NEAR_SEG NON_BANKED
#ifdef SERIAL_COMM_CHANNEL0
/** SCI 0 serves Tx and Rx events **/
void interrupt vfnSCI0_Tx_Rx_Isr (void);
#endif
#pragma CODE_SEG DEFAULT

#pragma CODE_SEG __NEAR_SEG NON_BANKED
#ifdef SERIAL_COMM_CHANNEL1
/** SCI 1 serves Tx and Rx events **/
void interrupt vfnSCI1_Tx_Rx_Isr (void);
#endif
#pragma CODE_SEG DEFAULT

/**************************************************************************************************/

#endif /* __SERIAL_H */