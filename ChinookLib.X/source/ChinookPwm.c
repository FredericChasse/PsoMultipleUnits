#include "..\headers\ChinookPwm.h"


/*
 * Private functions prototypes for the PWM
 */
static inline INT8  PwmClose            (PwmNum_t pwmNum);

static inline INT8  PwmOpen             (PwmNum_t pwmNum);

static inline INT32 PwmReadDutyCycle    (PwmNum_t pwmNum);

static inline INT32 PwmReadPulseOffset  (PwmNum_t pwmNum);

static inline INT8  PwmSetDutyCycle     (PwmNum_t pwmNum, UINT16 dutyCycle);

static inline INT8  PwmSetPulseOffset   (PwmNum_t pwmNum, UINT16 pulseOffset);


/*
 * Public structure of functions for the PWM
 */

struct sChinookPwm Pwm =
{
   .Close             = &PwmClose
  ,.Open              = &PwmOpen
  ,.ReadDutyCycle     = &PwmReadDutyCycle
  ,.ReadPulseOffset   = &PwmReadPulseOffset
  ,.SetDutyCycle      = &PwmSetDutyCycle
  ,.SetPulseOffset    = &PwmSetPulseOffset
};


/*
 * Declarations of private functions
 */

static inline INT8 PwmClose (PwmNum_t pwmNum)
{
  switch (pwmNum)
  {
    case PWM_1  :   CloseOC1();
                    return 0;
                    break;

    case PWM_2  :   CloseOC2();
                    return 0;
                    break;

    case PWM_3  :   CloseOC3();
                    return 0;
                    break;

    case PWM_4  :   CloseOC4();
                    return 0;
                    break;

    case PWM_5  :   CloseOC5();
                    return 0;
                    break;

    default     :   return -1;    // Invalid pwmNum
                    break;
  }
}

static inline INT8 PwmOpen (PwmNum_t pwmNum)
{
  switch (pwmNum)
  {
    case PWM_1  :   OpenOC1( OC_ON | OC_TIMER_MODE16 | OC_TIMER3_SRC | OC_CONTINUE_PULSE , ReadPeriod3() / 2, 0);
                    return 0;
                    break;

    case PWM_2  :   OpenOC2( OC_ON | OC_TIMER_MODE16 | OC_TIMER3_SRC | OC_CONTINUE_PULSE , ReadPeriod3() / 2, 0);
                    return 0;
                    break;

    case PWM_3  :   OpenOC3( OC_ON | OC_TIMER_MODE16 | OC_TIMER3_SRC | OC_CONTINUE_PULSE , ReadPeriod3() / 2, 0);
                    return 0;
                    break;

    case PWM_4  :   OpenOC4( OC_ON | OC_TIMER_MODE16 | OC_TIMER3_SRC | OC_CONTINUE_PULSE , ReadPeriod3() / 2, 0);
                    return 0;
                    break;

    case PWM_5  :   OpenOC5( OC_ON | OC_TIMER_MODE16 | OC_TIMER3_SRC | OC_CONTINUE_PULSE , ReadPeriod3() / 2, 0);
                    return 0;
                    break;

    default     :   return -1;    // Invalid pwmNum
                    break;
  }
}

static inline INT32 PwmReadDutyCycle (PwmNum_t pwmNum)
{
  switch (pwmNum)
  {
    case PWM_1  :   return (INT32) ((ReadRegOC1(1) - ReadRegOC1(0)) * PWM_PRECISION_DIVIDER / ReadPeriod3() + 0.5);
                    break;

    case PWM_2  :   return (INT32) ((ReadRegOC2(1) - ReadRegOC2(0)) * PWM_PRECISION_DIVIDER / ReadPeriod3() + 0.5);
                    break;

    case PWM_3  :   return (INT32) ((ReadRegOC3(1) - ReadRegOC3(0)) * PWM_PRECISION_DIVIDER / ReadPeriod3() + 0.5);
                    break;

    case PWM_4  :   return (INT32) ((ReadRegOC4(1) - ReadRegOC4(0)) * PWM_PRECISION_DIVIDER / ReadPeriod3() + 0.5);
                    break;

    case PWM_5  :   return (INT32) ((ReadRegOC5(1) - ReadRegOC5(0)) * PWM_PRECISION_DIVIDER / ReadPeriod3() + 0.5);
                    break;

    default     :   return -1;    // Invalid pwmNum
                    break;
  }
}

static inline INT32 PwmReadPulseOffset (PwmNum_t pwmNum)
{
  switch (pwmNum)
  {
    case PWM_1  :   return (INT32) (ReadRegOC1(0) * PWM_PRECISION_DIVIDER / ReadPeriod3() + 0.5);
                    break;

    case PWM_2  :   return (INT32) (ReadRegOC2(0) * PWM_PRECISION_DIVIDER / ReadPeriod3() + 0.5);
                    break;

    case PWM_3  :   return (INT32) (ReadRegOC3(0) * PWM_PRECISION_DIVIDER / ReadPeriod3() + 0.5);
                    break;

    case PWM_4  :   return (INT32) (ReadRegOC4(0) * PWM_PRECISION_DIVIDER / ReadPeriod3() + 0.5);
                    break;

    case PWM_5  :   return (INT32) (ReadRegOC5(0) * PWM_PRECISION_DIVIDER / ReadPeriod3() + 0.5);
                    break;

    default     :   return -1;    // Invalid pwmNum
                    break;
  }
}

static inline INT8 PwmSetDutyCycle (PwmNum_t pwmNum, UINT16 dutyCycle)
{
  UINT16  pulseStart
         ,pulseStop
         ,timerPeriod
         ;

  timerPeriod = ReadPeriod3();

  if ( (dutyCycle > PWM_PRECISION_DIVIDER) || (dutyCycle < 0) )   // If dutyCycle is not between 0% and 100%
  {
    return -1;                                  // Exit function
  }

  pulseStart = Pwm.ReadPulseOffset(pwmNum) * timerPeriod / PWM_PRECISION_DIVIDER;

  pulseStop = ((UINT16) (dutyCycle * timerPeriod / PWM_PRECISION_DIVIDER + pulseStart + 0.5)) % timerPeriod;

  switch (pwmNum)
  {
    case PWM_1  :   SetPulseOC1(pulseStart, pulseStop);
                    return 0;
                    break;

    case PWM_2  :   SetPulseOC2(pulseStart, pulseStop);
                    return 0;
                    break;

    case PWM_3  :   SetPulseOC3(pulseStart, pulseStop);
                    return 0;
                    break;

    case PWM_4  :   SetPulseOC4(pulseStart, pulseStop);
                    return 0;
                    break;

    case PWM_5  :   SetPulseOC5(pulseStart, pulseStop);
                    return 0;
                    break;

    default     :   return -1;    // Invalid pwmNum
                    break;
  }

  return 0;   // Return SUCCESS
}

static inline INT8 PwmSetPulseOffset (PwmNum_t pwmNum, UINT16 pulseOffset)
{
  UINT32  pulseStart
         ,previousStart
         ,pulseStop
         ,timerPeriod
         ;
  
  if ( (pulseOffset > PWM_PRECISION_DIVIDER) || (pulseOffset < 0) )   // If pulseOffset is not between 0% and 100%
  {
    return -1;                                      // Exit function
  }

  timerPeriod   = ReadPeriod3();

  previousStart = Pwm.ReadPulseOffset(pwmNum) * timerPeriod / PWM_PRECISION_DIVIDER;

  pulseStart    = (UINT32) pulseOffset * timerPeriod / PWM_PRECISION_DIVIDER;


  /*
   *  The following piece of code adjusts the pulse stop depending on the
   *  offset needed and the timer period. Then, it adpjusts the pulse start and
   *  stop of the PWM module needed.
   *  Example :
   *  timerPeriod = 0xFFFF
   *  dutyCycle   = 80%
   *  pulseOffset = 25% -> pulseStart = 0xFFFF / 4 = 0x3FFF
   *  pulseStop   = pulseOffset + 0xFFFF * 0.8 = 0x10CCB
   *  pulseStop > timerPeriod => pulseStop = pulseStop % timerPeriod = 0x0CCC
   *
   */


  switch (pwmNum)
  {
    case PWM_1  :   pulseStop  = ReadRegOC1(1);

                    pulseStop = (pulseStop - previousStart + pulseStart) % timerPeriod;

                    SetPulseOC1(pulseStart, pulseStop);

                    return 0;

                    break;

    case PWM_2  :   pulseStop  = ReadRegOC2(1);

                    pulseStop = (pulseStop - previousStart + pulseStart) % timerPeriod;

                    SetPulseOC2(pulseStart, pulseStop);

                    return 0;

                    break;

    case PWM_3  :   pulseStop  = ReadRegOC3(1);

                    pulseStop = (pulseStop - previousStart + pulseStart) % timerPeriod;

                    SetPulseOC3(pulseStart, pulseStop);

                    return 0;

                    break;

    case PWM_4  :   pulseStop  = ReadRegOC4(1);

                    pulseStop = (pulseStop - previousStart + pulseStart) % timerPeriod;

                    SetPulseOC4(pulseStart, pulseStop);

                    return 0;

                    break;

    case PWM_5  :   pulseStop  = ReadRegOC5(1);

                    pulseStop = (pulseStop - previousStart + pulseStart) % timerPeriod;

                    SetPulseOC5(pulseStart, pulseStop);

                    return 0;

                    break;

    default     :   return -1;    // Invalid pwmNum
                    break;
  }
}