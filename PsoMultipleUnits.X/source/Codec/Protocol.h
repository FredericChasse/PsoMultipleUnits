//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Source files
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Protocol.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the definitions needed for the serial interface 
//           with MATLAB.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "Setup.h"


// Header definitions
//==============================================================================
#define PROTOCOL_DELIMITER      (0x7E)

typedef enum
{
  NEW_RNG_SEED  = 0x00
 ,START_ACQ     = 0x01
 ,END_ACQ       = 0x02
 ,SET_PERTURB   = 0x03
 ,SET_DBG_DATA  = 0x04
  
 ,UNITS_DATA    = 0x05
 ,PSO_DATA      = 0x06
 ,ADC_DATA      = 0x07
 ,PPSO_PNO_DATA = 0x08
} ProtocolMsgType_t;

typedef struct
{
  UINT8   delimiter;
  UINT8   type;
  UINT16  lengthOfPayload;
} ProtocolHeader_t;
extern const size_t sizeOfProtocolHeader;
//==============================================================================


// Payload definitions
//==============================================================================

typedef struct
{
  UINT64 seed1;
  UINT64 seed2;
} ProtocolNewRngSeedPayload_t;
extern const size_t sizeOfNewRngSeedPayload;

typedef struct
{
  UINT32 iteration;
  INT16  amplitude;
  UINT8  nUnits;
  UINT8 *units;
} ProtocolSetPerturbPayload_t;
extern const size_t sizeOfSetPerturbPayloadBase;

typedef struct
{
  UINT8 oSendDebugData;
} ProtocolSetDebugDataPayload_t;
extern const size_t sizeOfSetDebugDataPayloadBase;

typedef enum
{
  CLASSIC_PSO               = 0x00
 ,PARALLEL_PSO              = 0x01
 ,PARALLEL_PSO_MULTI_SWARM  = 0x02
 ,MULTI_UNIT                = 0x03
 ,EXTREMUM_SEEKING          = 0x04
 ,CHARACTERIZATION          = 0x05
 ,PPSO_PNO                  = 0x06
 ,PNO                       = 0x07
 ,DEBUG_ADC                 = 0x08
} ProtocolTypeOfAlgo_t;

#define PROTOCOL_START_ALGO   ('!')
typedef struct
{
  UINT8 startAlgoChar;
  UINT8 typeOfAlgo;
  UINT8 nUnits;
  UINT8 *units;
} ProtocolStartAcqPayload_t;
extern const size_t sizeOfStartAcqPayloadBase;

#define PROTOCOL_STOP_ALGO    ('x')
typedef struct
{
  UINT8 stopAlgoChar;
} ProtocolStopAcqPayload_t;
extern const size_t sizeOfStopAcqPayload;

typedef struct
{
  float   timestamp_ms;
  UINT8   nUnits;
  UINT8   nData;
  float  *positions;
  float  *powers;
} ProtocolUnitsDataPayload_t;
extern const size_t sizeOfUnitsTimestamp;
extern const size_t sizeOfUnitsNUnits;
extern const size_t sizeOfUnitsNData;
extern const size_t sizeOfUnitsOnePosition;
extern const size_t sizeOfUnitsOnePower;
extern const size_t sizeOfUnitsDataPayloadBase;

typedef struct
{
  UINT8   oNewPacket;
  UINT8   nUnits;
  UINT16  nData;
  UINT16 *data;
} ProtocolAdcDataPayload_t;
extern const size_t sizeOfAdcONewPacket;
extern const size_t sizeOfAdcNUnits;
extern const size_t sizeOfAdcNData;
extern const size_t sizeOfAdcOneData;
extern const size_t sizeOfAdcDataPayloadBase;

typedef struct
{
  UINT16  iteration;
  UINT8   nParticles;
  UINT16  nData;
  float   *speed;
  float   *pos;
  float   *fitness;
} ProtocolPsoDataPayload_t;
extern const size_t sizeOfPsoIteration;
extern const size_t sizeOfPsoNParticles;
extern const size_t sizeOfPsoNData;
extern const size_t sizeOfPsoOneParticleSpeed;
extern const size_t sizeOfPsoOneParticlePos;
extern const size_t sizeOfPsoOneParticlefitness;
extern const size_t sizeOfPsoDataPayloadBase;

typedef struct
{
  UINT16 iteration;
  UINT8  nGroups;
  UINT8  groups[N_UNITS_TOTAL];
  UINT8  groupLengths[N_UNITS_TOTAL];
} ProtocolPpsoPnoDataPayload_t;
extern const size_t sizeOfPpsoPnoIteration;
extern const size_t sizeOfPpsoPnoNGroups;
extern const size_t sizeOfPpsoPsoGroups;
extern const size_t sizeOfPpsoPsoGroupLengths;
extern const size_t sizeOfPpsoPnoDataPayloadBase;
//==============================================================================

// Protocol message definition
//==============================================================================
typedef struct
{
  ProtocolHeader_t header;
  UINT8           *payload;
} ProtocolMsg_t;
//==============================================================================


#endif // __PROTOCOL_H__