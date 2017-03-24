//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : PsoParticle.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the particles of PSO.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __PSO_PARTICLE_H__
#define __PSO_PARTICLE_H__

#include "Setup.h"
#include "UnitArray.h"
#include "PsoSwarm.h"

// Public definitions
//==============================================================================

typedef void  (*PsoParticleInit_fct)            (void *ctx, UINT8 id);
typedef UINT8 (*PsoParticleGetId_fct)           (void *ctx);
typedef void  (*PsoParticleSetId_fct)           (void *ctx, UINT8 id);
typedef float (*PsoParticleGetPos_fct)         (void *ctx);
typedef float (*PsoParticleGetFitness_fct)      (void *ctx);
typedef float (*PsoParticleGetSpeed_fct)        (void *ctx);
typedef void  (*PsoParticleSetPos_fct)          (void *ctx, float pos);
typedef void  (*PsoParticleSetSpeed_fct)        (void *ctx, float speed);
typedef void  (*PsoParticleSetFitness_fct)      (void *ctx, float fitness);
typedef BOOL  (*PsoParticleFsmStep_fct)         (void *ctx, PsoSwarmInterface_t *swarm);
typedef BOOL  (*PsoParticleSentinelEval_fct)    (void *ctx);
typedef void  (*PsoParticleComputeSpeed_fct)    (void *ctx, PsoSwarmInterface_t *swarm);
typedef void  (*PsoParticleComputePos_fct)      (void *ctx, PsoSwarmInterface_t *swarm);
typedef void  (*PsoParticleInitSpeed_fct)       (void *ctx, PsoSwarmInterface_t *swarm);
typedef void  (*PsoParticleInitPos_fct)         (void *ctx, PsoSwarmInterface_t *swarm);
typedef void  (*PsoParticleRelease_fct)         (void *ctx);
typedef void  (*PsoParticleSetSteadyState_fct)  (void *ctx, size_t bufSize, float oscAmp);
typedef void  (*PsoParticleSetPbest_fct)        (void *ctx, Position_t *pbest);
typedef void  (*PsoParticleSetPbestAbs_fct)     (void *ctx, Position_t *pbestAbs);
typedef void  (*PsoParticleComputePbest_fct)    (void *ctx);
typedef BOOL  (*PsoParticleEvalSteadyState_fct) (void *ctx);

typedef struct
{
  void *ctx;
  PsoParticleInit_fct             Init;
  PsoParticleGetId_fct            Getid;
  PsoParticleSetId_fct            SetId;
  PsoParticleGetPos_fct           GetPos;
  PsoParticleGetFitness_fct       GetFitness;
  PsoParticleGetSpeed_fct         GetSpeed;
  PsoParticleSetPos_fct           SetPos;
  PsoParticleSetSpeed_fct         SetSpeed;
  PsoParticleSetFitness_fct       SetFitness;
  PsoParticleFsmStep_fct          FsmStep;
  PsoParticleSentinelEval_fct     SentinelEval;
  PsoParticleComputeSpeed_fct     ComputeSpeed;
  PsoParticleComputePos_fct       ComputePos;
  PsoParticleInitSpeed_fct        InitSpeed;
  PsoParticleInitPos_fct          InitPos;
  PsoParticleRelease_fct          Release;
  PsoParticleSetSteadyState_fct   SetSteadyState;
  PsoParticleSetPbest_fct         SetPbest;
  PsoParticleSetPbestAbs_fct      SetPbestAbs;
  PsoParticleComputePbest_fct     ComputePbest;
  PsoParticleEvalSteadyState_fct  EvalSteadyState;
} PsoParticleInterface_t;


// Public functions
//==============================================================================

const PsoParticleInterface_t * PsoParticleInterface (void);



#endif // __PSO_PARTICLE_H__