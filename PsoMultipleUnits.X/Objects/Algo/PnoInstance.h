//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : PnoInstance.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the methods for an instance of P&O.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __PNO_INSTANCE_H__
#define __PNO_INSTANCE_H__

#include "Setup.h"
#include "UnitArray.h"

// Public definitions
//==============================================================================


typedef void  (*PnoiInit_fct)             (void *ctx, UINT8 id, float delta, float pos, float umin, float umax);
typedef float (*PnoiComputePos_fct)       (void *ctx);
typedef void  (*PnoiSetPos_fct)           (void *ctx, float pos);
typedef void  (*PnoiSetFitness_fct)       (void *ctx, float fitness);
typedef void  (*PnoiRelease_fct)          (void *ctx);
typedef void  (*PnoiSetSteadyState_fct)   (void *ctx, UINT8 nSamplesForSs, UINT8 oscAmp);

typedef struct
{
  void                   *ctx;
  PnoiInit_fct            Init;
  PnoiComputePos_fct      ComputePos;
  PnoiSetPos_fct          SetPos;
  PnoiSetFitness_fct      SetFitness;
  PnoiRelease_fct         Release;
  PnoiSetSteadyState_fct  SetSteadyState;
} PnoInstanceInterface_t;


// Public functions
//==============================================================================

const PnoInstanceInterface_t * PnoInstanceInterface (void);



#endif // __PNO_INSTANCE_H__