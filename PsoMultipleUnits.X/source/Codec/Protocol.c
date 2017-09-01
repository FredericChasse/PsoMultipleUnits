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

const size_t sizeOfNewRngSeedPayload        = 16;

const size_t sizeOfSetPerturbPayloadBase    = 7;

const size_t sizeOfInitPerturbPayloadBase   = 2;

const size_t sizeOfSetDebugDataPayloadBase  = 1;

const size_t sizeOfStartAcqPayloadBase      = 3;

const size_t sizeOfStopAcqPayload           = 1;

const size_t sizeOfUnitsTimestamp           = 4;
const size_t sizeOfUnitsNUnits              = 1;
const size_t sizeOfUnitsNData               = 1;
const size_t sizeOfUnitsOnePosition         = 4;
const size_t sizeOfUnitsOnePower            = 4;
const size_t sizeOfUnitsDataPayloadBase     = 6;

const size_t sizeOfPsoIteration             = 2;
const size_t sizeOfPsoNParticles            = 1;
const size_t sizeOfPsoNData                 = 1;
const size_t sizeOfPsoOneParticleSpeed      = 4;
const size_t sizeOfPsoOneParticlePos        = 4;
const size_t sizeOfPsoOneParticleFitness    = 4;
const size_t sizeOfPsoDataPayloadBase       = 4;

const size_t sizeOfProtocolHeader           = 4;

const size_t sizeOfAdcONewPacket            = 1;
const size_t sizeOfAdcNUnits                = 1;
const size_t sizeOfAdcNData                 = 2;
const size_t sizeOfAdcOneData               = 2;
const size_t sizeOfAdcDataPayloadBase       = 4;

const size_t sizeOfPpsoPnoIteration         = 2;
const size_t sizeOfPpsoPnoNGroups           = 1;
const size_t sizeOfPpsoPsoGroups            = N_UNITS_TOTAL;
const size_t sizeOfPpsoPsoGroupLengths      = N_UNITS_TOTAL;
const size_t sizeOfPpsoPnoDataPayloadBase   = 2*N_UNITS_TOTAL + 3;