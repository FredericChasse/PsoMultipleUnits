//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Source files
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Codec.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the methods needed for the serial interface 
//           with MATLAB.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __CODEC_H__
#define __CODEC_H__

#include "Setup.h"
#include "Protocol.h"

// Public definitions
//==============================================================================

typedef UINT8 (*CodecInit_fct)          (void *ctx, UartModule_t uartChannel);
typedef UINT8 (*CodecFsmStep_fct)       (void *ctx);
typedef BOOL  (*CodecIsLinkActive_fct)  (void *ctx);

typedef struct
{
  void                 *ctx;
  CodecInit_fct         Init;
  CodecFsmStep_fct      FsmStep;
  CodecIsLinkActive_fct IsLinkActive;
} CodecInterface_t;

// Public functions
//==============================================================================

const CodecInterface_t * CodecInterface(void);

#endif // __CODEC_H__