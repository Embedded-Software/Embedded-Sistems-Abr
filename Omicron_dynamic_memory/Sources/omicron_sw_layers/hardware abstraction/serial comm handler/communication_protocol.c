/****************************************************************************************************/
/**
\file       communication_protocol.c
\brief      High Level Serial communication protocol services.
\author     Abraham Tezmol
\version    1.0
\date       31/10/2008
*/
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/

/** Own headers */
/** Serial communication protocol definitions */
#include    "communication_protocol.h"
/** RPM control definitions */
#include    "rpm_control.h"

#include    "scheduler.h"

/** Used modules */

/*****************************************************************************************************
* Declaration of module wide FUNCTIONs 
*****************************************************************************************************/

void vfnCOM_Init( void  );
void vfnCOM_SendAck( void );
void vfnCOM_Analyse( void );

/*****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs 
*****************************************************************************************************/

/*****************************************************************************************************
* Declaration of module wide TYPEs 
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide VARIABLEs 
*****************************************************************************************************/

               /* Maximum Rx buffer length minun header, service and length bytes */
               UINT8   au8COM_RX_message[ kCOM_RX_MaxSize - 3];
               UINT8   u8COM_State;
               UINT32  u32COM_control_timer;
               UINT8   u8RxFormatID;
               UINT8   u8COM_RxLength;
               UINT8   u8COM_Byte_counter;
               UINT8   u8COM_RxLength;
               UINT8   u8COM_Checksum;
               UINT8   u8RxSid;
               UINT8   u8RxData;
               UINT8   u8RxByteCount;
               UINT8   au8COM_TX_message[] = { 81, kCOM_Ack, 3, NOT_ACKNOWLEDGED, 0, 0 , 0};
               UINT8   au8COM_TX_RPM_message[]= { 82, kCOM_Get_RPM, 3, 0, 0 , 0, 0};
               UINT8   u8Ack;
               UINT16 u16RPM_temp;

/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/

/*****************************************************************************************************
* Code of module wide FUNCTIONS
*****************************************************************************************************/

/**************************************************************************************************/
/**
* \brief    vfnCommProtocol_Init - Initialization of low-level SCI resources needed for communications
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo     
*/
void vfnCommProtocol_Init(void)
{
    
	/* SCI Initialization */
    vfnSCI_Init(&SCI_config[0]);
	

    vfnCOM_Init();
}

/**************************************************************************************************/
/**
* \brief    vfnCOM_Init - Reset of cummunication tasks
* \author   Abraham Tezmol 
* \param    void
* \return   void
* \todo     
*/
void vfnCOM_Init( void  )
{
    /* Reset Communication State Machine Status*/
    u8COM_State          = COM_STATE_WAIT_FOR_RX;
    /* Reset timeout control counter */
    u32COM_control_timer = 0u;
    /* Set default acknowledge state */ 
    u8Ack                = NOT_ACKNOWLEDGED;
}

/**************************************************************************************************/
/**
* \brief    vfnCOM_RxByte - Rx processing of 1 byte
* \author   Abraham Tezmol 
* \param    void
* \return   void
* \todo     
*/
void vfnCOM_RxByte(void )
{
    static UINT8 u8ActivationStatus;
    
    if( u8SCI_CheckRx( SERIAL_COMM_CHANNEL ) != 0u )
    {
        u8ActivationStatus = u8Scheduler_EventActivationHandler(&vfnCOM_TxByte, TASKS_EVENT_A, 1);
        vfnSCI_ClearRx( SERIAL_COMM_CHANNEL );
        
    }
}

/**************************************************************************************************/
/**
* \brief    vfnCOM_TxByte - Tx processing of 1 byte
* \author   Abraham Tezmol 
* \param    void
* \return   void
* \todo     
*/
void vfnCOM_TxByte(void)
{
    /* Acknowledge */
    vfnSCI_WriteBufferTx( SERIAL_COMM_CHANNEL, &au8COM_TX_message[0], 7);         /* Send Acknowledge */
}

/****************************************************************************************************/
/**
* \brief    
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo     
*/
    
void vfnCheckCommands(void)
{
    UINT8 u8Index;


    u32COM_control_timer++;

    switch(u8COM_State) 
    {
      /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        case COM_STATE_WAIT_FOR_RX:
            /* Waiting for reception of Format Byte */
                if( u8SCI_CheckRx( SERIAL_COMM_CHANNEL ) == 0u )
                {
                /* In case no data bytes have been received, check for timeout */
                    if( u32COM_control_timer >= COM_RX_TIMEOUT_MAX )
                    {
                    /* In case timeout elapses, discard any previous communication */
                        vfnCOM_Init( );
                        u8Ack = NOT_ACKNOWLEDGED;
                        /* Send acknowledge to serial interface */
                        vfnCOM_SendAck( );
                    }
                }
                else
                {
                /* Format Byte received, proceed to verify for valid Format byte */
                    u8RxFormatID = u8SCI_ReadRx( SERIAL_COMM_CHANNEL );
                    /* If invalid Format Byte                                           */
                    /* or invalid message size (no data bytes or too many data bytes)   */
                    if( u8RxFormatID  != VALID_HEADER )
                    {
                    /* Discard message */
                        vfnSCI_ClearRx( SERIAL_COMM_CHANNEL );
                    }
                    else
                    { /* Valid Format ID, process message data */ 
                    /* Start message reception */
                        u8COM_State          = COM_STATE_VALID_HEADER;
                    /* Begin calculation of Checksum */
                        u8COM_Checksum       = u8RxFormatID;
                    /* Reset byte counter */
                        u8COM_Byte_counter   = 0u;
                    }
                }
            /* end of state */
                break;
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/      
            case COM_STATE_VALID_HEADER:
            /* Waiting for reception of Valid ID Byte */
                if( u8SCI_CheckRx( SERIAL_COMM_CHANNEL ) != 0u )
                {
                /* Service ID received, proceed to verify for valid Format byte */
                    u8RxSid = u8SCI_ReadRx( SERIAL_COMM_CHANNEL );
                    /* If invalid Format Byte                                           */
                    /* or invalid message size (no data bytes or too many data bytes)   */
                    if ( 
                         (u8RxSid == kCOM_Pause) || (u8RxSid == kCOM_Set_Crank_Prof) || (u8RxSid == kCOM_Set_Cram_Prof) || 
                         (u8RxSid == kCOM_Set_RPM) ||(u8RxSid == kCOM_Set_Cam_Phase) || (u8RxSid == kCOM_Enable_Cam_Port) || 
                         (u8RxSid == kCOM_Set_Crank_Type_Direction)
                       )
                    { 
                    /* Valid Service ID, process message data */ 
                    /* Start message reception */
                        u8COM_State          = COM_STATE_VALID_SID;
                    /* Proceed with calculation of Checksum */
                        u8COM_Checksum       |= u8RxSid;
                    }
                    else
                    {
                        vfnSCI_ClearRx( SERIAL_COMM_CHANNEL );
                    } 
                }
            /* end of state */
                break;
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/      
            case COM_STATE_VALID_SID:
            /* Waiting for reception of Format Byte */
                if( u8SCI_CheckRx( SERIAL_COMM_CHANNEL ) != 0u )
                {
                /* Length byte received */
                    u8COM_RxLength = u8SCI_ReadRx( SERIAL_COMM_CHANNEL );
                    
                /* Valid Service ID, process message data */ 
                /* Start message reception */
                    u8COM_State          = COM_STATE_RECEIVING;
                /* Proceed with calculation of Checksum */
                    u8COM_Checksum       |= u8COM_RxLength;
                }
            /* end of state */
                break;
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
            case COM_STATE_RECEIVING:
            /* Proceed to message reception */
                u8RxByteCount = u8SCI_CheckRx( SERIAL_COMM_CHANNEL );

            /* Check for overflow condition */
                if( u8RxByteCount >= (kCOM_RX_MaxSize - 3) )
                {
                /* Discharge Message */
                    u8COM_State          = COM_STATE_WAIT_FOR_RX;
                    vfnSCI_ClearRx( SERIAL_COMM_CHANNEL );
                }
            /* NO overflow and data in Rx buffer */
                else  if( u8RxByteCount != 0u )
                {

                /* Process expected data bytes one by one */
                    for(u8Index = 0; u8Index<u8RxByteCount; u8Index++)
                    {
                    /* Read 1 data byte */
                        u8RxData = u8SCI_ReadRx( SERIAL_COMM_CHANNEL );
                    /* Check if byte is a messag character or a checksum byte */
                        if( u8COM_Byte_counter < u8COM_RxLength )
                        {
                        /* Byte is a message character, save received character */
                            au8COM_RX_message[ u8COM_Byte_counter ] = u8RxData;
                        /* Update checksum calculation */
                            u8COM_Checksum |= u8RxData;
                        /* Update number of expected data bytes */
                            ++u8COM_Byte_counter;
                        }
                        else
                        { /* Byte corresponds to checksum, verify checksum */ 
                            if( u8COM_Checksum != u8RxData )
                            { /* Checksum invalid, discharge message */ 
                                u8COM_State = COM_STATE_WAIT_FOR_RX;
                                vfnSCI_ClearRx( SERIAL_COMM_CHANNEL );
                                u8Ack = NOT_ACKNOWLEDGED;
                                /* Send acknowledge to serial interface */
                                vfnCOM_SendAck( );
                            }
                            else
                            { 
                            /* Clear Communication time out - Valid message has been received */
                                u32COM_control_timer = 0;
                            /* Checksum valid, proceed to call appropriate service */ 
                                vfnCOM_Analyse( );
                            /* Discard already processed message in order to flush RX buffer */
                                vfnSCI_ClearRx( SERIAL_COMM_CHANNEL );
                            /* Send acknowledge to serial interface */
                                vfnCOM_SendAck( );
                            }
                        /* Message processing complete. Exit while loop*/
                            u8RxByteCount = 0u;
                        }
                    }
                }
            /* NO overflow and NO data in Rx buffer */
                else
                {
                /* Check for reception inter message max timeout */
                    if( u32COM_control_timer >= COM_RX_INTER_MSG_MAX )
                    { /* Reception inter message max timeout expired, discharge message */ 
                        u8COM_State          = COM_STATE_WAIT_FOR_RX;
                    }
                /* Reception inter message max timeout has NOT expired, do nothing */
                    else
                    {
                    }
                }
            /* end of state */
                break;
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        default:      
            /* end of state */
                break;                 
    }

}
    
/**************************************************************************************************/
/**
* \brief    vfnCOM_SendAck - Send communication acknowledge to serial interface
* \author   Manuel Cortes / Abraham Tezmol
* \param    void
* \return   void
* \todo     
*/
void vfnCOM_SendAck(void) 
{           
    /* Acknowledge */
    au8COM_TX_message[3] = u8Ack;
    au8COM_TX_message[4] = u8RxSid;
    vfnSCI_WriteBufferTx( SERIAL_COMM_CHANNEL, au8COM_TX_message, COMMAND_SIZE );         /* Send Acknowledge */  
}

/**************************************************************************************************/
/**
* \brief    vfnCOM_SendRPM - Send current value of RPM to serial interface
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo     
*/
void vfnCOM_SendRPM(void) 
{           
    /* Acknowledge */
    /** Request access to shared data */
    XGATE_lock_hw_semaphore_1();
    
    au8COM_TX_RPM_message[3] = (UINT8)u16RPMValue;
    au8COM_TX_RPM_message[4] = (UINT8)(u16RPMValue>>8);
    au8COM_TX_RPM_message[5] = cam_crank_channels[0].u8Direction;
    
    /* Release access to shared data */
    XGATE_release_hw_semaphore_1();

    vfnSCI_WriteBufferTx( SERIAL_COMM_CHANNEL, au8COM_TX_RPM_message, 7 );         /* Send Acknowledge */  
}

/**************************************************************************************************/
/**
* \brief    vfnCOM_Analyse - Definition of available communication services
* \author   Manuel Cortes / Abraham Tezmol
* \param    void
* \return   void
* \todo     
*/
void vfnCOM_Analyse(void) 
{
        
    UINT8 u8Counter;
    UINT8 u8Temp;
    UINT8 u8Temp2;
    UINT16 u16Temp;
    
    u8Ack = NOT_ACKNOWLEDGED;    

    /* Invoke appropriate diagnostic service depending upon Diagnostic Service Identifier */
    switch( u8RxSid )
    {
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /* Pause CAM / CRANK signal generation Service */
        case kCOM_Pause:
            /* Get PIT command */
            if( au8COM_RX_message[0] == PIT_PAUSE)                                            
            {
                vfnPIT_Stop();
            }
            else
            {
                vfnPIT0_Start();
            }
            u8Ack = ACKNOWLEDGED;
            /* end of state */
            break;
        
        case kCOM_Set_Crank_Prof: 
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /* Set Crank Profile Service */       
            /** Request access to shared data */
            XGATE_lock_hw_semaphore_1();

            cam_crank_channels[0].u8ProfileSize = u8COM_RxLength;
            /* Set number of teeth */
            u8NumberOfTeeth = ( u8COM_RxLength>>1 );                                           
            
            for( u8Counter = 0; u8Counter < u8COM_RxLength; u8Counter++ )
            {
            /* Set Crankshaft configuration */
                u8CrankConfig[u8Counter] = au8COM_RX_message[u8Counter];      
            }
            /* Release access to shared data */
            XGATE_release_hw_semaphore_1();

            u8Ack = ACKNOWLEDGED;
            /* end of state */
            break;  
        
        case kCOM_Set_Cram_Prof:
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /* Set CAM profile Service */          
            /** Request access to shared data */
            XGATE_lock_hw_semaphore_1();

            for( u8Counter = 1; u8Counter < 4; u8Counter++)
            {
                cam_crank_channels[u8Counter].u8ProfileSize = u8COM_RxLength;
            }
            
            for( u8Counter = 0; u8Counter < u8COM_RxLength; u8Counter++ )
            {
            /* Set Crankshaft configuration */
                u8CamConfig[u8Counter] = au8COM_RX_message[u8Counter];      
            }

            /* Release access to shared data */
            XGATE_release_hw_semaphore_1();  

            u8Ack = ACKNOWLEDGED;
            /* end of state */                
            break;
        
        case kCOM_Set_RPM:
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /* Set RPM Service */             
            /* Get first nibble of RPM's */
            u8Temp = au8COM_RX_message[0];
            /* Get second nibble of RPM's */
            u16Temp = (UINT16)au8COM_RX_message[1];                
            /* Calculate RPMs */
            u16RPM_temp = (UINT16)((u16Temp<<8) | (UINT16)u8Temp); 
            /* Set RPM's value if within allowed range */

            /** Request access to shared data */
            XGATE_lock_hw_semaphore_1();

            if (u16RPMValue <= POSITIVE_RPM_MAX)
            {    
                u16RPMValue =   u16RPM_temp;
                u8Ack = ACKNOWLEDGED;                       
            }

            /* Release access to shared data */
            XGATE_release_hw_semaphore_1();

            /* end of state */
            break;
        
        case kCOM_Set_Crank_Type_Direction:
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /* Set CRANK Type and Direction Service */  
            /** Request access to shared data */
            XGATE_lock_hw_semaphore_1();

            /* Get u8Direction */
            cam_crank_channels[0].u8DirConfig = au8COM_RX_message[0];    
            /* Get u8DirConfig */                 
            cam_crank_channels[0].u8Direction = au8COM_RX_message[1];

            /* Release access to shared data */
            XGATE_release_hw_semaphore_1();            

            /* end of state */
            u8Ack = ACKNOWLEDGED;
            break;
        
        case kCOM_Set_Cam_Phase:
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /* Set CAM phase Service */
            /* Get Channel */
            u8Temp  = au8COM_RX_message[0];
            /* Get Phase */             
            u8Temp2 = au8COM_RX_message[1];
                
            /** Request access to shared data */
            XGATE_lock_hw_semaphore_1();

            u16Temp = (UINT16)( cam_crank_channels[0].u8ArrayIndex + u8Temp2 ); 
            if( u16Temp >= ( u8NumberOfTeeth << 1 ) )
            {
                u8Temp2 = (UINT8)( u16Temp - ( u8NumberOfTeeth << 1 ) );     
            }
            else
            {
                u8Temp2 = (UINT8)(u16Temp);
            }  
            /* Set Camshaft phase */
            cam_crank_channels[ u8Temp + 1 ].u8ArrayIndex = u8Temp2;

            /* Release access to shared data */
            XGATE_release_hw_semaphore_1();
            
            u8Ack = ACKNOWLEDGED;
            /* end of state */
            break;
        
        case kCOM_Enable_Cam_Port:   
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /* Enable CAM port Service */    
            /* Get Channel */
            u8Temp = au8COM_RX_message[0];                         
            /* Get status */
            u16Temp = (UINT16)(au8COM_RX_message[1]);            
            
            /** Request access to shared data */
            XGATE_lock_hw_semaphore_1();
            
            if( u16Temp == PORT_ENABLE)                                       
            {
                cam_crank_channels[ u8Temp + 1 ].u8BitMask = (1<<(u8Temp + 1));       /* Enable channel */                    
            }
            else
            {
                *(cam_crank_channels[ u8Temp + 1 ].u8Port) &= 
                ~(cam_crank_channels[ u8Temp + 1 ].u8BitMask);                        /* Set 0 */
                cam_crank_channels[ u8Temp + 1 ].u8BitMask = 0u;                      /* Disable channel */
            }
            
            /* Release access to shared data */
            XGATE_release_hw_semaphore_1();

            u8Ack = ACKNOWLEDGED;
            /* end of state */
            break;

        default:
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /* Default state - NO valid service */
            u8Ack = NOT_ACKNOWLEDGED;
            /* end of state */
            break;              
    }
}




    