/*
 * UART.c
 *
 *  Created on: Sep 25, 2014
 *      Author: DLThrasher
 */


//*****************************************************************************
//		Includes
//*****************************************************************************
#include "UART.h"

//*****************************************************************************
//		Global Functions
//*****************************************************************************
//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void UART_Initialize(void)
{
   //
   // Enable the GPIO Peripheral used by the UART.
   //
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

   //
   // Enable UART0
   //
   SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

   //
   // Configure GPIO Pins for UART mode.
   //
   GPIOPinConfigure(1);
   GPIOPinConfigure(0x401);
   GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

   //
   // Use the internal 16MHz oscillator as the UART clock source.
   //
   UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

   //
   // Initialize the UART for console I/O.
   //
   UARTStdioConfig(0, 115200, 16000000);
}
