#include "..\headers\ChinookTimer.h"


/*
 * Private functions prototypes for the Timer
 */

static inline INT8          TimerClose                    (TimerNum_t numTimer);
static inline INT8          TimerReset                    (TimerNum_t numTimer);
static inline void          TimerDelayMs                  (UINT32 delay_ms);
static inline INT8          TimerConfigInterrupt          (TimerNum_t numTimer, UINT8 priority, UINT8 subPriority);
static inline INT8          TimerDisableInterrupt         (TimerNum_t numTimer);
static inline INT8          TimerEnableInterrupt          (TimerNum_t numTimer);
static inline INT32         TimerOpen                     (TimerNum_t numTimer, UINT32 period, TimerScale_t timerScale);
static inline TimerScale_t  TimerReadTimerScale           (TimerNum_t numTimer);
static inline UINT32        TimerReadPeriodInCounts       (TimerNum_t numTimer);
static inline UINT32        TimerReadPeriodInTime         (TimerNum_t numTimer);
static inline INT32         TimerReadValue                (TimerNum_t numTimer);
static inline INT64         TimerReadOverflows            (TimerNum_t numTimer);
static inline INT8          TimerSetInterruptPriority     (TimerNum_t numTimer, UINT8 priority);
static inline INT8          TimerOpenTimer1WithExtClk     (UINT16 periodInCounts, UINT32 freqOfExtClkInHertz, TimerPrescaler_t prescaler);
static inline INT8          TimerTic                      (void);
static inline INT64         TimerToc                      (void);

/*
 * Public structure of functions for the Timer
 */

struct sChinookTimer Timer =
{
   .Close                 = &TimerClose
  ,.Reset                 = &TimerReset
  ,.OpenTimer1WithExtClk  = &TimerOpenTimer1WithExtClk
  ,.DelayMs               = &TimerDelayMs
  ,.ConfigInterrupt       = &TimerConfigInterrupt
  ,.DisableInterrupt      = &TimerDisableInterrupt
  ,.EnableInterrupt       = &TimerEnableInterrupt
  ,.Open                  = &TimerOpen
  ,.ReadPeriodInCounts    = &TimerReadPeriodInCounts
  ,.ReadPeriodInTime      = &TimerReadPeriodInTime
  ,.ReadTimerScale        = &TimerReadTimerScale
  ,.ReadValue             = &TimerReadValue
  ,.ReadOverflows         = &TimerReadOverflows
  ,.SetIntPriority        = &TimerSetInterruptPriority
  ,.Tic                   = &TimerTic
  ,.Toc                   = &TimerToc
  ,.Var.nOverflows        = {0}
  ,.Var.timerPeriodInTime = {0}
  ,.Var.timerScale        = {SCALE_US}
};


/*
 * Declarations of private functions
 */


static inline INT8 TimerClose(TimerNum_t numTimer)
{
  switch(numTimer)
  {
    case TIMER_1                :   CloseTimer1();
                                    break;

    case TIMER_INPUT_CAPTURE    :   // Same as TIMER_2
    case TIMER_2                :   CloseTimer2();
                                    break;

    case TIMER_PWM              :   // Same as TIMER_3
    case TIMER_3                :   CloseTimer3();
                                    break;

    case TIMER_4                :   CloseTimer4();
                                    break;

    case TIMER_5                :   CloseTimer5();
                                    break;

    default                     :   return -1;    // Wrong numTimer value
  }

  return 0;   // EXIT SUCCESS
}


static inline INT8 TimerReset(TimerNum_t numTimer)
{
  switch(numTimer)
  {
    case TIMER_1                :   WriteTimer1(0);
                                    break;

    case TIMER_INPUT_CAPTURE    :   // Same as TIMER_2
    case TIMER_2                :   WriteTimer2(0);
                                    break;

    case TIMER_PWM              :   // Same as TIMER_3
    case TIMER_3                :   WriteTimer3(0);
                                    break;

    case TIMER_4                :   WriteTimer4(0);
                                    break;

    case TIMER_5                :   WriteTimer5(0);
                                    break;

    default                     :   return -1;    // Wrong numTimer value
  }

  return 0;   // EXIT SUCCESS
}


static inline void TimerDelayMs(UINT32 delayMs)
{
  UINT32 coreTickRate = CORE_TICK_RATE * TIMER_SCALE_MS * delayMs;
  
  OpenCoreTimer(coreTickRate);
  while (!mCTGetIntFlag());
  mCTClearIntFlag();
  CloseCoreTimer();
}


BOOL _oTicInvoked = 0;
static inline INT8 TimerTic()
{
  
  if (_oTicInvoked)
  {
    return -1;
  }
  
  _oTicInvoked = 1;

  mCTClearIntFlag();
  WriteCoreTimer(0);
  OpenCoreTimer(0xFFFFFFFF);

  return 0;
}


static inline INT64 TimerToc()
{
  if (!_oTicInvoked)
  {
    return -1;
  }
  _oTicInvoked = 0;
  
  INT64 tok = ReadCoreTimer();

  if (mCTGetIntFlag())  // An overflow has occured, period is too small
  {
    CloseCoreTimer();
    mCTClearIntFlag();
    return -2;
  }
  else
  {
    CloseCoreTimer();
    return tok * 25;
  }
}


static inline INT8 TimerConfigInterrupt (TimerNum_t numTimer, UINT8 priority, UINT8 subPriority)
{
  /* Matrix of interrupt priorities */
  UINT16  prio[5][8]    = {
                            { T1_INT_PRIOR_0, T1_INT_PRIOR_1, T1_INT_PRIOR_2, T1_INT_PRIOR_3, T1_INT_PRIOR_4, T1_INT_PRIOR_5, T1_INT_PRIOR_6, T1_INT_PRIOR_7 }
                           ,{ T2_INT_PRIOR_0, T2_INT_PRIOR_1, T2_INT_PRIOR_2, T2_INT_PRIOR_3, T2_INT_PRIOR_4, T2_INT_PRIOR_5, T2_INT_PRIOR_6, T2_INT_PRIOR_7 }
                           ,{ T3_INT_PRIOR_0, T3_INT_PRIOR_1, T3_INT_PRIOR_2, T3_INT_PRIOR_3, T3_INT_PRIOR_4, T3_INT_PRIOR_5, T3_INT_PRIOR_6, T3_INT_PRIOR_7 }
                           ,{ T4_INT_PRIOR_0, T4_INT_PRIOR_1, T4_INT_PRIOR_2, T4_INT_PRIOR_3, T4_INT_PRIOR_4, T4_INT_PRIOR_5, T4_INT_PRIOR_6, T4_INT_PRIOR_7 }
                           ,{ T5_INT_PRIOR_0, T5_INT_PRIOR_1, T5_INT_PRIOR_2, T5_INT_PRIOR_3, T5_INT_PRIOR_4, T5_INT_PRIOR_5, T5_INT_PRIOR_6, T5_INT_PRIOR_7 }
                          };

  /* Matrix of interrupt sub-priorities */
  UINT16  subPrio[5][4] = {
                            { T1_INT_SUB_PRIOR_0, T1_INT_SUB_PRIOR_1, T1_INT_SUB_PRIOR_2, T1_INT_SUB_PRIOR_3 }
                           ,{ T2_INT_SUB_PRIOR_0, T2_INT_SUB_PRIOR_1, T2_INT_SUB_PRIOR_2, T2_INT_SUB_PRIOR_3 }
                           ,{ T3_INT_SUB_PRIOR_0, T3_INT_SUB_PRIOR_1, T3_INT_SUB_PRIOR_2, T3_INT_SUB_PRIOR_3 }
                           ,{ T4_INT_SUB_PRIOR_0, T4_INT_SUB_PRIOR_1, T4_INT_SUB_PRIOR_2, T4_INT_SUB_PRIOR_3 }
                           ,{ T5_INT_SUB_PRIOR_0, T5_INT_SUB_PRIOR_1, T5_INT_SUB_PRIOR_2, T5_INT_SUB_PRIOR_3 }
                          };

  if ( (priority > 7) || (subPriority > 3) )
  {
    return -1;    // Wrong arguments
  }

  switch(numTimer)
  {
    case TIMER_1                :   ConfigIntTimer1(T1_INT_OFF | prio[0][priority] | subPrio[0][subPriority]);
                                    break;

    case TIMER_INPUT_CAPTURE    :   // Same as TIMER_2
    case TIMER_2                :   ConfigIntTimer2(T2_INT_OFF | prio[1][priority] | subPrio[1][subPriority]);
                                    break;

    case TIMER_PWM              :   // Same as TIMER_3
    case TIMER_3                :   ConfigIntTimer3(T3_INT_OFF | prio[2][priority] | subPrio[2][subPriority]);
                                    break;

    case TIMER_4                :   ConfigIntTimer4(T4_INT_OFF | prio[3][priority] | subPrio[3][subPriority]);
                                    break;

    case TIMER_5                :   ConfigIntTimer5(T5_INT_OFF | prio[4][priority] | subPrio[4][subPriority]);
                                    break;

    default                     :   return -1;    // Wrong numTimer value
  }

  return 0;
}


static inline INT8 TimerDisableInterrupt (TimerNum_t numTimer)
{
  switch(numTimer)
  {
    case TIMER_1                :   DisableIntT1;
                                    break;

    case TIMER_INPUT_CAPTURE    :   // Same as TIMER_2
    case TIMER_2                :   DisableIntT2;
                                    break;

    case TIMER_PWM              :   // Same as TIMER_3
    case TIMER_3                :   DisableIntT3;
                                    break;

    case TIMER_4                :   DisableIntT4;
                                    break;

    case TIMER_5                :   DisableIntT5;
                                    break;

    default                     :   return -1;    // Wrong numTimer value
  }

  return 0;   // EXIT SUCCESS
}


static inline INT8 TimerEnableInterrupt (TimerNum_t numTimer)
{
  switch(numTimer)
  {
    case TIMER_1                :   EnableIntT1;
                                    break;

    case TIMER_INPUT_CAPTURE    :   // Same as TIMER_2
    case TIMER_2                :   EnableIntT2;
                                    break;

    case TIMER_PWM              :   // Same as TIMER_3
    case TIMER_3                :   EnableIntT3;
                                    break;

    case TIMER_4                :   EnableIntT4;
                                    break;

    case TIMER_5                :   EnableIntT5;
                                    break;

    default                     :   return -1;    // Wrong numTimer value
  }

  return 0;   // EXIT SUCCESS
}

static inline INT8 TimerOpenTimer1WithExtClk (UINT16 periodInCounts, UINT32 freqOfExtClkInHertz, TimerPrescaler_t prescaler)
{
  UINT16 ps;

  switch (prescaler)
  {
    case PRESCALER_1 :
      ps = T1_PS_1_1;
      break;

    case PRESCALER_8 :
      ps = T1_PS_1_8;
      break;

    case PRESCALER_64 :
      ps = T1_PS_1_64;
      break;

    case PRESCALER_256 :
      ps = T1_PS_1_256;
      break;

    default :
      return -1;
  }

  Timer.Close(TIMER_1);

  T1CONbits.TSYNC = 0;

  Timer.Reset(TIMER_1);

  Timer.Var.timerPeriodInTime[0] = (periodInCounts + 1) / freqOfExtClkInHertz;

  Timer.Var.timerScale[0] = SCALE_S;
  
  OpenTimer1(T1_ON | T1_SOURCE_EXT | ps, periodInCounts);

  return 0;   // EXIT_SUCCESS
}


static inline INT32 TimerOpen(TimerNum_t numTimer, UINT32 period, TimerScale_t timerScale)
{

  UINT8   clockRatio    = 0;    // Peripheral clock ratio to 10MHz (80MHz = 8, 40MHz = 4, 20MHz = 2 and 10MHz = 1;

  UINT16  timerPeriod   = 0
         ,t1PS          = 1
         ,t2PS          = 1
         ,t3PS          = 1
         ,t4PS          = 1
         ,t5PS          = 1
         ;

  TimerPrescaler_t prescaler = PRESCALER_1;

  UINT32 peripheralClock;

  if (period == 0)
  {
    return -1;      // Wrong period value
  }

  peripheralClock = GetPeripheralClock();

  clockRatio = peripheralClock / 10e6 + 0.5;    // Ratio to 10 MHz

  /* Mettre exemple de calcul */

  /*
   * solve ( 65535 >= PeripheralClock * period * timerScale / prescaler + 0.5, period )
   *                  PeripheralClock = SystemClock / FPBDIV = 80MHz / 8 = 10MHz
   *                  2^16 - 1 = 65535
   */

  switch (timerScale)
  {
    case SCALE_S    :   if ( (period > 1) || (clockRatio != 1) );
                        {
                          return -1;      // timerPeriod will be higher than 65535
                        }

                        prescaler = PRESCALER_256;

                        timerPeriod = (UINT16) (peripheralClock * TIMER_SCALE_S * period / prescaler + 0.5);

                        t1PS = T1_PS_1_256;
                        t2PS = T2_PS_1_256;
                        t3PS = T3_PS_1_256;
                        t4PS = T4_PS_1_256;
                        t5PS = T5_PS_1_256;

                        break;

    case SCALE_MS   :   if ( period <= ( 6 / clockRatio ) )
                        {
                          prescaler = PRESCALER_1;
                          t1PS = T1_PS_1_1;
                          t2PS = T2_PS_1_1;
                          t3PS = T3_PS_1_1;
                          t4PS = T4_PS_1_1;
                          t5PS = T5_PS_1_1;
                        }
                        else if ( period <= ( 52 / clockRatio ) )
                        {
                          prescaler = PRESCALER_8;
                          t1PS = T1_PS_1_8;
                          t2PS = T2_PS_1_8;
                          t3PS = T3_PS_1_8;
                          t4PS = T4_PS_1_8;
                          t5PS = T5_PS_1_8;
                        }
                        else if ( period <= ( 419 / clockRatio ) )
                        {
                          prescaler = PRESCALER_64;
                          t1PS = T1_PS_1_64;
                          t2PS = T2_PS_1_64;
                          t3PS = T3_PS_1_64;
                          t4PS = T4_PS_1_64;
                          t5PS = T5_PS_1_64;
                        }
                        else if ( period <= ( 1677 / clockRatio ) )
                        {
                          prescaler = PRESCALER_256;
                          t1PS = T1_PS_1_256;
                          t2PS = T2_PS_1_256;
                          t3PS = T3_PS_1_256;
                          t4PS = T4_PS_1_256;
                          t5PS = T5_PS_1_256;
                        }
                        else
                        {
                          return -1;      // timerPeriod will be higher than 65535 (2^16 - 1)
                        }

                        timerPeriod = (UINT16) (peripheralClock * TIMER_SCALE_MS * period / prescaler + 0.5);

                        break;

    case SCALE_US   :   if ( period <= ( 6553 / clockRatio ) )
                        {
                          prescaler = PRESCALER_1;
                          t1PS = T1_PS_1_1;
                          t2PS = T2_PS_1_1;
                          t3PS = T3_PS_1_1;
                          t4PS = T4_PS_1_1;
                          t5PS = T5_PS_1_1;
                        }
                        else if ( period <= ( 52427 / clockRatio ) )
                        {
                          prescaler = PRESCALER_8;
                          t1PS = T1_PS_1_8;
                          t2PS = T2_PS_1_8;
                          t3PS = T3_PS_1_8;
                          t4PS = T4_PS_1_8;
                          t5PS = T5_PS_1_8;
                        }
                        else if ( period <= ( 419421 / clockRatio ) )
                        {
                          prescaler = PRESCALER_64;
                          t1PS = T1_PS_1_64;
                          t2PS = T2_PS_1_64;
                          t3PS = T3_PS_1_64;
                          t4PS = T4_PS_1_64;
                          t5PS = T5_PS_1_64;
                        }
                        else if ( period <= ( 1677683 / clockRatio ) )
                        {
                          prescaler = PRESCALER_256;
                          t1PS = T1_PS_1_256;
                          t2PS = T2_PS_1_256;
                          t3PS = T3_PS_1_256;
                          t4PS = T4_PS_1_256;
                          t5PS = T5_PS_1_256;
                        }
                        else
                        {
                          return -1;      // timerPeriod will be higher than 65535 (2^16 - 1)
                        }

                        timerPeriod = (UINT16) (peripheralClock * TIMER_SCALE_US * period / prescaler + 0.5);

                        break;

    case SCALE_NS   :   if ( period <= ( 6553450 / clockRatio ) )
                        {
                          prescaler = PRESCALER_1;
                          t1PS = T1_PS_1_1;
                          t2PS = T2_PS_1_1;
                          t3PS = T3_PS_1_1;
                          t4PS = T4_PS_1_1;
                          t5PS = T5_PS_1_1;
                        }
                        else if ( period <= ( 52427600 / clockRatio ) )
                        {
                          prescaler = PRESCALER_8;
                          t1PS = T1_PS_1_8;
                          t2PS = T2_PS_1_8;
                          t3PS = T3_PS_1_8;
                          t4PS = T4_PS_1_8;
                          t5PS = T5_PS_1_8;
                        }
                        else if ( period <= ( 419420800 / clockRatio ) )
                        {
                          prescaler = PRESCALER_64;
                          t1PS = T1_PS_1_64;
                          t2PS = T2_PS_1_64;
                          t3PS = T3_PS_1_64;
                          t4PS = T4_PS_1_64;
                          t5PS = T5_PS_1_64;
                        }
                        else if ( period <= ( 1677683200 / clockRatio ) )
                        {
                          prescaler = PRESCALER_256;
                          t1PS = T1_PS_1_256;
                          t2PS = T2_PS_1_256;
                          t3PS = T3_PS_1_256;
                          t4PS = T4_PS_1_256;
                          t5PS = T5_PS_1_256;
                        }
                        else
                        {
                          return -1;      // timerPeriod will be higher than 65535 (2^16 - 1)
                        }

                        timerPeriod = (UINT16) (peripheralClock * TIMER_SCALE_NS * period / prescaler + 0.5);

                        break;

    default         :   return -1;    // Wrong value for timerScale
  }
  
  switch(numTimer)
  {
    case TIMER_1                :   OpenTimer1(T1_ON | T1_SOURCE_INT | t1PS, timerPeriod);
                                    Timer.Var.timerPeriodInTime[0] = period;
                                    Timer.Var.timerScale[0] = timerScale;
                                    break;

    case TIMER_INPUT_CAPTURE    :   // Same as TIMER_2
    case TIMER_2                :   OpenTimer2(T2_ON | T2_SOURCE_INT | t2PS, timerPeriod);
                                    Timer.Var.timerPeriodInTime[1] = period;
                                    Timer.Var.timerScale[1] = timerScale;
                                    break;

    case TIMER_PWM              :   // Same as TIMER_3
    case TIMER_3                :   OpenTimer3(T3_ON | T3_SOURCE_INT | t3PS, timerPeriod);
                                    Timer.Var.timerPeriodInTime[2] = period;
                                    Timer.Var.timerScale[2] = timerScale;
                                    break;

    case TIMER_4                :   OpenTimer4(T4_ON | T4_SOURCE_INT | t4PS, timerPeriod);
                                    Timer.Var.timerPeriodInTime[3] = period;
                                    Timer.Var.timerScale[3] = timerScale;
                                    break;

    case TIMER_5                :   OpenTimer5(T5_ON | T5_SOURCE_INT | t5PS, timerPeriod);
                                    Timer.Var.timerPeriodInTime[4] = period;
                                    Timer.Var.timerScale[4] = timerScale;
                                    break;

    default                     :   return -1;    // Wrong numTimer value
  }

  return timerPeriod;   // Exit SUCCESS
}


static inline TimerScale_t TimerReadTimerScale (TimerNum_t numTimer)
{

  TimerScale_t timerScale;

  switch(numTimer)
  {
    case TIMER_1                :   timerScale = Timer.Var.timerScale[0];
                                    break;

    case TIMER_INPUT_CAPTURE    :   // Same as TIMER_2
    case TIMER_2                :   timerScale = Timer.Var.timerScale[1];
                                    break;

    case TIMER_PWM              :   // Same as TIMER_3
    case TIMER_3                :   timerScale = Timer.Var.timerScale[2];
                                    break;

    case TIMER_4                :   timerScale = Timer.Var.timerScale[3];
                                    break;

    case TIMER_5                :   timerScale = Timer.Var.timerScale[4];
                                    break;
  }

  return timerScale;    // EXIT SUCCESS
}


static inline UINT32 TimerReadPeriodInCounts (TimerNum_t numTimer)
{

  UINT32 period;

  switch(numTimer)
  {
    case TIMER_1                :   period = ReadPeriod1();
                                    break;

    case TIMER_INPUT_CAPTURE    :   // Same as TIMER_2
    case TIMER_2                :   period = ReadPeriod2();
                                    break;

    case TIMER_PWM              :   // Same as TIMER_3
    case TIMER_3                :   period = ReadPeriod3();
                                    break;

    case TIMER_4                :   period = ReadPeriod4();
                                    break;

    case TIMER_5                :   period = ReadPeriod5();
                                    break;
  }

  return period;    // EXIT SUCCESS
}


static inline UINT32 TimerReadPeriodInTime (TimerNum_t numTimer)
{

  UINT32 period;

  switch(numTimer)
  {
    case TIMER_1                :   period = Timer.Var.timerPeriodInTime[0];
                                    break;

    case TIMER_INPUT_CAPTURE    :   // Same as TIMER_2
    case TIMER_2                :   period = Timer.Var.timerPeriodInTime[1];
                                    break;

    case TIMER_PWM              :   // Same as TIMER_3
    case TIMER_3                :   period = Timer.Var.timerPeriodInTime[2];
                                    break;

    case TIMER_4                :   period = Timer.Var.timerPeriodInTime[3];
                                    break;

    case TIMER_5                :   period = Timer.Var.timerPeriodInTime[4];
                                    break;
  }

  return period;    // EXIT SUCCESS
}


static inline INT64 TimerReadOverflows (TimerNum_t numTimer)
{

  UINT32 timerOverflows;

  switch(numTimer)
  {
    case TIMER_1                :   timerOverflows = Timer.Var.nOverflows[0];
                                    break;

    case TIMER_INPUT_CAPTURE    :   // Same as TIMER_2
    case TIMER_2                :   timerOverflows = Timer.Var.nOverflows[1];
                                    break;

    case TIMER_PWM              :   // Same as TIMER_3
    case TIMER_3                :   timerOverflows = Timer.Var.nOverflows[2];
                                    break;

    case TIMER_4                :   timerOverflows = Timer.Var.nOverflows[3];
                                    break;

    case TIMER_5                :   timerOverflows = Timer.Var.nOverflows[4];
                                    break;

    default                     :   return -1;    // Wrong numTimer value
  }

  return timerOverflows;    // EXIT SUCCESS
}


static inline INT32 TimerReadValue (TimerNum_t numTimer)
{

  INT32 value;

  switch(numTimer)
  {
    case TIMER_1                :   value = ReadTimer1();
                                    break;

    case TIMER_INPUT_CAPTURE    :   // Same as TIMER_2
    case TIMER_2                :   value = ReadTimer2();
                                    break;

    case TIMER_PWM              :   // Same as TIMER_3
    case TIMER_3                :   value = ReadTimer3();
                                    break;

    case TIMER_4                :   value = ReadTimer4();
                                    break;

    case TIMER_5                :   value = ReadTimer5();
                                    break;

    default                     :   return -1;    // Wrong numTimer value
  }

  return value;    // EXIT SUCCESS
}


static inline INT8 TimerSetInterruptPriority (TimerNum_t numTimer, UINT8 priority)
{
  /* Matrix of interrupt priorities */
  UINT16  prio[5][8]    = {
                            { T1_INT_PRIOR_0, T1_INT_PRIOR_1, T1_INT_PRIOR_2, T1_INT_PRIOR_3, T1_INT_PRIOR_4, T1_INT_PRIOR_5, T1_INT_PRIOR_6, T1_INT_PRIOR_7 }
                           ,{ T2_INT_PRIOR_0, T2_INT_PRIOR_1, T2_INT_PRIOR_2, T2_INT_PRIOR_3, T2_INT_PRIOR_4, T2_INT_PRIOR_5, T2_INT_PRIOR_6, T2_INT_PRIOR_7 }
                           ,{ T3_INT_PRIOR_0, T3_INT_PRIOR_1, T3_INT_PRIOR_2, T3_INT_PRIOR_3, T3_INT_PRIOR_4, T3_INT_PRIOR_5, T3_INT_PRIOR_6, T3_INT_PRIOR_7 }
                           ,{ T4_INT_PRIOR_0, T4_INT_PRIOR_1, T4_INT_PRIOR_2, T4_INT_PRIOR_3, T4_INT_PRIOR_4, T4_INT_PRIOR_5, T4_INT_PRIOR_6, T4_INT_PRIOR_7 }
                           ,{ T5_INT_PRIOR_0, T5_INT_PRIOR_1, T5_INT_PRIOR_2, T5_INT_PRIOR_3, T5_INT_PRIOR_4, T5_INT_PRIOR_5, T5_INT_PRIOR_6, T5_INT_PRIOR_7 }
                          };

  if (priority > 7)
  {
    return -1;    // Wrong argument
  }

  switch(numTimer)
  {
    case TIMER_1                :   SetPriorityIntT1(prio[0][priority]);
                                    break;

    case TIMER_INPUT_CAPTURE    :   // Same as TIMER_2
    case TIMER_2                :   SetPriorityIntT2(prio[1][priority]);
                                    break;

    case TIMER_PWM              :   // Same as TIMER_3
    case TIMER_3                :   SetPriorityIntT3(prio[2][priority]);
                                    break;

    case TIMER_4                :   SetPriorityIntT4(prio[3][priority]);
                                    break;

    case TIMER_5                :   SetPriorityIntT5(prio[4][priority]);
                                    break;

    default                     :   return -1;    // Wrong numTimer value
  }

  return 0;   // Exit SUCCESS
}
