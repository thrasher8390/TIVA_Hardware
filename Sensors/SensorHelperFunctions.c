/*
 * SensorHelperFunctions.c
 *
 *  Created on: May 1, 2016
 *      Author: DLThrasher
 */

//*****************************************************************************
//    Includes
//*****************************************************************************
#include "SensorHelperFunctions.h"
#include "math.h"
#include "stdio.h"
//*****************************************************************************
//    Defines
//*****************************************************************************

//*****************************************************************************
//    Variables
//*****************************************************************************

//*****************************************************************************
//    LocalFunctionPrototypes
//*****************************************************************************

//*****************************************************************************
//    Global Functions
//*****************************************************************************
/*
 * \brief normalizes the Sensor Data
 */
void SenosorHelperFunctions__Normalize(struct Sensor_Data * vector)
{
  float R = __sqrtf(vector->x*vector->x + vector->y*vector->y + vector->z*vector->z);
  vector->normalized_x = vector->x / R;
  vector->normalized_y = vector->y / R;
  vector->normalized_z = vector->z / R;
}

/*!
 * \brief prints out the x,y,z values in ADXL345_Data format
 */
void SenosorHelperFunctions__PrintSensorReadings(const char * const tag, struct Sensor_Data data)
{
   if(false)
   {
      UARTprintf(tag);
      if(false)
      {
         #define SIZE_OF_DATA (6)
         char pcBuffer[SIZE_OF_DATA];
         snprintf(pcBuffer, SIZE_OF_DATA, "%f,", data.normalized_x);
         UARTprintf(pcBuffer);
         UARTprintf(",");
         snprintf(pcBuffer, SIZE_OF_DATA, "%f,", data.normalized_y);
         UARTprintf(pcBuffer);
         UARTprintf(",");
         snprintf(pcBuffer, SIZE_OF_DATA, "%f", data.normalized_z);
         UARTprintf(pcBuffer);
      }
      UARTprintf("\n");
   }

}

//*****************************************************************************
//    Local Functions
//*****************************************************************************


