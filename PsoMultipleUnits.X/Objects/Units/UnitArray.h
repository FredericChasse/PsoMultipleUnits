//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : UnitArray.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the methods for the abstract Unit object.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __UNIT_ARRAY_H__
#define __UNIT_ARRAY_H__

#include "Setup.h"  

typedef void  (*UnitArrayInit_fct)                (void *ctx);
typedef void  (*UnitArraySetUnitPos_fct)          (void *ctx, UINT8 idx, float pos);
typedef float (*UnitArrayGetUnitPos_fct)          (void *ctx, UINT8 idx);
typedef void  (*UnitArrayEvalUnitPower_fct)       (void *ctx, UINT8 idx);
typedef float (*UnitArrayGetUnitPower_fct)        (void *ctx, UINT8 idx);
typedef INT8  (*UnitArrayAddUnitToArray_fct)      (void *ctx, void *unit);
typedef INT8  (*UnitArrayRemoveUnitFromArray_fct) (void *ctx, UINT8 idx);
typedef void* (*UnitArrayGetNewArray)             (void *ctx, UINT8 *id, UINT8 nUnits);
typedef void  (*UnitArrayReleaseArray)            (void *ctx);

typedef struct UnitArrayInterface
{
  void                             *ctx;
  UnitArrayInit_fct                 Init;
  UnitArraySetUnitPos_fct           SetPos;
  UnitArrayGetUnitPos_fct           GetPos;
  UnitArrayEvalUnitPower_fct        EvalPower;
  UnitArrayGetUnitPower_fct         GetPower;
  UnitArrayAddUnitToArray_fct       AddUnitToArray;
  UnitArrayRemoveUnitFromArray_fct  RemoveUnitFromArray;
  UnitArrayGetNewArray              GetNewArray;
  UnitArrayReleaseArray             ReleaseArray;
} UnitArrayInterface_t;


const UnitArrayInterface_t * UnitArrayInterface(UINT8 idx);


#endif // __UNIT_ARRAY_H__