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


// Private definitions
//==============================================================================

typedef enum
{
  PSO_TYPE_PARALLEL_PSO
 ,PSO_TYPE_PSO_1D
} PsoType_t;

typedef struct
{
  UINT8 nSwarms;
  UnitArrayInterface_t *unitArray;
  void *swarms[N_UNITS_TOTAL + 1];
  float sampleTime;
  float timeElapsed;
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
};

Pso_t _pso1d = 
{
  .type         = PSO_TYPE_PSO_1D
 ,.nSwarms      = 0
 ,.swarms       = {0}
 ,.unitArray    = 0
 ,.sampleTime   = 0.08
 ,.timeElapsed  = 0
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
#warning "TBD."
//  pso->swarms = 0;
  return 0;
}


INT8 _Pso1d_Init (Pso_t *pso, UnitArrayInterface_t *unitArray)
{
  pso->unitArray = unitArray;
  pso->nSwarms = pso->unitArray->GetNUnits(pso->unitArray->ctx);
  pso->timeElapsed = 0;
#warning "TBD."
//  pso->swarms = 0;
  return 0;
}


INT8 _ParallelPso_Run (Pso_t *pso)
{
  
}


INT8 _Pso1d_Run (Pso_t *pso)
{
  
}


INT8 _Pso_Close (Pso_t *pso)
{
  pso->nSwarms = 0;
  pso->timeElapsed = 0;
  pso->unitArray = 0;
#warning "TBD."
//  pso->swarms = 0;
  return 0;
}


float _Pso_GetTimeElapsed (Pso_t *pso)
{
  return pso->timeElapsed;
}


// Public functions
//==============================================================================

const AlgoInterface_t * ParallelPsoInterface(void)
{
  return &_parallelPso_if;
}


const AlgoInterface_t * Pso1dInterface(void)
{
  return &_pso1d_if;
}