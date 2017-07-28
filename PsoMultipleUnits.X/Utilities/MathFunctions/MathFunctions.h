//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Utilities
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : LinkedList.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements different math functions.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __MATH_FUNCTIONS_H__
#define __MATH_FUNCTIONS_H__

#include "Setup.h"

// Public definitions
//==============================================================================

/*
 * Structure used for the Tustin discrete integrators
 */
typedef struct
{
  float  previousValue
        ,currentValue
        ;
} TustinValue_t;

typedef struct
{
  float  current
        ,previous
        ,oldest
        ;
} TustinValue2_t;

typedef struct
{
  UINT32 current
        ,previous
        ,oldest
        ;
} TustinValue32_t;

// Public functions
//==============================================================================

#define MAX(_x, _y)   ( ((_x) > (_y)) ? (_x) :  (_y) )
#define MIN(_x, _y)   ( ((_x) < (_y)) ? (_x) :  (_y) )
#define ABS(_x)       ( ((_x) >=  0 ) ? (_x) : -(_x) )

void TustinZ  (TustinValue_t *input, TustinValue_t *output, float acqTime);
void HpfZ     (TustinValue_t *input, TustinValue_t *output, float acqTime, float wh);
void LpfZ     (TustinValue_t *input, TustinValue_t *output, float acqTime, float wl);
void NpfZ     (TustinValue2_t *input, TustinValue2_t *output, float acqTime, float wn);
//void NpfZ32   (TustinValue32_t *input, TustinValue32_t *output, UINT32 acqTimeInUs, UINT32 wn);
inline INT16 NpfZ32 (INT16 uk0, INT16 uk1, INT16 uk2, INT16 yk1, INT16 yk2);
void NpfZ32Static (TustinValue32_t *input, TustinValue32_t *output);
void NpfZ32StaticOptimized (TustinValue32_t *input, TustinValue32_t *output);

#endif // __MATH_FUNCTIONS_H__