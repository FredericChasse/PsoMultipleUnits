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


// Private definitions
//==============================================================================

#define N_INSTANCES_TOTAL      (N_UNITS_TOTAL)

typedef struct
{
  UINT8 id;
  UINT8 linkKey;
  Position_t pos;
  
  float umin;
  float umax;
  float k;
  float delta;
  
  SteadyStatePno_t ss;
  float ssBuf[STEADY_STATE_PNO_MAX_SAMPLES];
} PnoInstance_t;


// Private prototypes
//==============================================================================

void  _PnoInstance_Init             (PnoInstance_t *pnoi, UINT8 id, float delta, float pos, float umin, float umax);
void  _PnoInstance_SetSteadyState   (PnoInstance_t *pnoi, UINT8 samplesForSs, UINT8 oscAmp);
float _PnoInstance_ComputePos       (PnoInstance_t *pnoi);
void  _PnoInstance_SetPos           (PnoInstance_t *pnoi, float pos);
void  _PnoInstance_SetFitness       (PnoInstance_t *pnoi, float fitness);
void  _PnoInstance_Release          (PnoInstance_t *pnoi);


// Private variables
//==============================================================================

PnoInstance_t           _instances    [N_INSTANCES_TOTAL];
PnoInstanceInterface_t  _instances_if [N_INSTANCES_TOTAL];

LinkedList_t _unusedInstances       = {NULL, NULL, 0, N_INSTANCES_TOTAL};
LinkedList_t _usedInstances         = {NULL, NULL, 0, N_INSTANCES_TOTAL};
Node_t       _instancesNodes          [N_INSTANCES_TOTAL];

static BOOL _oInstanceArrayInitialized = 0;


// Private functions
//==============================================================================

void _PnoInstance_Init (PnoInstance_t *pnoi, UINT8 id, float delta, float pos, float umin, float umax)
{
  pnoi->id = id;
  pnoi->delta = delta;
  Position_Reset(&pnoi->pos);
  pnoi->pos.curPos = pos;
  pnoi->k = 1;
  pnoi->umax = umax;
  pnoi->umin = umin;
}


float _PnoInstance_ComputePos (PnoInstance_t *pnoi)
{
  SteadyStatePno_AddSample(&pnoi->ss, &pnoi->pos.curPos);
  
  if (pnoi->pos.curFitness < pnoi->pos.prevFitness)
  {
    pnoi->k = -pnoi->k;
  }
  
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
  Node_t *node = &_instancesNodes[pnoi->linkKey];
  LinkedList_RemoveNode(node->list, node);
  LinkedList_AddToEnd(&_unusedInstances, node);
  
//  SteadyState_Reset(&pnoi->steadyState);
  Position_Reset(&pnoi->pos);
    pnoi->delta
  = pnoi->id
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


// Public functions
//==============================================================================

const PnoInstanceInterface_t * PnoInstanceInterface (void)
{
  UINT8 i;
  Node_t *temp;
  
  if (!_oInstanceArrayInitialized)
  {
    _oInstanceArrayInitialized = 1;
    
    _unusedInstances.head = (void *) &_instancesNodes[0];
    _unusedInstances.count = 1;
    
    for (i = 0; i < N_INSTANCES_TOTAL; i++)
    {
      // Init the instance itself and its interface
      _instances[i].linkKey = i;
      
      _instances_if[i].ctx            = (void *)                  &_instances[i];
      _instances_if[i].Init           = (PnoiInit_fct)            &_PnoInstance_Init;
      _instances_if[i].ComputePos     = (PnoiComputePos_fct)      &_PnoInstance_ComputePos;
      _instances_if[i].SetPos         = (PnoiSetPos_fct)          &_PnoInstance_SetPos;
      _instances_if[i].SetFitness     = (PnoiSetFitness_fct)      &_PnoInstance_SetFitness;
      _instances_if[i].Release        = (PnoiRelease_fct)         &_PnoInstance_Release;
      _instances_if[i].SetSteadyState = (PnoiSetSteadyState_fct)  &_PnoInstance_SetSteadyState;
      
      // Init the linked list
      _instancesNodes[i].ctx = (void *) &_instances_if[i];
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
    return NULL;
  }
  
  temp = _unusedInstances.tail;
  LinkedList_RemoveNode(&_unusedInstances, temp);
  LinkedList_AddToEnd(&_usedInstances, temp);
  return temp->ctx;
}