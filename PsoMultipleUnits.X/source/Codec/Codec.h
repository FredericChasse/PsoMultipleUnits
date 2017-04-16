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

#define MAX_DECODER_LENGTH    (100)

typedef enum
{
  DECODER_RET_MSG_NO_MSG
 ,DECODER_RET_MSG_RNG_SEED
 ,DECODER_RET_MSG_START_ALGO
 ,DECODER_RET_MSG_STOP_ALGO
} DecoderReturnMsg_t;

typedef UINT8               (*CodecInit_fct)            (void *ctx, UartModule_t uartChannel);
typedef DecoderReturnMsg_t  (*CodecDecoderFsmStep_fct)  (void *ctx, UINT8 *rxMsg);
typedef UINT8               (*CodecCodeNewUnitsMsg_fct) (void *ctx, ProtocolUnitsDataPayload_t *newMsg);
typedef UINT8               (*CodecCodeNewPsoMsg_fct)   (void *ctx, ProtocolPsoDataPayload_t *newMsg);

typedef struct
{
  void                     *ctx;
  CodecInit_fct             Init;
  CodecDecoderFsmStep_fct   DecoderFsmStep;
  CodecCodeNewUnitsMsg_fct  CodeNewUnitsMsg;
  CodecCodeNewPsoMsg_fct    CodeNewPsoMsg;
} CodecInterface_t;

// Public functions
//==============================================================================

const CodecInterface_t * CodecInterface(void);


#endif // __CODEC_H__