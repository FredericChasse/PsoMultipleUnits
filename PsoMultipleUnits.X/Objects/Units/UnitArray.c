//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : UnitArray.c
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the methods for the abstract Unit object.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "UnitArray.h"
#include "Unit.h"
#include "LinkedList.h"
#include "StateMachine.h"   // For debugging


// Private definitions
//==============================================================================

#define N_ARRAYS_TOTAL      ((N_UNITS_TOTAL << 2) | 1)  // N_UNITS_TOTAL*4 + 1

typedef struct
{
  UINT8 id;
  UINT8 nUnits;
  UnitInterface_t *units[N_UNITS_TOTAL];
  UINT8 linkKey;
} UnitArray_t;


// Private prototypes
//==============================================================================
void  _UnitArray_Init                 (UnitArray_t *array, UINT8 id);
void  _UnitArray_SetUnitPos           (UnitArray_t *array, UINT8 idx, float pos);
void  _UnitArray_SetUnitPosIdx        (UnitArray_t *array, UINT8 idx, UINT8 pos);
float _UnitArray_GetUnitPos           (UnitArray_t *array, UINT8 idx);
float _UnitArray_GetUnitPower         (UnitArray_t *array, UINT8 idx);
INT8  _UnitArray_AddUnitToArray       (UnitArray_t *array, UnitInterface_t *unit);
INT8  _UnitArray_RemoveUnitFromArray  (UnitArray_t *array, UINT8 idx);
UINT8 _UnitArray_GetNUnits            (UnitArray_t *array);
void* _UnitArray_GetUnitHandle        (UnitArray_t *array, UINT8 idx);
void  _UnitArray_Release              (UnitArray_t *array);
void  _UnitArray_GetPosLimits         (UnitArray_t *array, float *minPos, float *maxPos);
void  _UnitArray_GetUnitLimitsIdx     (UnitArray_t *array, UINT8 id, UINT8 *minPos, UINT8 *maxPos);
void  _UnitArray_SetPower             (UnitArray_t *array, UINT8 idx, float power);
UINT8 _UnitArray_GetUnitPosIdx        (UnitArray_t *array, UINT8 idx);
UINT8 _UnitArray_GetUnitId            (UnitArray_t *array, UINT8 idx);
void* _UnitArray_CreateSubArray       (UnitArray_t *array, UINT8 *idx, UINT8 nIdx);


// Private variables
//==============================================================================

static BOOL oArraysInitialized = 0;

UnitArray_t _arrays[N_ARRAYS_TOTAL];

UnitArrayInterface_t _arrays_if[N_ARRAYS_TOTAL];


LinkedList_t _unusedArrays = {NULL, NULL, 0, N_ARRAYS_TOTAL};
LinkedList_t _usedArrays   = {NULL, NULL, 0, N_ARRAYS_TOTAL};
Node_t       _arraysNodes[N_ARRAYS_TOTAL];


// Private functions
//==============================================================================

void _UnitArray_Init (UnitArray_t *array, UINT8 id)
{
  UINT8 i;
  array->id = id;
  if (array->linkKey == (N_ARRAYS_TOTAL - 1))
  {
    for (i = 0; i < N_UNITS_TOTAL; i++)
    {
      array->units[i] = (UnitInterface_t *) UnitInterface(i);
      array->units[i]->Init(array->units[i]->ctx);
    }
    array->nUnits = N_UNITS_TOTAL;
  }
}


void _UnitArray_SetPower (UnitArray_t *array, UINT8 idx, float power)
{
  array->units[idx]->SetPower(array->units[idx]->ctx, power);
}


void _UnitArray_GetPosLimits (UnitArray_t *array, float *minPos, float *maxPos)
{
  array->units[0]->GetPosLimits(array->units[0]->ctx, minPos, maxPos);
}


void _UnitArray_GetUnitLimitsIdx (UnitArray_t *array, UINT8 id, UINT8 *minPos, UINT8 *maxPos)
{
  array->units[id]->GetPosLimitsIdx(array->units[id]->ctx, minPos, maxPos);
}


void _UnitArray_Release (UnitArray_t *array)
{
  INT16 i;
  Node_t *node = &_arraysNodes[array->linkKey];
  LinkedList_RemoveNode(node->list, node);
  LinkedList_AddToEnd(&_unusedArrays, node);
  
  for (i = array->nUnits-1; i >= 0; i--)
  {
    _UnitArray_RemoveUnitFromArray(array, i);
  }
}


UINT8 _UnitArray_GetNUnits (UnitArray_t *array)
{
  return array->nUnits;
}


void _UnitArray_SetUnitPosIdx (UnitArray_t *array, UINT8 idx, UINT8 pos)
{
  if (idx < array->nUnits)
  {
    array->units[idx]->SetPosWithIdx(array->units[idx]->ctx, pos);
  }
}


void _UnitArray_SetUnitPos (UnitArray_t *array, UINT8 idx, float pos)
{
  __assert(idx < array->nUnits);
  array->units[idx]->SetPos(array->units[idx]->ctx, pos);
}


void * _UnitArray_GetUnitHandle (UnitArray_t *array, UINT8 idx)
{
  if (idx >= array->nUnits)
  {
    return 0;
  }
  return (void *) array->units[idx];
}


float _UnitArray_GetUnitPos (UnitArray_t *array, UINT8 idx)
{
  if (idx < array->nUnits)
  {
    return array->units[idx]->GetPos(array->units[idx]->ctx);
  }
  else
  {
    return -1;
  }
}


float _UnitArray_GetUnitPower (UnitArray_t *array, UINT8 idx)
{
  if (idx < array->nUnits)
  {
    return array->units[idx]->GetPower(array->units[idx]->ctx);
  }
}


UINT8 _UnitArray_GetUnitId (UnitArray_t *array, UINT8 idx)
{
  return array->units[idx]->GetId(array->units[idx]->ctx);
}


UINT8 _UnitArray_GetUnitPosIdx (UnitArray_t *array, UINT8 idx)
{
  return array->units[idx]->GetPosIdx(array->units[idx]->ctx);
}


INT8 _UnitArray_AddUnitToArray (UnitArray_t *array, UnitInterface_t *unit)
{
  if (array->nUnits == N_UNITS_TOTAL)
  {
    return -1;
  }
  array->units[array->nUnits] = unit;
  array->nUnits++;
  return 0;
}


void * _UnitArray_CreateSubArray (UnitArray_t *array, UINT8 *idx, UINT8 nIdx)
{
  if (nIdx > array->nUnits)
  {
    return NULL;
  }
  UINT8 i;
  UnitArrayInterface_t *newArray = (UnitArrayInterface_t *) UnitArrayInterface();
  
  for (i = 0; i < nIdx; i++)
  {
    _UnitArray_AddUnitToArray((UnitArray_t *) newArray->ctx, (UnitInterface_t *) _UnitArray_GetUnitHandle(array, idx[i]));
  }
  
  return (void *) newArray;
}


INT8 _UnitArray_RemoveUnitFromArray (UnitArray_t *array, UINT8 idx)
{
  UINT8 len;
  
  if (idx >= array->nUnits)
  {
    return -1;
  }
  if (idx == array->nUnits)
  {
    array->units[array->nUnits-1] = 0;
    array->nUnits--;
  }
  else
  {
    len = array->nUnits - idx - 1;
    memmove(&array->units[idx], &array->units[idx + 1], len * sizeof(UnitInterface_t *));
    array->units[array->nUnits-1] = 0;
    array->nUnits--;
  }
  return 0;
}


// Public functions
//==============================================================================
const UnitArrayInterface_t * UnitArrayInterface(void)
{
  UINT8 i;
  Node_t *temp;
  
  if (!oArraysInitialized)
  {
    oArraysInitialized = 1;
    
    _unusedArrays.head = (void *) &_arraysNodes[0];
    _unusedArrays.count = 1;
    
    for (i = 0; i < N_ARRAYS_TOTAL; i++)
    {
      // Init the array itself and its interface
      _arrays[i].linkKey                = i;
      _arrays[i].nUnits                 = 0;
      _arrays[i].id                     = 0;
      
      _arrays_if[i].ctx                 = (void *)                            &_arrays[i];
      _arrays_if[i].Init                = (UnitArrayInit_fct)                 &_UnitArray_Init;
      _arrays_if[i].SetPos              = (UnitArraySetUnitPos_fct)           &_UnitArray_SetUnitPos;
      _arrays_if[i].GetPos              = (UnitArrayGetUnitPos_fct)           &_UnitArray_GetUnitPos;
      _arrays_if[i].GetPower            = (UnitArrayGetUnitPower_fct)         &_UnitArray_GetUnitPower;
      _arrays_if[i].AddUnitToArray      = (UnitArrayAddUnitToArray_fct)       &_UnitArray_AddUnitToArray;
      _arrays_if[i].RemoveUnitFromArray = (UnitArrayRemoveUnitFromArray_fct)  &_UnitArray_RemoveUnitFromArray;
      _arrays_if[i].GetNUnits           = (UnitArrayGetNUnits_fct)            &_UnitArray_GetNUnits;
      _arrays_if[i].GetUnitHandle       = (UnitArrayGetUnitHandle_fct)        &_UnitArray_GetUnitHandle;
      _arrays_if[i].Release             = (UnitArrayRelease_fct)              &_UnitArray_Release;
      _arrays_if[i].GetPosLimits        = (UnitArrayGetPosLimits_fct)         &_UnitArray_GetPosLimits;
      _arrays_if[i].GetUnitLimitsIdx    = (UnitArrayGetUnitLimitsIdx_fct)     &_UnitArray_GetUnitLimitsIdx;
      _arrays_if[i].SetPower            = (UnitArraySetPower_fct)             &_UnitArray_SetPower;
      _arrays_if[i].GetUnitPosIdx       = (UnitArrayGetUnitPosIdx_fct)        &_UnitArray_GetUnitPosIdx;
      _arrays_if[i].GetUnitId           = (UnitArrayGetUnidId_fct)            &_UnitArray_GetUnitId;
      _arrays_if[i].CreateSubArray      = (UnitArrayCreateSubArray_fct)       &_UnitArray_CreateSubArray;
      _arrays_if[i].SetPosIdx           = (UnitArraySetUnitPosIdx_fct)        &_UnitArray_SetUnitPosIdx;
      
      // Init the linked list
      _arraysNodes[i].ctx = (void *) &_arrays_if[i];
      _arraysNodes[i].key = i;
      if (i < (N_ARRAYS_TOTAL - 1))
      {
        _arraysNodes[i].next = &_arraysNodes[i + 1];
      }
      else
      {
        _arraysNodes[i].next = NULL;
      }
    }
    LinkedList_Init(&_unusedArrays, &_arraysNodes[0]);
  }
  
  if (_unusedArrays.count == 0)
  {
    return NULL;
  }
  
  temp = _unusedArrays.tail;
  LinkedList_RemoveNode(&_unusedArrays, temp);
  LinkedList_AddToEnd(&_usedArrays, temp);
  return temp->ctx;
}