/****************************************************************************************************/
/**
\file       rpm_control.c
\brief      RMP control functions.
\author     Abraham Tezmol
\version    1.0
\date       06/01/2012
*/
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/

/** Own headers */
#include    "rpm_control.h"

/** Used modules */

/*****************************************************************************************************
* Declaration of module wide FUNCTIONs 
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs 
*****************************************************************************************************/

/*****************************************************************************************************
* Declaration of module wide TYPEs 
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide VARIABLEs 
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/

/*****************************************************************************************************
* Code of module wide FUNCTIONS
*****************************************************************************************************/

/**************************************************************************************************/
/**
* \brief    vfnGetRPMButtons - Main RPM control function using push buttons.
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo     
*/
void vfnGetRPMButtons(void)
{
    /** Request to write to Shared data */
    XGATE_lock_hw_semaphore_1();
    
    /** RPM control for BIDIRECTIONAL configuration */
    if (cam_crank_channels[0].u8DirConfig == BIDIRECTIONAL)
    {
        switch(cam_crank_channels[0].u8Direction )
        {
            case FORWARD:        
                if (PP0_push_button == PUSH_BUTTON_ACTIVE)
                {
                    if (u16RPMValue <= (POSITIVE_RPM_MAX - RPM_PushButton_Delta))
                    {
                        u16RPMValue = u16RPMValue + RPM_PushButton_Delta;
                    }
                }
                if (PP1_push_button == PUSH_BUTTON_ACTIVE)
                {
                    if (u16RPMValue >= RPM_PushButton_Delta )
                    {
                        u16RPMValue -= RPM_PushButton_Delta;
                        if (u16RPMValue == 0u)
                        {
                            cam_crank_channels[0].u8Direction = REVERSE;
                        }
                    }
                    else 
                    {
                        /* Take care of the 0 RPM condition */
                        if (u16RPMValue == 0u) 
                        {
                            cam_crank_channels[0].u8Direction = REVERSE;
                            u16RPMValue += RPM_PushButton_Delta;
                        }
                    }
                }
                break;
                
            case REVERSE:        
                if (PP0_push_button == PUSH_BUTTON_ACTIVE)
                {
                    if (u16RPMValue >= RPM_PushButton_Delta )
                    {
                        u16RPMValue -= RPM_PushButton_Delta;
                        if (u16RPMValue == 0u)
                        {
                            cam_crank_channels[0].u8Direction = FORWARD;
                        }
                    } 
                    else 
                    {
                        /* Take care of the 0 RPM condition */
                        if (u16RPMValue == 0u) 
                        {
                            cam_crank_channels[0].u8Direction = FORWARD;
                            u16RPMValue += RPM_PushButton_Delta;
                        }
                    }
                }
                if (PP1_push_button == PUSH_BUTTON_ACTIVE)
                {
                    if (u16RPMValue <= (NEGATIVE_RPM_MIN - RPM_PushButton_Delta))
                    {
                        u16RPMValue = u16RPMValue + RPM_PushButton_Delta;
                    }
                }
                break; 
                
            default:
                break;       
        }
    }
    /** RPM control for UNIDIRECTIONAL configuration */
    else
    {
        if (PP0_push_button == PUSH_BUTTON_ACTIVE)
        {
            if (u16RPMValue <= (POSITIVE_RPM_MAX - RPM_PushButton_Delta))
            {
                u16RPMValue = u16RPMValue + RPM_PushButton_Delta;
            }   
        }
        if (PP1_push_button == PUSH_BUTTON_ACTIVE)
        {        
            if (u16RPMValue > RPM_PushButton_Delta )
            {
            u16RPMValue -= RPM_PushButton_Delta;
            }
        }
    }
    /* Release access to shared data */
    XGATE_release_hw_semaphore_1();
}  

/**************************************************************************************************/
/**
* \brief    vfnSetPITCounts - Set PIT timeout counts based on current value of RPM.
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo     
*/
void vfnSetPITCounts(void)
{
    static UINT32 u32Temp;
    /** Request to write to Shared data */
    XGATE_lock_hw_semaphore_1();
    
    u32Temp = (UINT32)( (UINT32)(u16RPMValue) * (UINT32)(u8NumberOfTeeth) );
    if(u32Temp != 0u)
    { 
      u16PITTimeOut = (UINT16)((UINT32)(NUM_PIT_EVENTS) / u32Temp);
    }
    else
    {
      u16PITTimeOut = 0xFFFF;
    }
    
    /* Clear semaphore 1 by writing mask=1 and flag=0 */
    XGATE_release_hw_semaphore_1();
}