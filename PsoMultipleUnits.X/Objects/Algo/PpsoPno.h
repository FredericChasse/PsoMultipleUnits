//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : PpsoPno.h
// Author  : Frederic Chasse
// Date    : 2017-07-16
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the Particle Swarm Optimization hybrid with P&O.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __PPSO_PNO_H__
#define __PPSO_PNO_H__

#include "Setup.h"
#include "UnitArray.h"
#include "AlgoInterface.h"

// Public definitions
//==============================================================================

typedef enum
{
  PPSOC
 ,PPSOCD
} PpsocType_t;

// Public functions
//==============================================================================
const AlgoInterface_t * PpsoPnoInterface (PpsocType_t type);




#endif // __PPSO_PNO_H__