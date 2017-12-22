//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : StateFunctions.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the C file for the functions of the state machine of
//           the system.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : Function names can and should be renamed by the user to improve the
//           readability of the code.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "..\headers\StateFunctions.h"
#include "MathFunctions.h"


//==============================================================================
// Variable declarations
//==============================================================================

extern struct sAllCells sCellValues;
extern const float potRealValues[256];
extern const float potRealValuesInverse[256];
extern UINT16 cellVoltageRaw[16];
extern volatile UINT16 nSamples;
const float kFilter = 0.1;
UINT16 dbgAdcData[N_TOTAL_SAMPLES] = {0};
extern BOOL oDbgAdc;
extern BOOL oSendAdcData;

// All the buttons used. 3 on the steering wheel, 3 on the board
sButtonStates_t buttons =
{
  .chng   .byte = 0
 ,.buttons.byte = 0

 // switches on board are at 1 when not pressed
 ,.buttons.bits.sw1 = 1
 ,.buttons.bits.sw2 = 1
 ,.buttons.bits.sw3 = 1
};
 

//==============================================================================
// State Machine private functions prototypes
//==============================================================================


//==============================================================================
// Solar cells functions
//==============================================================================

inline float ComputeCellPower (UINT8 cellIndex, UINT8 potIndex)
{
  float volt = (float) sCellValues.cells[cellIndex].cellVolt_mV / 1000;
  sCellValues.cells[cellIndex].cellPowerFloat = volt * volt * potRealValuesInverse[potIndex];
//  sCellValues.cells[cellIndex].cellPowerFloat = sCellValues.cells[cellIndex].cellVoltFloat * sCellValues.cells[cellIndex].cellVoltFloat / potRealValues[potIndexValue];
//  sCellValues.cells[cellIndex].cellPowerFloat = sCellValues.cells[cellIndex].cellVoltFloat * sCellValues.cells[cellIndex].cellVoltFloat * potRealValuesInverse[potIndex];
//  sCellValues.cells[cellIndex].cellPowerFloat = (sCellValues.cells[cellIndex].cellVolt_mV * sCellValues.cells[cellIndex].cellVolt_mV) * potRealValuesInverse[potIndex];
  return sCellValues.cells[cellIndex].cellPowerFloat;
}

const UINT32 wn = 1526*2*PI;
const UINT32 T = ADC_TIMER_PERIOD * N_UNITS_TOTAL;

//TustinValue32_t in[16] = {0}, out[16] = {0};
TustinValue32_t in = {0}, out = {0};

volatile UINT16 cellVoltRaw[N_SAMPLES_PER_ADC_READ][N_UNITS_TOTAL] = {0};
volatile UINT32 cellVoltRawMean[N_UNITS_TOTAL] = {0};

extern volatile BOOL oAdcReady;
extern volatile BOOL oAcqOngoing;

inline void ComputeMeanAdcValues (void)
{
  UINT16 i = 0, j = 0, tmp = 0;
  UINT32 meanCellRaw[N_UNITS_TOTAL] = {0};
  
  UINT16 iEnd[N_UNITS_TOTAL] = {N_SAMPLES_PER_ADC_READ};
  UINT16 iStart[N_UNITS_TOTAL] = {N_SAMPLES_TO_DROP};
  UINT16 divider[N_UNITS_TOTAL];
  
  
//  for (j = 0; j < N_UNITS_TOTAL; j++)
//  {
//    meanCellRaw[j] = cellVoltRawMean[j] / N_SAMPLES_PER_ADC_READ;
//  }
  
  
  for (j = 0; j < N_UNITS_TOTAL; j++)
  {
    tmp = N_SAMPLES_PER_ADC_READ - 2;
    for (i = N_SAMPLES_TO_DROP; i < tmp; i++)
    {
//      if (cellVoltRaw[i][j] == 0 && cellVoltRaw[i+1][j] > 0)
      if (cellVoltRaw[i][j] < 5 && cellVoltRaw[i+1][j] >= 5)
//      if (cellVoltRaw[i][j] < 5 && cellVoltRaw[i+1][j] >= 5 && cellVoltRaw[i+2][j] >= 5)
      {
        iStart[j] = i+1;
        break;
      }
    }
    
    tmp = iStart[j] + 1;
    for (i = N_SAMPLES_PER_ADC_READ - 2; i > tmp; i--)
    {
//      if ((cellVoltRaw[i-1][j] > 0) && (cellVoltRaw[i][j] == 0))
//      if ((cellVoltRaw[i-1][j] >= 5) && (cellVoltRaw[i][j] < 5))
      if ((cellVoltRaw[i-1][j] < 5) && (cellVoltRaw[i][j] >= 5))
//      if ((cellVoltRaw[i-2][j] < 5) && (cellVoltRaw[i-1][j] >= 5) && (cellVoltRaw[i][j] >= 5))
      {
        iEnd[j] = i;
//        iEnd[j] = i-1;
        break;
      }
    }
  
    for (i = iStart[j]; i < iEnd[j]; i++)
    {
      meanCellRaw[j] += cellVoltRaw[i][j];
    }
    
    divider[j] = iEnd[j] - iStart[j];
    if (divider[j] == 0)
    {
      divider[j] = 1;
    }
    
    meanCellRaw[j] = (meanCellRaw[j] / divider[j]);
  }
  

  sCellValues.cells[ 1].cellVolt_mV = (meanCellRaw[ 0] * VREF_MV) / 1023;
  sCellValues.cells[ 2].cellVolt_mV = (meanCellRaw[ 1] * VREF_MV) / 1023;
  sCellValues.cells[ 3].cellVolt_mV = (meanCellRaw[ 2] * VREF_MV) / 1023;

  sCellValues.cells[ 4].cellVolt_mV = (meanCellRaw[ 3] * VREF_MV) / 1023;
  sCellValues.cells[ 5].cellVolt_mV = (meanCellRaw[ 4] * VREF_MV) / 1023;
  sCellValues.cells[ 6].cellVolt_mV = (meanCellRaw[ 5] * VREF_MV) / 1023;
  sCellValues.cells[ 7].cellVolt_mV = (meanCellRaw[ 6] * VREF_MV) / 1023;

  sCellValues.cells[ 8].cellVolt_mV = (meanCellRaw[ 7] * VREF_MV) / 1023;
  sCellValues.cells[ 9].cellVolt_mV = (meanCellRaw[ 8] * VREF_MV) / 1023;
  sCellValues.cells[10].cellVolt_mV = (meanCellRaw[ 9] * VREF_MV) / 1023;
  sCellValues.cells[11].cellVolt_mV = (meanCellRaw[10] * VREF_MV) / 1023;

  sCellValues.cells[12].cellVolt_mV = (meanCellRaw[11] * VREF_MV) / 1023;
  sCellValues.cells[13].cellVolt_mV = (meanCellRaw[12] * VREF_MV) / 1023;
  sCellValues.cells[14].cellVolt_mV = (meanCellRaw[13] * VREF_MV) / 1023;
  sCellValues.cells[15].cellVolt_mV = (meanCellRaw[14] * VREF_MV) / 1023;
}


//==============================================================================
// Buttons functions
//==============================================================================
void AssessButtons (void)
{
  // <editor-fold defaultstate="collapsed" desc="Check changes on board">
  // <editor-fold defaultstate="collapsed" desc="Change on SW1 on board">
  if (buttons.buttons.bits.sw1 != SW1)
  {
    buttons.buttons.bits.sw1    = SW1;
    buttons.chng.bits.sw1       =   1;
  }
  // </editor-fold>

  // <editor-fold defaultstate="collapsed" desc="Change on SW2 on board">
  if (buttons.buttons.bits.sw2 != SW2)
  {
    buttons.buttons.bits.sw2    = SW2;
    buttons.chng.bits.sw2       =   1;
  }
  // </editor-fold>

  // <editor-fold defaultstate="collapsed" desc="Change on SW3 on board">
  if (buttons.buttons.bits.sw3 != SW3)
  {
    buttons.buttons.bits.sw3    = SW3;
    buttons.chng.bits.sw3       =   1;
  }
  // </editor-fold>
  // </editor-fold>

  // <editor-fold defaultstate="collapsed" desc="Assess changes">
  if (buttons.chng.byte)  // If any change has occured on any button
  {
    // <editor-fold defaultstate="collapsed" desc="SW1 on board">
    if (buttons.chng.bits.sw1)
    {
      buttons.chng.bits.sw1 = 0;

      if (!buttons.buttons.bits.sw1)     // If SW1 is pressed
      {
        
      }
      else                                // If SW1 is not pressed
      {
        
      }
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="SW2 on board">
    if (buttons.chng.bits.sw2)
    {
      buttons.chng.bits.sw2 = 0;

      if (!buttons.buttons.bits.sw2)     // If SW2 is pressed
      {
        
      }
      else                              // If SW2 is not pressed
      {

      }
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="SW3 on board">
    if (buttons.chng.bits.sw3)
    {
      buttons.chng.bits.sw3 = 0;

      if (!buttons.buttons.bits.sw3)     // If SW3 is pressed
      {
        
      }
      else                          // If SW3 is not pressed
      {
        
      }
    }
    // </editor-fold>
  }
  // </editor-fold>
}