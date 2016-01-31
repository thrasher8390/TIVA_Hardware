/*
 * GPIO.c
 *
 *  Created on: Sep 25, 2014
 *      Author: DLThrasher
 */

//*****************************************************************************
//		Includes
//*****************************************************************************
#include "GPIO.h"
#include "LED.h"


//*****************************************************************************
//    Local static Functions
//*****************************************************************************
static void portBInitialize(void);

//*****************************************************************************
//		Global Functions
//*****************************************************************************
/*!
 * \brief Init GPIO
 */
void GPIO_Initialize(void)
{
   portBInitialize();
	//PORT D
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
   //D-0 = UNUSED
   //D-1 = UNUSED
   //D-2 = UNUSED
   //D-3 = UNUSED
   //D-4 = UNUSED
   //D-5 = UNUSED
   //D-6 = TESTPOINT 0
	GPIOPinTypeGPIOOutput(TESTPOINT_0_PORT, TESTPOINT_0_PIN);
	GPIOPinWrite(TESTPOINT_0_PORT, 0xFF, CLEAR);

	//PORT F
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
   //F-0 = UNUSED
   //F-1 = RED LED
   GPIOPinTypeGPIOOutput(LED_PORT, LED_RED_PIN);
   //F-2 = BLUE LED
   GPIOPinTypeGPIOOutput(LED_PORT, LED_BLUE_PIN);
   //F-3 = GREEN LED
   GPIOPinTypeGPIOOutput(LED_PORT, LED_GREEN_PIN);
   //F-4 = UNUSED
   //F-5 = UNUSED
   //F-6 = UNUSED
}

//*****************************************************************************
//    Local static Functions
//*****************************************************************************

static void portBInitialize(void)
{
   //PORT B
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
   //B-0 = UNUSED
   //B-1 = UNUSED
   //B-2 = UNUSED
   //B-3 = UNUSED
   //B-4 = PWM for Motor
   GPIOPinConfigure(GPIO_PB4_M0PWM2);
   GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);
   //B-5 = PWM for Motor
   GPIOPinConfigure(GPIO_PB5_M0PWM3);
   GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);
   //B-6 = PWM for Motor
   GPIOPinConfigure(GPIO_PB6_M0PWM0);
   GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
   //B-7 = PWM for Motor
   GPIOPinConfigure(GPIO_PB7_M0PWM1);
   GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);
   //B-8 = UNUSED
}
