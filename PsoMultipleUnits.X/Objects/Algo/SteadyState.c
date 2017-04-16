//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : SteadyState.c
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the steady state methods.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "SteadyState.h"
#include "ByteBuffer.h"
#include "MathFunctions.h"


// Private definitions
//==============================================================================


// Private prototypes
//==============================================================================

float _SteadyState_GetMeanSamples (SteadyState_t *ss);
float _SteadyState_GetMinSamples  (SteadyState_t *ss);
float _SteadyState_GetMaxSamples  (SteadyState_t *ss);


// Private variables
//==============================================================================



// Private functions
//==============================================================================

float _SteadyState_GetMeanSamples (SteadyState_t *ss)
{
  size_t i = 0;
  float mean = 0;
  for (i = 0; i < ss->bufSize; i++)
  {
    mean += ss->buf[i];
  }
  mean /= ss->bufSize;
  return mean;
}


float _SteadyState_GetMaxSamples (SteadyState_t *ss)
{
  size_t i = 0;
  float max = 0;
  for (i = 0; i < ss->bufSize; i++)
  {
    max = MAX(max, ss->buf[i]);
  }
  return max;
}

float _SteadyState_GetMinSamples (SteadyState_t *ss)
{
  size_t i = 0;
  float min = ss->buf[0];
  for (i = 0; i < ss->bufSize; i++)
  {
    min = MIN(min, ss->buf[i]);
  }
  return min;
}


// Public functions
//==============================================================================

BOOL SteadyState_CheckForSteadyState (SteadyState_t *ss)
{
  float mean, min, max, upper, lower;
  
  if (ss->count < ss->bufSize)
  {
    ss->oInSteadyState = 0;
    return 0;
  }
  
  mean = _SteadyState_GetMeanSamples(ss);
  min  = _SteadyState_GetMinSamples (ss);
  max  = _SteadyState_GetMaxSamples (ss);
  upper = max - mean;
  lower = min - max;
  if (  ( ( ABS(upper) / mean ) >= ss->oscAmp )
     || ( ( ABS(lower) / mean ) >= ss->oscAmp ) )
  {
    ss->oInSteadyState = 0;
    return 0;
  }
  else
  {
    ss->oInSteadyState = 1;
    return 1;
  }
}


void SteadyState_Init (SteadyState_t *ss, float *sampleBuf, size_t bufSize, float oscAmp)
{
  ss->buf = sampleBuf;
  ss->bufSize = bufSize;
  ss->oscAmp = oscAmp;
  ss->oInSteadyState = 0;
  
  BUFFER_RESET(ss);
}


void SteadyState_AddSample (SteadyState_t *ss, float *newSample)
{
  BUFFER_ADD_FAST(ss, newSample);
}


void SteadyState_Reset (SteadyState_t *ss)
{
  BUFFER_RESET(ss);
  ss->oInSteadyState = 0;
}