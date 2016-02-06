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

   //BW_rate
   while(!I2C_WRITEVERIFY0(ADXL345_ADDRESS_0,ADXL345_BW_RATE,ADXL345_BWRATE_100))
   {}

	//Data Format (pg26)
	//Inverter is Active High
	//10 bit resolution
	//right-justified
	//+/- 2g Range
   //SELF_TEST = 0 / SPI = 0 / INT_INVERT = 0 / 0 / FULL_RES = 0 / Justify = 0 / Range = 00
   uint8_t dataFormat = 0b00000000;
   while(!I2C_WRITEVERIFY0(ADXL345_ADDRESS_0,ADXL345_DATA_FORMAT,dataFormat))
   {}

   //Fifo_control
   while(!I2C_WRITEVERIFY0(ADXL345_ADDRESS_0,ADXL345_FIFO_CTL, ADXL345_FIFOCTL_BYPASS))
   {}

	//Int Map (Any bit that is set will trigger Int 2 pin on the ADXL
	//Lets set up the DATA for Pin 1 and any errors on Pin 2.
	while(!I2C_WRITEVERIFY0(ADXL345_ADDRESS_0,ADXL345_INT_MAP, ADXL345_INTEN_OVERRUN | ADXL345_INTEN_WATERMARK))
   {}

   //Int Enable Reg
   while(!I2C_WRITEVERIFY0(ADXL345_ADDRESS_0,ADXL345_INT_ENABLE,ADXL345_INTEN_OVERRUN | ADXL345_INTEN_WATERMARK | ADXL345_INTEN_DATARDY))
   {}

   //Start Measureing
   while(!I2C_WRITEVERIFY0(ADXL345_ADDRESS_0,ADXL345_POWER_CTL,ADXL345_POWERCTL_MEASURE))
   {}

	//Init Information
	DeviceID = I2C_Read0(ADXL345_ADDRESS_0, ADXL345_REG_UID);
	UARTprintf("ReadValue UID: %d\n",DeviceID);

	//This is needed to clear out the interrupt.
	ADXL345_Read();
}

/*!
 * \brief Reads Instantaneous DATA from Accelerometer
 * \notes Should take place in Foreground which is ran periodically from a Timer!!!
 */
void ADXL345_Read()
{
   //Lets just read the uid at first
   //TODO Implement multi byte read so data does not change between reads!
  // I2C_Read0(ADXL345_ADDRESS_0, DATAX1);
  // I2C_Read0(ADXL345_ADDRESS_0, DATAX0);

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
	struct ADXL345_Data readingSum = AverageReadings;

   readingSum.x += InstantaneousReadings.x;
   readingSum.y += InstantaneousReadings.y;
   readingSum.z += InstantaneousReadings.z;

   AverageReadings.x = readingSum.x >> 1;
   AverageReadings.y = readingSum.y >> 1;
   AverageReadings.z = readingSum.z >> 1;

   printADXL345Readings("Average ADXL345\n", InstantaneousReadings);

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


//*****************************************************************************
//    Interrupts
//*****************************************************************************
void ADXL345__InterruptIRQ(void)
{
   ADXL345_Read();
}
