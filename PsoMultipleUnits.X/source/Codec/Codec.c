//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Source files
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Codec.c
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

#include "Codec.h"
#include "ByteBuffer.h"


// Private definitions
//==============================================================================

typedef enum
{
  S_CODEC_STANDBY
 ,S_CODEC_START_DECODE
 ,S_CODEC_DECODING
 ,S_CODEC_END_DECODE
} CodecState_t;

typedef struct
{
  CodecState_t state;
  UartModule_t uartChannel;
  ByteBuffer_t *decoder;
  ByteBuffer_t *coder;
  BOOL          oLinkActive;
} Codec_t;

// Private prototypes
//==============================================================================

UINT8 _Codec_Init           (Codec_t *c, UartModule_t uartChannel);
UINT8 _Codec_FsmStep        (Codec_t *c);
BOOL  _Codec_IsLinkActive   (Codec_t *c);
UINT8 _Codec_DecoderFsm     (Codec_t *c);
UINT8 _Codec_CoderFsm       (Codec_t *c);

UINT8 _Codec_Decode         (UINT8 *newBytes, UINT8 nNewBytes);

// Private variables
//==============================================================================

Codec_t _codec;

#define MAX_DECODER_LENGTH    (3)
ByteBuffer_t _decoder;
UINT8        _decoderBuf[MAX_DECODER_LENGTH];
#define MAX_CODER_LENGTH      (40)
ByteBuffer_t _coder;
UINT8        _coderBuf[MAX_CODER_LENGTH];

const CodecInterface_t _codec_if = 
{
  .ctx      = (void *)            &_codec
 ,.Init     = (CodecInit_fct)     &_Codec_Init
 ,.FsmStep  = (CodecFsmStep_fct)  &_Codec_FsmStep
};

// Private functions
//==============================================================================

UINT8 _Codec_Init (Codec_t *c, UartModule_t uartChannel)
{
  c->oLinkActive  = 0;
  c->state        = S_CODEC_STANDBY;
  c->uartChannel  = uartChannel;
  c->decoder      = &_decoder;
  c->coder        = &_coder;
  ByteBufferInit(c->decoder, _decoderBuf, MAX_DECODER_LENGTH);
  ByteBufferInit(c->coder  , _decoderBuf, MAX_DECODER_LENGTH);
}


BOOL _Codec_IsLinkActive (Codec_t *c)
{
  return c->oLinkActive;
}


UINT8 _Codec_CoderFsm (Codec_t *c)
{
  
}


UINT8 _Codec_DecoderFsm (Codec_t *c)
{
  sUartLineBuffer_t buffer;
  UINT8 i;
  INT32 err;
  if (Uart.Var.oIsRxDataAvailable[c->uartChannel])
  {
    err = Uart.GetRxFifoBuffer(c->uartChannel, &buffer, FALSE);
    if (err >= 0)
    {
      for (i = 0; i < buffer.length; i++)
      {
        BUFFER_ADD_FAST(c->decoder, &buffer.buffer[i]);
      }
    }
  }
}


UINT8 _Codec_Decode (UINT8 *newBytes, UINT8 nNewBytes)
{
  UINT8 i;
}


UINT8 _Codec_FsmStep (Codec_t *c)
{
  switch (c->state)
  {
    case S_CODEC_STANDBY:
      break;
      
    case S_CODEC_START_DECODE:
      break;
      
    case S_CODEC_DECODING:
      break;
      
    case S_CODEC_END_DECODE:
      break;
      
    default:
      break;
  }
}


// Public functions
//==============================================================================

const CodecInterface_t * CodecInterface (void)
{
  return &_codec_if;
}