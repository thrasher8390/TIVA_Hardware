/*
 * I2C.h
 *
 *  Created on: Nov 17, 2014
 *      Author: DLThrasher
 */

#ifndef I2C_H_
#define I2C_H_
//*****************************************************************************
//		Includes
//*****************************************************************************
#include "Project.h"
#include "inc/hw_i2c.h"
#include "driverlib/i2c.h"

//*****************************************************************************
//		Global Functions
//*****************************************************************************
extern void       I2C_Init0();
extern uint32_t   I2C_Read0(uint16_t , uint16_t );
extern void       I2C_Read0Multiiple(uint16_t device_address, uint16_t device_register, uint8_t *startofread, uint32_t numBytes);
extern void       I2C_Write0(uint16_t , uint16_t , uint8_t );
extern BOOLEAN    I2C_WRITEVERIFY0(uint16_t , uint16_t , uint8_t );


#endif /* I2C_H_ */
