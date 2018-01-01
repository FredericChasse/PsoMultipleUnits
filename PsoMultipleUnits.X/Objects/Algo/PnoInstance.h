//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : PnoInstance.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the methods for an instance of P&O.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __PNO_INSTANCE_H__
#define __PNO_INSTANCE_H__

#include "Setup.h"
#include "UnitArray.h"

// Public definitions
//==============================================================================

typedef enum
{
  PNO_TYPE_CLASSIC
 ,PNO_TYPE_SWARM
} PnoType_t;

typedef void  (*PnoiInit_fct)             (void *ctx, UINT8 delta, UINT8 pos, UINT8 umin, UINT8 umax, float perturbOsc);
typedef float (*PnoiComputePos_fct)       (void *ctx, UINT8 *oPerturbed);
typedef void  (*PnoiSetPos_fct)           (void *ctx, float pos);
typedef void  (*PnoiSetPosIdx_fct)        (void *ctx, UINT8 pos);
typedef float (*PnoiGetPos_fct)           (void *ctx);
typedef UINT8 (*PnoiGetPosIdx_fct)        (void *ctx);
typedef void  (*PnoiSetFitness_fct)       (void *ctx, float fitness);
typedef void  (*PnoiRelease_fct)          (void *ctx);
typedef void  (*PnoiSetSteadyState_fct)   (void *ctx, UINT8 nSamplesForSs, UINT8 oscAmp);
typedef BOOL  (*PnoiGetSteadyState_fct)   (void *ctx);
typedef void  (*PnoiSetId_fct)            (void *ctx, UINT8 id);

typedef struct
{
  PnoiInit_fct            Init;
  PnoiComputePos_fct      ComputePos;
  PnoiSetPos_fct          SetPos;
  PnoiSetPosIdx_fct       SetPosIdx;
  PnoiGetPos_fct          GetPos;
  PnoiGetPosIdx_fct       GetPosIdx;
  PnoiSetFitness_fct      SetFitness;
  PnoiRelease_fct         Release;
  PnoiSetSteadyState_fct  SetSteadyState;
  PnoiGetSteadyState_fct  GetSteadyState;
  PnoiSetId_fct           SetId;
} PnoInstanceInterface_t;


// Public functions
//==============================================================================

const PnoInstanceInterface_t * PnoInstanceInterface (PnoType_t type);

size_t PnoInstance_GetNUsedInstances (void);

#endif // __PNO_INSTANCE_H__