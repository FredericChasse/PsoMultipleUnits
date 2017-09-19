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
#include "Potentiometer.h"


#define UNIT_0_ADC    (2)
#define UNIT_1_ADC    (3)
#define UNIT_2_ADC    (4)
#define UNIT_3_ADC    (5)
#define UNIT_4_ADC    (8)
#define UNIT_5_ADC    (9)
#define UNIT_6_ADC    (10)
#define UNIT_7_ADC    (11)
#define UNIT_8_ADC    (12)
#define UNIT_9_ADC    (13)
#define UNIT_10_ADC   (14)
#define UNIT_11_ADC   (15)

extern const UINT8 unitAdcs[N_UNITS_TOTAL];

#define UNIT_0_POT    (2)
#define UNIT_1_POT    (3)
#define UNIT_2_POT    (4)
#define UNIT_3_POT    (5)
#define UNIT_4_POT    (8)
#define UNIT_5_POT    (9)
#define UNIT_6_POT    (10)
#define UNIT_7_POT    (11)
#define UNIT_8_POT    (12)
#define UNIT_9_POT    (13)
#define UNIT_10_POT   (14)
#define UNIT_11_POT   (15)

extern const UINT8 unitPots[N_UNITS_TOTAL];

#define UNIT_0_PWM    (2)
#define UNIT_1_PWM    (3)
#define UNIT_2_PWM    (4)
#define UNIT_3_PWM    (5)
#define UNIT_4_PWM    (8)
#define UNIT_5_PWM    (9)
#define UNIT_6_PWM    (10)
#define UNIT_7_PWM    (11)
#define UNIT_8_PWM    (12)
#define UNIT_9_PWM    (13)
#define UNIT_10_PWM   (14)
#define UNIT_11_PWM   (15)

extern const UINT8 unitsPwms[N_UNITS_TOTAL];


#define GLOBAL_MIN_POS_IDX    (POT_MIN_INDEX)
#define UNIT_0_MIN_POS_IDX    (58)
#define UNIT_1_MIN_POS_IDX    (32)
#define UNIT_2_MIN_POS_IDX    (32)
#define UNIT_3_MIN_POS_IDX    (58)
#define UNIT_4_MIN_POS_IDX    (GLOBAL_MIN_POS_IDX)
#define UNIT_5_MIN_POS_IDX    (GLOBAL_MIN_POS_IDX)
#define UNIT_6_MIN_POS_IDX    (GLOBAL_MIN_POS_IDX)
#define UNIT_7_MIN_POS_IDX    (GLOBAL_MIN_POS_IDX)
#define UNIT_8_MIN_POS_IDX    (GLOBAL_MIN_POS_IDX)
#define UNIT_9_MIN_POS_IDX    (GLOBAL_MIN_POS_IDX)
#define UNIT_10_MIN_POS_IDX   (GLOBAL_MIN_POS_IDX)
#define UNIT_11_MIN_POS_IDX   (GLOBAL_MIN_POS_IDX)

extern const UINT8 unitsMinPosIdx[N_UNITS_TOTAL];


#define GLOBAL_MIN_POS        (MIN_POT_VALUE)
#define UNIT_0_MIN_POS        (277.4510f)
#define UNIT_1_MIN_POS        (175.4902f)
#define UNIT_2_MIN_POS        (175.4902f)
#define UNIT_3_MIN_POS        (277.4510f)
#define UNIT_4_MIN_POS        (GLOBAL_MIN_POS)
#define UNIT_5_MIN_POS        (GLOBAL_MIN_POS)
#define UNIT_6_MIN_POS        (GLOBAL_MIN_POS)
#define UNIT_7_MIN_POS        (GLOBAL_MIN_POS)
#define UNIT_8_MIN_POS        (GLOBAL_MIN_POS)
#define UNIT_9_MIN_POS        (GLOBAL_MIN_POS)
#define UNIT_10_MIN_POS       (GLOBAL_MIN_POS)
#define UNIT_11_MIN_POS       (GLOBAL_MIN_POS)

extern const float unitsMinPos[N_UNITS_TOTAL];


#define UNIT_0_OPT_POS_IDX    (82)    /* 371.5686 */
#define UNIT_1_OPT_POS_IDX    (36)    /* 191.1765 */
#define UNIT_2_OPT_POS_IDX    (49)    /* 242.1569 */
#define UNIT_3_OPT_POS_IDX    (80)    /* 363.7255 */
#define UNIT_4_OPT_POS_IDX    (100)
#define UNIT_5_OPT_POS_IDX    (100)
#define UNIT_6_OPT_POS_IDX    (100)
#define UNIT_7_OPT_POS_IDX    (100)
#define UNIT_8_OPT_POS_IDX    (100)
#define UNIT_9_OPT_POS_IDX    (100)
#define UNIT_10_OPT_POS_IDX   (100)
#define UNIT_11_OPT_POS_IDX   (100)

extern const UINT8 unitsOptPosIdx[N_UNITS_TOTAL];


#define GLOBAL_MAX_POS_IDX    (POT_MAX_INDEX)
#define UNIT_0_MAX_POS_IDX    (141)   /* 602.9412 */
#define UNIT_1_MAX_POS_IDX    (141)   /* 602.9412 */
#define UNIT_2_MAX_POS_IDX    (141)   /* 602.9412 */
#define UNIT_3_MAX_POS_IDX    (141)   /* 602.9412 */
#define UNIT_4_MAX_POS_IDX    (GLOBAL_MAX_POS_IDX)
#define UNIT_5_MAX_POS_IDX    (GLOBAL_MAX_POS_IDX)
#define UNIT_6_MAX_POS_IDX    (GLOBAL_MAX_POS_IDX)
#define UNIT_7_MAX_POS_IDX    (GLOBAL_MAX_POS_IDX)
#define UNIT_8_MAX_POS_IDX    (GLOBAL_MAX_POS_IDX)
#define UNIT_9_MAX_POS_IDX    (GLOBAL_MAX_POS_IDX)
#define UNIT_10_MAX_POS_IDX   (GLOBAL_MAX_POS_IDX)
#define UNIT_11_MAX_POS_IDX   (GLOBAL_MAX_POS_IDX)

extern const UINT8 unitsMaxPosIdx[N_UNITS_TOTAL];


#endif // __UNIT_MAPPING_H__