//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Unit.c
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the methods for the abstract Unit object.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "Unit.h"
#include "Potentiometer.h"
#include "StateMachine.h"
#include "UnitMapping.h"


// Private definitions
//==============================================================================
typedef struct
{
  UINT8 id;
  UINT8 adcNum;
  UINT8 potNum;
  UINT8 posIdx;
  float pos;
  float power;
  float minPos;
  float maxPos;
  UINT8 minPosIdx;
  UINT8 maxPosIdx;
  BOOL  oFirstSetupDone;
} Unit_t;


// Private prototypes
//==============================================================================
void  _Unit_Init          (Unit_t *unit);
INT8  _Unit_SetPos        (Unit_t *unit, float pos);
INT8  _Unit_SetPosWithIdx (Unit_t *unit, UINT8 idx);
float _Unit_GetPos        (Unit_t *unit);
float _Unit_GetPower      (Unit_t *unit);
UINT8 _Unit_GetId         (Unit_t *unit);
UINT8 _Unit_GetPosIdx     (Unit_t *unit);
void  _Unit_GetPosLimits  (Unit_t *unit, float *minPos, float *maxPos);
void  _Unit_SetPower      (Unit_t *unit, float power);


// Private variables
//==============================================================================

extern struct sAllCells sCellValues;

Unit_t _units[N_UNITS_TOTAL];

UnitInterface_t _units_if[N_UNITS_TOTAL];

static BOOL _oUnitsInitialized = 0;


// Private functions
//==============================================================================

void _Unit_Init (Unit_t *unit)
{
  if (!unit->oFirstSetupDone)
  {
    unit->oFirstSetupDone = 1;
    SetPot(unit->potNum, 0);
    unit->pos = potRealValues[POT_MIN_INDEX];
    unit->posIdx = POT_MIN_INDEX;
    unit->minPos = potRealValues[POT_MIN_INDEX];
    unit->minPosIdx = POT_MIN_INDEX;
    unit->maxPos = potRealValues[POT_MAX_INDEX];   // 402.9412 Ohms
  }
}


void _Unit_SetPower (Unit_t *unit, float power)
{
  unit->power = power;
}


INT8 _Unit_SetPosWithIdx (Unit_t *unit, UINT8 idx)
{
  if (idx != unit->posIdx)
  {
    SetPot(unit->potNum, idx);
    unit->pos = potRealValues[idx];
    unit->posIdx = idx;
  }
  return 0;
}


INT8 _Unit_SetPos (Unit_t *unit, float pos)
{
  INT16 ret;
  if (pos != unit->pos)
  {
    ret = ComputePotValueFloat2Dec(pos);
    if (ret >= 0)
    {
      SetPot(unit->potNum, ret);
      unit->pos = pos;
      unit->posIdx = ret;
      return 0;
    }
    else
    {
      return -1;
    }
  }
  return 0;
}


UINT8 _Unit_GetPosIdx (Unit_t *unit)
{
  return unit->posIdx;
}


void _Unit_GetPosLimits (Unit_t *unit, float *minPos, float *maxPos)
{
  *minPos = unit->minPos;
  *maxPos = unit->maxPos;
}


float _Unit_GetPos (Unit_t *unit)
{
  return unit->pos;
}


float _Unit_GetPower (Unit_t *unit)
{
//  unit->power = sCellValues.cells[unit->adcNum].cellPowerFloat;
  return unit->power;
}


UINT8 _Unit_GetId (Unit_t *unit)
{
  return unit->id;
}


// Public functions
//==============================================================================
const UnitInterface_t * UnitInterface(UINT8 id)
{
  UINT8 i;
  
  if (id > N_UNITS_TOTAL)
  {
    return 0;
  }
  
  if (!_oUnitsInitialized)
  {
    _oUnitsInitialized = 1;
    
    for (i = 0; i < N_UNITS_TOTAL; i++)
    {
      _units_if[i].ctx            = (void *)                &_units[i];
      _units_if[i].GetId          = (UnitGetId_fct)         &_Unit_GetId;
      _units_if[i].GetPos         = (UnitGetPos_fct)        &_Unit_GetPos;
      _units_if[i].GetPosIdx      = (UnitGetPosIdx_fct)     &_Unit_GetPosIdx;
      _units_if[i].GetPosLimits   = (UnitGetPosLimits_fct)  &_Unit_GetPosLimits;
      _units_if[i].GetPower       = (UnitGetPower_fct)      &_Unit_GetPower;
      _units_if[i].Init           = (UnitInit_fct)          &_Unit_Init;
      _units_if[i].SetPos         = (UnitSetPos_fct)        &_Unit_SetPos;
      _units_if[i].SetPosWithIdx  = (UnitSetPosWithIdx_fct) &_Unit_SetPosWithIdx;
      _units_if[i].SetPower       = (UnitSetPower_fct)      &_Unit_SetPower;
      
      _units[i].adcNum              = unitAdcs[i];
      _units[i].potNum              = unitPots[i];
      _units[i].id                  = i;
      _units[i].pos                 = potRealValues[POT_MIN_INDEX];
      _units[i].posIdx              = POT_MIN_INDEX;
      _units[i].minPos              = potRealValues[POT_MIN_INDEX];
      _units[i].minPosIdx           = POT_MIN_INDEX;
      _units[i].maxPos              = MAX_POT_VALUE;
      _units[i].maxPosIdx           = POT_MAX_INDEX;
      _units[i].power               = 0;
      _units[i].oFirstSetupDone     = 0;
    }
  }
  
  return &_units_if[id];
}