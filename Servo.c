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
#include "Timers.h"

/* Private datatypes ---------------------------------------------------------*/

typedef enum
{
   ONE_EIGHTY_DEGREE = 0,  /*will only accept angles between 0x0000 and 0x7FFF*/
   THREE_SIXTY_DEGREE
} SERVO_TYPE;

typedef struct
{
   SERVO_NAME name;
   SERVO_TYPE type;
   UINT16 angle;
   UINT16 captureCompareVal;
   UINT32 port;
   UINT8 pin;
} SERVO_MOTOR;

typedef struct
{
   SERVO_MOTOR Motors[4];
} SERVO_MOTORS;

/* Private defines -----------------------------------------------------------*/
#define ONE_EIGHTY_DEGREES (0x7FFF)

//Defines for servo timing requirements
#define CLK_FREQUENCY                     (SYSTEM_CLOCK_FREQUENCY_HZ)
#define PRESCALER                         (0x000A)
#define SERVO_UPDATE_TIME_IN_MS           (27)
// NOTE: SERVO_UPDATE_TIME_IN_TIMER_TICKS < 0xFFFF
#define SERVO_UPDATE_TIME_IN_TIMER_TICKS  (ONE_MS_IN_TIMER_TICKS * SERVO_UPDATE_TIME_IN_MS)//(30118)//(SERVO_UPDATE_TIME_IN_MS*ONE_MS_IN_TIMER_TICKS)
#define NO_REPETITION                     (0)
#define ONE_MS_IN_TIMER_TICKS             (1455)//((CLK_FREQUENCY/PRESCALER)/1000) ///(1455)

#define TIMER_INTERRUPTS      (  TIM1_IT_Update | \
                                 TIM1_IT_CC1    | \
                                 TIM1_IT_CC2    | \
                                 TIM1_IT_CC3    | \
                                 TIM1_IT_CC4)

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
SERVO_MOTORS ServoMotors;

/* Private function prototypes -----------------------------------------------*/
void timerForServos_Init(void);
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
         (SERVO_MOTOR){MOTOR_X_MINUS,  THREE_SIXTY_DEGREE,  0x0000, 0, GPIO_PORTA_BASE, GPIO_PIN_2},
         (SERVO_MOTOR){MOTOR_X_PLUS,   THREE_SIXTY_DEGREE,  0x0000, 0, GPIO_PORTA_BASE, GPIO_PIN_3},
         (SERVO_MOTOR){MOTOR_Y_MINUS,  THREE_SIXTY_DEGREE,  0x0000, 0, GPIO_PORTA_BASE, GPIO_PIN_4},
         (SERVO_MOTOR){MOTOR_Y_PLUS,   THREE_SIXTY_DEGREE,  0x0000, 0, GPIO_PORTA_BASE, GPIO_PIN_5}
      };
  
   //send servos to their initial positions
   for(i = 0; i < NUM_SERVO_MOTORS; i++)
   {
      //initialize the appropriate outputs
      GPIOPinTypeGPIOOutput(ServoMotors.Motors[i].port, ServoMotors.Motors[i].pin);
      setServoOutput(&(ServoMotors.Motors[i]));
   }
}

/**
  * @brief  Set the angle for a specific servo motor
  * @param  SERVO_NAME servo
  *         UINT16 angle
  * @retval FALSE if the angle was invalid for the motor specified, otherwise TRUE
  */
bool ServoModule_SetServoAngle(UINT8 servo, UINT16 angle)
{
  bool retVal = FALSE;
  
  //only send the motor to the level if it is valid for this motor type
  if ((ServoMotors.Motors[servo].type == THREE_SIXTY_DEGREE) ||
      ((ServoMotors.Motors[servo].type == ONE_EIGHTY_DEGREE) &&
       (angle <= ONE_EIGHTY_DEGREES)))
  {
      ServoMotors.Motors[servo].angle = angle;
      setServoOutput(&ServoMotors.Motors[servo]);
      retVal = TRUE;
  }
  return retVal;
}

/**
  * @brief  Deassert the specified output
  * @param  
  * @retval 
  */
void ServoModule_DeassertServoOutputs(UINT8 captureCompareNumTriggered)
{
   GPIOPinWrite(ServoMotors.Motors[captureCompareNumTriggered].port, ServoMotors.Motors[captureCompareNumTriggered].pin, 0);
}

/**
  * @brief  Assert all of the servo outputs
  * @param  
  * @retval 
  */
void ServoModule_AssertServoOutputs(void)
{
   UINT8 i = 0;
   
   //assert all outputs
   for (i=0; i<NUM_SERVO_MOTORS; i++)
   {
      GPIOPinWrite(ServoMotors.Motors[i].port, ServoMotors.Motors[i].pin, 1);
   }   
}

/* Private function ----------------------------------------------------------*/


/**
  * @brief  Sets timer value for specified motor based on angle and motor type
  * @param  SERVO_MOTOR const * const motor
  * @retval 
  */
void setServoOutput(SERVO_MOTOR * const motor)
{
   UINT32 temp32 = 0;
   UINT16 captureCompareValue = 0;

   //scale up the angle
   temp32 = ((UINT32)motor->angle * (UINT32)ONE_MS_IN_TIMER_TICKS);   
   
   if (motor->type == ONE_EIGHTY_DEGREE)
   {
      //divide by 0x8000 by shifting 15
      captureCompareValue = (UINT16)(temp32 >> 15);
   }
   else
   {
      //divide by 0x10000 by shifting by 16
      captureCompareValue = (UINT16)(temp32 >> 16);
   }

   //the first mS is required to be high
   captureCompareValue += (UINT16)ONE_MS_IN_TIMER_TICKS;
   
   motor->captureCompareVal = captureCompareValue;

   //set the appropriate capture compare value
   switch (motor->name)
   {
      case MOTOR_X_MINUS:
      {
         //TimerMatchSet(TIMER1,TIMER_A, motor->captureCompareVal);
      }
      break;
      case MOTOR_X_PLUS:
      {

      }
      break;
      case MOTOR_Y_MINUS:
      {

      }
      break;
      case MOTOR_Y_PLUS:
      {

      }
      break;
      default:
      {
        //just something to breakpoint on
        //we should never reach this
        captureCompareValue = 0;
      }
      break;
   }

}
