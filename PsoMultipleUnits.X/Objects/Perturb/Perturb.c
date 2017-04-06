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


// Private definitions
//==============================================================================
typedef struct
{
  UINT16  intensities[N_UNITS_TOTAL];
  UINT8   leds      [16];
  UINT8   nLeds;
} Perturb_t;


// Private prototypes
//==============================================================================

void    _Perturb_Init             (Perturb_t *p, UINT16 intensityInit);
UINT16  _Perturb_GetUnitIntensity (Perturb_t *p, UINT8 unit);
void    _Perturb_SetUnitIntensity (Perturb_t *p, UINT8 unit, UINT16 intensity);

// Private variables
//==============================================================================

Perturb_t _perturb = 
{
  .intensities = {0}
 ,.nLeds      = N_UNITS_TOTAL
 ,.leds       = {UNIT_0_PWM, UNIT_1_PWM, UNIT_2_PWM, UNIT_3_PWM, UNIT_4_PWM, UNIT_5_PWM, UNIT_6_PWM, UNIT_7_PWM}
};

const PerturbInterface_t _perturb_if = 
{
  .ctx = (void *) &_perturb
 ,.Init = (PerturbInit_fct) &_Perturb_Init
 ,.SetUnitIntensity = (PerturbSetUnitIntensity_fct) &_Perturb_SetUnitIntensity
 ,.GetUnitIntensity = (PerturbGetUnitIntensity_fct) &_Perturb_GetUnitIntensity
};

// Private functions
//==============================================================================

void _Perturb_Init (Perturb_t *p, UINT16 intensityInit)
{
  UINT8 i;
  InitLedDriver();
  for (i = 0; i < 16; i++)
  {
    SetLedDutyCycle(i, 0);
  }
  
  for (i = 0; i < N_UNITS_TOTAL; i++)
  {
    SetLedDutyCycle(p->leds[i], intensityInit);
    p->intensities[i] = intensityInit;
  }
}


void _Perturb_SetUnitIntensity (Perturb_t *p, UINT8 unit, UINT16 intensity)
{
  p->intensities[unit] = intensity;
  SetLedDutyCycle(p->leds[unit], intensity);
}


UINT16 _Perturb_GetUnitIntensity (Perturb_t *p, UINT8 unit)
{
  return p->intensities[unit];
}


// Public functions
//==============================================================================
const PerturbInterface_t * PerturbInterface(void)
{
  return &_perturb_if;
}