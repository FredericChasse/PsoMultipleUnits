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

#include "SteadyStatePno.h"
#include "ByteBuffer.h"
#include "MathFunctions.h"


// Private definitions
//==============================================================================


// Private prototypes
//==============================================================================

float _SteadyStatePno_GetMeanSamples (SteadyStatePno_t *ss);
float _SteadyStatePno_GetMinSamples  (SteadyStatePno_t *ss);
float _SteadyStatePno_GetMaxSamples  (SteadyStatePno_t *ss);


// Private variables
//==============================================================================



// Private functions
//==============================================================================

float _SteadyStatePno_GetMeanSamples (SteadyStatePno_t *ss)
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


float _SteadyStatePno_GetMaxSamples (SteadyStatePno_t *ss)
{
  size_t i = 0;
  float max = 0;
  for (i = 0; i < ss->bufSize; i++)
  {
    max = MAX(max, ss->buf[i]);
  }
  return max;
}

float _SteadyStatePno_GetMinSamples (SteadyStatePno_t *ss)
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

BOOL SteadyStatePno_CheckForSteadyState (SteadyStatePno_t *ss)
{
  float mean, min, max, upper, lower;
  
  if (ss->count < ss->bufSize)
  {
    ss->oInSteadyState = 0;
    return 0;
  }
  
  mean = _SteadyStatePno_GetMeanSamples(ss);
  min  = _SteadyStatePno_GetMinSamples (ss);
  max  = _SteadyStatePno_GetMaxSamples (ss);
  upper = max - mean;
  lower = mean - min;
  if (  ( ABS(upper) >= (ss->oscAmp*ss->delta) )
     || ( ABS(lower) >= (ss->oscAmp*ss->delta) ) )
  {
    ss->oInSteadyState = 0;
  }
  else
  {
    ss->oInSteadyState = 1;
  }
  
  return ss->oInSteadyState;
}


void SteadyStatePno_Init (SteadyStatePno_t *ss, float *sampleBuf, size_t bufSize, UINT8 oscAmp, float delta)
{
  ss->buf = sampleBuf;
  ss->bufSize = bufSize;
  ss->oscAmp = oscAmp;
  ss->oInSteadyState = 0;
  ss->delta = delta;
  
  BUFFER_RESET(ss);
}


void SteadyStatePno_AddSample (SteadyStatePno_t *ss, float *newSample)
{
  BUFFER_ADD_FAST(ss, newSample);
}


void SteadyStatePno_Reset (SteadyStatePno_t *ss)
{
  BUFFER_RESET(ss);
  ss->oInSteadyState = 0;
}


float SteadyStatePno_GetMeanPos (SteadyStatePno_t *ss)
{
  return _SteadyStatePno_GetMeanSamples(ss);
}