/*
 * File:   ChinookTimer.h
 * Author: Frederic
 *
 * Created on 15 octobre 2014, 17:36
 */

#ifndef __CHINOOK_TIMER__
#define	__CHINOOK_TIMER__

#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>
#include "ChinookHardwareProfile.h"

#define TIMER_SCALE_S    1.0            // Sets the scale used for the timers' period in seconds
#define TIMER_SCALE_MS   0.001          // Sets the scale used for the timers' period in milliseconds
#define TIMER_SCALE_US   0.000001       // Sets the scale used for the timers' period in microseconds
#define TIMER_SCALE_NS   0.000000001    // Sets the scale used for the timers' period in nanoseconds


//==============================================================================
// Variable declarations
//==============================================================================

typedef enum
{
   TIMER_1
  ,TIMER_INPUT_CAPTURE    // Same as TIMER_2
  ,TIMER_2
  ,TIMER_PWM              // Same as TIMER_3
  ,TIMER_3
  ,TIMER_4
  ,TIMER_5
  ,TIMER_23     // 32 bits timer
  ,TIMER_45     // 32 bits timer
} TimerNum_t;


typedef enum
{
  PRESCALER_1   =   1
// ,PRESCALER_2   =   2     // Not on Timer1
// ,PRESCALER_4   =   4     // Not on Timer1
 ,PRESCALER_8   =   8
// ,PRESCALER_16  =   16    // Not on Timer1
// ,PRESCALER_32  =   32    // Not on Timer1
 ,PRESCALER_64  =   64
 ,PRESCALER_256 =   256
} TimerPrescaler_t;


typedef enum
{
   SCALE_S    // Not usable if Peripheral Clock > 10 MHz (80 MHz / 8)
  ,SCALE_MS
  ,SCALE_US
  ,SCALE_NS
} TimerScale_t;


struct sChinookTimerVar
{
  /**
   * @Description   Number of times the TimerX reached its max value. Incremented
   *                in the interrupt. Each index is the matrix corresponds to
   *                a different Timer (TIMER1 = index 0, TIMER2 = index 1, ...).
   *
   * @Example       UINT32 nOverflow = Timer.Var.nOverflows[0];\n
   *                Read the number of overflows that occured on TIMER_1.
   */
  volatile UINT32 nOverflows[5];


  /**
   * @Description   Time (in the scale that was entered in Timer.Open()) of the
   *                Timer. Each index is the matrix corresponds to
   *                a different Timer (TIMER1 = index 0, TIMER2 = index 1, ...).
   */
  UINT32 timerPeriodInTime[5];


  /**
   * @Description   Scale (ms, us, ns, ...) in which the timer was opened. Each
   *                index is the matrix corresponds toa different Timer
   *                (TIMER1 = index 0, TIMER2 = index 1, ...).
   */
  TimerScale_t timerScale[5];
};


//==============================================================================
// Public structure declaration
//==============================================================================

struct sChinookTimer
{

  /**
   * @Prototype   static inline INT8 TimerClose (TimerNum_t numTimer);
   *
   * @Description This function closes the Timer module specified by the argument.
   *
   * @Example     err = Timer.Close(TIMER_1);\n
   *              This example closes the Timer1.
   *
   * @Param       TimerNum_t numTimer\n
   *              TimerNum_t is an enum which can take the values TIMER_1,
   *              TIMER_INPUT_CAPTURE, TIMER_2, TIMER_PWM, TIMER_3, TIMER_4 and
   *              TIMER_5. TIMER_PWM is the same as TIMER_3 and TIMER_INPUT_CAPTURE
   *              is the same as TIMER_2.
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : timerNum is invalid.
   */
  INT8 (*Close) (TimerNum_t numTimer);


  /**
   * @Prototype   static inline UINT32 TimerTic(UINT32 period, TimerScale_t scale);
   *
   * @Description This function starts the Core Timer at the period entered in argument.
   *              The function Timer.Toc() must be used afterwards. DO NOT use
   *              the function Timer.DelayMs() between Tic and Toc as all these
   *              functions use the Core Timer.
   *
   * @Example     UINT32 coreTickRate = Timer.Tic(1500, SCALE_US);\n
   *              // ... Function ... //\n
   *              INT32 time = Timer.Toc(1500, coreTickRate);\n
   *              if (time "less than" 0)\n
   *              {\n
   *                // ... Error code ... //\n
   *              }\n
   *              else\n
   *              {\n
   *                // ... Good value code ... //\n
   *              }\n\n
   *              This example opens the Core Timer at a period of 1500 us and
   *              puts the time spent in variable "time".
   *
   * @Param       UINT32 period\n
   *              In the range of the time you presume you'll be acquiring.
   *
   * @Param       TimerScale_t scale\n
   *              The timer scale of the period.\n
   *              Possible values : \n
   *              SCALE_S, SCALE_MS, SCALE_US, SCALE_NS.\n
   *              NOTE : SCALE_S is the least precise.
   *
   * @Return      UINT32 coreTickRate\n
   *              Necessary for using function Timer.Toc().
   */
  UINT32 (*Tic) (UINT32 period, TimerScale_t scale);


  /**
   * @Prototype   static inline INT32 TimerToc (UINT32 period, UINT32 coreTickRate);
   *
   * @Description This function reads the current value of the Core Timer and
   *              closes it. It must be used AFTER Timer.Tic(). DO NOT use
   *              the function Timer.DelayMs() between Tic and Toc as all these
   *              functions use the Core Timer.
   *
   * @Example     UINT32 coreTickRate = Timer.Tic(1500, SCALE_US);\n
   *              // ... Function ... //\n
   *              INT32 time = Timer.Toc(1500, coreTickRate);\n
   *              if (time "less than" 0)\n
   *              {\n
   *                // ... Error code ... //\n
   *              }\n
   *              else\n
   *              {\n
   *                // ... Good value code ... //\n
   *              }\n\n
   *              This example opens the Core Timer at a period of 1500 us and
   *              puts the time spent in variable "time".
   *
   * @Param       UINT32 period\n
   *              The period used in Timer.Tic().
   *
   * @Param       UINT32 coreTickRate\n
   *              The value returned by Timer.Tic().
   *
   * @Return      INT32 time\n
   *              If the value returned is -1, an overflow has occured, which means
   *              that the chosen period is not long enough. Else, the function returns
   *              the time spent between the functions Timer.Tic() and Timer.Toc().
   *              The scale of the value is the same as entered in function Timer.Tic().
   */
  INT32 (*Toc) (UINT32 period, UINT32 coreTickRate);


  /**
   * @Prototype   static inline INT8 TimerOpenTimer1WithExtClk (UINT16 periodInCounts, UINT32 freqOfExtClkInHertz, TimerPrescaler_t prescaler);
   *
   * @Description This function opens the TIMER_1 using an external clock connected
   *              to the pin T1CK. The function Timer.EnableInterrupt(TIMER_1)
   *              still needs to be used afterwards.
   *
   * @Example     err = Timer.OpenTimer1WithExtClk(0xFFFF, 32768, PRESCALER_1);\n
   *              This example opens the Timer1 using an external clock of 32.768
   *              kHz. The timer will count to 0xFFFF before having an interrupt.
   *              The prescaler used in 1:1.
   *
   * @Param       UINT16 periodInCounts\n
   *              The max value you want to reach before generating an interrupt.\n
   *              Possible values : 0x1 to 0xFFFF.
   *
   * @Param       UINT32 freqOfExtClkInHertz\n
   *              The value in Hertz (Hz) of the external clock used.
   *
   * @Param       TimerPrescaler_t prescaler\n
   *              The prescaler you want to use.\n
   *              Possible values : \n
   *              PRESCALER_1 = prescaler of 1:1\n
   *              PRESCALER_8 = prescaler of 1:8\n
   *              PRESCALER_64 = prescaler of 1:64\n
   *              PRESCALER_256 = prescaler of 1:256
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : prescaler is invalid.
   */
  INT8 (*OpenTimer1WithExtClk) (UINT16 periodInCounts, UINT32 freqOfExtClkInHertz, TimerPrescaler_t prescaler);


  /**
   * @Prototype   static inline INT8 TimerReset (TimerNum_t numTimer);
   *
   * @Description This function writes zero (0) to the timer's register.
   *
   * @Example     err = Timer.Reset(TIMER_1);\n
   *              This example resets the Timer1.
   *
   * @Param       TimerNum_t numTimer\n
   *              TimerNum_t is an enum which can take the values TIMER_1,
   *              TIMER_INPUT_CAPTURE, TIMER_2, TIMER_PWM, TIMER_3, TIMER_4 and
   *              TIMER_5. TIMER_PWM is the same as TIMER_3 and TIMER_INPUT_CAPTURE
   *              is the same as TIMER_2.
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : timerNum is invalid.
   */
  INT8 (*Reset) (TimerNum_t numTimer);


  /**
   * @Prototype   static inline void TimerDelay (UINT32 delayMs);
   *
   * @Description This function creates a delay in milliseconds.
   *
   * @Example     err = Timer.Delay(TIMER_1);\n
   *              This example closes the Timer1.
   *
   * @Param       UINT32 delayMs\n
   *              number of milliseconds you want the delay to be.
   *
   * @Return      None.
   */
  void (*DelayMs) (UINT32 delayMs);


  /**
   * @Prototype   static inline INT8 TimerConfigInterrupt (TimerNum_t numTimer, UINT8 priority, UINT8 subPriority);
   *
   * @Description This function configures the interrupt of the Timer module specified
   *              by the argument numTimer to the priority and subpriority wanted
   *              WITHOUT ENABLING IT. To enable the interrupt, one must
   *              use the function Timer.EnableInterrupt after configuring it.
   *
   * @Example     err = Timer.ConfigInterrupt(TIMER_1, 1, 3);\n
   *              This example configures the interrupt for TIMER_1 with a priority
   *              of 1 and a subpriority of 3.
   *
   * @Param       TimerNum_t numTimer, UINT8 priority, UINT8 subPriority\n
   *              TimerNum_t is an enum which can take the values TIMER_1,
   *              TIMER_INPUT_CAPTURE, TIMER_2, TIMER_PWM, TIMER_3, TIMER_4 and
   *              TIMER_5. TIMER_PWM is the same as TIMER_3 and TIMER_INPUT_CAPTURE
   *              is the same as TIMER_2.\n\n
   *              priority is the desired priority of the interrupt to set. Its values have
   *              a range from 0 to 7, seven (7) being the highest priority and
   *              one (1) being the lowest priority. Note that a priority of zero (0)
   *              will disable the interrupt.\n\n
   *              subPriority is the desired subpriority of the interrupt to set.
   *              Its values have a range from 0 to 3, three (3) being the highest
   *              priority and zero (0) being the lowest priority.\n\n
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : one or more arguments are invalid.
   */
  INT8 (*ConfigInterrupt) (TimerNum_t numTimer, UINT8 priority, UINT8 subPriority);


  /**
   * @Prototype   static inline INT8 TimerDisableInterrupt (TimerNum_t numTimer);
   *
   * @Description This function quickly disables the interrupt of the Timer module
   *              specified by the argument numTimer.
   *
   * @Example     err = Timer.EnableInterrupt(TIMER_1);\n
   *              This example disables the interrupt for TIMER_1.
   *
   * @Param       TimerNum_t numTimer, UINT8 priority, UINT8 subPriority\n
   *              TimerNum_t is an enum which can take the values TIMER_1,
   *              TIMER_INPUT_CAPTURE, TIMER_2, TIMER_PWM, TIMER_3, TIMER_4 and
   *              TIMER_5. TIMER_PWM is the same as TIMER_3 and TIMER_INPUT_CAPTURE
   *              is the same as TIMER_2.
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : numTimer is invalid.
   */
  INT8 (*DisableInterrupt) (TimerNum_t numTimer);


  /**
   * @Prototype   static inline INT8 TimerEnableInterrupt (TimerNum_t numTimer);
   *
   * @Description This function quickly enables the interrupt of the Timer module
   *              specified by the argument numTimer.
   *
   * @Example     err = Timer.EnableInterrupt(TIMER_1);\n
   *              This example enables the interrupt for TIMER_1.
   *
   * @Param       TimerNum_t numTimer, UINT8 priority, UINT8 subPriority\n
   *              TimerNum_t is an enum which can take the values TIMER_1,
   *              TIMER_INPUT_CAPTURE, TIMER_2, TIMER_PWM, TIMER_3, TIMER_4 and
   *              TIMER_5. TIMER_PWM is the same as TIMER_3 and TIMER_INPUT_CAPTURE
   *              is the same as TIMER_2.
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : numTimer is invalid.
   */
  INT8 (*EnableInterrupt) (TimerNum_t numTimer);


  /**
   * @Prototype   static inline INT32 TimerOpen (TimerNum_t numTimer, UINT32 period, TimerScale_t timerScale);
   *
   * @Description This function opens the Timer module specified by the argument numTimer
   *
   * @Example     timerCounter = Timer.Open(TIMER_1, 500, SCALE_US);\n
   *              This example opens the Timer1 with a period of 500 us.
   *
   * @Param       TimerNum_t numTimer, UINT32 period, TimerScale_t timerScale\n
   *              TimerNum_t is an enum which can take the values TIMER_1,
   *              TIMER_INPUT_CAPTURE, TIMER_2, TIMER_PWM, TIMER_3, TIMER_4 and
   *              TIMER_5. TIMER_PWM is the same as TIMER_3 and TIMER_INPUT_CAPTURE
   *              is the same as TIMER_2.\n\n
   *              timerScale is the scale of the period. It can be SCALE_S for seconds,
   *              SCALE_MS for milliseconds, SCALE_US for microseconds and SCALE_NS
   *              for nanoseconds.\n\n
   *              period is the desired period of the timer in terms of timerScale.\n
   *              Possible values of the period are, for each timerScale : \n
   *              SCALE_S   : 1   (This is the slowest and least precise scale. This
   *              scale is also only usable if the PeripheralClock == 10 MHz)\n
   *              SCALE_MS  : 1 - 1677\n
   *              SCALE_US  : 1 - 1677683\n
   *              SCALE_NS  : 1 - 1677683200\n
   *              Note that the smaller the number (of the period), the more precise
   *              the timer will be, so choose your scaler in consequence.
   *
   * @Return      INT32\n
   *              On success : the timer counter value calculated\n
   *              On failure : -1\n
   *              Conditions of failure : timerNum is invalid, period is out
   *                                      of range, timerScale is invalid.
   */
  INT32 (*Open) (TimerNum_t numTimer, UINT32 period, TimerScale_t timerScale);


  /**
   * @Prototype   static inline INT64 TimerReadOverflows (TimerNum_t numTimer);
   *
   * @Description This function returns the number of overflows that occured in
   *              the timer.
   *
   * @Example     timer3Overflows = Timer.ReadOverflows(TIMER_3);\n
   *              This example returns the number of overflows of TIMER_3.
   *
   * @Param       TimerNum_t numTimer, UINT8 priority, UINT8 subPriority\n
   *              TimerNum_t is an enum which can take the values TIMER_1,
   *              TIMER_INPUT_CAPTURE, TIMER_2, TIMER_PWM, TIMER_3, TIMER_4 and
   *              TIMER_5. TIMER_PWM is the same as TIMER_3 and TIMER_INPUT_CAPTURE
   *              is the same as TIMER_2.
   *
   * @Return      INT64\n
   *              On success : a UINT32 containing the number of overflows\n
   *              On failure : -1\n
   *              Reason of failure : Wrong argument.
   */
  INT64 (*ReadOverflows) (TimerNum_t numTimer);


  /**
   * @Prototype   static inline TimerScale_t TimerReadTimerScale (TimerNum_t numTimer);
   *
   * @Description This function returns the SCALE in which the timer was opened.
   *
   * @Example     timer3Scale = Timer.ReadTimerScale(TIMER_3);\n
   *              This example returns the scale of TIMER_3.
   *
   * @Param       TimerNum_t numTimer, UINT8 priority, UINT8 subPriority\n
   *              TimerNum_t is an enum which can take the values TIMER_1,
   *              TIMER_INPUT_CAPTURE, TIMER_2, TIMER_PWM, TIMER_3, TIMER_4 and
   *              TIMER_5. TIMER_PWM is the same as TIMER_3 and TIMER_INPUT_CAPTURE
   *              is the same as TIMER_2.
   *
   * @Return      TimerScale_t\n
   *              The scale of the timer.
   */
  TimerScale_t (*ReadTimerScale) (TimerNum_t numTimer);


  /**
   * @Prototype   static inline UINT32 TimerReadPeriodInCounts (TimerNum_t numTimer);
   *
   * @Description This function reads the period of the timer specified by
   *              numTimer in counts. To have it in time (s, ms, us or ns), please
   *              ask the developpers.
   *
   * @Example     timer3Period = Timer.ReadPeriodInCounts(TIMER_3);\n
   *              This example returns the period of TIMER_3.
   *
   * @Param       TimerNum_t numTimer, UINT8 priority, UINT8 subPriority\n
   *              TimerNum_t is an enum which can take the values TIMER_1,
   *              TIMER_INPUT_CAPTURE, TIMER_2, TIMER_PWM, TIMER_3, TIMER_4 and
   *              TIMER_5. TIMER_PWM is the same as TIMER_3 and TIMER_INPUT_CAPTURE
   *              is the same as TIMER_2.
   *
   * @Return      UINT32\n
   *              The 16 or 32 bits value of the timer period.
   */
  UINT32 (*ReadPeriodInCounts) (TimerNum_t numTimer);


  /**
   * @Prototype   static inline UINT32 TimerReadPeriodInTime (TimerNum_t numTimer);
   *
   * @Description This function reads the period of the timer specified by
   *              numTimer in time and returns it in the scale in which the timer
   *              was opened.
   *
   * @Example     timer3Period = Timer.ReadPeriodInTimer(TIMER_3);\n
   *              This example returns the period in time of TIMER_3.
   *
   * @Param       TimerNum_t numTimer, UINT8 priority, UINT8 subPriority\n
   *              TimerNum_t is an enum which can take the values TIMER_1,
   *              TIMER_INPUT_CAPTURE, TIMER_2, TIMER_PWM, TIMER_3, TIMER_4 and
   *              TIMER_5. TIMER_PWM is the same as TIMER_3 and TIMER_INPUT_CAPTURE
   *              is the same as TIMER_2.
   *
   * @Return      UINT32\n
   *              The 16 or 32 bits value of the timer period
   */
  UINT32 (*ReadPeriodInTime) (TimerNum_t numTimer);


  /**
   * @Prototype   static inline INT32 TimerReadValue (TimerNum_t numTimer);
   *
   * @Description This function reads the current value of the timer specified by
   *              numTimer in counts. To have it in time (s, ms, us or ns), please
   *              ask the developpers.
   *
   * @Example     timer3Value = Timer.ReadValue(TIMER_3);\n
   *              This example returns the value of TIMER_3.
   *
   * @Param       TimerNum_t numTimer, UINT8 priority, UINT8 subPriority\n
   *              TimerNum_t is an enum which can take the values TIMER_1,
   *              TIMER_INPUT_CAPTURE, TIMER_2, TIMER_PWM, TIMER_3, TIMER_4 and
   *              TIMER_5. TIMER_PWM is the same as TIMER_3 and TIMER_INPUT_CAPTURE
   *              is the same as TIMER_2.
   *
   * @Return      INT32\n
   *              On success : the 16 bits value of the timer\n
   *              On failure : -1\n
   *              Conditions of failure : numTimer is invalid.
   */
  INT32 (*ReadValue) (TimerNum_t numTimer);


  /**
   * @Prototype   static inline INT8 TimerSetInterruptPriority (TimerNum_t numTimer, UINT8 priority);
   *
   * @Description This function sets the priority of the interrupt of the Timer module
   *              specified by the argument numTimer to the priority needed.
   *
   * @Example     err = Timer.SetIntPriority(TIMER_1, 2);\n
   *              This example sets the priority of the interrupt for TIMER_1 to 2.
   *
   * @Param       TimerNum_t numTimer, UINT8 priority, UINT8 subPriority\n
   *              TimerNum_t is an enum which can take the values TIMER_1,
   *              TIMER_INPUT_CAPTURE, TIMER_2, TIMER_PWM, TIMER_3, TIMER_4 and
   *              TIMER_5. TIMER_PWM is the same as TIMER_3 and TIMER_INPUT_CAPTURE
   *              is the same as TIMER_2.\n\n
   *              priority is the desired priority of the interrupt to set. Its values have
   *              a range from 0 to 7, seven (7) being the highest priority and
   *              one (1) being the lowest priority. Note that a priority of zero (0)
   *              will disable the interrupt.
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : one or more arguments are invalid.
   */
  INT8 (*SetIntPriority) (TimerNum_t numTimer, UINT8 priority);


  /**
   * @Description   Contains certains variables useful for the Timers.
   *                Write a dot "." after writing "Var" to see a description of
   *                the variables available.
   */
  struct sChinookTimerVar Var;

};

#endif	/* __CHINOOK_TIMER__ */
