/*
 * I2C.c
 *
 *  Created on: Nov 17, 2014
 *      Author: DLThrasher
 */

//*****************************************************************************
//		Includes
//*****************************************************************************
#include "I2C.h"
#include "Scheduler.h"

//*****************************************************************************
//    Defines
//*****************************************************************************
#define MULT_READ_QUEUE_SIZE (8)
//*****************************************************************************
//    Data
//*****************************************************************************
volatile I2CMultReadStruct I2CMultRead = {.MultiReadActive = FALSE};
static I2CMultReadStruct MultReadQueue[MULT_READ_QUEUE_SIZE];
volatile UINT8 QueueWriteIndex = 0;
volatile UINT8 QueueReadIndex = 0;

//*****************************************************************************
//    LocalFunctionPrototypes
//*****************************************************************************
static void multReadStart(void);
//*****************************************************************************
//		Global Functions
//*****************************************************************************

/*!
 * \brief Init I2C
 */
void I2C_Init0(void)
{
   SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

   //reset I2C module
   SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

   //Enable interrupts to be used by Multi Read
   I2CMasterIntEnable(I2C0_BASE);

   //enable interrupt so that we can finish the mult read
   I2CMasterIntEnableEx(I2C0_BASE,I2C_INT_MASK);
   IntEnable(INT_I2C0);
   IntPrioritySet(INT_I2C0, I2C_INTERRUPT_PRIORITY);

   // Enable and initialize the I2C0 master module.  Use the system clock for
   // the I2C0 module.  The last parameter sets the I2C data transfer rate.
   // If false the data rate is set to 100kbps and if true the data rate will
   // be set to 400kbps.
   I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), TRUE);
}

BOOLEAN I2C_WriteVerify0(uint16_t device_address, uint16_t device_register, uint8_t device_data)
{
	BOOLEAN writeSuccessful = FALSE;
	I2C_Write0(device_address,device_register,device_data);
	uint32_t read = I2C_Read0(device_address,device_register);
	if(read == device_data)
	{
		writeSuccessful = TRUE;
	}
	return writeSuccessful;
}

void I2C_Write0(uint16_t device_address, uint16_t device_register, uint8_t device_data)
{
   //send control byte and register address byte to slave device
   I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

   //specify that we want to communicate to device address with an intended write to bus
   I2CMasterSlaveAddrSet(I2C0_BASE, device_address, false);

   //register to be read
   I2CMasterDataPut(I2C0_BASE, device_register);

   //wait for MCU to finish transaction
   while(I2CMasterBusy(I2C0_BASE));

   //******************Send Data
   //specify data to be written to the above mentioned device_register
   I2CMasterDataPut(I2C0_BASE, device_data);
   //******************Finish
   //wait while checking for MCU to complete the transaction
   I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
   //******************Wait
   //wait for MCU & device to complete transaction
   while(I2CMasterBusy(I2C0_BASE));
}

UINT32 I2C_Read0(uint16_t device_address, uint16_t device_register)
{
	//specify that we are writing (a register address) to the
	//slave device
	I2CMasterSlaveAddrSet(I2C0_BASE, device_address, false);

	//specify register to be read
	I2CMasterDataPut(I2C0_BASE, device_register);

	//send control byte and register address byte to slave device
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

	//wait for MCU to finish transaction
	while(I2CMasterBusy(I2C0_BASE));

	//specify that we are going to read from slave device
	I2CMasterSlaveAddrSet(I2C0_BASE, device_address, true);

	//send control byte and read from the register we
	//specified
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

	//wait for MCU to finish transaction
	while(I2CMasterBusy(I2C0_BASE));

	//return data pulled from the specified register
	return I2CMasterDataGet(I2C0_BASE);
 }

/*
 * \BRIEF THIS IS INTERRUPT DRIVEN which queues up a multiple read struct
 *  We need to pass a callback function for when everything is complete.
 */
void I2C_Read0Multiiple(UINT16 device_address, UINT16 device_register, UINT8 *startofread, UINT32 numBytes, void (*getNextValue)(void))
{
   I2CMultReadStruct addToQueue;
   addToQueue.MultiReadActive = FALSE;
   addToQueue.State = I2C_MULTREAD__START;
   addToQueue.Address = device_address;
   addToQueue.Register = device_register;
   addToQueue.DataLocation = startofread;
   addToQueue.DataLocationLength = numBytes;
   addToQueue.CallbackFunction = getNextValue;

   if(((QueueReadIndex == 0) && (QueueWriteIndex == MULT_READ_QUEUE_SIZE-1)) ||
         (QueueWriteIndex == QueueReadIndex -1))
   {
      //overflow
      int i = 0;
   }
   else
   {
      MultReadQueue[QueueWriteIndex] = addToQueue;
      //Roll over the write index
      if((QueueWriteIndex + 1) >= MULT_READ_QUEUE_SIZE)
      {
         QueueWriteIndex = 0;
      }
      else
      {
         QueueWriteIndex++;
      }
   }
}


void I2C__MultReadStart(void)
{
   //Only start a multi read if it isn't already active
   if(I2CMultRead.MultiReadActive == FALSE)
   {
      //Make sure we havent caught up tot he write index
      if(QueueWriteIndex != QueueReadIndex)
      {
         I2CMultRead = MultReadQueue[QueueReadIndex++];
         I2CMultRead.MultiReadActive = TRUE;
         //Start the process
         I2C_Interrupt0();
      }

      //roll over the read index
      if(QueueReadIndex >= MULT_READ_QUEUE_SIZE)
      {
         QueueReadIndex = 0;
      }
   }
}
/*
 * \brief Interrupt driven I2C Module that is for Multi Reads
 */
void I2C_Interrupt0(void)
{
   static UINT32 numReads = 0;
   UINT32 interruptStatus = I2CMasterIntStatusEx(I2C0_BASE,false);
   I2CMasterIntClear(I2C0_BASE);
   I2CMasterIntClearEx(I2C0_BASE,I2C_INT_MASK);

   //Lets only run our Interrupt driven I2C read if it is active.
   if(I2CMultRead.MultiReadActive)
   {
      //We also need to check for I2C_MULTREAD__START because I2C_Read0Multiiple calls this function
      //TODO se if there is a way that we can trigger the interrupt (DT 2/14/2016 Looked quick and couldn't find anything)
      if(   (interruptStatus & I2C_MASTER_INT_DATA) ||
            (I2CMultRead.State == I2C_MULTREAD__START)
            )
      {
         switch(I2CMultRead.State)
            {
               case I2C_MULTREAD__START:
                  {
                     //specify that we are writing (a register address) to the
                     //slave device
                     I2CMasterSlaveAddrSet(I2C0_BASE, I2CMultRead.Address, false);

                     //I2CMasterDataPut(I2C0_BASE, device_register);
                     //specify register to be read
                     I2CMasterDataPut(I2C0_BASE, I2CMultRead.Register);

                     //send control byte and register address byte to slave device
                     I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);//I2C_MASTER_CMD_SINGLE_SEND

                     I2CMultRead.State = I2C_MULTREAD__SEND_REGISTER;
                  }
                  break;
               case I2C_MULTREAD__SEND_REGISTER:
                  {
                     //specify that we are going to read from slave device
                    I2CMasterSlaveAddrSet(I2C0_BASE, I2CMultRead.Address, true);

                    //send control byte and read from the register we
                    //specified
                    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
                    numReads = 0;
                  }
                  I2CMultRead.State = I2C_MULTREAD__CONTINUE_READ;
                  break;
               case I2C_MULTREAD__CONTINUE_READ:
                  {
                     I2CMultRead.DataLocation[numReads++] = I2CMasterDataGet(I2C0_BASE);
                     I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
                  }
                  //END OF READING
                  if(numReads == I2CMultRead.DataLocationLength)
                  {
                     I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
                     I2CMultRead.CallbackFunction();
                     I2CMultRead.State = I2C_MULTREAD__END_READ;
                     I2CMultRead.MultiReadActive = FALSE;
                  }
                  break;
               case I2C_MULTREAD__END_READ:
               default:
                  {
                     I2CMultRead.State = I2C_MULTREAD__START;
                     I2CMultRead.MultiReadActive = FALSE;
                  }
                  break;
            }
      }
   }
}

