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


// Private definitions
//==============================================================================

typedef struct
{
  UINT8 id;
  UINT8 nUnits;
  UnitInterface_t *units[N_UNITS_TOTAL];
} UnitArray_t;

typedef struct
{
  UINT8 inUse;
  UnitArray_t *arraysInUse[N_UNITS_TOTAL + 1];
};


// Private prototypes
//==============================================================================
void  UnitArray_Init                (UnitArray_t *array);
void  UnitArray_SetUnitPos          (UnitArray_t *array, UINT8 idx, float pos);
float UnitArray_GetUnitPos          (UnitArray_t *array, UINT8 idx);
void  UnitArray_EvalUnitPower       (UnitArray_t *array, UINT8 idx);
float UnitArray_GetUnitPower        (UnitArray_t *array, UINT8 idx);


// Private variables
//==============================================================================

UnitArray_t _arrays[N_UNITS_TOTAL + 1] = 
{
  {0, N_UNITS_TOTAL, {0}}
 ,{1, 0, {0}}
 ,{2, 0, {0}}
 ,{3, 0, {0}}
 ,{4, 0, {0}}
 ,{5, 0, {0}}
 ,{6, 0, {0}}
 ,{7, 0, {0}}
 ,{8, 0, {0}}
};

UINT8 _arraysInUse = 1;   /*!< Used to determine how many arrays are active. */

const UnitArrayInterface_t _arrays_if[N_UNITS_TOTAL + 1] = 
{
  { (void *)                            &_arrays[0]
  , (UnitArrayInit_fct)                 &UnitArray_Init
  , (UnitArraySetUnitPos_fct)           &UnitArray_SetUnitPos
  , (UnitArrayGetUnitPos_fct)           &UnitArray_GetUnitPos
  , (UnitArrayEvalUnitPower_fct)        &UnitArray_EvalUnitPower
  , (UnitArrayGetUnitPower_fct)         &UnitArray_GetUnitPower
  , (UnitArrayAddUnitToArray_fct)       &UnitArray_AddUnitToArray
  , (UnitArrayRemoveUnitFromArray_fct)  &UnitArray_RemoveUnitFromArray
  }
 ,{ (void *)                            &_arrays[1]
  , (UnitArrayInit_fct)                 &UnitArray_Init
  , (UnitArraySetUnitPos_fct)           &UnitArray_SetUnitPos
  , (UnitArrayGetUnitPos_fct)           &UnitArray_GetUnitPos
  , (UnitArrayEvalUnitPower_fct)        &UnitArray_EvalUnitPower
  , (UnitArrayGetUnitPower_fct)         &UnitArray_GetUnitPower
  , (UnitArrayAddUnitToArray_fct)       &UnitArray_AddUnitToArray
  , (UnitArrayRemoveUnitFromArray_fct)  &UnitArray_RemoveUnitFromArray
  }
 ,{ (void *)                            &_arrays[2]
  , (UnitArrayInit_fct)                 &UnitArray_Init
  , (UnitArraySetUnitPos_fct)           &UnitArray_SetUnitPos
  , (UnitArrayGetUnitPos_fct)           &UnitArray_GetUnitPos
  , (UnitArrayEvalUnitPower_fct)        &UnitArray_EvalUnitPower
  , (UnitArrayGetUnitPower_fct)         &UnitArray_GetUnitPower
  , (UnitArrayAddUnitToArray_fct)       &UnitArray_AddUnitToArray
  , (UnitArrayRemoveUnitFromArray_fct)  &UnitArray_RemoveUnitFromArray
  }
 ,{ (void *)                            &_arrays[3]
  , (UnitArrayInit_fct)                 &UnitArray_Init
  , (UnitArraySetUnitPos_fct)           &UnitArray_SetUnitPos
  , (UnitArrayGetUnitPos_fct)           &UnitArray_GetUnitPos
  , (UnitArrayEvalUnitPower_fct)        &UnitArray_EvalUnitPower
  , (UnitArrayGetUnitPower_fct)         &UnitArray_GetUnitPower
  , (UnitArrayAddUnitToArray_fct)       &UnitArray_AddUnitToArray
  , (UnitArrayRemoveUnitFromArray_fct)  &UnitArray_RemoveUnitFromArray
  }
 ,{ (void *)                            &_arrays[4]
  , (UnitArrayInit_fct)                 &UnitArray_Init
  , (UnitArraySetUnitPos_fct)           &UnitArray_SetUnitPos
  , (UnitArrayGetUnitPos_fct)           &UnitArray_GetUnitPos
  , (UnitArrayEvalUnitPower_fct)        &UnitArray_EvalUnitPower
  , (UnitArrayGetUnitPower_fct)         &UnitArray_GetUnitPower
  , (UnitArrayAddUnitToArray_fct)       &UnitArray_AddUnitToArray
  , (UnitArrayRemoveUnitFromArray_fct)  &UnitArray_RemoveUnitFromArray
  }
 ,{ (void *)                            &_arrays[5]
  , (UnitArrayInit_fct)                 &UnitArray_Init
  , (UnitArraySetUnitPos_fct)           &UnitArray_SetUnitPos
  , (UnitArrayGetUnitPos_fct)           &UnitArray_GetUnitPos
  , (UnitArrayEvalUnitPower_fct)        &UnitArray_EvalUnitPower
  , (UnitArrayGetUnitPower_fct)         &UnitArray_GetUnitPower
  , (UnitArrayAddUnitToArray_fct)       &UnitArray_AddUnitToArray
  , (UnitArrayRemoveUnitFromArray_fct)  &UnitArray_RemoveUnitFromArray
  }
 ,{ (void *)                            &_arrays[6]
  , (UnitArrayInit_fct)                 &UnitArray_Init
  , (UnitArraySetUnitPos_fct)           &UnitArray_SetUnitPos
  , (UnitArrayGetUnitPos_fct)           &UnitArray_GetUnitPos
  , (UnitArrayEvalUnitPower_fct)        &UnitArray_EvalUnitPower
  , (UnitArrayGetUnitPower_fct)         &UnitArray_GetUnitPower
  , (UnitArrayAddUnitToArray_fct)       &UnitArray_AddUnitToArray
  , (UnitArrayRemoveUnitFromArray_fct)  &UnitArray_RemoveUnitFromArray
  }
 ,{ (void *)                            &_arrays[7]
  , (UnitArrayInit_fct)                 &UnitArray_Init
  , (UnitArraySetUnitPos_fct)           &UnitArray_SetUnitPos
  , (UnitArrayGetUnitPos_fct)           &UnitArray_GetUnitPos
  , (UnitArrayEvalUnitPower_fct)        &UnitArray_EvalUnitPower
  , (UnitArrayGetUnitPower_fct)         &UnitArray_GetUnitPower
  , (UnitArrayAddUnitToArray_fct)       &UnitArray_AddUnitToArray
  , (UnitArrayRemoveUnitFromArray_fct)  &UnitArray_RemoveUnitFromArray
  }
 ,{ (void *)                            &_arrays[8]
  , (UnitArrayInit_fct)                 &UnitArray_Init
  , (UnitArraySetUnitPos_fct)           &UnitArray_SetUnitPos
  , (UnitArrayGetUnitPos_fct)           &UnitArray_GetUnitPos
  , (UnitArrayEvalUnitPower_fct)        &UnitArray_EvalUnitPower
  , (UnitArrayGetUnitPower_fct)         &UnitArray_GetUnitPower
  , (UnitArrayAddUnitToArray_fct)       &UnitArray_AddUnitToArray
  , (UnitArrayRemoveUnitFromArray_fct)  &UnitArray_RemoveUnitFromArray
  }
};


// Private functions
//==============================================================================

void UnitArray_Init (UnitArray_t *array)
{
  UINT8 i;
  if (array->id == 0)
  {
    for (i = 0; i < N_UNITS_TOTAL; i++)
    {
      array->units[i] = (UnitInterface_t *) UnitInterface(i);
    }
  }
}


void UnitArray_SetUnitPos (UnitArray_t *array, UINT8 idx, float pos)
{
  if (idx < array->nUnits)
  {
    array->units[idx]->SetPos(array->units[idx]->ctx, pos);
  }
}


float UnitArray_GetUnitPos (UnitArray_t *array, UINT8 idx)
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


void UnitArray_EvalUnitPower (UnitArray_t *array, UINT8 idx)
{
  if (idx < array->nUnits)
  {
    return array->units[idx]->EvalPower(array->units[idx]->ctx);
  }
}


float UnitArray_GetUnitPower (UnitArray_t *array, UINT8 idx)
{
  if (idx < array->nUnits)
  {
    return array->units[idx]->GetPower(array->units[idx]->ctx);
  }
}


INT8 UnitArray_AddUnitToArray (UnitArray_t *array, UnitInterface_t *unit)
{
  if (array->nUnits == N_UNITS_TOTAL)
  {
    return -1;
  }
  array->units[array->nUnits] = unit;
  array->nUnits++;
  return 0;
}


INT8 UnitArray_RemoveUnitFromArray (UnitArray_t *array, UINT8 idx)
{
  if (idx >= array->nUnits)
  {
    return -1;
  }
  memmove(&array->units[idx], &array->units[idx + 1], sizeof(UnitInterface_t *));
  array->units[array->nUnits] = 0;
  array->nUnits--;
  return 0;
}


// Public functions
//==============================================================================
const UnitArrayInterface_t * UnitArrayInterface(UINT8 idx)
{
  if (idx > N_UNITS_TOTAL)
  {
    return 0;
  }
  
  return &_arrays_if[idx];
}