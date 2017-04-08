//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Source files
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Protocol.c
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

#include "Protocol.h"
#include "Setup.h"

const size_t sizeOfNewRngSeedPayload    = 16;

const size_t sizeOfStartAcqPayloadBase  = 2;

const size_t sizeOfStopAcqPayload       = 1;

const size_t sizeOfUnitsTimestamp       = 4;
const size_t sizeOfUnitsNUnits          = 1;
const size_t sizeOfUnitsNData           = 1;
const size_t sizeOfUnitsOnePosition     = 4;
const size_t sizeOfUnitsOnePower        = 4;
const size_t sizeOfUnitsDataPayloadBase = 6;

const size_t sizeOfProtocolHeader       = 4;