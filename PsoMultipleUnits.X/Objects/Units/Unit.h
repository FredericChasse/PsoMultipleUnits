//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Unit.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the methods for the abstract Unit object.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __UNIT_H__
#define __UNIT_H__

#include "Setup.h"

typedef enum
{
  UNIT_TYPE_STATIC_FUNC
 ,UNIT_TYPE_PV_CELL
} UnitType_t;

typedef void  (*UnitInit_fct)           (void *ctx);
typedef INT8  (*UnitSetPos_fct)         (void *ctx, float pos);
typedef float (*UnitGetPos_fct)         (void *ctx);
typedef float (*UnitGetPower_fct)       (void *ctx);
typedef UINT8 (*UnitGetId_fct)          (void *ctx);
typedef void  (*UnitGetPosLimits_fct)   (void *ctx, float *minPos, float *maxPos);
typedef void  (*UnitSetPower_fct)       (void *ctx, float power);

typedef struct
{
  void                 *ctx;
  UnitInit_fct          Init;
  UnitSetPos_fct        SetPos;
  UnitGetPos_fct        GetPos;
  UnitGetPower_fct      GetPower;
  UnitGetId_fct         GetId;
  UnitGetPosLimits_fct  GetPosLimits;
  UnitSetPower_fct      SetPower;
} UnitInterface_t;


// Public prototypes
//==============================================================================
const UnitInterface_t * UnitInterface(UINT8 id);


#endif // __UNIT_H__