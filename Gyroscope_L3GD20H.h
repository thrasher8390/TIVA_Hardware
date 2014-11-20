/*
 * Gyroscope_L3GD20H.h
 *
 *  Created on: Nov 19, 2014
 *      Author: DLThrasher
 */

#ifndef GYROSCOPE_L3GD20H_H_
#define GYROSCOPE_L3GD20H_H_
//*****************************************************************************
//		Includes
//*****************************************************************************
#include "Project.h"

//*****************************************************************************
//		Defines
//*****************************************************************************
//If CA0 is tied to gnd
#define GYRO_ADDRESS_0 		(0x006A)
//If CA0 is tied to vcc
#define GYRO_ADDRESS_1 		(0x006B)

#define GYRO_ADDR_WHOAMI	(0x0F)
#define GYRO_ADDR_CTRL1		(0x20)
#define GYRO_ADDR_CTRL2		(0x21)
#define GYRO_ADDR_CTRL3		(0x22)
#define GYRO_ADDR_CTRL4		(0x23)
#define GYRO_ADDR_CTRL5		(0x24)
#define GYRO_ADDR_REFERENCE	(0x25)
#define GYRO_ADDR_OUTTEMP	(0x26)
#define GYRO_ADDR_STATUS	(0x27)
#define GYRO_ADDR_OUTXLOW	(0x28)
#define GYRO_ADDR_OUTXHIGH	(0x29)
#define GYRO_ADDR_OUTYLOW	(0x2A)
#define GYRO_ADDR_OUTYHIGH	(0x2B)
#define GYRO_ADDR_OUTZLOW	(0x2C)
#define GYRO_ADDR_OUTZHIGH	(0x2D)
#define GYRO_ADDR_FIFOCTRL	(0x2E)
#define GYRO_ADDR_FIFOSRC	(0x2F)
#define GYRO_ADDR_IGCFG 	(0x30)
#define GYRO_ADDR_IGSRC 	(0x31)
#define GYRO_ADDR_IGTHSXH 	(0x32)
#define GYRO_ADDR_IGTHSXL 	(0x33)
#define GYRO_ADDR_IGTHSYH 	(0x34)
#define GYRO_ADDR_IGTHSYL 	(0x35)
#define GYRO_ADDR_IGTHSZH 	(0x36)
#define GYRO_ADDR_IGTHSZL 	(0x37)
#define GYRO_ADDR_IGURATION (0x38)
#define GYRO_ADDR_LOWODR	(0x39)

//*****************************************************************************
//		Global Functions
//*****************************************************************************
extern void Gyro_Init();
extern uint8_t 	Gyro_Read();

#endif /* GYROSCOPE_L3GD20H_H_ */
