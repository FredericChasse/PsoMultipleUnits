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
#include "MathFunctions.h"


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
#ifdef ONLY_2_UART_BUFFERS
  UINT8           uartBufIdx;
#endif
} Codec_t;

// Private prototypes
//==============================================================================

UINT8               _Codec_Init             (Codec_t *c, UartModule_t uartChannel);
DecoderReturnMsg_t  _Codec_DecoderFsmStep   (Codec_t *c, UINT8 *rxMsg);
UINT8               _Codec_CodeNewUnitsMsg  (Codec_t *c, ProtocolUnitsDataPayload_t *newMsg);
UINT8               _Codec_CodeNewPsoMsg    (Codec_t *c, ProtocolPsoDataPayload_t *newMsg);
UINT8               _Codec_CodeNewAdcMsg    (Codec_t *c, ProtocolAdcDataPayload_t *newMsg);

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
 ,.CodeNewUnitsMsg  = (CodecCodeNewUnitsMsg_fct)  &_Codec_CodeNewUnitsMsg
 ,.CodeNewPsoMsg    = (CodecCodeNewPsoMsg_fct)    &_Codec_CodeNewPsoMsg
 ,.CodeNewAdcMsg    = (CodecCodeNewAdcMsg_fct)    &_Codec_CodeNewAdcMsg
};

// Private functions
//==============================================================================

UINT8 _Codec_Init (Codec_t *c, UartModule_t uartChannel)
{
  c->uartChannel      = uartChannel;
#ifdef ONLY_2_UART_BUFFERS
  if (uartChannel == U_MATLAB)
  {
    c->uartBufIdx = U_MATLAB_IDX;
  }
  else
  {
    c->uartBufIdx = U_DBG_IDX;
  }
#endif
  c->decoder          = &_decoder;
  c->decoder->buffer  = &_decoderBuf;
  c->decoder->state   = S_DECODER_STANDBY;
  
  ByteBufferInit(c->decoder->buffer, _decoderArray, MAX_DECODER_LENGTH);
}

UINT8 _Codec_CodeNewAdcMsg (Codec_t *c, ProtocolAdcDataPayload_t *newMsg)
{
  sUartLineBuffer_t buf = {0};
  size_t length = newMsg->nUnits * newMsg->nData * sizeOfAdcOneData;
  size_t tmpLen = length + sizeOfAdcDataPayloadBase;
  size_t left = length + sizeOfAdcDataPayloadBase + sizeOfProtocolHeader;
  UINT16 nData = newMsg->nData;
  UINT16 ptr = 0;
  
  ProtocolHeader_t header = 
  {
    .delimiter        = PROTOCOL_DELIMITER
   ,.type             = ADC_DATA
   ,.lengthOfPayload  = sizeOfAdcDataPayloadBase + length
  };
  
  while(left > (sizeOfAdcDataPayloadBase + sizeOfProtocolHeader))
  {
    buf.length = 0;
    tmpLen = MIN(left, UART_LINE_BUFFER_LENGTH);
    if (tmpLen < UART_LINE_BUFFER_LENGTH)
    {
      newMsg->oNewPacket = 0;
    }
    else
    {
      newMsg->oNewPacket = 1;
    }
    tmpLen -= sizeOfAdcDataPayloadBase + sizeOfProtocolHeader;
    tmpLen &= ~1;
    
    header.lengthOfPayload = tmpLen + sizeOfAdcDataPayloadBase;
    
    memcpy(&buf.buffer[0], &header, sizeOfProtocolHeader);
    buf.length += sizeOfProtocolHeader;
  
    memcpy(&buf.buffer[buf.length], &newMsg->oNewPacket, sizeOfAdcONewPacket);
    buf.length += sizeOfAdcONewPacket;
  
    memcpy(&buf.buffer[buf.length], &newMsg->nUnits, sizeOfAdcNUnits);
    buf.length += sizeOfAdcNUnits;
    
    nData = tmpLen / newMsg->nUnits / sizeOfAdcOneData;

    memcpy(&buf.buffer[buf.length], &nData, sizeOfAdcNData);
    buf.length += sizeOfAdcNData;

    memcpy(&buf.buffer[buf.length], &newMsg->data[ptr], tmpLen);
    buf.length += tmpLen;
    ptr+=nData;

#ifdef ONLY_2_UART_BUFFERS
    while(!Uart.Var.uartTxFifo[c->uartBufIdx].bufEmpty);
#else
    while(!Uart.Var.uartTxFifo[c->uartChannel].bufEmpty);
#endif
    while(Uart.PutTxFifoBuffer(c->uartChannel, &buf) < 0);
    
    left -= tmpLen;
  }
  
  return 0;
}

UINT8 _Codec_CodeNewPsoMsg (Codec_t *c, ProtocolPsoDataPayload_t *newMsg)
{
  sUartLineBuffer_t buf = {0};
  size_t length = newMsg->nParticles * newMsg->nData * 4;
  
  ProtocolHeader_t header = 
  {
    .delimiter        = PROTOCOL_DELIMITER
   ,.type             = PSO_DATA
   ,.lengthOfPayload  = sizeOfPsoDataPayloadBase + length * 3
  };
  memcpy(&buf.buffer[0], &header, sizeOfProtocolHeader);
  buf.length += sizeOfProtocolHeader;
  
  memcpy(&buf.buffer[buf.length], &newMsg->iteration, sizeOfPsoIteration);
  buf.length += sizeOfPsoIteration;
  
  memcpy(&buf.buffer[buf.length], &newMsg->nParticles, sizeOfPsoNParticles);
  buf.length += sizeOfPsoNParticles;
  
  memcpy(&buf.buffer[buf.length], &newMsg->nData, sizeOfPsoNData);
  buf.length += sizeOfPsoNData;
  
  memcpy(&buf.buffer[buf.length], newMsg->speed, length);
  buf.length += length;
  
  memcpy(&buf.buffer[buf.length], newMsg->pos, length);
  buf.length += length;
  
  memcpy(&buf.buffer[buf.length], newMsg->fitness, length);
  buf.length += length;
  
  while(Uart.PutTxFifoBuffer(c->uartChannel, &buf) < 0);
  
  return 0;
}


UINT8 _Codec_CodeNewUnitsMsg (Codec_t *c, ProtocolUnitsDataPayload_t *newMsg)
{
  sUartLineBuffer_t buf = {0};
  size_t length = newMsg->nUnits * newMsg->nData * 4;
  
  ProtocolHeader_t header = 
  {
    .delimiter        = PROTOCOL_DELIMITER
   ,.type             = UNITS_DATA
   ,.lengthOfPayload  = sizeOfUnitsDataPayloadBase + length * 2
  };
  memcpy(&buf.buffer[0], &header, sizeOfProtocolHeader);
  buf.length += sizeOfProtocolHeader;
  
  memcpy(&buf.buffer[buf.length], &newMsg->timestamp_ms, sizeOfUnitsTimestamp);
  buf.length += sizeOfUnitsTimestamp;
  
  memcpy(&buf.buffer[buf.length], &newMsg->nUnits, sizeOfUnitsNUnits);
  buf.length += sizeOfUnitsNUnits;
  
  memcpy(&buf.buffer[buf.length], &newMsg->nData, sizeOfUnitsNData);
  buf.length += sizeOfUnitsNData;
  
  memcpy(&buf.buffer[buf.length], newMsg->positions, length);
  buf.length += length;
  
  memcpy(&buf.buffer[buf.length], newMsg->powers, length);
  buf.length += length;
  
  while(Uart.PutTxFifoBuffer(c->uartChannel, &buf) < 0);
  
  return 0;
}


DecoderReturnMsg_t _Codec_DecoderFsmStep (Codec_t *c, UINT8 *rxMsg)
{
  sUartLineBuffer_t buffer;
  UINT8 i;
  INT32 err;
  UINT8 nUnits;
  static UINT32 dbgCount = 0;
  // Get new characters pending
  //-------------------------------------------------------------
  
#ifdef ONLY_2_UART_BUFFERS
  if (Uart.Var.oIsRxDataAvailable[c->uartBufIdx])
#else
  if (Uart.Var.oIsRxDataAvailable[c->uartChannel])
#endif
  {
    err = Uart.GetRxFifoBuffer(c->uartChannel, &buffer, FALSE);
    if (err >= 0)
    {
      for (i = 0; i < buffer.length; i++)
      {
        BUFFER_ADD_FAST(c->decoder->buffer, &buffer.buffer[i]);
        dbgCount++;
      }
    }
  }
  //-------------------------------------------------------------
  
  // Decoding RX messages
  //-------------------------------------------------------------
  static ProtocolHeader_t header = {0};
  UINT8 byte;
  static UINT8 sizeOfPayload;
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
          nUnits = rxBuf[2];
          if (nUnits)
          {
            if ( (PROTOCOL_START_ALGO == rxBuf[0]) && ((sizeOfStartAcqPayloadBase + nUnits) == sizeOfPayload) )
            {
              memcpy(rxMsg, &rxBuf[1], sizeOfStartAcqPayloadBase + nUnits);
              return DECODER_RET_MSG_START_ALGO;
            }
            else
            {
              return DECODER_RET_MSG_NO_MSG;
            }
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