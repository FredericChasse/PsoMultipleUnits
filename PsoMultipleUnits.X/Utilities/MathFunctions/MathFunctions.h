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

// Public functions
//==============================================================================

#define MAX(_x, _y)   ( ((_x) > (_y)) ? (_x) :  (_y) )
#define MIN(_x, _y)   ( ((_x) < (_y)) ? (_x) :  (_y) )
#define ABS(_x)       ( ((_x) >=  0 ) ? (_x) : -(_x) )

void TustinZ  (TustinValue_t *input, TustinValue_t *output, float acqTime);
void HpfZ     (TustinValue_t *input, TustinValue_t *output, float acqTime, float wh);
void LpfZ     (TustinValue_t *input, TustinValue_t *output, float acqTime, float wl);

#endif // __MATH_FUNCTIONS_H__