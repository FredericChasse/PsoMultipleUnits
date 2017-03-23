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
#include "SteadyState.h"
#include "PsoParticle.h"


// Private definitions
//==============================================================================

typedef struct
{
  UINT8 id;
  UINT8 nParticles;
  PsoParticleInterface_t *particles;
  UnitArrayInterface_t *unitArray;
  PsoSwarmType_t type;
  Position_t    gbest;
  float         c1;
  float         c2;
  float         posMin;
  float         posMax;
  float         omega;
  float         perturbAmp;
  float         sentinelMargin;
  UINT8         nUnitsPerParticle;
  UINT8         minParticles;
  SteadyState_t steadyState;
  BOOL          oSwarmLocked;
} PsoSwarm_t;



// Private prototypes
//==============================================================================


// Private variables
//==============================================================================

PsoSwarm_t _swarms[N_UNITS_TOTAL + 1];

const PsoSwarmInterface_t *_swarms_if[N_UNITS_TOTAL + 1];// = 
//{
//  {(void *) &_swarms[0]}
// ,{(void *) &_swarms[1]}
// ,{(void *) &_swarms[2]}
// ,{(void *) &_swarms[3]}
// ,{(void *) &_swarms[4]}
// ,{(void *) &_swarms[5]}
// ,{(void *) &_swarms[6]}
// ,{(void *) &_swarms[7]}
// ,{(void *) &_swarms[8]}
//};


// Private functions
//==============================================================================




// Public functions
//==============================================================================

void Position_Reset (Position_t *pos)
{
  pos->curFitness  = 0;
  pos->curPos       = 0;
  pos->prevFitness  = 0;
  pos->prevPos      = 0;
}