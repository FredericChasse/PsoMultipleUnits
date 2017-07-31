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

#include "DebugAdc.h"
#include "Potentiometer.h"

// Private definitions
//==============================================================================

typedef struct
{
  UnitArrayInterface_t *unitArray;
  UINT32 iteration;
  float sampleTime;
  float timeElapsed;
  UINT8 nUnits;
  UINT16 currentPosIdx;
  float currentPos;
} DbgAdc_t;

// Private prototypes
//==============================================================================

INT8  _DbgAdc_Init            (DbgAdc_t *d, UnitArrayInterface_t *unitArray);
INT8  _DbgAdc_Run             (DbgAdc_t *d);
float _DbgAdc_GetTimeElapsed  (DbgAdc_t *d);
void  _DbgAdc_Release         (DbgAdc_t *d);
void  _DbgAdc_GetDebugData    (DbgAdc_t *d, void *ret);

// Private variables
//==============================================================================

DbgAdc_t _dbgAdc = 
{
  .unitArray      = NULL
 ,.sampleTime     = SAMPLING_TIME_FLOAT
 ,.timeElapsed    = 0
 ,.nUnits         = 0
 ,.currentPos     = MIN_POT_VALUE
 ,.currentPosIdx  = 0
 ,.iteration      = 0
};

const AlgoInterface_t _dbgAdc_if = 
{
  .ctx            = (void *)                  &_dbgAdc
 ,.Init           = (AlgoInit_fct)            &_DbgAdc_Init
 ,.Run            = (AlgoRun_fct)             &_DbgAdc_Run
 ,.GetTimeElapsed = (AlgoGetTimeElapsed_fct)  &_DbgAdc_GetTimeElapsed
 ,.Release        = (AlgoRelease_fct)         &_DbgAdc_Release
 ,.GetDebugData   = (AlgoGetDebugData_fct)    &_DbgAdc_GetDebugData
};

// Private functions
//==============================================================================

INT8 _DbgAdc_Init (DbgAdc_t *d, UnitArrayInterface_t *unitArray)
{
  UINT8 i = 0;
  
  d->iteration      = 0;
  d->currentPosIdx  = 125;
  d->currentPos     = potRealValues[d->currentPosIdx];
  d->timeElapsed    = 0;
  d->unitArray      = unitArray;
  d->nUnits         = unitArray->GetNUnits(unitArray->ctx);
  
  for (i = 0; i < d->nUnits; i++)
  {
//    unitArray->SetPos(unitArray->ctx, i, potRealValues[d->currentPosIdx]);
//    unitArray->SetPos(unitArray->ctx, i, potRealValues[1]);
    unitArray->SetPosIdx(unitArray->ctx, i, d->currentPosIdx);
  }
  
  return 0;
}


void _DbgAdc_GetDebugData (DbgAdc_t *d, void *ret)
{
  return;
}


void _DbgAdc_Release (DbgAdc_t *d)
{
  d->iteration      = 0;
  d->currentPos     = MIN_POT_VALUE;
  d->currentPosIdx  = 0;
  d->nUnits         = 0;
  d->timeElapsed    = 0;
  d->unitArray      = NULL;
}


INT8 _DbgAdc_Run (DbgAdc_t *d)
{
  UINT8 i;
  static BOOL oToggle = 0;
  
  d->iteration++;
  d->timeElapsed += d->sampleTime;
  
//  if (oToggle)
//  {
//    oToggle = 0;
//    d->currentPosIdx -= 2;
//  }
//  else
//  {
//    oToggle = 1;
//    d->currentPosIdx += 2;
//  }
//  
////  if (d->iteration < 10)
////  {
////    d->currentPosIdx = 0;
////  }
////  else if (d->iteration < 60)
////  {
////    d->currentPosIdx = 255;
////  }
////  else
////  {
////    d->currentPosIdx = 255 >> 1;
////  }
//  
//  if (d->currentPosIdx <= 255)
//  {
//    for (i = 0; i < d->nUnits; i++)
//    {
//      d->unitArray->SetPosIdx(d->unitArray->ctx, i, d->currentPosIdx);
////      d->unitArray->SetPos(d->unitArray->ctx, i, potRealValues[d->currentPosIdx]);
////      c->unitArray->SetPos(c->unitArray->ctx, i, potRealValues[1]);
//    }
//  }
  
  return 0;
}


float _DbgAdc_GetTimeElapsed (DbgAdc_t *d)
{
  return d->timeElapsed;
}



// Public functions
//==============================================================================

const AlgoInterface_t * DebugAdcInterface (void)
{
  return &_dbgAdc_if;
}