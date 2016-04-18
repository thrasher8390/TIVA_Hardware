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
#define GYRO_NUMBER_OF_READINGS (6)
//*****************************************************************************
//		Variables
//*****************************************************************************
static UINT8 RawGyroReadings[GYRO_NUMBER_OF_READINGS];
static struct Gyro_Data ValidatedGyroReadings;
static UINT8 DeviceID = 0;
static BOOLEAN DataIsReady = FALSE;
//*****************************************************************************
//    LocalFunctionPrototypes
//*****************************************************************************
static void validateReadings();
static void printGyroReadings(const char *tag, struct Gyro_Data data);
//*****************************************************************************
//		Global Functions
//*****************************************************************************

void Gyro_Init()
{
	//DataFormat
	while(!I2C_WriteVerify0(GYRO_ADDRESS,GYRO_ADDR_CTRL1, CTRL1_ODRHZ_100_12 | CTRL1_NORMAL_MODE |CTRL1_ENABLEXYZ)){}
	while(!I2C_WriteVerify0(GYRO_ADDRESS,GYRO_ADDR_CTRL2, CTRL2_HPFMODE_NORMAL)){}
	while(!I2C_WriteVerify0(GYRO_ADDRESS,GYRO_ADDR_CTRL3, CTRL3_INT2_DRDY_ENABLE)){}
	//TODO find definition of full scale and dsp. degres...
	while(!I2C_WriteVerify0(GYRO_ADDRESS,GYRO_ADDR_CTRL4, CTRL4_FULLSCALE_500)){}
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
      printGyroReadings("Average gyro\n", ValidatedGyroReadings);
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
   DataIsReady = TRUE;
}

/*!
 * \brief prints out the x,y,z values in ADXL345_Data format
 */
static void printGyroReadings(const char *tag, struct Gyro_Data data)
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
      //x is negative
      if(data.x & 0x8000)
      {
         UARTprintf("-%d,",(0xFFFF-data.x));
      }
      else
      {
         UARTprintf("%d,",data.x);
      }

      //y is negative
      if(data.y & 0x8000)
      {
         UARTprintf("-%d,",(0xFFFF-data.y));
      }
      else
      {
         UARTprintf("%d,",data.y);
      }

      //z is negative
      if(data.z & 0x8000)
      {
         UARTprintf("-%d",(0xFFFF-data.z));
      }
      else
      {
         UARTprintf("%d",data.z);
      }
      UARTprintf("\n");
   }
}
//*****************************************************************************
//    Local Functions
//*****************************************************************************
void Gyro__InterruptIRQ(void)
{
   Gyro_Read();
}
