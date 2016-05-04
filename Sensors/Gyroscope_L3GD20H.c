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
#include "SensorHelperFunctions.h"
#include "I2C.h"
//*****************************************************************************
//		Defines
//*****************************************************************************
#define GYRO_ADDRESS (GYRO_ADDRESS_0)
#define GYRO_NUMBER_OF_READINGS (6)
//*****************************************************************************
//		Variables
//*****************************************************************************
static UINT8 RawGyroReadings[GYRO_NUMBER_OF_READINGS];
static struct Sensor_Data ValidatedGyroReadings;
static UINT8 DeviceID = 0;
static BOOLEAN DataIsReady = FALSE;
//*****************************************************************************
//    LocalFunctionPrototypes
//*****************************************************************************
static void validateReadings();
//*****************************************************************************
//		Global Functions
//*****************************************************************************

void Gyro_Init()
{
	//DataFormat
	while(!I2C_WriteVerify0(GYRO_ADDRESS,GYRO_ADDR_CTRL1, CTRL1_ODRHZ_400_20 | CTRL1_NORMAL_MODE |CTRL1_ENABLEXYZ)){}
	while(!I2C_WriteVerify0(GYRO_ADDRESS,GYRO_ADDR_CTRL2, CTRL2_HPFMODE_NORMAL)){}
	while(!I2C_WriteVerify0(GYRO_ADDRESS,GYRO_ADDR_CTRL3, CTRL3_INT2_DRDY_ENABLE)){}
	//TODO find definition of full scale and dsp. degres...
	while(!I2C_WriteVerify0(GYRO_ADDRESS,GYRO_ADDR_CTRL4, CTRL4_FULLSCALE_2000)){}
	//TODO need to understand HPFilter!
	while(!I2C_WriteVerify0(GYRO_ADDRESS,GYRO_ADDR_CTRL5, 0x00)){}
	while(!I2C_WriteVerify0(GYRO_ADDRESS,GYRO_ADDR_REFERENCE, 0x00)){}
	while(!I2C_WriteVerify0(GYRO_ADDRESS,GYRO_ADDR_FIFOCTRL, 0x00)){}

	DeviceID = I2C_Read0(GYRO_ADDRESS, GYRO_ADDR_WHOAMI);
}


void Gyro_Read(void)
{
   I2C_Read0Multiiple(GYRO_ADDRESS, GYRO_ADDR_OUTXLOW | 0x80, RawGyroReadings, GYRO_NUMBER_OF_READINGS, validateReadings);
}

/*!
 *\brief This processes the readings meaning that it needs to be called faster than what our data is coming in at.
 */
void Gyro__ProcessReadings(void)
{
   //Process the data!
   if(DataIsReady == TRUE)
   {
      DataIsReady = FALSE;
      SenosorHelperFunctions__PrintSensorReadings("GYRO:", ValidatedGyroReadings);
   }
}
//*****************************************************************************
//		Local Functions
//*****************************************************************************
/*!
 *\brief This pulls the readings from the Raw Gyro into validated Gyro
 */
static void validateReadings()
{
   //error condition we didn't use the data before we are about to write over it
   if(DataIsReady == TRUE)
   {
      //asm(" bpkt #0\n");
   }
   //Pull out the data read from Accelerometer
   uint8_t xHigh  = RawGyroReadings[1];
   uint8_t xLow   = RawGyroReadings[0];
   ValidatedGyroReadings.x = ((xHigh << 8) + xLow);

   uint8_t yHigh  = RawGyroReadings[3];
   uint8_t yLow   = RawGyroReadings[2];
   ValidatedGyroReadings.y = ((yHigh << 8) + yLow);

   uint8_t zHigh  = RawGyroReadings[5];
   uint8_t zLow   = RawGyroReadings[4];
   ValidatedGyroReadings.z = ((zHigh << 8) + zLow);

   SenosorHelperFunctions__Normalize(&ValidatedGyroReadings);

   DataIsReady = TRUE;
}

//*****************************************************************************
//    Local Functions
//*****************************************************************************
void Gyro__InterruptIRQ(void)
{
   Gyro_Read();
}
