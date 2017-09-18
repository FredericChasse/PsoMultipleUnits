//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Characterization.c
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the methods needed for characterizing the
//           units.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "Characterization.h"
#include "Potentiometer.h"
#include "MathFunctions.h"

// Private definitions
//==============================================================================

typedef struct
{
  UnitArrayInterface_t *unitArray;
  float sampleTime;
  UINT32 nSamplesForAcq;
  UINT32 currentSample;
  float timeElapsed;
  UINT8 nUnits;
  UINT16 currentPosIdx;
  float currentPos;
  UINT16 posIncrement;
  UINT16 minPosIdx;
  UINT16 maxPosIdx;
  float  minPos;
  float  maxPos;
} Charac_t;

// Private prototypes
//==============================================================================

INT8  _Charac_Init            (Charac_t *c, UnitArrayInterface_t *unitArray);
INT8  _Charac_Run             (Charac_t *c);
float _Charac_GetTimeElapsed  (Charac_t *c);
void  _Charac_Release         (Charac_t *c);
void  _Charac_GetDebugData    (Charac_t *c, void *ret);

// Private variables
//==============================================================================

Charac_t _charac = 
{
  .unitArray      = NULL
 ,.sampleTime     = SAMPLING_TIME_FLOAT
 ,.nSamplesForAcq = N_LARGE_SAMPLES_FOR_ALGO
 ,.currentSample  = 0
 ,.timeElapsed    = 0
 ,.nUnits         = 0
 ,.currentPosIdx  = POT_MIN_INDEX
 ,.currentPos     = MIN_POT_VALUE
 ,.posIncrement   = 4
 ,.minPosIdx      = POT_MIN_INDEX
 ,.maxPosIdx      = POT_MAX_INDEX
 ,.minPos         = MIN_POT_VALUE
 ,.maxPos         = MAX_POT_VALUE
};

const AlgoInterface_t _charac_if = 
{
  .ctx            = (void *)                  &_charac
 ,.Init           = (AlgoInit_fct)            &_Charac_Init
 ,.Run            = (AlgoRun_fct)             &_Charac_Run
 ,.GetTimeElapsed = (AlgoGetTimeElapsed_fct)  &_Charac_GetTimeElapsed
 ,.Release        = (AlgoRelease_fct)         &_Charac_Release
 ,.GetDebugData   = (AlgoGetDebugData_fct)    &_Charac_GetDebugData
};

// Private functions
//==============================================================================

INT8 _Charac_Init (Charac_t *c, UnitArrayInterface_t *unitArray)
{
  UINT8 i = 0;
  UINT8 minPosIdx, maxPosIdx;
  float minPos, maxPos;
  
  c->timeElapsed    = 0;
  c->currentSample  = 0;
  c->unitArray      = unitArray;
  c->nUnits         = unitArray->GetNUnits(unitArray->ctx);
  unitArray->GetPosLimits(unitArray->ctx, &minPos, &maxPos);
  minPosIdx = ComputePotValueFloat2Dec(minPos);
  maxPosIdx = ComputePotValueFloat2Dec(maxPos);
  c->currentPos     = minPos;
  c->currentPosIdx  = minPosIdx;
  
  for (i = 0; i < c->nUnits; i++)
  {
    unitArray->SetPos(unitArray->ctx, i, potRealValues[c->currentPosIdx]);
  }
}


void _Charac_GetDebugData (Charac_t *c, void *ret)
{
  return;
}


void _Charac_Release (Charac_t *c)
{
  c->currentPos     = c->minPos;
  c->currentPosIdx  = c->minPosIdx;
  c->currentSample  = 0;
  c->nUnits         = 0;
  c->timeElapsed    = 0;
  c->unitArray      = NULL;
}


INT8 _Charac_Run (Charac_t *c)
{
  UINT8 i;
  UINT16 tmpIdx;
    
  c->timeElapsed += c->sampleTime;
  
  if (++c->currentSample >= c->nSamplesForAcq)
  {
    c->currentSample = 0;

    tmpIdx = c->currentPosIdx + c->posIncrement;
    c->currentPosIdx = MIN(tmpIdx, c->maxPosIdx);
    c->currentPos = potRealValues[c->currentPosIdx];
    
    for (i = 0; i < c->nUnits; i++)
    {
      c->unitArray->SetPosIdx(c->unitArray->ctx, i, c->currentPosIdx);
    }
  }
  
  return 0;
}


float _Charac_GetTimeElapsed (Charac_t *c)
{
  return c->timeElapsed;
}



// Public functions
//==============================================================================

const AlgoInterface_t * CharacterizationInterface (void)
{
  return &_charac_if;
}