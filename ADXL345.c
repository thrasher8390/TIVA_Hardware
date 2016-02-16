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
static uint8_t RawADXLReadings[6];
struct ADXL345_Data ValidatedADXLReadings; 	/*!< These values are read every Foreground cycle (UNDEFINED_FOREGROUND_CYCLE_TIME) */
uint8_t DeviceID;
static BOOLEAN DataIsReady = FALSE;
//*****************************************************************************
//		local Functions
//*****************************************************************************
static void printADXL345Readings(const char *, struct ADXL345_Data);
static void validateReadings(void);

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
   while(!I2C_WRITEVERIFY0(ADXL345_ADDRESS_0,ADXL345_BW_RATE,ADXL345_BWRATE_800))
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
	//Lets set up the DATA for Pin 2
   //TODO INT 1 was broken during development
	while(!I2C_WRITEVERIFY0(ADXL345_ADDRESS_0,ADXL345_INT_MAP, ADXL345_INTEN_DATARDY))
   {}

   //Int Enable Reg
   while(!I2C_WRITEVERIFY0(ADXL345_ADDRESS_0,ADXL345_INT_ENABLE,ADXL345_INTEN_OVERRUN | ADXL345_INTEN_WATERMARK | ADXL345_INTEN_DATARDY))
   {}

   //Start Measureing
   while(!I2C_WRITEVERIFY0(ADXL345_ADDRESS_0,ADXL345_POWER_CTL,ADXL345_POWERCTL_MEASURE))
   {}

	//Init Information
	DeviceID = I2C_Read0(ADXL345_ADDRESS_0, ADXL345_REG_UID);
	//UARTprintf("ReadValue UID: %d\n",DeviceID);

	//This is needed to clear out the interrupt.
	ADXL345_Read();
}

/*!
 * \brief Reads Instantaneous DATA from Accelerometer
 * \notes Should take place in Foreground which is ran periodically from a Timer!!!
 */
void ADXL345_Read()
{
	I2C_Read0Multiiple(ADXL345_ADDRESS_0,DATAX0,RawADXLReadings, 6,validateReadings);
}

/*!
 *\brief This processes the readings meaning that it needs to be called faster than what our data is coming in at.
 */
void ADXL345__ProcessReadings()
{

   //Process the data!
   if(DataIsReady == TRUE)
   {
      SET_TESTPOINT_0();
      DataIsReady = FALSE;
      printADXL345Readings("Average ADXL345\n", ValidatedADXLReadings);
      CLEAR_TESTPOINT_0();
   }

}

//*****************************************************************************
//		Local Functions
//*****************************************************************************

/*!
 *\brief This pulls the readings from the Raw ADXL into validated ADXL
 */
static void validateReadings()
{
   //error condition we didn't use the data before we are about to write over it
   if(DataIsReady == TRUE)
   {
      //asm(" bpkt #0\n");
   }
   //Pull out the data read from Accelerometer
   uint8_t xHigh  = RawADXLReadings[1];
   uint8_t xLow   = RawADXLReadings[0];
   ValidatedADXLReadings.x = ((xHigh << 8) + xLow);

   uint8_t yHigh  = RawADXLReadings[3];
   uint8_t yLow   = RawADXLReadings[2];
   ValidatedADXLReadings.y = ((yHigh << 8) + yLow);

   uint8_t zHigh  = RawADXLReadings[5];
   uint8_t zLow   = RawADXLReadings[4];
   ValidatedADXLReadings.z = ((zHigh << 8) + zLow);
   DataIsReady = TRUE;
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
//    Interrupts
//*****************************************************************************
void ADXL345__InterruptIRQ(void)
{
   ADXL345_Read();
}
