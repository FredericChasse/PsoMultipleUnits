//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : ExtremumSeeking.c
// Author  : Frederic Chasse
// Date    : 2017-04-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the methods needed for the Extremum Seeking
//           algorithm.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "ExtremumSeeking.h"
#include "Potentiometer.h"
#include "MathFunctions.h"
#include "Position.h"

// Private definitions
//==============================================================================

typedef struct
{
  float         ap;
  float         wp;
  float         wl;
  float         wh;
  float         k;
  float         umin;
  float         umax;
  float         uinit;
  Position_t    pos;
  TustinValue_t hpfOutput;
  TustinValue_t hpfInput;
  TustinValue_t lpfOutput;
  TustinValue_t lpfInput;
  TustinValue_t grad;
} ExtSeekParam_t;

typedef struct
{
  UnitArrayInterface_t *unitArray;
  float sampleTime;
  float timeElapsed;
  UINT8 nUnits;
  ExtSeekParam_t param[N_UNITS_TOTAL];
} ExtSeek_t;


// Private prototypes
//==============================================================================

INT8  _ExtSeek_Init            (ExtSeek_t *es, UnitArrayInterface_t *unitArray);
INT8  _ExtSeek_Run             (ExtSeek_t *es);
float _ExtSeek_GetTimeElapsed  (ExtSeek_t *es);
void  _ExtSeek_Release         (ExtSeek_t *es);
void  _ExtSeek_GetDebugData    (ExtSeek_t *es, void *ret);

// Private variables
//==============================================================================

ExtSeek_t _es = 
{
  .unitArray      = NULL
 ,.sampleTime     = SAMPLING_TIME_FLOAT
 ,.timeElapsed    = 0
 ,.nUnits         = 0
 ,.param          = {0}
};

const AlgoInterface_t _es_if = 
{
  .ctx            = (void *)                  &_es
 ,.Init           = (AlgoInit_fct)            &_ExtSeek_Init
 ,.Run            = (AlgoRun_fct)             &_ExtSeek_Run
 ,.GetTimeElapsed = (AlgoGetTimeElapsed_fct)  &_ExtSeek_GetTimeElapsed
 ,.Release        = (AlgoRelease_fct)         &_ExtSeek_Release
 ,.GetDebugData   = (AlgoGetDebugData_fct)    &_ExtSeek_GetDebugData
};

// Private functions
//==============================================================================

INT8 _ExtSeek_Init (ExtSeek_t *es, UnitArrayInterface_t *unitArray)
{
  UINT8 i = 0;
  
  es->timeElapsed    = 0;
  es->unitArray      = unitArray;
  es->nUnits         = unitArray->GetNUnits(unitArray->ctx);
  
  for (i = 0; i < es->nUnits; i++)
  {
    unitArray->SetPos(unitArray->ctx, i, potRealValues[0]);
  }
}


void _ExtSeek_GetDebugData (ExtSeek_t *es, void *ret)
{
  return;
}


void _ExtSeek_Release (ExtSeek_t *es)
{
  es->nUnits         = 0;
  es->timeElapsed    = 0;
  es->unitArray      = NULL;
}


INT8 _ExtSeek_Run (ExtSeek_t *es)
{
  UINT8 i;
  
  es->timeElapsed += es->sampleTime;
  
  return 0;
}


float _ExtSeek_GetTimeElapsed (ExtSeek_t *es)
{
  return es->timeElapsed;
}



// Public functions
//==============================================================================

const AlgoInterface_t * ExtremumSeekingInterface (void)
{
  return &_es_if;
}