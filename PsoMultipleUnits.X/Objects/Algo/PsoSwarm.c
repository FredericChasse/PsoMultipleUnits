//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : PsoSwarm.c
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the swarms of PSO.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "PsoSwarm.h"


// Private definitions
//==============================================================================

typedef struct
{
  UINT8 id;
  UINT8 nParticles;
  void *particles;    // TBD
  UnitArrayInterface_t *unitArray;
  PsoSwarmType_t type;
  Position_t gbest;
  float c1;
  float c2;
  float posMin;
  float posMax;
  float omega;
  float perturbAmp;
  float sentinelMargin;
  UINT8 nUnitsPerParticle;
  UINT8 minParticles;
//  struct SteadyState_t steadyState; // TBD
} PsoSwarm_t;



// Private prototypes
//==============================================================================


// Private variables
//==============================================================================

PsoSwarm_t _swarm0;
PsoSwarm_t _swarm1;
PsoSwarm_t _swarm2;
PsoSwarm_t _swarm3;
PsoSwarm_t _swarm4;
PsoSwarm_t _swarm5;
PsoSwarm_t _swarm6;
PsoSwarm_t _swarm7;
PsoSwarm_t _swarm8;

PsoSwarm_t *_swarms[N_UNITS_TOTAL + 1] = 
{
  &_swarm0
 ,&_swarm1
 ,&_swarm2
 ,&_swarm3
 ,&_swarm4
 ,&_swarm5
 ,&_swarm6
 ,&_swarm7
 ,&_swarm8
};



// Private functions
//==============================================================================




// Public functions
//==============================================================================

