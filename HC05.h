/*
 * HC05.h
 *
 *  Created on: Feb 2, 2015
 *      Author: DLThrasher
 */

#ifndef HC05_H_
#define HC05_H_

//*****************************************************************************
//    Includes
//*****************************************************************************
#include "Project.h"

//Structs
typedef struct
{
  uint8_t CMD;
  uint8_t CMD_SUB;
  uint8_t LENGTH;
  uint8_t DATA[6];
}BLUETOOTH_CMD;

typedef enum
{
   CMD = 0,
   CMD_SUB = 1,
   LENGTH = 2,
   END_OF_HEADER_INDEX = LENGTH,
   BEGINNING_OF_DATA_INDEX = 3
}cmdIndexes;

#define INVALID_CMD (0xFF)

//Functions
extern void HC05_Initialize(void);
extern void HC05__RxInterrupt(void);
extern BLUETOOTH_CMD HC05__GetCommand(void);
#endif /* HC05_H_ */
