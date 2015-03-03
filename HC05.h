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
  uint8_t LENGTH;
  uint8_t DATA[6];
}BLUETOOTH_CMD;

typedef enum
{
   CMD = 0,
   LENGTH = 1,
   END_OF_HEADER_INDEX = LENGTH,
   BEGINNING_OF_DATA_INDEX = 2
}cmdIndexes;

//Functions
extern void HC05_Initialize(void);
extern void HC05__RxInterrupt(void);
extern BLUETOOTH_CMD HC05__GetCommand(void);
#endif /* HC05_H_ */
