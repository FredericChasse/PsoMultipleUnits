//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Perturb.c
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the methods for the abstract Perturbation object.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "Perturb.h"
#include "LedDriver.h"
#include "UnitMapping.h"
#include "MathFunctions.h"
#include "StateMachine.h"   // For debugging


// Private definitions
//==============================================================================

#define MIN_LED_INTENSITY   (0)
#define MAX_LED_INTENSITY   (600)

#define N_PERTURB_TOTAL     (10)

typedef struct
{
  UINT32  iteration;
  UINT8   units[N_UNITS_TOTAL];
  UINT8   nUnits;
  INT16   amplitude;
} PerturbInstance_t;

typedef struct
{
  UINT32  iteration;
  INT16   intensities[N_UNITS_TOTAL];
  INT16   initVal    [N_UNITS_TOTAL];
  UINT8   leds       [N_UNITS_TOTAL];
  UINT8   nLeds;
  UINT8               nPerturbs;
  PerturbInstance_t  *perturbs;
} Perturb_t;


// Private prototypes
//==============================================================================

void    _Perturb_Init               (Perturb_t *p, UINT16 intensityInit);
UINT16  _Perturb_GetUnitIntensity   (Perturb_t *p, UINT8 unit);
void    _Perturb_SetUnitIntensity   (Perturb_t *p, UINT8 unit, UINT16 intensity);
INT8    _Perturb_SetNewPerturb      (Perturb_t *p, UINT8 *units, UINT8 nUnits, INT16 amplitude, UINT32 iteration);
void    _Perturb_Reset              (Perturb_t *p);
void    _Perturb_Run                (Perturb_t *p);


// Private variables
//==============================================================================

PerturbInstance_t _perturbs[N_PERTURB_TOTAL] = 
{
  {.iteration =  0 }
 ,{.amplitude =  0 }
 ,{.nUnits    =  0 }
 ,{.units     = {0}}
};

Perturb_t _perturb = 
{
  .intensities  = {0}
 ,.initVal      = {0}
 ,.iteration    = 0
 ,.nLeds        = N_UNITS_TOTAL
 ,.leds         = {0}
 ,.nPerturbs    = 0
 ,.perturbs     = _perturbs
};

const PerturbInterface_t _perturb_if = 
{
  .ctx                = (void *)                        &_perturb
 ,.Init               = (PerturbInit_fct)               &_Perturb_Init
 ,.SetUnitIntensity   = (PerturbSetUnitIntensity_fct)   &_Perturb_SetUnitIntensity
 ,.GetUnitIntensity   = (PerturbGetUnitIntensity_fct)   &_Perturb_GetUnitIntensity
 ,.SetNewPerturb      = (PerturbSetNewPerturb_fct)      &_Perturb_SetNewPerturb
 ,.Reset              = (PerturbReset_fct)              &_Perturb_Reset
 ,.Run                = (PerturbRun_ftc)                &_Perturb_Run
};


// Private functions
//==============================================================================

void _Perturb_Init (Perturb_t *p, UINT16 intensityInit)
{
  UINT8 i;
  InitLedDriver();
  for (i = 0; i < N_LED_TOTAL; i++)
  {
    SetLedDutyCycle(i, 0);
  }
  
  for (i = 0; i < N_UNITS_TOTAL; i++)
  {
    p->leds[i] = unitsPwms[i];
    SetLedDutyCycle(p->leds[i], intensityInit);
    p->intensities[i] = intensityInit;
    p->initVal[i] = intensityInit;
  }
}


void _Perturb_SetUnitIntensity (Perturb_t *p, UINT8 unit, UINT16 intensity)
{
  intensity = MIN(intensity, MAX_LED_INTENSITY);
  p->intensities[unit] = intensity;
  SetLedDutyCycle(p->leds[unit], intensity);
}


UINT16 _Perturb_GetUnitIntensity (Perturb_t *p, UINT8 unit)
{
  return p->intensities[unit];
}


INT8 _Perturb_SetNewPerturb (Perturb_t *p, UINT8 *units, UINT8 nUnits, INT16 amplitude, UINT32 iteration)
{
  if (p->nPerturbs == N_PERTURB_TOTAL)
  {
    return -1;
  }
  
  memcpy(p->perturbs[p->nPerturbs].units, units, nUnits);
  p->perturbs[p->nPerturbs].nUnits = nUnits;
  p->perturbs[p->nPerturbs].amplitude = amplitude;
  p->perturbs[p->nPerturbs].iteration = iteration;
  p->nPerturbs++;
  
  return 0;
}


void _Perturb_Reset (Perturb_t *p)
{
  UINT8 i;
  INT8 ret;
  p->nPerturbs = 0;
  p->iteration = 0;
  
  for (i = 0; i < p->nLeds; i++)
  {
    p->intensities[i] = p->initVal[i];
    ret = SetLedDutyCycle(p->leds[i], p->intensities[i]);
    __assert(ret == 0, "_Perturb_Reset");
  }
}


void _Perturb_Run (Perturb_t *p)
{
  UINT8 i, j;
  PerturbInstance_t *pb;
  p->iteration++;
  INT16 intensity;
  
  for (i = 0; i < p->nPerturbs; i++)
  {
    pb = &p->perturbs[i];
    if (p->iteration == pb->iteration)
    {
      for (j = 0; j < pb->nUnits; j++)
      {
        intensity = p->intensities[pb->units[j]] + pb->amplitude;
        intensity = MIN(intensity, MAX_LED_INTENSITY);
        intensity = MAX(intensity, MIN_LED_INTENSITY);
        p->intensities[pb->units[j]] = intensity;
        SetLedDutyCycle(p->leds[pb->units[j]], intensity);
      }
    }
  }
}


// Public functions
//==============================================================================
const PerturbInterface_t * PerturbInterface(void)
{
  return &_perturb_if;
}