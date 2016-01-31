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
#include "I2C.h"

//*****************************************************************************
//		Defines
//*****************************************************************************

#define	NUMBER_OF_ADXL345_READINGS_TO_AVERAGE     (32)		/*!<Effects AVERAGE_ADXL345_READINGS*/
#define	AVERAGE_ADXL345_READINGS(x)               ((x>>5))

//*****************************************************************************
//		Data
//*****************************************************************************

struct ADXL345_Data AverageReadings;         /*!< This is average every 'NUMBER_OF_ADXL345_READINGS_TO_AVERAGE' Foreground Cycles = 32 * UNDEFINED_FOREGROUND_CYCLE_TIME */
struct ADXL345_Data InstantaneousReadings; 	/*!< These values are read every Foreground cycle (UNDEFINED_FOREGROUND_CYCLE_TIME) */
uint8_t DeviceID;

//*****************************************************************************
//		local Functions
//*****************************************************************************

static void averageReadings();
static void printADXL345Readings();

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

	//Set Data Format (pg26)
	//SELF_TEST = 0 / SPI = 0 / INT_INVERT = 0 / 0 / FULL_RES = 0 / Justify = 0 / Range = 00
	uint8_t dataFormat = 0x00;
	I2C_Write0(ADXL345_ADDRESS_0,ADXL345_DATA_FORMAT,dataFormat);

	//Init Information
	DeviceID = I2C_Read0(ADXL345_ADDRESS_0, ADXL345_REG_UID);
	UARTprintf("ReadValue UID: %d\n",DeviceID);
}

/*!
 * \brief Reads Instantaneous DATA from Accelerometer
 * \notes Should take place in Foreground which is ran periodically from a Timer!!!
 */
void ADXL345_Read()
{

	//Lets just read the uid at first
	//TODO Implement multi byte read so data does not change between reads!
   I2C_Read0(ADXL345_ADDRESS_0, DATAX1);
   I2C_Read0(ADXL345_ADDRESS_0, DATAX0);


	uint8_t ADXLReadings[6];
   //TODO Implement multi byte read so data does not change between reads!
	I2C_Read0Multiiple(ADXL345_ADDRESS_0,DATAX0,ADXLReadings, 6);
	uint8_t xHigh  = ADXLReadings[1];
	uint8_t xLow   = ADXLReadings[0];
   InstantaneousReadings.x = ((xHigh << 8) + xLow);

   uint8_t yHigh  = ADXLReadings[3];
   uint8_t yLow   = ADXLReadings[2];
   InstantaneousReadings.y = ((yHigh << 8) + yLow);

   uint8_t zHigh  = ADXLReadings[5];
   uint8_t zLow   = ADXLReadings[4];
   InstantaneousReadings.z = ((zHigh << 8) + zLow);
   //printADXL345Readings("Instantaneous ADXL345\n", InstantaneousReadings);

	averageReadings();
}

//*****************************************************************************
//		Local Functions
//*****************************************************************************

/*!
 *\brief This averages the readings over a sent number of readings
 */
void averageReadings()
{
	static uint8_t numberOfReadingsTaken = 0;
	static struct ADXL345_Data readingSum = {0,0,0};
	//Keep summing values until we have enough
	if(numberOfReadingsTaken <= NUMBER_OF_ADXL345_READINGS_TO_AVERAGE)
	{
		readingSum.x += InstantaneousReadings.x;
		readingSum.y += InstantaneousReadings.y;
		readingSum.z += InstantaneousReadings.z;
		numberOfReadingsTaken++;
	}
	//Lets Average and reset our averaging varialbes
	else
	{
		AverageReadings.x = AVERAGE_ADXL345_READINGS(readingSum.x);
		AverageReadings.y = AVERAGE_ADXL345_READINGS(readingSum.y);
		AverageReadings.z = AVERAGE_ADXL345_READINGS(readingSum.z);
		numberOfReadingsTaken = 0;
		readingSum.x = 0;
		readingSum.y = 0;
		readingSum.z = 0;
		//printADXL345Readings("Average ADXL345\n", InstantaneousReadings);
	}
}

/*!
 * \brief prints out the x,y,z values in ADXL345_Data format
 */
void printADXL345Readings(const char *tag, struct ADXL345_Data data)
{
	if(false)
	{
		UARTprintf(tag);
		UARTprintf("ReadValueX: %d\n",data.x);
		UARTprintf("ReadValueY: %d\n",data.y);
		UARTprintf("ReadValueZ: %d\n",data.z);
		UARTprintf("\n");
	}
	if(true)
	{
		UARTprintf("%d,%d,%d\n",data.x,data.y,data.z);
	}

}

