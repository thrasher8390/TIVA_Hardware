/*
 * PWM.h
 *
 *  Created on: Oct 20, 2015
 *      Author: DLThrasher
 */

#ifndef TIVA_HARDWARE_PWM_H_
#define TIVA_HARDWARE_PWM_H_


//============================================================
//Includes
//============================================================
#include "Project.h"

//============================================================
//Defines
//============================================================
#define PWM0   (PWM0_BASE)
#define PWM_CLOCK_DIVIDE (16) //PLEASE SEE SysCtlPWMClockSet

//============================================================
//Macros
//============================================================
//Converts us into PWM ticks
#define PWM_US(x) (x*SYSTEM_CLOCK_FREQUENCY_MHZ/PWM_CLOCK_DIVIDE)

//============================================================
//Global Functions
//============================================================
extern void PWM_Initialize(void);

#endif /* TIVA_HARDWARE_PWM_H_ */
