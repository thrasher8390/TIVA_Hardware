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

void Gyro_Init()
{
	//Lets initialize the I2C module that will be used for the ADXL345
	//Init I2C Module 0
	I2C_Init0();
	//DataFormat
	I2C_Write0(GYRO_ADDRESS,GYRO_ADDR_CTRL1, CTRL1_MINDR | CTRL1_MINBW | CTRL1_NORMAL_MODE |CTRL1_ENABLEXYZ);
	I2C_Write0(GYRO_ADDRESS,GYRO_ADDR_CTRL2, CTRL2_HPFMODE_NORMAL);
	I2C_Write0(GYRO_ADDRESS,GYRO_ADDR_CTRL3, 0x00);
	//TODO find definition of full scale and dsp. degres...
	I2C_Write0(GYRO_ADDRESS,GYRO_ADDR_CTRL4, CTRL4_FULLSCALE_500);
	//TODO need to understand HPFilter!
	I2C_Write0(GYRO_ADDRESS,GYRO_ADDR_CTRL5, 0x00);
	I2C_Write0(GYRO_ADDRESS,GYRO_ADDR_REFERENCE, 0x00);
	I2C_Write0(GYRO_ADDRESS,GYRO_ADDR_FIFOCTRL, 0x00);
}


uint8_t Gyro_Read()
{
	uint8_t readData = 0;

	int16_t xTempLow = I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_OUTXLOW);
	int16_t xTempHgih = I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_OUTXHIGH);
	int16_t yTempLow = I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_OUTYLOW);
	int16_t yTempHigh = I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_OUTYHIGH);
	int16_t zTempLow = I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_OUTZLOW);
	int16_t zTempHigh = I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_OUTZHIGH);






	UARTprintf("Who Am I: %d\n",I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_WHOAMI));
	UARTprintf("Control 1: %d\n",I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_CTRL1));
	UARTprintf("Control 2: %d\n",I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_CTRL2));
	UARTprintf("Control 5: %d\n",I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_CTRL5));
	UARTprintf("Status: %d\n",I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_STATUS));
	int16_t xTemp = (int16_t)(((I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_OUTXHIGH))<<8) | I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_OUTXLOW)) ;
	UARTprintf("ReadValueX: %d\n",xTemp);
	UARTprintf("ReadValueY: %d\n",(int16_t)(((I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_OUTYHIGH))<<8) | I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_OUTYLOW)));
	UARTprintf("ReadValueZ: %d\n",(int16_t)(((I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_OUTZHIGH))<<8) | I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_OUTZLOW)));
	UARTprintf("\n");
	return readData;
}
//*****************************************************************************
//		Local Functions
//*****************************************************************************
