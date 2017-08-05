/*
 * File:   ChinookPorts.h
 * Author: Frederic
 *
 * Created on 15 octobre 2014, 17:36
 */

#ifndef __CHINOOK_PWM__
#define	__CHINOOK_PWM__

#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>
#include "ChinookHardwareProfile.h"


#define PWM_PRECISION_DIVIDER   1000.0        // Sets the precision of the % used (1000 = 0.1%, 100 = 1%, 10 = 10%, etc.)

typedef enum
{
     PWM_1
    ,PWM_2
    ,PWM_3
    ,PWM_4
    ,PWM_5
} PwmNum_t;


struct sChinookPwm
{

  /**
   * @Prototype   static inline INT8 PwmClose (PwmNum_t pwmNum);
   *
   * @Description This function closes the PWM module specified by the argument
   *
   * @Example     err = Pwm.Close( PWM_1 );\n
   *              This example closes the PWM_1
   *
   * @Param       PwmNum_t pwmNum\n
   *              PwmNum_t is an enum which can take the values PWM_1,
   *              PWM_2, PWM_3, PWM_4 and PWM_5
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : pwmNum is invalid
   */
  INT8 (*Close) (PwmNum_t pwmNum);


  /**
   * @Prototype   static inline INT8 PwmOpen (PwmNum_t pwmNum);
   *
   * @Description This function opens the PWM module specified by the argument with
   *              the following parameters : \n
   *                Use Timer3 as a 16 bits timer\n
   *                Mode continuous pulse\n
   *                Duty cycle = 50%\n
   *                Offset = 0%\n\n
   *              The fact that each PWM module use Timer3 implies that Timer3
   *              must be set using the Timer functions before using the PWM functions.
   *
   * @Example     err = Pwm.Open( PWM_1 );\n
   *              This example opens the PWM_1
   *
   * @Param       PwmNum_t pwmNum\n
   *              PwmNum_t is an enum which can take the values PWM_1,
   *              PWM_2, PWM_3, PWM_4 and PWM_5
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : pwmNum is invalid
   */
  INT8 (*Open) (PwmNum_t pwmNum);


  /**
   * @Prototype   static inline INT8 PwmReadDutyCycle (PwmNum_t pwmNum);
   *
   * @Description This function reads the duty cycle of the PWM module specified
   *              by the argument
   *
   * @Example     dutyCycle = Pwm.ReadDutyCycle( PWM_1 );\n
   *              This example reads the duty cycle of the PWM_1
   *
   * @Param       PwmNum_t pwmNum\n
   *              PwmNum_t is an enum which can take the values PWM_1,
   *              PWM_2, PWM_3, PWM_4 and PWM_5
   *
   * @Return      INT32\n
   *              On success : The function will return the duty cycle expressed
   *                           in percentage of the Timer3 period. Also, to add
   *                           precision and avoid to pass floats in the argument,
   *                           the value has been multiplied by a factor of 10.
   *                           This means that a duty cycle of 25.5% will return 255.\n
   *              On failure : -1\n
   *              Conditions of failure : pwmNum is invalid
   */
  INT32 (*ReadDutyCycle) (PwmNum_t pwmNum);


  /**
   * @Prototype   static inline INT8 PwmReadPulseOffset (PwmNum_t pwmNum);
   *
   * @Description This function reads the offset of the PWM module specified
   *              by the argument
   *
   * @Example     offset = Pwm.ReadPulseOffset( PWM_1 );\n
   *              This example reads the offset of the PWM_1
   *
   * @Param       PwmNum_t pwmNum\n
   *              PwmNum_t is an enum which can take the values PWM_1,
   *              PWM_2, PWM_3, PWM_4 and PWM_5
   *
   * @Return      INT32\n
   *              On success : The function will return the offset expressed
   *                           in percentage of the Timer3 period. Also, to add
   *                           precision and avoid to pass floats in the argument,
   *                           the value has been multiplied by a factor of 10.
   *                           This means that an offset of 25.5% will return 255.\n
   *              On failure : -1\n
   *              Conditions of failure : pwmNum is invalid
   */
  INT32 (*ReadPulseOffset) (PwmNum_t pwmNum);


  /**
   * @Prototype   static inline INT8 PwmSetDutyCycle (PwmNum_t pwmNum, UINT16 dutyCycle);
   *
   * @Description This function sets the duty cycle of the PWM module specified
   *              by the argument
   *
   * @Example     err = Pwm.SetDutyCycle( PWM_1, 555 );\n
   *              This example sets the duty cycle of the PWM_1 to 55.5%
   *
   * @Param       PwmNum_t pwmNum, UINT16 dutyCycle\n
   *              PwmNum_t is an enum which can take the values PWM_1,
   *              PWM_2, PWM_3, PWM_4 and PWM_5\n\n
   *              dutyCycle is the duty cycle desired with a precision of 0.1%.
   *              To achieve this precision and avoid passing floats in function
   *              arguments, the percentage needed has to be multiplied by 10. This
   *              means that a desired duty cycle of 48.6% must be entered as 486.
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : pwmNum is invalid
   */
  INT8 (*SetDutyCycle) (PwmNum_t pwmNum, UINT16 dutyCycle);


  /**
   * @Prototype   static inline INT8 PwmSetPulseOffset (PwmNum_t pwmNum, UINT16 pulseOffset);
   *
   * @Description This function sets the offset of the PWM module specified
   *              by the argument
   *
   * @Example     err = Pwm.SetPulseOffset( PWM_1, 255 );\n
   *              This example sets the duty cycle of the PWM_1 to 25.5%
   *
   * @Param       PwmNum_t pwmNum, UINT16 pulseOffset\n
   *              PwmNum_t is an enum which can take the values PWM_1,
   *              PWM_2, PWM_3, PWM_4 and PWM_5\n\n
   *              pulseOffset is the offset desired with a precision of 0.1%.
   *              To achieve this precision and avoid passing floats in function
   *              arguments, the percentage needed has to be multiplied by 10. This
   *              means that a desired offset of 25.8% must be entered as 258.
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : pwmNum is invalid
   */
  INT8 (*SetPulseOffset) (PwmNum_t pwmNum, UINT16 pulseOffset);

};


#endif	/* __CHINOOK_PWM__ */

