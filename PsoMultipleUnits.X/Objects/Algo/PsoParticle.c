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
#include "SteadyState.h"
#include "LinkedList.h"


// Private definitions
//==============================================================================

#define N_PARTICLES_TOTAL      (100)

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
} PsoParticle_t;


// Private prototypes
//==============================================================================

void  _Particle_Init          (PsoParticle_t *p, UINT8 id);
UINT8 _Particle_GetId         (PsoParticle_t *p);
void  _Particle_SetId         (PsoParticle_t *p, UINT8 id);
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

static BOOL _oParticleArrayInitialized = 0;

PsoParticle_t           _particles    [N_PARTICLES_TOTAL];
PsoParticleInterface_t  _particles_if [N_PARTICLES_TOTAL];

LinkedList_t _unusedParticles = {NULL, NULL, 0, N_PARTICLES_TOTAL};
LinkedList_t _usedParticles   = {NULL, NULL, 0, N_PARTICLES_TOTAL};
Node_t       _particlesNodes[N_PARTICLES_TOTAL];

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
  p->id = id;
}


void _Particle_SetId (PsoParticle_t *p, UINT8 id)
{
  p->id = id;
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
  Node_t *temp;
  
  if (!_oParticleArrayInitialized)
  {
    _oParticleArrayInitialized = 1;
    
    _unusedParticles.head = (void *) &_particlesNodes[0];
    _unusedParticles.count = 1;
    
    for (i = 0; i < N_PARTICLES_TOTAL; i++)
    {
      // Init the particle itself and its interface
      _Particle_Init(&_particles[i], 0);
      _particles_if[i].ctx          = (void *)                      &_particles[i];
      _particles_if[i].ComputePos   = (PsoParticleComputePos_fct)   &_Particle_ComputePos;
      _particles_if[i].ComputeSpeed = (PsoParticleComputeSpeed_fct) &_Particle_ComputeSpeed;
      _particles_if[i].FsmStep      = (PsoParticleFsmStep_fct)      &_Particle_FsmStep;
      _particles_if[i].GetFitness   = (PsoParticleGetFitness_fct)   &_Particle_GetFitness;
      _particles_if[i].GetPos       = (PsoParticleGetPos_fct)       &_Particle_GetPos;
      _particles_if[i].GetSpeed     = (PsoParticleGetSpeed_fct)     &_Particle_GetSpeed;
      _particles_if[i].Getid        = (PsoParticleGetId_fct)        &_Particle_GetId;
      _particles_if[i].Init         = (PsoParticleInit_fct)         &_Particle_Init;
      _particles_if[i].InitPos      = (PsoParticleInitPos_fct)      &_Particle_InitPos;
      _particles_if[i].InitSpeed    = (PsoParticleInitSpeed_fct)    &_Particle_InitSpeed;
      _particles_if[i].SentinelEval = (PsoParticleSentinelEval_fct) &_Particle_SentinelEval;
      _particles_if[i].SetFitness   = (PsoParticleSetFitness_fct)   &_Particle_SetFitness;
      _particles_if[i].SetId        = (PsoParticleSetId_fct)        &_Particle_SetId;
      _particles_if[i].SetPos       = (PsoParticleSetPos_fct)       &_Particle_SetPos;
      _particles_if[i].SetSpeed     = (PsoParticleSetSpeed_fct)     &_Particle_SetSpeed;
      
      // Init the linked list
      _particlesNodes[i].ctx = (void *) &_particles_if[i];
      _particlesNodes[i].key = i;
      if (i < (N_PARTICLES_TOTAL - 1))
      {
        _particlesNodes[i].next = &_particlesNodes[i + 1];
      }
      else
      {
        _particlesNodes[i].next = NULL;
      }
    }
    
    LinkedList_Init(&_unusedParticles, &_particlesNodes[0]);
  }
  
  if (_unusedParticles.count == 0)
  {
    return NULL;
  }
  
  temp = _unusedParticles.tail;
  LinkedList_RemoveNode(&_unusedParticles, temp);
  LinkedList_AddToEnd(&_usedParticles, temp);
  return temp->ctx;
}