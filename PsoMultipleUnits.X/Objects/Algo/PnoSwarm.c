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
  UINT8 id;
  UINT8 linkKey;
  
  UnitArrayInterface_t *unitArray;
  
  UINT8 nInstances;
  UINT32 iteration;
  PnoParam_t param[N_UNITS_TOTAL];
  PnoInstanceInterface_t *instances[N_UNITS_TOTAL];
} PnoSwarm_t;


// Private prototypes
//==============================================================================

typedef INT8  (*PnoSwarmInit_fct)             (void *ctx, UnitArrayInterface_t *unitArray);
typedef void  (*PnoSwarmComputeAllPos_fct)    (void *ctx, float *newPos, BOOL *oPerturbed, UINT8 *nPerturbed);
typedef void  (*PnoSwarmRemoveInstances_fct)  (void *ctx, UINT8 *idx, UINT8 nInstances);
typedef void  (*PnoSwarmRelease_fct)          (void *ctx);

INT8  _Pno_Init             (PnoSwarm_t *pno, UnitArrayInterface_t *unitArray);
void  _Pno_ComputeAllPos    (PnoSwarm_t *pno, float *newPos, BOOL *oPerturbed, UINT8 *nPerturbed);
void  _Pno_RemoveInstances  (PnoSwarm_t *pno, UINT8 *idx, UINT8 nInstances);
void  _Pno_Release          (PnoSwarm_t *pno);
void  _Pno_GetDebugData     (PnoSwarm_t *pno, void *ret);

static int _CompareFunc (const void *p1, const void *p2);


// Private variables
//==============================================================================

PnoSwarm_t _pno = 
{
  .unitArray      = NULL
 ,.instances      = NULL
 ,.nInstances     = 0
 ,.param          = {0}
};

PnoSwarm_t            _pnos         [N_PNO_SWARMS_TOTAL];
PnoSwarmInterface_t   _pnos_if      [N_PNO_SWARMS_TOTAL];

LinkedList_t          _unusedPnos = {NULL, NULL, 0, N_PNO_SWARMS_TOTAL};
LinkedList_t          _usedPnos   = {NULL, NULL, 0, N_PNO_SWARMS_TOTAL};
Node_t                _pnosNodes    [N_PNO_SWARMS_TOTAL];

static BOOL _oPnoArrayInitialized = 0;


// Private functions
//==============================================================================

INT8 _Pno_Init (PnoSwarm_t *pno, UnitArrayInterface_t *unitArray)
{
  UINT8 i = 0;
  
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


void _Pno_GetDebugData (PnoSwarm_t *pno, void *ret)
{
  return;
}


void _Pno_Release (PnoSwarm_t *pno)
{
  UINT8 i;
  
  for (i = 0; i < pno->nInstances; i++)
  {
    pno->instances[i]->Release(pno->instances[i]->ctx);
  }
  pno->nInstances     = 0;
  pno->unitArray      = NULL;
}


INT8 _Pno_Run (PnoSwarm_t *pno)
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


void _Swarm_RemoveParticles (PsoSwarm_t *s, UINT8 *idx, UINT8 nParticles)
{
  UINT8 i;
  
  if (nParticles > s->nParticles)
  {
    return;
  }
  
  qsort( (void *) idx, (size_t) nParticles, sizeof(UINT8), &_CompareFunc);
  
  for (i = 0; i < nParticles; i++)
  {
    s->particles[idx[i]]->Release(s->particles[idx[i]]->ctx);
    _Swarm_ShiftParticlesLeft(s, idx[i]);
    s->nParticles--;
  }
  
  for (i = 0; i < s->nParticles; i++)
  {
    s->particles[i]->SetId(s->particles[i]->ctx, i);
  }
}


void _PnoSwarm_ShiftInstancesLeft (PnoSwarm_t *pno, UINT8 idxToShift)
{
  if (idxToShift == (pno->nInstances - 1))
  {
    return;
  }
  UINT8 offset = pno->nInstances - idxToShift - 1;
  memmove(&pno->instances[idxToShift], &pno->instances[idxToShift + 1], offset * sizeof(PnoInstanceInterface_t *));
}


static int _CompareFunc (const void *p1, const void *p2)
{
  if ( *(UINT8 *) p1  > *(UINT8 *) p2 ) return -1;
  if ( *(UINT8 *) p1 == *(UINT8 *) p2 ) return  0;
  if ( *(UINT8 *) p1  < *(UINT8 *) p2 ) return  1;
}


// Public functions
//==============================================================================

const PnoSwarmInterface_t * PnoSwarmInterface (void)
{
  UINT8 i;
  Node_t *temp;
  
  if (!_oPnoArrayInitialized)
  {
    _oPnoArrayInitialized = 1;
    
    _unusedInstances.head = (void *) &_instancesNodes[0];
    _unusedInstances.count = 1;
    
    for (i = 0; i < N_PNO_SWARMS_TOTAL; i++)
    {
      // Init the instance itself and its interface
      _pnos[i].linkKey = i;
      
      _pnos_if[i].ctx            = (void *)                  &_pnos[i];
      _pnos_if[i].Init           = (PnoiInit_fct)            &_PnoInstance_Init;
      _pnos_if[i].ComputePos     = (PnoiComputePos_fct)      &_PnoInstance_ComputePosClassic;
      _pnos_if[i].SetPos         = (PnoiSetPos_fct)          &_PnoInstance_SetPos;
      _pnos_if[i].SetFitness     = (PnoiSetFitness_fct)      &_PnoInstance_SetFitness;
      _pnos_if[i].Release        = (PnoiRelease_fct)         &_PnoInstance_Release;
      _pnos_if[i].SetSteadyState = (PnoiSetSteadyState_fct)  &_PnoInstance_SetSteadyState;
      _pnos_if[i].GetSteadyState = (PnoiGetSteadyState_fct)  &_PnoInstance_GetSteadyState;
      
      // Init the linked list
      _pnosNodes[i].ctx = (void *) &_pnos_if[i];
      _pnosNodes[i].key = i;
      
      if (i < (N_PNO_SWARMS_TOTAL - 1))
      {
        _pnosNodes[i].next = &_pnosNodes[i + 1];
      }
      else
      {
        _pnosNodes[i].next = NULL;
      }
    }
    
    LinkedList_Init(&_unusedPnos, &_pnosNodes[0]);
  }
  
  if (_unusedPnos.count == 0)
  {
    return NULL;
  }
  
  temp = _unusedPnos.tail;
  LinkedList_RemoveNode(&_unusedPnos, temp);
  LinkedList_AddToEnd(&_usedPnos, temp);
  return temp->ctx;
}