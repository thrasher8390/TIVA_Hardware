/**
  ******************************************************************************
  * @file    servo_control.h
  * @author  Mike Barber
  * @date    21-February-2015
  * @brief   
  ******************************************************************************
*/
#ifndef __SERVO_CONTROL_H
#define __SERVO_CONTROL_H

/* Includes ------------------------------------------------------------------*/
#include "Project.h"

/* Public datatypes ----------------------------------------------------------*/
typedef enum
{
   MOTOR_X_MINUS = 0,
   MOTOR_X_PLUS,
   MOTOR_Y_MINUS,
   MOTOR_Y_PLUS,
   NUM_SERVO_MOTORS
} SERVO_NAME;

/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Public function prototypes ------------------------------------------------*/
extern void ServoModule_Init(void);
extern bool ServoModule_SetServoAngle(UINT8 servo, UINT16 angle);
extern void ServoModule_DeassertServoOutputs(UINT8 captureCompareNumTriggered);
extern void ServoModule_AssertServoOutputs(void);
extern void ServoModule_Fire(uint8_t numberToFire);

#endif /* __SERVO_CONTROL_H */
