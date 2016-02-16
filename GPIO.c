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
#include "Sensors.h"


//*****************************************************************************
//    Local static Functions
//*****************************************************************************
static void portAInitialize(void);
static void portBInitialize(void);
static void portDInitialize(void);

//*****************************************************************************
//		Global Functions
//*****************************************************************************
/*!
 * \brief Init GPIO
 */
void GPIO_Initialize(void)
{
   portAInitialize();
   portBInitialize();
   portDInitialize();

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


static void portAInitialize(void)
{
   //PORT A
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
   //A-0 = UART for PRINTF
   GPIOPinConfigure(GPIO_PA0_U0RX);

   GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0);
   //A-1 = UART FOR PRINTF
   GPIOPinConfigure(GPIO_PA1_U0TX);
   GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_1);
   //A-2 = TESTPOINT 1
   GPIOPinTypeGPIOOutput(TESTPOINT_1_PORT, TESTPOINT_1_PIN);
   GPIOPinWrite(TESTPOINT_1_PORT, TESTPOINT_1_PIN, CLEAR);
   //A-3 = UNUSED
   //A-4 = UNUSED
   //A-5 = UNUSED
   //A-6 = UNUSED
   //A-7 = UNUSED
}

static void portBInitialize(void)
{
   //PORT B
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
   //B-0 = UNUSED
   //B-1 = UNUSED
   //B-2 = I2C Clock
   GPIOPinConfigure(GPIO_PB2_I2C0SCL);
   GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
   //B-3 = I2C Data
   GPIOPinConfigure(GPIO_PB3_I2C0SDA);
   GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
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

static void portDInitialize(void)
{
   //PORT D
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
   //D-0 = UNUSED
   //D-1 = UNUSED
   //D-2 = UNUSED
   //D-3 = UNUSED
   //D-4 = UNUSED
   //D-5 = UNUSED
   //D-6 = ADXL345 INTERRUPT
   GPIOPinTypeGPIOInput(SENSORS_INT_PORT,SENSORS_INT_PIN); /*!< Input*/
   GPIOPadConfigSet(SENSORS_INT_PORT, SENSORS_INT_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD); /*!< Pull Down*/
   GPIOIntTypeSet(SENSORS_INT_PORT, SENSORS_INT_PIN, GPIO_RISING_EDGE); /*!< Rising Edge Interrupt*/
   GPIOIntClear(SENSORS_INT_PORT,SENSORS_INT_PIN);
   //D-7 = TESTPOINT 0
   GPIOPinTypeGPIOOutput(TESTPOINT_0_PORT, TESTPOINT_0_PIN);
   GPIOPinWrite(TESTPOINT_0_PORT, TESTPOINT_0_PIN, CLEAR);


   //All Port D
   GPIOIntEnable(GPIO_PORTD_BASE, SENSORS_INT_PIN);

   IntEnable(INT_GPIOD);
   IntPrioritySet(INT_GPIOD, GPIO_INTERRUPT_PRIORITY__SENSOR_PIN);
}

//*****************************************************************************
//    Interrupts
//*****************************************************************************

void IntGPIOd(void)
{
   UINT32 pinStatus = GPIOIntStatus(SENSORS_INT_PORT, 0xFF);/*!< Get all interrupts*/
   if(pinStatus & SENSORS_INT_PIN)
   {
      GPIOIntClear(SENSORS_INT_PORT,SENSORS_INT_PIN);
      Sensors__InterruptIRQ();
   }
}
