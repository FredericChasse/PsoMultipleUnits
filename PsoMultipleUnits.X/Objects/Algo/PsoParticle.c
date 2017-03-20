//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : PsoParticle.c
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the particles of PSO.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "PsoParticle.h"
#include "PsoSwarm.h"


// Private definitions
//==============================================================================

#warning "TBD."
typedef enum
{
  PARTICLE_STATE_1
 ,PARTICLE_STATE_2
} ParticleState_t;

typedef struct
{
  UINT8 id;
  Position_t pbest;
  Position_t pbestAbs;
  Position_t pos;
  Position_t optPos;
  float curSpeed;
  float prevSpeed;
  float jSteady;
  BOOL oSentinelWarning;
  ParticleState_t state;
//  SteadyState_t steadyState;  // TBD
} PsoParticle_t;


// Private prototypes
//==============================================================================



// Private variables
//==============================================================================



// Private functions
//==============================================================================




// Public functions
//==============================================================================

