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
#include "Scheduler.h"        /*!<Run*/
#include "I2C.h"

#define PERIOD_OF_TIMER_0_US  (1000)//Need to adjust SCHEDULER_SECONDS(x) if changed
#define TIME0_PRESCALE        (8)
#define TIME0_INTERRUPT_PERIOD (PERIOD_OF_TIMER_0_US*PLL_FREQUENCY_MHZ/TIME0_PRESCALE) //
#define MS_IN_FORGROUND_CLOCK0(x) ((((UINT32)x)*1000)/PERIOD_OF_TIMER_0_US)

#define PERIOD_OF_TIMER_1_US (1000)
#define TIME1_SET(x) (((UINT32)(x))*SYSTEM_CLOCK_FREQUENCY_MHZ) //
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
   TimerPrescaleSet(TIMER0,TIMER_A,TIME0_PRESCALE);
   TimerLoadSet(TIMER0,TIMER_A,TIME0_INTERRUPT_PERIOD-1);
   //Enable Timer
   TimerIntEnable(TIMER0, TIMER_TIMA_TIMEOUT);
   TimerEnable(TIMER0, TIMER_A);
   IntEnable(INT_TIMER0A);
   IntPrioritySet(INT_TIMER0A, TIMER_INTERRUPT_PRIORITY__FOREGROUND);
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
      SET_TESTPOINT_0();
      Scheduler__Run();
      I2C__MultReadStart();
      Sensors_Run();
      CLEAR_TESTPOINT_0();
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
      ADXL345__ProcessReadings();
      Gyro__ProcessReadings();

   }
}

