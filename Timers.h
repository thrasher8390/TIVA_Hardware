/*
 * Timers.h
 *
 *  Created on: Sep 25, 2014
 *      Author: DLThrasher
 */

#ifndef TIMERS_H_
#define TIMERS_H_

//============================================================
//Includes
//============================================================
#include "Project.h"

//============================================================
//Defines
//============================================================
/*!
 *  \brief Init timer module
 */
#define TIMER0 				(TIMER0_BASE)
#define TIMER0_PRESCALER 	(SYSTEM_CLOCK_FREQUENCY_MHZ) //each count is 1 us
#define Timer0_CLOCK_MHZ   (SYSTEM_CLOCK_FREQUENCY_MHZ/TIMER0_PRESCALER)

#define TIMER0_COUNT_US()	(TimerValueGet(TIMER0, TIMER_A))
//============================================================
//Global Functions
//============================================================
extern void Timers_Initialize(void);			//Timers.c
#endif /* TIMERS_H_ */
