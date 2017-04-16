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

#define PSO_SWARM_MAX_PARTICLES   (10)
#define N_SWARMS_TOTAL            (N_UNITS_TOTAL + 1)

typedef struct
{
  float curPos;
  float curFitness;
  float prevPos;
  float prevFitness;
} Position_t;

typedef enum
{
  PSO_SWARM_TYPE_PARALLEL_PSO_MULTI_SWARM
 ,PSO_SWARM_TYPE_PARALLEL_PSO_SUB_SWARM
 ,PSO_SWARM_TYPE_PARALLEL_PSO
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
  UINT32        iteration;
  UINT8         currentParticle;
} PsoSwarmParam_t;

typedef INT8    (*PsoSwarmInit_fct)                       (void *ctx, UnitArrayInterface_t *unitArray, PsoSwarmParam_t *param, UINT8 id);
typedef void    (*PsoSwarmComputeGbest_fct)               (void *ctx);
typedef void    (*PsoSwarmRandomizeAllParticles_fct)      (void *ctx);
typedef void    (*PsoSwarmRandomizeCertainParticles_fct)  (void *ctx, UINT8 *idx, UINT8 nParticlesToRandomize);
typedef INT8    (*PsoSwarmAddParticle_fct)                (void *ctx, void *p);
typedef void *  (*PsoSwarmGetParticle_fct)                (void *ctx, UINT8 idx);
typedef void    (*PsoSwarmRemoveParticles_fct)            (void *ctx, UINT8 *idx, UINT8 nParticles);
typedef UINT8   (*PsoSwarmCheckForPerturb_fct)            (void *ctx, UINT8 *idxPerturbed);
typedef void    (*PsoSwarmRelease_fct)                    (void *ctx);
typedef void    (*PsoSwarmGetParam_fct)                   (void *ctx, PsoSwarmParam_t *paramDest);
typedef void    (*PsoSwarmGetGbest_fct)                   (void *ctx, Position_t *gbestDest);
typedef UINT32  (*PsoSwarmGetIteration_fct)               (void *ctx);
typedef void    (*PsoSwarmIterationIncrement_fct)         (void *ctx);
typedef void    (*PsoSwarmSetAllParticlesFitness_fct)     (void *ctx, float *fitBuf);
typedef void    (*PsoSwarmSetParticleFitness_fct)         (void *ctx, UINT8 idx, float fitness);
typedef UINT8   (*PsoSwarmGetNParticles_fct)              (void *ctx);
typedef void    (*PsoSwarmIncrementCurParticle_fct)       (void *ctx);
typedef UINT8   (*PsoSwarmGetCurParticle_fct)             (void *ctx);
typedef void    (*PsoSwarmComputeAllParticlesPbest_fct)   (void *ctx);
typedef BOOL    (*PsoSwarmEvalSteadyState_fct)            (void *ctx);
typedef UINT8   (*PsoSwarmComputeNextPos_fct)             (void *ctx, float *positions, UINT8 *idxToRemove);
typedef UINT8   (*PsoSwarmGetId_fct)                      (void *ctx);
typedef void *  (*PsoSwarmGetUnitArray_fct)               (void *ctx);
typedef void    (*PsoSwarmSetId_fct)                      (void *ctx, UINT8 id);
typedef float   (*PsoSwarmGetParticlePos_fct)             (void *ctx, UINT8 idx);
typedef float   (*PsoSwarmGetParticleSpeed_fct)           (void *ctx, UINT8 idx);
typedef float   (*PsoSwarmGetParticleFitness_fct)         (void *ctx, UINT8 idx);
  
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
  PsoSwarmGetIteration_fct              GetIteration;
  PsoSwarmIterationIncrement_fct        IterationInc;
  PsoSwarmSetAllParticlesFitness_fct    SetAllParticlesFitness;
  PsoSwarmSetParticleFitness_fct        SetParticleFitness;
  PsoSwarmGetNParticles_fct             GetNParticles;
  PsoSwarmIncrementCurParticle_fct      IncCurrentParticle;
  PsoSwarmGetCurParticle_fct            GetCurParticle;
  PsoSwarmComputeAllParticlesPbest_fct  ComputeAllPbest;
  PsoSwarmEvalSteadyState_fct           EvalSteadyState;
  PsoSwarmComputeNextPos_fct            ComputeNextPos;
  PsoSwarmGetId_fct                     GetId;
  PsoSwarmGetUnitArray_fct              GetUnitArray;
  PsoSwarmSetId_fct                     SetId;
  PsoSwarmGetParticlePos_fct            GetParticlePos;
  PsoSwarmGetParticleSpeed_fct          GetParticleSpeed;
  PsoSwarmGetParticleFitness_fct        GetParticleFitness;
} PsoSwarmInterface_t;


// Public functions
//==============================================================================

const PsoSwarmInterface_t * PsoSwarmInterface (void);

void Position_Reset (Position_t *pos);



#endif // __PSO_SWARM_H__