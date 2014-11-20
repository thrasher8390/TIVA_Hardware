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
#include "I2C.h"

//*****************************************************************************
//		Defines
//*****************************************************************************
#define ADXL345_POWER_CTL  		(0x2D)	//Power Control Register
#define ADXL345_DATA_FORMAT 	(0x31)
#define ADXL345_INT_ENABLE 		(0x2E)
#define ADXL345_FIFO_CTL 		(0x38)
#define ADXL345_INT_MAP 		(0x2F)
#define ADXL345_BW_RATE 		(0x2C)
#define ADXL345_REG_UID			(0x00)
#define DATAX0  				(0x32)	//X-Axis Data 0
#define DATAX1  				(0x33)	//X-Axis Data 1
#define DATAY0  				(0x34)	//Y-Axis Data 0
#define DATAY1  				(0x35)	//Y-Axis Data 1
#define DATAZ0  				(0x36)	//Z-Axis Data 0
#define DATAZ1  				(0x37)	//Z-Axis Data 1

//DataFormat Values
#define ADXL345_DATAFORMAT_4G 	(0x01)

//Power CTL Values
#define ADXL345_POWERCTL_UNKNOWN (0x08)

//INT
#define ADXL345_INTMAP_INT1		(0x7F)
#define ADXL345_INTEN_ENABLE	(0x08)

//BW Rate
#define ADXL345_BWRATE_3200 	(0x0F)
#define ADXL345_BWRATE_100		(0x0A)
//FIFO Controll
#define ADXL345_FIFOCTL_BYPASS (0x00)
//*****************************************************************************
//		Global Functions
//*****************************************************************************
extern void 	ADXL345_Init();
extern uint8_t 	ADXL345_Read();


#endif /* ADXL345_H_ */
