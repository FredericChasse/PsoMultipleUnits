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
#include "PnoSwarm.h"
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
  PsoSwarmParam_t swarmParam;
  PnoSwarmParam_t pnoParam;
  PnoSwarmInterface_t *pnos[N_UNITS_TOTAL];
  PsoSwarmInterface_t *seqSwarms[N_UNITS_TOTAL];
  PsoSwarmInterface_t *paraSwarms[N_UNITS_TOTAL];
  ClassifierInterface_t *classifier;
} PpsoPno_t;


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
 ,.nParaSwarms  = 0
 ,.nSeqSwarms   = 0
 ,.nPnos        = 0
 ,.paraSwarms   = {NULL}
 ,.seqSwarms    = {NULL}
 ,.pnos         = {NULL}
 ,.classifier   = NULL
 ,.swarmParam   = {NULL}
 ,.pnoParam     = {NULL}
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
    pso->pnos[i]->Release(pso->pnos[i]->ctx);
    pso->pnos[i] = NULL;
  }
  
  pso->classifier->Release(pso->classifier->ctx);
  
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
  
  pso->classifier = (ClassifierInterface_t *) ClassifierInterface();
  pso->classifier->Init(pso->classifier->ctx, unitArray, 3*POT_STEP_VALUE);
  
  pso->unitArray->GetPosLimits(pso->unitArray->ctx, &minPos, &maxPos);
  
  const PsoSwarmParam_t _swarmParam = 
  {
    .c1                     = 1
   ,.c2                     = 2
   ,.omega                  = 0.4
   ,.posMin                 = minPos
   ,.posMax                 = maxPos
   ,.minParticles           = 3
   ,.perturbAmp             = 2*POT_STEP_VALUE
   ,.sentinelMargin         = 0.05
   ,.type                   = PSO_SWARM_TYPE_PARALLEL_PSO_MULTI_SWARM
   ,.nSamplesForSteadyState = 5
   ,.steadyStateOscAmp      = 0.01
   ,.iteration              = 0
   ,.currentParticle        = 0
  };
  
  const PnoSwarmParam_t _pnoParam = 
  {
    .delta_int      = 1
   ,.delta          = _pnoParam.delta_int * POT_STEP_VALUE
   ,.uinit_int      = POT_MAX_INDEX/2
   ,.uinit          = potRealValues[_pnoParam.uinit_int]
   ,.umax_int       = POT_MAX_INDEX
   ,.umax           = potRealValues[_pnoParam.umax_int]
   ,.umin_int       = POT_MIN_INDEX
   ,.umin           = potRealValues[_pnoParam.umin_int]
   ,.nSamplesForSs  = 6
   ,.oscAmp         = 2
   ,.perturbOsc     = 0.05
  };
  
  memcpy(&pso->swarmParam, &_swarmParam, sizeof(PsoSwarmParam_t));
  memcpy(&pso->pnoParam  , &_pnoParam  , sizeof(PnoSwarmParam_t));
  
  swarmArray = (UnitArrayInterface_t *) UnitArrayInterface();
  swarmArray->Init(swarmArray->ctx, 1);
  nUnits = unitArray->GetNUnits(unitArray->ctx);
  for (i = 0; i < nUnits; i++)
  {
    swarmArray->AddUnitToArray(swarmArray->ctx, unitArray->GetUnitHandle(unitArray->ctx, i));
  }
  
  pso->paraSwarms[0] = (PsoSwarmInterface_t *) PsoSwarmInterface();
  pso->paraSwarms[0]->Init(pso->paraSwarms[0]->ctx, swarmArray, &pso->swarmParam, 0);
  
  for (i = 0; i < nUnits; i++)
  {
    unitArray->SetPos(unitArray->ctx, i, pso->paraSwarms[0]->GetParticlePos(pso->paraSwarms[0]->ctx, i));
  }
}


INT8 _PpsoPno_Run (PpsoPno_t *pso)
{
  UnitArrayInterface_t *array = pso->unitArray;
  PsoSwarmInterface_t *swarm;
  PnoSwarmInterface_t *pno;
  UINT8  i = 0
        ,j
        ,nUnits = array->GetNUnits(array->ctx)
        ,nParticles
        ,iUnit
        ,curParticle
        ;
  BOOL oInSteadyState;
  UINT8 idxPerturbed[N_UNITS_TOTAL];
  float fitness[N_UNITS_TOTAL];
  float nextPositions[N_UNITS_TOTAL];
  UINT32 nPerturbed;
  UINT32 iAlgo = 0;
  UINT8 nIdxToRemove;
  UINT8 idxToRemove[N_UNITS_TOTAL];
  UINT8 allIdxPerturbed[N_UNITS_TOTAL][N_UNITS_TOTAL] = {0};
  UINT8 allIdxPerturbedSize[N_UNITS_TOTAL] = {0};
  UINT8 allIdxToRemove[N_UNITS_TOTAL][N_UNITS_TOTAL] = {0};
  UINT8 allIdxToRemoveSize[N_UNITS_TOTAL] = {0};
  
  
  // <editor-fold defaultstate="collapsed" desc="Update algo values and classifier">
  pso->iteration++;
  pso->timeElapsed += pso->sampleTime;
  
  if (pso->iteration >= 27)
  {
    nIdxToRemove = 0;
  }
  
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
    
    swarm->IterationInc(swarm->ctx);
    
    nParticles = swarm->GetNParticles(swarm->ctx);
    
    // Update the fitness of the particles
    //--------------------------------------
    swarm->UpdateParticlesFitness(swarm->ctx);
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
    
    // Particles that haven't found their optimum
    //-----------------------------------
    if (nIdxToRemove)
    {
      memcpy(&allIdxToRemove[iAlgo][0], idxToRemove, nIdxToRemove);
      allIdxToRemoveSize[iAlgo] = nIdxToRemove;
    }
    else
    {
      allIdxToRemoveSize[iAlgo] = 0;
    }
    //-----------------------------------
    
    // Particles perturbed
    //-----------------------------------
    if (nPerturbed)
    {
      memcpy(&allIdxPerturbed[iAlgo][0], idxPerturbed, nPerturbed);
      allIdxPerturbedSize[iAlgo] = nPerturbed;
    }
    else
    {
      allIdxPerturbedSize[iAlgo] = 0;
    }
    //-----------------------------------
    
    array = swarm->GetUnitArray(swarm->ctx);
    nUnits = array->GetNUnits(array->ctx);
    
    for (iUnit = 0; iUnit < nUnits; iUnit++)
    {
      array->SetPos(array->ctx, iUnit, nextPositions[iUnit]);
    }
    
    iAlgo++;
  }
  //----------------------------------------------------------------------------
  // </editor-fold>
  
  // <editor-fold defaultstate="collapsed" desc="P&O">
  // P&O
  //----------------------------------------------------------------------------
  for (i = 0; i < pso->nPnos; i++)
  {
    pno = pso->pnos[i];
    
    pno->IncIteration(pno->ctx);
    
    nPerturbed = pno->ComputeAllPos(pno->ctx, nextPositions, idxPerturbed);
    
    nUnits = pno->GetNInstances(pno->ctx);
    
    array = pno->GetArray(pno->ctx);
    
    for (iUnit = 0; iUnit < nUnits; iUnit++)
    {
      if (pno->GetSteadyState(pno->ctx, iUnit))
      {
        nextPositions[iUnit] = pso->classifier->GetBestPos(pso->classifier->ctx, array->GetUnitId(array->ctx, iUnit));
        pno->SetPos(pno->ctx, iUnit, nextPositions[iUnit]);
      }
      
      array->SetPos(array->ctx, iUnit, nextPositions[iUnit]);
    }
    
    if (nPerturbed)
    {
      memcpy(&allIdxPerturbed[iAlgo][0], idxPerturbed, nPerturbed);
      allIdxPerturbedSize[iAlgo] = nPerturbed;
    }
    iAlgo++;
  }
  //----------------------------------------------------------------------------
  // </editor-fold>
  
  // <editor-fold defaultstate="collapsed" desc="Sequential PSO">
  // Sequential PSO
  //----------------------------------------------------------------------------
  for (i = 0; i < pso->nSeqSwarms; i++)
  {
    swarm = pso->seqSwarms[i];
    nParticles = swarm->GetNParticles(swarm->ctx);
    array = swarm->GetUnitArray(swarm->ctx);
    
    curParticle = swarm->GetCurParticle(swarm->ctx);
    swarm->SetParticleFitness(swarm->ctx, curParticle, array->GetPower(array->ctx, 0));

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
      nPerturbed = swarm->CheckForPerturb(swarm->ctx, idxPerturbed);
      //-----------------------------------

      // Check for steady state of the particles and the swarm
      //-----------------------------------
      oInSteadyState = swarm->EvalSteadyState(swarm->ctx);
      //-----------------------------------

      // Compute next positions
      //-----------------------------------
      swarm->ComputeNextPos(swarm->ctx, nextPositions, 0);
      //-----------------------------------
      
      // Assess perturbations
      //-----------------------------------
      if (nPerturbed)
      {
        memcpy(&allIdxPerturbed[iAlgo][0], idxPerturbed, nPerturbed);
        allIdxPerturbedSize[iAlgo] = nPerturbed;
      }
      //-----------------------------------
      
      // Assess steady state
      //-----------------------------------
      if (oInSteadyState)
      {
        nParticles = swarm->GetNParticles(swarm->ctx);
        for (iUnit = 0; iUnit < nParticles; i++)
        {
          swarm->SetParticlePos(swarm->ctx, iUnit, pso->classifier->GetBestPos(pso->classifier->ctx, array->GetUnitId(array->ctx, 0)));
        }
        for (j = 0; j < pso->swarmParam.nSamplesForSteadyState; j++)
        {
          swarm->EvalSteadyState(swarm->ctx);
        }
      }
      //-----------------------------------
    }

    swarm->IncCurrentParticle(swarm->ctx);
    curParticle = swarm->GetCurParticle(swarm->ctx);

    nextPositions[0] = swarm->GetParticlePos(swarm->ctx, curParticle);
    array->SetPos(array->ctx, 0, nextPositions[0]);
    
    iAlgo++;
  }
  //----------------------------------------------------------------------------
  // </editor-fold>
  
  // <editor-fold defaultstate="collapsed" desc="Classifier">
  // Classifier
  //----------------------------------------------------------------------------
  
  UINT8 algoIdxPerturbed[N_UNITS_TOTAL] = {0};
  UINT8 algoIdxPerturbedSize = 0;
  UINT8 tmp, idx;
  iAlgo = 0;
  nPerturbed = 0;
  
  // Set the correct algo indexes
  //-----------------------------------
  if (pso->nPnos)
  {
    tmp = pso->nParaSwarms + pso->nPnos;
    for (i = pso->nParaSwarms; i < tmp; i++)
    {
      algoIdxPerturbed[algoIdxPerturbedSize++] = i;
    }
  }
  if (pso->nSeqSwarms)
  {
    tmp = pso->nParaSwarms + pso->nPnos + pso->nSeqSwarms;
    for (i = pso->nParaSwarms + pso->nPnos; i < tmp; i++)
    {
      algoIdxPerturbed[algoIdxPerturbedSize++] = i;
    }
  }
  if (pso->nParaSwarms)
  {
    tmp = pso->nParaSwarms;
    for (i = 0; i < tmp; i++)
    {
      algoIdxPerturbed[algoIdxPerturbedSize++] = i;
    }
  }
  //-----------------------------------
  
  // Assess perturbed P&O instances
  //-----------------------------------
  for (i = 0; i < pso->nPnos; i++)
  {
    idx = algoIdxPerturbed[iAlgo++];
    pno = pso->pnos[i];
    if (allIdxPerturbedSize[idx])
    {
      array = pno->GetArray(pno->ctx);
      for (iUnit = 0; iUnit < allIdxPerturbedSize[idx]; iUnit++)
      {
        idxPerturbed[nPerturbed++] = array->GetUnitId(array->ctx, allIdxPerturbed[idx][iUnit]);
      }
      
      if (allIdxPerturbedSize[idx] == pno->GetNInstances(pno->ctx))
      {
        pno->Release(pno->ctx);
        _PpsoPno_ShiftPnosLeft(pso, i);
        i--;
        pso->nPnos--;
        array->Release(array->ctx);
      }
      else
      {
        pno->RemoveInstances(pno->ctx, &allIdxPerturbed[idx][0], allIdxPerturbedSize[idx]);
        
        qsort( (void *) &allIdxPerturbed[idx][0], (size_t) allIdxPerturbedSize[idx], sizeof(UINT8), &_CompareFunc);
        for (iUnit = allIdxPerturbedSize[idx]; iUnit > 0; iUnit--)
        {
          array->RemoveUnitFromArray(array->ctx, allIdxPerturbed[idx][iUnit-1]);
        }
      }
    }
  }
  if (pso->nPnos)
  {
    for (i = 0; i < pso->nPnos; i++)
    {
      pso->pnos[i]->SetId(pso->pnos[i]->ctx, i);
    }
  }
  //-----------------------------------
  
  // Assess perturbed Sequential PSO
  //-----------------------------------
  for (i = 0; i < pso->nSeqSwarms; i++)
  {
    idx = algoIdxPerturbed[iAlgo++];
    swarm = pso->seqSwarms[i];
    if (allIdxPerturbedSize[idx])
    {
      array = swarm->GetUnitArray(swarm->ctx);
      for (iUnit = 0; iUnit < allIdxPerturbedSize[idx]; iUnit++)
      {
        idxPerturbed[nPerturbed++] = array->GetUnitId(array->ctx, allIdxPerturbed[idx][iUnit]);
      }
      
      swarm->Release(swarm->ctx);
      _PpsoPno_ShiftSeqSwarmsLeft(pso, i);
      i--;
      pso->nSeqSwarms--;
      array->Release(array->ctx);
    }
  }
  if (pso->nSeqSwarms)
  {
    for (i = 0; i < pso->nSeqSwarms; i++)
    {
      pso->seqSwarms[i]->SetId(pso->seqSwarms[i]->ctx, i);
    }
  }
  //-----------------------------------
  
  // Assess perturbed Parallel PSO
  //-----------------------------------
  void *units[N_UNITS_TOTAL];
  UnitArrayInterface_t *newArray;
  for (i = 0; i < pso->nParaSwarms; i++)
  {
    swarm = pso->paraSwarms[i];
    array = swarm->GetUnitArray(swarm->ctx);
    
    if (allIdxToRemoveSize[i])
    {
      qsort( (void *) &allIdxToRemove[i][0], (size_t) allIdxToRemoveSize[idx], sizeof(UINT8), &_CompareFunc);
      for (iUnit = 0; iUnit < allIdxToRemoveSize[i]; iUnit++)
      {
        nextPositions[iUnit] = swarm->GetParticlePos(swarm->ctx, allIdxToRemove[i][iUnit]);
        units[iUnit] = array->GetUnitHandle(array->ctx, allIdxToRemove[i][iUnit]);
      }
//      swarm->RemoveParticles(swarm->ctx, &allIdxToRemove[i][0], allIdxToRemoveSize[i]);
      
      newArray = (UnitArrayInterface_t *) UnitArrayInterface();
//      for (iUnit = allIdxToRemoveSize[i]; iUnit > 0; iUnit--)
//      {
//        array->RemoveUnitFromArray(array->ctx, allIdxToRemove[i][iUnit-1]);
//      }
      for (iUnit = 0; iUnit < allIdxToRemoveSize[i]; iUnit++)
      {
        newArray->AddUnitToArray(newArray->ctx, units[iUnit]);
      }
      
      pso->pnos[pso->nPnos++] = (PnoSwarmInterface_t *) PnoSwarmInterface();
      pno = pso->pnos[pso->nPnos - 1];
      pno->Init(pno->ctx, newArray, &pso->pnoParam, pso->nPnos-1);
      nUnits = pno->GetNInstances(pno->ctx);
      for (iUnit = 0; iUnit < nUnits; iUnit++)
      {
        pno->SetPos(pno->ctx, iUnit, nextPositions[iUnit]);
        newArray->SetPos(newArray->ctx, iUnit, nextPositions[iUnit]);
      }
    }
    
    idx = algoIdxPerturbed[iAlgo++];
    
    if (allIdxPerturbedSize[idx])
    {
      for (iUnit = 0; iUnit < allIdxPerturbedSize[idx]; iUnit++)
      {
        idxPerturbed[nPerturbed++] = array->GetUnitId(array->ctx, allIdxPerturbed[idx][iUnit]);
      }
      
      if (allIdxToRemove[i])
      {
        memcpy(&allIdxPerturbed[idx][allIdxPerturbedSize[idx]], &allIdxToRemove[i][0], allIdxToRemoveSize[i]);
        allIdxPerturbedSize[idx] += allIdxToRemoveSize[i];
      }
      
      nUnits = swarm->GetNParticles(swarm->ctx);
      if (allIdxPerturbedSize[idx] == nUnits)
      {
        swarm->Release(swarm->ctx);
        array->Release(array->ctx);
        _PpsoPno_ShiftParaSwarmsLeft(pso, i);
        pso->nParaSwarms--;
        i--;
      }
      else if ((nUnits - allIdxPerturbedSize[idx]) < pso->swarmParam.minParticles)
      {
        swarm->RemoveParticles(swarm->ctx, &allIdxPerturbed[idx][0], allIdxPerturbedSize[idx]);
        for (iUnit = 0; iUnit < allIdxPerturbedSize[idx]; iUnit++)
        {
          array->RemoveUnitFromArray(array->ctx, allIdxPerturbed[idx][iUnit]);
        }
        nUnits = swarm->GetNParticles(swarm->ctx);
        newArray = (UnitArrayInterface_t *) UnitArrayInterface();
        for (iUnit = 0; iUnit < nUnits; iUnit++)
        {
          nextPositions[iUnit] = swarm->GetParticlePos(swarm->ctx, iUnit);
          units[iUnit] = array->GetUnitHandle(array->ctx, iUnit);
          newArray->AddUnitToArray(newArray->ctx, units[iUnit]);
        }
        swarm->Release(swarm->ctx);
        array->Release(array->ctx);
        pso->nParaSwarms--;
        i--;
        pso->pnos[pso->nPnos] = (PnoSwarmInterface_t *) PnoSwarmInterface();
        pno = pso->pnos[pso->nPnos++];
        pno->Init(pno->ctx, newArray, &pso->pnoParam, pso->nPnos - 1);
        for (iUnit = 0; iUnit < nUnits; iUnit++)
        {
          pno->SetPos(pno->ctx, iUnit, nextPositions[iUnit]);
        }
      }
      else
      {
        swarm->RemoveParticles(swarm->ctx, &allIdxPerturbed[idx][0], allIdxPerturbedSize[idx]);
        for (iUnit = 0; iUnit < allIdxPerturbedSize[idx]; iUnit++)
        {
          array->RemoveUnitFromArray(array->ctx, allIdxPerturbed[idx][iUnit]);
        }
      }
    }
    else if (allIdxToRemoveSize[i])
    {
      nUnits = swarm->GetNParticles(swarm->ctx);
      if (allIdxToRemoveSize[i] == nUnits)
      {
        swarm->Release(swarm->ctx);
        array->Release(array->ctx);
        _PpsoPno_ShiftParaSwarmsLeft(pso, i);
        pso->nParaSwarms--;
        i--;
      }
      else if ((nUnits - allIdxToRemoveSize[i]) < pso->swarmParam.minParticles)
      {
        swarm->RemoveParticles(swarm->ctx, &allIdxToRemove[i][0], allIdxToRemoveSize[i]);
        for (iUnit = 0; iUnit < allIdxToRemoveSize[i]; iUnit++)
        {
          array->RemoveUnitFromArray(array->ctx, allIdxToRemove[i][iUnit]);
        }
        nUnits = swarm->GetNParticles(swarm->ctx);
        newArray = (UnitArrayInterface_t *) UnitArrayInterface();
        for (iUnit = 0; iUnit < nUnits; iUnit++)
        {
          nextPositions[iUnit] = swarm->GetParticlePos(swarm->ctx, iUnit);
          units[iUnit] = array->GetUnitHandle(array->ctx, iUnit);
          newArray->AddUnitToArray(newArray->ctx, units[iUnit]);
        }
        swarm->Release(swarm->ctx);
        array->Release(array->ctx);
        pso->nParaSwarms--;
        i--;
        pso->pnos[pso->nPnos] = (PnoSwarmInterface_t *) PnoSwarmInterface();
        pno = pso->pnos[pso->nPnos++];
        pno->Init(pno->ctx, newArray, &pso->pnoParam, pso->nPnos - 1);
        for (iUnit = 0; iUnit < nUnits; iUnit++)
        {
          pno->SetPos(pno->ctx, iUnit, nextPositions[iUnit]);
        }
      }
      else
      {
        swarm->RemoveParticles(swarm->ctx, &allIdxToRemove[i][0], allIdxToRemoveSize[idx]);
        for (iUnit = 0; iUnit < allIdxToRemoveSize[idx]; iUnit++)
        {
          array->RemoveUnitFromArray(array->ctx, allIdxToRemove[i][iUnit]);
        }
      }
    }
  }
  //-----------------------------------
  
  // Reposition perturbed units
  //-----------------------------------
  UINT8 groups[N_UNITS_TOTAL];
  UINT8 nGroups, iGroup;
  UINT8 lengths[N_UNITS_TOTAL];
  UINT32 offset = 0;
  if (nPerturbed)
  {
    nGroups = pso->classifier->Classify(pso->classifier->ctx, idxPerturbed, nPerturbed, groups, lengths);
    pso->classifier->ResetValues(pso->classifier->ctx, idxPerturbed, nPerturbed);
    array = pso->unitArray;
    
    for (iGroup = 0; iGroup < nGroups; iGroup++)
    {
      
      if (lengths[iGroup] < pso->swarmParam.minParticles)   // Sequential PSO
      {
        for (iUnit = 0; iUnit < lengths[iGroup]; iUnit++)
        {
          newArray = array->CreateSubArray(array->ctx, &groups[offset + iUnit], 1);
          pso->seqSwarms[pso->nSeqSwarms] = (PsoSwarmInterface_t *) PsoSwarmInterface();
          swarm = pso->seqSwarms[pso->nSeqSwarms];
          pso->swarmParam.type = PSO_SWARM_TYPE_PSO_1D;
          swarm->Init(swarm->ctx, newArray, &pso->swarmParam, pso->nSeqSwarms++);
          swarm->RandomizeAllParticles(swarm->ctx);
          newArray->SetPos(newArray->ctx, 0, swarm->GetParticlePos(swarm->ctx, 0));
        }
        offset += lengths[iGroup];
      }
      else  // Parallel PSO
      {
        newArray = array->CreateSubArray(array->ctx, &groups[offset], lengths[iGroup]);
        offset += lengths[iGroup];
        pso->paraSwarms[pso->nParaSwarms] = (PsoSwarmInterface_t *) PsoSwarmInterface();
        swarm = pso->paraSwarms[pso->nParaSwarms];
        pso->swarmParam.type = PSO_SWARM_TYPE_PARALLEL_PSO_MULTI_SWARM;
        swarm->Init(swarm->ctx, newArray, &pso->swarmParam, pso->nParaSwarms++);
        swarm->RandomizeAllParticles(swarm->ctx);
        for (iUnit = 0; iUnit < lengths[iGroup]; iUnit++)
        {
          newArray->SetPos(newArray->ctx, iUnit, swarm->GetParticlePos(swarm->ctx, iUnit));
        }
      }
    }
  }
  //-----------------------------------
  
  //----------------------------------------------------------------------------
  // </editor-fold>

  return 0;
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
  memmove(&pso->pnos[idxToShift], &pso->pnos[idxToShift + 1], offset * sizeof(PnoSwarmInterface_t *));
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