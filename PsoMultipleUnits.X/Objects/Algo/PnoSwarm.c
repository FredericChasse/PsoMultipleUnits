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
  UINT8 id;
  UINT8 linkKey;
  
  UnitArrayInterface_t *unitArray;
  
  UINT8 nInstances;
  UINT32 iteration;
  PnoSwarmParam_t param[N_UNITS_TOTAL];
  PnoInstanceInterface_t *instances[N_UNITS_TOTAL];
} PnoSwarm_t;


// Private prototypes
//==============================================================================

INT8  _PnoSwarm_Init              (PnoSwarm_t *pno, UnitArrayInterface_t *unitArray, PnoSwarmParam_t *param, UINT8 id);
UINT8 _PnoSwarm_ComputeAllPos     (PnoSwarm_t *pno, float *newPos, UINT8 *idxPerturbed);
void  _PnoSwarm_RemoveInstances   (PnoSwarm_t *pno, UINT8 *idx, UINT8 nInstances);
void  _PnoSwarm_Release           (PnoSwarm_t *pno);
BOOL  _PnoSwarm_GetSteadyState    (PnoSwarm_t *pno, UINT8 idx);
void  _PnoSwarm_SetPos            (PnoSwarm_t *pno, UINT8 idx, float pos);
UINT8 _PnoSwarm_GetNInstances     (PnoSwarm_t *pno);
void  _PnoSwarm_SetFitness        (PnoSwarm_t *pno, UINT8 idx, float fitness);
void  _PnoSwarm_IncIteration      (PnoSwarm_t *pno);
void* _PnoSwarm_GetArray          (PnoSwarm_t *pno);
void  _PnoSwarm_SetId             (PnoSwarm_t *pno, UINT8 id);

static int _CompareFunc (const void *p1, const void *p2);
void _PnoSwarm_ShiftInstancesLeft (PnoSwarm_t *pno, UINT8 idxToShift);


// Private variables
//==============================================================================

PnoSwarm_t            _pnos         [N_PNO_SWARMS_TOTAL];
PnoSwarmInterface_t   _pnos_if      [N_PNO_SWARMS_TOTAL];

LinkedList_t          _unusedPnos = {NULL, NULL, 0, N_PNO_SWARMS_TOTAL};
LinkedList_t          _usedPnos   = {NULL, NULL, 0, N_PNO_SWARMS_TOTAL};
Node_t                _pnosNodes    [N_PNO_SWARMS_TOTAL];

static BOOL _oPnoArrayInitialized = 0;


// Private functions
//==============================================================================

INT8 _PnoSwarm_Init (PnoSwarm_t *pno, UnitArrayInterface_t *unitArray, PnoSwarmParam_t *param, UINT8 id)
{
  UINT8 i = 0;
  
  pno->id             = id;
  pno->iteration      = 0;
  pno->unitArray      = unitArray;
  pno->nInstances     = unitArray->GetNUnits(unitArray->ctx);
  
  for (i = 0; i < pno->nInstances; i++)
  {
    pno->instances[i] = (PnoInstanceInterface_t *) PnoInstanceInterface(PNO_SWARM);
    memcpy(&pno->param[i], param, sizeof(PnoSwarmParam_t));
//    pno->param[i].delta = POT_STEP_VALUE;
//    pno->param[i].uinit = potRealValues[POT_MAX_INDEX/2];
//    pno->param[i].umax = potRealValues[POT_MIN_INDEX];
//    pno->param[i].umin = potRealValues[POT_MAX_INDEX];
//    pno->param[i].nSamplesForSs = 6;
//    pno->param[i].oscAmp = 2;
//    pno->param[i].perturbOsc = 0.05;
    
    pno->instances[i]->Init(pno->instances[i]->ctx, i, pno->param[i].delta_int, pno->param[i].uinit_int, pno->param[i].umin_int, pno->param[i].umax_int, pno->param[i].perturbOsc);
    pno->instances[i]->SetSteadyState(pno->instances[i]->ctx, pno->param[i].nSamplesForSs, pno->param[i].oscAmp);
  }
}


void _PnoSwarm_Release (PnoSwarm_t *pno)
{
  UINT8 i;
  Node_t *node = &_pnosNodes[pno->linkKey];
  
  for (i = 0; i < pno->nInstances; i++)
  {
    pno->instances[i]->Release(pno->instances[i]->ctx);
  }
  pno->nInstances     = 0;
  pno->unitArray      = NULL;
  pno->id             = 0;
  pno->iteration      = 0;
  
  LinkedList_RemoveNode(node->list, node);
  LinkedList_AddToEnd(&_unusedPnos, node);
}


void _PnoSwarm_IncIteration (PnoSwarm_t *pno)
{
  pno->iteration++;
}


UINT8 _PnoSwarm_ComputeAllPos (PnoSwarm_t *pno, float *newPos, UINT8 *idxPerturbed)
{
  UINT8 i;
  PnoInstanceInterface_t *pnoi;
  BOOL oPerturbed;
  UINT8 nPerturbed = 0;
  
  for (i = 0; i < pno->nInstances; i++)
  {
    pnoi = pno->instances[i];
    
    pnoi->SetFitness(pnoi->ctx, pno->unitArray->GetPower(pno->unitArray->ctx, i));
    if (pno->iteration == 1)
    {
      pnoi->SetFitness(pnoi->ctx, pno->unitArray->GetPower(pno->unitArray->ctx, i));
    }
    
    newPos[i] = pnoi->ComputePos(pnoi->ctx, &oPerturbed);
    if (oPerturbed)
    {
      idxPerturbed[nPerturbed++] = i;
    }
  }
  
  return nPerturbed;
}


void * _PnoSwarm_GetArray (PnoSwarm_t *pno)
{
  return (void *) pno->unitArray;
}


void _PnoSwarm_RemoveInstances (PnoSwarm_t *pno, UINT8 *idx, UINT8 nInstances)
{
  UINT8 i;
  
  if (nInstances > pno->nInstances)
  {
    return;
  }
  
  qsort( (void *) idx, (size_t) nInstances, sizeof(UINT8), &_CompareFunc);
  
  for (i = 0; i < nInstances; i++)
  {
    pno->instances[idx[i]]->Release(pno->instances[idx[i]]->ctx);
    _PnoSwarm_ShiftInstancesLeft(pno, idx[i]);
    pno->nInstances--;
  }
  
  for (i = 0; i < pno->nInstances; i++)
  {
    pno->instances[i]->SetId(pno->instances[i]->ctx, i);
  }
}


void _PnoSwarm_SetId (PnoSwarm_t *pno, UINT8 id)
{
  pno->id = id;
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


BOOL _PnoSwarm_GetSteadyState (PnoSwarm_t *pno, UINT8 idx)
{
  return pno->instances[idx]->GetSteadyState(pno->instances[idx]->ctx);
}


void _PnoSwarm_SetPos (PnoSwarm_t *pno, UINT8 idx, float pos)
{
  pno->instances[idx]->SetPos(pno->instances[idx]->ctx, pos);
}


void _PnoSwarm_SetFitness (PnoSwarm_t *pno, UINT8 idx, float fitness)
{
  pno->instances[idx]->SetFitness(pno->instances[idx]->ctx, fitness);
}


UINT8 _PnoSwarm_GetNInstances (PnoSwarm_t *pno)
{
  return pno->nInstances;
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
    
    _unusedPnos.head = (void *) &_pnosNodes[0];
    _unusedPnos.count = 1;
    
    for (i = 0; i < N_PNO_SWARMS_TOTAL; i++)
    {
      // Init the instance itself and its interface
      _pnos[i].linkKey    = i;
      _pnos[i].iteration  = 0;
      _pnos[i].nInstances = 0;
      _pnos[i].unitArray  = NULL;
      
      _pnos_if[i].ctx             = (void *)                      &_pnos[i];
      _pnos_if[i].Init            = (PnoSwarmInit_fct)            &_PnoSwarm_Init;
      _pnos_if[i].ComputeAllPos   = (PnoSwarmComputeAllPos_fct)   &_PnoSwarm_ComputeAllPos;
      _pnos_if[i].GetNInstances   = (PnoSwarmGetNInstances_fct)   &_PnoSwarm_GetNInstances;
      _pnos_if[i].GetSteadyState  = (PnoSwarmGetSteadyState_fct)  &_PnoSwarm_GetSteadyState;
      _pnos_if[i].Release         = (PnoSwarmRelease_fct)         &_PnoSwarm_Release;
      _pnos_if[i].RemoveInstances = (PnoSwarmRemoveInstances_fct) &_PnoSwarm_RemoveInstances;
      _pnos_if[i].SetFitness      = (PnoSwarmSetFitness_fct)      &_PnoSwarm_SetFitness;
      _pnos_if[i].SetPos          = (PnoSwarmSetPos_fct)          &_PnoSwarm_SetPos;
      _pnos_if[i].IncIteration    = (PnoSwarmIncIteration_fct)    &_PnoSwarm_IncIteration;
      _pnos_if[i].GetArray        = (PnoSwarmGetArray_fct)        &_PnoSwarm_GetArray;
      _pnos_if[i].SetId           = (PnoSwarmSetId_fct)           &_PnoSwarm_SetId;
      
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