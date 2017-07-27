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


/*  
 * Second order Notch Pass Filter
 *
 *  Y(s)           s^2 + wn^2
 * ------ == ----------------------
 *  U(s)       s^2 + 2*wn*s + wn^2
 */
void NpfZ (TustinValue2_t *input, TustinValue2_t *output, float acqTime, float wn)
{
  output->oldest = output->previous;
  output->previous = output->current;
  
  float wn2 = wn*wn;
  float wnT = wn*acqTime;
  float wn2T = wn2*acqTime;
  float wn2T2 = wn2T*acqTime;
  float uk0 = input->current;
  float uk1 = input->previous;
  float uk2 = input->oldest;
  float yk1 = output->previous;
  float yk2 = output->oldest;
  output->current = ((wn2T2+4)*uk0+(2*wn2T2-8)*uk1+(wn2T2+4)*uk2-(2*wn2T2-8)*yk1-(wn2T2-4*wnT+4)*yk2)/(wn2T2+4*wnT+4);
}

void NpfZ32 (TustinValue32_t *input, TustinValue32_t *output, UINT32 acqTimeInUs, UINT32 wn)
{
  output->oldest = output->previous;
  output->previous = output->current;
  
  float T = acqTimeInUs*0.000001;
  float wn2 = wn*wn;
//  float wnT = wn*acqTimeInUs;
//  float wn2T = wn2*acqTimeInUs;
//  float wn2T2 = wn2T*acqTimeInUs;
  float wnT = wn*T;
  float wn2T = wn2*T;
  float wn2T2 = wn2T*T;
  float uk0 = input->current;
  float uk1 = input->previous;
  float uk2 = input->oldest;
  float yk1 = output->previous;
  float yk2 = output->oldest;
//  output->current = ((wn2T2+4)*uk0+(2*wn2T2-8)*uk1+(wn2T2+4)*uk2-(2*wn2T2-8)*yk1-(wn2T2-4*wnT+4)*yk2)/(wn2T2+4*wnT+4);
//  output->current /= 1000000;
  
  output->current = (((uk0+2*uk1+uk2-2*yk1-yk2)*wn2T2+4*wnT*yk2+4*(uk0-2*uk1+uk2+2*yk1-yk2))/(wn2T2+4*wnT+4));
//  output->current /= 1000000;
}