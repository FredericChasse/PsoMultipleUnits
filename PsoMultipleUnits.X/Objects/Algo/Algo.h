//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Algo.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the header file for the classes of Parallel PSO.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __ALGO_H__
#define __ALGO_H__

#include "Setup.h"
#include "UnitArray.h"

// Public definitions
//==============================================================================

typedef enum
{
  ALGO_TYPE_PARALLEL_PSO
 ,ALGO_TYPE_PSO_1D
 ,ALGO_TYPE_EXTREMUM_SEEKING
} AlgoType_t;

typedef void (*AlgoInit_fct)  (void *ctx, AlgoType_t algoType, UnitArrayInterface_t *unitArray);
typedef void (*AlgoRun_fct)   (void *ctx);
typedef void (*AlgoClose_fct) (void *ctx);

typedef struct
{
  void                 *ctx;
  AlgoInit_fct          Init;
  AlgoRun_fct           Run;
  AlgoClose_fct         Close;
} AlgoInterface_t;


// Public functions
//==============================================================================
const AlgoInterface_t * AlgoInterface(void);




#endif // __ALGO_H__