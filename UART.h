/*
 * UART.h
 *
 *  Created on: Sep 25, 2014
 *      Author: DLThrasher
 */

#ifndef UART_H_
#define UART_H_
//*****************************************************************************
//		Includes
//*****************************************************************************
#include "Project.h"

//*****************************************************************************
//		Global Functions
//*****************************************************************************

extern void UART_Initialize(void);

extern void UART3IntHandler(void);
extern void UART_Send(const uint8_t *pui8Buffer, uint32_t ui32Count);
#endif /* UART_H_ */
