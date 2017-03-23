//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Byte Buffer
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : ByteBuffer.c
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the byte buffer methods.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "ByteBuffer.h"


// Private definitions
//==============================================================================


// Private prototypes
//==============================================================================



// Private variables
//==============================================================================



// Private functions
//==============================================================================


// Public functions
//==============================================================================

INT8 ByteBufferAdd (ByteBuffer_t *buf, UINT8 *byte)
{
  if (BUFFER_IS_FULL(buf))
  {
    return -1;
  }
  BUFFER_ADD_FAST(buf, byte);
  return 0;
}


INT8 ByteBufferGet (ByteBuffer_t *buf, UINT8 *byte)
{
  if (BUFFER_IS_EMPTY(buf))
  {
    return -1;
  }
  
  BUFFER_GET_FAST(buf, byte);
  return 0;
}


void ByteBufferInit (ByteBuffer_t *buf, UINT8 *workBuffer, size_t bufSize)
{
  ByteBufferReset(buf);
  buf->bufSize = bufSize;
  buf->buf = workBuffer;
}


void ByteBufferReset (ByteBuffer_t *buf)
{
  buf->count = buf->inIdx = buf->outIdx = 0;
}