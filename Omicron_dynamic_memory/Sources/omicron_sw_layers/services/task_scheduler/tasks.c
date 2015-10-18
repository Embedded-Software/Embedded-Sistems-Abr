/****************************************************************************************************/
/**
\file       tasks.c
\brief      Multi-thread Task scheduler - list of tasks.
\author     Abraham Tezmol
\version    1.0
\date       07/09/2013
*/
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/

/** Scheduler function prototypes definitions */
#include    "tasks.h"

/*****************************************************************************************************
* Definition of  VARIABLEs - 
*****************************************************************************************************/


/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/

/*****************************************************************************************************
* Code of module wide FUNCTIONS
*****************************************************************************************************/

/* List of tasks to be executed @ 1ms */
void TASKS_LIST_1MS( void )
{
    /* vfnCheckCommands(); */
    vfnCOM_RxByte();
    vfnSchedulepoint();
}

/* List of tasks to be executed @ 2ms, first group */
void TASKS_LIST_2MS_A(void)
{
    vfnCOPWatchdog_Reset();
    vfnSchedulepoint();
}
/* List of tasks to be executed @ 2ms, second group */
void TASKS_LIST_2MS_B( void )
{   
    vfnSchedulepoint();
}
    
/* List of tasks to be executed @ 10ms */
void TASKS_LIST_10MS( )
{
    vfnGetRPMButtons();
    vfnSchedulepoint();
    vfnSetPITCounts();
}
/* List of tasks to be executed @ 50ms */
void TASKS_LIST_50MS( void )
{
    vfnSchedulepoint();
}
/* List of tasks to be executed @ 100ms */
void TASKS_LIST_100MS( void )
{ 
    /* vfnCOM_SendRPM();*/
    vfnSchedulepoint();
}


