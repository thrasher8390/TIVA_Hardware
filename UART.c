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
#include "LED.h"
#include "HelperFunctions.h"

//*****************************************************************************
//    Static Local Functions
//*****************************************************************************
static void initUART0();
static void initUART3();
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
   initUART0();
   initUART3();

}

static void initUART0()
{
   //
   // Enable UART0
   //
   SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

   //
   // Use the internal 16MHz oscillator as the UART clock source.
   //
   UARTClockSourceSet(UART0_BASE, UART_CLOCK_SYSTEM);

   //
   // Initialize the UART for console I/O.
   //
   UARTStdioConfig(0, 115200, SYSTEM_CLOCK_FREQUENCY_HZ);
}

static void initUART3()
{
   //
   // Enable UART3
   //
   SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);

   //
   // Use the internal 16MHz oscillator as the UART clock source.
   //
   UARTClockSourceSet(UART3_BASE, UART_CLOCK_SYSTEM);
   //
   // Enable the GPIO Peripheral used by the UART.
   //
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

   //
   // Configure GPIO Pins for UART mode.
   //
   GPIOPinConfigure(GPIO_PC6_U3RX);
   GPIOPinConfigure(GPIO_PC7_U3TX);
   GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

   //
   // Use the internal 16MHz oscillator as the UART clock source.
   //
   UARTConfigSetExpClk(UART3_BASE, SYSTEM_CLOCK_FREQUENCY_HZ, 9600,
         (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

   //
   // We are configured for buffered output so enable the master interrupt
   // for this UART and the receive interrupts.  We don't actually enable the
   // transmit interrupt in the UART itself until some data has been placed
   // in the transmit buffer.
   //
   UARTIntDisable(UART3_BASE, 0xFFFFFFFF);
   UARTIntEnable(UART3_BASE, UART_INT_RT);
   UARTEnable(UART3_BASE);
   IntEnable(INT_UART3);
}

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void UART_Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
   //
   // Loop while there are more characters to send.
   //
   while (ui32Count--) {
      //
      // Write the next character to the UART.
      //
      UARTCharPut(UART3_BASE, *pui8Buffer++);
   }
}

