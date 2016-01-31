/*
 * PWM.c
 *
 *  Created on: Oct 20, 2015
 *      Author: DLThrasher
 */

//*****************************************************************************
//    Includes
//*****************************************************************************
#include "PWM.h"

//*****************************************************************************
//    Defines
//*****************************************************************************


//*****************************************************************************
//    Global Functions
//*****************************************************************************
void PWM_Initialize(void)
{
   //DisableTimer
   SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
   PWMGenDisable(PWM0, PWM_GEN_1);
   PWMGenConfigure(PWM0, PWM_GEN_1, PWM_GEN_MODE_DOWN);
   PWMGenPeriodSet(PWM0, PWM_GEN_1, (20*16)-1);
   PWMPulseWidthSet(PWM0, PWM_OUT_2,4*16);
   PWMOutputState(PWM0,PWM_OUT_2_BIT|PWM_OUT_3_BIT ,true);
   PWMGenEnable(PWM0, PWM_GEN_1);
}
