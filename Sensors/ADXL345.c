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
#include "SensorHelperFunctions.h"
#include "I2C.h"

//*****************************************************************************
//		Defines
//*****************************************************************************

//*****************************************************************************
//		Data
//*****************************************************************************
static UINT8 RawADXLReadings[6];
struct Sensor_Data ValidatedADXLReadings; 	/*!< These values are read every Foreground cycle (UNDEFINED_FOREGROUND_CYCLE_TIME) */
uint8_t DeviceID;
static BOOLEAN DataIsReady = FALSE;
//*****************************************************************************
//		local Functions
//*****************************************************************************
static void validateReadings(void);

//*****************************************************************************
//		Global Functions
//*****************************************************************************

/*!
 * \brief Init I2C
 */
void ADXL345_Init()
{
   //BW_rate
   while(!I2C_WriteVerify0(ADXL345_ADDRESS_0,ADXL345_BW_RATE,ADXL345_BWRATE_400))
   {}

	//Data Format (pg26)
	//Inverter is Active High
	//10 bit resolution
	//right-justified
	//+/- 2g Range
   //SELF_TEST = 0 / SPI = 0 / INT_INVERT = 0 / 0 / FULL_RES = 0 / Justify = 0 / Range = 00
   uint8_t dataFormat = 0b00000000;
   while(!I2C_WriteVerify0(ADXL345_ADDRESS_0,ADXL345_DATA_FORMAT,dataFormat))
   {}

   //Fifo_control
   while(!I2C_WriteVerify0(ADXL345_ADDRESS_0,ADXL345_FIFO_CTL, ADXL345_FIFOCTL_BYPASS))
   {}

	//Int Map (Any bit that is set will trigger Int 2 pin on the ADXL
	//Lets set up the DATA for Pin 2
   //TODO INT 1 was broken during development
	while(!I2C_WriteVerify0(ADXL345_ADDRESS_0,ADXL345_INT_MAP, ADXL345_INTEN_DATARDY))
   {}

   //Int Enable Reg
   while(!I2C_WriteVerify0(ADXL345_ADDRESS_0,ADXL345_INT_ENABLE,ADXL345_INTEN_OVERRUN | ADXL345_INTEN_WATERMARK | ADXL345_INTEN_DATARDY))
   {}

   //Start Measureing
   while(!I2C_WriteVerify0(ADXL345_ADDRESS_0,ADXL345_POWER_CTL,ADXL345_POWERCTL_MEASURE))
   {}

	//Init Information
	DeviceID = I2C_Read0(ADXL345_ADDRESS_0, ADXL345_REG_UID);
	//UARTprintf("ReadValue UID: %d\n",DeviceID);
}

/*!
 * \brief Reads Instantaneous DATA from Accelerometer
 * \notes Should take place in Foreground which is ran periodically from a Timer!!!
 */
void ADXL345_Read()
{
	I2C_Read0Multiiple(ADXL345_ADDRESS_0, DATAX0, RawADXLReadings, 6, validateReadings);
}

/*!
 *\brief This processes the readings meaning that it needs to be called faster than what our data is coming in at.
 *\details I'm running at 400Hz so I should have new data every
 */
void ADXL345__ProcessReadings()
{

   //Process the data!
   if(DataIsReady == TRUE)
   {
      DataIsReady = FALSE;
      SenosorHelperFunctions__PrintSensorReadings("A:", ValidatedADXLReadings);
   }

}

//*****************************************************************************
//		Local Functions
//*****************************************************************************

static float convert2ByteToFloat(UINT8 high, UINT8 low)
{
   UINT16 value = ((high << 8) + low);
   return (float)(value & 0x8000 ? -(0x10000-value) : value);
}

/*!
 *\brief This pulls the readings from the Raw ADXL into validated ADXL
 */
static void validateReadings()
{
   //error condition we didn't use the data before we are about to write over it
   if(DataIsReady == TRUE)
   {
      int i = 0;
   }
   //Pull out the data read from Accelerometer
   uint8_t xHigh  = RawADXLReadings[1];
   uint8_t xLow   = RawADXLReadings[0];
   ValidatedADXLReadings.x = convert2ByteToFloat(xHigh, xLow);

   uint8_t yHigh  = RawADXLReadings[3];
   uint8_t yLow   = RawADXLReadings[2];
   ValidatedADXLReadings.y = convert2ByteToFloat(yHigh, yLow);

   uint8_t zHigh  = RawADXLReadings[5];
   uint8_t zLow   = RawADXLReadings[4];
   ValidatedADXLReadings.z = convert2ByteToFloat(zHigh, zLow);

   SenosorHelperFunctions__Normalize(&ValidatedADXLReadings);
   DataIsReady = TRUE;
}

//*****************************************************************************
//    Interrupts
//*****************************************************************************
void ADXL345__InterruptIRQ(void)
{
   ADXL345_Read();
}