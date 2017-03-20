//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Algo.c
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the different classes needed for Parallel PSO.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "Algo.h"


// Private definitions
//==============================================================================

typedef struct
{
  AlgoType_t            type;
  UnitArrayInterface_t *units;
} Algo_t;


// Private prototypes
//==============================================================================
void _Algo_Close(Algo_t *algo);
void _Algo_Init (Algo_t *algo, AlgoType_t algoType, UnitArrayInterface_t *unitArray);
void _Algo_Run  (Algo_t *algo);


// Private variables
//==============================================================================
Algo_t _algo;

const AlgoInterface_t _algo_if =
{
  .ctx = (void *) &_algo
 ,.Close = (AlgoClose_fct) &_Algo_Close
 ,.Init = (AlgoInit_fct) &_Algo_Init
 ,.Run = (AlgoRun_fct) &_Algo_Run
};


// Private functions
//==============================================================================

void _Algo_Close(Algo_t *algo)
{
  
}


void _Algo_Init (Algo_t *algo, AlgoType_t algoType, UnitArrayInterface_t *unitArray)
{
  algo->units = unitArray;
  algo->type  = algoType;
  
  switch (algo->type)
  {
    case ALGO_TYPE_PARALLEL_PSO:
      break;
    case ALGO_TYPE_PSO_1D:
      break;
    case ALGO_TYPE_EXTREMUM_SEEKING:
      break;
    default:
      break;
  }
}


void _Algo_Run  (Algo_t *algo)
{
  
}


// Public functions
//==============================================================================

const AlgoInterface_t * AlgoInterface(void)
{
  return &_algo_if;
}