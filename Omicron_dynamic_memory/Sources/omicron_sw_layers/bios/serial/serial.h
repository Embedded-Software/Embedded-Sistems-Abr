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
#include "configuration.h"
/** MCU derivative information */
#include    __MCU_DERIVATIVE
/** Variable types and common definitions */
#include "typedefs.h"

#include "serial_cfg.h"

/** Used modules */
/** PLL prototypes and definitions */
#include "pll.h"
/** Dynamic Memory allocation services */
#include "memory_allocation.h"
  

/*****************************************************************************************************
* Declaration of module wide TYPEs 
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs 
*****************************************************************************************************/

/* Toda esta parte correspondiente a la configuración se elimina */
/*
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
*/
/* */
/* New typed (struct) for the Channels status control */

typedef struct 
{
	UINT8 *	            u8SCI_RxData;
	UINT8 *	            u8SCI_TxData;
	UINT8 * 	          pu8SCI_Receive_Data_ptr;
	UINT8 * 	          pu8SCI_Read_Receive_Data_ptr;
	UINT8 *	            pu8SCI_Read_Transmit_Data_ptr;
	UINT8  		          u8SCI_RxLength;
	UINT8 		          u8SCI_TxLength;
  UINT8  		          u8SCI_Comm_Status;	
  enum tSCI_Channel   SCI_channel;
} tSCIchannel_status;

/* New typedef (struct) for the Driver status control */

typedef struct
{
    tSCIchannel_status * SCIchannel_status;
    UINT8                u8Number_of_SCI_channels;
} tSCIdriver_status;

/* Extraído del archivo mc9s12xep100.h */
/* Todas estas direcciones de memoria son útiles para el manejo del SCI */
/*** SCI0BD - SCI 0 Baud Rate Register; 0x000000C8 ***/
/*** SCI0ACR1 - SCI 0 Alternative Control Register 1; 0x000000C9 ***/
/*** SCI0ACR2 - SCI 0 Alternative Control Register 2; 0x000000CA ***/
/*** SCI0CR2 - SCI 0 Control Register 2; 0x000000CB ***/
/*** SCI0SR1 - SCI 0 Status Register 1; 0x000000CC ***/
/*** SCI0SR2 - SCI 0 Status Register 2; 0x000000CD ***/
/*** SCI0DRH - SCI 0 Data Register High; 0x000000CE ***/
/*** SCI0DRL - SCI 0 Data Register Low; 0x000000CF ***/

/*** SCI1BD - SCI 1 Baud Rate Register; 0x000000D0 ***/
/*** SCI1ACR1 - SCI 1 Alternative Control Register 1; 0x000000D1 ***/
/*** SCI1ACR2 - SCI 1 Alternative Control Register 2; 0x000000D2 ***/
/*** SCI1CR2 - SCI 1 Control Register 2; 0x000000D3 ***/
/*** SCI1SR1 - SCI 1 Status Register 1; 0x000000D4 ***/
/*** SCI1SR2 - SCI 1 Status Register 2; 0x000000D5 ***/
/*** SCI1DRH - SCI 1 Data Register High; 0x000000D6 ***/
/*** SCI1DRL - SCI 1 Data Register Low; 0x000000D7 ***/

/* Formato de los registros de configuración de los canales del SCI */
/* Extraído del archivo serial.c*/
/* SCIxCR1 = 0x00u;                                */
/*           0b00000000
               ||||||||____ Even parity bit, even parity
               |||||||_____ Parity function bit, parity disabled 
               ||||||______ Idle character bit count begins after start bit 
               |||||_______ Idle line wakeup
               ||||________ One start bit, eight data bits, one stop bit
               |||_________ Rx input internally connected to Tx output
               ||__________ SCI enabled in wait mode
               |___________ Loop Select bit, normal operation enabled
*/

/* SCIxCR2 = 0x00u;                                */
/*           0b00000000
               ||||||||____ SendBrakeBit
               |||||||_____ Receiver Wakeup bit
               ||||||______ Receiver Enable bit
               |||||_______ Transmiter Enable Bit
               ||||________ Idle Line Interrupt Enable Bit
               |||_________ Receiver full interrupt enable bit
               ||__________ Transmission complete interrupt enable bit
               |___________ Transmitter Interrupt Enable Bit
*/
/* Formato de los registros de estado de los canales del SCI */
/* Extraído del texto HCS12 Serial Communications Interface */
/* SCIxSR1 = 0x00u;                                */
/*           0b00000000
               ||||||||_____ Transmit Data Register Empty Flag
               |||||||______ Transmit Complete Flag
               ||||||_______ Receive Data Register Full Flag
               |||||________ Idle Line Flag
               ||||_________ Overrun Flag
               |||__________ Noise Flag
               ||___________ Framing Error Flag
               |____________ Parity Error Flag
*/

/* SCIxSR2 = 0x00u;                                */
/*           0b00000000
               ||||||||_____ 0
               |||||||______ 0
               ||||||_______ 0
               |||||________ 0
               ||||_________ 0
               |||__________ Break Transmit Character Length
               ||___________ Transmitter Pin Data Direction in Single-Wire Mode
               |____________ Receiver Active Flag
*/

/* Valores por default para los registros de configuración del SCI */
#define SCIxCR1_DEFAULT         0x00u    
#define SCIxCR2_DEFAULT         0x00u

/* Máscaras para activar la funcionalidad en los registros de configuración del SCI */

#define _SR1_RDRF                   (UINT8)32
#define _SR1_TDRE                   (UINT8)128
#define _SR1_OR                     (UINT8)8
#define _CR2_RE                     (UINT8)4
#define _CR2_TE                     (UINT8)8
#define _CR2_TIE                    (UINT8)128

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

extern UINT8 u8SCI_TxData[SCI_TX_BUFFER_SIZE];
extern UINT8 u8SCI_RxData[SCI_RX_BUFFER_SIZE];

/*******************************************************************************/
/* Toda esta parte correspondiente a los arreglos con los datos recibidos y que se transmitirán se elimina */
/* */
/* Pointer to current location on Reception array */
/* extern tu8ptr_far pu8SCI0_Receive_Data_ptr; */
/* Pointer to actual location being read from the input buffer */
/* extern tu8ptr_far pu8SCI0_Read_Receive_Data_ptr; */
/* Pointer to actual location being read from the output buffer */
/* extern tu8ptr_far pu8SCI0_Read_Transmit_Data_ptr; */
/* Transmision array */
/* extern tu8ptr_far pu8SCI0_TxData; */
/* Length of Transmited frame */
/* extern UINT8 u8SCI0_TxLength; */
/* */
/*******************************************************************************/
 
/* Funciones y macros para acceder a escribir y leer en los registros del SCI */
#define WRITE_SCIxBD(SCI_Channel, Val)      (*((&SCI0BD)  + (8 * SCI_Channel)) = (UINT16)Val)
#define WRITE_SCIxCR1(SCI_Channel, Val)     (*((&SCI0CR1) + (8 * SCI_Channel)) = (UINT8) Val)
#define WRITE_SCIxCR2(SCI_Channel, Val)     (*((&SCI0CR2) + (8 * SCI_Channel)) = (UINT8) Val)
#define WRITE_SCIxSR2(SCI_Channel, Val)     (*((&SCI0SR2) + (8 * SCI_Channel))|= (UINT8) Val)
#define WRITE_SCIxDRL(SCI_Channel, Val)     (*((&SCI0DRL) + (8 * SCI_Channel)) = (UINT8) Val)
#define READ_SCIxCR2(SCI_Channel)             (*((&SCI0CR2) + (8 * SCI_Channel)))
#define READ_SCIxSR1(SCI_Channel)             (*((&SCI0SR1) + (8 * SCI_Channel)))
#define READ_SCIxDRL(SCI_Channel)             (*((&SCI0DRL) + (8 * SCI_Channel)))

/*****************************************************************************************************
* Declaration of module wide FUNCTIONS
*****************************************************************************************************/

/** SCI Initalization, Configuration and On */
void vfnSCI_Init(const tSCIdriver_config * SCIdriver_config);

/** SCI Port disable (transmission and reception */
void vfnSCI_OFF(enum tSCI_Channel u8SCIPort);

/** SCI Byte read from receive buffer **/
UINT8 u8SCI_ReadRx(enum tSCI_Channel u8SCIPort);

/** SCI Low-Level Byte Transmit function **/
void vfnSCI_WriteTx(enum tSCI_Channel u8SCIPort, UINT8 u8TxDataByte);

/** SCI Status - to be used by upper layers */
UINT8 u8SCI_GetStatus(enum tSCI_Channel u8SCIPort); 

/** SCI Data bytes available from receive buffer **/
UINT8 u8SCI_CheckRx(enum tSCI_Channel u8SCIPort);

/** Discard any incomming data on SCI receive buffer **/
void vfnSCI_ClearRx(enum tSCI_Channel u8SCIPort);

/** Discard any queued data on SCI transmit buffer **/
void vfnSCI_ClearTx(enum tSCI_Channel u8SCIPort);

/** SCI Low-Level Buffer Transmit function - interrupt driven scheme */
void vfnSCI_WriteBufferTx(enum tSCI_Channel  u8SCIPort, UINT8*  pu8TxDataBuffer, UINT8 u8BufferLenght);

#pragma CODE_SEG __NEAR_SEG NON_BANKED

void TxRx_Handler(enum tSCI_Channel u8SCIPort);

/** SCI 0 serves Tx and Rx events **/
void interrupt vfnSCI0_TxRx_Isr (void);

/** SCI 1 serves Tx and Rx events **/
void interrupt vfnSCI1_TxRx_Isr (void);

#pragma CODE_SEG DEFAULT

/**************************************************************************************************/

#endif /* __SERIAL_H */