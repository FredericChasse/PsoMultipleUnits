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
  float curFitness;
  float prevPos;
  float prevFitness;
} Position_t;

typedef enum
{
  PSO_SWARM_TYPE_PARALLEL_PSO
 ,PSO_SWARM_TYPE_PSO_1D
} PsoSwarmType_t;

typedef struct
{
  void *ctx;
} PsoSwarmInterface_t;


// Public functions
//==============================================================================

const PsoSwarmInterface_t * PsoSwarmInterface(void);

void Position_Reset (Position_t *pos);



#endif // __PSO_SWARM_H__