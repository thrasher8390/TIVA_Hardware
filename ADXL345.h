/*
 * ADXL345.h
 *
 *  Created on: Nov 17, 2014
 *      Author: DLThrasher
 */

#ifndef ADXL345_H_
#define ADXL345_H_

//*****************************************************************************
//		Includes
//*****************************************************************************
#include "Project.h"
#include "stdio.h"


//*****************************************************************************
//		Defines
//*****************************************************************************
#define ADXL345_ADDRESS_0     (0x0053)

#define ADXL345_POWER_CTL     (0x2D)	//Power Control Register
#define ADXL345_DATA_FORMAT   (0x31)
#define ADXL345_INT_ENABLE    (0x2E)
#define ADXL345_FIFO_CTL      (0x38)
#define ADXL345_INT_MAP       (0x2F)
#define ADXL345_INT_SOURCE    (0x30)
#define ADXL345_BW_RATE       (0x2C)
#define ADXL345_REG_UID       (0x00)
#define ADXL345_DATA_FORMAT   (0x31)
#define DATAX0                (0x32)	//X-Axis Data 0
#define DATAX1                (0x33)	//X-Axis Data 1
#define DATAY0                (0x34)	//Y-Axis Data 0
#define DATAY1                (0x35)	//Y-Axis Data 1
#define DATAZ0                (0x36)	//Z-Axis Data 0
#define DATAZ1                (0x37)	//Z-Axis Data 1

//DataFormat Values
#define ADXL345_DATAFORMAT_4G    (0x01)//Interrupt invert (B5)

//Power CTL Values
#define ADXL345_POWERCTL_SLEEP   (0x04)
#define ADXL345_POWERCTL_MEASURE (0x08)

//INT MAP
#define ADXL345_INTMAP_INT2      (0x00)

//INT ENABLE
#define ADXL345_INTEN_DISABLE    (0x00)
#define ADXL345_INTEN_OVERRUN    (0x01)
#define ADXL345_INTEN_WATERMARK  (0x02)
#define ADXL345_INTEN_FREEFALL   (0x04)
#define ADXL345_INTEN_INACTIVITY (0x08)
#define ADXL345_INTEN_ACTIVITY   (0x10)
#define ADXL345_INTEN_DOUBTAP    (0x20)
#define ADXL345_INTEN_SINGTAP    (0x40)
#define ADXL345_INTEN_DATARDY    (0x80)

//BW Rate
#define ADXL345_BWRATE_3200      (0x0F)
#define ADXL345_BWRATE_1600      (0x0E)
#define ADXL345_BWRATE_800       (0x0D)
#define ADXL345_BWRATE_400       (0x0C)
#define ADXL345_BWRATE_100       (0x0A)

//FIFO Controll
#define ADXL345_FIFOCTL_BYPASS   (0x00)
//*****************************************************************************
//		Global Structs
//*****************************************************************************
struct ADXL345_Data
{
	int32_t x;
	int32_t y;
	int32_t z;
};
//*****************************************************************************
//		Global Functions
//*****************************************************************************
extern void    ADXL345_Init(void);
extern void    ADXL345_Read(void);
extern void    ADXL345__InterruptIRQ(void);
extern void    ADXL345__ProcessReadings(void);

#endif /* ADXL345_H_ */
