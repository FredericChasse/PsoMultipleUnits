//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// MFC Polarization
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Polarization.c
// Author  : Frederic Chasse
// Date    : 2017-09-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the methods for creating a polarization curve
//           of MFCs.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "Polarization.h"
#include "Potentiometer.h"
#include "MathFunctions.h"
#include "UnitMapping.h"


// Private definitions
//==============================================================================

typedef enum
{
  POLAR_FORWARD
 ,POLAR_BACKWARD
 ,POLAR_STOP
} PolarDirection_t;

typedef struct
{
  UnitArrayInterface_t *unitArray;
  float sampleTime;
  UINT32 nSamplesForAcq;
  UINT32 currentSample;
  float timeElapsed;
  UINT8 nUnits;
  UINT16 currentPosIdx[N_UNITS_TOTAL];
  float currentPos[N_UNITS_TOTAL];
  UINT16 posIncrement;
  UINT16 minPosIdx[N_UNITS_TOTAL];
  UINT16 maxPosIdx[N_UNITS_TOTAL];
  float  minPos[N_UNITS_TOTAL];
  float  maxPos[N_UNITS_TOTAL];
  PolarDirection_t direction[N_UNITS_TOTAL];
  UINT16 finalPosIdx[N_UNITS_TOTAL];
  float  finalPos[N_UNITS_TOTAL];
} Polar_t;

// Private prototypes
//==============================================================================

INT8  _Polar_Init            (Polar_t *p, UnitArrayInterface_t *unitArray);
INT8  _Polar_Run             (Polar_t *p);
float _Polar_GetTimeElapsed  (Polar_t *p);
void  _Polar_Release         (Polar_t *p);
void  _Polar_GetDebugData    (Polar_t *p, void *ret);

// Private variables
//==============================================================================

Polar_t _polar = 
{
  .unitArray      = NULL
 ,.sampleTime     = SAMPLING_TIME_FLOAT
 ,.nSamplesForAcq = N_LARGE_SAMPLES_FOR_ALGO
 ,.currentSample  = 0
 ,.timeElapsed    = 0
 ,.nUnits         = 0
 ,.currentPosIdx  = {POT_MIN_INDEX}
 ,.currentPos     = {MIN_POT_VALUE}
 ,.posIncrement   = 4
 ,.minPosIdx      = {POT_MIN_INDEX}
 ,.maxPosIdx      = {POT_MAX_INDEX}
 ,.minPos         = {MIN_POT_VALUE}
 ,.maxPos         = {MAX_POT_VALUE}
 ,.direction      = {POLAR_FORWARD}
 ,.finalPos       = {MIN_POT_VALUE}
 ,.finalPosIdx    = {POT_MIN_INDEX}
};

const AlgoInterface_t _polar_if = 
{
  .ctx            = (void *)                  &_polar
 ,.Init           = (AlgoInit_fct)            &_Polar_Init
 ,.Run            = (AlgoRun_fct)             &_Polar_Run
 ,.GetTimeElapsed = (AlgoGetTimeElapsed_fct)  &_Polar_GetTimeElapsed
 ,.Release        = (AlgoRelease_fct)         &_Polar_Release
 ,.GetDebugData   = (AlgoGetDebugData_fct)    &_Polar_GetDebugData
};

// Private functions
//==============================================================================

INT8 _Polar_Init (Polar_t *p, UnitArrayInterface_t *unitArray)
{
  UINT8 i = 0;
  UINT8 minPosIdx, maxPosIdx;
  UINT8 unitId;
  
  p->timeElapsed    = 0;
  p->currentSample  = 0;
  p->unitArray      = unitArray;
  p->nUnits         = unitArray->GetNUnits(unitArray->ctx);
  
  for (i = 0; i < p->nUnits; i++)
  {
    unitArray->GetUnitLimitsIdx(unitArray->ctx, i, &minPosIdx, &maxPosIdx);
    unitId = unitArray->GetUnitId(unitArray->ctx, i);
    p->minPosIdx[i]     = minPosIdx;
//    p->maxPosIdx[i]     = maxPosIdx;
    p->maxPosIdx[i]     = unitsMaxPosIdx[unitId];   // Shortcut
    p->minPos[i]        = potRealValues[p->minPosIdx[i]];
    p->maxPos[i]        = potRealValues[p->maxPosIdx[i]];
    p->currentPos[i]    = p->minPos[i];
    p->currentPosIdx[i] = p->minPosIdx[i];
    p->finalPosIdx[i]   = unitsOptPosIdx[unitId];
    p->finalPos[i]      = potRealValues[p->finalPosIdx[i]];
    p->direction[i]     = POLAR_FORWARD;
    
    unitArray->SetPosIdx(unitArray->ctx, i, p->currentPosIdx[i]);
  }
}


void _Polar_GetDebugData (Polar_t *p, void *ret)
{
  return;
}


void _Polar_Release (Polar_t *p)
{
  p->currentSample  = 0;
  p->nUnits         = 0;
  p->timeElapsed    = 0;
  p->unitArray      = NULL;
}


INT8 _Polar_Run (Polar_t *p)
{
  UINT8 i;
  INT16 tmpIdx;
    
  p->timeElapsed += p->sampleTime;
  
  if (++p->currentSample >= p->nSamplesForAcq)
  {
    p->currentSample = 0;

    for (i = 0; i < p->nUnits; i++)
    {
      switch (p->direction[i])
      {
        // Going up
        case POLAR_FORWARD:
          tmpIdx = p->currentPosIdx[i] + p->posIncrement;
          p->currentPosIdx[i] = MIN(tmpIdx, p->maxPosIdx[i]);
          p->currentPos[i] = potRealValues[p->currentPosIdx[i]];

          p->unitArray->SetPosIdx(p->unitArray->ctx, i, p->currentPosIdx[i]);

          if (p->currentPosIdx[i] == p->maxPosIdx[i])
          {
            p->direction[i] = POLAR_BACKWARD;
          }
          break;
          
        // Going down
        case POLAR_BACKWARD:
          tmpIdx = (INT16) p->currentPosIdx[i] - (INT16) p->posIncrement;
          p->currentPosIdx[i] = MAX(tmpIdx, (INT16) p->minPosIdx[i]);
          p->currentPos[i] = potRealValues[p->currentPosIdx[i]];

          p->unitArray->SetPosIdx(p->unitArray->ctx, i, p->currentPosIdx[i]);

          if (p->currentPosIdx[i] == p->minPosIdx[i])
          {
            p->direction[i] = POLAR_STOP;
          }
          break;
          
        // Stay still
        case POLAR_STOP:
        default:
          p->currentPosIdx[i] = p->finalPosIdx[i];
          p->currentPos[i]    = p->finalPos[i];

          p->unitArray->SetPosIdx(p->unitArray->ctx, i, p->currentPosIdx[i]);
          break;
      }
    }
  }
  
  return 0;
}


float _Polar_GetTimeElapsed (Polar_t *p)
{
  return p->timeElapsed;
}



// Public functions
//==============================================================================

const AlgoInterface_t * PolarizationInterface (void)
{
  return &_polar_if;
}