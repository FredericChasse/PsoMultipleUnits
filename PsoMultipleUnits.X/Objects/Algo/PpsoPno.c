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
#include "Protocol.h"       // For debugging
#include "StateMachine.h"   // For debugging


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
  ProtocolPpsoPnoDataPayload_t classifierGroups;
} PpsoPno_t;

#define SEND_DEBUG_DATA_TO_UART
//#define SEND_ALGO_STEP_TO_UART

typedef enum
{
  DBG_PARA_SWARM
 ,DBG_SEQ_SWARM
 ,DBG_PNO
} DbgDataType_t;


// Private prototypes
//==============================================================================

INT8  _PpsoPno_Init               (PpsoPno_t *pso, UnitArrayInterface_t *unitArray);
INT8  _PpsoPno_Run                (PpsoPno_t *pso);
float _PpsoPno_GetTimeElapsed     (PpsoPno_t *pso);
void  _PpsoPno_Release            (PpsoPno_t *pso);
void  _PpsoPno_GetDebugData       (PpsoPno_t *pso, ProtocolPpsoPnoDataPayload_t *ret);

void _PpsoPno_ShiftParaSwarmsLeft (PpsoPno_t *pso, UINT8 idxToShift);
void _PpsoPno_ShiftSeqSwarmsLeft  (PpsoPno_t *pso, UINT8 idxToShift);
void _PpsoPno_ShiftPnosLeft       (PpsoPno_t *pso, UINT8 idxToShift);
static int _CompareFunc           (const void *p1, const void *p2);

void _SendAlgoStatus              (PpsoPno_t *pso);
void _SendCreatingSwarm           (PpsoPno_t *pso, DbgDataType_t type, UINT8 idx);
void _SendReleasingSwarm          (PpsoPno_t *pso, DbgDataType_t type, UINT8 idx);
void _SendAlgoStep                (UINT8 step);
void _SendUnitsPos                (PpsoPno_t *pso);
void _SendClassifierResults       (PpsoPno_t *pso);
void _SendPerturbedUnits          (PpsoPno_t *pso, UINT8 *idx, UINT8 nUnits);
void _SendUnitsToRemove           (PpsoPno_t *pso, UINT8 swarmId, UINT8 *units, UINT8 nUnits);
void _SendSwarmPerturbedUnits     (PpsoPno_t *pso, DbgDataType_t type, UINT8 swarmId, UINT8 *units, UINT8 nUnits);


// Private variables
//==============================================================================

PpsoPno_t _ppsoPno = 
{
  .unitArray        = NULL
 ,.sampleTime       = SAMPLING_TIME_FLOAT
 ,.timeElapsed      = 0
 ,.iteration        = 0
 ,.nParaSwarms      = 0
 ,.nSeqSwarms       = 0
 ,.nPnos            = 0
 ,.paraSwarms       = {NULL}
 ,.seqSwarms        = {NULL}
 ,.pnos             = {NULL}
 ,.classifier       = NULL
 ,.swarmParam       = {NULL}
 ,.pnoParam         = {NULL}
 ,.classifierGroups = {0, 0, {0}, {0}}
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

void _PpsoPno_GetDebugData (PpsoPno_t *pso, ProtocolPpsoPnoDataPayload_t *ret)
{
  memcpy(ret, &pso->classifierGroups, sizeof(ProtocolPpsoPnoDataPayload_t));
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
  UINT8 minPosIdx, maxPosIdx;
  pso->unitArray    = unitArray;
  pso->nParaSwarms  = 1;
  pso->nSeqSwarms   = 0;
  pso->nPnos        = 0;
  pso->timeElapsed  = 0;
  pso->sampleTime   = SAMPLING_TIME_FLOAT;
  
  pso->classifier = (ClassifierInterface_t *) ClassifierInterface();
  __assert(pso->classifier);
  pso->classifier->Init(pso->classifier->ctx, unitArray, 3*POT_STEP_VALUE);
  
  pso->unitArray->GetPosLimits(pso->unitArray->ctx, &minPos, &maxPos);
  minPosIdx = ComputePotValueFloat2Dec(minPos);
  maxPosIdx = ComputePotValueFloat2Dec(maxPos);
  
  const PsoSwarmParam_t _swarmParam = 
  {    
//    .c1                     = 0.8
//   ,.c2                     = 1.5
//   ,.omega                  = 0.4
    .c1                     = 1
   ,.c2                     = 2
   ,.omega                  = 0.3
    
   ,.posMin                 = minPos
   ,.posMax                 = maxPos
   ,.minParticles           = 3
   ,.perturbAmp             = 1*POT_STEP_VALUE
//   ,.perturbAmp             = 2*POT_STEP_VALUE
   ,.sentinelMargin         = 0.05
   ,.type                   = PSO_SWARM_TYPE_PARALLEL_PSO_MULTI_SWARM
   ,.nSamplesForSteadyState = 5
   ,.steadyStateOscAmp      = 0.05
   ,.iteration              = 0
   ,.currentParticle        = 0
  };
  
  const PnoSwarmParam_t _pnoParam = 
  {    
//    .delta_int      = 5
//   ,.delta          = 5 * POT_STEP_VALUE
//   ,.uinit_int      = POT_MAX_INDEX/2
//   ,.uinit          = potRealValues[POT_MAX_INDEX/2]
//   ,.umax_int       = POT_MAX_INDEX
//   ,.umax           = potRealValues[POT_MAX_INDEX]
//   ,.umin_int       = POT_MIN_INDEX
//   ,.umin           = potRealValues[POT_MIN_INDEX]
//   ,.nSamplesForSs  = 6
//   ,.oscAmp         = 2
//   ,.perturbOsc     = 0.05

    .delta_int      = 1
   ,.delta          = 1 * POT_STEP_VALUE
   ,.uinit_int      = maxPosIdx
   ,.uinit          = potRealValues[maxPosIdx]
   ,.umax_int       = maxPosIdx
   ,.umax           = potRealValues[maxPosIdx]
   ,.umin_int       = minPosIdx
   ,.umin           = potRealValues[minPosIdx]
   ,.nSamplesForSs  = 12
   ,.oscAmp         = 2
   ,.perturbOsc     = 0.05
  };
  
  memcpy(&pso->swarmParam, &_swarmParam, sizeof(PsoSwarmParam_t));
  memcpy(&pso->pnoParam  , &_pnoParam  , sizeof(PnoSwarmParam_t));
  
  swarmArray = (UnitArrayInterface_t *) UnitArrayInterface();
  __assert(swarmArray);
  swarmArray->Init(swarmArray->ctx, 1);
  nUnits = unitArray->GetNUnits(unitArray->ctx);
  for (i = 0; i < nUnits; i++)
  {
    swarmArray->AddUnitToArray(swarmArray->ctx, unitArray->GetUnitHandle(unitArray->ctx, i));
  }
  
  pso->paraSwarms[0] = (PsoSwarmInterface_t *) PsoSwarmInterface();
  __assert(pso->paraSwarms[0]);
  pso->paraSwarms[0]->Init(pso->paraSwarms[0]->ctx, swarmArray, &pso->swarmParam, 0);
  
  for (i = 0; i < nUnits; i++)
  {
    unitArray->SetPos(unitArray->ctx, i, pso->paraSwarms[0]->GetParticlePos(pso->paraSwarms[0]->ctx, i));
  }
  
  pso->classifierGroups.nGroups = 0;
}


INT8 _PpsoPno_Run (PpsoPno_t *pso)
{
  // <editor-fold defaultstate="collapsed" desc="Init">
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
  float nextPositions[N_UNITS_TOTAL];
  UINT32 nPerturbed;
  UINT32 iAlgo = 0;
  UINT8 nIdxToRemove;
  UINT8 idxToRemove[N_UNITS_TOTAL];
  UINT8 allIdxPerturbed[N_UNITS_TOTAL][N_UNITS_TOTAL] = {0};
  UINT8 allIdxPerturbedSize[N_UNITS_TOTAL] = {0};
  UINT8 allIdxToRemove[N_UNITS_TOTAL][N_UNITS_TOTAL] = {0};
  UINT8 allIdxToRemoveSize[N_UNITS_TOTAL] = {0};
  
  pso->classifierGroups.nGroups = 0;
  
#ifdef SEND_DEBUG_DATA_TO_UART
  _SendAlgoStatus(pso);
  _SendUnitsPos(pso);
#endif
  
  // </editor-fold>
  
  // <editor-fold defaultstate="collapsed" desc="Update algo values and classifier">
  pso->iteration++;
  pso->timeElapsed += pso->sampleTime;
  
  pso->classifier->UpdateValues(pso->classifier->ctx);
  // </editor-fold>
  
  // <editor-fold defaultstate="collapsed" desc="Parallel PSO">
  // Parallel PSO
  //----------------------------------------------------------------------------
  for (i = 0; i < pso->nParaSwarms; i++)
  {
    swarm = pso->paraSwarms[i];
    __assert(swarm);
    
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
#ifdef SEND_DEBUG_DATA_TO_UART
  _SendUnitsToRemove(pso, i, idxToRemove, nIdxToRemove);
#endif
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
#ifdef SEND_DEBUG_DATA_TO_UART
  _SendSwarmPerturbedUnits(pso, DBG_PARA_SWARM, i, idxPerturbed, nPerturbed);
#endif
      memcpy(&allIdxPerturbed[iAlgo][0], idxPerturbed, nPerturbed);
      allIdxPerturbedSize[iAlgo] = nPerturbed;
    }
    else
    {
      allIdxPerturbedSize[iAlgo] = 0;
    }
    //-----------------------------------
    
    array = swarm->GetUnitArray(swarm->ctx);
    __assert(array);
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
    __assert(pno);
    
    pno->IncIteration(pno->ctx);
    
    nPerturbed = pno->ComputeAllPos(pno->ctx, nextPositions, idxPerturbed);
    
    nUnits = pno->GetNInstances(pno->ctx);
    
    array = pno->GetArray(pno->ctx);
    __assert(array);
    
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
#ifdef SEND_DEBUG_DATA_TO_UART
  _SendSwarmPerturbedUnits(pso, DBG_PNO, i, idxPerturbed, nPerturbed);
#endif
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
    __assert(swarm);
    nParticles = swarm->GetNParticles(swarm->ctx);
    array = swarm->GetUnitArray(swarm->ctx);
    __assert(array);
    
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
#ifdef SEND_DEBUG_DATA_TO_UART
  _SendSwarmPerturbedUnits(pso, DBG_SEQ_SWARM, i, idxPerturbed, nPerturbed);
#endif
        memcpy(&allIdxPerturbed[iAlgo][0], idxPerturbed, nPerturbed);
        allIdxPerturbedSize[iAlgo] = nPerturbed;
      }
      //-----------------------------------
      
      // Assess steady state
      //-----------------------------------
      if (oInSteadyState)
      {
        nParticles = swarm->GetNParticles(swarm->ctx);
        for (iUnit = 0; iUnit < nParticles; iUnit++)
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
  
  // <editor-fold defaultstate="collapsed" desc="Set the correct algo indexes">
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
  // </editor-fold>
  
  
  // <editor-fold defaultstate="collapsed" desc="Assess perturbed P&O instances">
  // Assess perturbed P&O instances
  //-----------------------------------
  for (i = 0; i < pso->nPnos; i++)
  {
    idx = algoIdxPerturbed[iAlgo++];
    pno = pso->pnos[i];
    __assert(pno);
    if (allIdxPerturbedSize[idx])
    {
      array = pno->GetArray(pno->ctx);
      __assert(array);
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
        qsort( (void *) &allIdxPerturbed[idx][0], (size_t) allIdxPerturbedSize[idx], sizeof(UINT8), &_CompareFunc);
        pno->RemoveInstances(pno->ctx, &allIdxPerturbed[idx][0], allIdxPerturbedSize[idx]);
        
        for (iUnit = 0; iUnit < allIdxPerturbedSize[idx]; iUnit++)
        {
          array->RemoveUnitFromArray(array->ctx, allIdxPerturbed[idx][iUnit]);
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
  // </editor-fold>
  
 
  // <editor-fold defaultstate="collapsed" desc="Assess perturbed Sequential PSO">
  // Assess perturbed Sequential PSO
  //-----------------------------------
  for (i = 0; i < pso->nSeqSwarms; i++)
  {
    idx = algoIdxPerturbed[iAlgo++];
    swarm = pso->seqSwarms[i];
    __assert(swarm);
    if (allIdxPerturbedSize[idx])
    {
      array = swarm->GetUnitArray(swarm->ctx);
      __assert(array);
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
  // </editor-fold>
  
  
  // <editor-fold defaultstate="collapsed" desc="Assess perturbed Parallel PSO">
  // Assess perturbed Parallel PSO
  //-----------------------------------
  void *units[N_UNITS_TOTAL];
  UnitArrayInterface_t *newArray;
  UINT8 tmpUnitsToRemove[N_UNITS_TOTAL], nUnitsToRemove = 0;
  for (i = 0; i < pso->nParaSwarms; i++)
  {
    swarm = pso->paraSwarms[i];
    __assert(swarm);
    array = swarm->GetUnitArray(swarm->ctx);
    __assert(array);
    
    idx = algoIdxPerturbed[iAlgo++];
    
    if (allIdxToRemoveSize[idx])
    {
      qsort( (void *) &allIdxToRemove[idx][0], (size_t) allIdxToRemoveSize[idx], sizeof(UINT8), &_CompareFunc);
      for (iUnit = 0; iUnit < allIdxToRemoveSize[idx]; iUnit++)
      {
        nextPositions[iUnit] = swarm->GetParticlePos(swarm->ctx, allIdxToRemove[idx][iUnit]);
        units[iUnit] = array->GetUnitHandle(array->ctx, allIdxToRemove[idx][iUnit]);
      }
      
      newArray = (UnitArrayInterface_t *) UnitArrayInterface();
      __assert(newArray);
      
      for (iUnit = 0; iUnit < allIdxToRemoveSize[idx]; iUnit++)
      {
        newArray->AddUnitToArray(newArray->ctx, units[iUnit]);
      }
      
      pso->pnos[pso->nPnos] = (PnoSwarmInterface_t *) PnoSwarmInterface();
      __assert(pso->pnos[pso->nPnos]);
      pno = pso->pnos[pso->nPnos];
      pno->Init(pno->ctx, newArray, &pso->pnoParam, pso->nPnos++);
      nUnits = pno->GetNInstances(pno->ctx);
      for (iUnit = 0; iUnit < nUnits; iUnit++)
      {
        pno->SetPos(pno->ctx, iUnit, nextPositions[iUnit]);
        newArray->SetPos(newArray->ctx, iUnit, nextPositions[iUnit]);
      }
    }
    
    if (allIdxPerturbedSize[idx])
    {
      for (iUnit = 0; iUnit < allIdxPerturbedSize[idx]; iUnit++)
      {
        idxPerturbed[nPerturbed++] = array->GetUnitId(array->ctx, allIdxPerturbed[idx][iUnit]);
      }
      
      if (allIdxToRemoveSize[idx])
      {
        memcpy(&allIdxPerturbed[idx][allIdxPerturbedSize[idx]], &allIdxToRemove[idx][0], allIdxToRemoveSize[idx]);
        allIdxPerturbedSize[idx] += allIdxToRemoveSize[idx];
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
        qsort( (void *) &allIdxPerturbed[idx][0], (size_t) allIdxPerturbedSize[idx], sizeof(UINT8), &_CompareFunc);
        swarm->RemoveParticles(swarm->ctx, &allIdxPerturbed[idx][0], allIdxPerturbedSize[idx]);
        for (iUnit = 0; iUnit < allIdxPerturbedSize[idx]; iUnit++)
        {
          array->RemoveUnitFromArray(array->ctx, allIdxPerturbed[idx][iUnit]);
        }
        nUnits = swarm->GetNParticles(swarm->ctx);
        newArray = (UnitArrayInterface_t *) UnitArrayInterface();
      __assert(newArray);
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
      __assert(pso->pnos[pso->nPnos]);
        pno = pso->pnos[pso->nPnos];
        pno->Init(pno->ctx, newArray, &pso->pnoParam, pso->nPnos++);
        for (iUnit = 0; iUnit < nUnits; iUnit++)
        {
          pno->SetPos(pno->ctx, iUnit, nextPositions[iUnit]);
        }
      }
      else
      {
        qsort( (void *) &allIdxPerturbed[idx][0], (size_t) allIdxPerturbedSize[idx], sizeof(UINT8), &_CompareFunc);
        swarm->RemoveParticles(swarm->ctx, &allIdxPerturbed[idx][0], allIdxPerturbedSize[idx]);
        for (iUnit = 0; iUnit < allIdxPerturbedSize[idx]; iUnit++)
        {
          array->RemoveUnitFromArray(array->ctx, allIdxPerturbed[idx][iUnit]);
        }
      }
    }
    else if (allIdxToRemoveSize[idx])
    {
      nUnits = swarm->GetNParticles(swarm->ctx);
      
      if (allIdxToRemoveSize[idx] == nUnits)
      {
        swarm->Release(swarm->ctx);
        array->Release(array->ctx);
        _PpsoPno_ShiftParaSwarmsLeft(pso, i);
        pso->nParaSwarms--;
        i--;
      }
      else if ((nUnits - allIdxToRemoveSize[idx]) < pso->swarmParam.minParticles)
      {
        swarm->RemoveParticles(swarm->ctx, &allIdxToRemove[idx][0], allIdxToRemoveSize[idx]);
        for (iUnit = 0; iUnit < allIdxToRemoveSize[idx]; iUnit++)
        {
          array->RemoveUnitFromArray(array->ctx, allIdxToRemove[idx][iUnit]);
        }
        nUnits = swarm->GetNParticles(swarm->ctx);
        newArray = (UnitArrayInterface_t *) UnitArrayInterface();
        __assert(newArray);
        for (iUnit = 0; iUnit < nUnits; iUnit++)
        {
          nextPositions[iUnit] = swarm->GetParticlePos(swarm->ctx, iUnit);
          units[iUnit] = array->GetUnitHandle(array->ctx, iUnit);
          newArray->AddUnitToArray(newArray->ctx, units[iUnit]);
        }
        swarm->Release(swarm->ctx);
        array->Release(array->ctx);
        _PpsoPno_ShiftParaSwarmsLeft(pso, i);
        pso->nParaSwarms--;
        i--;
        pso->pnos[pso->nPnos] = (PnoSwarmInterface_t *) PnoSwarmInterface();
        __assert(pso->pnos[pso->nPnos]);
        pno = pso->pnos[pso->nPnos];
        pno->Init(pno->ctx, newArray, &pso->pnoParam, pso->nPnos++);
        for (iUnit = 0; iUnit < nUnits; iUnit++)
        {
          pno->SetPos(pno->ctx, iUnit, nextPositions[iUnit]);
        }
      }
      else
      {
        swarm->RemoveParticles(swarm->ctx, &allIdxToRemove[idx][0], allIdxToRemoveSize[idx]);
        for (iUnit = 0; iUnit < allIdxToRemoveSize[idx]; iUnit++)
        {
          array->RemoveUnitFromArray(array->ctx, allIdxToRemove[idx][iUnit]);
        }
      }
    }
  }
  if (pso->nParaSwarms)
  {
    for (i = 0; i < pso->nParaSwarms; i++)
    {
      pso->paraSwarms[i]->SetId(pso->paraSwarms[i]->ctx, i);
    }
  }
  //-----------------------------------
  // </editor-fold>
  
  
  // <editor-fold defaultstate="collapsed" desc="Reposition perturbed units">
  // Reposition perturbed units
  //-----------------------------------
  UINT8 groups[N_UNITS_TOTAL];
  UINT8 nGroups, iGroup;
  UINT8 lengths[N_UNITS_TOTAL];
  UINT32 offset = 0;
  if (nPerturbed)
  {
#ifdef SEND_DEBUG_DATA_TO_UART
  _SendPerturbedUnits(pso, idxPerturbed, nPerturbed);
#endif
    nGroups = pso->classifier->Classify(pso->classifier->ctx, idxPerturbed, nPerturbed, groups, lengths);
    if (nGroups)
    {
      pso->classifierGroups.iteration = pso->iteration;
      pso->classifierGroups.nGroups = nGroups;
      memcpy(pso->classifierGroups.groups, groups, N_UNITS_TOTAL);
      memcpy(pso->classifierGroups.groupLengths, lengths, N_UNITS_TOTAL);
#ifdef SEND_DEBUG_DATA_TO_UART
  _SendClassifierResults(pso);
#endif
    }
    pso->classifier->ResetValues(pso->classifier->ctx, idxPerturbed, nPerturbed);
    array = pso->unitArray;
    
    for (iGroup = 0; iGroup < nGroups; iGroup++)
    {
      if (lengths[iGroup] < pso->swarmParam.minParticles)   // Sequential PSO
      {
        for (iUnit = 0; iUnit < lengths[iGroup]; iUnit++)
        {
          newArray = array->CreateSubArray(array->ctx, &groups[offset + iUnit], 1);
          __assert(newArray);
          pso->seqSwarms[pso->nSeqSwarms] = (PsoSwarmInterface_t *) PsoSwarmInterface();
          __assert(pso->seqSwarms[pso->nSeqSwarms]);
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
        __assert(newArray);
        pso->paraSwarms[pso->nParaSwarms] = (PsoSwarmInterface_t *) PsoSwarmInterface();
        __assert(pso->paraSwarms[pso->nParaSwarms]);
        swarm = pso->paraSwarms[pso->nParaSwarms];
        pso->swarmParam.type = PSO_SWARM_TYPE_PARALLEL_PSO_MULTI_SWARM;
        swarm->Init(swarm->ctx, newArray, &pso->swarmParam, pso->nParaSwarms++);
        swarm->RandomizeAllParticles(swarm->ctx);
        for (iUnit = 0; iUnit < lengths[iGroup]; iUnit++)
        {
          newArray->SetPos(newArray->ctx, iUnit, swarm->GetParticlePos(swarm->ctx, iUnit));
        }
        offset += lengths[iGroup];
      }
    }
  }
  //-----------------------------------
  // </editor-fold>
  
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



const UINT8  strReleasing[] = "Releasing "
            ,strReleasingLen = 10
            ,strCreating[] = "Creating "
            ,strCreatingLen = 9
            ,strEnter[] = "\n\r"
            ,strEnterLen = 2
            ,strNSeqSwarms[] = "N seq swarms = "
            ,strNSeqSwarmsLen = 15
            ,strNPnos[] = "N P&O = "
            ,strNPnosLen = 8
            ,strNParaSwarms[] = "N para swarms = "
            ,strNParaSwarmsLen = 16
            ,strSpace = '0'
            ,strSpaceLen = 1
            ,strNumbers[N_UNITS_TOTAL] = {'0','1','2','3','4','5','6','7','8','9','A','B'}
            ,strSwarmCreateError[] = "Creating overflow "
            ,strSwarmCreateErrorLen = 18
            ,strSwarmReleaseError[] = "Releasing void "
            ,strSwarmReleaseErrorLen = 15
            ,strSeqSwarm[] = "seq swarm"
            ,strSeqSwarmLen = 9
            ,strParaSwarm[] = "para swarm"
            ,strParaSwarmLen = 10
            ,strPnoSwarm[] = "pno swarm"
            ,strPnoSwarmLen = 9
            ,strInvalidParameter[] = "Invalid parameter!"
            ,strInvalidParameterLen = 18
            ;
void _SendAlgoStatus (PpsoPno_t *pso)
{
  sUartLineBuffer_t buf = {0};
  
  if (pso->nParaSwarms > N_UNITS_TOTAL || pso->nPnos > N_UNITS_TOTAL || pso->nSeqSwarms > N_UNITS_TOTAL)
  {
    const UINT8 strErrorPso[] = "Wrong PSO parameters\n\r", strErrorPsoLen = 22;
    memcpy(&buf.buffer[0], strErrorPso, strErrorPsoLen);
    buf.length = strErrorPsoLen;
    while(Uart.PutTxFifoBuffer(U_DBG, &buf) < 0);
    return;
  }
  
  const UINT8 strIteration[] = "i = ", strIterationLen = 4;
  memcpy(&buf.buffer[0], strIteration, strIterationLen);
  buf.length = strIterationLen;
  buf.length += sprintf(&buf.buffer[buf.length], "%i", pso->iteration);
  while (Uart.PutTxFifoBuffer(U_DBG, &buf) < 0);
  
  memcpy(&buf.buffer[0], strEnter, strEnterLen);
  buf.length = strEnterLen;
  
  memcpy(&buf.buffer[buf.length], strNParaSwarms, strNParaSwarmsLen);
  buf.length += strNParaSwarmsLen;
  buf.buffer[buf.length] = strNumbers[pso->nParaSwarms];
  buf.length++;
  memcpy(&buf.buffer[buf.length], strEnter, strEnterLen);
  buf.length += strEnterLen;
  
  memcpy(&buf.buffer[buf.length], strNSeqSwarms, strNSeqSwarmsLen);
  buf.length += strNSeqSwarmsLen;
  buf.buffer[buf.length] = strNumbers[pso->nSeqSwarms];
  buf.length++;
  memcpy(&buf.buffer[buf.length], strEnter, strEnterLen);
  buf.length += strEnterLen;
  
  memcpy(&buf.buffer[buf.length], strNPnos, strNPnosLen);
  buf.length += strNPnosLen;
  buf.buffer[buf.length] = strNumbers[pso->nPnos];
  buf.length++;
  memcpy(&buf.buffer[buf.length], strEnter, strEnterLen);
  buf.length += strEnterLen;
  
  memcpy(&buf.buffer[buf.length], strEnter, strEnterLen);
  buf.length += strEnterLen;
  
  while (Uart.PutTxFifoBuffer(U_DBG, &buf) < 0);
  while (!Uart.Var.uartTxFifo[U_DBG_IDX].bufEmpty);
}


void _SendCreatingSwarm (PpsoPno_t *pso, DbgDataType_t type, UINT8 idx)
{
  sUartLineBuffer_t buf = {0};
  switch (type)
  {
    case DBG_PARA_SWARM:
      if (idx >= N_UNITS_TOTAL || pso->nParaSwarms >= (N_UNITS_TOTAL - 1))
      {
        memcpy(&buf.buffer[0], strSwarmCreateError, strSwarmCreateErrorLen);
        buf.length = strSwarmCreateErrorLen;
        memcpy(&buf.buffer[buf.length], strParaSwarm, strParaSwarmLen);
        buf.length += strParaSwarmLen;
        memcpy(&buf.buffer[buf.length], strEnter, strEnterLen);
        buf.length += strEnterLen;
      }
      else
      {
        memcpy(&buf.buffer[0], strCreating, strCreatingLen);
        buf.length = strCreatingLen;
        memcpy(&buf.buffer[buf.length], strParaSwarm, strParaSwarmLen);
        buf.length += strParaSwarmLen;
        buf.buffer[buf.length] = strSpace;
        buf.length++;
        buf.buffer[buf.length] = strNumbers[idx];
        buf.length++;
        memcpy(&buf.buffer[buf.length], strEnter, strEnterLen);
        buf.length += strEnterLen;
      }
      break;
      
    case DBG_SEQ_SWARM:
      if (idx >= N_UNITS_TOTAL || pso->nSeqSwarms >= (N_UNITS_TOTAL - 1))
      {
        memcpy(&buf.buffer[0], strSwarmCreateError, strSwarmCreateErrorLen);
        buf.length = strSwarmCreateErrorLen;
        memcpy(&buf.buffer[buf.length], strSeqSwarm, strSeqSwarmLen);
        buf.length += strSeqSwarmLen;
        memcpy(&buf.buffer[buf.length], strEnter, strEnterLen);
        buf.length += strEnterLen;
      }
      else
      {
        memcpy(&buf.buffer[0], strCreating, strCreatingLen);
        buf.length = strCreatingLen;
        memcpy(&buf.buffer[buf.length], strSeqSwarm, strSeqSwarmLen);
        buf.length += strSeqSwarmLen;
        buf.buffer[buf.length] = strSpace;
        buf.length++;
        buf.buffer[buf.length] = strNumbers[idx];
        buf.length++;
        memcpy(&buf.buffer[buf.length], strEnter, strEnterLen);
        buf.length += strEnterLen;
      }
      break;
      
    case DBG_PNO:
      if (idx >= N_UNITS_TOTAL || pso->nPnos >= (N_UNITS_TOTAL - 1))
      {
        memcpy(&buf.buffer[0], strSwarmCreateError, strSwarmCreateErrorLen);
        buf.length = strSwarmCreateErrorLen;
        memcpy(&buf.buffer[buf.length], strPnoSwarm, strPnoSwarmLen);
        buf.length += strPnoSwarmLen;
        memcpy(&buf.buffer[buf.length], strEnter, strEnterLen);
        buf.length += strEnterLen;
      }
      else
      {
        memcpy(&buf.buffer[0], strCreating, strCreatingLen);
        buf.length = strCreatingLen;
        memcpy(&buf.buffer[buf.length], strPnoSwarm, strPnoSwarmLen);
        buf.length += strPnoSwarmLen;
        buf.buffer[buf.length] = strSpace;
        buf.length++;
        buf.buffer[buf.length] = strNumbers[idx];
        buf.length++;
        memcpy(&buf.buffer[buf.length], strEnter, strEnterLen);
        buf.length += strEnterLen;
      }
      break;
      
    default:
      memcpy(&buf.buffer[0], strInvalidParameter, strInvalidParameterLen);
      buf.length = strInvalidParameterLen;
      break;
  }
  
  memcpy(&buf.buffer[buf.length], strEnter, strEnterLen);
  buf.length += strEnterLen;
  
  while (Uart.PutTxFifoBuffer(U_DBG, &buf) < 0);
  while (!Uart.Var.uartTxFifo[U_DBG_IDX].bufEmpty);
}


void _SendReleasingSwarm (PpsoPno_t *pso, DbgDataType_t type, UINT8 idx)
{
  sUartLineBuffer_t buf = {0};
  switch (type)
  {
    case DBG_PARA_SWARM:
      if (pso->paraSwarms[idx] == 0 || pso->nParaSwarms <= idx)
      {
        memcpy(&buf.buffer[0], strSwarmReleaseError, strSwarmReleaseErrorLen);
        buf.length = strSwarmReleaseErrorLen;
        memcpy(&buf.buffer[buf.length], strParaSwarm, strParaSwarmLen);
        buf.length += strParaSwarmLen;
        memcpy(&buf.buffer[buf.length], strEnter, strEnterLen);
        buf.length += strEnterLen;
      }
      else
      {
        memcpy(&buf.buffer[0], strReleasing, strReleasingLen);
        buf.length = strReleasingLen;
        memcpy(&buf.buffer[buf.length], strParaSwarm, strParaSwarmLen);
        buf.length += strParaSwarmLen;
        buf.buffer[buf.length] = strSpace;
        buf.length++;
        buf.buffer[buf.length] = strNumbers[idx];
        buf.length++;
        memcpy(&buf.buffer[buf.length], strEnter, strEnterLen);
        buf.length += strEnterLen;
      }
      break;
      
    case DBG_SEQ_SWARM:
      if (pso->seqSwarms[idx] == 0 || pso->nSeqSwarms <= idx)
      {
        memcpy(&buf.buffer[0], strSwarmReleaseError, strSwarmReleaseErrorLen);
        buf.length = strSwarmReleaseErrorLen;
        memcpy(&buf.buffer[buf.length], strSeqSwarm, strSeqSwarmLen);
        buf.length += strSeqSwarmLen;
        memcpy(&buf.buffer[buf.length], strEnter, strEnterLen);
        buf.length += strEnterLen;
      }
      else
      {
        memcpy(&buf.buffer[0], strReleasing, strReleasingLen);
        buf.length = strReleasingLen;
        memcpy(&buf.buffer[buf.length], strSeqSwarm, strSeqSwarmLen);
        buf.length += strSeqSwarmLen;
        buf.buffer[buf.length] = strSpace;
        buf.length++;
        buf.buffer[buf.length] = strNumbers[idx];
        buf.length++;
        memcpy(&buf.buffer[buf.length], strEnter, strEnterLen);
        buf.length += strEnterLen;
      }
      break;
      
    case DBG_PNO:
      if (pso->pnos[idx] == 0 || pso->nPnos <= idx)
      {
        memcpy(&buf.buffer[0], strSwarmReleaseError, strSwarmReleaseErrorLen);
        buf.length = strSwarmReleaseErrorLen;
        memcpy(&buf.buffer[buf.length], strPnoSwarm, strPnoSwarmLen);
        buf.length += strPnoSwarmLen;
        memcpy(&buf.buffer[buf.length], strEnter, strEnterLen);
        buf.length += strEnterLen;
      }
      else
      {
        memcpy(&buf.buffer[0], strReleasing, strReleasingLen);
        buf.length = strReleasingLen;
        memcpy(&buf.buffer[buf.length], strPnoSwarm, strPnoSwarmLen);
        buf.length += strPnoSwarmLen;
        buf.buffer[buf.length] = strSpace;
        buf.length++;
        buf.buffer[buf.length] = strNumbers[idx];
        buf.length++;
        memcpy(&buf.buffer[buf.length], strEnter, strEnterLen);
        buf.length += strEnterLen;
      }
      break;
      
    default:
      memcpy(&buf.buffer[0], strInvalidParameter, strInvalidParameterLen);
      buf.length = strInvalidParameterLen;
      break;
  }
  
  memcpy(&buf.buffer[buf.length], strEnter, strEnterLen);
  buf.length += strEnterLen;
  
  while (Uart.PutTxFifoBuffer(U_DBG, &buf) < 0);
  while (!Uart.Var.uartTxFifo[U_DBG_IDX].bufEmpty);
}


void _SendAlgoStep (UINT8 step)
{
  sUartLineBuffer_t buf;
  
  buf.length = sprintf(buf.buffer, "Step %i\n\r", step);
  while (Uart.PutTxFifoBuffer(U_DBG, &buf) < 0);
  while (!Uart.Var.uartTxFifo[U_DBG_IDX].bufEmpty);
  Timer.DelayMs(1);
}


void _SendUnitsPos (PpsoPno_t *pso)
{
  UINT8 i, j, nUnits, id;
  PsoSwarmInterface_t *swarm;
  PnoSwarmInterface_t *pno;
  sUartLineBuffer_t buf = {0};
  UnitArrayInterface_t *array;
  
  for (i = 0; i < pso->nParaSwarms; i++)
  {
    memcpy(&buf.buffer[buf.length], strParaSwarm, strParaSwarmLen);
    buf.length += strParaSwarmLen;
    buf.buffer[buf.length++] = strNumbers[i];
    buf.buffer[buf.length++] = ':';
    swarm = pso->paraSwarms[i];
    array = swarm->GetUnitArray(swarm->ctx);
    nUnits = array->GetNUnits(array->ctx);
    for (j = 0; j < nUnits; j++)
    {
      buf.buffer[buf.length++] = ' ';
      id = array->GetUnitId(array->ctx, j);
      buf.buffer[buf.length++] = strNumbers[id];
    }
    buf.buffer[buf.length++] = '\n';
    buf.buffer[buf.length++] = '\r';
  }
  if (buf.length)
  {
    while(Uart.PutTxFifoBuffer(U_DBG, &buf) < 0);
  }
  
  buf.length = 0;
  for (i = 0; i < pso->nSeqSwarms; i++)
  {
    memcpy(&buf.buffer[buf.length], strSeqSwarm, strSeqSwarmLen);
    buf.length += strSeqSwarmLen;
    buf.buffer[buf.length++] = strNumbers[i];
    buf.buffer[buf.length++] = ':';
    swarm = pso->seqSwarms[i];
    array = swarm->GetUnitArray(swarm->ctx);
    nUnits = array->GetNUnits(array->ctx);
    for (j = 0; j < nUnits; j++)
    {
      buf.buffer[buf.length++] = ' ';
      id = array->GetUnitId(array->ctx, j);
      buf.buffer[buf.length++] = strNumbers[id];
    }
    buf.buffer[buf.length++] = '\n';
    buf.buffer[buf.length++] = '\r';
  }
  if (buf.length)
  {
    while(Uart.PutTxFifoBuffer(U_DBG, &buf) < 0);
  }
  
  buf.length = 0;
  for (i = 0; i < pso->nPnos; i++)
  {
    memcpy(&buf.buffer[buf.length], strPnoSwarm, strPnoSwarmLen);
    buf.length += strPnoSwarmLen;
    buf.buffer[buf.length++] = strNumbers[i];
    buf.buffer[buf.length++] = ':';
    pno = pso->pnos[i];
    array = pno->GetArray(pno->ctx);
    nUnits = array->GetNUnits(array->ctx);
    for (j = 0; j < nUnits; j++)
    {
      buf.buffer[buf.length++] = ' ';
      id = array->GetUnitId(array->ctx, j);
      buf.buffer[buf.length++] = strNumbers[id];
    }
    buf.buffer[buf.length++] = '\n';
    buf.buffer[buf.length++] = '\r';
  }
  buf.buffer[buf.length++] = '\n';
  buf.buffer[buf.length++] = '\r';
  while(Uart.PutTxFifoBuffer(U_DBG, &buf) < 0);
  while(!Uart.Var.uartTxFifo[U_DBG_IDX].bufEmpty);
}


void _SendPerturbedUnits (PpsoPno_t *pso, UINT8 *idx, UINT8 nUnits)
{
  sUartLineBuffer_t buf = {0};
  UnitArrayInterface_t *array = pso->unitArray;
  UINT8 i;
  const UINT8 strPerturbedUnits[] = "Perturbed units = ", strPerturbedUnitsLen = 18;
  
  memcpy(&buf.buffer[buf.length], strPerturbedUnits, strPerturbedUnitsLen);
  buf.length += strPerturbedUnitsLen;
  
  for (i = 0; i < nUnits; i++)
  {
    buf.buffer[buf.length++] = strNumbers[array->GetUnitId(array->ctx, i)];
    buf.buffer[buf.length++] = ' ';
  }
  buf.buffer[buf.length++] = '\n';
  buf.buffer[buf.length++] = '\r';
  
  while (Uart.PutTxFifoBuffer(U_DBG, &buf) < 0);
  while (!Uart.Var.uartTxFifo[U_DBG_IDX].bufEmpty);
}


void _SendClassifierResults (PpsoPno_t *pso)
{
  sUartLineBuffer_t buf = {0};
  UINT8 i, j, offset = 0;;
  const UINT8  strGroup[]   = "Group "
              ,strGroupLen  = 6
              ,strEqual[]   = " = "
              ,strEqualLen  = 3
              ;
  
  for (i = 0; i < pso->classifierGroups.nGroups; i++)
  {
    memcpy(&buf.buffer[buf.length], strGroup, strGroupLen);
    buf.length += strGroupLen;
    buf.buffer[buf.length++] = strNumbers[i];
    memcpy(&buf.buffer[buf.length], strEqual, strEqualLen);
    buf.length += strEqualLen;
    for (j = 0; j < pso->classifierGroups.groupLengths[i]; j++)
    {
      buf.buffer[buf.length++] = strNumbers[pso->classifierGroups.groups[j + offset]];
      buf.buffer[buf.length++] = ' ';
    }
    offset += pso->classifierGroups.groupLengths[i];
    buf.buffer[buf.length++] = '\n';
    buf.buffer[buf.length++] = '\r';
  }
  buf.buffer[buf.length++] = '\n';
  buf.buffer[buf.length++] = '\r';
  
  while (Uart.PutTxFifoBuffer(U_DBG, &buf) < 0);
  while (!Uart.Var.uartTxFifo[U_DBG_IDX].bufEmpty);
}


void _SendSwarmPerturbedUnits(PpsoPno_t *pso, DbgDataType_t type, UINT8 swarmId, UINT8 *units, UINT8 nUnits)
{
  sUartLineBuffer_t buf = {0};
  UINT8 i;
  const UINT8  strErrorMsg[]        = "Wrong parameter!\n\r"
              ,strErrorMsgLen       = 18
              ,strPerturbedUnits[]  = " perturbed units: "
              ,strPerturbedUnitsLen = 18;
              ;
  
  switch(type)
  {
    case DBG_PARA_SWARM:
      memcpy(&buf.buffer[buf.length], strParaSwarm, strParaSwarmLen);
      buf.length += strParaSwarmLen;
      break;
    case DBG_SEQ_SWARM:
      memcpy(&buf.buffer[buf.length], strSeqSwarm, strSeqSwarmLen);
      buf.length += strSeqSwarmLen;
      break;
    case DBG_PNO:
      memcpy(&buf.buffer[buf.length], strPnoSwarm, strPnoSwarmLen);
      buf.length += strPnoSwarmLen;
      break;
    default:
      memcpy(&buf.buffer[buf.length], strErrorMsg, strErrorMsgLen);
      buf.length = strErrorMsgLen;
      while (Uart.PutTxFifoBuffer(U_DBG, &buf) < 0);
  }
  
  buf.buffer[buf.length++] = strNumbers[swarmId];
  memcpy(&buf.buffer[buf.length], strPerturbedUnits, strPerturbedUnitsLen);
  buf.length += strPerturbedUnitsLen;
  
  for (i = 0; i < nUnits; i++)
  {
    buf.buffer[buf.length++] = strNumbers[units[i]];
    buf.buffer[buf.length++] = ' ';
  }
  buf.buffer[buf.length++] = '\n';
  buf.buffer[buf.length++] = '\r';
  
  while (Uart.PutTxFifoBuffer(U_DBG, &buf) < 0);
  while (!Uart.Var.uartTxFifo[U_DBG_IDX].bufEmpty);
}


void _SendUnitsToRemove (PpsoPno_t *pso, UINT8 swarmId, UINT8 *units, UINT8 nUnits)
{
  sUartLineBuffer_t buf = {0};
  UINT8 i;
  const UINT8  strRemoveUnits[]  = " units to remove: "
              ,strRemoveUnitsLen = 18;
              ;
  
  memcpy(&buf.buffer[buf.length], strParaSwarm, strParaSwarmLen);
  buf.length += strParaSwarmLen;
  
  buf.buffer[buf.length++] = strNumbers[swarmId];
  
  memcpy(&buf.buffer[buf.length], strRemoveUnits, strRemoveUnitsLen);
  buf.length += strRemoveUnitsLen;
  
  for (i = 0; i < nUnits; i++)
  {
    buf.buffer[buf.length++] = strNumbers[units[i]];
    buf.buffer[buf.length++] = ' ';
  }
  buf.buffer[buf.length++] = '\n';
  buf.buffer[buf.length++] = '\r';
  
  while (Uart.PutTxFifoBuffer(U_DBG, &buf) < 0);
  while (!Uart.Var.uartTxFifo[U_DBG_IDX].bufEmpty);
}


// Public functions
//==============================================================================

const AlgoInterface_t * PpsoPnoInterface()
{
  return &_ppsoPno_if;
}