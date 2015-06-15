/*
 * Timers.c
 *
 *  Created on: Sep 25, 2014
 *      Author: DLThrasher
 */

//*****************************************************************************
//		Includes
//*****************************************************************************
#include "Timers.h"
#include "Sensors.h"
#include "LED.h"

#define PERIOD_OF_TIMER_0_US (800)
#define TIME0_INTERRUPT_PERIOD (PERIOD_OF_TIMER_0_US*SYSTEM_CLOCK_FREQUENCY_MHZ) //250us * CLOCK OF TIMER 0(16mhz) = 4000
#define MS_IN_FORGROUND_CLOCK(x) ((((UINT32)x)*1000)/PERIOD_OF_TIMER_0_US)


//*****************************************************************************
//		Global Functions
//*****************************************************************************
void Timers_Initialize(void)
{
	//DisableTimer
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerDisable(TIMER0, TIMER_A);

	//Periodic / TAMIE
	TimerConfigure(TIMER0, TIMER_CFG_A_PERIODIC_UP);
   TimerLoadSet(TIMER0,TIMER_A,TIME0_INTERRUPT_PERIOD-1);
	//Enable Timer
   TimerIntEnable(TIMER0, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER0, TIMER_A);
	IntEnable(INT_TIMER0A);

	TimerIntClear(TIMER0,0xFFFF);
}
UINT32 BOOL = FALSE;
void Timers_Timer0Interrupt(void)
{
   UINT32 status = TimerIntStatus(TIMER0,TRUE);
   if(status & TIMER_TIMA_TIMEOUT)
   {
      SET_TESTPOINT_0();
      TimerIntClear(TIMER0, TIMER_TIMA_TIMEOUT);
      Sensors_Run();
      CLEAR_TESTPOINT_0();
   }
}
