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
 ,UNIT_12_ADC
 ,UNIT_13_ADC
 ,UNIT_14_ADC
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
 ,UNIT_12_PWM
 ,UNIT_13_PWM
 ,UNIT_14_PWM
};