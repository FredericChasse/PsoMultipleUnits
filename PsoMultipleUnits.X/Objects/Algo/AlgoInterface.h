//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : AlgoInterface.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is file provides an interface for all possible algorithms.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __ALGO_INTERFACE_H__
#define __ALGO_INTERFACE_H__

#include "Setup.h"
#include "UnitArray.h"


// Public definitions
//==============================================================================

typedef INT8  (*AlgoInit_fct)           (void *ctx, UnitArrayInterface_t *unitArray);
typedef INT8  (*AlgoRun_fct)            (void *ctx);
typedef float (*AlgoGetTimeElapsed_fct) (void *ctx);
typedef void  (*AlgoRelease_fct)        (void *ctx);
typedef void  (*AlgoGetDebugData_fct)   (void *ctx, void *ret);

typedef struct
{
  void                   *ctx;
  AlgoInit_fct            Init;
  AlgoRun_fct             Run;
  AlgoGetTimeElapsed_fct  GetTimeElapsed;
  AlgoRelease_fct         Release;
  AlgoGetDebugData_fct    GetDebugData;
} AlgoInterface_t;




#endif // __ALGO_INTERFACE_H__