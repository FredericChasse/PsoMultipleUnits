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


// Public functions
//==============================================================================

const AlgoInterface_t * PnoSwarmInterface (void);



#endif // __PNO_SWARM_H__