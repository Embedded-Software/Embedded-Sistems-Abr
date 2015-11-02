/****************************************************************************************************/
/**
  \mainpage
  \n 
  \brief        Main application (main module)
  \author       Abraham Tezmol Otero, M.S.E.E
  \project      Omicron 
  \version      1.0
  \date         26/Jun/2009
   
  Program compiled with CW for HCS12(X) v5.0, tested on DEMO9S12XEP100 board
*/
/****************************************************************************************************/

/** Main application common definitions */
#include    "main.h"
#include    "GPIO.h"


/*****************************************************************************************************
* Definition of module wide VARIABLEs 
*****************************************************************************************************/


/****************************************************************************************************
* Declaration of module wide FUNCTIONs 
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of global wide MACROs / #DEFINE-CONSTANTs
*****************************************************************************************************/

/*****************************************************************************************************
* Declaration of module wide TYPEs 
*****************************************************************************************************/

/******************************************************************************************************
* Definition of module wide VARIABLEs 
******************************************************************************************************/

/******************************************************************************************************
* Code of module wide FUNCTIONS
******************************************************************************************************/

extern Pwm_ConfigChannel ConfigPtr0;
void interrupt Pwm_Init_Thread(Pwm_ConfigChannel* ConfigPtr) ;

void vfnBackgroundSubsystemTasks(void);

/*~~~~~~~ Main Code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void main(void) 
{       
    int i=0;
    int j=0;    

    /* Disable interrupts */
    INTERRUPT_DISABLE(); 
    /* XGATE initialization */
    vfn_XGATE_CPU_init();
    /* Initialize Interrupt Module */
    vfnInterrupt_Init();          
    /* PLL initialization to the desired target frequency */
    vfnPLL_Init();
    /* Dynamic Memory Allocation initialization */
    vfnMemAlloc_NearInit(&NearMemAlloc_config[0]);
    vfnMemAlloc_Init(&DynamicMemAlloc_config[0]);
    /* Microcontroller Input/Output pins initialization */
    vfnInputs_Outputs_Init();
    /* Initialize Serial Communications */
    /* vfnCommProtocol_Init(); */
    /* Initialize Task Scheduler */ 
    vfnScheduler_Init();
    /* Start execution of task scheduler */
    vfnScheduler_Start();
    /* Arm COP watchdog */
    /* vfnCOPWatchdog_Init();*/
    /* Once all initializations are complete, enable interrupts */
    INTERRUPT_ENABLE();     
    /* Initialize CAM/Crank emulation */
     vfnCamCrank_Init();
    /* Start CAM/Crank emulation */
    /* vfnCamCrank_Start();*/
    
    /* Initialize CAM/Crank emulation */
     vfnPwm_Init();
    /* Start CAM/Crank emulation */
     vfnPwm_Start();
    
    //Initialize Serial Communication Driver
    vfnSCI_Init(&SCI_config[0]);
    vfnSCI_WriteBufferTx(SCI_CH0, cadena, 44);
    
    
    
    /*-- Loop through all the periodic tasks from Task Scheduler --*/
    for(;;)
    {        
        
        /* Perform all scheduled tasks */
        vfnTask_Scheduler();
        /* Perform all background susbystem control tasks */
        vfnBackgroundSubsystemTasks();
                   
    }
}
/*~~~~~~~ End of Main Code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* Background tasks related to Subsystem control */
void vfnBackgroundSubsystemTasks(void) 
{;}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*

typedef struct pwm_configType 
{
  Pwm_ChannelType xChannelType; 
  UINT8           u8IdleState;
  Pwm_PeriodType  xPeriodType; 
  UINT16          u16DutyCycle; 
}Pwm_ConfigChannel;
  */
  
void interrupt PWM_Handler()
{
 // unsigned char temp;
  //Clear XGATE interrupt flag - SCI0 is channel $6B
  XGIF1 = 0x0800;

  //Initialise buffer with new values
  /*ConfigPtr0.xChannelType = 4;
  temp = ConfigPtr0.character[0];
  ConfigPtr0.u8IdleState[0] = ConfigPtr0.character[1];
  ConfigPtr0.xPeriodType[1] = ConfigPtr0.character[2];
  ConfigPtr0.u16DutyCycle[2] = temp;  */
  
  /* Enable the PWM/PIT interrupt to start the start Initialization*/
 // SCI0CR2_SCTIE = 1;
}