//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : PnoSwarm.c
// Author  : Frederic Chasse
// Date    : 2017-04-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the methods needed for the Perturb & Observe
//           swarm algorithm used with PPSO-P&O.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "PnoSwarm.h"
#include "Potentiometer.h"
#include "MathFunctions.h"
#include "Position.h"
#include "PnoInstance.h"
#include "LinkedList.h"

// Private definitions
//==============================================================================

#define N_PNO_SWARMS_TOTAL    (N_UNITS_TOTAL)

typedef struct
{
  float         delta;
  float         umin;
  float         umax;
  float         uinit;
  UINT8         nSamplesForSs;
  UINT8         oscAmp;
  float         perturbOsc;
} PnoParam_t;

typedef struct
{
  UnitArrayInterface_t *unitArray;
  
  float sampleTime;
  float timeElapsed;
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

PnoInstance_t           _instances    [N_PNO_SWARMS_TOTAL];
PnoInstanceInterface_t  _instances_if [N_PNO_SWARMS_TOTAL];

LinkedList_t _unusedInstances       = {NULL, NULL, 0, N_PNO_SWARMS_TOTAL};
LinkedList_t _usedInstances         = {NULL, NULL, 0, N_PNO_SWARMS_TOTAL};
Node_t       _instancesNodes          [N_PNO_SWARMS_TOTAL];

static BOOL _oInstanceArrayInitialized = 0;


// Private functions
//==============================================================================

INT8 _Pno_Init (Pno_t *pno, UnitArrayInterface_t *unitArray)
{
  UINT8 i = 0;
  
  pno->timeElapsed    = 0;
  pno->unitArray      = unitArray;
  pno->nInstances     = unitArray->GetNUnits(unitArray->ctx);
  
  for (i = 0; i < pno->nInstances; i++)
  {
    pno->instances[i] = (PnoInstanceInterface_t *) PnoInstanceInterface(PNO_SWARM);
    pno->param[i].delta = POT_STEP_VALUE;
    pno->param[i].uinit = potRealValues[POT_MAX_INDEX/2];
    pno->param[i].umax = potRealValues[POT_MIN_INDEX];
    pno->param[i].umin = potRealValues[POT_MAX_INDEX];
    pno->param[i].nSamplesForSs = 6;
    pno->param[i].oscAmp = 2;
    pno->param[i].perturbOsc = 0.05;
    
    pno->instances[i]->Init(pno->instances[i]->ctx, i, pno->param[i].delta, pno->param[i].uinit, pno->param[i].umin, pno->param[i].umax, pno->param[i].perturbOsc);
    pno->instances[i]->SetSteadyState(pno->instances[i]->ctx, pno->param[i].nSamplesForSs, pno->param[i].oscAmp);
    
    unitArray->SetPos(unitArray->ctx, i, potRealValues[POT_MAX_INDEX/2]);
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
    pno->instances[i]->Release(pno->instances[i]->ctx);
  }
  pno->nInstances     = 0;
  pno->timeElapsed    = 0;
  pno->unitArray      = NULL;
}


INT8 _Pno_Run (Pno_t *pno)
{
  UINT8 i;
  BOOL oPerturbed[N_UNITS_TOTAL];
  PnoInstanceInterface_t *pnoi;
  float newPos;
  BOOL oFirstIteration = pno->timeElapsed == 0 ? 1 : 0;
  
  pno->timeElapsed += pno->sampleTime;
  
  for (i = 0; i < pno->nInstances; i++)
  {
    pnoi = pno->instances[i];
    
    pnoi->SetFitness(pnoi->ctx, pno->unitArray->GetPower(pno->unitArray->ctx, i));
    if (oFirstIteration)
    {
      pnoi->SetFitness(pnoi->ctx, pno->unitArray->GetPower(pno->unitArray->ctx, i));
    }
    
    newPos = pnoi->ComputePos(pnoi->ctx, &oPerturbed[i]);
    pno->unitArray->SetPos(pno->unitArray->ctx, i, newPos);
  }
  
  return 0;
}


float _Pno_GetTimeElapsed (Pno_t *pno)
{
  return pno->timeElapsed;
}


// Public functions
//==============================================================================

const AlgoInterface_t * PnoSwarmInterface (void)
{
  return &_pno_if;
}