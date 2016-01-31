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
//Initial period should be 0
#define PWM_PERIOD_INITIAL (PWM_US(20000))//DT this sould be looked at.
#define PWM_WIDTH_INITIAL  (PWM_US(1))
//*****************************************************************************
//    Local Static Defines
//*****************************************************************************
static void pwm0Init(void);
static void pwmGen0Init(void);
static void pwmGen1Init(void);
//*****************************************************************************
//    Global Functions
//*****************************************************************************

/*
 * \brief Initialization for PWM module which will output to 4 individual servos
 * \details PWM0 Module has 8 possible outputs enabled for 4 genrators. We will use Gen_0 adn Gen_1 to enable outputs 0,1,2,3
 */
void PWM_Initialize(void)
{
   //Enable the system clock
   SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
   //Since our system clock is currently at 16MHz our PWM has 1 us resolution
   SysCtlPWMClockSet(SYSCTL_PWMDIV_16);

   //Init all motor outputs
   pwm0Init();
}

//*****************************************************************************
//    Local Functions
//*****************************************************************************

static void pwm0Init(void)
{
   //Disable all generation
   PWMGenDisable(PWM0, PWM_GEN_0);
   PWMGenDisable(PWM0, PWM_GEN_1);
   PWMGenDisable(PWM0, PWM_GEN_2);
   PWMGenDisable(PWM0, PWM_GEN_3);

   //Configure OUT 0 & 1
   pwmGen0Init();
   //Configure OUT 2 & 3
   pwmGen1Init();

   //Enable Gen 0 & 1
   PWMGenEnable(PWM0, PWM_GEN_0);
   PWMGenEnable(PWM0, PWM_GEN_1);
}
static void pwmGen0Init(void)
{
   PWMGenConfigure(PWM0, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_DBG_RUN | PWM_GEN_MODE_NO_SYNC);
   PWMGenPeriodSet(PWM0, PWM_GEN_0, PWM_PERIOD_INITIAL);
   //OUT 0 & 1 pulse width init
   PWMPulseWidthSet(PWM0, PWM_OUT_0 , PWM_WIDTH_INITIAL);
   PWMPulseWidthSet(PWM0, PWM_OUT_1 , PWM_WIDTH_INITIAL);

   //Enable output for 0 & 1
   PWMOutputState(PWM0,PWM_OUT_0_BIT ,true);
   PWMOutputState(PWM0,PWM_OUT_1_BIT ,true);
}

static void pwmGen1Init(void)
{
   PWMGenConfigure(PWM0, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_DBG_RUN | PWM_GEN_MODE_NO_SYNC);
   PWMGenPeriodSet(PWM0, PWM_GEN_1, PWM_PERIOD_INITIAL);
   //OUT 2 & 3 pulse width init
   PWMPulseWidthSet(PWM0, PWM_OUT_2 , PWM_WIDTH_INITIAL);
   PWMPulseWidthSet(PWM0, PWM_OUT_3 , PWM_WIDTH_INITIAL);

   //Enable output for 2 & 3
   PWMOutputState(PWM0,PWM_OUT_2_BIT ,true);
   PWMOutputState(PWM0,PWM_OUT_3_BIT ,true);
}
