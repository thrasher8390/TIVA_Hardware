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
//    Defines
//*****************************************************************************
#define I2C_INT_MASK (I2C_MASTER_INT_DATA)

//*****************************************************************************
//    Data Types
//*****************************************************************************
typedef enum
{
   I2C_MULTREAD__START,
   I2C_MULTREAD__SEND_REGISTER,
   I2C_MULTREAD__CONTINUE_READ,
   I2C_MULTREAD__END_READ
}I2C_MULTREAD_STATES;

typedef struct
{
    BOOLEAN MultiReadActive;
    I2C_MULTREAD_STATES State;
    UINT16 Address;
    UINT16 Register;
    UINT8* DataLocation;
    UINT32 DataLocationLength;
    int (*CallbackFunction)(void);
}I2CMultReadStruct;

//*****************************************************************************
//		Global Functions
//*****************************************************************************
extern void       I2C_Init0();
extern uint32_t   I2C_Read0(uint16_t , uint16_t );
extern void       I2C_Read0Multiiple(uint16_t device_address, uint16_t device_register, uint8_t *startofread, uint32_t numBytes, int (*getNextValue)(void));
extern void       I2C_Write0(uint16_t , uint16_t , uint8_t );
extern BOOLEAN    I2C_WRITEVERIFY0(uint16_t , uint16_t , uint8_t );
extern void       I2C_Interrupt0(void);

#endif /* I2C_H_ */
