//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : PnoSwarm.h
// Author  : Frederic Chasse
// Date    : 2017-04-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the methods needed for the Perturb & Observe
//           swarm algorithm used with PPSO-P&O.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __PNO_SWARM_H__
#define __PNO_SWARM_H__

#include "Setup.h"
#include "UnitArray.h"

// Public definitions
//==============================================================================

typedef struct
{
  float         delta;
  float         umin;
  float         umax;
  float         uinit;
  UINT8         nSamplesForSs;
  UINT8         oscAmp;
  float         perturbOsc;
} PnoSwarmParam_t;

typedef INT8  (*PnoSwarmInit_fct)             (void *ctx, UnitArrayInterface_t *unitArray, PnoSwarmParam_t *param, UINT8 id);
typedef UINT8 (*PnoSwarmComputeAllPos_fct)    (void *ctx, float *newPos, UINT8 *idxPerturbed);
typedef void  (*PnoSwarmRemoveInstances_fct)  (void *ctx, UINT8 *idx, UINT8 nInstances);
typedef BOOL  (*PnoSwarmGetSteadyState_fct)   (void *ctx, UINT8 idx);
typedef void  (*PnoSwarmRelease_fct)          (void *ctx);
typedef void  (*PnoSwarmSetPos_fct)           (void *ctx, UINT8 idx, float pos);
typedef void  (*PnoSwarmSetFitness_fct)       (void *ctx, UINT8 idx, float fitness);
typedef UINT8 (*PnoSwarmGetNInstances_fct)    (void *ctx);
typedef void  (*PnoSwarmIncIteration_fct)     (void *ctx);
typedef void* (*PnoSwarmGetArray_fct)         (void *ctx);
typedef void  (*PnoSwarmSetId_fct)            (void *ctx, UINT8 id);

typedef struct
{
  void                       *ctx;
  PnoSwarmInit_fct            Init;
  PnoSwarmComputeAllPos_fct   ComputeAllPos;
  PnoSwarmRemoveInstances_fct RemoveInstances;
  PnoSwarmRelease_fct         Release;
  PnoSwarmGetSteadyState_fct  GetSteadyState;
  PnoSwarmSetPos_fct          SetPos;
  PnoSwarmGetNInstances_fct   GetNInstances;
  PnoSwarmSetFitness_fct      SetFitness;
  PnoSwarmIncIteration_fct    IncIteration;
  PnoSwarmGetArray_fct        GetArray;
  PnoSwarmSetId_fct           SetId;
} PnoSwarmInterface_t;

// Public functions
//==============================================================================

const PnoSwarmInterface_t * PnoSwarmInterface (void);



#endif // __PNO_SWARM_H__