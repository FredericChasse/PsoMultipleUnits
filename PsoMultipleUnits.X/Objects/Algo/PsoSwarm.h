//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : PsoSwarm.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the swarms of PSO.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __PSO_SWARM_H__
#define __PSO_SWARM_H__

#include "Setup.h"
#include "UnitArray.h"

// Public definitions
//==============================================================================

typedef struct
{
  float curPos;
  float curPower;
  float prevPos;
  float prevPower;
} Position_t;

#warning "TBD."
//typedef struct SteadyState_t;

typedef enum
{
  PSO_SWARM_TYPE_PARALLEL_PSO
 ,PSO_SWARM_TYPE_PSO_1D
} PsoSwarmType_t;

// Public functions
//==============================================================================





#endif // __PSO_SWARM_H__