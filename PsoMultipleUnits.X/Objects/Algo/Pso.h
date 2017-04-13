//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Pso.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the Particle Swarm Optimization.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __PSO_H__
#define __PSO_H__

#include "Setup.h"
#include "UnitArray.h"
#include "AlgoInterface.h"

// Public definitions
//==============================================================================

typedef enum
{
  PSO_TYPE_PARALLEL_PSO_MULTI_SWARM
 ,PSO_TYPE_PARALLEL_PSO
 ,PSO_TYPE_PSO_1D
} PsoType_t;


// Public functions
//==============================================================================
const AlgoInterface_t * PsoInterface(PsoType_t psoType);




#endif // __PSO_H__