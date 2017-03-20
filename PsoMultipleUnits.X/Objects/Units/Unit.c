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
#include "StateMachine.h"


// Private definitions
//==============================================================================
typedef struct
{
  UINT8 id;
  UINT8 adcNum;
  UINT8 potNum;
  float pos;
  float power;
} Unit_t;


// Private prototypes
//==============================================================================
void  _Unit_Init      (Unit_t *unit);
INT8  _Unit_SetPos    (Unit_t *unit, float pos);
float _Unit_GetPos    (Unit_t *unit);
float _Unit_GetPower  (Unit_t *unit);
UINT8 _Unit_GetId     (Unit_t *unit);

// Private variables
//==============================================================================

extern struct sAllCells sCellValues;

Unit_t _units[N_UNITS_TOTAL] = 
{
  {.id = 0, .adcNum =  8, .potNum =  8, .pos = 50, .power = 0}
 ,{.id = 1, .adcNum =  9, .potNum =  9, .pos = 50, .power = 0}
 ,{.id = 2, .adcNum = 10, .potNum = 10, .pos = 50, .power = 0}
 ,{.id = 3, .adcNum = 11, .potNum = 11, .pos = 50, .power = 0}
 ,{.id = 4, .adcNum = 12, .potNum = 12, .pos = 50, .power = 0}
 ,{.id = 5, .adcNum = 13, .potNum = 13, .pos = 50, .power = 0}
 ,{.id = 6, .adcNum = 14, .potNum = 14, .pos = 50, .power = 0}
 ,{.id = 7, .adcNum = 15, .potNum = 15, .pos = 50, .power = 0}
};

const UnitInterface_t _unit0_if = 
{
  .ctx        = (void *)            &_units[0]
 ,.GetId      = (UnitGetId_fct)     &_Unit_GetId 
 ,.GetPos     = (UnitGetPos_fct)    &_Unit_GetPos 
 ,.GetPower   = (UnitGetPower_fct)  &_Unit_GetPower 
 ,.Init       = (UnitInit_fct)      &_Unit_Init 
 ,.SetPos     = (UnitSetPos_fct)    &_Unit_SetPos
};
const UnitInterface_t _unit1_if = 
{
  .ctx        = (void *)            &_units[1]
 ,.GetId      = (UnitGetId_fct)     &_Unit_GetId 
 ,.GetPos     = (UnitGetPos_fct)    &_Unit_GetPos 
 ,.GetPower   = (UnitGetPower_fct)  &_Unit_GetPower 
 ,.Init       = (UnitInit_fct)      &_Unit_Init 
 ,.SetPos     = (UnitSetPos_fct)    &_Unit_SetPos
};
const UnitInterface_t _unit2_if = 
{
  .ctx        = (void *)            &_units[2]
 ,.GetId      = (UnitGetId_fct)     &_Unit_GetId 
 ,.GetPos     = (UnitGetPos_fct)    &_Unit_GetPos 
 ,.GetPower   = (UnitGetPower_fct)  &_Unit_GetPower 
 ,.Init       = (UnitInit_fct)      &_Unit_Init 
 ,.SetPos     = (UnitSetPos_fct)    &_Unit_SetPos
};
const UnitInterface_t _unit3_if = 
{
  .ctx        = (void *)            &_units[3]
 ,.GetId      = (UnitGetId_fct)     &_Unit_GetId 
 ,.GetPos     = (UnitGetPos_fct)    &_Unit_GetPos 
 ,.GetPower   = (UnitGetPower_fct)  &_Unit_GetPower 
 ,.Init       = (UnitInit_fct)      &_Unit_Init 
 ,.SetPos     = (UnitSetPos_fct)    &_Unit_SetPos
};
const UnitInterface_t _unit4_if = 
{
  .ctx        = (void *)            &_units[4]
 ,.GetId      = (UnitGetId_fct)     &_Unit_GetId 
 ,.GetPos     = (UnitGetPos_fct)    &_Unit_GetPos 
 ,.GetPower   = (UnitGetPower_fct)  &_Unit_GetPower 
 ,.Init       = (UnitInit_fct)      &_Unit_Init 
 ,.SetPos     = (UnitSetPos_fct)    &_Unit_SetPos
};
const UnitInterface_t _unit5_if = 
{
  .ctx        = (void *)            &_units[5]
 ,.GetId      = (UnitGetId_fct)     &_Unit_GetId 
 ,.GetPos     = (UnitGetPos_fct)    &_Unit_GetPos 
 ,.GetPower   = (UnitGetPower_fct)  &_Unit_GetPower 
 ,.Init       = (UnitInit_fct)      &_Unit_Init 
 ,.SetPos     = (UnitSetPos_fct)    &_Unit_SetPos
};
const UnitInterface_t _unit6_if = 
{
  .ctx        = (void *)            &_units[6]
 ,.GetId      = (UnitGetId_fct)     &_Unit_GetId 
 ,.GetPos     = (UnitGetPos_fct)    &_Unit_GetPos 
 ,.GetPower   = (UnitGetPower_fct)  &_Unit_GetPower 
 ,.Init       = (UnitInit_fct)      &_Unit_Init 
 ,.SetPos     = (UnitSetPos_fct)    &_Unit_SetPos
};
const UnitInterface_t _unit7_if = 
{
  .ctx        = (void *)            &_units[7]
 ,.GetId      = (UnitGetId_fct)     &_Unit_GetId 
 ,.GetPos     = (UnitGetPos_fct)    &_Unit_GetPos 
 ,.GetPower   = (UnitGetPower_fct)  &_Unit_GetPower 
 ,.Init       = (UnitInit_fct)      &_Unit_Init 
 ,.SetPos     = (UnitSetPos_fct)    &_Unit_SetPos
};

const UnitInterface_t *_units_if[N_UNITS_TOTAL] = 
{
  &_unit0_if
 ,&_unit1_if
 ,&_unit2_if
 ,&_unit3_if
 ,&_unit4_if
 ,&_unit5_if
 ,&_unit6_if
 ,&_unit7_if
};

// Private functions
//==============================================================================

void _Unit_Init (Unit_t *unit)
{
}

INT8 _Unit_SetPos (Unit_t *unit, float pos)
{
  INT8 ret;
  if (pos != unit->pos)
  {
    ret = ComputePotValueFloat2Dec(pos);
    if (ret >= 0)
    {
      SetPot(unit->potNum, ret);
      unit->pos = pos;
      return 0;
    }
    else
    {
      return -1;
    }
  }
  return 0;
}


float _Unit_GetPos (Unit_t *unit)
{
  return unit->pos;
}


float _Unit_GetPower (Unit_t *unit)
{
  unit->power = sCellValues.cells[unit->adcNum].cellPowerFloat;
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
  if (id > N_UNITS_TOTAL)
  {
    return 0;
  }
  
  return _units_if[id];
}