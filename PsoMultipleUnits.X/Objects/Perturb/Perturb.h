//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Perturb.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the methods for the abstract Perturbation object.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __PERTURB_H__
#define __PERTURB_H__

#include "Setup.h"


typedef void    (*PerturbInit_fct)              (void *ctx, UINT16 intensityInit);
typedef void    (*PerturbSetUnitIntensity_fct)  (void *ctx, UINT8 unit, UINT16 intensity);
typedef UINT16  (*PerturbGetUnitIntensity_fct)  (void *ctx, UINT8 unit);

typedef struct
{
  void                       *ctx;
  PerturbInit_fct             Init;
  PerturbSetUnitIntensity_fct SetUnitIntensity;
  PerturbGetUnitIntensity_fct GetUnitIntensity;
} PerturbInterface_t;


// Public prototypes
//==============================================================================
const PerturbInterface_t * PerturbInterface(void);


#endif // __PERTURB_H__