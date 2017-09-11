//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : PnoInstance.c
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the methods for the P&O instances.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "SteadyState.h"
#include "LinkedList.h"
#include "MathFunctions.h"
#include "Potentiometer.h"  // To compute positions
#include "PnoInstance.h"
#include "Position.h"
#include "SteadyStatePno.h"
#include "StateMachine.h"   // For debugging


// Private definitions
//==============================================================================

#define N_INSTANCES_TOTAL      (N_UNITS_TOTAL)

typedef struct
{
  PnoInstanceInterface_t base;
  
  UINT8 id;
  Position_t pos;
  
  float umin;
  float umax;
  INT8  k;
  float delta;
  float perturbOsc;
  
  INT16 prevPotIdx;
  INT16 curPotIdx;
  INT16 delta_int;
  UINT8 umin_int;
  UINT8 umax_int;
  
  SteadyStatePno_t ss;
  float ssBuf[STEADY_STATE_PNO_MAX_SAMPLES];
} PnoInstance_t;


// Private prototypes
//==============================================================================

void  _PnoInstance_Init               (PnoInstance_t *pnoi, UINT8 delta, UINT8 pos, UINT8 umin, UINT8 umax, float perturbOsc);
void  _PnoInstance_SetSteadyState     (PnoInstance_t *pnoi, UINT8 samplesForSs, UINT8 oscAmp);
BOOL  _PnoInstance_GetSteadyState     (PnoInstance_t *pnoi);
float _PnoInstance_ComputePosClassic  (PnoInstance_t *pnoi, UINT8 *oPerturbed);
float _PnoInstance_ComputePosSwarm    (PnoInstance_t *pnoi, UINT8 *oPerturbed);
void  _PnoInstance_SetPos             (PnoInstance_t *pnoi, float pos);
float _PnoInstance_GetPos             (PnoInstance_t *pnoi);
void  _PnoInstance_SetPosIdx          (PnoInstance_t *pnoi, UINT8 pos);
UINT8 _PnoInstance_GetPosIdx          (PnoInstance_t *pnoi);
void  _PnoInstance_SetFitness         (PnoInstance_t *pnoi, float fitness);
void  _PnoInstance_Release            (PnoInstance_t *pnoi);
void  _PnoInstance_SetId              (PnoInstance_t *pnoi, UINT8 id);


const PnoiComputePos_fct   dbgComputePosSwarm = (PnoiComputePos_fct) &_PnoInstance_ComputePosSwarm
                          ,dbgComputePosClassic = (PnoiComputePos_fct) &_PnoInstance_ComputePosClassic
                          ;
const PnoiInit_fct dbgInit = (PnoiInit_fct) &_PnoInstance_Init;
const PnoiSetSteadyState_fct dbgSetSteadyState = (PnoiSetSteadyState_fct) &_PnoInstance_SetSteadyState;
const PnoiGetSteadyState_fct dbgGetSteadyState = (PnoiGetSteadyState_fct) &_PnoInstance_GetSteadyState;
const PnoiSetPos_fct dbgSetPos = (PnoiSetPos_fct) &_PnoInstance_SetPos;
const PnoiGetPos_fct dbgGetPos = (PnoiGetPos_fct) &_PnoInstance_GetPos;
const PnoiSetPosIdx_fct dbgSetPosIdx = (PnoiSetPosIdx_fct) &_PnoInstance_SetPosIdx;
const PnoiGetPosIdx_fct dbgGetPosIdx = (PnoiGetPosIdx_fct) &_PnoInstance_GetPosIdx;
const PnoiSetFitness_fct dbgSetFitness = (PnoiSetFitness_fct) &_PnoInstance_SetFitness;
const PnoiRelease_fct dbgRelease = (PnoiRelease_fct) &_PnoInstance_Release;
const PnoiSetId_fct dbgSetId = (PnoiSetId_fct) &_PnoInstance_SetId;



// Private variables
//==============================================================================

PnoInstance_t _instances          [N_INSTANCES_TOTAL];

LinkedList_t  _unusedInstances  = {NULL, NULL, 0, N_INSTANCES_TOTAL};
LinkedList_t  _usedInstances    = {NULL, NULL, 0, N_INSTANCES_TOTAL};
Node_t        _instancesNodes     [N_INSTANCES_TOTAL];

static BOOL _oInstanceArrayInitialized = 0;


// Private functions
//==============================================================================

void _PnoInstance_Init (PnoInstance_t *pnoi, UINT8 delta, UINT8 pos, UINT8 umin, UINT8 umax, float perturbOsc)
{
  pnoi->delta_int = delta;
  pnoi->delta = delta*POT_STEP_VALUE;
  Position_Reset(&pnoi->pos);
  SteadyStatePno_Reset(&pnoi->ss);
  pnoi->prevPotIdx = pnoi->curPotIdx = pos;
  pnoi->pos.prevPos = pnoi->pos.curPos = potRealValues[pos];
  pnoi->k = 1;
  pnoi->umax_int = umax;
  pnoi->umin_int = umin;
  pnoi->umax = potRealValues[umax];
  pnoi->umin = potRealValues[umin];
  pnoi->perturbOsc = perturbOsc;
}


void _PnoInstance_SetId (PnoInstance_t *pnoi, UINT8 id)
{
  pnoi->id = id;
}


float _PnoInstance_ComputePosClassic (PnoInstance_t *pnoi, UINT8 *oPerturbed)
{
  *oPerturbed = 0;
  
  if (pnoi->pos.curFitness < pnoi->pos.prevFitness)
  {
    pnoi->k = -pnoi->k;
  }
  
  pnoi->prevPotIdx = pnoi->curPotIdx;
  pnoi->curPotIdx += pnoi->delta_int*pnoi->k;
  
  if (pnoi->curPotIdx > pnoi->umax_int)
  {
    pnoi->curPotIdx = pnoi->umax_int;
  }
  if (pnoi->curPotIdx < pnoi->umin_int)
  {
    pnoi->curPotIdx = pnoi->umin_int;
  }
  pnoi->pos.prevPos = pnoi->pos.curPos;
  pnoi->pos.curPos += potRealValues[pnoi->curPotIdx];
  
  return (float) pnoi->curPotIdx;
}


float _PnoInstance_ComputePosSwarm (PnoInstance_t *pnoi, UINT8 *oPerturbed)
{
  float error;
  SteadyStatePno_AddSample(&pnoi->ss, &pnoi->pos.curPos);
  
  if (pnoi->pos.curFitness < pnoi->pos.prevFitness)
  {
    pnoi->k = -pnoi->k;
  }
  
  if (!SteadyStatePno_CheckForSteadyState(&pnoi->ss))
  {
    *oPerturbed = 0;
    pnoi->pos.prevPos = pnoi->pos.curPos;
    pnoi->pos.curPos += pnoi->delta*pnoi->k;
    if (pnoi->pos.curPos > pnoi->umax)
    {
      pnoi->pos.curPos = pnoi->umax;
    }
    if (pnoi->pos.curPos < pnoi->umin)
    {
      pnoi->pos.curPos = pnoi->umin;
    }
  }
  else
  {
    if (pnoi->pos.curPos == pnoi->pos.prevPos)
    {
      error = (pnoi->pos.curFitness - pnoi->pos.prevFitness)/pnoi->pos.prevFitness;
      if ( ABS(error) > pnoi->perturbOsc )
      {
        *oPerturbed = 1;
      }
      else
      {
        *oPerturbed = 0;
      }
    }
    else
    {
      *oPerturbed = 0;
    }
    pnoi->pos.prevPos = pnoi->pos.curPos;
  }
  
  return pnoi->pos.curPos;
}


float _PnoInstance_GetPos (PnoInstance_t *pnoi)
{
  return pnoi->pos.curPos;
}


UINT8 _PnoInstance_GetPosIdx (PnoInstance_t *pnoi)
{
  return pnoi->curPotIdx;
}


void _PnoInstance_SetPosIdx (PnoInstance_t *pnoi, UINT8 pos)
{
  pnoi->prevPotIdx = pnoi->curPotIdx;
  pnoi->curPotIdx = pos;
  pnoi->pos.prevPos = pnoi->pos.curPos;
  pnoi->pos.curPos = potRealValues[pos];
}


void _PnoInstance_SetPos (PnoInstance_t *pnoi, float pos)
{
  pnoi->pos.prevPos = pnoi->pos.curPos;
  pnoi->pos.curPos = pos;
}


void _PnoInstance_SetFitness (PnoInstance_t *pnoi, float fitness)
{
  pnoi->pos.prevFitness = pnoi->pos.curFitness;
  pnoi->pos.curFitness = fitness;
}


void _PnoInstance_Release (PnoInstance_t *pnoi)
{
  Node_t *node = LinkedList_FindNode(&_usedInstances, pnoi);
  __assert(node);
  LinkedList_RemoveNode(node->list, node);
  LinkedList_AddToEnd(&_unusedInstances, node);
  
  Position_Reset(&pnoi->pos);
    pnoi->delta
  = pnoi->k
  = pnoi->umin
  = pnoi->umax
  = 0
  ;
    
  SteadyStatePno_Reset(&pnoi->ss);
}


void _PnoInstance_SetSteadyState (PnoInstance_t *pnoi, UINT8 samplesForSs, UINT8 oscAmp)
{
  SteadyStatePno_Reset(&pnoi->ss);
  SteadyStatePno_Init (&pnoi->ss, pnoi->ssBuf, samplesForSs, oscAmp, pnoi->delta);
}


BOOL _PnoInstance_GetSteadyState (PnoInstance_t *pnoi)
{
  return pnoi->ss.oInSteadyState;
}


// Public functions
//==============================================================================

const PnoInstanceInterface_t * PnoInstanceInterface (PnoType_t type)
{
  UINT8 i;
  Node_t *temp;
  
  if (type > PNO_SWARM)
  {
    return NULL;
  }
  
  if (!_oInstanceArrayInitialized)
  {
    _oInstanceArrayInitialized = 1;
    
    _unusedInstances.head = (void *) &_instancesNodes[0];
    _unusedInstances.count = 1;
    
    for (i = 0; i < N_INSTANCES_TOTAL; i++)
    {
      // Init the instance itself and its interface
      _instances[i].id                  = i;
      _instances[i].base.Init           = (PnoiInit_fct)            &_PnoInstance_Init;
      _instances[i].base.ComputePos     = (PnoiComputePos_fct)      &_PnoInstance_ComputePosClassic;
      _instances[i].base.SetPos         = (PnoiSetPos_fct)          &_PnoInstance_SetPos;
      _instances[i].base.GetPos         = (PnoiGetPos_fct)          &_PnoInstance_GetPos;
      _instances[i].base.SetPosIdx      = (PnoiSetPosIdx_fct)       &_PnoInstance_SetPosIdx;
      _instances[i].base.GetPosIdx      = (PnoiGetPosIdx_fct)       &_PnoInstance_GetPosIdx;
      _instances[i].base.SetFitness     = (PnoiSetFitness_fct)      &_PnoInstance_SetFitness;
      _instances[i].base.Release        = (PnoiRelease_fct)         &_PnoInstance_Release;
      _instances[i].base.SetSteadyState = (PnoiSetSteadyState_fct)  &_PnoInstance_SetSteadyState;
      _instances[i].base.GetSteadyState = (PnoiGetSteadyState_fct)  &_PnoInstance_GetSteadyState;
      _instances[i].base.SetId          = (PnoiSetId_fct)           &_PnoInstance_SetId;
      
      // Init the linked list
      _instancesNodes[i].ctx = (void *) &_instances[i];
      _instancesNodes[i].key = i;
      
      if (i < (N_INSTANCES_TOTAL - 1))
      {
        _instancesNodes[i].next = &_instancesNodes[i + 1];
      }
      else
      {
        _instancesNodes[i].next = NULL;
      }
    }
    
    LinkedList_Init(&_unusedInstances, &_instancesNodes[0]);
  }
  
  if (_unusedInstances.count == 0)
  {
    LED1_ON();
    return NULL;
  }
  
  temp = _unusedInstances.tail;
  LinkedList_RemoveNode(&_unusedInstances, temp);
  LinkedList_AddToEnd(&_usedInstances, temp);
  
  
  if (type == PNO_CLASSIC)
  {
    ((PnoInstance_t *) temp->ctx)->base.ComputePos = (PnoiComputePos_fct) &_PnoInstance_ComputePosClassic;
  }
  else
  {
    ((PnoInstance_t *) temp->ctx)->base.ComputePos = (PnoiComputePos_fct) &_PnoInstance_ComputePosSwarm;
  }
  return temp->ctx;
}