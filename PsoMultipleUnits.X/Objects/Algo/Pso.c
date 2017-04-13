//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Pso.c
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the Particle Swarm Optimization.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "Pso.h"
#include "PsoSwarm.h"

// Private definitions
//==============================================================================

typedef struct
{
  UINT8 nSwarms;
  UnitArrayInterface_t *unitArray;
  PsoSwarmInterface_t *swarms[N_UNITS_TOTAL + 1];
  float sampleTime;
  float timeElapsed;
  UINT32 iteration;
  PsoType_t type;
} Pso_t;


// Private prototypes
//==============================================================================

INT8  _ParallelPso_Init   (Pso_t *pso, UnitArrayInterface_t *unitArray);
INT8  _Pso1d_Init         (Pso_t *pso, UnitArrayInterface_t *unitArray);
INT8  _ParallelPso_Run    (Pso_t *pso);
INT8  _Pso1d_Run          (Pso_t *pso);
float _Pso_GetTimeElapsed (Pso_t *pso);
void  _Pso_Release        (Pso_t *pso);

void _Pso_RemoveSubSwarm  (Pso_t *pso, UINT8 swarmId);
void _Pso_CreateSubSwarms (Pso_t *pso, UINT8 *particlesToRemove, UINT8 nParticlesToRemove);
void _Pso_ShiftSwarmsLeft (Pso_t *pso, UINT8 idxToShift);
static int _CompareFunc   (const void *p1, const void *p2);

// Private variables
//==============================================================================

Pso_t _parallelPso = 
{
  .type         = PSO_TYPE_PARALLEL_PSO_MULTI_SWARM
 ,.nSwarms      = 0
 ,.swarms       = {0}
 ,.unitArray    = 0
 ,.sampleTime   = SAMPLING_TIME_FLOAT
 ,.timeElapsed  = 0
 ,.iteration    = 0
};

Pso_t _pso1d = 
{
  .type         = PSO_TYPE_PSO_1D
 ,.nSwarms      = 0
 ,.swarms       = {0}
 ,.unitArray    = 0
 ,.sampleTime   = SAMPLING_TIME_FLOAT
 ,.timeElapsed  = 0
 ,.iteration    = 0
};

const AlgoInterface_t _parallelPso_if =
{
  .ctx            = (void *)                  &_parallelPso
 ,.Init           = (AlgoInit_fct)            &_ParallelPso_Init
 ,.Run            = (AlgoRun_fct)             &_ParallelPso_Run
 ,.GetTimeElapsed = (AlgoGetTimeElapsed_fct)  &_Pso_GetTimeElapsed
 ,.Release        = (AlgoRelease_fct)         &_Pso_Release
};

const AlgoInterface_t _pso1d_if =
{
  .ctx            = (void *)                  &_pso1d
 ,.Init           = (AlgoInit_fct)            &_Pso1d_Init
 ,.Run            = (AlgoRun_fct)             &_Pso1d_Run
 ,.GetTimeElapsed = (AlgoGetTimeElapsed_fct)  &_Pso_GetTimeElapsed
 ,.Release        = (AlgoRelease_fct)         &_Pso_Release
};

// Private functions
//==============================================================================

INT8 _ParallelPso_Init (Pso_t *pso, UnitArrayInterface_t *unitArray)
{
  UnitArrayInterface_t *swarmArray;
  UINT8 nUnits, i;
  float minPos, maxPos;
  
  pso->unitArray    = unitArray;
  pso->nSwarms      = 1;
  pso->timeElapsed  = 0;
  pso->sampleTime   = SAMPLING_TIME_FLOAT;
  
  swarmArray  = (UnitArrayInterface_t *) UnitArrayInterface();
  swarmArray->Init(swarmArray->ctx, 1);
  nUnits      = unitArray->GetNUnits(unitArray->ctx);
  for (i = 0; i < nUnits; i++)
  {
    swarmArray->AddUnitToArray(swarmArray->ctx, unitArray->GetUnitHandle(unitArray->ctx, i));
  }
  
  pso->unitArray->GetPosLimits(pso->unitArray->ctx, &minPos, &maxPos);
  const PsoSwarmParam_t swarmParam = 
  {
    .c1                     = 1
   ,.c2                     = 2
   ,.omega                  = 0.4
   ,.posMin                 = minPos
   ,.posMax                 = maxPos
   ,.minParticles           = 3
   ,.perturbAmp             = 15.7
   ,.sentinelMargin         = 0.05
   ,.type                   = PSO_SWARM_TYPE_PARALLEL_PSO
   ,.nSamplesForSteadyState = 5
   ,.steadyStateOscAmp      = 0.01
   ,.iteration              = 0
   ,.currentParticle        = 0
  };
  
  pso->swarms[0] = (PsoSwarmInterface_t *) PsoSwarmInterface();
  pso->swarms[0]->Init(pso->swarms[0]->ctx, swarmArray, (PsoSwarmParam_t *) &swarmParam, 0);
  
  for (i = 0; i < nUnits; i++)
  {
    unitArray->SetPos(unitArray->ctx, i, pso->swarms[0]->GetParticlePos(pso->swarms[0]->ctx, i));
  }
  
  return 0;
}


void _Pso_Release (Pso_t *pso)
{
  UINT8 i;
  for (i = 0; i < pso->nSwarms; i++)
  {
    pso->swarms[i]->Release(pso->swarms[i]->ctx);
    pso->swarms[i] = NULL;
  }
  
  pso->iteration    = 0;
  pso->nSwarms      = 0;
  pso->timeElapsed  = 0;
  pso->unitArray    = NULL;
}


INT8 _Pso1d_Init (Pso_t *pso, UnitArrayInterface_t *unitArray)
{
  UINT8 i;
  float minPos, maxPos;
  pso->unitArray    = unitArray;
  pso->nSwarms      = pso->unitArray->GetNUnits(pso->unitArray->ctx);
  pso->timeElapsed  = 0;
  pso->sampleTime   = SAMPLING_TIME_FLOAT;
  
  pso->unitArray->GetPosLimits(pso->unitArray->ctx, &minPos, &maxPos);
  const PsoSwarmParam_t swarmParam = 
  {
    .c1                     = 1
   ,.c2                     = 2
   ,.omega                  = 0.4
   ,.posMin                 = minPos
   ,.posMax                 = maxPos
   ,.minParticles           = 3
   ,.perturbAmp             = 15.7
   ,.sentinelMargin         = 0.05
   ,.type                   = PSO_SWARM_TYPE_PSO_1D
   ,.nSamplesForSteadyState = 5
   ,.steadyStateOscAmp      = 0.01
   ,.iteration              = 0
   ,.currentParticle        = 0
  };
  
  for (i = 0; i < pso->nSwarms; i++)
  {
    pso->swarms[i] = (PsoSwarmInterface_t *) PsoSwarmInterface();
    pso->swarms[i]->Init(pso->swarms[i]->ctx, unitArray, (PsoSwarmParam_t *) &swarmParam, i);
    
    unitArray->SetPos(unitArray->ctx, i, pso->swarms[i]->GetParticlePos(pso->swarms[i]->ctx, 0));
  }
  return 0;
}


INT8 _ParallelPso_Run (Pso_t *pso)
{
  UnitArrayInterface_t *array;
  PsoSwarmInterface_t *swarm;
  UINT8  i              = 0
        ,iSwarm         = 0
        ,nSwarmsMem     = pso->nSwarms    // So we don't do a swarm twice in the same iteration
        ,curParticle    = 0
        ,nParticles     = 0
        ,nParticlesMem  = 0
        ,nUnits         = 0
        ;
  UINT8 idxPerturbed[PSO_SWARM_MAX_PARTICLES];
  UINT8 nPerturbed = 0;
  UINT8 swarmsToDelete[N_SWARMS_TOTAL];
  UINT8 nSwarmsToDelete = 0;
  UINT8 idxToRemove[N_SWARMS_TOTAL];
  UINT8 nIdxToRemove = 0;
  float fitness[N_UNITS_TOTAL];
  float nextPositions[PSO_SWARM_MAX_PARTICLES];
  
  pso->iteration++;
  pso->timeElapsed += pso->sampleTime;

  // Parallel swarm
  //----------------------------------------------------------------------------
  iSwarm = 0;
  swarm = pso->swarms[0];
  array = swarm->GetUnitArray(swarm->ctx);
  nParticles = swarm->GetNParticles(swarm->ctx);
  if (nParticles != 0)
  {
    swarm->IterationInc(swarm->ctx);
    
    // Set the fitness of each particle
    //--------------------------------------
    for (i = 0; i < nParticles; i++)
    {
      swarm->SetParticleFitness(swarm->ctx, i, array->GetPower(array->ctx, i));
    }
    //--------------------------------------
    
    // Compute pbest and gbest
    //--------------------------------------
    swarm->ComputeAllPbest(swarm->ctx);
    swarm->ComputeGbest   (swarm->ctx);
    //--------------------------------------
        
    // Check for perturbations
    //-----------------------------------
    nPerturbed = swarm->CheckForPerturb(swarm->ctx, idxPerturbed);
    //-----------------------------------

    // Check for steady state of the particles
    //-----------------------------------
    swarm->EvalSteadyState(swarm->ctx);
    //-----------------------------------
        
    // Compute next positions
    //-----------------------------------
    nIdxToRemove = swarm->ComputeNextPos(swarm->ctx, nextPositions, idxToRemove);
    //-----------------------------------
    
    if (nPerturbed)
    {
      if (nPerturbed == nParticles)
      {
        swarm->RandomizeAllParticles(swarm->ctx);
      }
      else
      {
        swarm->RandomizeCertainParticles(swarm->ctx, idxPerturbed, nPerturbed);
      }
    }
    
    if (nIdxToRemove)
    {
      _Pso_CreateSubSwarms(pso, idxToRemove, nIdxToRemove);
    }
    
    nUnits = array->GetNUnits(array->ctx);
    for (i = 0; i < nUnits; i++)
    {
      array->SetPos(array->ctx, i, nextPositions[i]);
    }
    
    for (i = nSwarmsMem; i < pso->nSwarms; i++)
    {
      swarm = pso->swarms[i];
      array = swarm->GetUnitArray(swarm->ctx);
      array->SetPos(array->ctx, 0, swarm->GetParticlePos(swarm->ctx, 0));
    }
  }
  //----------------------------------------------------------------------------
  
  // Swarms 1D
  //----------------------------------------------------------------------------
  for (iSwarm = 1; iSwarm < nSwarmsMem; iSwarm++)
  {
    swarm = pso->swarms[iSwarm];
    nParticles = swarm->GetNParticles(swarm->ctx);
    
    if (nParticles != 0)
    {
      array = swarm->GetUnitArray(swarm->ctx);
      fitness[0] = array->GetPower(array->ctx, 0);
      curParticle = swarm->GetCurParticle(swarm->ctx);
      swarm->SetParticleFitness(swarm->ctx, curParticle, fitness[0]);
      
      if (curParticle == (nParticles - 1))  // Ready for a real iteration
      {
        swarm->IterationInc(swarm->ctx);
        
        // Compute Pbest and Gbest
        //-----------------------------------
        swarm->ComputeAllPbest(swarm->ctx);
        swarm->ComputeGbest   (swarm->ctx);
        //-----------------------------------
        
        // Check for perturbations
        //-----------------------------------
        swarm->CheckForPerturb(swarm->ctx, idxPerturbed);
        //-----------------------------------
        
        // Check for steady state of the particles and the swarm
        //-----------------------------------
        swarm->EvalSteadyState(swarm->ctx);
        //-----------------------------------
        
        // Compute next positions
        //-----------------------------------
        nIdxToRemove = swarm->ComputeNextPos(swarm->ctx, nextPositions, 0);
        //-----------------------------------
        
        if (nIdxToRemove)
        {
          swarmsToDelete[nSwarmsToDelete++] = iSwarm;
        }
      }
      
      swarm->IncCurrentParticle(swarm->ctx);
      curParticle = swarm->GetCurParticle(swarm->ctx);
      array->SetPos(array->ctx, 0, swarm->GetParticlePos(swarm->ctx, curParticle));
    }
  }
  //----------------------------------------------------------------------------
  
  if (nSwarmsToDelete)
  {
    swarm = pso->swarms[0];
    nParticlesMem = swarm->GetNParticles(swarm->ctx);
    
    for (i = nSwarmsToDelete - 1; i >= 0 ; i--)
    {
      _Pso_RemoveSubSwarm(pso, swarmsToDelete[i]);
    }
    
    array = swarm->GetUnitArray(swarm->ctx);
    nParticles = swarm->GetNParticles(swarm->ctx);
    for (i = nParticlesMem; i < nParticles; i++)
    {
      array->SetPos(array->ctx, i, swarm->GetParticlePos(swarm->ctx, i));
    }
  }
  return 0;
}


void _Pso_RemoveSubSwarm (Pso_t *pso, UINT8 swarmId)
{
  PsoSwarmInterface_t *mainSwarm = pso->swarms[0];
  void *particle = mainSwarm->GetParticle(mainSwarm->ctx, 0);
  
  UnitArrayInterface_t *oldArray = pso->swarms[swarmId]->GetUnitArray(pso->swarms[swarmId]->ctx);
  UnitArrayInterface_t *mainArray = mainSwarm->GetUnitArray(mainSwarm->ctx);
  void *unit = oldArray->GetUnitHandle(oldArray->ctx, 0);
  oldArray->Release(oldArray->ctx);
  
  pso->swarms[swarmId]->Release(pso->swarms[swarmId]->ctx);
  _Pso_ShiftSwarmsLeft(pso, swarmId);
  pso->nSwarms--;
  
  mainSwarm->AddParticle(mainSwarm->ctx, particle);
  mainArray->AddUnitToArray(mainArray->ctx, unit);
}


void _Pso_CreateSubSwarms (Pso_t *pso, UINT8 *particlesToRemove, UINT8 nParticlesToRemove)
{
  PsoSwarmInterface_t    *swarm = pso->swarms[0];
  UnitArrayInterface_t   *initArray = swarm->GetUnitArray(swarm->ctx)
                        ,*newArrays[N_UNITS_TOTAL] = {0}
                        ;
  PsoSwarmInterface_t    *newSwarms[N_UNITS_TOTAL] = {0};
  void *units[N_UNITS_TOTAL] = {0};
  UINT8 i;
  PsoSwarmParam_t swarmParam;
  
  for (i = 0; i < nParticlesToRemove; i++)
  {
    newArrays[i] = (UnitArrayInterface_t *) UnitArrayInterface();
    newSwarms[i] = (PsoSwarmInterface_t  *) PsoSwarmInterface ();
    units    [i] = initArray->GetUnitHandle(initArray->ctx, particlesToRemove[i]);
    newArrays[i]->Init(newArrays[i]->ctx, i+2);
    newArrays[i]->AddUnitToArray(newArrays[i]->ctx, units[i]);
  }
  
  swarm->RemoveParticles(swarm->ctx, particlesToRemove, nParticlesToRemove);
  
  qsort( (void *) particlesToRemove, (size_t) nParticlesToRemove, sizeof(UINT8), &_CompareFunc);
  
  for (i = 0; i < nParticlesToRemove; i++)
  {
    initArray->RemoveUnitFromArray(initArray->ctx, particlesToRemove[i]);
  }
  
  swarm->GetParam(swarm->ctx, &swarmParam);
  
  swarmParam.currentParticle = 0;
  swarmParam.iteration = 0;
  swarmParam.type = PSO_SWARM_TYPE_PARALLEL_PSO_SUB_SWARM;
  
  for (i = 0; i < nParticlesToRemove; i++)
  {
    newSwarms[i]->Init(newSwarms[i]->ctx, newArrays[i], &swarmParam, pso->nSwarms);
    pso->swarms[pso->nSwarms] = newSwarms[i];
    pso->nSwarms++;
    newSwarms[i]->RandomizeAllParticles(newSwarms[i]->ctx);
  }
}


INT8 _Pso1d_Run (Pso_t *pso)
{
  UnitArrayInterface_t *array = pso->unitArray;
  PsoSwarmInterface_t *swarm;
  UINT8  i            = 0
        ,iSwarm       = 0
        ,nUnits       = array->GetNUnits(array->ctx)
        ,curParticle  = 0
        ,nParticles   = 0
        ;
  UINT8 idxPerturbed[PSO_SWARM_MAX_PARTICLES];
  float fitness[N_UNITS_TOTAL];
  float nextPositions[N_UNITS_TOTAL+1];
  
  pso->iteration++;
  pso->timeElapsed += pso->sampleTime;
  
  for (i = 0; i < nUnits; i++)
  {
    fitness[i] = array->GetPower(array->ctx, i);
  }
  
  for (iSwarm = 0; iSwarm < pso->nSwarms; iSwarm++)
  {
    swarm = pso->swarms[iSwarm];
    nParticles = swarm->GetNParticles(swarm->ctx);
    
    if (nParticles != 0)
    {
      curParticle = swarm->GetCurParticle(swarm->ctx);
      swarm->SetParticleFitness(swarm->ctx, curParticle, fitness[iSwarm]);
      
      if (curParticle == (nParticles - 1))  // Ready for a real iteration
      {
        swarm->IterationInc(swarm->ctx);
        
        // Compute Pbest and Gbest
        //-----------------------------------
        swarm->ComputeAllPbest(swarm->ctx);
        swarm->ComputeGbest   (swarm->ctx);
        //-----------------------------------
        
        // Check for perturbations
        //-----------------------------------
        swarm->CheckForPerturb(swarm->ctx, idxPerturbed);
        //-----------------------------------
        
        // Check for steady state of the particles and the swarm
        //-----------------------------------
        swarm->EvalSteadyState(swarm->ctx);
        //-----------------------------------
        
        // Compute next positions
        //-----------------------------------
        swarm->ComputeNextPos(swarm->ctx, nextPositions, 0);
        //-----------------------------------
      }
      
      swarm->IncCurrentParticle(swarm->ctx);
      curParticle = swarm->GetCurParticle(swarm->ctx);
      
      pso->unitArray->SetPos(pso->unitArray->ctx, iSwarm, swarm->GetParticlePos(swarm->ctx, curParticle));
    }
  }
  return 0;
}


float _Pso_GetTimeElapsed (Pso_t *pso)
{
  return pso->timeElapsed;
}


void _Pso_ShiftSwarmsLeft (Pso_t *pso, UINT8 idxToShift)
{
  if (idxToShift == (pso->nSwarms - 1))
  {
    return;
  }
  UINT8 offset = pso->nSwarms - idxToShift - 1;
  memmove(&pso->swarms[idxToShift], &pso->swarms[idxToShift + 1], offset * sizeof(PsoSwarmInterface_t *));
}


static int _CompareFunc (const void *p1, const void *p2)
{
  if ( *(UINT8 *) p1  > *(UINT8 *) p2 ) return -1;
  if ( *(UINT8 *) p1 == *(UINT8 *) p2 ) return  0;
  if ( *(UINT8 *) p1  < *(UINT8 *) p2 ) return  1;
}


// Public functions
//==============================================================================

const AlgoInterface_t * PsoInterface(PsoType_t psoType)
{
  switch (psoType)
  {
    case PSO_TYPE_PSO_1D:
      return &_pso1d_if;
    case PSO_TYPE_PARALLEL_PSO_MULTI_SWARM:
      return &_parallelPso_if;
    default:
      return NULL;
  }
}