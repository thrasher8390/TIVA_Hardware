/**
   ******************************************************************************
   * @file    servo_control.c
   * @author  Mike Barber
   * @date    21-February-2015
   * @brief   
   ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "Servo.h"
#include "pwm.h"
#include "Scheduler.h"

/* Private datatypes ---------------------------------------------------------*/
typedef struct
{
   SERVO_NAME name;
   UINT32 powerUS; /*!< Can be between 0-1000us*/
   UINT32 pwmModule;
   UINT32 pmwOutput;
} SERVO_MOTOR;

typedef struct
{
   SERVO_MOTOR Motors[4];
} SERVO_MOTORS;

/* Private defines -----------------------------------------------------------*/
#define MAX_POWER_US (1000)

//Defines for servo timing requirements
#define SERVO_UPDATE_TIME_IN_MS           (27)

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
SERVO_MOTORS ServoMotors;

/* Private function prototypes -----------------------------------------------*/
void setServoOutput(SERVO_MOTOR * const motor); 
/* Public functions-----------------------------------------------------------*/

/**
  * @brief  Initialize the Servo Module
  * @param  None
  * @retval None
  */
void ServoModule_Init(void)
{
   UINT8 i;
   
   //initialize motor array
   ServoMotors = 
      (SERVO_MOTORS){
         (SERVO_MOTOR){MOTOR_X_MINUS, 0, PWM0, PWM_OUT_0},
         (SERVO_MOTOR){MOTOR_X_PLUS,  0, PWM0, PWM_OUT_1},
         (SERVO_MOTOR){MOTOR_Y_MINUS, 0, PWM0, PWM_OUT_2},
         (SERVO_MOTOR){MOTOR_Y_PLUS,  0, PWM0, PWM_OUT_3}
      };

   //Init the motor signals
   PWM_Initialize();

   //send servos to their initial positions
   for(i = 0; i < NUM_SERVO_MOTORS; i++)
   {
      //Set to High End
      ServoMotors.Motors[i].powerUS = 800;
      //initialize the appropriate outputs
      setServoOutput(&(ServoMotors.Motors[i]));
   }
   Scheduler__SetResetCountValue(SCHEDULER__SERVO,SCHEDULER_SECONDS(2));
   while(Scheduler__GetTimerState(SCHEDULER__SERVO) == SCHEDULER_STATE__RUNNING);

   //send servos to their initial positions
   for(i = 0; i < NUM_SERVO_MOTORS; i++)
   {
      //Set to Low End
      ServoMotors.Motors[i].powerUS = 0;
      //initialize the appropriate outputs
      setServoOutput(&(ServoMotors.Motors[i]));
   }

}

/**
  * @brief  Set the angle for a specific servo motor
  * @param  SERVO_NAME servo
  *         UINT16 angle
  * @retval FALSE if the angle was invalid for the motor specified, otherwise TRUE
  */
bool ServoModule_SetServoPower(UINT8 servo, UINT16 powerUS)
{
  bool retVal = FALSE;
  
  //only send the motor to the level if it is valid for this motor type
  if (powerUS <= MAX_POWER_US)
  {
      ServoMotors.Motors[servo].powerUS = powerUS;
      setServoOutput(&ServoMotors.Motors[servo]);
      retVal = TRUE;
  }

  return retVal;
}

/**
  * @brief  Set the angle for a specific servo motor
  * @param  SERVO_NAME servo
  *         UINT16 angle
  * @retval FALSE if the angle was invalid for the motor specified, otherwise TRUE
  */
UINT32 ServoModule_GetServoPower(UINT8 servo)
{
   return ServoMotors.Motors[servo].powerUS;
}

/* Private function ----------------------------------------------------------*/

/**
  * @brief  Sets timer value for specified motor based on angle and motor type
  * @param  SERVO_MOTOR const * const motor
  * @retval 
  */
void setServoOutput(SERVO_MOTOR * const motor)
{
   //0 power = 1000us
   UINT32 power = PWM_US(1000) + PWM_US(motor->powerUS);

   PWMPulseWidthSet(motor->pwmModule, motor->pmwOutput , power);
}
