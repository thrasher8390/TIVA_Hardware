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

   //enable GPIO peripheral that contains I2C
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

   // Configure the pin muxing for I2C0 functions on port B2 and B3.
   GPIOPinConfigure(GPIO_PB2_I2C0SCL);
   GPIOPinConfigure(GPIO_PB3_I2C0SDA);

   // Select the I2C function for these pins.
   GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
   GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

   // Enable and initialize the I2C0 master module.  Use the system clock for
   // the I2C0 module.  The last parameter sets the I2C data transfer rate.
   // If false the data rate is set to 100kbps and if true the data rate will
   // be set to 400kbps.
   I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);

   //Thrasher - Why was this here?
   //clear I2C FIFOs
   //HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
}

uint32_t I2C_Read0(uint16_t device_address, uint16_t device_register)
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

void I2C_Write0(uint16_t device_address, uint16_t device_register, uint8_t device_data)
{
   //specify that we want to communicate to device address with an intended write to bus
   I2CMasterSlaveAddrSet(I2C0_BASE, device_address, false);

   //register to be read
   I2CMasterDataPut(I2C0_BASE, device_register);

   //send control byte and register address byte to slave device
   I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

   //wait for MCU to finish transaction
   while(I2CMasterBusy(I2C0_BASE));

   //********************Register
   //put next piece of data into I2C FIFO
   I2CMasterDataPut(I2C0_BASE, device_register);
   //******************Continue
   //send next data that was just placed into FIFO
   I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
   //******************Wait
   // Wait until MCU is done transferring.
   while(I2CMasterBusy(I2C0_BASE));

   //TODO We can make this into a multiple write function!
   ///RIGHT HERE WE COULD SEND MORE DATA CONTINUE AND WAIT!

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
