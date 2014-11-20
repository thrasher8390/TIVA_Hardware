/*
 * ADXL345.c
 *
 *  Created on: Nov 17, 2014
 *      Author: DLThrasher
 */
//*****************************************************************************
//		Includes
//*****************************************************************************
#include "ADXL345.h"


//*****************************************************************************
//		Defines
//*****************************************************************************
#define ADXL345_ADDRESS_0 (0x001D)
//*****************************************************************************
//		Global Functions
//*****************************************************************************
/*!
 * \brief Init I2C
 */


void ADXL345_Init()
{
	//Lets initialize the I2C module that will be used for the ADXL345
	//Init I2C Module 0
	I2C_Init0();

	//DataFormat
	I2C_Write0(ADXL345_ADDRESS_0,ADXL345_DATA_FORMAT, ADXL345_DATAFORMAT_4G);
	//Power Control
	I2C_Write0(ADXL345_ADDRESS_0,ADXL345_POWER_CTL,ADXL345_POWERCTL_UNKNOWN);
	//Int Enable/
	I2C_Write0(ADXL345_ADDRESS_0,ADXL345_INT_ENABLE,ADXL345_INTEN_ENABLE);
	//Int Map
	I2C_Write0(ADXL345_ADDRESS_0,ADXL345_INT_MAP,ADXL345_INTMAP_INT1);
	//BW_rate
	I2C_Write0(ADXL345_ADDRESS_0,ADXL345_BW_RATE,ADXL345_BWRATE_100);
	//Fifo_control
	I2C_Write0(ADXL345_ADDRESS_0,ADXL345_FIFO_CTL, ADXL345_FIFOCTL_BYPASS);
}
uint8_t readData;
uint8_t ADXL345_Read()
{
	//Lets just read the uid at first
	UARTprintf("ReadValue UID: %d\n",I2C_Read0(ADXL345_ADDRESS_0, ADXL345_REG_UID));;
	UARTprintf("ReadValueX: %d\n",(((I2C_Read0(ADXL345_ADDRESS_0, DATAX1)&0x03)<<8) + I2C_Read0(ADXL345_ADDRESS_0, DATAX0)));
	//UARTprintf("ReadValue: %d\n",I2C_Read0(ADXL345_ADDRESS_0, DATAX1));
	UARTprintf("ReadValueY: %d\n",(((I2C_Read0(ADXL345_ADDRESS_0, DATAY1) & 0x03)<<8)+I2C_Read0(ADXL345_ADDRESS_0, DATAY0)));
	//UARTprintf("ReadValue: %d\n",I2C_Read0(ADXL345_ADDRESS_0, DATAY1));
	UARTprintf("ReadValueZ: %d\n",(((I2C_Read0(ADXL345_ADDRESS_0, DATAZ1)&0x03)<<8)+I2C_Read0(ADXL345_ADDRESS_0, DATAZ0)));
	UARTprintf("\n");
	//UARTprintf("ReadValue: %d\n",I2C_Read0(ADXL345_ADDRESS_0, DATAZ1));
	return readData;
}

