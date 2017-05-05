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
#include "PsoSwarm.h"   // For debug
#include "Characterization.h"
#include "ExtremumSeeking.h"
#include "Codec.h"
#include "Protocol.h"
#include "Rng.h"


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

//BOOL oSmoothData = 1;
BOOL oSmoothData = 0;

UINT16 nSamples = 0;

extern UINT32 iteration;

extern float sinus[2][15];

BOOL oSessionActive = 0;

AlgoInterface_t *algo;
UnitArrayInterface_t   *algoArray
                      ,*mainArray
                      ;
PerturbInterface_t *perturb;

CodecInterface_t *codec;

BOOL oSendPsoDbgData = 0;
BOOL oAlgoIsPso = 0;

UINT8 dbgIdx = 0;
float dbgPos[3] = {320.5882,410.7843,948.0392};
BOOL oDbgBtn = 0;


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
  // Current state == StateError
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pState == &StateError)
  {
    if (ERROR_2_ACQ)
    {
      pState = &StateAcq;
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
    if (COMP_2_ACQ)
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
  
//  START_INTERRUPTS;
  
//  // Init digital potentiometers AD8403
//  InitPot(0);
//  InitPot(1);
  InitPot(2);
  InitPot(3);
//  TurnOnPot(2);
//  TurnOnPot(3);
  
//  Timer.DelayMs(10000);
//  LED1_ON;
  
  mainArray = (UnitArrayInterface_t *)  UnitArrayInterface();
  algoArray = (UnitArrayInterface_t *)  UnitArrayInterface();
  perturb   = (PerturbInterface_t   *)  PerturbInterface  ();
  codec     = (CodecInterface_t     *)  CodecInterface    ();
  algo      = NULL;
  
  codec->Init(codec->ctx, U_MATLAB);
  
  mainArray->Init(mainArray->ctx, 0);
  for (i = 0; i < N_UNITS_TOTAL; i++)
  {
    mainArray->SetPos(mainArray->ctx, i, potRealValues[0]);
  }
  algoArray->Init(algoArray->ctx, 1);
  
  START_INTERRUPTS;
  
  perturb->Init(perturb->ctx, 500);
  
  nSamples = 0;
}


//===============================================================
// Name     : StateAcq
// Purpose  : Check for received data
//===============================================================
void StateAcq(void)
{
  float dbg; UINT8 idx;
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
      
      if (oSessionActive)
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
  
  UINT64 seed1, seed2;
  UINT8 retBuf[MAX_DECODER_LENGTH];
  DecoderReturnMsg_t ret;
  UINT8 nUnits;
  UINT8 i;
  UINT8 units[N_UNITS_TOTAL];
  ret = codec->DecoderFsmStep(codec->ctx, retBuf);
  switch (ret)
  {
    case DECODER_RET_MSG_RNG_SEED:
      memcpy(&seed1, &retBuf[0], 8);
      memcpy(&seed2, &retBuf[8], 8);
      Rng_InitSeed(seed1, seed2);
      break;
      
    case DECODER_RET_MSG_START_ALGO:
      if (!oSessionActive) // Ensure we are not already started
      {
        nUnits = retBuf[1];
        memcpy(units, &retBuf[2], nUnits);
        for (i = 0; i < nUnits; i++)
        {
          algoArray->AddUnitToArray(algoArray->ctx, mainArray->GetUnitHandle(mainArray->ctx, units[i]));
        }
        switch (retBuf[0])
        {
          case CLASSIC_PSO:
            oAlgoIsPso      = 1;
            oSessionActive  = 1;
            nSamples        = 0;  // Reset the samples
            algo = (AlgoInterface_t *) PsoInterface(PSO_TYPE_PSO_1D);
            algo->Init(algo->ctx, algoArray);
            break;
            
          case PARALLEL_PSO_MULTI_SWARM:
            oAlgoIsPso      = 1;
            oSessionActive  = 1;
            nSamples        = 0;  // Reset the samples
            algo = (AlgoInterface_t *) PsoInterface(PSO_TYPE_PARALLEL_PSO_MULTI_SWARM);
            algo->Init(algo->ctx, algoArray);
            break;
            
          case CHARACTERIZATION:
            oAlgoIsPso      = 0;
            oSessionActive  = 1;
            nSamples        = 0;  // Reset the samples
            algo = (AlgoInterface_t *) CharacterizationInterface();
            algo->Init(algo->ctx, algoArray);
            break;
            
          case PARALLEL_PSO:
            oAlgoIsPso      = 1;
            oSessionActive  = 1;
            nSamples        = 0;  // Reset the samples
            algo = (AlgoInterface_t *) PsoInterface(PSO_TYPE_PARALLEL_PSO);
            algo->Init(algo->ctx, algoArray);
            break;
            
          case EXTREMUM_SEEKING:
            oAlgoIsPso      = 0;
            oSessionActive  = 1;
            nSamples        = 0;  // Reset the samples
            algo = (AlgoInterface_t *) ExtremumSeekingInterface();
            algo->Init(algo->ctx, algoArray);
            break;
            
          default:
            oAlgoIsPso      = 0;
            for (i = 0; i < nUnits; i++)
            {
              algoArray->RemoveUnitFromArray(algoArray->ctx, 0);
            }
            break;
        }
      }
      break;
      
    case DECODER_RET_MSG_STOP_ALGO:
      if (oSessionActive)   // To ensure that we are currently running
      {
        oSessionActive = 0;
        oAlgoIsPso     = 0;
        algo->Release(algo->ctx);
        nUnits = algoArray->GetNUnits(algoArray->ctx);
        for (i = 0; i < nUnits; i++)
        {
          algoArray->RemoveUnitFromArray(algoArray->ctx, 0);
        }
      }
      break;
      
    default:
      LED1_ON;
    case DECODER_RET_MSG_NO_MSG:
      break;
  }
}


//===============================================================
// Name     : StateCompute
// Purpose  : Compute algorithm.
//===============================================================
void StateCompute(void)
{  
  oNewSample = 0;
  
  UINT8 i, id;
  float  positions[N_UNITS_TOTAL]
        ,powers   [N_UNITS_TOTAL]
        ,pSpeed   [PSO_SWARM_MAX_PARTICLES]
        ,pFitness [PSO_SWARM_MAX_PARTICLES]
        ,pPos     [PSO_SWARM_MAX_PARTICLES]
        ;
  UINT8 nUnits = algoArray->GetNUnits(algoArray->ctx);
  
  ProtocolUnitsDataPayload_t newUnitsPayload = {0};
  ProtocolPsoDataPayload_t   newPsoPayload   = {0};
  
  ComputeMeanAdcValues();
  for (i = 0; i < nUnits; i++)
  {
    positions[i]  = algoArray->GetPos(algoArray->ctx, i);
    id = algoArray->GetUnitId(algoArray->ctx, i);
    powers[i]     = ComputeCellPower(unitAdcs[id], algoArray->GetUnitPosIdx(algoArray->ctx, i));
    algoArray->SetPower(algoArray->ctx, i, powers[i]);
  }
  
  newUnitsPayload.timestamp_ms = algo->GetTimeElapsed(algo->ctx);
  newUnitsPayload.nData        = 1;
  newUnitsPayload.nUnits       = nUnits;
  newUnitsPayload.positions    = positions;
  newUnitsPayload.powers       = powers;
  
  codec->CodeNewUnitsMsg(codec->ctx, &newUnitsPayload);
  
  if (oSendPsoDbgData && oAlgoIsPso)
  {
    newPsoPayload.pos     = pPos;
    newPsoPayload.fitness = pFitness;
    newPsoPayload.speed   = pSpeed;
    algo->GetDebugData(algo->ctx, &newPsoPayload);
    codec->CodeNewPsoMsg(codec->ctx, &newPsoPayload);
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
