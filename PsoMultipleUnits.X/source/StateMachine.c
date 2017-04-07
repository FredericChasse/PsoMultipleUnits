//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : StateMachine.c
// Author  : Frederic Chasse
// Date    : 2015-02-25
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the C file for the state machine of the system.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : State names can and should be renamed by the user to improve the
//           readability of the code.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "..\headers\StateMachine.h"
#include "Perturb.h"
#include "UnitArray.h"
#include "UnitMapping.h"
#include "AlgoInterface.h"
#include "Pso.h"
#include "Characterization.h"
#include "Codec.h"


extern volatile BOOL   oAdcReady
                      ,oTimer3Ready
                      ;

extern UINT8 matlabPacketSize;

extern const float potRealValuesInverse[256];

BOOL  oSendData       = 0
     ,oNewSample      = 0
     ,oErrorFlag      = 0
     ;

UINT32 cellVoltageRaw [16] = {0};

struct sAllCells sCellValues = {0};

sUartFifoBuffer_t matlabData = 
{
   .bufEmpty   = 1
  ,.bufFull    = 0
  ,.inIdx      = 0
  ,.outIdx     = 0
  ,.lineBuffer = {0}
  ,.maxBufSize = UART_LINE_BUFFER_LENGTH
};

BOOL oSmoothData = 1;

UINT16 nSamples = 0;

extern UINT32 iteration;

extern float sinus[2][15];

BOOL oFirstTimeCallFromMatlab = 1;

AlgoInterface_t *algo;
UnitArrayInterface_t   *algoArray
                      ,*mainArray
                      ;
PerturbInterface_t *perturb;

CodecInterface_t *codec;

//==============================================================================
//	STATES OF STATE MACHINE
//==============================================================================

//===============================================================
// Name     : StateScheduler
// Purpose  : Decides which state is next depending on current
//            state and flags. Used as a function
//===============================================================
void StateScheduler(void)
{
  // Clear WDT
  //============================
  Wdt.Clear();
  //============================

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state == StateInit
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  if (pState == &StateInit)
  {
    if (INIT_2_ACQ)
    {
      pState = &StateAcq;
    }
    else if (INIT_2_SEND_DATA)
    {
      pState = &StateSendData;
    }
    else if (INIT_2_ERROR)
    {
      pState = &StateError;
    }
    else if (INIT_2_COMP)
    {
      pState = &StateCompute;
    }
    else
    {
      pState = &StateError;   // Go to Error state by default
    }
  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state == StateAcq
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pState == &StateAcq)
  {
    if (ACQ_2_COMP)
    {
      pState = &StateCompute;
    }
    else if (ACQ_2_SEND_DATA)
    {
      pState = &StateSendData;
    }
    else if (ACQ_2_ACQ)
    {
      pState = &StateAcq;
    }
    else if (ACQ_2_ERROR)
    {
      pState = &StateError;
    }
    else
    {
      pState = &StateError;   // Go to Error state by default
    }
  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state == StateSendData
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pState == &StateSendData)
  {
    if (SEND_DATA_2_ACQ)
    {
      pState = &StateAcq;
    }
    else if (SEND_DATA_2_SEND_DATA)
    {
      pState = &StateSendData;
    }
    else if (SEND_DATA_2_ERROR)
    {
      pState = &StateError;
    }
    else if (SEND_DATA_2_COMP)
    {
      pState = &StateCompute;
    }
    else
    {
      pState = &StateError;   // Go to Error state by default
    }
  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state == StateError
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pState == &StateError)
  {
    if (ERROR_2_ACQ)
    {
      pState = &StateAcq;
    }
    else if (ERROR_2_SEND_DATA)
    {
      pState = &StateSendData;
    }
    else if (ERROR_2_COMP)
    {
      pState = &StateCompute;
    }
    else if (ERROR_2_ERROR)
    {
      pState = &StateError;
    }
    else
    {
      pState = &StateError;   // Go to Error state by default
    }
  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state == StateCompute
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pState == &StateCompute)
  {
    if (COMP_2_SEND_DATA)
    {
      pState = &StateSendData;
    }
    else if (COMP_2_ACQ)
    {
      pState = &StateAcq;
    }
    else if (COMP_2_COMP)
    {
      pState = &StateCompute;
    }
    else if (COMP_2_ERROR)
    {
      pState = &StateError;
    }
    else
    {
      pState = &StateError;   // Go to Error state by default
    }
  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state undetermined
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else
  {
    pState = &StateError;   // Go to Error state by default
  }

}


//===============================================================
// Name     : StateInit
// Purpose  : Initialization of the system.
//===============================================================
void StateInit(void)
{
  UINT8 i = 0;

  INTDisableInterrupts();   // Disable all interrupts of the system.

  INIT_PORTS;
  INIT_TIMER;
  INIT_ADC;
  INIT_UART;
  INIT_SKADI;
  INIT_I2C;
  INIT_SPI;
  INIT_WDT;
  
  START_INTERRUPTS;
  
//  // Init digital potentiometers AD8403
//  InitPot(0);
//  InitPot(1);
  InitPot(2);
  InitPot(3);
  
  mainArray = (UnitArrayInterface_t *)  UnitArrayInterface();
  algoArray = (UnitArrayInterface_t *)  UnitArrayInterface();
  perturb   = (PerturbInterface_t   *)  PerturbInterface  ();
  codec     = (CodecInterface_t     *)  CodecInterface    ();
  
  codec->Init(codec->ctx, U_MATLAB);
  
  mainArray->Init(mainArray->ctx, 0);
  for (i = 0; i < N_UNITS_TOTAL; i++)
  {
    mainArray->SetPos(mainArray->ctx, i, potRealValues[0]);
  }
  algoArray->Init(algoArray->ctx, 1);
  
  for (i = 0; i < N_UNITS_TOTAL; i++)
  {
    algoArray->AddUnitToArray(algoArray->ctx, mainArray->GetUnitHandle(mainArray->ctx, i));
  }
  perturb->Init(perturb->ctx, 500);

}


//===============================================================
// Name     : StateAcq
// Purpose  : Check for received data
//===============================================================
void StateAcq(void)
{
  INT32 err = 0;
  
  //==================================================================
  // ADC READ
  // Check cells voltage
  //==================================================================
  if (oAdcReady)
  {
    oAdcReady = 0;

    GetAdcValues();

    nSamples++;
    if (nSamples >= N_SAMPLES_PER_ADC_READ)
    {
      nSamples = 0;
      
      if (codec->IsLinkActive(codec->ctx))
      {
        oNewSample = 1;   // Go to stateCompute
      }
    }
  }
  
  //==================================================================
  // Check for skadi messages, U_MATLAB msgs or button changes
  //==================================================================
  Skadi.GetCmdMsgFifo();
  
  AssessButtons();
  
  codec->FsmStep(codec->ctx);
}


//===============================================================
// Name     : StateSendData
// Purpose  : Send data on UART.
//===============================================================
void StateSendData(void)
{
  oSendData = 0;
  
  sUartLineBuffer_t localBuffer = 
  { 
     .buffer = {0} 
    ,.length =  0 
  };
  
  UINT32 i = 0;
  
  for (i = 0; i < matlabPacketSize; i++)
  {
    FifoRead(&matlabData, &localBuffer.buffer[i]);
    localBuffer.length++;
  }
  
  Uart.PutTxFifoBuffer(U_MATLAB, &localBuffer);
}


//===============================================================
// Name     : StateCompute
// Purpose  : Compute algorithm.
//===============================================================
void StateCompute(void)
{  
  oNewSample = 0;
  
  UINT8 i;
  float pos, power;
  UINT8 nUnits = algoArray->GetNUnits(algoArray->ctx);
  
  ComputeMeanAdcValues();
  for (i = 0; i < nUnits; i++)
  {
    pos = algoArray->GetPos(algoArray->ctx, i);
    power = ComputeCellPower(unitAdcs[i], pos);
    algoArray->SetPower(algoArray->ctx, i, power);
  }
  
  algo->Run(algo->ctx);
}


//===============================================================
// Name     : StateError
// Purpose  : Error state of the system. Used to assess and
//            correct errors in the system.
//===============================================================
void StateError(void)
{
  INTDisableInterrupts();   // Disable all interrupts of the system.
  
  LED1_ON;
  LED2_OFF;
  
  while(1)
  {
    LED1_TOGGLE;
    LED2_TOGGLE;
    
    Timer.DelayMs(500);
  }
}
