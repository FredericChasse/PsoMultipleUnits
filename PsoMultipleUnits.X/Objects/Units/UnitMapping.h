//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : UnitMapping.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file describes the link between ADC, LED driver and units.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __UNIT_MAPPING_H__
#define __UNIT_MAPPING_H__

#include "Setup.h"


extern const UINT8 unitAdcs[N_UNITS_TOTAL];

#define UNIT_0_ADC    (1)
#define UNIT_1_ADC    (2)
#define UNIT_2_ADC    (3)
#define UNIT_3_ADC    (4)
#define UNIT_4_ADC    (5)
#define UNIT_5_ADC    (6)
#define UNIT_6_ADC    (7)
#define UNIT_7_ADC    (8)
#define UNIT_8_ADC    (9)
#define UNIT_9_ADC    (10)
#define UNIT_10_ADC   (11)
#define UNIT_11_ADC   (12)
#define UNIT_12_ADC   (13)
#define UNIT_13_ADC   (14)
#define UNIT_14_ADC   (15)

#define UNIT_0_POT    (1)
#define UNIT_1_POT    (2)
#define UNIT_2_POT    (3)
#define UNIT_3_POT    (4)
#define UNIT_4_POT    (5)
#define UNIT_5_POT    (6)
#define UNIT_6_POT    (7)
#define UNIT_7_POT    (8)
#define UNIT_8_POT    (9)
#define UNIT_9_POT    (10)
#define UNIT_10_POT   (11)
#define UNIT_11_POT   (12)
#define UNIT_12_POT   (13)
#define UNIT_13_POT   (14)
#define UNIT_14_POT   (15)

extern const UINT8 unitsPwms[N_UNITS_TOTAL];

#define UNIT_0_PWM    (1)
#define UNIT_1_PWM    (2)
#define UNIT_2_PWM    (3)
#define UNIT_3_PWM    (4)
#define UNIT_4_PWM    (5)
#define UNIT_5_PWM    (6)
#define UNIT_6_PWM    (7)
#define UNIT_7_PWM    (8)
#define UNIT_8_PWM    (9)
#define UNIT_9_PWM    (10)
#define UNIT_10_PWM   (11)
#define UNIT_11_PWM   (12)
#define UNIT_12_PWM   (13)
#define UNIT_13_PWM   (14)
#define UNIT_14_PWM   (15)


#endif // __UNIT_MAPPING_H__