//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : SteadyStatePno.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the steady state methods for P&O.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __STEADY_STATE_PNO_H__
#define __STEADY_STATE_PNO_H__

#include "Setup.h"

// Public definitions
//==============================================================================

#define STEADY_STATE_PNO_MAX_SAMPLES      (20)

typedef struct
{
  // Circular sample buffer
  float  *buf;
  size_t  inIdx;
  size_t  outIdx;
  size_t  bufSize;
  size_t  count;
  
  float   delta;
  UINT8   oscAmp;
  BOOL    oInSteadyState;
} SteadyStatePno_t;

// Public functions
//==============================================================================

BOOL  SteadyStatePno_CheckForSteadyState (SteadyStatePno_t *ss);
void  SteadyStatePno_Init (SteadyStatePno_t *ss, float *sampleBuf, size_t bufSize, UINT8 oscAmp, float delta);
void  SteadyStatePno_AddSample (SteadyStatePno_t *ss, float *newSample);
void  SteadyStatePno_Reset (SteadyStatePno_t *ss);
float SteadyStatePno_GetMeanPos (SteadyStatePno_t *ss);



#endif // __STEADY_STATE_PNO_H__