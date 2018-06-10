//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// RNG
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Rng.c
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the PCG family RNG.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "pcg_basic.h"
#include "Rng.h"


// Private definitions
//==============================================================================

typedef pcg32_random_t Rng_t;


// Private variables
//==============================================================================

BOOL oRngIsInitialized = 0;

BOOL oReuseValues = 1;

static Rng_t rng = 
{
  .inc   = 1  // Must always be odd.
 ,.state = 0  
};


// Private prototypes
//==============================================================================

double _RandUint32ToDouble (UINT32 rand);


// Private functions
//==============================================================================

double _RandUint32ToDouble (UINT32 rand)
{
#define MAX_UINT32_INVERTED     (2.32830643653869628906e-010)
  const double inv32 = MAX_UINT32_INVERTED;
  return ((double) rand) * inv32;
}


// Public functions
//==============================================================================

void Rng_InitSeed (UINT64 seed1, UINT64 seed2)
{
  if ( (!oRngIsInitialized) || oReuseValues)
  {
    rng.inc   = 1;
    rng.state = 0;
    
    seed2 |= 1;   // Must always be odd.
    pcg32_srandom_r(&rng, seed1, seed2);
    oRngIsInitialized = 1;
  }
}


float Rng_GetRandFloat (void)
{
  return _RandUint32ToDouble(pcg32_random_r(&rng));
}


float Rng_GetRandBoundedFloat (float bound1, float bound2)
{
  UINT32 rand = pcg32_random_r(&rng);
  double fRand = _RandUint32ToDouble(rand);
  float range = bound2 - bound1;
  return fRand*range + bound1;
}


UINT32 Rng_GetRandBoundedUint32 (UINT32 bound1, UINT32 bound2)
{
  UINT32 bound;
  if (bound1 != 0)
  {
    bound = bound2 - bound1;
  }
  else
  {
    bound = bound2;
  }
  
  return pcg32_boundedrand_r(&rng, bound) + bound1;
}


UINT32 Rng_GetRandUint32 (void)
{
  return pcg32_random_r(&rng);
}