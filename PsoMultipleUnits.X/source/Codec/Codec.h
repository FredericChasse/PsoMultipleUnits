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
 ,DECODER_RET_MSG_NEW_PERTURB
 ,DECODER_RET_MSG_SET_DEBUG_DATA
 ,DECODER_RET_MSG_INIT_PERTURB
} DecoderReturnMsg_t;

typedef UINT8               (*CodecInit_fct)                (void *ctx, UartModule_t uartChannel);
typedef DecoderReturnMsg_t  (*CodecDecoderFsmStep_fct)      (void *ctx, UINT8 *rxMsg);
typedef UINT8               (*CodecCodeNewUnitsMsg_fct)     (void *ctx, ProtocolUnitsDataPayload_t *newMsg);
typedef UINT8               (*CodecCodeNewPsoMsg_fct)       (void *ctx, ProtocolPsoDataPayload_t *newMsg);
typedef UINT8               (*CodecCodeNewPpsoPnoMsg_fct)   (void *ctx, ProtocolPpsoPnoDataPayload_t *newMsg);
typedef UINT8               (*CodecCodeNewAdcMsg_fct)       (void *ctx, ProtocolAdcDataPayload_t *newMsg);

typedef struct
{
  void                       *ctx;
  CodecInit_fct               Init;
  CodecDecoderFsmStep_fct     DecoderFsmStep;
  CodecCodeNewUnitsMsg_fct    CodeNewUnitsMsg;
  CodecCodeNewPsoMsg_fct      CodeNewPsoMsg;
  CodecCodeNewPpsoPnoMsg_fct  CodeNewPpsoPnoMsg;
  CodecCodeNewAdcMsg_fct      CodeNewAdcMsg;
} CodecInterface_t;

// Public functions
//==============================================================================

const CodecInterface_t * CodecInterface(void);


#endif // __CODEC_H__