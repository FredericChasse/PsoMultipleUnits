//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Utilities
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : MathFunctions.c
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

#include "MathFunctions.h"

// Public functions
//==============================================================================

/*
 * Function : TustinZ
 * Desc     : Discrete integrator using Tustin's method
 * Graphic example :
 *
 *         _______
 *  X(s)  |   1   |  Y(s)
 * ------>| ----- |------>
 *        |   s   |
 *        |_______|
 * 
 *   Y(s)      1        Y(z)     T     z + 1
 *  -----  =  ---  ==> ----- =  --- * -------
 *   X(s)      s        X(z)     2     z - 1
 *
 *  iLaplace {Z} => y(n) = y(n-1) + T/2 * ( x(n-1) + x(n) )
 *
 */
void TustinZ (TustinValue_t *input, TustinValue_t *output, float acqTime)
{
  output->previousValue = output->currentValue;
  output->currentValue  = output->previousValue + acqTime/2 * (input->currentValue + input->previousValue);
}


/*  
 * High Pass Filter
 *
 *  Y(s)         s
 * ------ == ---------
 *  U(s)       s + wh
 */
void HpfZ (TustinValue_t *input, TustinValue_t *output, float acqTime, float wh)
{
  float coeff = wh*acqTime;
  output->previousValue = output->currentValue;
  output->currentValue  = ( 2 * (input->currentValue - input->previousValue) 
                            - output->previousValue * (coeff - 2) ) 
                          / (coeff + 2);
}


/*  
 * Low Pass Filter
 *
 *  Y(s)        wl
 * ------ == ---------
 *  U(s)       s + wl
 */
void LpfZ (TustinValue_t *input, TustinValue_t *output, float acqTime, float wl)
{
  float coeff = wl*acqTime;
  output->previousValue = output->currentValue;
  output->currentValue  = ( ( input->currentValue + input->previousValue) * coeff 
                            - output->previousValue * (coeff - 2) ) 
                          / (coeff + 2);
}