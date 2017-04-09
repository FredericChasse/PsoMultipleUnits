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

// Private definitions
//==============================================================================

typedef struct
{
  UnitArrayInterface_t *unitArray;
  float sampleTime;
  float timeElapsed;
  UINT8 nUnits;
  UINT16 currentPosIdx;
  float currentPos;
} Charac_t;

// Private prototypes
//==============================================================================

INT8  _Charac_Init            (Charac_t *c, UnitArrayInterface_t *unitArray);
INT8  _Charac_Run             (Charac_t *c);
float _Charac_GetTimeElapsed  (Charac_t *c);
void  _Charac_Release         (Charac_t *c);

// Private variables
//==============================================================================

Charac_t _charac = 
{
  .unitArray      = NULL
 ,.sampleTime     = SAMPLING_TIME_FLOAT
 ,.timeElapsed    = 0
 ,.nUnits         = 0
 ,.currentPos     = MIN_POT_VALUE
 ,.currentPosIdx  = 0
};

const AlgoInterface_t _charac_if = 
{
  .ctx            = (void *)                  &_charac
 ,.Init           = (AlgoInit_fct)            &_Charac_Init
 ,.Run            = (AlgoRun_fct)             &_Charac_Run
 ,.GetTimeElapsed = (AlgoGetTimeElapsed_fct)  &_Charac_GetTimeElapsed
 ,.Release        = (AlgoRelease_fct)         &_Charac_Release
};

// Private functions
//==============================================================================

INT8 _Charac_Init (Charac_t *c, UnitArrayInterface_t *unitArray)
{
  UINT8 i = 0;
  
  c->currentPos     = MIN_POT_VALUE;
  c->currentPosIdx  = 0;
  c->timeElapsed    = 0;
  c->unitArray      = unitArray;
  c->nUnits         = unitArray->GetNUnits(unitArray->ctx);
  
  for (i = 0; i < c->nUnits; i++)
  {
    unitArray->SetPos(unitArray->ctx, i, potRealValues[c->currentPosIdx]);
//    unitArray->SetPos(unitArray->ctx, i, potRealValues[1]);
  }
}


void _Charac_Release (Charac_t *c)
{
  c->currentPos     = MIN_POT_VALUE;
  c->currentPosIdx  = 0;
  c->nUnits         = 0;
  c->timeElapsed    = 0;
  c->unitArray      = NULL;
}


INT8 _Charac_Run (Charac_t *c)
{
  UINT8 i;
  
  c->timeElapsed += c->sampleTime;
  
  if (c->currentPosIdx <= 255)
  {
    c->currentPosIdx++;
    for (i = 0; i < c->nUnits; i++)
    {
      c->unitArray->SetPos(c->unitArray->ctx, i, potRealValues[c->currentPosIdx]);
//      c->unitArray->SetPos(c->unitArray->ctx, i, potRealValues[1]);
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