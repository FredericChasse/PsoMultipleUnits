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

typedef void  (*UnitArrayInit_fct)                (void *ctx, UINT8 id);
typedef void  (*UnitArraySetUnitPos_fct)          (void *ctx, UINT8 idx, float pos);
typedef void  (*UnitArraySetUnitPosIdx_fct)       (void *ctx, UINT8 idx, UINT8 pos);
typedef float (*UnitArrayGetUnitPos_fct)          (void *ctx, UINT8 idx);
typedef float (*UnitArrayGetUnitPower_fct)        (void *ctx, UINT8 idx);
typedef INT8  (*UnitArrayAddUnitToArray_fct)      (void *ctx, void *unit);
typedef INT8  (*UnitArrayRemoveUnitFromArray_fct) (void *ctx, UINT8 idx);
typedef UINT8 (*UnitArrayGetNUnits_fct)           (void *ctx);
typedef void* (*UnitArrayGetUnitHandle_fct)       (void *ctx, UINT8 idx);
typedef void  (*UnitArrayRelease_fct)             (void *ctx);
typedef void  (*UnitArrayGetPosLimits_fct)        (void *ctx, float *minPos, float *maxPos);
typedef void  (*UnitArraySetPower_fct)            (void *ctx, UINT8 idx, float power);
typedef UINT8 (*UnitArrayGetUnitPosIdx_fct)       (void *ctx, UINT8 idx);
typedef UINT8 (*UnitArrayGetUnidId_fct)           (void *ctx, UINT8 idx);
typedef void* (*UnitArrayCreateSubArray_fct)      (void *ctx, UINT8 *idx, UINT8 nIdx);

typedef struct UnitArrayInterface
{
  void                             *ctx;
  UnitArrayInit_fct                 Init;
  UnitArraySetUnitPosIdx_fct        SetPosIdx;
  UnitArraySetUnitPos_fct           SetPos;
  UnitArrayGetUnitPos_fct           GetPos;
  UnitArrayGetUnitPower_fct         GetPower;
  UnitArrayAddUnitToArray_fct       AddUnitToArray;
  UnitArrayRemoveUnitFromArray_fct  RemoveUnitFromArray;
  UnitArrayGetNUnits_fct            GetNUnits;
  UnitArrayGetUnitHandle_fct        GetUnitHandle;
  UnitArrayRelease_fct              Release;
  UnitArrayGetPosLimits_fct         GetPosLimits;
  UnitArraySetPower_fct             SetPower;
  UnitArrayGetUnitPosIdx_fct        GetUnitPosIdx;
  UnitArrayGetUnidId_fct            GetUnitId;
  UnitArrayCreateSubArray_fct       CreateSubArray;
} UnitArrayInterface_t;


const UnitArrayInterface_t * UnitArrayInterface(void);

size_t UnitArray_GetNUsedArrays (void);

#endif // __UNIT_ARRAY_H__