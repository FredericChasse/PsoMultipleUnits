//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : UnitMapping.c
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file describes the link between ADC, LED driver and units.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "UnitMapping.h"

const UINT8 unitAdcs[N_UNITS_TOTAL] = 
{ 
  UNIT_0_ADC
 ,UNIT_1_ADC
 ,UNIT_2_ADC
 ,UNIT_3_ADC
 ,UNIT_4_ADC
 ,UNIT_5_ADC
 ,UNIT_6_ADC
 ,UNIT_7_ADC
 ,UNIT_8_ADC
 ,UNIT_9_ADC
 ,UNIT_10_ADC
 ,UNIT_11_ADC
};

const UINT8 unitPots[N_UNITS_TOTAL] = 
{ 
  UNIT_0_POT
 ,UNIT_1_POT
 ,UNIT_2_POT
 ,UNIT_3_POT
 ,UNIT_4_POT
 ,UNIT_5_POT
 ,UNIT_6_POT
 ,UNIT_7_POT
 ,UNIT_8_POT
 ,UNIT_9_POT
 ,UNIT_10_POT
 ,UNIT_11_POT
};

const UINT8 unitsPwms[N_UNITS_TOTAL] = 
{
  UNIT_0_PWM
 ,UNIT_1_PWM
 ,UNIT_2_PWM
 ,UNIT_3_PWM
 ,UNIT_4_PWM
 ,UNIT_5_PWM
 ,UNIT_6_PWM
 ,UNIT_7_PWM
 ,UNIT_8_PWM
 ,UNIT_9_PWM
 ,UNIT_10_PWM
 ,UNIT_11_PWM
};

const UINT8 unitsMinPosIdx[N_UNITS_TOTAL] = 
{
  UNIT_0_MIN_POS_IDX
 ,UNIT_1_MIN_POS_IDX
 ,UNIT_2_MIN_POS_IDX
 ,UNIT_3_MIN_POS_IDX
 ,UNIT_4_MIN_POS_IDX
 ,UNIT_5_MIN_POS_IDX
 ,UNIT_6_MIN_POS_IDX
 ,UNIT_7_MIN_POS_IDX
 ,UNIT_8_MIN_POS_IDX
 ,UNIT_9_MIN_POS_IDX
 ,UNIT_10_MIN_POS_IDX
 ,UNIT_11_MIN_POS_IDX
};

const float unitsMinPos[N_UNITS_TOTAL] = 
{
  UNIT_0_MIN_POS
 ,UNIT_1_MIN_POS
 ,UNIT_2_MIN_POS
 ,UNIT_3_MIN_POS
 ,UNIT_4_MIN_POS
 ,UNIT_5_MIN_POS
 ,UNIT_6_MIN_POS
 ,UNIT_7_MIN_POS
 ,UNIT_8_MIN_POS
 ,UNIT_9_MIN_POS
 ,UNIT_10_MIN_POS
 ,UNIT_11_MIN_POS
};