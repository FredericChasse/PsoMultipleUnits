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

typedef enum
{
  PNOI_VAL_POS1
 ,PNOI_VAL_POS2
 ,PNOI_VAL_POS3
} PnoiValidationState_t;

typedef struct
{
  PnoInstanceInterface_t base;
  
  UINT8 id;
  Position_t pos;
  Position_t pbestAbs;
  
  float umin;
  float umax;
  INT8  k;
  float delta;
  float deltaMem;
  INT16 deltaMemInt;
  float perturbOsc;
  
  BOOL  oFirstSteadyState;
  BOOL  oLastPos;
  UINT8 divCount;
  UINT8 divCountMax;
  
  INT16 prevPotIdx;
  INT16 curPotIdx;
  INT16 delta_int;
  UINT8 umin_int;
  UINT8 umax_int;
  
  BOOL  oInValMode;
  Position_t valPos1;
  Position_t valPos2;
  Position_t valPos3;
  Position_t valPosMem;
  PnoiValidationState_t valState;
  float deltaVal;
  INT16 deltaVal_int;
  BOOL oCheckingPerturb;
  
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
float _PnoInstance_ComputePosOeppcd   (PnoInstance_t *pnoi, UINT8 *oPerturbed);
void  _PnoInstance_SetPos             (PnoInstance_t *pnoi, float pos);
float _PnoInstance_GetPos             (PnoInstance_t *pnoi);
void  _PnoInstance_SetPosIdx          (PnoInstance_t *pnoi, UINT8 pos);
UINT8 _PnoInstance_GetPosIdx          (PnoInstance_t *pnoi);
void  _PnoInstance_SetFitness         (PnoInstance_t *pnoi, float fitness);
void  _PnoInstance_Release            (PnoInstance_t *pnoi);
void  _PnoInstance_SetId              (PnoInstance_t *pnoi, UINT8 id);
float _PnoInstance_GetBestPos         (PnoInstance_t *pnoi);
void  _PnoInstance_SetDelta           (PnoInstance_t *pnoi, UINT8 delta);
void  _PnoInstance_ResetSteadyState   (PnoInstance_t *pnoi);


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
  pnoi->deltaMem = pnoi->delta;
  pnoi->deltaMemInt = pnoi->delta_int;
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
  pnoi->oFirstSteadyState = 1;
  pnoi->oLastPos = 0;
  pnoi->divCount = 0;
  pnoi->divCountMax = 40;
  
  pnoi->oInValMode = 0;
  Position_Reset(&pnoi->valPos1);
  Position_Reset(&pnoi->valPos2);
  Position_Reset(&pnoi->valPos3);
  Position_Reset(&pnoi->valPosMem);
  pnoi->valState = PNOI_VAL_POS1;
  pnoi->deltaVal_int = 4;
  pnoi->deltaVal = pnoi->deltaVal_int*POT_STEP_VALUE;
  pnoi->oCheckingPerturb = 0;
  
  pnoi->pbestAbs.curPos = pnoi->pbestAbs.prevPos = pnoi->pos.curPos;
  pnoi->pbestAbs.curFitness = pnoi->pbestAbs.prevFitness = 0;
}


void _PnoInstance_SetDelta (PnoInstance_t *pnoi, UINT8 delta)
{
  pnoi->delta_int = delta;
  pnoi->delta = delta*POT_STEP_VALUE;
  pnoi->ss.delta = pnoi->delta;
}


void _PnoInstance_ResetSteadyState   (PnoInstance_t *pnoi)
{
  SteadyStatePno_Reset(&pnoi->ss);
}


void _PnoInstance_SetId (PnoInstance_t *pnoi, UINT8 id)
{
  pnoi->id = id;
}


float _PnoInstance_GetBestPos (PnoInstance_t *pnoi)
{
  float value = SteadyStatePno_GetMeanPos(&pnoi->ss);
  ComputePotValueDec2Float(ComputePotValueFloat2Dec(value), &value);
  return value;
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
    pnoi->k = -ABS(pnoi->k);
  }
  if (pnoi->curPotIdx < pnoi->umin_int)
  {
    pnoi->curPotIdx = pnoi->umin_int;
    pnoi->k = -ABS(pnoi->k);
  }
  pnoi->pos.prevPos = pnoi->pos.curPos;
  pnoi->pos.curPos += potRealValues[pnoi->curPotIdx];
  
  return (float) pnoi->curPotIdx;
}


float _PnoInstance_ComputePosOeppcd (PnoInstance_t *pnoi, UINT8 *oPerturbed)
{
  float error;
  size_t i;
  *oPerturbed = 0;
  
  SteadyStatePno_AddSample(&pnoi->ss, &pnoi->pos.curPos);
  
  if (pnoi->pos.curFitness > pnoi->pbestAbs.curFitness)
  {
    pnoi->pbestAbs.prevFitness = pnoi->pbestAbs.curFitness;
    pnoi->pbestAbs.prevPos = pnoi->pbestAbs.curPos;
    pnoi->pbestAbs.curFitness = pnoi->pos.curFitness;
    pnoi->pbestAbs.curPos = pnoi->pos.curPos;
  }
  
  if (pnoi->pos.curFitness < pnoi->pos.prevFitness)
  {
    pnoi->k = -pnoi->k;
  }
  
  SteadyStatePno_CheckForSteadyState(&pnoi->ss);
  
  if (pnoi->oInValMode)
  {
    switch(pnoi->valState)
    {
      case PNOI_VAL_POS1:
        pnoi->valPos1.curFitness = pnoi->pos.curFitness;
        pnoi->pos.prevPos = pnoi->pos.curPos;
        pnoi->pos.curPos = pnoi->valPos2.curPos;
        pnoi->valState = PNOI_VAL_POS2;
        break;
      case PNOI_VAL_POS2:
        pnoi->valPos2.curFitness = pnoi->pos.curFitness;
        pnoi->pos.prevPos = pnoi->pos.curPos;
        pnoi->pos.curPos = pnoi->valPos3.curPos;
        pnoi->valState = PNOI_VAL_POS3;
        break;
      case PNOI_VAL_POS3:
        pnoi->valState = PNOI_VAL_POS1;
        pnoi->valPos3.curFitness = pnoi->pos.curFitness;
        
        // Position in an optimum
        if ( (pnoi->valPos1.curFitness <= pnoi->valPos3.curFitness) && (pnoi->valPos2.curFitness <= pnoi->valPos3.curFitness) )
        {
          pnoi->oCheckingPerturb = 0;
          pnoi->pos.prevPos = pnoi->pos.curPos;
          pnoi->oLastPos = 1;
          pnoi->oFirstSteadyState = 0;
          for (i = 0; i < pnoi->ss.bufSize; i++)
          {
            SteadyStatePno_AddSample(&pnoi->ss, &pnoi->pos.curPos);
          }
          SteadyStatePno_CheckForSteadyState(&pnoi->ss);
        }
        else
        {
          pnoi->delta = pnoi->deltaMem;
          pnoi->delta_int = pnoi->deltaMemInt;
          SteadyStatePno_Init (&pnoi->ss, pnoi->ssBuf, pnoi->ss.bufSize, pnoi->ss.oscAmp, pnoi->delta);
          pnoi->oLastPos = 0;
          pnoi->oFirstSteadyState = 1;
          if (pnoi->oCheckingPerturb)
          {
            pnoi->oCheckingPerturb = 0;
            *oPerturbed = 1;
          }
        }
        pnoi->pbestAbs.curFitness = pnoi->pos.curFitness;
        pnoi->pbestAbs.curPos = pnoi->pos.curPos;
        
        // Check perturb
        error = (pnoi->pos.curFitness - pnoi->valPosMem.curFitness)/pnoi->valPosMem.curFitness;
        if ( ABS(error) > pnoi->perturbOsc )
        {
          *oPerturbed = 1;
        }
        Position_Reset(&pnoi->valPos1);
        Position_Reset(&pnoi->valPos2);
        Position_Reset(&pnoi->valPos3);
        Position_Reset(&pnoi->valPosMem);
        pnoi->oInValMode = 0;
        break;
      default:
        break;
    }
  }
  else if (pnoi->ss.oInSteadyState)
  {
    if (pnoi->oFirstSteadyState)
    {
      pnoi->oFirstSteadyState = 0;
      pnoi->deltaMemInt = pnoi->delta_int;
      pnoi->deltaMem = pnoi->delta;
      pnoi->delta_int = 1;
      pnoi->delta = pnoi->delta_int * POT_STEP_VALUE;
      SteadyStatePno_Init (&pnoi->ss, pnoi->ssBuf, pnoi->ss.bufSize, pnoi->ss.oscAmp, pnoi->delta);
      pnoi->pos.prevPos = pnoi->pos.curPos;
      pnoi->pos.curPos += pnoi->delta*pnoi->k;
      if (pnoi->pos.curPos > pnoi->umax)
      {
        pnoi->pos.curPos = pnoi->umax;
        pnoi->k = -ABS(pnoi->k);
      }
      if (pnoi->pos.curPos < pnoi->umin)
      {
        pnoi->pos.curPos = pnoi->umin;
        pnoi->k = ABS(pnoi->k);
      }
    }
    else
    {
      if (!pnoi->oLastPos)
      {
        pnoi->oLastPos = 1;
        pnoi->pos.prevPos = pnoi->pos.curPos;
        pnoi->pos.curPos = pnoi->pbestAbs.curPos;
      }
      else
      {
        if (pnoi->pos.prevPos == pnoi->pos.curPos)
        {
          error = (pnoi->pos.curFitness - pnoi->pos.prevFitness)/pnoi->pos.prevFitness;
          if ( ABS(error) > pnoi->perturbOsc )
          {
//            *oPerturbed = 1;
            pnoi->oCheckingPerturb = 1;
          }
//          if (++pnoi->divCount == pnoi->divCountMax)
          if ((++pnoi->divCount == pnoi->divCountMax) || pnoi->oCheckingPerturb)
          {
            pnoi->divCount = 0;
            pnoi->oInValMode = 1;
            pnoi->pos.prevPos = pnoi->pos.curPos;
            pnoi->valPosMem.curPos = pnoi->pos.curPos;
            pnoi->valPosMem.curFitness = pnoi->pos.curFitness;
            pnoi->valPos1.curPos = pnoi->pos.curPos - pnoi->deltaVal;
            pnoi->valPos2.curPos = pnoi->pos.curPos + pnoi->deltaVal;
            pnoi->valPos3.curPos = pnoi->pos.curPos;
            pnoi->valState = PNOI_VAL_POS1;
            pnoi->pos.curPos = pnoi->valPos1.curPos;
          }
        }
        pnoi->pos.prevPos = pnoi->pos.curPos;
      }
    }
  }
  else
  {
    pnoi->pos.prevPos = pnoi->pos.curPos;
    pnoi->pos.curPos += pnoi->delta*pnoi->k;
    if (pnoi->pos.curPos > pnoi->umax)
    {
      pnoi->pos.curPos = pnoi->umax;
      pnoi->k = -ABS(pnoi->k);
    }
    if (pnoi->pos.curPos < pnoi->umin)
    {
      pnoi->pos.curPos = pnoi->umin;
      pnoi->k = ABS(pnoi->k);
    }
  }
  
  return pnoi->pos.curPos;
}


float _PnoInstance_ComputePosSwarm (PnoInstance_t *pnoi, UINT8 *oPerturbed)
{
  float error;
  SteadyStatePno_AddSample(&pnoi->ss, &pnoi->pos.curPos);
  
  if (pnoi->pos.curFitness > pnoi->pbestAbs.curFitness)
  {
    pnoi->pbestAbs.prevFitness = pnoi->pbestAbs.curFitness;
    pnoi->pbestAbs.prevPos = pnoi->pbestAbs.curPos;
    pnoi->pbestAbs.curFitness = pnoi->pos.curFitness;
    pnoi->pbestAbs.curPos = pnoi->pos.curPos;
  }
  
  if (pnoi->pos.curFitness < pnoi->pos.prevFitness)
  {
    pnoi->k = -pnoi->k;
  }
  
  SteadyStatePno_CheckForSteadyState(&pnoi->ss);
  
  if (pnoi->ss.oInSteadyState)
  {
    if (pnoi->oFirstSteadyState)
    {
      *oPerturbed = 0;
      pnoi->oFirstSteadyState = 0;
      SteadyStatePno_Reset(&pnoi->ss);
      pnoi->delta_int = 1;
      pnoi->delta = pnoi->delta_int * POT_STEP_VALUE;
      SteadyStatePno_Init (&pnoi->ss, pnoi->ssBuf, pnoi->ss.bufSize, pnoi->ss.oscAmp, pnoi->delta);
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
      if (!pnoi->oLastPos)
      {
        *oPerturbed = 0;
        pnoi->oLastPos = 1;
        pnoi->pos.prevPos = pnoi->pos.curPos;
//        pnoi->pos.curPos = _PnoInstance_GetBestPos(pnoi);
        pnoi->pos.curPos = pnoi->pbestAbs.curPos;
      }
      else
      {
        if (pnoi->pos.prevPos == pnoi->pos.curPos)
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
//        if (++pnoi->divCount == pnoi->divCountMax)
//        {
//          pnoi->divCount = 0;
//          SteadyStatePno_Reset(&pnoi->ss);
//          pnoi->oLastPos = 0;
//          pnoi->pos.prevPos = pnoi->pos.curPos;
//          pnoi->pos.curPos += pnoi->delta*pnoi->k;
//          if (pnoi->pos.curPos > pnoi->umax)
//          {
//            pnoi->pos.curPos = pnoi->umax;
//            pnoi->k = -ABS(pnoi->k);
//          }
//          if (pnoi->pos.curPos < pnoi->umin)
//          {
//            pnoi->pos.curPos = pnoi->umin;
//            pnoi->k = ABS(pnoi->k);
//          }
//        }
//        else
//        {
//          pnoi->pos.prevPos = pnoi->pos.curPos;
//        }
      }
    }
  }
  else
  {
    *oPerturbed = 0;
    pnoi->pos.prevPos = pnoi->pos.curPos;
    pnoi->pos.curPos += pnoi->delta*pnoi->k;
    if (pnoi->pos.curPos > pnoi->umax)
    {
      pnoi->pos.curPos = pnoi->umax;
      pnoi->k = -ABS(pnoi->k);
    }
    if (pnoi->pos.curPos < pnoi->umin)
    {
      pnoi->pos.curPos = pnoi->umin;
      pnoi->k = ABS(pnoi->k);
    }
  }
  
//  if (!SteadyStatePno_CheckForSteadyState(&pnoi->ss))
//  {
//    *oPerturbed = 0;
//    pnoi->pos.prevPos = pnoi->pos.curPos;
//    pnoi->pos.curPos += pnoi->delta*pnoi->k;
//    if (pnoi->pos.curPos > pnoi->umax)
//    {
//      pnoi->pos.curPos = pnoi->umax;
//    }
//    if (pnoi->pos.curPos < pnoi->umin)
//    {
//      pnoi->pos.curPos = pnoi->umin;
//    }
//  }
//  else
//  {
//    if (pnoi->pos.curPos == pnoi->pos.prevPos)
//    {
//      error = (pnoi->pos.curFitness - pnoi->pos.prevFitness)/pnoi->pos.prevFitness;
//      if ( ABS(error) > pnoi->perturbOsc )
//      {
//        *oPerturbed = 1;
//      }
//      else
//      {
//        *oPerturbed = 0;
//      }
//    }
//    else
//    {
//      *oPerturbed = 0;
//    }
//    pnoi->pos.prevPos = pnoi->pos.curPos;
//  }
  
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
  INT8 ret;
  Node_t *node = LinkedList_FindNode(&_usedInstances, pnoi);
  __assert(node, "_PnoInstance_Release");
  
  ret = LinkedList_RemoveNode(node->list, node);
  __assert(ret == 0, "_PnoInstance_Release remove node");
  
  ret = LinkedList_AddToEnd(&_unusedInstances, node);
  __assert(ret==0, "_PnoInstance_Release add to end");
  
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

const PnoInstanceInterface_t * PnoInstanceInterface (PnoInstanceType_t type)
{
  UINT8 i;
  Node_t *temp;
  
  if (type > PNO_TYPE_OEPPCD)
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
      _instances[i].base.GetBestPos     = (PnoiGetBestPos_fct)      &_PnoInstance_GetBestPos;
      _instances[i].base.SetDelta       = (PnoiSetDelta_fct)        &_PnoInstance_SetDelta;
      _instances[i].base.ResetSs        = (PnoiResetSs_fct)         &_PnoInstance_ResetSteadyState;
      
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
  
  
  if (type == PNO_TYPE_CLASSIC)
  {
    ((PnoInstance_t *) temp->ctx)->base.ComputePos = (PnoiComputePos_fct) &_PnoInstance_ComputePosClassic;
  }
  else if (type == PNO_TYPE_SWARM)
  {
    ((PnoInstance_t *) temp->ctx)->base.ComputePos = (PnoiComputePos_fct) &_PnoInstance_ComputePosSwarm;
  }
  else
  {
    ((PnoInstance_t *) temp->ctx)->base.ComputePos = (PnoiComputePos_fct) &_PnoInstance_ComputePosOeppcd;
  }
  return temp->ctx;
}

size_t PnoInstance_GetNUsedInstances (void)
{
  return _usedInstances.count;
}