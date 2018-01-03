//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Pno.c
// Author  : Frederic Chasse
// Date    : 2017-04-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the methods needed for the Perturb & Observe
//           algorithm.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "Pno.h"
#include "Potentiometer.h"
#include "MathFunctions.h"
#include "Position.h"
#include "PnoInstance.h"

// Private definitions
//==============================================================================

typedef struct
{
  float         delta;
  INT16         delta_int;
  float         umin;
  INT16         umin_int;
  float         umax;
  INT16         umax_int;
  float         uinit;
  INT16         uinit_int;
  UINT8         nSamplesForSs;
  UINT8         oscAmp;
  float         perturbOsc;
} PnoParam_t;

typedef struct
{
  UnitArrayInterface_t *unitArray;
  
  float sampleTime;
  float timeElapsed;
  UINT32 iteration;
  UINT8 nInstances;
  PnoParam_t param[N_UNITS_TOTAL];
  PnoInstanceInterface_t *instances[N_UNITS_TOTAL];
} Pno_t;


// Private prototypes
//==============================================================================

INT8  _Pno_Init            (Pno_t *pno, UnitArrayInterface_t *unitArray);
INT8  _Pno_Run             (Pno_t *pno);
float _Pno_GetTimeElapsed  (Pno_t *pno);
void  _Pno_Release         (Pno_t *pno);
void  _Pno_GetDebugData    (Pno_t *pno, void *ret);


// Private variables
//==============================================================================

Pno_t _pno = 
{
  .unitArray      = NULL
 ,.instances      = NULL
 ,.sampleTime     = SAMPLING_TIME_FLOAT
 ,.timeElapsed    = 0
 ,.nInstances     = 0
 ,.param          = {0}
};

const AlgoInterface_t _pno_if = 
{
  .ctx            = (void *)                  &_pno
 ,.Init           = (AlgoInit_fct)            &_Pno_Init
 ,.Run            = (AlgoRun_fct)             &_Pno_Run
 ,.GetTimeElapsed = (AlgoGetTimeElapsed_fct)  &_Pno_GetTimeElapsed
 ,.Release        = (AlgoRelease_fct)         &_Pno_Release
 ,.GetDebugData   = (AlgoGetDebugData_fct)    &_Pno_GetDebugData
};


// Private functions
//==============================================================================

INT8 _Pno_Init (Pno_t *pno, UnitArrayInterface_t *unitArray)
{
  UINT8 i = 0;
  
  pno->timeElapsed    = 0;
  pno->iteration      = 0;
  pno->unitArray      = unitArray;
  pno->nInstances     = unitArray->GetNUnits(unitArray->ctx);
  float posMin, posMax;
  UINT8 posMinIdx, posMaxIdx;
  
  unitArray->GetPosLimits(unitArray->ctx, &posMin, &posMax);
  posMinIdx = ComputePotValueFloat2Dec(posMin);
  posMaxIdx = ComputePotValueFloat2Dec(posMax);
  
  for (i = 0; i < pno->nInstances; i++)
  {
    pno->instances[i] = (PnoInstanceInterface_t *) PnoInstanceInterface(PNO_TYPE_CLASSIC);
    pno->param[i].delta_int = 2;
    pno->param[i].delta = pno->param[i].delta_int*POT_STEP_VALUE;
//    pno->param[i].uinit_int = posMinIdx;
//    pno->param[i].uinit_int = 0;   // 50
    pno->param[i].uinit_int = 38;   // 199.0196
    pno->param[i].uinit = potRealValues[pno->param[i].uinit_int];
    pno->param[i].umax_int = posMaxIdx;
    pno->param[i].umax = potRealValues[pno->param[i].umax_int];
    pno->param[i].umin_int = POT_MIN_INDEX;
    pno->param[i].umin = potRealValues[pno->param[i].umin_int];
    pno->param[i].nSamplesForSs = 6;
    pno->param[i].oscAmp = 2;
    pno->param[i].perturbOsc = 0.05;
    
    pno->instances[i]->Init(pno->instances[i], pno->param[i].delta_int, pno->param[i].uinit_int, pno->param[i].umin_int, pno->param[i].umax_int, pno->param[i].perturbOsc);
    pno->instances[i]->SetSteadyState(pno->instances[i], pno->param[i].nSamplesForSs, pno->param[i].oscAmp);
    
    unitArray->SetPosIdx(unitArray->ctx, i, pno->instances[i]->GetPosIdx(pno->instances[i]));
  }
}


void _Pno_GetDebugData (Pno_t *pno, void *ret)
{
  return;
}


void _Pno_Release (Pno_t *pno)
{
  UINT8 i;
  
  for (i = 0; i < pno->nInstances; i++)
  {
    pno->instances[i]->Release(pno->instances[i]);
  }
  pno->nInstances     = 0;
  pno->timeElapsed    = 0;
  pno->iteration      = 0;
  pno->unitArray      = NULL;
}


INT8 _Pno_Run (Pno_t *pno)
{
  UINT8 i;
  UINT8 dummy;
  PnoInstanceInterface_t *pnoi;
  UINT8 newPos;
  BOOL oFirstIteration = pno->iteration == 0 ? 1 : 0;
  
  pno->timeElapsed += pno->sampleTime;
  pno->iteration++;
  
  for (i = 0; i < pno->nInstances; i++)
  {
    pnoi = pno->instances[i];
    
    pnoi->SetFitness(pnoi, pno->unitArray->GetPower(pno->unitArray->ctx, i));
    if (oFirstIteration)
    {
      pnoi->SetFitness(pnoi, pno->unitArray->GetPower(pno->unitArray->ctx, i));
    }
    
    newPos = pnoi->ComputePos(pnoi, &dummy);
    pno->unitArray->SetPosIdx(pno->unitArray->ctx, i, newPos);
  }
  
  return 0;
}


float _Pno_GetTimeElapsed (Pno_t *pno)
{
  return pno->timeElapsed;
}


// Public functions
//==============================================================================

const AlgoInterface_t * PnoInterface (void)
{
  return &_pno_if;
}