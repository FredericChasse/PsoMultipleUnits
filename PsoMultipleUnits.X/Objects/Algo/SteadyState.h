//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : SteadyState.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the steady state methods.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __STEADY_STATE_H__
#define __STEADY_STATE_H__

#include "Setup.h"

// Public definitions
//==============================================================================

#define MAX(_x, _y)   ( (_x > _y) ? _x :  _y )
#define MIN(_x, _y)   ( (_x < _y) ? _x :  _y )
#define ABS(_x)       ( (_x >= 0) ? _x : -_x )

#define STEADY_STATE_MAX_SAMPLES      (10)

typedef struct
{
  // Circular sample buffer
  float  *buf;
  size_t  inIdx;
  size_t  outIdx;
  size_t  bufSize;
  size_t  count;
  
  float   oscAmp;
  BOOL    oInSteadyState;
} SteadyState_t;

// Public functions
//==============================================================================

BOOL SteadyState_CheckForSteadyState (SteadyState_t *ss);
void SteadyState_Init (SteadyState_t *ss, float *sampleBuf, size_t bufSize, float oscAmp);
void SteadyState_AddSample (SteadyState_t *ss, float *newSample);
void SteadyState_Reset (SteadyState_t *ss);



#endif // __STEADY_STATE_H__