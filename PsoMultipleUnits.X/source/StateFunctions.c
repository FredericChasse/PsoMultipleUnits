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
extern BOOL oSmoothData;
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

extern BOOL oDbgBtn;
  
//float sinus[2][15] = { {0 , .4189 , .8378 , 1.2566 , 1.6755 , 2.0944 , 2.5133 , 2.9322 , 3.3510 , 3.7699 , 4.1888 , 4.6077 , 5.0265 , 5.4454 , 5.8643} ,
float sinus[2][15] = { {1 , 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 ,12 ,13, 14, 15} ,
                       {0 , .4067 , .7431 , .9511  , .9945  , .8660  , .5878  , .2079  , -.2079 , -.5878 , -.8660 , -.9945 , -.9511 , -.7431 , -.4067} };
 

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

inline void GetAdcValues (void)
{
  UINT8 i;
  memcpy((void *) &cellVoltRaw[nSamples][0], (void *) &Adc.Var.adcReadValues[1], 30);  // sizeof(UINT16) * 15 = 30
//  memcpy((void *) &cellVoltageRaw[0], (void *) &Adc.Var.adcReadValues[0], 64);  // sizeof(UINT32) * 16 = 64
//  memcpy((void *) &cellVoltageRaw[0], (void *) &Adc.Var.adcReadValues[0], 32);  // sizeof(UINT16) * 16 = 32
//
////  sCellValues.cells[ 0].cellVoltRaw[nSamples] = cellVoltageRaw[ 0];
//  sCellValues.cells[ 1].cellVoltRaw[nSamples] = cellVoltageRaw[ 1];
//  sCellValues.cells[ 2].cellVoltRaw[nSamples] = cellVoltageRaw[ 2];
//  sCellValues.cells[ 3].cellVoltRaw[nSamples] = cellVoltageRaw[ 3];
//  
//  sCellValues.cells[ 4].cellVoltRaw[nSamples] = cellVoltageRaw[ 4];
//  sCellValues.cells[ 5].cellVoltRaw[nSamples] = cellVoltageRaw[ 5];
//  sCellValues.cells[ 6].cellVoltRaw[nSamples] = cellVoltageRaw[ 6];
//  sCellValues.cells[ 7].cellVoltRaw[nSamples] = cellVoltageRaw[ 7];
//  
//  sCellValues.cells[ 8].cellVoltRaw[nSamples] = cellVoltageRaw[ 8];
//  sCellValues.cells[ 9].cellVoltRaw[nSamples] = cellVoltageRaw[ 9];
//  sCellValues.cells[10].cellVoltRaw[nSamples] = cellVoltageRaw[10];
//  sCellValues.cells[11].cellVoltRaw[nSamples] = cellVoltageRaw[11];
//  
//  sCellValues.cells[12].cellVoltRaw[nSamples] = cellVoltageRaw[12];
//  sCellValues.cells[13].cellVoltRaw[nSamples] = cellVoltageRaw[13];
//  sCellValues.cells[14].cellVoltRaw[nSamples] = cellVoltageRaw[14];
//  sCellValues.cells[15].cellVoltRaw[nSamples] = cellVoltageRaw[15];
  
//  in.oldest = in.previous;
//  in.previous = in.current;
//  in.current = cellVoltageRaw[10];
//  
////  UINT32 coreTickRate = Timer.Tic(15000, SCALE_NS);
//  NpfZ32StaticOptimized(&in, &out);
////  INT32 time = Timer.Toc(15000, coreTickRate);
//  sCellValues.cells[10].cellVoltRaw[nSamples] = out.current;
}

extern volatile BOOL oAdcReady;
extern volatile BOOL oAcqOngoing;
INT16 uk0[15] = {0}, uk1[15] = {0}, uk2[15] = {0}, yk0[15] = {0}, yk1[15] = {0}, yk2[15] = {0};
INT16 uk0_2 = 0, uk1_2 = 0, uk2_2 = 0, yk0_2 = 0, yk1_2 = 0, yk2_2 = 0;
INT16 uk0_3 = 0, uk1_3 = 0, uk2_3 = 0, yk0_3 = 0, yk1_3 = 0, yk2_3 = 0;

void ResetFilterValues (void)
{
  memset(uk0, 0, 30);
  memset(uk1, 0, 30);
  memset(uk2, 0, 30);
  memset(yk0, 0, 30);
  memset(yk1, 0, 30);
  memset(yk2, 0, 30);
  
  uk0_2 = uk0_3 = uk1_2 = uk1_3 = uk2_2 = uk2_3 = yk0_2 = yk0_3 = yk2_2 = yk2_3 = yk1_2 = yk1_3 = 0;
  
  while (oAcqOngoing);
  nSamples = 0;
}

inline void ComputeMeanAdcValues (void)
{
  UINT16 i = N_SAMPLES_TO_DROP, j = 0;
  UINT32 meanCellRaw[N_UNITS_TOTAL] = {0};
  
  UINT16 iEnd[N_UNITS_TOTAL] = {N_SAMPLES_PER_ADC_READ};
  UINT16 iStart[N_UNITS_TOTAL] = {N_SAMPLES_TO_DROP};
  UINT16 maxVal[N_UNITS_TOTAL] = {0};
  UINT16 halfMaxVal[N_UNITS_TOTAL];
  UINT16 firstVal[N_UNITS_TOTAL], lastVal[N_UNITS_TOTAL];
  UINT16 divider[N_UNITS_TOTAL];
  
  if (oDbgAdc && oSendAdcData)
  {
    for (i = iStart[1], j = 0; i < iEnd[1]; i++, j++)
    {
      dbgAdcData[j] = cellVoltRaw[i][1];
    }
  }
  
  for (j = 0; j < N_UNITS_TOTAL; j++)
  {
    for (i = N_SAMPLES_TO_DROP; i < N_SAMPLES_PER_ADC_READ; i++)
    {
      meanCellRaw[j] += cellVoltRaw[i][j];
    }
    
    meanCellRaw[j] = meanCellRaw[j] / N_TOTAL_SAMPLES;
  }

  if (oSmoothData)  // Smoothing function
  {
    sCellValues.cells[ 1].cellVoltFloat = (kFilter*sCellValues.cells[ 0].cellVoltFloat) + (meanCellRaw[ 0] * VREF / 1024.0f)*(1 - kFilter);
    sCellValues.cells[ 2].cellVoltFloat = (kFilter*sCellValues.cells[ 1].cellVoltFloat) + (meanCellRaw[ 1] * VREF / 1024.0f)*(1 - kFilter);
    sCellValues.cells[ 3].cellVoltFloat = (kFilter*sCellValues.cells[ 2].cellVoltFloat) + (meanCellRaw[ 2] * VREF / 1024.0f)*(1 - kFilter);
    sCellValues.cells[ 4].cellVoltFloat = (kFilter*sCellValues.cells[ 3].cellVoltFloat) + (meanCellRaw[ 3] * VREF / 1024.0f)*(1 - kFilter);
   
    sCellValues.cells[ 5].cellVoltFloat = (kFilter*sCellValues.cells[ 4].cellVoltFloat) + (meanCellRaw[ 4] * VREF / 1024.0f)*(1 - kFilter);
    sCellValues.cells[ 6].cellVoltFloat = (kFilter*sCellValues.cells[ 5].cellVoltFloat) + (meanCellRaw[ 5] * VREF / 1024.0f)*(1 - kFilter);
    sCellValues.cells[ 7].cellVoltFloat = (kFilter*sCellValues.cells[ 6].cellVoltFloat) + (meanCellRaw[ 6] * VREF / 1024.0f)*(1 - kFilter);
    sCellValues.cells[ 8].cellVoltFloat = (kFilter*sCellValues.cells[ 7].cellVoltFloat) + (meanCellRaw[ 7] * VREF / 1024.0f)*(1 - kFilter);
    
    sCellValues.cells[ 9].cellVoltFloat = (kFilter*sCellValues.cells[ 8].cellVoltFloat) + (meanCellRaw[ 8] * VREF / 1024.0f)*(1 - kFilter);
    sCellValues.cells[10].cellVoltFloat = (kFilter*sCellValues.cells[ 9].cellVoltFloat) + (meanCellRaw[ 9] * VREF / 1024.0f)*(1 - kFilter);
    sCellValues.cells[11].cellVoltFloat = (kFilter*sCellValues.cells[10].cellVoltFloat) + (meanCellRaw[10] * VREF / 1024.0f)*(1 - kFilter);
    sCellValues.cells[12].cellVoltFloat = (kFilter*sCellValues.cells[11].cellVoltFloat) + (meanCellRaw[11] * VREF / 1024.0f)*(1 - kFilter);
    
    sCellValues.cells[13].cellVoltFloat = (kFilter*sCellValues.cells[12].cellVoltFloat) + (meanCellRaw[12] * VREF / 1024.0f)*(1 - kFilter);
    sCellValues.cells[14].cellVoltFloat = (kFilter*sCellValues.cells[13].cellVoltFloat) + (meanCellRaw[13] * VREF / 1024.0f)*(1 - kFilter);
    sCellValues.cells[15].cellVoltFloat = (kFilter*sCellValues.cells[14].cellVoltFloat) + (meanCellRaw[14] * VREF / 1024.0f)*(1 - kFilter);
//    sCellValues.cells[15].cellVoltFloat = (kFilter*sCellValues.cells[15].cellVoltFloat) + (meanCellRaw[15] * VREF / 1024.0f)*(1 - kFilter);
  }
  else
  {
    sCellValues.cells[ 1].cellVolt_mV = (meanCellRaw[ 0] * VREF_MV) >> 10;
    sCellValues.cells[ 2].cellVolt_mV = (meanCellRaw[ 1] * VREF_MV) >> 10;
    sCellValues.cells[ 3].cellVolt_mV = (meanCellRaw[ 2] * VREF_MV) >> 10;
    
    sCellValues.cells[ 4].cellVolt_mV = (meanCellRaw[ 3] * VREF_MV) >> 10;
    sCellValues.cells[ 5].cellVolt_mV = (meanCellRaw[ 4] * VREF_MV) >> 10;
    sCellValues.cells[ 6].cellVolt_mV = (meanCellRaw[ 5] * VREF_MV) >> 10;
    sCellValues.cells[ 7].cellVolt_mV = (meanCellRaw[ 6] * VREF_MV) >> 10;
    
    sCellValues.cells[ 8].cellVolt_mV = (meanCellRaw[ 7] * VREF_MV) >> 10;
    sCellValues.cells[ 9].cellVolt_mV = (meanCellRaw[ 8] * VREF_MV) >> 10;
    sCellValues.cells[10].cellVolt_mV = (meanCellRaw[ 9] * VREF_MV) >> 10;
    sCellValues.cells[11].cellVolt_mV = (meanCellRaw[10] * VREF_MV) >> 10;
    
    sCellValues.cells[12].cellVolt_mV = (meanCellRaw[11] * VREF_MV) >> 10;
    sCellValues.cells[13].cellVolt_mV = (meanCellRaw[12] * VREF_MV) >> 10;
    sCellValues.cells[14].cellVolt_mV = (meanCellRaw[13] * VREF_MV) >> 10;
    sCellValues.cells[15].cellVolt_mV = (meanCellRaw[14] * VREF_MV) >> 10;
  }
}


//==============================================================================
// Buttons functions
//==============================================================================
void AssessButtons (void)
{ 
  sUartLineBuffer_t buffer = 
  { 
     .buffer = {0} 
    ,.length =  0 
  };
  
  UINT16 i = 0;
  
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
        memcpy(buffer.buffer, sinus, 120);
        buffer.length = 120;
        
        Uart.PutTxFifoBuffer(U_MATLAB, &buffer);
        while(!SW1);
        for (i = 0; i < 15; i++)
        {
          sinus[0][i] += 2*PI;
        }
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
        nSamples = 0;
        oDbgBtn = 1;
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