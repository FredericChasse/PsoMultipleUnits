//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : PsoParticle.c
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the particles of PSO.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "PsoParticle.h"
#include "PsoSwarm.h"
#include "SteadyState.h"
#include "BipBuffer.h"


// Private definitions
//==============================================================================

#define PARTICLE_BUFFER_SIZE      (100)

typedef enum
{
  PARTICLE_STATE_SEARCHING
 ,PARTICLE_STATE_PERTUR_OCCURRED
 ,PARTICLE_STATE_VALIDATE_OPTIMUM
 ,PARTICLE_STATE_STEADY_STATE
} ParticleState_t;

typedef struct
{
  UINT8 id;
  Position_t pbest;
  Position_t pbestAbs;
  Position_t pos;
  Position_t optPos;
  float curSpeed;
  float prevSpeed;
  float jSteady;
  BOOL oSentinelWarning;
  ParticleState_t state;
  SteadyState_t steadyState;
  BOOL          oLocked;
} PsoParticle_t;


// Private prototypes
//==============================================================================

void  _Particle_Init          (PsoParticle_t *p, UINT8 id);
void  _Particle_Release       (PsoParticle_t *p);
UINT8 _Particle_GetId         (PsoParticle_t *p);
float _Particle_GetPos        (PsoParticle_t *p);
float _Particle_GetFitness    (PsoParticle_t *p);
float _Particle_GetSpeed      (PsoParticle_t *p);
void  _Particle_SetPos        (PsoParticle_t *p, float pos);
void  _Particle_SetSpeed      (PsoParticle_t *p, float speed);
void  _Particle_SetFitness    (PsoParticle_t *p, float fitness);
BOOL  _Particle_FsmStep       (PsoParticle_t *p, PsoSwarmInterface_t *swarm);
BOOL  _Particle_SentinelEval  (PsoParticle_t *p);
void  _Particle_ComputeSpeed  (PsoParticle_t *p, PsoSwarmInterface_t *swarm);
void  _Particle_ComputePos    (PsoParticle_t *p, PsoSwarmInterface_t *swarm);
void  _Particle_InitSpeed     (PsoParticle_t *p, PsoSwarmInterface_t *swarm);
void  _Particle_InitPos       (PsoParticle_t *p, PsoSwarmInterface_t *swarm);



// Private variables
//==============================================================================

PsoParticle_t _particles[PARTICLE_BUFFER_SIZE];
PsoParticleInterface_t _particles_if[PARTICLE_BUFFER_SIZE];
static BOOL _oParticleArrayInitialized = 0;


// Private functions
//==============================================================================

void _Particle_Init (PsoParticle_t *p, UINT8 id)
{
  p->curSpeed = 0;
  p->prevSpeed = 0;
  p->jSteady = 0;
  p->oSentinelWarning = 0;
  p->state = PARTICLE_STATE_SEARCHING;
  SteadyState_Reset(&p->steadyState);
  Position_Reset(&p->optPos);
  Position_Reset(&p->pbest);
  Position_Reset(&p->pbestAbs);
  Position_Reset(&p->pos);
  p->oLocked = 1;
  p->id = id;
}


void _Particle_Release (PsoParticle_t *p)
{
  p->oLocked = 0;
}


UINT8 _Particle_GetId (PsoParticle_t *p)
{
  return p->id;
}


float _Particle_GetPos (PsoParticle_t *p)
{
  return p->pos.curPos;
}


float _Particle_GetFitness (PsoParticle_t *p)
{
  return p->pos.curFitness;
}


float _Particle_GetSpeed (PsoParticle_t *p)
{
  return p->curSpeed;
}


void _Particle_SetPos (PsoParticle_t *p, float pos)
{
  p->pos.prevPos = p->pos.curPos;
  p->pos.curPos  = pos;
}


void _Particle_SetSpeed (PsoParticle_t *p, float speed)
{
  p->prevSpeed = p->curSpeed;
  p->curSpeed  = speed;
}


void _Particle_SetFitness (PsoParticle_t *p, float fitness)
{
  p->pos.prevFitness = p->pos.curFitness;
  p->pos.curFitness  = fitness;
}


BOOL _Particle_FsmStep (PsoParticle_t *p, PsoSwarmInterface_t *swarm)
{
  BOOL oRemoveParticle = 0;
  return oRemoveParticle;
}


BOOL _Particle_SentinelEval (PsoParticle_t *p)
{
  BOOL oPerturbedOccurred = 0;
  return oPerturbedOccurred;
}


void  _Particle_ComputeSpeed  (PsoParticle_t *p, PsoSwarmInterface_t *swarm)
{
  
}


void  _Particle_ComputePos    (PsoParticle_t *p, PsoSwarmInterface_t *swarm)
{
  
}


void  _Particle_InitSpeed     (PsoParticle_t *p, PsoSwarmInterface_t *swarm)
{
  
}


void  _Particle_InitPos       (PsoParticle_t *p, PsoSwarmInterface_t *swarm)
{
  
}


// Public functions
//==============================================================================

const PsoParticleInterface_t * PsoParticleInterface (void)
{
  UINT8 i;
  if (!_oParticleArrayInitialized)
  {
    _oParticleArrayInitialized = 1;
    for (i = 0; i < PARTICLE_BUFFER_SIZE; i++)
    {
      _particles_if[i].ctx = (void *) &_particles[i];
    }
  }
  
  for (i = 0; i < PARTICLE_BUFFER_SIZE; i++)
  {
    if (!_particles[i].oLocked)
    {
      break;
    }
  }
  
  _particles[i].oLocked = 1;
  return &_particles_if[i];
}