/*
 * SensorHelperFunctions.h
 *
 *  Created on: May 1, 2016
 *      Author: DLThrasher
 */

#ifndef TIVA_HARDWARE_SENSORS_SENSORHELPERFUNCTIONS_H_
#define TIVA_HARDWARE_SENSORS_SENSORHELPERFUNCTIONS_H_

//*****************************************************************************
//    Includes
//*****************************************************************************
#include "Project.h"

//*****************************************************************************
//    Data Types & Enums
//*****************************************************************************
struct Sensor_Data
{
   float x;
   float y;
   float z;
   float normalized_x;
   float normalized_y;
   float normalized_z;
};
//*****************************************************************************
//    Defines
//*****************************************************************************

//*****************************************************************************
//    Global Functions Prototypes
//*****************************************************************************
void SenosorHelperFunctions__Normalize(struct Sensor_Data *);
void SenosorHelperFunctions__PrintSensorReadings(const char * const tag, struct Sensor_Data);

#endif /* TIVA_HARDWARE_SENSORS_SENSORHELPERFUNCTIONS_H_ */
