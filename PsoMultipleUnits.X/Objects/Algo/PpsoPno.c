//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : PpsoPno.c
// Author  : Frederic Chasse
// Date    : 2017-07-16
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the Particle Swarm Optimization hybrid with P&O.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "PpsoPno.h"
#include "PsoSwarm.h"
#include "SteadyState.h"
#include "Classifier.h"
#include "PnoInstance.h"
#include "Potentiometer.h"


// Private definitions
//==============================================================================

typedef struct
{
  UnitArrayInterface_t *unitArray;
  float sampleTime;
  float timeElapsed;
  UINT32 iteration;
  UINT8 nParaSwarms;
  UINT8 nSeqSwarms;
  UINT8 nPnos;
  void *pnos[N_UNITS_TOTAL+1];
  PsoSwarmInterface_t *seqSwarms[N_UNITS_TOTAL + 1];
  PsoSwarmInterface_t *paraSwarms[N_UNITS_TOTAL + 1];
  ClassifierInterface_t *classifier;
} PpsoPno_t;

typedef struct
{
  float         delta;
  float         umin;
  float         umax;
  float         uinit;
  UINT8         nSamplesForSs;
  UINT8         oscAmp;
} PnoParam_t;

typedef struct
{
  UnitArrayInterface_t *unitArray;
  
  float sampleTime;
  float timeElapsed;
  UINT8 nInstances;
  PnoParam_t param[N_UNITS_TOTAL];
  PnoInstanceInterface_t *instances[N_UNITS_TOTAL];
} Pno_t;


// Private prototypes
//==============================================================================

INT8  _PpsoPno_Init               (PpsoPno_t *pso, UnitArrayInterface_t *unitArray);
INT8  _PpsoPno_Run                (PpsoPno_t *pso);
float _PpsoPno_GetTimeElapsed     (PpsoPno_t *pso);
void  _PpsoPno_Release            (PpsoPno_t *pso);
void  _PpsoPno_GetDebugData       (PpsoPno_t *pso, void *ret);

void _PpsoPno_ShiftParaSwarmsLeft (PpsoPno_t *pso, UINT8 idxToShift);
void _PpsoPno_ShiftSeqSwarmsLeft  (PpsoPno_t *pso, UINT8 idxToShift);
void _PpsoPno_ShiftPnosLeft       (PpsoPno_t *pso, UINT8 idxToShift);
static int _CompareFunc           (const void *p1, const void *p2);


// Private variables
//==============================================================================

PpsoPno_t _ppsoPno = 
{
  .unitArray    = NULL
 ,.sampleTime   = SAMPLING_TIME_FLOAT
 ,.timeElapsed  = 0
 ,.iteration    = 0
};

const AlgoInterface_t _ppsoPno_if =
{
  .ctx            = (void *)                  &_ppsoPno
 ,.Init           = (AlgoInit_fct)            &_PpsoPno_Init
 ,.Run            = (AlgoRun_fct)             &_PpsoPno_Run
 ,.GetTimeElapsed = (AlgoGetTimeElapsed_fct)  &_PpsoPno_GetTimeElapsed
 ,.Release        = (AlgoRelease_fct)         &_PpsoPno_Release
 ,.GetDebugData   = (AlgoGetDebugData_fct)    &_PpsoPno_GetDebugData
};


// Private functions
//==============================================================================

void _PpsoPno_GetDebugData (PpsoPno_t *pso, void *ret)
{
  return;
}


void _PpsoPno_Release (PpsoPno_t *pso)
{
  UINT8 i;
  for (i = 0; i < pso->nParaSwarms; i++)
  {
    pso->paraSwarms[i]->Release(pso->paraSwarms[i]->ctx);
    pso->paraSwarms[i] = NULL;
  }
  
  for (i = 0; i < pso->nSeqSwarms; i++)
  {
    pso->seqSwarms[i]->Release(pso->seqSwarms[i]->ctx);
    pso->seqSwarms[i] = NULL;
  }
  
  for (i = 0; i < pso->nPnos; i++)
  {
//    pso->pnos[i]->Release(pso->pnos[i]->ctx);
    pso->pnos[i] = NULL;
  }
  
  pso->iteration    = 0;
  pso->nParaSwarms  = 0;
  pso->nSeqSwarms   = 0;
  pso->nPnos        = 0;
  pso->timeElapsed  = 0;
  pso->unitArray    = NULL;
}


INT8 _PpsoPno_Init (PpsoPno_t *pso, UnitArrayInterface_t *unitArray)
{
  UnitArrayInterface_t *swarmArray;
  UINT8 nUnits;
  UINT8 i;
  float minPos, maxPos;
  pso->unitArray    = unitArray;
  pso->nParaSwarms  = 1;
  pso->nSeqSwarms   = 0;
  pso->nPnos        = 0;
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
   ,.perturbAmp             = 3*POT_STEP_VALUE
   ,.sentinelMargin         = 0.05
   ,.type                   = PSO_SWARM_TYPE_PARALLEL_PSO_MULTI_SWARM
   ,.nSamplesForSteadyState = 5
   ,.steadyStateOscAmp      = 0.01
   ,.iteration              = 0
   ,.currentParticle        = 0
  };
  
  pso->paraSwarms[0] = (PsoSwarmInterface_t *) PsoSwarmInterface();
  pso->paraSwarms[0]->Init(pso->paraSwarms[0]->ctx, swarmArray, (PsoSwarmParam_t *) &swarmParam, 0);
  
  for (i = 0; i < nUnits; i++)
  {
    unitArray->SetPos(unitArray->ctx, i, pso->paraSwarms[0]->GetParticlePos(pso->paraSwarms[0]->ctx, i));
  }
}


INT8 _PpsoPno_Run (PpsoPno_t *pso)
{
  UnitArrayInterface_t *array = pso->unitArray;
  PsoSwarmInterface_t *swarm;
//  PnoInterface_t *pno;
  UINT8  i            = 0
        ,nUnits       = array->GetNUnits(array->ctx)
        ,nParticles
        ;
  UINT8 idxPerturbed[N_UNITS_TOTAL];
  float fitness[N_UNITS_TOTAL];
  float nextPositions[N_UNITS_TOTAL];
  
  // <editor-fold defaultstate="collapsed" desc="Update algo values and classifier">
  pso->iteration++;
  pso->timeElapsed += pso->sampleTime;
  
  for (i = 0; i < nUnits; i++)
  {
    fitness[i] = array->GetPower(array->ctx, i);
  }
  
  pso->classifier->UpdateValues(pso->classifier->ctx);
  // </editor-fold>
  
  // <editor-fold defaultstate="collapsed" desc="Parallel PSO">
  // Parallel PSO
  //----------------------------------------------------------------------------
  for (i = 0; i < pso->nParaSwarms; i++)
  {
    swarm = pso->paraSwarms[i];
  }
  //----------------------------------------------------------------------------
  // </editor-fold>
  
  // <editor-fold defaultstate="collapsed" desc="P&O">
  // P&O
  //----------------------------------------------------------------------------
  for (i = 0; i < pso->nPnos; i++)
  {
//    pno = pso->pnos[i];
  }
  //----------------------------------------------------------------------------
  // </editor-fold>
  
  // <editor-fold defaultstate="collapsed" desc="Sequential PSO">
  // Sequential PSO
  //----------------------------------------------------------------------------
  for (i = 0; i < pso->nSeqSwarms; i++)
  {
    swarm = pso->seqSwarms[i];
  }
  //----------------------------------------------------------------------------
  // </editor-fold>
  
  // <editor-fold defaultstate="collapsed" desc="Classifier">
  // Classifier
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  // </editor-fold>

//  if (nParticles != 0)
//  {
//    for (i = 0; i < nParticles; i++)
//    {
//      swarm->SetParticleFitness(swarm->ctx, i, fitness[i]);
//    }
//    
//    swarm->IterationInc(swarm->ctx);
//
//    // Compute Pbest and Gbest
//    //-----------------------------------
//    swarm->ComputeAllPbest(swarm->ctx);
//    swarm->ComputeGbest   (swarm->ctx);
//    //-----------------------------------
//
//    // Check for perturbations
//    //-----------------------------------
//    swarm->CheckForPerturb(swarm->ctx, idxPerturbed);
//    //-----------------------------------
//
//    // Check for steady state of the particles and the swarm
//    //-----------------------------------
//    swarm->EvalSteadyState(swarm->ctx);
//    //-----------------------------------
//
//    // Compute next positions
//    //-----------------------------------
//    swarm->ComputeNextPos(swarm->ctx, nextPositions, 0);
//    //-----------------------------------
//
//    for (i = 0; i < nParticles; i++)
//    {
//      pso->unitArray->SetPos(pso->unitArray->ctx, i, swarm->GetParticlePos(swarm->ctx, i));
//    }
//  }
//  return 0;
}


float _PpsoPno_GetTimeElapsed (PpsoPno_t *pso)
{
  return pso->timeElapsed;
}


void _PpsoPno_ShiftPnosLeft (PpsoPno_t *pso, UINT8 idxToShift)
{
  if (idxToShift == (pso->nPnos - 1))
  {
    return;
  }
  UINT8 offset = pso->nPnos - idxToShift - 1;
//  memmove(&pso->pnos[idxToShift], &pso->pnos[idxToShift + 1], offset * sizeof(PnoInterface_t *));
}


void _PpsoPno_ShiftParaSwarmsLeft (PpsoPno_t *pso, UINT8 idxToShift)
{
  if (idxToShift == (pso->nParaSwarms - 1))
  {
    return;
  }
  UINT8 offset = pso->nParaSwarms - idxToShift - 1;
  memmove(&pso->paraSwarms[idxToShift], &pso->paraSwarms[idxToShift + 1], offset * sizeof(PsoSwarmInterface_t *));
}


void _PpsoPno_ShiftSeqSwarmsLeft (PpsoPno_t *pso, UINT8 idxToShift)
{
  if (idxToShift == (pso->nSeqSwarms - 1))
  {
    return;
  }
  UINT8 offset = pso->nSeqSwarms - idxToShift - 1;
  memmove(&pso->seqSwarms[idxToShift], &pso->seqSwarms[idxToShift + 1], offset * sizeof(PsoSwarmInterface_t *));
}


static int _CompareFunc (const void *p1, const void *p2)
{
  if ( *(UINT8 *) p1  > *(UINT8 *) p2 ) return -1;
  if ( *(UINT8 *) p1 == *(UINT8 *) p2 ) return  0;
  if ( *(UINT8 *) p1  < *(UINT8 *) p2 ) return  1;
}


// Public functions
//==============================================================================

const AlgoInterface_t * PpsoPnoInterface()
{
  return &_ppsoPno_if;
}