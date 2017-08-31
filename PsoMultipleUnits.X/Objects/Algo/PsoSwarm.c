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
#include "LinkedList.h"
#include "MathFunctions.h"
#include "Potentiometer.h"  // To compute positions


// Private definitions
//==============================================================================

typedef struct
{
  UINT8                   id;
  UINT8                   linkKey;
  UINT8                   nParticles;
  PsoParticleInterface_t *particles[PSO_SWARM_MAX_PARTICLES];
  UnitArrayInterface_t   *unitArray;
  Position_t              gbest;
  PsoSwarmParam_t         param;
  UINT8                   nParticlesPerUnit;
  BOOL                    oInSteadyState;
  BOOL                    oResetParticles;
} PsoSwarm_t;



// Private prototypes
//==============================================================================

BOOL    _IsSwarmParamValid                (PsoSwarmParam_t *param);
static int _CompareFunc                   (const void *p1, const void *p2);
void    _Swarm_SetSteadyState             (PsoSwarm_t *s);
void    _Swarm_ShiftParticlesLeft         (PsoSwarm_t *s, UINT8 idxToShift);

INT8    _Swarm_Init                       (PsoSwarm_t *s, UnitArrayInterface_t *unitArray, PsoSwarmParam_t *param, UINT8 id);
void    _Swarm_ComputeGbest               (PsoSwarm_t *s);
void    _Swarm_RandomizeAllParticles      (PsoSwarm_t *s);
void    _Swarm_RandomizeCertainParticles  (PsoSwarm_t *s, UINT8 *idx, UINT8 nParticlesToRandomize);
INT8    _Swarm_AddParticle                (PsoSwarm_t *s, PsoParticleInterface_t *p);
void *  _Swarm_GetParticle                (PsoSwarm_t *s, UINT8 idx);
void    _Swarm_RemoveParticles            (PsoSwarm_t *s, UINT8 *idx, UINT8 nParticles);
UINT8   _Swarm_CheckForPerturb            (PsoSwarm_t *s, UINT8 *idxPerturbed);
void    _Swarm_Release                    (PsoSwarm_t *s);
void    _Swarm_GetGbest                   (PsoSwarm_t *s, Position_t *gbestDest);
void    _Swarm_GetParam                   (PsoSwarm_t *s, PsoSwarmParam_t *paramDest);
UINT32  _Swarm_GetIteration               (PsoSwarm_t *s);
UINT32  _Swarm_IterationIncrement         (PsoSwarm_t *s);
void    _Swarm_SetAllParticlesFitness     (PsoSwarm_t *s, float *fitBuf);
void    _Swarm_SetParticleFitness         (PsoSwarm_t *s, UINT8 idx, float fitness);
UINT8   _Swarm_GetNParticles              (PsoSwarm_t *s);
UINT8   _Swarm_GetCurrentParticle         (PsoSwarm_t *s);
void    _Swarm_IncrementCurrentParticle   (PsoSwarm_t *s);
void    _Swarm_ComputeAllParticlesPbest   (PsoSwarm_t *s);
BOOL    _Swarm_EvalSteadyState            (PsoSwarm_t *s);
UINT8   _Swarm_GetId                      (PsoSwarm_t *s);
void    _Swarm_SetId                      (PsoSwarm_t *s, UINT8 id);
float   _Swarm_GetParticlePos             (PsoSwarm_t *s, UINT8 idx);
float   _Swarm_GetParticleSpeed           (PsoSwarm_t *s, UINT8 idx);
float   _Swarm_GetParticleFitness         (PsoSwarm_t *s, UINT8 idx);
void *  _Swarm_GetUnitArray               (PsoSwarm_t *s);
UINT8   _Swarm1d_ComputeNextPositions     (PsoSwarm_t *s, float *positions, UINT8 *dummy);
UINT8   _SwarmPara_ComputeNextPositions   (PsoSwarm_t *s, float *positions, UINT8 *idxToRemove);
UINT8   _SubSwarm_ComputeNextPositions    (PsoSwarm_t *s, float *positions, UINT8 *idxToRemove);
void    _Swarm_UpdateParticlesFitness     (PsoSwarm_t *s);
void    _Swarm_SetParticlePos             (PsoSwarm_t *s, UINT8 idx, float pos);


// Private variables
//==============================================================================

extern const float potRealValues[256];

static BOOL _oSwarmsInitialized = 0;

PsoSwarm_t          _swarms   [N_SWARMS_TOTAL] = {0};
PsoSwarmInterface_t _swarms_if[N_SWARMS_TOTAL];

LinkedList_t _unusedSwarms =  {NULL, NULL, 0, N_SWARMS_TOTAL};
LinkedList_t _usedSwarms   =  {NULL, NULL, 0, N_SWARMS_TOTAL};
Node_t       _swarmsNodes     [N_SWARMS_TOTAL];


// Private functions
//==============================================================================

INT8 _Swarm_Init (PsoSwarm_t *s, UnitArrayInterface_t *unitArray, PsoSwarmParam_t *param, UINT8 id)
{
  UINT8 i;
  
  if ( _IsSwarmParamValid(param) || (unitArray == NULL) || (s == NULL) )
  {
    return -1;
  }
  
  s->id               = id;
  s->unitArray        = unitArray;
  s->oInSteadyState   = 0;
  s->oResetParticles  = 0;
  Position_Reset(&s->gbest);
  
  memcpy(&s->param, param, sizeof(PsoSwarmParam_t));
  
  if (s->param.type == PSO_SWARM_TYPE_PARALLEL_PSO_MULTI_SWARM)
  {
    s->nParticles = s->unitArray->GetNUnits(s->unitArray->ctx);
    s->nParticlesPerUnit = 1;
    _swarms_if[s->linkKey].ComputeNextPos = (PsoSwarmComputeNextPos_fct) &_SwarmPara_ComputeNextPositions;
  }
  else if (s->param.type == PSO_SWARM_TYPE_PSO_1D)
  {
    s->nParticles = s->param.minParticles;
    s->nParticlesPerUnit = s->param.minParticles;
    _swarms_if[s->linkKey].ComputeNextPos = (PsoSwarmComputeNextPos_fct) &_Swarm1d_ComputeNextPositions;
  }
  else if (s->param.type == PSO_SWARM_TYPE_PARALLEL_PSO_SUB_SWARM)
  {
    s->nParticles = s->param.minParticles;
    s->nParticlesPerUnit = s->param.minParticles;
    _swarms_if[s->linkKey].ComputeNextPos = (PsoSwarmComputeNextPos_fct) &_SubSwarm_ComputeNextPositions;
  }
  else if (s->param.type == PSO_SWARM_TYPE_PARALLEL_PSO)
  {
    s->nParticles = unitArray->GetNUnits(unitArray->ctx);
    s->nParticlesPerUnit = 1;
    _swarms_if[s->linkKey].ComputeNextPos = (PsoSwarmComputeNextPos_fct) &_Swarm1d_ComputeNextPositions;
  }
  
  for (i = 0; i < s->nParticles; i++)
  {
    s->particles[i] = (PsoParticleInterface_t *) PsoParticleInterface();
    s->particles[i]->Init(s->particles[i]->ctx, i);
  }
  
  _Swarm_SetSteadyState(s);
  
  _Swarm_RandomizeAllParticles(s);
  
  return 0;
}


void _Swarm_ComputeAllParticlesPbest (PsoSwarm_t *s)
{
  UINT8 i;
  for (i = 0; i < s->nParticles; i++)
  {
    s->particles[i]->ComputePbest(s->particles[i]->ctx);
  }
}


UINT8 _Swarm_GetNParticles (PsoSwarm_t *s)
{
  return s->nParticles;
}


UINT32 _Swarm_GetIteration (PsoSwarm_t *s)
{
  return s->param.iteration;
}


UINT32 _Swarm_IterationIncrement (PsoSwarm_t *s)
{
  s->param.iteration++;
}


void _Swarm_Release (PsoSwarm_t *s)
{
  UINT8 i;
  Node_t *node = &_swarmsNodes[s->linkKey];
  
  s->param.iteration = 0;
  
  for (i = 0; i < s->nParticles; i++)
  {
    s->particles[i]->Release(s->particles[i]->ctx);
  }
  
  s->unitArray->Release(s->unitArray->ctx);
  
  LinkedList_RemoveNode(node->list, node);
  LinkedList_AddToEnd(&_unusedSwarms, node);
}


void _Swarm_GetGbest (PsoSwarm_t *s, Position_t *gbestDest)
{
  *gbestDest = s->gbest;
}


void _Swarm_GetParam (PsoSwarm_t *s, PsoSwarmParam_t *paramDest)
{
  *paramDest = s->param;
}


void _Swarm_SetSteadyState (PsoSwarm_t *s)
{
  UINT8 i;
  
  for (i = 0; i < s->nParticles; i++)
  {
    s->particles[i]->SetSteadyState(s->particles[i]->ctx, s->param.nSamplesForSteadyState, s->param.steadyStateOscAmp);
  }
}


void _Swarm_ComputeGbest (PsoSwarm_t *s)
{
  float max = 0, temp, fitness;
  UINT8 i, iBest;
  for (i = 0; i < s->nParticles; i++)
  {
    temp = max;
    fitness = s->particles[i]->GetFitness(s->particles[i]->ctx);
    max  = MAX(max, fitness);
    if (max != temp)
    {
      iBest = i;
    }
  }
  
  s->gbest.prevFitness = s->gbest.curFitness;
  s->gbest.prevPos = s->gbest.curPos;
  s->gbest.curFitness = max;
  s->gbest.curPos = s->particles[iBest]->GetPos(s->particles[iBest]->ctx);
}


void _Swarm_RandomizeAllParticles (PsoSwarm_t *s)
{
//  float range = s->param.posMax - s->param.posMin;
//  float sectionLength = range / s->nParticles;
//  float sections[PSO_SWARM_MAX_PARTICLES] = {0};
//  sections[0] = s->param.posMin;
//  sections[s->nParticles - 1] = s->param.posMax;
  UINT8 range = POT_MAX_INDEX - POT_MIN_INDEX;
  UINT8 sectionLength = (float) range / (float) s->nParticles + 0.5f;
  UINT8 sections[PSO_SWARM_MAX_PARTICLES + 1] = {0};
  sections[0] = POT_MIN_INDEX;
  sections[s->nParticles] = POT_MAX_INDEX;
  
  UINT8 i;
  PsoParticleInterface_t *p;
  float tmpPos;
  Position_t tmpPbest = {0};
  UINT8 potIdx;   // Potentiometer index
  
  for (i = 1; i < s->nParticles; i++)
  {
    sections[i] = sectionLength * i;
  }
  
  for (i = 0; i < s->nParticles; i++)
  {
    p = s->particles[i];
    
//    tmpPos = Rng_GetRandFloat() * sectionLength + sections[i];
//    tmpPos = MIN(MAX(s->param.posMin, tmpPos), s->param.posMax);
//    potIdx = ComputePotValueFloat2Dec(tmpPos);
    potIdx = Rng_GetRandBoundedUint32(sections[i], sections[i + 1]);
    tmpPos = potRealValues[potIdx];
    p->SetPos(p->ctx, tmpPos);
    tmpPos = p->GetPos(p->ctx);
    tmpPbest.curPos = tmpPos;
    p->SetPbest(p->ctx, &tmpPbest);
    p->SetPbestAbs(p->ctx, &tmpPbest);
  }
}


void _Swarm_RandomizeCertainParticles (PsoSwarm_t *s, UINT8 *idx, UINT8 nParticlesToRandomize)
{
  UINT8 i, potIdx;
  PsoParticleInterface_t *p;
  float tmpPos;
  Position_t tmpPbest = {0};
  float range = s->param.posMax - s->param.posMin;
  
  for (i = 0; i < nParticlesToRandomize; i++)
  {
    p = s->particles[idx[i]];
    
//    tmpPos = Rng_GetRandFloat() * range;
//    tmpPos = MIN(MAX(s->param.posMin, tmpPos), s->param.posMax);
    potIdx = Rng_GetRandBoundedUint32(POT_MIN_INDEX, POT_MAX_INDEX);
    tmpPos = potRealValues[potIdx];
    
    p->SetPos(p->ctx, tmpPos);
    tmpPos = p->GetPos(p->ctx);
    tmpPbest.curPos = tmpPos;
    p->SetPbest(p->ctx, &tmpPbest);
    p->SetPbestAbs(p->ctx, &tmpPbest);
  }
}


INT8 _Swarm_AddParticle (PsoSwarm_t *s, PsoParticleInterface_t *p)
{
  if (s->nParticles == PSO_SWARM_MAX_PARTICLES)
  {
    return -1;
  }
  
  s->particles[s->nParticles] = p;
  p->SetId(p->ctx, s->nParticles);
  s->nParticles++;
  return 0;
}


void * _Swarm_GetParticle (PsoSwarm_t *s, UINT8 idx)
{
  if (idx >= PSO_SWARM_MAX_PARTICLES)
  {
    return NULL;
  }
  return s->particles[idx];
}


void _Swarm_RemoveParticles (PsoSwarm_t *s, UINT8 *idx, UINT8 nParticles)
{
  UINT8 i;
  
  if (nParticles > s->nParticles)
  {
    return;
  }
  
  qsort( (void *) idx, (size_t) nParticles, sizeof(UINT8), &_CompareFunc);
  
  for (i = 0; i < nParticles; i++)
  {
    s->particles[idx[i]]->Release(s->particles[idx[i]]->ctx);
    _Swarm_ShiftParticlesLeft(s, idx[i]);
    s->nParticles--;
  }
  
  for (i = 0; i < s->nParticles; i++)
  {
    s->particles[i]->SetId(s->particles[i]->ctx, i);
  }
}


UINT8 _Swarm_CheckForPerturb (PsoSwarm_t *s, UINT8 *idxPerturbed)
{
  UINT8  nPerturbed = 0
        ,i = 0
        ;
  
  for (i = 0; i < s->nParticles; i++)
  {
    if (s->particles[i]->SentinelEval(s->particles[i]->ctx, s->param.sentinelMargin))
    {
      idxPerturbed[nPerturbed] = i;
      nPerturbed++;
      if (s->param.type == PSO_SWARM_TYPE_PSO_1D || s->param.type == PSO_SWARM_TYPE_PARALLEL_PSO)
      {
        s->oResetParticles = 1;
      }
    }
  }
  
  return nPerturbed;
}


UINT8 _Swarm_GetCurrentParticle (PsoSwarm_t *s)
{
  return s->param.currentParticle;
}


void _Swarm_IncrementCurrentParticle (PsoSwarm_t *s)
{
  s->param.currentParticle = (s->param.currentParticle + 1) == s->nParticles ? 0 : s->param.currentParticle + 1;
}


void _Swarm_SetAllParticlesFitness (PsoSwarm_t *s, float *fitBuf)
{
  UINT8 i;
  for (i = 0; i < s->nParticles; i++)
  {
    s->particles[i]->SetFitness(s->particles[i]->ctx, fitBuf[i]);
  }
}


void * _Swarm_GetUnitArray (PsoSwarm_t *s)
{
  return s->unitArray;
}


void _Swarm_SetParticleFitness (PsoSwarm_t *s, UINT8 idx, float fitness)
{
  s->particles[idx]->SetFitness(s->particles[idx]->ctx, fitness);
}


void _Swarm_UpdateParticlesFitness (PsoSwarm_t *s)
{
  UINT8 i;
  for (i = 0; i < s->nParticles; i++)
  {
    s->particles[i]->SetFitness(s->particles[i]->ctx, s->unitArray->GetPower(s->unitArray->ctx, i));
  }
}


UINT8 _SubSwarm_ComputeNextPositions (PsoSwarm_t *s, float *positions, UINT8 *idxToRemove)
{
  UINT8 i;
  UINT8 idxInSteadyState[PSO_SWARM_MAX_PARTICLES];
  UINT8 nIdxInSteadyState = 0;
  UINT8 idxPerturbed[PSO_SWARM_MAX_PARTICLES];
  UINT8 nPerturbed = 0;
  UINT8 nToRemove = 0;
  BOOL  oFirstSwarmActive = 0;
  UINT8 mainSwarmIdx = N_SWARMS_TOTAL - 1;
  PsoParticleInterface_t *p;
  for (i = 0; i < s->nParticles; i++)
  {
    p = s->particles[i];
    p->FsmStep(p->ctx, &_swarms_if[s->linkKey]);
    if (p->GetSteadyState(p->ctx))
    {
      idxInSteadyState[nIdxInSteadyState++] = i;
    }
    if (p->GetSentinelState(p->ctx))
    {
      idxPerturbed[nPerturbed++] = i;
    }
  }
  
  if (nIdxInSteadyState == s->nParticles)
  {
    for (i = 0; i < s->nParticles; i++)
    {
      s->particles[i]->SetPos(s->particles[i]->ctx, s->gbest.curPos);
      s->particles[i]->SetFitness(s->particles[i]->ctx, s->gbest.curFitness);
    }
  }
  
  if (nPerturbed)
  {
    _Swarm_RandomizeAllParticles(s);
    for (i = 0; i < s->nParticles; i++)
    {
      s->particles[i]->InitSpeed(s->particles[i]->ctx, &_swarms_if[s->linkKey]);
    }
    
    for (i = 0; i < _swarms[mainSwarmIdx].nParticles; i++)
    {
      if (_swarms[mainSwarmIdx].particles[i]->IsSearching(_swarms[mainSwarmIdx].particles[i]->ctx))
      {
        oFirstSwarmActive = 1;
        break;
      }
    }
    
    if (oFirstSwarmActive)
    {
      nToRemove = 1;
      for (i = 0; i < s->nParticles; i++)
      {
        s->particles[i]->ResetSteadyState(s->particles[i]->ctx);
      }
    }
  }
  
  for (i = 0; i < s->nParticles; i++)
  {
    positions[i] = s->particles[i]->GetPos(s->particles[i]->ctx);
  }
  
  return nToRemove;
}


UINT8 _SwarmPara_ComputeNextPositions (PsoSwarm_t *s, float *positions, UINT8 *idxToRemove)
{
  UINT8 i;
  UINT8 idxPerturbed[PSO_SWARM_MAX_PARTICLES];
  UINT8 nPerturbed = 0;
  UINT8 nToRemove = 0;
  PsoParticleInterface_t *p;
  for (i = 0; i < s->nParticles; i++)
  {
    p = s->particles[i];
    if ( p->FsmStep(p->ctx, &_swarms_if[s->linkKey]))
    {
      idxToRemove[nToRemove] = i;
      nToRemove++;
    }
    
    if ( p->GetSentinelState(p->ctx))
    {
      idxPerturbed[nPerturbed] = i;
      nPerturbed++;
    }
  }
  
  for (i = 0; i < s->nParticles; i++)
  {
    positions[i] = s->particles[i]->GetPos(s->particles[i]->ctx);
  }
  
  return nToRemove;
}


UINT8 _Swarm1d_ComputeNextPositions (PsoSwarm_t *s, float *positions, UINT8 *dummy)
{
  UINT8 i;
  
  if (s->oResetParticles)
  {
    s->oResetParticles = 0;
    for (i = 0; i < s->nParticles; i++)
    {
      s->particles[i]->Init     (s->particles[i]->ctx, i);
      s->particles[i]->InitSpeed(s->particles[i]->ctx, &_swarms_if[s->linkKey]);
    }
    _Swarm_RandomizeAllParticles(s);
    Position_Reset(&s->gbest);
  }
  else
  {
    if ( (s->param.iteration == 1) /* && (!s->oInSteadyState) */ )
    {
      for (i = 0; i < s->nParticles; i++)
      {
        s->particles[i]->InitSpeed (s->particles[i]->ctx, &_swarms_if[s->linkKey]);
        s->particles[i]->ComputePos(s->particles[i]->ctx, &_swarms_if[s->linkKey]);
      }
    }
    else
    {
      for (i = 0; i < s->nParticles; i++)
      {
        s->particles[i]->ComputeSpeed (s->particles[i]->ctx, &_swarms_if[s->linkKey]);
        s->particles[i]->ComputePos   (s->particles[i]->ctx, &_swarms_if[s->linkKey]);
      }
    }
  }
  
  for (i = 0; i < s->nParticles; i++)
  {
    positions[i] = s->particles[i]->GetPos(s->particles[i]->ctx);
  }
  
  return 0;
}


void _Swarm_SetParticlePos (PsoSwarm_t *s, UINT8 idx, float pos)
{
  s->particles[idx]->SetPos(s->particles[idx]->ctx, pos);
}


UINT8 _Swarm_GetId (PsoSwarm_t *s)
{
  return s->id;
}


void _Swarm_SetId (PsoSwarm_t *s, UINT8 id)
{
  s->id = id;
}


float _Swarm_GetParticlePos (PsoSwarm_t *s, UINT8 idx)
{
  return s->particles[idx]->GetPos(s->particles[idx]->ctx);
}


float _Swarm_GetParticleSpeed (PsoSwarm_t *s, UINT8 idx)
{
  return s->particles[idx]->GetSpeed(s->particles[idx]->ctx);
}


float _Swarm_GetParticleFitness (PsoSwarm_t *s, UINT8 idx)
{
  return s->particles[idx]->GetFitness(s->particles[idx]->ctx);
}


BOOL _Swarm_EvalSteadyState (PsoSwarm_t *s)
{
  UINT8 i;
  s->oInSteadyState = 1;
  for (i = 0; i < s->nParticles; i++)
  {
    if (!s->particles[i]->EvalSteadyState(s->particles[i]->ctx))
    {
      s->oInSteadyState = 0;
    }
  }
  return s->oInSteadyState;
}


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


void _Swarm_ShiftParticlesLeft (PsoSwarm_t *s, UINT8 idxToShift)
{
  if (idxToShift == (s->nParticles - 1))
  {
    return;
  }
  UINT8 offset = s->nParticles - idxToShift - 1;
  memmove(&s->particles[idxToShift], &s->particles[idxToShift + 1], offset * sizeof(PsoParticleInterface_t *));
}


static int _CompareFunc (const void *p1, const void *p2)
{
  if ( *(UINT8 *) p1  > *(UINT8 *) p2 ) return -1;
  if ( *(UINT8 *) p1 == *(UINT8 *) p2 ) return  0;
  if ( *(UINT8 *) p1  < *(UINT8 *) p2 ) return  1;
}


// Public functions
//==============================================================================

const PsoSwarmInterface_t * PsoSwarmInterface (void)
{
  UINT8 i;
  Node_t *temp;
  if (!_oSwarmsInitialized)
  {
    _oSwarmsInitialized = 1;
    
    _unusedSwarms.head = (void *) &_swarmsNodes[0];
    _unusedSwarms.count = 1;
    
    for (i = 0; i < N_SWARMS_TOTAL; i++)
    {
      _swarms   [i].linkKey                     = i;
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
      _swarms_if[i].GetGbest                    = (PsoSwarmGetGbest_fct)                  &_Swarm_GetGbest;
      _swarms_if[i].GetParam                    = (PsoSwarmGetParam_fct)                  &_Swarm_GetParam;
      _swarms_if[i].GetIteration                = (PsoSwarmGetIteration_fct)              &_Swarm_GetIteration;
      _swarms_if[i].IterationInc                = (PsoSwarmIterationIncrement_fct)        &_Swarm_IterationIncrement;
      _swarms_if[i].SetParticleFitness          = (PsoSwarmSetParticleFitness_fct)        &_Swarm_SetParticleFitness;
      _swarms_if[i].SetAllParticlesFitness      = (PsoSwarmSetAllParticlesFitness_fct)    &_Swarm_SetAllParticlesFitness;
      _swarms_if[i].GetNParticles               = (PsoSwarmGetNParticles_fct)             &_Swarm_GetNParticles;
      _swarms_if[i].IncCurrentParticle          = (PsoSwarmIncrementCurParticle_fct)      &_Swarm_IncrementCurrentParticle;
      _swarms_if[i].GetCurParticle              = (PsoSwarmGetCurParticle_fct)            &_Swarm_GetCurrentParticle;
      _swarms_if[i].ComputeAllPbest             = (PsoSwarmComputeAllParticlesPbest_fct)  &_Swarm_ComputeAllParticlesPbest;
      _swarms_if[i].EvalSteadyState             = (PsoSwarmEvalSteadyState_fct)           &_Swarm_EvalSteadyState;
      _swarms_if[i].ComputeNextPos              = (PsoSwarmComputeNextPos_fct)            0;
      _swarms_if[i].GetId                       = (PsoSwarmGetId_fct)                     &_Swarm_GetId;
      _swarms_if[i].GetUnitArray                = (PsoSwarmGetUnitArray_fct)              &_Swarm_GetUnitArray;
      _swarms_if[i].SetId                       = (PsoSwarmSetId_fct)                     &_Swarm_SetId;
      _swarms_if[i].GetParticlePos              = (PsoSwarmGetParticlePos_fct)            &_Swarm_GetParticlePos;
      _swarms_if[i].GetParticleFitness          = (PsoSwarmGetParticleFitness_fct)        &_Swarm_GetParticleFitness;
      _swarms_if[i].GetParticleSpeed            = (PsoSwarmGetParticleSpeed_fct)          &_Swarm_GetParticleSpeed;
      _swarms_if[i].UpdateParticlesFitness      = (PsoSwarmUpdateParticlesFitness_fct)    &_Swarm_UpdateParticlesFitness;
      _swarms_if[i].SetParticlePos              = (PsoSwarmSetParticlePos_fct)            &_Swarm_SetParticlePos;
      
      // Init the linked list
      _swarmsNodes[i].ctx = (void *) &_swarms_if[i];
      _swarmsNodes[i].key = i;
      if (i < (N_SWARMS_TOTAL - 1))
      {
        _swarmsNodes[i].next = &_swarmsNodes[i + 1];
      }
      else
      {
        _swarmsNodes[i].next = NULL;
      }
    }
    LinkedList_Init(&_unusedSwarms, &_swarmsNodes[0]);
  }
  
  if (_unusedSwarms.count == 0)
  {
    return NULL;
  }
  
  temp = _unusedSwarms.tail;
  LinkedList_RemoveNode(&_unusedSwarms, temp);
  LinkedList_AddToEnd(&_usedSwarms, temp);
  return temp->ctx;
}