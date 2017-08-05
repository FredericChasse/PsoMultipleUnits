//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// ChinookLib
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : ChinookInputCapture.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : TBD.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : TBD.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "..\headers\ChinookInputCapture.h"
#include "..\headers\ChinookTimer.h"


//==============================================================================
// Variable definitions
//==============================================================================
extern struct sChinookTimer Timer;


//==============================================================================
// Private functions prototypes
//==============================================================================
static inline INT8    InputCaptureOpen                    (InpCapModule_t inpCapModuleId, UINT16 config);
static inline INT8    InputCaptureClose                   (InpCapModule_t inpCapModuleId);
static inline INT8    InputCaptureConfigInterrupt         (InpCapModule_t inpCapModuleId, UINT16 intPriority, UINT16 intSubPriority);
static inline INT8    InputCaptureEnableInterrupt         (InpCapModule_t inpCapModuleId);
static inline INT8    InputCaptureDisableInterrupt        (InpCapModule_t inpCapModuleId);
static inline INT32   InputCaptureReadCapture             (InpCapModule_t inpCapModuleId);
static inline INT8    InputCaptureIsCaptureReady          (InpCapModule_t inpCapModuleId);
static inline INT64   InputCaptureGetTimeBetweenCaptures  (InpCapModule_t inpCapModuleId, TimerScale_t deltaScale);
static inline INT8    InputCaptureGetDirection            (InpCapModule_t inpCap1, InpCapModule_t inpCap2, INT64 timeBetweenCaptures, TimerScale_t scale);


//==============================================================================
// Public structure of functions for the I2C
//==============================================================================
struct sChinookInputCapture InputCapture =
{
   .Open                          = &InputCaptureOpen
  ,.Close                         = &InputCaptureClose
  ,.ConfigInterrupt               = &InputCaptureConfigInterrupt
  ,.EnableInterrupt               = &InputCaptureEnableInterrupt
  ,.DisableInterrupt              = &InputCaptureDisableInterrupt
  ,.GetDirection                  = &InputCaptureGetDirection
  ,.ReadCapture                   = &InputCaptureReadCapture
  ,.IsCaptureReady                = &InputCaptureIsCaptureReady
  ,.GetTimeBetweenCaptures        = &InputCaptureGetTimeBetweenCaptures
  ,.Var.currentCaptureCountValue  = {0}
  ,.Var.previousCaptureCountValue = {0}
  ,.Var.currentTimerOverflows     = {0}
  ,.Var.previousTimerOverflows    = {0}
  ,.Var.timerUsed                 = {TIMER_2}
};


//==============================================================================
// Private functions declarations
//==============================================================================

static inline INT8 InputCaptureGetDirection (InpCapModule_t inpCap1, InpCapModule_t inpCap2, INT64 timeBetweenCaptures, TimerScale_t scale)
{
  UINT32  previousCaptureTime
         ,currentCaptureTime
         ,previousTimerOverflows
         ,currentTimerOverflows
         ,tempTime
         ,offset
         ,captureDelta
         ;

  InpCapModule_t first
                ,second
                ;

  UINT16  timerPeriodinCounts
         ,timerPeriodinTime
         ;

  double  capturePrecision
         ,timerPrecision
         ;

  TimerScale_t timerScale;

  TimerNum_t numTimer
            ,numTimer2
            ;

  if (timeBetweenCaptures < 0)
  {
    return -1;
  }

  if ( (inpCap1 > IC5) || (inpCap2 > IC5) )
  {
    return -1;    // Wrong arguments
  }
  if (scale > SCALE_NS)
  {
    return -1;    // Wrong arguments
  }

  // Get necessary timer variables
  numTimer  = InputCapture.Var.timerUsed[inpCap1];
  numTimer2 = InputCapture.Var.timerUsed[inpCap2];
  if (numTimer != numTimer2)
  {
    return -1;    // Wrong timers used
  }

  // Get current values of captures
  previousCaptureTime     = InputCapture.Var.currentCaptureCountValue[inpCap1];
  currentCaptureTime      = InputCapture.Var.currentCaptureCountValue[inpCap2];
  previousTimerOverflows  = InputCapture.Var.currentTimerOverflows   [inpCap1];
  currentTimerOverflows   = InputCapture.Var.currentTimerOverflows   [inpCap2];

  if (currentTimerOverflows > previousTimerOverflows)
  {
    first  = inpCap1;
    second = inpCap2;
  }
  else if (currentTimerOverflows == previousTimerOverflows)
  {
    if (currentCaptureTime >= previousCaptureTime)
    {
      first  = inpCap1;
      second = inpCap2;
    }
    else
    {
      tempTime = previousCaptureTime;
      previousCaptureTime = currentCaptureTime;
      currentCaptureTime = tempTime;

      first  = inpCap2;
      second = inpCap1;
    }
  }
  else
  {
    tempTime = previousTimerOverflows;
    previousTimerOverflows = currentTimerOverflows;
    currentTimerOverflows = tempTime;

    tempTime = previousCaptureTime;
    previousCaptureTime = currentCaptureTime;
    currentCaptureTime = tempTime;

    first  = inpCap2;
    second = inpCap1;
  }


  timerPeriodinCounts = Timer.ReadPeriodInCounts(numTimer);
  timerPeriodinTime   = Timer.ReadPeriodInTime  (numTimer);

  offset = timerPeriodinCounts * (currentTimerOverflows - previousTimerOverflows);

  timerScale = Timer.ReadTimerScale(numTimer);

  switch (scale)
  {
    case SCALE_S    :   capturePrecision = TIMER_SCALE_S;
                        break;

    case SCALE_MS   :   capturePrecision = TIMER_SCALE_MS;
                        break;

    case SCALE_US   :   capturePrecision = TIMER_SCALE_US;
                        break;

    case SCALE_NS   :   capturePrecision = TIMER_SCALE_NS;
                        break;

    default         :   return -1;    // Wrong argument
  }

  switch (timerScale)
  {
    case SCALE_S    :   timerPrecision = TIMER_SCALE_S;
                        break;

    case SCALE_MS   :   timerPrecision = TIMER_SCALE_MS;
                        break;

    case SCALE_US   :   timerPrecision = TIMER_SCALE_US;
                        break;

    case SCALE_NS   :   timerPrecision = TIMER_SCALE_NS;
                        break;

    default         :   return -1;    // Wrong argument
  }

  if (scale == timerScale)
  {
    captureDelta = (((float)(offset + currentCaptureTime - previousCaptureTime))/timerPeriodinCounts) * timerPeriodinTime + 0.5;
  }
  else
  {
    captureDelta = (((float)(offset + currentCaptureTime - previousCaptureTime))/timerPeriodinCounts) * timerPeriodinTime * (timerPrecision / capturePrecision) + 0.5;
  }

  if (captureDelta < timeBetweenCaptures * 2 / 5)
  {
    return first;   // EXIT_SUCCESS
  }
  else
  {
    return second;   // EXIT_SUCCESS
  }
}


static inline INT64 InputCaptureGetTimeBetweenCaptures (InpCapModule_t inpCapModuleId, TimerScale_t deltaScale)
{
  UINT32  previousCaptureTime
         ,currentCaptureTime
         ,previousTimerOverflows
         ,currentTimerOverflows
         ,offset
         ,captureSpeed
         ;

  UINT16  timerPeriodinCounts
         ,timerPeriodinTime
         ;

  double  capturePrecision
         ,timerPrecision
         ;

  TimerScale_t timerScale;

  TimerNum_t numTimer;

  if (inpCapModuleId > IC5)
  {
    return -1;    // Wrong arguments
  }
  if (deltaScale > SCALE_NS)
  {
    return -1;    // Wrong arguments
  }

  // Get current values of captures
  previousCaptureTime     = InputCapture.Var.previousCaptureCountValue[inpCapModuleId];
  currentCaptureTime      = InputCapture.Var.currentCaptureCountValue [inpCapModuleId];
  previousTimerOverflows  = InputCapture.Var.previousTimerOverflows   [inpCapModuleId];
  currentTimerOverflows   = InputCapture.Var.currentTimerOverflows    [inpCapModuleId];

  // Get necessary timer variables
  numTimer = InputCapture.Var.timerUsed[inpCapModuleId];

  timerPeriodinCounts = Timer.ReadPeriodInCounts(numTimer);
  timerPeriodinTime   = Timer.ReadPeriodInTime(numTimer);

  offset = timerPeriodinCounts * (currentTimerOverflows - previousTimerOverflows);

  timerScale = Timer.ReadTimerScale(numTimer);

  switch (deltaScale)
  {
    case SCALE_S    :   capturePrecision = TIMER_SCALE_S;
                        break;

    case SCALE_MS   :   capturePrecision = TIMER_SCALE_MS;
                        break;

    case SCALE_US   :   capturePrecision = TIMER_SCALE_US;
                        break;

    case SCALE_NS   :   capturePrecision = TIMER_SCALE_NS;
                        break;

    default         :   return -1;    // Wrong argument
  }

  switch (timerScale)
  {
    case SCALE_S    :   timerPrecision = TIMER_SCALE_S;
                        break;

    case SCALE_MS   :   timerPrecision = TIMER_SCALE_MS;
                        break;

    case SCALE_US   :   timerPrecision = TIMER_SCALE_US;
                        break;

    case SCALE_NS   :   timerPrecision = TIMER_SCALE_NS;
                        break;

    default         :   return -1;    // Wrong argument
  }

  if (deltaScale == timerScale)
  {
    captureSpeed = (((float)(offset + currentCaptureTime - previousCaptureTime))/timerPeriodinCounts) * timerPeriodinTime + 0.5;
  }
  else
  {
    captureSpeed = (((float)(offset + currentCaptureTime - previousCaptureTime))/timerPeriodinCounts) * timerPeriodinTime * (timerPrecision / capturePrecision) + 0.5;
  }

  return captureSpeed;   // EXIT_SUCCESS
}


static inline INT8 InputCaptureOpen (InpCapModule_t inpCapModuleId, UINT16 config)
{
  if (config & IC_CAP_32BIT)
  {
    InputCapture.Var.timerUsed[inpCapModuleId] = TIMER_23;
  }
  else
  {
    if (config & IC_TIMER2_SRC)
    {
      InputCapture.Var.timerUsed[inpCapModuleId] = TIMER_2;
    }
    else // if (config & IC_TIMER3_SRC)
    {
      InputCapture.Var.timerUsed[inpCapModuleId] = TIMER_3;
    }
  }
  
  switch (inpCapModuleId)
  {
    case IC1    :   OpenCapture1(config);
                    break;
    case IC2    :   OpenCapture2(config);
                    break;
    case IC3    :   OpenCapture3(config);
                    break;
    case IC4    :   OpenCapture4(config);
                    break;
    case IC5    :   OpenCapture5(config);
                    break;
    default     :   return -1;  // EXIT_FAILURE
  }


  return 0;   // EXIT_SUCCESS
}


static inline INT8 InputCaptureClose (InpCapModule_t inpCapModuleId)
{
  switch (inpCapModuleId)
  {
    case IC1    :   CloseCapture1();
                    break;
    case IC2    :   CloseCapture2();
                    break;
    case IC3    :   CloseCapture3();
                    break;
    case IC4    :   CloseCapture4();
                    break;
    case IC5    :   CloseCapture5();
                    break;
    default     :   return -1;  // EXIT_FAILURE
  }

  return 0;   // EXIT_SUCCESS
}


static inline INT8 InputCaptureConfigInterrupt (InpCapModule_t inpCapModuleId, UINT16 intPriority, UINT16 intSubPriority)
{
  if (intPriority > 7)
  {
    return -1;
  }

  if (intSubPriority > 4)
  {
    return -1;
  }

  switch (inpCapModuleId)
  {
    case IC1    :   ConfigIntCapture1(IC_INT_OFF | intPriority | (intSubPriority << 3));
                    break;
    case IC2    :   ConfigIntCapture2(IC_INT_OFF | intPriority | (intSubPriority << 3));
                    break;
    case IC3    :   ConfigIntCapture3(IC_INT_OFF | intPriority | (intSubPriority << 3));
                    break;
    case IC4    :   ConfigIntCapture4(IC_INT_OFF | intPriority | (intSubPriority << 3));
                    break;
    case IC5    :   ConfigIntCapture5(IC_INT_OFF | intPriority | (intSubPriority << 3));
                    break;
    default     :   return -1;  // EXIT_FAILURE
  }

  return 0;   // EXIT_SUCCESS
}


static inline INT8 InputCaptureDisableInterrupt (InpCapModule_t inpCapModuleId)
{
  switch (inpCapModuleId)
  {
    case IC1    :   DisableIntIC1;
                    break;
    case IC2    :   DisableIntIC2;
                    break;
    case IC3    :   DisableIntIC3;
                    break;
    case IC4    :   DisableIntIC4;
                    break;
    case IC5    :   DisableIntIC5;
                    break;
    default     :   return -1;  // EXIT_FAILURE
  }

  return 0;   // EXIT_SUCCESS
}


static inline INT8 InputCaptureEnableInterrupt (InpCapModule_t inpCapModuleId)
{
  switch (inpCapModuleId)
  {
    case IC1    :   EnableIntIC1;
                    break;
    case IC2    :   EnableIntIC2;
                    break;
    case IC3    :   EnableIntIC3;
                    break;
    case IC4    :   EnableIntIC4;
                    break;
    case IC5    :   EnableIntIC5;
                    break;
    default     :   return -1;  // EXIT_FAILURE
  }

  return 0;   // EXIT_SUCCESS
}


static inline INT8 InputCaptureIsCaptureReady (InpCapModule_t inpCapModuleId)
{
  switch (inpCapModuleId)
  {
    case IC1    :   return mIC1CaptureReady();
                    break;
    case IC2    :   return mIC2CaptureReady();
                    break;
    case IC3    :   return mIC3CaptureReady();
                    break;
    case IC4    :   return mIC4CaptureReady();
                    break;
    case IC5    :   return mIC5CaptureReady();
                    break;
    default     :   return -1;  // EXIT_FAILURE
  }

  return 0;   // EXIT_SUCCESS
}


static inline INT32 InputCaptureReadCapture (InpCapModule_t inpCapModuleId)
{
  switch (inpCapModuleId)
  {
    case IC1    :   return mIC1ReadCapture();
                    break;
    case IC2    :   return mIC2ReadCapture();
                    break;
    case IC3    :   return mIC3ReadCapture();
                    break;
    case IC4    :   return mIC4ReadCapture();
                    break;
    case IC5    :   return mIC5ReadCapture();
                    break;
    default     :   return -1;  // EXIT_FAILURE
  }

  return 0;   // EXIT_SUCCESS
}
