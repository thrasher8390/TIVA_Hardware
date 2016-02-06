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

#define PERIOD_OF_TIMER_0_US (1600)
#define TIME0_INTERRUPT_PERIOD (PERIOD_OF_TIMER_0_US*SYSTEM_CLOCK_FREQUENCY_MHZ) //250us * CLOCK OF TIMER 0(16mhz) = 4000
#define MS_IN_FORGROUND_CLOCK0(x) ((((UINT32)x)*1000)/PERIOD_OF_TIMER_0_US)

#define PERIOD_OF_TIMER_1_US (20000)
#define TIME1_SET(x) (((UINT32)(x))*SYSTEM_CLOCK_FREQUENCY_MHZ) //250us * CLOCK OF TIMER 0(16mhz) = 4000
#define MS_IN_FORGROUND_CLOCK1(x) ((((UINT32)x)*1000)/PERIOD_OF_TIMER_1_US)


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


	//Set up timer to operate in PWM Mode
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

   TimerDisable(TIMER1, TIMER_A);
   //Periodic / TAMIE
   TimerConfigure(TIMER1, TIMER_CFG_A_PERIODIC_UP);
   TimerLoadSet(TIMER1,TIMER_A,TIME1_SET(PERIOD_OF_TIMER_1_US)-1);

   //Enable Timer
   TimerIntEnable(TIMER1, TIMER_TIMA_TIMEOUT);
   TimerEnable(TIMER1, TIMER_A);
   IntEnable(INT_TIMER1A);
   TimerIntClear(TIMER1,0xFFFF);

}

//Foreground Timer!!!!
void Timers_Timer0Interrupt(void)
{
   UINT32 status = TimerIntStatus(TIMER0,TRUE);
   if(status & TIMER_TIMA_TIMEOUT)
   {
      TimerIntClear(TIMER0, TIMER_TIMA_TIMEOUT);
   }
}

//This must have highest priority!!!
//Timer that is going to be used for Motor controls
void Timers_Timer1Interrupt(void)
{
   UINT32 status = TimerIntStatus(TIMER1,TRUE);
   if(status & TIMER_TIMA_TIMEOUT)
   {
      TimerIntClear(TIMER1, TIMER_TIMA_TIMEOUT);
   }
}

