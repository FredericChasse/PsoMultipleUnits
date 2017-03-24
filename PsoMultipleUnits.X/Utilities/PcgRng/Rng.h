//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// RNG
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Rng.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the PCG family RNG.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __RNG_H__
#define __RNG_H__

#include "Setup.h"


// Public prototypes
//==============================================================================

void    Rng_InitSeed              (UINT64  seed1, UINT64  seed2);
float   Rng_GetRandFloat          (void);
float   Rng_GetRandBoundedFloat   (float  bound1, float  bound2);
UINT32  Rng_GetRandBoundedUint32  (UINT32 bound1, UINT32 bound2);
UINT32  Rng_GetRandUint32         (void);



#endif // __RNG_H__