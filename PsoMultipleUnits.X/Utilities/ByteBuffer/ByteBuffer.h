//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Byte Buffer
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : ByteBuffer.h
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the byte buffer methods.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __BYTE_BUFFER_H__
#define __BYTE_BUFFER_H__

#include "Setup.h"

// Public definitions
//==============================================================================

typedef struct
{
  UINT8 *buf;
  size_t inIdx;
  size_t outIdx;
  size_t bufSize;
  size_t count;
} ByteBuffer_t;


// Public functions
//==============================================================================

/*!
 * Define method to add a byte to a circular buffer. This method has no protection,
 * so use it carefully.
 */
#define BUFFER_ADD_FAST(_b, _c)     { (_b)->buf[(_b)->inIdx] = *(_c); \
                                      (_b)->inIdx = (_b)->inIdx + 1 == (_b)->bufSize ? 0 : (_b)->inIdx + 1; \
                                      (_b)->count++; }

/*!
 * Define method to get a byte from a circular buffer. This method has no protection,
 * so use it carefully.
 */
#define BUFFER_GET_FAST(_b, _c)     { *(_c) = (_b)->buf[(_b)->outIdx]; \
                                      (_b)->outIdx = (_b)->outIdx + 1 == (_b)->bufSize ? 0 : (_b)->outIdx + 1; \
                                      (_b)->count--; }

/*!
 * Define method to check if a buffer is full.
 */
#define BUFFER_IS_FULL(_b)          ( (_b)->count == (_b)->bufSize )

/*!
 * Define method to check if a buffer is empty.
 */
#define BUFFER_IS_EMPTY(_b)         ( (_b)->count == 0 )

/*!
 * Define method to reset a buffer.
 */
#define BUFFER_RESET(_b)            { (_b)->count = (_b)->inIdx = (_b)->outIdx = 0; }


/*!
 * Add a byte to a buffer.
 * \return 0 on success, -1 if buffer is full.
 */
INT8  ByteBufferAdd   (ByteBuffer_t *buf, UINT8 *byte);

/*!
 * Get a byte from a buffer.
 * \return 0 on success, -1 if buffer is empty.
 */
INT8  ByteBufferGet   (ByteBuffer_t *buf, UINT8 *byte);

/*!
 * Init a buffer.
 * \param workBuffer The static array used by the circular buffer.
 * \param bufSize The size of workBuffer.
 */
void  ByteBufferInit  (ByteBuffer_t *buf, UINT8 *workBuffer, size_t bufSize);

/*!
 * Reset the indexes and counter of a buffer.
 */
void  ByteBufferReset (ByteBuffer_t *buf);



#endif // __BYTE_BUFFER_H__