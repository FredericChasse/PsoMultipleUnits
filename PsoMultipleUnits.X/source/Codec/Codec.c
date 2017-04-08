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
  S_DECODER_STANDBY
 ,S_DECODER_GETTING_HEADER
 ,S_DECODER_GETTING_PAYLOAD
 ,S_DECODER_ANALYZE_PAYLOAD
} DecoderState_t;

typedef struct
{
  ByteBuffer_t   *buffer;
  DecoderState_t  state;
} Decoder_t;

typedef struct
{
  Decoder_t      *decoder;
  UartModule_t    uartChannel;
} Codec_t;

// Private prototypes
//==============================================================================

UINT8               _Codec_Init           (Codec_t *c, UartModule_t uartChannel);
DecoderReturnMsg_t  _Codec_DecoderFsmStep (Codec_t *c, UINT8 *rxMsg);
UINT8               _Codec_CodeNewMsg     (Codec_t *c, ProtocolUnitsDataPayload_t *newMsg);

// Private variables
//==============================================================================

Codec_t   _codec;
Decoder_t _decoder;

ByteBuffer_t _decoderBuf;
UINT8        _decoderArray[MAX_DECODER_LENGTH];

const CodecInterface_t _codec_if = 
{
  .ctx              = (void *)                    &_codec
 ,.Init             = (CodecInit_fct)             &_Codec_Init
 ,.DecoderFsmStep   = (CodecDecoderFsmStep_fct)   &_Codec_DecoderFsmStep
 ,.CodeNewMsg       = (CodecCodeNewMsg_fct)       &_Codec_CodeNewMsg
};

// Private functions
//==============================================================================

UINT8 _Codec_Init (Codec_t *c, UartModule_t uartChannel)
{
  c->uartChannel      = uartChannel;
  c->decoder          = &_decoder;
  c->decoder->buffer  = &_decoderBuf;
  c->decoder->state   = S_DECODER_STANDBY;
  
  ByteBufferInit(c->decoder->buffer, _decoderArray, MAX_DECODER_LENGTH);
}


UINT8 _Codec_CodeNewMsg (Codec_t *c, ProtocolUnitsDataPayload_t *newMsg)
{
  sUartLineBuffer_t buf = {0};
  const ProtocolHeader_t header = 
  {
    .delimiter        = PROTOCOL_DELIMITER
   ,.type             = UNITS_DATA
   ,.lengthOfPayload  = sizeOfUnitsDataPayload
  };
  memcpy(&buf.buffer[0], &header, sizeOfProtocolHeader);
  buf.length += sizeOfProtocolHeader;
  
  memcpy(&buf.buffer[buf.length], &newMsg->timestamp_ms, sizeOfUnitsTimestamp);
  buf.length += sizeOfUnitsTimestamp;
  
  memcpy(&buf.buffer[buf.length], &newMsg->nUnits, sizeOfUnitsNUnits);
  buf.length += sizeOfUnitsNUnits;
  
  memcpy(&buf.buffer[buf.length], &newMsg->nData, sizeOfUnitsNData);
  buf.length += sizeOfUnitsNData;
  
  memcpy(&buf.buffer[buf.length], newMsg->positions, sizeOfUnitsOnePosition * newMsg->nUnits);
  buf.length += sizeOfUnitsOnePosition * newMsg->nUnits;
  
  memcpy(&buf.buffer[buf.length], newMsg->powers, sizeOfUnitsOnePower * newMsg->nUnits);
  buf.length += sizeOfUnitsOnePower * newMsg->nUnits;
  
  while(Uart.PutTxFifoBuffer(c->uartChannel, &buf) < 0);
  
  return 0;
}


DecoderReturnMsg_t _Codec_DecoderFsmStep (Codec_t *c, UINT8 *rxMsg)
{
  sUartLineBuffer_t buffer;
  UINT8 i;
  INT32 err;
  // Get new characters pending
  //-------------------------------------------------------------
  if (Uart.Var.oIsRxDataAvailable[c->uartChannel])
  {
    err = Uart.GetRxFifoBuffer(c->uartChannel, &buffer, FALSE);
    if (err >= 0)
    {
      for (i = 0; i < buffer.length; i++)
      {
        BUFFER_ADD_FAST(c->decoder->buffer, &buffer.buffer[i]);
      }
    }
  }
  //-------------------------------------------------------------
  
  // Decoding RX messages
  //-------------------------------------------------------------
  static ProtocolHeader_t header = {0};
  UINT8 byte;
  UINT8 sizeOfPayload;
  static UINT8 rxBuf[MAX_DECODER_LENGTH] = {0};
  static UINT8 nData;
  
  switch(c->decoder->state)
  {
    case S_DECODER_STANDBY:
      if (c->decoder->buffer->count == 0)
      {
        return DECODER_RET_MSG_NO_MSG;
      }
      else
      {
        // If first character is not the correct delimiter, ignore it and quit
        if (PROTOCOL_DELIMITER != c->decoder->buffer->buf[c->decoder->buffer->outIdx])
        {
          BUFFER_GET_FAST(c->decoder->buffer, &byte);
          return DECODER_RET_MSG_NO_MSG;
        }
        // Else go directly to next state
        c->decoder->state = S_DECODER_GETTING_HEADER;
      }
      
    case S_DECODER_GETTING_HEADER:
      while ( (nData < sizeOfProtocolHeader) && (c->decoder->buffer->count) )
      {
        BUFFER_GET_FAST(c->decoder->buffer, &rxBuf[nData]);
        nData++;
      }
      
      if (nData == sizeOfProtocolHeader)  // We got the header
      {
        memcpy(&header, rxBuf, nData);
        nData = 0;
        sizeOfPayload = header.lengthOfPayload;
        c->decoder->state = S_DECODER_GETTING_PAYLOAD;
      }
      
      if (c->decoder->buffer->count == 0)
      {
        return DECODER_RET_MSG_NO_MSG;
      }
      
    case S_DECODER_GETTING_PAYLOAD:
      while ( (nData < sizeOfPayload) && (c->decoder->buffer->count) )
      {
        BUFFER_GET_FAST(c->decoder->buffer, &rxBuf[nData]);
        nData++;
      }
      
      if (nData == sizeOfPayload)  // We got the payload, we analyze it directly
      {
        nData = 0;
        c->decoder->state = S_DECODER_ANALYZE_PAYLOAD;
      }
      else
      {
        return DECODER_RET_MSG_NO_MSG;
      }
      
    case S_DECODER_ANALYZE_PAYLOAD:
      switch (header.type)
      {
        case NEW_RNG_SEED:
          c->decoder->state = S_DECODER_STANDBY;
          if (sizeOfNewRngSeedPayload == sizeOfPayload) // Valid payload
          {
            memcpy(rxMsg, rxBuf, sizeOfPayload);
            return DECODER_RET_MSG_RNG_SEED;
          }
          else
          {
            return DECODER_RET_MSG_NO_MSG;
          }
          
        case START_ACQ:
          c->decoder->state = S_DECODER_STANDBY;
          if ( (PROTOCOL_START_ALGO == rxBuf[0]) && (sizeOfStartAcqPayload == sizeOfPayload) )
          {
            memcpy(rxMsg, &rxBuf[1], 1);
            return DECODER_RET_MSG_START_ALGO;
          }
          else
          {
            return DECODER_RET_MSG_NO_MSG;
          }
          
        case END_ACQ:
          c->decoder->state = S_DECODER_STANDBY;
          if ( (PROTOCOL_STOP_ALGO == rxBuf[0]) && (sizeOfStopAcqPayload == sizeOfPayload) )
          {
            return DECODER_RET_MSG_STOP_ALGO;
          }
          else
          {
            return DECODER_RET_MSG_NO_MSG;
          }
          
        default:
          c->decoder->state = S_DECODER_STANDBY;
          return DECODER_RET_MSG_NO_MSG;
      }
      
    default:
      c->decoder->state = S_DECODER_STANDBY;
      return DECODER_RET_MSG_NO_MSG;
  }
  //-------------------------------------------------------------
}


// Public functions
//==============================================================================

const CodecInterface_t * CodecInterface (void)
{
  return &_codec_if;
}