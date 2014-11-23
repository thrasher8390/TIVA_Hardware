/*
 * Gyroscope_L3GD20H.c
 *
 *  Created on: Nov 19, 2014
 *      Author: DLThrasher
 */
//*****************************************************************************
//		Includes
//*****************************************************************************
#include "Gyroscope_L3GD20H.h"
#include "I2C.h"
//*****************************************************************************
//		Defines
//*****************************************************************************
#define GYRO_ADDRESS (GYRO_ADDRESS_0)
//*****************************************************************************
//		Variables
//*****************************************************************************
//*****************************************************************************
//		Global Functions
//*****************************************************************************

#define MAXDR (0xC0)
#define MAXBW (0x30)
#define ENABLEXYZ (0x03)
void Gyro_Init()
{
	//Lets initialize the I2C module that will be used for the ADXL345
	//Init I2C Module 0
	I2C_Init0();

	//DataFormat
	I2C_Write0(GYRO_ADDRESS,GYRO_ADDR_CTRL1, MAXDR | MAXBW | ENABLEXYZ); //MaxDR|
	I2C_Write0(GYRO_ADDRESS,GYRO_ADDR_CTRL2, 0x00);
	I2C_Write0(GYRO_ADDRESS,GYRO_ADDR_CTRL3, 0x00);
	I2C_Write0(GYRO_ADDRESS,GYRO_ADDR_CTRL4, 0x00);
	I2C_Write0(GYRO_ADDRESS,GYRO_ADDR_CTRL5, 0x00);
	I2C_Write0(GYRO_ADDRESS,GYRO_ADDR_REFERENCE, 0x00);
	I2C_Write0(GYRO_ADDRESS,GYRO_ADDR_FIFOCTRL, 0x00);
}


uint8_t Gyro_Read()
{
	uint8_t readData = 0;
	UARTprintf("Who Am I: %d\n",I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_WHOAMI));;
	UARTprintf("ReadValueX: %d\n",(((I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_OUTXHIGH))<<8) + I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_OUTXLOW)));
	UARTprintf("ReadValueY: %d\n",(((I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_OUTYHIGH))<<8)+I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_OUTYLOW)));
	UARTprintf("ReadValueZ: %d\n",(((I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_OUTZHIGH))<<8)+I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_OUTZLOW)));
	UARTprintf("\n");
	return readData;
}
//*****************************************************************************
//		Local Functions
//*****************************************************************************
