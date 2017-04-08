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

extern const size_t sizeOfProtocolHeader;

typedef enum
{
  NEW_RNG_SEED  = 0x00
 ,START_ACQ     = 0x01
 ,END_ACQ       = 0x02
  
 ,UNITS_DATA    = 0x03
} ProtocolMsgType_t;

typedef struct
{
  UINT8   delimiter;
  UINT8   type;
  UINT16  lengthOfPayload;
} ProtocolHeader_t;
//==============================================================================


// Payload definitions
//==============================================================================

typedef struct
{
  UINT64 seed1;
  UINT64 seed2;
} ProtocolNewRngSeedPayload_t;
extern const size_t sizeOfNewRngSeedPayload;

typedef enum
{
  CLASSIC_PSO               = 0x00
 ,PARALLEL_PSO              = 0x01
 ,PARALLEL_PSO_MULTI_SWARM  = 0x02
 ,MULTI_UNIT                = 0x03
 ,EXTREMUM_SEEKING          = 0x04
 ,CHARACTERIZATION          = 0x05
} ProtocolTypeOfAlgo_t;

#define PROTOCOL_START_ALGO   ('!')
typedef struct
{
  UINT8 startAlgoChar;
  UINT8 typeOfAlgo;
} ProtocolStartAcqPayload_t;
extern const size_t sizeOfStartAcqPayload;

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
extern const size_t sizeOfUnitsDataPayload;
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