/*
 * HC05.c
 *
 *  Created on: Feb 2, 2015
 *      Author: DLThrasher
 */





#include "HC05.h"
#include "UART.h"
#include "LED.h"
#include "HelperFunctions.h"


//============================================================
//Local Functions
//============================================================
static void addCommandToFifo(BLUETOOTH_CMD);
//============================================================
//ENUMS
//============================================================
//============================================================
//defines
//============================================================


//============================================================
//Local Variables
//============================================================
//local array accessed globally through GET
#define COMMAND_FIFO_SIZE (32)
#define COMMAND_FIFO_BIT_MASK (0x1F)
#define COMMAND_FIFO_START_INDEX (0)

static BLUETOOTH_CMD CommandFIFO[COMMAND_FIFO_SIZE];
static uint8_t CommandFIFOReadIndex = 0;
static uint8_t CommandFIFOWriteIndex = 0;



void HC05_Initialize()
{
   UART_Initialize();
}

/*
*\brief Adding command to FIFO
*/
void addCommandToFifo(BLUETOOTH_CMD addedCMD)
{
   //Don't add command if you are going to overwrite the next command
   if(((CommandFIFOWriteIndex+1) & COMMAND_FIFO_BIT_MASK) != CommandFIFOReadIndex)
   {
   CommandFIFO[CommandFIFOWriteIndex++] = addedCMD;
   }
   else
   {
    //ERROR Our Command Fifo is about to overwrite our next command
   }
   //wrap the index back to COMMAND_FIFO_START_INDEX
   CommandFIFOWriteIndex &= COMMAND_FIFO_BIT_MASK;
}

/*
*\brief Returns a command if there is one that needs to be proccessed
*/
BLUETOOTH_CMD HC05__GetCommand()
{
   BLUETOOTH_CMD CMD = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
   if(CommandFIFOReadIndex != CommandFIFOWriteIndex)
   {
    CMD = CommandFIFO[CommandFIFOReadIndex++];
    LED_ChangeColor(LED_GREEN);
   }
   else
   {
    //We have processed all of our commands! YAY we are fast!!!
    /* Toggle LED3 */
    //LED_ChangeColor(LED_RED);
   }
   //Wrap the index back to COMMAND_FIFO_START_INDEX
   CommandFIFOReadIndex &= COMMAND_FIFO_BIT_MASK;
   return CMD;
}


int32_t readData;

/*!
 * \brief Interrupt Handler for UART3
 */
void HC05__RxInterrupt(void)
{
   uint32_t interruptStatus = UARTIntStatus(UART3_BASE, 1);
   uint8_t RxIndex = 0;
   BLUETOOTH_CMD RxCommand;

   //Decipher message once no more bytes are coming in
   if(interruptStatus & UART_INT_RT)
   {
      // Clear the asserted interrupts.
      UARTIntClear(UART3_BASE, UART_INT_RT);

      // Loop while there are characters in the receive FIFO.
      while (UARTCharsAvail(UART3_BASE))
      {
         // Read the next character from the UART and write it back to the UART.
         readData = UARTCharGetNonBlocking(UART3_BASE);

         switch(RxIndex)
         {
            case CMD:
            {
               RxCommand.CMD = readData;
               break;
            }
            case END_OF_HEADER_INDEX:
            {
               RxCommand.LENGTH = readData;
               break;
            }
            case BEGINNING_OF_DATA_INDEX:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            {
               RxCommand.DATA[RxIndex - BEGINNING_OF_DATA_INDEX] = readData;
               break;
            }
            default:
            {
               RxIndex = 0;
               break;
            }
         }

         if((RxIndex >= END_OF_HEADER_INDEX) &&
            (RxIndex >= (END_OF_HEADER_INDEX + RxCommand.LENGTH))
            )
         {
            RxIndex = 0;
            addCommandToFifo(RxCommand);
         }
         else
         {
            RxIndex++;
         }
      }
   }
}

