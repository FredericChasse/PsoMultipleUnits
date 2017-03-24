//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : PsoSwarm.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the swarms of PSO.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __PSO_SWARM_H__
#define __PSO_SWARM_H__

#include "Setup.h"
#include "UnitArray.h"

// Public definitions
//==============================================================================

typedef struct
{
  float curPos;
  float curFitness;
  float prevPos;
  float prevFitness;
} Position_t;

typedef enum
{
  PSO_SWARM_TYPE_PARALLEL_PSO
 ,PSO_SWARM_TYPE_PSO_1D
} PsoSwarmType_t;

typedef struct
{
  PsoSwarmType_t type;
  float         c1;
  float         c2;
  float         posMin;
  float         posMax;
  float         omega;
  float         perturbAmp;
  float         sentinelMargin;
  float         steadyStateOscAmp;
  size_t        nSamplesForSteadyState;
  UINT8         minParticles;
} PsoSwarmParam_t;

typedef INT8  (*PsoSwarmInit_fct)                       (void *ctx, UnitArrayInterface_t *unitArray, PsoSwarmParam_t *param);
typedef void  (*PsoSwarmComputeGbest_fct)               (void *ctx);
typedef void  (*PsoSwarmRandomizeAllParticles_fct)      (void *ctx);
typedef void  (*PsoSwarmRandomizeCertainParticles_fct)  (void *ctx, UINT8 *idx, UINT8 nParticlesToRandomize);
typedef INT8  (*PsoSwarmAddParticle_fct)                (void *ctx, void *p);
typedef void* (*PsoSwarmGetParticle_fct)                (void *ctx, UINT8 idx);
typedef void  (*PsoSwarmRemoveParticles_fct)            (void *ctx, UINT8 *idx, UINT8 nParticles);
typedef UINT8 (*PsoSwarmCheckForPerturb_fct)            (void *ctx, UINT8 *idxPerturbed);
typedef void  (*PsoSwarmRelease_fct)                    (void *ctx);
typedef void  (*PsoSwarmGetParam_fct)                   (void *ctx, PsoSwarmParam_t *paramDest);
typedef void  (*PsoSwarmGetGbest_fct)                   (void *ctx, Position_t *gbestDest);

typedef struct
{
  void                                 *ctx;
  PsoSwarmInit_fct                      Init;
  PsoSwarmComputeGbest_fct              ComputeGbest;
  PsoSwarmRandomizeAllParticles_fct     RandomizeAllParticles;
  PsoSwarmRandomizeCertainParticles_fct RandomizeCertainParticles;
  PsoSwarmAddParticle_fct               AddParticle;
  PsoSwarmGetParticle_fct               GetParticle;
  PsoSwarmRemoveParticles_fct           RemoveParticles;
  PsoSwarmCheckForPerturb_fct           CheckForPerturb;
  PsoSwarmRelease_fct                   Release;
  PsoSwarmGetParam_fct                  GetParam;
  PsoSwarmGetGbest_fct                  GetGbest;
} PsoSwarmInterface_t;


// Public functions
//==============================================================================

const PsoSwarmInterface_t * PsoSwarmInterface (UINT8 id);

void Position_Reset (Position_t *pos);



#endif // __PSO_SWARM_H__