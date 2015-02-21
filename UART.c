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
   GPIOPinConfigure(GPIO_PA0_U0RX);
   GPIOPinConfigure(GPIO_PA1_U0TX);
   GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

   //
   // Use the internal 16MHz oscillator as the UART clock source.
   //
   UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

   //
   // Initialize the UART for console I/O.
   //
   UARTStdioConfig(0, 115200, SYSTEM_CLOCK_FREQUENCY);
}

static void initUART3()
{
   //
   // Enable UART3
   //
   SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
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
   UARTConfigSetExpClk(UART3_BASE, SYSTEM_CLOCK_FREQUENCY, 38400,
         (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

   //
   // Enable the UART interrupt.
   //UARTFIFOLevelSet(UART3_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);

   //
   // Remember which interrupt we are dealing with.
   //

   //
   // We are configured for buffered output so enable the master interrupt
   // for this UART and the receive interrupts.  We don't actually enable the
   // transmit interrupt in the UART itself until some data has been placed
   // in the transmit buffer.
   //
   UARTIntDisable(UART3_BASE, 0xFFFFFFFF);
   UARTIntEnable(UART3_BASE, UART_INT_RX | UART_INT_RT);
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

int32_t receive;

/*!
 * \brief Interrupt Handler for UART3
 */
void UART3IntHandler(void)
{
   uint32_t ui32Status;

   //
   // Get the interrrupt status.
   //
   ui32Status = UARTIntStatus(UART3_BASE, 0xff);

   //
   // Clear the asserted interrupts.
   //
   UARTIntClear(UART3_BASE, ui32Status);

   //
   // Loop while there are characters in the receive FIFO.
   //
   while (UARTCharsAvail(UART3_BASE)) {
      //
      // Read the next character from the UART and write it back to the UART.
      //

      receive = UARTCharGetNonBlocking(UART3_BASE);

      //
      // Blink the LED to show a character transfer is occuring.
      //
      LED_ChangeColor(LED_RED_BLUE);

      //
      // Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.
      //
      DelayMS(1000);

      //
      // Turn off the LED
      //
      LED_ChangeColor(LED_RED_GREEN);

   }
}
