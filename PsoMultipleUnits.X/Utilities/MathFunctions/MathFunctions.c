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


///*  
// * Second order Low Pass Filter
// *
// *  Y(s)          wl^2
// * ------ == --------------
// *  U(s)       (s + wl)^2
// */
//INT16 Lpf2Z (INT16 uk0, INT16 uk1, INT16 uk2, INT16 yk1, INT16 yk2)
//{
//  
//}


//const INT32 _wl = 189;
//const INT32 _fTimes2 = 13333;
//const INT16 _fTimes2PlusWl = 13522;
const INT32 _wl = 400;
const INT32 _fTimes2 = 13333;
const INT16 _fTimes2PlusWl = 13333 + 400;
/*  
 * First order Low Pass Filter
 *
 *  Y(s)         wl
 * ------ == ----------
 *  U(s)       s + wl
 * 
 * y(k) = [ (u(k) + u(k-1))*w*T - y(k-1)*(w*T-2) ] / (w*T + 2)
 */
INT16 LpfZStatic (INT16 uk0, INT16 uk1, INT16 yk1)
{
  return ( (uk0+uk1-yk1)*_wl + _fTimes2*yk1 ) / _fTimes2PlusWl;
}

//const INT64 __divider = 4*f^(2)+4*f*wl+wl^(2);
//const INT64 __f2Times4 = 177777778;
//const INT64 __wl = 400;
//const INT64 __fwlTimes4 = 10666667;
//const INT64 __wl2 = 160000;
//const INT64 __divider = 188604444;

//const INT64 __wl = 500;
//const INT64 __fwlTimes4 = 13333333;
//const INT64 __wl2 = 250000;
//const INT64 __divider = 191361111;

//const INT64 __wl = 650;
//const INT64 __fwlTimes4 = 17333333;
//const INT64 __wl2 = 422500;
//const INT64 __divider = 195533611;

//const INT64 __wl = 800;
//const INT64 __fwlTimes4 = 21333333;
//const INT64 __wl2 = 640000;
//const INT64 __divider = 199751111;

//const INT64 __f2Times4 = 177777778;
//const INT64 __wl = 1200;
//const INT64 __fwlTimes4 = 32000000;
//const INT64 __wl2 = 1440000;
//const INT64 __divider = 211217778;

const INT64 __f2Times4 = 17777777778;
const INT64 __wl = 628;
const INT64 __fwlTimes4 = 167466667;
const INT64 __wl2 = 394384;
//const INT64 __divider = 4*f^(2)+4*f*wl+wl^(2);
const INT64 __divider = 17945638828;

INT16 Lpf2ZStatic (INT64 uk0, INT64 uk1, INT64 uk2, INT64 yk1, INT64 yk2)
{
  INT64 coeff1 = 2*yk1;
  return (__f2Times4*(coeff1-yk2)+__fwlTimes4*yk2+(uk0+2*uk1+uk2-coeff1-yk2)*__wl2)/__divider;
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

//void NpfZ32 (TustinValue32_t *input, TustinValue32_t *output, UINT32 acqTimeInUs, UINT32 wn)
//{
//  output->oldest = output->previous;
//  output->previous = output->current;
//  
//  UINT64 wn2 = wn*wn;
//  UINT64 wnT = wn*acqTimeInUs;
//  UINT64 wn2T2 = wn2*acqTimeInUs*acqTimeInUs;
//  UINT64 uk0 = input->current;
//  UINT64 uk1 = input->previous;
//  UINT64 uk2 = input->oldest;
//  UINT64 yk1 = output->previous;
//  UINT64 yk2 = output->oldest;
////  output->current = ((wn2T2+4)*uk0+(2*wn2T2-8)*uk1+(wn2T2+4)*uk2-(2*wn2T2-8)*yk1-(wn2T2-4*wnT+4)*yk2)/(wn2T2+4*wnT+4);
////  output->current /= 1000000;
//  
//  output->current = (((uk0+2*uk1+uk2-2*yk1-yk2)*wn2T2/1000000+4*wnT*yk2/1000000+4*(uk0-2*uk1+uk2+2*yk1-yk2))/((wn2T2+4*wnT)/1000000+4));
//}

#define _FREQ (1.0f / (ADC_TIMER_SCALE_FLOAT * (float) ADC_TIMER_PERIOD * (float) N_UNITS_TOTAL))
#define _WN   (2.0f*PI*1526)

//const UINT32 _wn = 1526*2*PI;
const UINT32 _wn = _WN + 0.5f;
const UINT32 _T = ADC_TIMER_PERIOD * N_UNITS_TOTAL;
//const UINT32 _f = (1.0f / (ADC_TIMER_SCALE_FLOAT * ADC_TIMER_PERIOD * N_UNITS_TOTAL)) + 0.5f;
const UINT32 _f = _FREQ;
//const UINT64 _fwn = ((1.0f / (ADC_TIMER_SCALE_FLOAT * ADC_TIMER_PERIOD * N_UNITS_TOTAL)) + 0.5f) * (1526.0f*2.0f*PI);
const UINT64 _fwn = _FREQ*_WN;
//const INT64 _fwnTimes4 = 4*(((1.0f / (ADC_TIMER_SCALE_FLOAT * ADC_TIMER_PERIOD * N_UNITS_TOTAL)) + 0.5f) * (1526.0f*2.0f*PI));
//const INT64 _fwnTimes4 = _FREQ*_WN*4.0f + 0.5f;
//const INT64 _wn2 = (1526.0f*1526.0f*4.0f*PI);
//const INT64 _wn2 = _WN*_WN + 0.5f;
const UINT64 _f2 = ((UINT32) ((1.0f / (ADC_TIMER_SCALE_FLOAT * ADC_TIMER_PERIOD * N_UNITS_TOTAL)) + 0.5f)) * ((UINT32) ((1.0f / (ADC_TIMER_SCALE_FLOAT * ADC_TIMER_PERIOD * N_UNITS_TOTAL)) + 0.5f));
//const INT64 _f2Times4 = 4*(((UINT32) ((1.0f / (ADC_TIMER_SCALE_FLOAT * ADC_TIMER_PERIOD * N_UNITS_TOTAL)) + 0.5f)) * ((UINT32) ((1.0f / (ADC_TIMER_SCALE_FLOAT * ADC_TIMER_PERIOD * N_UNITS_TOTAL)) + 0.5f)));
//const INT64 _f2Times4 = _FREQ*_FREQ*4.0f + 0.5f;
const UINT32 _wnT = (1526*2*PI)*(ADC_TIMER_PERIOD * N_UNITS_TOTAL);
const UINT64 _wn2T2 = (1526*2*PI)*(1526*2*PI)*(ADC_TIMER_PERIOD * N_UNITS_TOTAL)*(ADC_TIMER_PERIOD * N_UNITS_TOTAL);


const double _fDivider = 4.0 / (0.00015*00015) + PI*8.0*1526.0/(0.00015) + PI*PI*4.0*1526.0*1526.0 + 0.5;
const double _fFwnTimes4 = 8.0*PI*1526.0/0.00015 + 0.5;
const double _fF2Times4 = 4.0 / (0.00015*0.00015) + 0.5;
const double _fWn2 = 4.0*PI*PI*1526.0*1526.0 + 0.5;

const INT64 _fwnTimes4 = 255683754;
const INT64 _wn2 = 91932444;
const INT64 _f2Times4 = 177777778;
const INT32 _divider = 525393975;
//const INT64 _divider = _FREQ*_FREQ*4.0f + _FREQ*_WN*4.0f + _WN*_WN + 0.5f;

// Measured with logic analyzer
//const INT64 _fwnTimes4 = 268414358;
//const INT64 _wn2 = 103351497;
//const INT64 _f2Times4 = 174274853;
//const INT32 _divider = 546040709;

void NpfZ32Static (TustinValue32_t *input, TustinValue32_t *output)
{
  output->oldest = output->previous;
  output->previous = output->current;
  
  UINT16 uk0 = input->current;
  UINT16 uk1 = input->previous;
  UINT16 uk2 = input->oldest;
  UINT16 yk1 = output->previous;
  UINT16 yk2 = output->oldest;
//  output->current = ((wn2T2+4)*uk0+(2*wn2T2-8)*uk1+(wn2T2+4)*uk2-(2*wn2T2-8)*yk1-(wn2T2-4*wnT+4)*yk2)/(wn2T2+4*wnT+4);
//  output->current /= 1000000;
  
  output->current = (((uk0+2*uk1+uk2-2*yk1-yk2)*_wn2T2/1000000+4*_wnT*yk2/1000000+4*(uk0-2*uk1+uk2+2*yk1-yk2))/((_wn2T2+4*_wnT)/1000000+4));
}

inline void NpfZ32StaticOptimized (TustinValue32_t *input, TustinValue32_t *output)
{
  output->oldest = output->previous;
  output->previous = output->current;
  
  INT16 uk0 = input->current;
  INT16 uk1 = input->previous;
  INT16 uk2 = input->oldest;
  INT16 yk1 = output->previous;
  INT16 yk2 = output->oldest;
  
//  output->current = ((uk0+2*uk1+uk2-2*yk1-yk2)*_wn2T2/1000000+4*_wnT*yk2/1000000+4*(uk0-2*uk1+uk2+2*yk1-yk2))
//                    /((_wn2T2+4*_wnT)/1000000+4);
  
//  output->current = ((_f2Times4*(uk0-2*uk1+uk2+2*yk1-yk2)+4*_fwn*yk2+(uk0+2*uk1+uk2-2*yk1-yk2)*_wn2)/(_f2Times4+_fwnTimes4+_wn2));
  
  INT16 coeff1 = 2*uk1; // max 2046, min 0
  INT16 coeff2 = 2*yk1; // max 2046, min 0
  INT16 add1 = uk2-yk2 + uk0; // max 2046
  INT16 add2 = coeff1 - coeff2; // max 2046, min -1023
  output->current = (  (_f2Times4*(add1 - add2))  // max 5.4560e+11, min 0
                     + (_fwnTimes4*yk2) // max 2.6153e+11, min 0
                     + ( (add1 + add2) * _wn2) // max 1.8809e+11, min 0
                    ) // max 1.1833e+12, min 0
                    /_divider;
//                    / (_f2Times4 + _fwnTimes4 + _wn2);  // fix at 525364651

  // total max 2.2524e+03, min 0
}


INT16 NpfZ32 (INT16 uk0, INT16 uk1, INT16 uk2, INT16 yk1, INT16 yk2)
{
  INT16 coeff1 = 2*uk1; // max 2046, min 0
  INT16 coeff2 = 2*yk1; // max 2046, min 0
  INT16 add1 = uk2-yk2 + uk0; // max 2046
  INT16 add2 = coeff1 - coeff2; // max 2046, min -1023
  return (  (_f2Times4*(add1 - add2))  // max 5.4560e+11, min 0
                     + (_fwnTimes4*yk2) // max 2.6153e+11, min 0
                     + ( (add1 + add2) * _wn2) // max 1.8809e+11, min 0
                    ) // max 1.1833e+12, min 0
                    /_divider;
//                    / (_f2Times4 + _fwnTimes4 + _wn2);  // fix at 525364651

  // total max 2.2524e+03, min 0
}