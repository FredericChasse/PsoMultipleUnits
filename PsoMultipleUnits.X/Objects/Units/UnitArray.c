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


// Private definitions
//==============================================================================

#define N_ARRAYS_TOTAL      ((N_UNITS_TOTAL << 1) | 1)  // N_UNITS_TOTAL*2 + 1

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
float _UnitArray_GetUnitPos           (UnitArray_t *array, UINT8 idx);
float _UnitArray_GetUnitPower         (UnitArray_t *array, UINT8 idx);
INT8  _UnitArray_AddUnitToArray       (UnitArray_t *array, UnitInterface_t *unit);
INT8  _UnitArray_RemoveUnitFromArray  (UnitArray_t *array, UINT8 idx);
UINT8 _UnitArray_GetNUnits            (UnitArray_t *array);
void* _UnitArray_GetUnitHandle        (UnitArray_t *array, UINT8 idx);
void  _UnitArray_Release              (UnitArray_t *array);


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
  if (array->linkKey == 0)
  {
    for (i = 0; i < N_UNITS_TOTAL; i++)
    {
      array->units[i] = (UnitInterface_t *) UnitInterface(i);
    }
    array->nUnits = N_UNITS_TOTAL;
  }
}


void _UnitArray_Release (UnitArray_t *array)
{
  Node_t *node = &_arraysNodes[array->linkKey];
  LinkedList_RemoveNode(node->list, node);
  LinkedList_AddToEnd(&_unusedArrays, node);
}


UINT8 _UnitArray_GetNUnits (UnitArray_t *array)
{
  return array->nUnits;
}


void _UnitArray_SetUnitPos (UnitArray_t *array, UINT8 idx, float pos)
{
  if (idx < array->nUnits)
  {
    array->units[idx]->SetPos(array->units[idx]->ctx, pos);
  }
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


void _UnitArray_SplitInto2Arrays (UnitArray_t *array, UINT8 *idxToSplit, UINT8 nToSplit, void *splitArray, void *keepArray)
{
  UnitInterface_t *unitsToKeep[N_UNITS_TOTAL];
  UnitInterface_t *unitsToSplit[N_UNITS_TOTAL];
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
      _arrays[i].linkKey = i;
      _UnitArray_Init(&_arrays[i], 0);
      
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