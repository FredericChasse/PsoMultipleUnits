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
#include "Rng.h"
#include "MathFunctions.h"
#include "Potentiometer.h"  // To compute positions


// Private definitions
//==============================================================================

#define N_PARTICLES_TOTAL      (100)

typedef enum
{
  PARTICLE_STATE_SEARCHING
 ,PARTICLE_STATE_PERTURB_OCCURRED
 ,PARTICLE_STATE_VALIDATE_OPTIMUM
 ,PARTICLE_STATE_STEADY_STATE
} ParticleState_t;

typedef struct
{
  float  jinit
        ,dinit
        ,jpos
        ,dpos
        ,jminus
        ,dminus
        ;
} ParticleOptPos_t;

typedef struct
{
  UINT8 id;
  Position_t pbest;
  Position_t pbestAbs;
  Position_t pos;
  ParticleOptPos_t optPos;
  float curSpeed;
  float prevSpeed;
  float jSteady;
  BOOL oSentinelWarning;
  ParticleState_t state;
  SteadyState_t steadyState;
  float steadyStateBuf[STEADY_STATE_MAX_SAMPLES];
  UINT8 linkKey;
} PsoParticle_t;


// Private prototypes
//==============================================================================

void  _Particle_ResetOptPos       (ParticleOptPos_t *optPos);

void  _Particle_Init              (PsoParticle_t *p, UINT8 id);
UINT8 _Particle_GetId             (PsoParticle_t *p);
void  _Particle_SetId             (PsoParticle_t *p, UINT8 id);
void  _Particle_Release           (PsoParticle_t *p);
void  _Particle_SetPbest          (PsoParticle_t *p, Position_t *pbest);
void  _Particle_SetPbestAbs       (PsoParticle_t *p, Position_t *pbestAbs);
void  _Particle_ComputePbest      (PsoParticle_t *p);
float _Particle_GetPos            (PsoParticle_t *p);
float _Particle_GetFitness        (PsoParticle_t *p);
float _Particle_GetSpeed          (PsoParticle_t *p);
void  _Particle_SetPos            (PsoParticle_t *p, float pos);
void  _Particle_SetSpeed          (PsoParticle_t *p, float speed);
void  _Particle_SetFitness        (PsoParticle_t *p, float fitness);
void  _Particle_SetJSteady        (PsoParticle_t *p, float jSteady);
BOOL  _Particle_FsmStep           (PsoParticle_t *p, PsoSwarmInterface_t *swarm);
BOOL  _Particle_SentinelEval      (PsoParticle_t *p, float margin);
void  _Particle_ComputeSpeed      (PsoParticle_t *p, PsoSwarmInterface_t *swarm);
void  _Particle_ComputePos        (PsoParticle_t *p, PsoSwarmInterface_t *swarm);
void  _Particle_InitSpeed         (PsoParticle_t *p, PsoSwarmInterface_t *swarm);
void  _Particle_InitPos           (PsoParticle_t *p, PsoSwarmInterface_t *swarm);
void  _Particle_SetSteadyState    (PsoParticle_t *p, size_t bufSize, float oscAmp);
BOOL  _Particle_EvalSteadyState   (PsoParticle_t *p);
BOOL  _Particle_GetSteadyState    (PsoParticle_t *p);
BOOL  _Particle_GetSentinelState  (PsoParticle_t *p);
BOOL  _Particle_IsSearching       (PsoParticle_t *p);
void  _Particle_ResetSteadyState  (PsoParticle_t *p);



// Private variables
//==============================================================================

extern const float potRealValues[256];

static BOOL _oParticleArrayInitialized = 0;

PsoParticle_t           _particles    [N_PARTICLES_TOTAL];
PsoParticleInterface_t  _particles_if [N_PARTICLES_TOTAL];

LinkedList_t _unusedParticles = {NULL, NULL, 0, N_PARTICLES_TOTAL};
LinkedList_t _usedParticles   = {NULL, NULL, 0, N_PARTICLES_TOTAL};
Node_t       _particlesNodes[N_PARTICLES_TOTAL];

// Private functions
//==============================================================================

void _Particle_ResetOptPos (ParticleOptPos_t *optPos)
{
    optPos->dinit
  = optPos->dminus
  = optPos->dpos
  = optPos->jinit
  = optPos->jminus
  = optPos->jpos
  = 0
  ;
}

void _Particle_Init (PsoParticle_t *p, UINT8 id)
{
  p->curSpeed = 0;
  p->prevSpeed = 0;
  p->jSteady = 0;
  p->oSentinelWarning = 0;
  p->state = PARTICLE_STATE_SEARCHING;
  SteadyState_Reset(&p->steadyState);
  _Particle_ResetOptPos(&p->optPos);
  Position_Reset(&p->pbest);
  Position_Reset(&p->pbestAbs);
  Position_Reset(&p->pos);
  p->id = id;
}


void _Particle_SetPbest (PsoParticle_t *p, Position_t *pbest)
{
  p->pbest.prevFitness = p->pbest.curFitness;
  p->pbest.prevPos = p->pbest.curPos;
  p->pbest.curFitness = pbest->curFitness;
  p->pbest.curPos = pbest->curPos;
}


void _Particle_SetPbestAbs (PsoParticle_t *p, Position_t *pbestAbs)
{
  p->pbestAbs.prevFitness = p->pbestAbs.curFitness;
  p->pbestAbs.prevPos = p->pbestAbs.curPos;
  p->pbestAbs.curFitness = pbestAbs->curFitness;
  p->pbestAbs.curPos = pbestAbs->curPos;
}


void _Particle_SetSteadyState(PsoParticle_t *p, size_t bufSize, float oscAmp)
{
  SteadyState_Init(&p->steadyState, p->steadyStateBuf, bufSize, oscAmp);
}


void _Particle_Release (PsoParticle_t *p)
{
  Node_t *node = &_particlesNodes[p->linkKey];
  LinkedList_RemoveNode(node->list, node);
  LinkedList_AddToEnd(&_unusedParticles, node);
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
  UINT8 potIdx = ComputePotValueFloat2Dec(pos);
  p->pos.prevPos = p->pos.curPos;
  p->pos.curPos  = potRealValues[potIdx];
}


void _Particle_SetSpeed (PsoParticle_t *p, float speed)
{
  p->prevSpeed = p->curSpeed;
  p->curSpeed  = speed;
}


BOOL _Particle_IsSearching (PsoParticle_t *p)
{
  return p->state == PARTICLE_STATE_SEARCHING;
}


void _Particle_SetJSteady (PsoParticle_t *p, float jSteady)
{
  p->jSteady = jSteady;
}


void _Particle_SetFitness (PsoParticle_t *p, float fitness)
{
  p->pos.prevFitness = p->pos.curFitness;
  p->pos.curFitness  = fitness;
}


BOOL _Particle_FsmStep (PsoParticle_t *p, PsoSwarmInterface_t *swarm)
{
  BOOL oRemoveParticle = 0;
  UINT8 potIdx;
  PsoSwarmParam_t param;
  swarm->GetParam(swarm->ctx, &param);
  
  if (p->steadyState.oInSteadyState && (p->state == PARTICLE_STATE_SEARCHING))
  {
    if (param.type == PSO_SWARM_TYPE_PARALLEL_PSO_SUB_SWARM)
    {
      p->jSteady = p->pos.curFitness;
      p->state = PARTICLE_STATE_STEADY_STATE;
    }
    else if (param.type == PSO_SWARM_TYPE_PARALLEL_PSO_MULTI_SWARM)
    {
      p->state = PARTICLE_STATE_VALIDATE_OPTIMUM;
    }
  }
  
  if (p->oSentinelWarning)
  {
    p->state = PARTICLE_STATE_PERTURB_OCCURRED;
  }
  
  switch (p->state)
  {
    case PARTICLE_STATE_SEARCHING:
      _Particle_ComputeSpeed(p, swarm);
      _Particle_ComputePos  (p, swarm);
      oRemoveParticle = 0;
      break;
      
    case PARTICLE_STATE_PERTURB_OCCURRED:
      _Particle_InitSpeed(p, swarm);
      _Particle_InitPos  (p, swarm);
      p->state = PARTICLE_STATE_SEARCHING;
      oRemoveParticle = 0;
      break;
      
    case PARTICLE_STATE_VALIDATE_OPTIMUM:
      if (p->optPos.jinit == 0)
      {
        p->optPos.jinit   = p->pos.curFitness;
        p->optPos.dinit   = p->pos.curPos;
        p->optPos.dminus  = p->pos.curPos - param.perturbAmp;
        potIdx = ComputePotValueFloat2Dec(p->optPos.dminus);
        p->optPos.dminus  = potRealValues[potIdx];
        p->optPos.dpos    = p->pos.curPos + param.perturbAmp;
        potIdx = ComputePotValueFloat2Dec(p->optPos.dpos);
        p->optPos.dpos    = potRealValues[potIdx];
        
        p->pos.prevPos    = p->pos.curPos;
        p->pos.curPos     = p->optPos.dminus;
        p->prevSpeed      = p->curSpeed;
        p->curSpeed       = -param.perturbAmp;
        
        oRemoveParticle   = 0;
      }
      else if (p->optPos.jminus == 0)
      {
        p->optPos.jminus  = p->pos.curFitness;
        
        p->pos.prevPos    = p->pos.curPos;
        p->pos.curPos     = p->optPos.dpos;
        p->prevSpeed      = p->curSpeed;
        p->curSpeed       = param.perturbAmp;
        
        oRemoveParticle   = 0;
      }
      else if (p->optPos.jpos == 0)
      {
        p->optPos.jpos    = p->pos.curFitness;
        
        p->pos.prevPos    = p->pos.curPos;
        p->pos.curPos     = p->optPos.dinit;
        p->prevSpeed      = p->curSpeed;
        p->curSpeed       = -param.perturbAmp;
        
        oRemoveParticle   = 0;
      }
      else
      {
        // If no perturbation occurred
        if (  (p->pos.curFitness < p->optPos.jinit) * (1 + param.sentinelMargin) 
           && (p->pos.curFitness > p->optPos.jinit) * (1 - param.sentinelMargin) )
        {
          // If the final position is an optimum
          if ( (p->optPos.jminus <= p->pos.curFitness) && (p->optPos.jpos <= p->pos.curFitness) )
          {
            p->state = PARTICLE_STATE_STEADY_STATE;
            p->jSteady = p->pos.curFitness;
            p->pos.prevPos = p->pos.curPos;
            p->pos.prevFitness = p->pos.curFitness;
            oRemoveParticle = 0;
          }
          else  // If the position is not an optimum
          {
            if (p->optPos.dinit == p->pbestAbs.curPos)  // If we were testing for Pbest
            {
              p->state = PARTICLE_STATE_SEARCHING;
              oRemoveParticle = 1;
            }
            else
            {
              p->pos.prevPos = p->pos.curPos;
              p->pos.curPos = p->pbestAbs.curPos;
              oRemoveParticle = 0;
            }
            _Particle_ResetOptPos(&p->optPos);
          }
        }
        else  // Perturbation occurred
        {
          p->state = PARTICLE_STATE_SEARCHING;
          oRemoveParticle = 0;
          _Particle_ResetOptPos(&p->optPos);
        }
      }
      break;
      
    case PARTICLE_STATE_STEADY_STATE:
      p->pos.prevPos = p->pos.curPos;
      p->prevSpeed = p->curSpeed;
      p->curSpeed = 0;
      oRemoveParticle = 0;
      break;
      
    default:
      p->state = PARTICLE_STATE_SEARCHING;
      oRemoveParticle = 0;
      break;
  }
  return oRemoveParticle;
}


BOOL _Particle_SentinelEval (PsoParticle_t *p, float margin)
{
  float jCompare;
  if (p->state == PARTICLE_STATE_STEADY_STATE)
  {
    jCompare = p->jSteady;
  }
  else
  {
    jCompare = p->pos.prevFitness;
  }
  if (p->pos.curPos == p->pos.prevPos)
  {
    if ( (ABS(p->pos.curFitness - jCompare) / p->pos.curFitness) >= margin)
    {
      p->oSentinelWarning = 1;
    }
    else
    {
      p->oSentinelWarning = 0;
    }
  }
  else
  {
    p->oSentinelWarning = 0;
  }
  return p->oSentinelWarning;
}


BOOL _Particle_GetSentinelState (PsoParticle_t *p)
{
  return p->oSentinelWarning;
}


void _Particle_ComputeSpeed (PsoParticle_t *p, PsoSwarmInterface_t *swarm)
{
  _Particle_ResetOptPos(&p->optPos);
  
  PsoSwarmParam_t param;
  Position_t gbest;
  float r1,r2;
  
  if (swarm->GetIteration(swarm->ctx) == 1)
  {
    _Particle_InitSpeed(p, swarm);
  }
  else
  {
    swarm->GetParam(swarm->ctx, &param);
    swarm->GetGbest(swarm->ctx, &gbest);
    
    r1 = Rng_GetRandFloat();
    r2 = Rng_GetRandFloat();

    p->prevSpeed = p->curSpeed;
    p->curSpeed =   param.omega * p->prevSpeed
                  + param.c1 * r1 * (p->pbest.curPos - p->pos.curPos)
                  + param.c2 * r2 * (gbest.curPos - p->pos.curPos)
                  ;
  }
}


void _Particle_InitSpeed (PsoParticle_t *p, PsoSwarmInterface_t *swarm)
{
  _Particle_ResetOptPos(&p->optPos);
  
  PsoSwarmParam_t param;
  Position_t gbest;
  float  rInit = Rng_GetRandFloat()
        ,r1    = Rng_GetRandFloat()
        ,r2    = Rng_GetRandFloat()
        ;
  swarm->GetParam(swarm->ctx, &param);
  swarm->GetGbest(swarm->ctx, &gbest);
  p->prevSpeed = p->curSpeed;
  p->curSpeed =   (20*rInit - 10)
                + param.omega * p->prevSpeed
                + param.c1 * r1 * (p->pbest.curPos - p->pos.curPos)
                + param.c2 * r2 * (gbest.curPos - p->pos.curPos)
                ;
}


void _Particle_ComputePos (PsoParticle_t *p, PsoSwarmInterface_t *swarm)
{
  UINT8 potIdx;
  PsoSwarmParam_t param;
  swarm->GetParam(swarm->ctx, &param);
  p->pos.prevPos = p->pos.curPos;
  p->pos.curPos = p->pos.prevPos + p->curSpeed;
  p->pos.curPos = MIN(MAX(param.posMin, p->pos.curPos), param.posMax);
  potIdx = ComputePotValueFloat2Dec(p->pos.curPos);
  p->pos.curPos = potRealValues[potIdx];
}


void _Particle_ComputePbest (PsoParticle_t *p)
{
  p->pbest.prevPos = p->pbest.curPos;
  p->pbest.prevFitness = p->pbest.curFitness;
  
  if (p->pos.curFitness > p->pos.prevFitness)
  {
    p->pbest.curPos     = p->pos.curPos;
    p->pbest.curFitness = p->pos.curFitness;
  }
  else
  {
    p->pbest.curPos     = p->pos.prevPos;
    p->pbest.curFitness = p->pos.prevFitness;
  }
  
  if (p->state != PARTICLE_STATE_VALIDATE_OPTIMUM)
  {
    if (p->pbest.curFitness > p->pbestAbs.curFitness)
    {
      p->pbestAbs.curPos     = p->pbest.curPos;
      p->pbestAbs.curFitness = p->pbest.curFitness;
    }
  }
}


BOOL _Particle_GetSteadyState (PsoParticle_t *p)
{
  return p->steadyState.oInSteadyState;
}


void _Particle_ResetSteadyState (PsoParticle_t *p)
{
  SteadyState_Reset(&p->steadyState);
}


BOOL _Particle_EvalSteadyState (PsoParticle_t *p)
{
  SteadyState_AddSample(&p->steadyState, &p->pos.curPos);
  if (SteadyState_CheckForSteadyState(&p->steadyState))
  {
    p->jSteady = p->pos.curFitness;
  }
  return p->steadyState.oInSteadyState;
}


void _Particle_InitPos (PsoParticle_t *p, PsoSwarmInterface_t *swarm)
{
  UINT8 potIdx;
  PsoSwarmParam_t param;
  swarm->GetParam(swarm->ctx, &param);
  p->pos.prevPos = p->pos.curPos;
  potIdx = Rng_GetRandBoundedUint32(POT_MIN_INDEX, POT_MAX_INDEX);
//  p->pos.curPos = Rng_GetRandFloat() * (param.posMax - param.posMin) + param.posMin;
//  p->pos.curPos = MIN(MAX(param.posMin, p->pos.curPos), param.posMax);
  p->pos.curPos = potRealValues[potIdx];
  Position_Reset(&p->pbestAbs);
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
      _particles[i].linkKey = i;
      _particles_if[i].ctx              = (void *)                          &_particles[i];
      _particles_if[i].ComputePos       = (PsoParticleComputePos_fct)       &_Particle_ComputePos;
      _particles_if[i].ComputeSpeed     = (PsoParticleComputeSpeed_fct)     &_Particle_ComputeSpeed;
      _particles_if[i].FsmStep          = (PsoParticleFsmStep_fct)          &_Particle_FsmStep;
      _particles_if[i].GetFitness       = (PsoParticleGetFitness_fct)       &_Particle_GetFitness;
      _particles_if[i].GetPos           = (PsoParticleGetPos_fct)           &_Particle_GetPos;
      _particles_if[i].GetSpeed         = (PsoParticleGetSpeed_fct)         &_Particle_GetSpeed;
      _particles_if[i].Getid            = (PsoParticleGetId_fct)            &_Particle_GetId;
      _particles_if[i].Init             = (PsoParticleInit_fct)             &_Particle_Init;
      _particles_if[i].InitPos          = (PsoParticleInitPos_fct)          &_Particle_InitPos;
      _particles_if[i].InitSpeed        = (PsoParticleInitSpeed_fct)        &_Particle_InitSpeed;
      _particles_if[i].SentinelEval     = (PsoParticleSentinelEval_fct)     &_Particle_SentinelEval;
      _particles_if[i].SetFitness       = (PsoParticleSetFitness_fct)       &_Particle_SetFitness;
      _particles_if[i].SetId            = (PsoParticleSetId_fct)            &_Particle_SetId;
      _particles_if[i].SetPos           = (PsoParticleSetPos_fct)           &_Particle_SetPos;
      _particles_if[i].SetSpeed         = (PsoParticleSetSpeed_fct)         &_Particle_SetSpeed;
      _particles_if[i].Release          = (PsoParticleRelease_fct)          &_Particle_Release;
      _particles_if[i].SetSteadyState   = (PsoParticleSetSteadyState_fct)   &_Particle_SetSteadyState;
      _particles_if[i].SetPbest         = (PsoParticleSetPbest_fct)         &_Particle_SetPbest;
      _particles_if[i].SetPbestAbs      = (PsoParticleSetPbestAbs_fct)      &_Particle_SetPbestAbs;
      _particles_if[i].ComputePbest     = (PsoParticleComputePbest_fct)     &_Particle_ComputePbest;
      _particles_if[i].EvalSteadyState  = (PsoParticleEvalSteadyState_fct)  &_Particle_EvalSteadyState;
      _particles_if[i].GetSteadyState   = (PsoParticleGetSteadyState_fct)   &_Particle_GetSteadyState;
      _particles_if[i].GetSentinelState = (PsoParticleGetSentinelState_fct) &_Particle_GetSentinelState;
      _particles_if[i].SetJSteady       = (PsoParticleSetJSteady_fct)       &_Particle_SetJSteady;
      _particles_if[i].IsSearching      = (PsoParticleIsSearching_fct)      &_Particle_IsSearching;
      _particles_if[i].ResetSteadyState = (PsoParticleResetSteadyState_fct) &_Particle_ResetSteadyState;
      
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