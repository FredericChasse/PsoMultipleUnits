//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : PnoSwarm.h
// Author  : Frederic Chasse
// Date    : 2017-04-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the methods needed for the Perturb & Observe
//           swarm algorithm used with PPSO-P&O.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __PNO_SWARM_H__
#define __PNO_SWARM_H__

#include "Setup.h"
#include "UnitArray.h"
#include "AlgoInterface.h"

// Public definitions
//==============================================================================

typedef INT8  (*PnoSwarmInit_fct)             (void *ctx, UnitArrayInterface_t *unitArray);
typedef void  (*PnoSwarmComputeAllPos_fct)    (void *ctx, float *newPos, BOOL *oPerturbed, UINT8 *nPerturbed);
typedef void  (*PnoSwarmRemoveInstances_fct)  (void *ctx, UINT8 *idx, UINT8 nInstances);
typedef void  (*PnoSwarmGetSteadyState_fct)   (void *ctx, UINT8 *idx);
typedef void  (*PnoSwarmRelease_fct)          (void *ctx);

typedef struct
{
  void                       *ctx;
  AlgoInit_fct                Init;
  PnoSwarmComputeAllPos_fct   ComputeAllPos;
  PnoSwarmRemoveInstances_fct RemoveInstances;
  AlgoRelease_fct             Release;
} PnoSwarmInterface_t;

// Public functions
//==============================================================================

const PnoSwarmInterface_t * PnoSwarmInterface (void);



#endif // __PNO_SWARM_H__