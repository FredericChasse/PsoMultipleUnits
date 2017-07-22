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
  BOOL  oFirstSetupDone;
} Unit_t;


// Private prototypes
//==============================================================================
void  _Unit_Init          (Unit_t *unit);
INT8  _Unit_SetPos        (Unit_t *unit, float pos);
float _Unit_GetPos        (Unit_t *unit);
float _Unit_GetPower      (Unit_t *unit);
UINT8 _Unit_GetId         (Unit_t *unit);
UINT8 _Unit_GetPosIdx     (Unit_t *unit);
void  _Unit_GetPosLimits  (Unit_t *unit, float *minPos, float *maxPos);
void  _Unit_SetPower      (Unit_t *unit, float power);

// Private variables
//==============================================================================

extern struct sAllCells sCellValues;

Unit_t _units[N_UNITS_TOTAL] = 
{
  {.id = 0 , .adcNum = UNIT_0_ADC , .potNum = UNIT_0_POT , .pos = 50, .posIdx = 0, .power = 0, .minPos = MIN_POT_VALUE, .maxPos = MAX_POT_VALUE, .oFirstSetupDone = 0}
 ,{.id = 1 , .adcNum = UNIT_1_ADC , .potNum = UNIT_1_POT , .pos = 50, .posIdx = 0, .power = 0, .minPos = MIN_POT_VALUE, .maxPos = MAX_POT_VALUE, .oFirstSetupDone = 0}
 ,{.id = 2 , .adcNum = UNIT_2_ADC , .potNum = UNIT_2_POT , .pos = 50, .posIdx = 0, .power = 0, .minPos = MIN_POT_VALUE, .maxPos = MAX_POT_VALUE, .oFirstSetupDone = 0}
 ,{.id = 3 , .adcNum = UNIT_3_ADC , .potNum = UNIT_3_POT , .pos = 50, .posIdx = 0, .power = 0, .minPos = MIN_POT_VALUE, .maxPos = MAX_POT_VALUE, .oFirstSetupDone = 0}
 ,{.id = 4 , .adcNum = UNIT_4_ADC , .potNum = UNIT_4_POT , .pos = 50, .posIdx = 0, .power = 0, .minPos = MIN_POT_VALUE, .maxPos = MAX_POT_VALUE, .oFirstSetupDone = 0}
 ,{.id = 5 , .adcNum = UNIT_5_ADC , .potNum = UNIT_5_POT , .pos = 50, .posIdx = 0, .power = 0, .minPos = MIN_POT_VALUE, .maxPos = MAX_POT_VALUE, .oFirstSetupDone = 0}
 ,{.id = 6 , .adcNum = UNIT_6_ADC , .potNum = UNIT_6_POT , .pos = 50, .posIdx = 0, .power = 0, .minPos = MIN_POT_VALUE, .maxPos = MAX_POT_VALUE, .oFirstSetupDone = 0}
 ,{.id = 7 , .adcNum = UNIT_7_ADC , .potNum = UNIT_7_POT , .pos = 50, .posIdx = 0, .power = 0, .minPos = MIN_POT_VALUE, .maxPos = MAX_POT_VALUE, .oFirstSetupDone = 0}
 ,{.id = 8 , .adcNum = UNIT_8_ADC , .potNum = UNIT_8_POT , .pos = 50, .posIdx = 0, .power = 0, .minPos = MIN_POT_VALUE, .maxPos = MAX_POT_VALUE, .oFirstSetupDone = 0}
 ,{.id = 9 , .adcNum = UNIT_9_ADC , .potNum = UNIT_9_POT , .pos = 50, .posIdx = 0, .power = 0, .minPos = MIN_POT_VALUE, .maxPos = MAX_POT_VALUE, .oFirstSetupDone = 0}
 ,{.id = 10, .adcNum = UNIT_10_ADC, .potNum = UNIT_10_POT, .pos = 50, .posIdx = 0, .power = 0, .minPos = MIN_POT_VALUE, .maxPos = MAX_POT_VALUE, .oFirstSetupDone = 0}
 ,{.id = 11, .adcNum = UNIT_11_ADC, .potNum = UNIT_11_POT, .pos = 50, .posIdx = 0, .power = 0, .minPos = MIN_POT_VALUE, .maxPos = MAX_POT_VALUE, .oFirstSetupDone = 0}
 ,{.id = 12, .adcNum = UNIT_12_ADC, .potNum = UNIT_12_POT, .pos = 50, .posIdx = 0, .power = 0, .minPos = MIN_POT_VALUE, .maxPos = MAX_POT_VALUE, .oFirstSetupDone = 0}
 ,{.id = 13, .adcNum = UNIT_13_ADC, .potNum = UNIT_13_POT, .pos = 50, .posIdx = 0, .power = 0, .minPos = MIN_POT_VALUE, .maxPos = MAX_POT_VALUE, .oFirstSetupDone = 0}
 ,{.id = 14, .adcNum = UNIT_14_ADC, .potNum = UNIT_14_POT, .pos = 50, .posIdx = 0, .power = 0, .minPos = MIN_POT_VALUE, .maxPos = MAX_POT_VALUE, .oFirstSetupDone = 0}
};

const UnitInterface_t _unit0_if = 
{
  .ctx          = (void *)                &_units[0]
 ,.GetId        = (UnitGetId_fct)         &_Unit_GetId 
 ,.GetPos       = (UnitGetPos_fct)        &_Unit_GetPos 
 ,.GetPower     = (UnitGetPower_fct)      &_Unit_GetPower 
 ,.Init         = (UnitInit_fct)          &_Unit_Init 
 ,.SetPos       = (UnitSetPos_fct)        &_Unit_SetPos
 ,.GetPosLimits = (UnitGetPosLimits_fct)  &_Unit_GetPosLimits
 ,.SetPower     = (UnitSetPower_fct)      &_Unit_SetPower
 ,.GetPosIdx    = (UnitGetPosIdx_fct)     &_Unit_GetPosIdx
};
const UnitInterface_t _unit1_if = 
{
  .ctx          = (void *)                &_units[1]
 ,.GetId        = (UnitGetId_fct)         &_Unit_GetId 
 ,.GetPos       = (UnitGetPos_fct)        &_Unit_GetPos 
 ,.GetPower     = (UnitGetPower_fct)      &_Unit_GetPower 
 ,.Init         = (UnitInit_fct)          &_Unit_Init 
 ,.SetPos       = (UnitSetPos_fct)        &_Unit_SetPos
 ,.GetPosLimits = (UnitGetPosLimits_fct)  &_Unit_GetPosLimits
 ,.SetPower     = (UnitSetPower_fct)      &_Unit_SetPower
 ,.GetPosIdx    = (UnitGetPosIdx_fct)     &_Unit_GetPosIdx
};
const UnitInterface_t _unit2_if = 
{
  .ctx          = (void *)                &_units[2]
 ,.GetId        = (UnitGetId_fct)         &_Unit_GetId 
 ,.GetPos       = (UnitGetPos_fct)        &_Unit_GetPos 
 ,.GetPower     = (UnitGetPower_fct)      &_Unit_GetPower 
 ,.Init         = (UnitInit_fct)          &_Unit_Init 
 ,.SetPos       = (UnitSetPos_fct)        &_Unit_SetPos
 ,.GetPosLimits = (UnitGetPosLimits_fct)  &_Unit_GetPosLimits
 ,.SetPower     = (UnitSetPower_fct)      &_Unit_SetPower
 ,.GetPosIdx    = (UnitGetPosIdx_fct)     &_Unit_GetPosIdx
};
const UnitInterface_t _unit3_if = 
{
  .ctx          = (void *)                &_units[3]
 ,.GetId        = (UnitGetId_fct)         &_Unit_GetId 
 ,.GetPos       = (UnitGetPos_fct)        &_Unit_GetPos 
 ,.GetPower     = (UnitGetPower_fct)      &_Unit_GetPower 
 ,.Init         = (UnitInit_fct)          &_Unit_Init 
 ,.SetPos       = (UnitSetPos_fct)        &_Unit_SetPos
 ,.GetPosLimits = (UnitGetPosLimits_fct)  &_Unit_GetPosLimits
 ,.SetPower     = (UnitSetPower_fct)      &_Unit_SetPower
 ,.GetPosIdx    = (UnitGetPosIdx_fct)     &_Unit_GetPosIdx
};
const UnitInterface_t _unit4_if = 
{
  .ctx          = (void *)                &_units[4]
 ,.GetId        = (UnitGetId_fct)         &_Unit_GetId 
 ,.GetPos       = (UnitGetPos_fct)        &_Unit_GetPos 
 ,.GetPower     = (UnitGetPower_fct)      &_Unit_GetPower 
 ,.Init         = (UnitInit_fct)          &_Unit_Init 
 ,.SetPos       = (UnitSetPos_fct)        &_Unit_SetPos
 ,.GetPosLimits = (UnitGetPosLimits_fct)  &_Unit_GetPosLimits
 ,.SetPower     = (UnitSetPower_fct)      &_Unit_SetPower
 ,.GetPosIdx    = (UnitGetPosIdx_fct)     &_Unit_GetPosIdx
};
const UnitInterface_t _unit5_if = 
{
  .ctx          = (void *)                &_units[5]
 ,.GetId        = (UnitGetId_fct)         &_Unit_GetId 
 ,.GetPos       = (UnitGetPos_fct)        &_Unit_GetPos 
 ,.GetPower     = (UnitGetPower_fct)      &_Unit_GetPower 
 ,.Init         = (UnitInit_fct)          &_Unit_Init 
 ,.SetPos       = (UnitSetPos_fct)        &_Unit_SetPos
 ,.GetPosLimits = (UnitGetPosLimits_fct)  &_Unit_GetPosLimits
 ,.SetPower     = (UnitSetPower_fct)      &_Unit_SetPower
 ,.GetPosIdx    = (UnitGetPosIdx_fct)     &_Unit_GetPosIdx
};
const UnitInterface_t _unit6_if = 
{
  .ctx          = (void *)                &_units[6]
 ,.GetId        = (UnitGetId_fct)         &_Unit_GetId 
 ,.GetPos       = (UnitGetPos_fct)        &_Unit_GetPos 
 ,.GetPower     = (UnitGetPower_fct)      &_Unit_GetPower 
 ,.Init         = (UnitInit_fct)          &_Unit_Init 
 ,.SetPos       = (UnitSetPos_fct)        &_Unit_SetPos
 ,.GetPosLimits = (UnitGetPosLimits_fct)  &_Unit_GetPosLimits
 ,.SetPower     = (UnitSetPower_fct)      &_Unit_SetPower
 ,.GetPosIdx    = (UnitGetPosIdx_fct)     &_Unit_GetPosIdx
};
const UnitInterface_t _unit7_if = 
{
  .ctx          = (void *)                &_units[7]
 ,.GetId        = (UnitGetId_fct)         &_Unit_GetId 
 ,.GetPos       = (UnitGetPos_fct)        &_Unit_GetPos 
 ,.GetPower     = (UnitGetPower_fct)      &_Unit_GetPower 
 ,.Init         = (UnitInit_fct)          &_Unit_Init 
 ,.SetPos       = (UnitSetPos_fct)        &_Unit_SetPos
 ,.GetPosLimits = (UnitGetPosLimits_fct)  &_Unit_GetPosLimits
 ,.SetPower     = (UnitSetPower_fct)      &_Unit_SetPower
 ,.GetPosIdx    = (UnitGetPosIdx_fct)     &_Unit_GetPosIdx
};
const UnitInterface_t _unit8_if = 
{
  .ctx          = (void *)                &_units[8]
 ,.GetId        = (UnitGetId_fct)         &_Unit_GetId 
 ,.GetPos       = (UnitGetPos_fct)        &_Unit_GetPos 
 ,.GetPower     = (UnitGetPower_fct)      &_Unit_GetPower 
 ,.Init         = (UnitInit_fct)          &_Unit_Init 
 ,.SetPos       = (UnitSetPos_fct)        &_Unit_SetPos
 ,.GetPosLimits = (UnitGetPosLimits_fct)  &_Unit_GetPosLimits
 ,.SetPower     = (UnitSetPower_fct)      &_Unit_SetPower
 ,.GetPosIdx    = (UnitGetPosIdx_fct)     &_Unit_GetPosIdx
};
const UnitInterface_t _unit9_if = 
{
  .ctx          = (void *)                &_units[9]
 ,.GetId        = (UnitGetId_fct)         &_Unit_GetId 
 ,.GetPos       = (UnitGetPos_fct)        &_Unit_GetPos 
 ,.GetPower     = (UnitGetPower_fct)      &_Unit_GetPower 
 ,.Init         = (UnitInit_fct)          &_Unit_Init 
 ,.SetPos       = (UnitSetPos_fct)        &_Unit_SetPos
 ,.GetPosLimits = (UnitGetPosLimits_fct)  &_Unit_GetPosLimits
 ,.SetPower     = (UnitSetPower_fct)      &_Unit_SetPower
 ,.GetPosIdx    = (UnitGetPosIdx_fct)     &_Unit_GetPosIdx
};
const UnitInterface_t _unit10_if = 
{
  .ctx          = (void *)                &_units[10]
 ,.GetId        = (UnitGetId_fct)         &_Unit_GetId 
 ,.GetPos       = (UnitGetPos_fct)        &_Unit_GetPos 
 ,.GetPower     = (UnitGetPower_fct)      &_Unit_GetPower 
 ,.Init         = (UnitInit_fct)          &_Unit_Init 
 ,.SetPos       = (UnitSetPos_fct)        &_Unit_SetPos
 ,.GetPosLimits = (UnitGetPosLimits_fct)  &_Unit_GetPosLimits
 ,.SetPower     = (UnitSetPower_fct)      &_Unit_SetPower
 ,.GetPosIdx    = (UnitGetPosIdx_fct)     &_Unit_GetPosIdx
};
const UnitInterface_t _unit11_if = 
{
  .ctx          = (void *)                &_units[11]
 ,.GetId        = (UnitGetId_fct)         &_Unit_GetId 
 ,.GetPos       = (UnitGetPos_fct)        &_Unit_GetPos 
 ,.GetPower     = (UnitGetPower_fct)      &_Unit_GetPower 
 ,.Init         = (UnitInit_fct)          &_Unit_Init 
 ,.SetPos       = (UnitSetPos_fct)        &_Unit_SetPos
 ,.GetPosLimits = (UnitGetPosLimits_fct)  &_Unit_GetPosLimits
 ,.SetPower     = (UnitSetPower_fct)      &_Unit_SetPower
 ,.GetPosIdx    = (UnitGetPosIdx_fct)     &_Unit_GetPosIdx
};
const UnitInterface_t _unit12_if = 
{
  .ctx          = (void *)                &_units[12]
 ,.GetId        = (UnitGetId_fct)         &_Unit_GetId 
 ,.GetPos       = (UnitGetPos_fct)        &_Unit_GetPos 
 ,.GetPower     = (UnitGetPower_fct)      &_Unit_GetPower 
 ,.Init         = (UnitInit_fct)          &_Unit_Init 
 ,.SetPos       = (UnitSetPos_fct)        &_Unit_SetPos
 ,.GetPosLimits = (UnitGetPosLimits_fct)  &_Unit_GetPosLimits
 ,.SetPower     = (UnitSetPower_fct)      &_Unit_SetPower
 ,.GetPosIdx    = (UnitGetPosIdx_fct)     &_Unit_GetPosIdx
};
const UnitInterface_t _unit13_if = 
{
  .ctx          = (void *)                &_units[13]
 ,.GetId        = (UnitGetId_fct)         &_Unit_GetId 
 ,.GetPos       = (UnitGetPos_fct)        &_Unit_GetPos 
 ,.GetPower     = (UnitGetPower_fct)      &_Unit_GetPower 
 ,.Init         = (UnitInit_fct)          &_Unit_Init 
 ,.SetPos       = (UnitSetPos_fct)        &_Unit_SetPos
 ,.GetPosLimits = (UnitGetPosLimits_fct)  &_Unit_GetPosLimits
 ,.SetPower     = (UnitSetPower_fct)      &_Unit_SetPower
 ,.GetPosIdx    = (UnitGetPosIdx_fct)     &_Unit_GetPosIdx
};
const UnitInterface_t _unit14_if = 
{
  .ctx          = (void *)                &_units[14]
 ,.GetId        = (UnitGetId_fct)         &_Unit_GetId 
 ,.GetPos       = (UnitGetPos_fct)        &_Unit_GetPos 
 ,.GetPower     = (UnitGetPower_fct)      &_Unit_GetPower 
 ,.Init         = (UnitInit_fct)          &_Unit_Init 
 ,.SetPos       = (UnitSetPos_fct)        &_Unit_SetPos
 ,.GetPosLimits = (UnitGetPosLimits_fct)  &_Unit_GetPosLimits
 ,.SetPower     = (UnitSetPower_fct)      &_Unit_SetPower
 ,.GetPosIdx    = (UnitGetPosIdx_fct)     &_Unit_GetPosIdx
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
 ,&_unit8_if
 ,&_unit9_if
 ,&_unit10_if
 ,&_unit11_if
 ,&_unit12_if
 ,&_unit13_if
 ,&_unit14_if
};

// Private functions
//==============================================================================

void _Unit_Init (Unit_t *unit)
{
  if (!unit->oFirstSetupDone)
  {
    unit->oFirstSetupDone = 1;
    SetPot(unit->potNum, 0);
    unit->pos = MIN_POT_VALUE;
    unit->posIdx = 0;
  }
}


void _Unit_SetPower (Unit_t *unit, float power)
{
  unit->power = power;
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