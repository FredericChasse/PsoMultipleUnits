//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : PsoSwarm.c
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the swarms of PSO.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "PsoSwarm.h"
#include "SteadyState.h"
#include "PsoParticle.h"


// Private definitions
//==============================================================================

#define PSO_SWARM_MAX_PARTICLES   (10)

typedef struct
{
  UINT8                   id;
  UINT8                   nParticles;
  PsoParticleInterface_t *particles[PSO_SWARM_MAX_PARTICLES];
  UnitArrayInterface_t   *unitArray;
  Position_t              gbest;
  PsoSwarmParam_t         param;
  UINT8                   nParticlesPerUnit;
  BOOL                    oInSteadyState;
} PsoSwarm_t;



// Private prototypes
//==============================================================================

BOOL    _IsSwarmParamValid                (PsoSwarmParam_t *param);
void    _Swarm_SetSteadyState             (PsoSwarm_t *s);

INT8    _Swarm_Init                       (PsoSwarm_t *s, UnitArrayInterface_t *unitArray, PsoSwarmParam_t *param);
void    _Swarm_ComputeGbest               (PsoSwarm_t *s);
void    _Swarm_RandomizeAllParticles      (PsoSwarm_t *s);
void    _Swarm_RandomizeCertainParticles  (PsoSwarm_t *s, UINT8 *idx, UINT8 nParticlesToRandomize);
void    _Swarm_AddParticle                (PsoSwarm_t *s, PsoParticleInterface_t *p);
void *  _Swarm_GetParticle                (PsoSwarm_t *s, UINT8 idx);
void    _Swarm_RemoveParticles            (PsoSwarm_t *s, UINT8 *idx, UINT8 nParticles);
UINT8   _Swarm_CheckForPerturb            (PsoSwarm_t *s, UINT8 *idxPerturbed);
void    _Swarm_Release                    (PsoSwarm_t *s);


// Private variables
//==============================================================================

static BOOL _oSwarmsInitialized = 0;

#define N_SWARMS_TOTAL    (N_UNITS_TOTAL + 1)

PsoSwarm_t _swarms[N_SWARMS_TOTAL] = {0};

PsoSwarmInterface_t _swarms_if[N_SWARMS_TOTAL];


// Private functions
//==============================================================================

INT8 _Swarm_Init (PsoSwarm_t *s, UnitArrayInterface_t *unitArray, PsoSwarmParam_t *param)
{
  UINT8 i;
  
  if ( _IsSwarmParamValid(param) || (unitArray == NULL) || (s == NULL) )
  {
    return -1;
  }
  
  s->unitArray = unitArray;
  s->oInSteadyState = 0;
  Position_Reset(&s->gbest);
  
  memcpy(&s->param, param, sizeof(PsoSwarmParam_t));
  
  if (s->param.type == PSO_SWARM_TYPE_PARALLEL_PSO)
  {
    s->nParticles = s->unitArray->GetNUnits(s->unitArray->ctx);
    s->nParticlesPerUnit = 1;
  }
  else if (s->param.type == PSO_SWARM_TYPE_PSO_1D)
  {
    s->nParticles = s->param.minParticles;
    s->nParticlesPerUnit = s->param.minParticles;
  }
  
  for (i = 0; i < s->nParticles; i++)
  {
    s->particles[i] = (PsoParticleInterface_t *) PsoParticleInterface();
    s->particles[i]->Init(s->particles[i]->ctx, i);
  }
  
  _Swarm_SetSteadyState(s);
  
  return 0;
}


void _Swarm_Release (PsoSwarm_t *s)
{
  
}


void _Swarm_SetSteadyState (PsoSwarm_t *s)
{
  UINT8 i;
  
  for (i = 0; i < s->nParticles; i++)
  {
    s->particles[i]->SetSteadyState(s->particles[i]->ctx, s->param.nSamplesForSteadyState, s->param.steadyStateOscAmp);
  }
}


void    _Swarm_ComputeGbest               (PsoSwarm_t *s) {}
void    _Swarm_RandomizeAllParticles      (PsoSwarm_t *s) {}
void    _Swarm_RandomizeCertainParticles  (PsoSwarm_t *s, UINT8 *idx, UINT8 nParticlesToRandomize) {}
void    _Swarm_AddParticle                (PsoSwarm_t *s, PsoParticleInterface_t *p) {}
void *  _Swarm_GetParticle                (PsoSwarm_t *s, UINT8 idx) {}
void    _Swarm_RemoveParticles            (PsoSwarm_t *s, UINT8 *idx, UINT8 nParticles) {}
UINT8   _Swarm_CheckForPerturb            (PsoSwarm_t *s, UINT8 *idxPerturbed) {}


BOOL _IsSwarmParamValid (PsoSwarmParam_t *param)
{
  return (param->c1 <= 0) 
  || (param->c2 <= 0) 
  || (param->minParticles > PSO_SWARM_MAX_PARTICLES) 
  || (param->omega <= 0) 
  || (param->perturbAmp < 0) 
  || (param->posMax <= 0)
  || (param->posMin <= 0)
  || (param->posMax <= param->posMin)
  || (param->sentinelMargin <= 0)
  || (param->type > PSO_SWARM_TYPE_PSO_1D)
  || (param->steadyStateOscAmp <= 0)
  || (param->nSamplesForSteadyState > STEADY_STATE_MAX_SAMPLES)
  ;
}



// Public functions
//==============================================================================

void Position_Reset (Position_t *pos)
{
  pos->curFitness   = 0;
  pos->curPos       = 0;
  pos->prevFitness  = 0;
  pos->prevPos      = 0;
}


const PsoSwarmInterface_t * PsoSwarmInterface (UINT8 id)
{
  UINT8 i;
  if (!_oSwarmsInitialized)
  {
    _oSwarmsInitialized = 1;
    
    for (i = 0; i < N_SWARMS_TOTAL; i++)
    {
      _swarms   [i].id                          = i;
      _swarms_if[i].ctx                         = (void *)                                &_swarms[i];
      _swarms_if[i].AddParticle                 = (PsoSwarmAddParticle_fct)               &_Swarm_AddParticle;
      _swarms_if[i].CheckForPerturb             = (PsoSwarmCheckForPerturb_fct)           &_Swarm_CheckForPerturb;
      _swarms_if[i].ComputeGbest                = (PsoSwarmComputeGbest_fct)              &_Swarm_ComputeGbest;
      _swarms_if[i].GetParticle                 = (PsoSwarmGetParticle_fct)               &_Swarm_GetParticle;
      _swarms_if[i].Init                        = (PsoSwarmInit_fct)                      &_Swarm_Init;
      _swarms_if[i].RandomizeAllParticles       = (PsoSwarmRandomizeAllParticles_fct)     &_Swarm_RandomizeAllParticles;
      _swarms_if[i].RandomizeCertainParticles   = (PsoSwarmRandomizeCertainParticles_fct) &_Swarm_RandomizeCertainParticles;
      _swarms_if[i].RemoveParticles             = (PsoSwarmRemoveParticles_fct)           &_Swarm_RemoveParticles;
      _swarms_if[i].Release                     = (PsoSwarmRelease_fct)                   &_Swarm_Release;
    }
  }
  
  if (id >= (N_SWARMS_TOTAL))
  {
    return NULL;
  }
  return &_swarms_if[id];
}