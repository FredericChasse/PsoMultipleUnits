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
INT8  _Pso_Close          (Pso_t *pso);
float _Pso_GetTimeElapsed (Pso_t *pso);

// Private variables
//==============================================================================

Pso_t _parallelPso = 
{
  .type         = PSO_TYPE_PARALLEL_PSO
 ,.nSwarms      = 0
 ,.swarms       = {0}
 ,.unitArray    = 0
 ,.sampleTime   = 0.08
 ,.timeElapsed  = 0
 ,.iteration    = 0
};

Pso_t _pso1d = 
{
  .type         = PSO_TYPE_PSO_1D
 ,.nSwarms      = 0
 ,.swarms       = {0}
 ,.unitArray    = 0
 ,.sampleTime   = 0.08
 ,.timeElapsed  = 0
 ,.iteration    = 0
};

const AlgoInterface_t _parallelPso_if =
{
  .ctx            = (void *)                  &_parallelPso
 ,.Init           = (AlgoInit_fct)            &_ParallelPso_Init
 ,.Run            = (AlgoRun_fct)             &_ParallelPso_Run
 ,.Close          = (AlgoClose_fct)           &_Pso_Close
 ,.GetTimeElapsed = (AlgoGetTimeElapsed_fct)  &_Pso_GetTimeElapsed
};

const AlgoInterface_t _pso1d_if =
{
  .ctx            = (void *)                  &_pso1d
 ,.Init           = (AlgoInit_fct)            &_Pso1d_Init
 ,.Run            = (AlgoRun_fct)             &_Pso1d_Run
 ,.Close          = (AlgoClose_fct)           &_Pso_Close
 ,.GetTimeElapsed = (AlgoGetTimeElapsed_fct)  &_Pso_GetTimeElapsed
};

// Private functions
//==============================================================================

INT8 _ParallelPso_Init (Pso_t *pso, UnitArrayInterface_t *unitArray)
{
  pso->unitArray = unitArray;
  pso->nSwarms = 1;
  pso->timeElapsed = 0;
  
  const PsoSwarmParam_t swarmParam = 
  {
    .c1                     = 1
   ,.c2                     = 2
   ,.omega                  = 0.4
   ,.posMin                 = 50
   ,.posMax                 = 1050
   ,.minParticles           = 3
   ,.perturbAmp             = 15
   ,.sentinelMargin         = 0.05
   ,.type                   = PSO_SWARM_TYPE_PARALLEL_PSO
   ,.nSamplesForSteadyState = 5
   ,.steadyStateOscAmp      = 0.01
   ,.iteration              = 0
   ,.currentParticle        = 0
  };
  
  pso->swarms[0] = (PsoSwarmInterface_t *) PsoSwarmInterface(0);
  pso->swarms[0]->Init(pso->swarms[0]->ctx, unitArray, (PsoSwarmParam_t *) &swarmParam);
  
  return 0;
}


INT8 _Pso1d_Init (Pso_t *pso, UnitArrayInterface_t *unitArray)
{
  UINT8 i;
  pso->unitArray = unitArray;
  pso->nSwarms = pso->unitArray->GetNUnits(pso->unitArray->ctx);
  pso->timeElapsed = 0;
  
  const PsoSwarmParam_t swarmParam = 
  {
    .c1                     = 1
   ,.c2                     = 2
   ,.omega                  = 0.4
   ,.posMin                 = 50
   ,.posMax                 = 1050
   ,.minParticles           = 3
   ,.perturbAmp             = 15
   ,.sentinelMargin         = 0.05
   ,.type                   = PSO_SWARM_TYPE_PSO_1D
   ,.nSamplesForSteadyState = 5
   ,.steadyStateOscAmp      = 0.01
   ,.iteration              = 0
   ,.currentParticle        = 0
  };
  
  for (i = 0; i < pso->nSwarms; i++)
  {
    pso->swarms[i] = (PsoSwarmInterface_t *) PsoSwarmInterface(i);
    pso->swarms[i]->Init(pso->swarms[i]->ctx, unitArray, (PsoSwarmParam_t *) &swarmParam);
  }
  return 0;
}


INT8 _ParallelPso_Run (Pso_t *pso)
{
  
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
  static float nextPositions[N_UNITS_TOTAL+1][PSO_SWARM_MAX_PARTICLES];
  
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
      swarm->SetParticleFitness(swarm->ctx, i, fitness[i]);
      
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
        swarm->ComputeNextPos(swarm->ctx, &nextPositions[iSwarm][0]);
        //-----------------------------------
      }
      
      swarm->IncCurrentParticle(swarm->ctx);
      curParticle = swarm->GetCurParticle(swarm->ctx);
      
      pso->unitArray->SetPos(pso->unitArray->ctx, iSwarm, nextPositions[iSwarm][curParticle]);
    }
  }
  
  
}


INT8 _Pso_Close (Pso_t *pso)
{
  UINT8 i;
  for (i = 0; i < pso->nSwarms; i++)
  {
    pso->swarms[i]->Release(pso->swarms[i]->ctx);
  }
  pso->nSwarms      = 0;
  pso->timeElapsed  = 0;
  pso->iteration    = 0;
  pso->unitArray    = NULL;
  return 0;
}


float _Pso_GetTimeElapsed (Pso_t *pso)
{
  return pso->timeElapsed;
}


// Public functions
//==============================================================================

const AlgoInterface_t * PsoInterface(PsoType_t psoType)
{
  switch (psoType)
  {
    case PSO_TYPE_PSO_1D:
      return &_pso1d_if;
    case PSO_TYPE_PARALLEL_PSO:
      return &_parallelPso_if;
    default:
      return NULL;
  }
}