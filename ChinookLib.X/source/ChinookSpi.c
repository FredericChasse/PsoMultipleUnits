#include "..\headers\ChinookSpi.h"


//==============================================================================
// Private functions prototypes for the SPI
//==============================================================================

static inline void    SpiClose                  (SpiNum_t numSpi);
static inline INT8    SpiConfigInterrupt        (SpiNum_t numSpi, UINT8 priority, UINT8 subPriority);
static inline void    SpiDisableChannel         (SpiNum_t numSpi);
static inline void    SpiDisableRxInterrupts    (SpiNum_t numSpi);
static inline void    SpiDisableTxInterrupts    (SpiNum_t numSpi);
static inline void    SpiEnableChannel          (SpiNum_t numSpi);
static inline INT8    SpiIsSpiBusy              (SpiNum_t numSpi);
static inline void    SpiEnableRxInterrupts     (SpiNum_t numSpi);
static inline void    SpiEnableTxInterrupts     (SpiNum_t numSpi);
static inline UINT32  SpiGetCharacter           (SpiNum_t numSpi);
static inline UINT32  SpiGetDataBuffer          (SpiNum_t numSpi, UINT32 *buffer            , UINT32 max_size);
static inline INT8    SpiOpen                   (SpiNum_t numSpi, SpiOpenFlags_t oOpenFlags , UINT32 bitRate);
static inline void    SpiSendCharacter          (SpiNum_t numSpi, const UINT32 data);
static inline void    SpiSendDataBuffer         (SpiNum_t numSpi, const UINT32 *buffer      , UINT32 size);
static inline INT32   SpiPutTxFifoBuffer        (SpiNum_t numSpi, sSpiLineBuffer_t * buffer);
static inline INT32   SpiGetRxFifoBuffer        (SpiNum_t numSpi, sSpiLineBuffer_t * buffer , BOOL reply);
static inline UINT32  SpiBlockingTransfer       (SpiNum_t numSpi, const UINT32 data);

//==============================================================================
// Public structure of functions for the SPI
//==============================================================================

struct sChinookSpi Spi =
{
   .Close                 = &SpiClose
  ,.ConfigInterrupt       = &SpiConfigInterrupt
  ,.DisableChannel        = &SpiDisableChannel
  ,.DisableRxInterrupts   = &SpiDisableRxInterrupts
  ,.DisableTxInterrupts   = &SpiDisableTxInterrupts
  ,.EnableChannel         = &SpiEnableChannel
  ,.GetCharacter          = &SpiGetCharacter
  ,.EnableRxInterrupts    = &SpiEnableRxInterrupts
  ,.EnableTxInterrupts    = &SpiEnableTxInterrupts
  ,.IsSpiBusy             = &SpiIsSpiBusy
  ,.Open                  = &SpiOpen
  ,.SendCharacter         = &SpiSendCharacter
  ,.GetRxFifoBuffer       = &SpiGetRxFifoBuffer
  ,.PutTxFifoBuffer       = &SpiPutTxFifoBuffer
  ,.BlockingTransfer      = &SpiBlockingTransfer
};

SpiEnhancedBuffer_t  userEnhancedTxBuffer = {0}
                    ,userEnhancedRxBuffer = {0}
                    ;


//==============================================================================
// Declarations of private functions
//==============================================================================


static inline UINT32 SpiBlockingTransfer (SpiNum_t numSpi, const UINT32 data)
{
  Spi.SendCharacter(numSpi, data);
  while(Spi.IsSpiBusy(numSpi));
  return Spi.GetCharacter(numSpi);
}


static inline void SpiClose(SpiNum_t numSpi)
{
  SpiChnClose(numSpi + 1);
}


static inline INT8 SpiIsSpiBusy (SpiNum_t numSpi)
{
  if (numSpi > SPI4)
  {
    return -1;
  }
  else
  {
    return SpiChnIsBusy(numSpi + 1);
  }
}


static inline INT8 SpiConfigInterrupt (SpiNum_t numSpi, UINT8 priority, UINT8 subPriority)
{
  INT8 err = 0;

  if ( (priority > 7) || (subPriority > 3) )  // Wrong arguments
  {
    return -1;  // EXIT_FAILURE
  }

  INTEnable(INT_SOURCE_SPI_TX(numSpi + 1), INT_DISABLED);   // Disable RX interrupts
  INTEnable(INT_SOURCE_SPI_RX(numSpi + 1), INT_DISABLED);   // Disable TX interrupts

  // Configure SPI Interrupt priority and subpriority
  INTSetVectorPriority    (INT_VECTOR_SPI (numSpi + 1), priority    );
  INTSetVectorSubPriority (INT_VECTOR_SPI (numSpi + 1), subPriority );

  return err;  // EXIT_SUCCESS
}


static inline void SpiDisableRxInterrupts(SpiNum_t numSpi)
{
  INTEnable(INT_SOURCE_SPI_RX(numSpi + 1), INT_DISABLED);   // Disable RX interrupts
}


static inline void SpiDisableTxInterrupts(SpiNum_t numSpi)
{
  INTEnable(INT_SOURCE_SPI_TX(numSpi + 1), INT_DISABLED);   // Disable TX interrupts
}


static inline void SpiEnableRxInterrupts(SpiNum_t numSpi)
{
  INTEnable(INT_SOURCE_SPI_RX(numSpi + 1), INT_ENABLED);   // Enable RX interrupts
}


static inline void SpiEnableTxInterrupts(SpiNum_t numSpi)
{
  INTEnable(INT_SOURCE_SPI_TX(numSpi + 1), INT_ENABLED);   // Enable TX interrupts
}


static inline void SpiDisableChannel (SpiNum_t numSpi)
{
  SpiChnEnable(numSpi + 1, 0);
}


static inline void SpiEnableChannel (SpiNum_t numSpi)
{
  SpiChnEnable(numSpi + 1, 1);
}


static inline INT8 SpiOpen(SpiNum_t numSpi, SpiOpenFlags_t oOpenFlags, UINT32 bitRate)
{
  INT8 err = 0;

  UINT16 srcClkDivider;
  
  UINT16 mode16, mode32;

//  if (  (oOpenFlags && SPI_MASTER_MODE)
//     && (oOpenFlags && SPI_SLAVE_MODE ) )  // Conflicting mode of operation
//  {
//    return -1;  // EXIT_FAILURE
//  }
//  if (  (oOpenFlags && SPI_MASTER_SS)
//     && (oOpenFlags && SPI_SLAVE_SS ) )  // Conflicting mode of operation
//  {
//    return -1;  // EXIT_FAILURE
//  }
//  if (  (oOpenFlags && SPI_8_BITS_CHAR )
//     && (oOpenFlags && SPI_16_BITS_CHAR)
//     && (oOpenFlags && SPI_32_BITS_CHAR) )  // Conflicting mode of operation
//  {
//    return -1;  // EXIT_FAILURE
//  }
//  if (  (oOpenFlags && SPI_TX_EVENT_BUFFER_NOT_FULL  )
//     && (oOpenFlags && SPI_TX_EVENT_BUFFER_HALF_EMPTY)
//     && (oOpenFlags && SPI_TX_EVENT_BUFFER_EMPTY     )
//     && (oOpenFlags && SPI_TX_EVENT_BUFFER_SR_EMPTY  ) )  // Conflicting mode of operation
//  {
//    return -1;  // EXIT_FAILURE
//  }
//  if (  (oOpenFlags && SPI_RX_EVENT_BUFFER_FULL     )
//     && (oOpenFlags && SPI_RX_EVENT_BUFFER_HALF_FULL)
//     && (oOpenFlags && SPI_RX_EVENT_BUFFER_NOT_EMPTY)
//     && (oOpenFlags && SPI_RX_EVENT_BUFFER_EMPTY    ) )  // Conflicting mode of operation
//  {
//    return -1;  // EXIT_FAILURE
//  }

  // srcClkDivider is the divider used to obtain
  // the needed bitrate from the system clock.
//  srcClkDivider = GetSystemClock() / bitRate;
  srcClkDivider = GetPeripheralClock() / bitRate;

  if (srcClkDivider < 2)    // Minimum value for the divider
  {
    return -1;
  }

  // Open SPI channel
  SpiChnOpen(numSpi + 1, oOpenFlags, srcClkDivider);
  
#if defined __32MX795F512L__      // SPI1, 2, 3, 4 only on Chipkit MAX32
  switch (numSpi)
  {
    case SPI1   :     mode16 = SPI2CONbits.MODE16;
                      mode32 = SPI2CONbits.MODE32;
                      break;
                      
    case SPI2   :     mode16 = SPI2CONbits.MODE16;
                      mode32 = SPI2CONbits.MODE32;
                      break;
                      
    case SPI3   :     mode16 = SPI3CONbits.MODE16;
                      mode32 = SPI3CONbits.MODE32;
                      break;
                      
    case SPI4   :     mode16 = SPI4CONbits.MODE16;
                      mode32 = SPI4CONbits.MODE32;
                      break;
                      
    default     :     mode16 = SPI1CONbits.MODE16;
                      mode32 = SPI1CONbits.MODE32;
                      break;
  }
#elif defined __32MX320F128H__    // SPI3, 4 not on this device (Uno32)
  switch (numSpi)
  {
    case SPI1   :     mode16 = SPI2CONbits.MODE16;
                      mode32 = SPI2CONbits.MODE32;
                      break;
                      
    case SPI2   :     mode16 = SPI2CONbits.MODE16;
                      mode32 = SPI2CONbits.MODE32;
                      break;
                      
    default     :     mode16 = SPI1CONbits.MODE16;
                      mode32 = SPI1CONbits.MODE32;
                      break;
  }
#elif defined __32MX795F512H__    // SPI1 not on this device
  switch (numSpi)
  {                      
    case SPI2   :     mode16 = SPI2CONbits.MODE16;
                      mode32 = SPI2CONbits.MODE32;
                      break;
                      
    case SPI3   :     mode16 = SPI3CONbits.MODE16;
                      mode32 = SPI3CONbits.MODE32;
                      break;
                      
    case SPI4   :     mode16 = SPI4CONbits.MODE16;
                      mode32 = SPI4CONbits.MODE32;
                      break;
                      
    default     :     mode16 = SPI2CONbits.MODE16;
                      mode32 = SPI2CONbits.MODE32;
                      break;
  }
#endif
  
  // Init FIFO buffers
  Spi.Var.oIsRxDataAvailable[0]            = 0;

  Spi.Var.spiRxFifo[0].bufEmpty            = 1;
  Spi.Var.spiRxFifo[0].bufFull             = 0;
  Spi.Var.spiRxFifo[0].inIdx               = 0;
  Spi.Var.spiRxFifo[0].outIdx              = 0;
  Spi.Var.spiRxFifo[0].lineBuffer.length   = 0;

  Spi.Var.spiTxFifo[0].bufEmpty            = 1;
  Spi.Var.spiTxFifo[0].bufFull             = 0;
  Spi.Var.spiTxFifo[0].inIdx               = 0;
  Spi.Var.spiTxFifo[0].outIdx              = 0;
  Spi.Var.spiTxFifo[0].lineBuffer.length   = 0;
  
  if      (mode16)    // 16 bits words
  {
    Spi.Var.spiRxFifo[0].maxBufSize        = SPI_LINE_BUFFER_LENGTH / 2;
    Spi.Var.spiTxFifo[0].maxBufSize        = SPI_LINE_BUFFER_LENGTH / 2;
    Spi.Var.spiRxFifo[0].lenghtOfData      = sizeof(UINT16);
    Spi.Var.spiTxFifo[0].lenghtOfData      = sizeof(UINT16);
    Spi.Var.spiRxFifo[0].lineBuffer.buffer = &userEnhancedRxBuffer.fifo16Bits;
    Spi.Var.spiTxFifo[0].lineBuffer.buffer = &userEnhancedTxBuffer.fifo16Bits;
    Spi.Var.spiRxFifo[0].maxWordsPerInt    = 8;
    Spi.Var.spiTxFifo[0].maxWordsPerInt    = 8;
  }
  else if (mode32)    // 32 bits words
  {
    Spi.Var.spiRxFifo[0].maxBufSize        = SPI_LINE_BUFFER_LENGTH / 4;
    Spi.Var.spiTxFifo[0].maxBufSize        = SPI_LINE_BUFFER_LENGTH / 4;
    Spi.Var.spiRxFifo[0].lenghtOfData      = sizeof(UINT32);
    Spi.Var.spiTxFifo[0].lenghtOfData      = sizeof(UINT32);
    Spi.Var.spiRxFifo[0].lineBuffer.buffer = &userEnhancedRxBuffer.fifo32Bits;
    Spi.Var.spiTxFifo[0].lineBuffer.buffer = &userEnhancedTxBuffer.fifo32Bits;
    Spi.Var.spiRxFifo[0].maxWordsPerInt    = 4;
    Spi.Var.spiTxFifo[0].maxWordsPerInt    = 4;
  }
  else                            // 8 bits words
  {
    Spi.Var.spiRxFifo[0].maxBufSize        = SPI_LINE_BUFFER_LENGTH;
    Spi.Var.spiTxFifo[0].maxBufSize        = SPI_LINE_BUFFER_LENGTH;
    Spi.Var.spiRxFifo[0].lenghtOfData      = sizeof(UINT8);
    Spi.Var.spiTxFifo[0].lenghtOfData      = sizeof(UINT8);
    Spi.Var.spiRxFifo[0].lineBuffer.buffer = &userEnhancedRxBuffer.fifo8Bits;
    Spi.Var.spiTxFifo[0].lineBuffer.buffer = &userEnhancedTxBuffer.fifo8Bits;
    Spi.Var.spiRxFifo[0].maxWordsPerInt    = 16;
    Spi.Var.spiTxFifo[0].maxWordsPerInt    = 16;
  }

  return err;   // EXIT SUCCESS
}


static inline UINT32 SpiGetCharacter(SpiNum_t numSpi)
{
  return SpiChnGetC(numSpi + 1);
}


static inline void SpiSendCharacter(SpiNum_t numSpi, const UINT32 data)
{
  SpiChnPutC(numSpi + 1, data);
}


static inline INT32 SpiGetRxFifoBuffer (SpiNum_t numSpi, sSpiLineBuffer_t * buffer, BOOL reply)
{
  UINT32 data;

  UINT16 nDataAvailable, i;

  INT8 err;

  if (Spi.Var.spiRxFifo[0].bufEmpty)
  {
    return -1;
  }
  
  size_t sizeOfData = Spi.Var.spiRxFifo[0].lenghtOfData;

  Spi.DisableRxInterrupts(numSpi);

  nDataAvailable = Spi.Var.spiRxFifo[0].lineBuffer.length;

  for (i = 0; i < nDataAvailable; i++)
  {
    err = SpiFifoRead((void *) &Spi.Var.spiRxFifo[0], &data);
    if (err < 0)
    {
      break;
    }
    memcpy(buffer->buffer + (i * sizeOfData), &data, sizeOfData);
    err = i + 1;
  }

  Spi.Var.oIsRxDataAvailable[0] = 0;

  Spi.EnableRxInterrupts(numSpi);

  if (err >= 0)
  {
    buffer->length = err;
  }

  if ( reply && (err >= 0) )
  {
    SpiPutTxFifoBuffer (numSpi, buffer);
  }

  return err;
}


static inline INT32 SpiPutTxFifoBuffer (SpiNum_t numSpi, sSpiLineBuffer_t * buffer)
{

  UINT16 nSpaceAvailable, i;
  INT8 err;

  if (buffer->length == 0)
  {
    return -1;
  }

  Spi.DisableTxInterrupts(numSpi);

  if (Spi.Var.spiTxFifo[0].bufFull)
  {
    Spi.EnableTxInterrupts(numSpi);
    return -1;
  }
  
  size_t sizeOfData = Spi.Var.spiRxFifo[0].lenghtOfData;

  nSpaceAvailable = Spi.Var.spiTxFifo[0].maxBufSize - Spi.Var.spiTxFifo[0].lineBuffer.length;

  if (nSpaceAvailable < buffer->length)
  {
    if (!Spi.Var.spiTxFifo[0].bufEmpty)
    {
      Spi.EnableTxInterrupts(numSpi);
    }
    return -1;
  }

  for (i = 0; i < buffer->length; i++)
  {
    err = SpiFifoWrite((void *) &Spi.Var.spiTxFifo[0], Spi.Var.spiTxFifo[0].lineBuffer.buffer + (i * sizeOfData));
    if (err < 0)
    {
      break;
    }
  }

  Spi.EnableTxInterrupts(numSpi);

  return i;
}


//INT8 SpiFifoWrite(sSpiFifoBuffer_t *fifo, UINT32 *data)
inline INT8 SpiFifoWrite(sSpiFifoBuffer_t *fifo, void *data)
{
  if (fifo->bufFull)
  {
    return -1;
  }
  fifo->bufEmpty = 0;
//  fifo->lineBuffer.buffer[fifo->inIdx] = *data;    // Check
  memcpy(fifo->lineBuffer.buffer + (fifo->inIdx * fifo->lenghtOfData), data, fifo->lenghtOfData);
  fifo->inIdx = (fifo->inIdx + 1) % fifo->maxBufSize;
  if (fifo->inIdx == fifo->outIdx)
  {
    fifo->bufFull = 1;
  }
  fifo->lineBuffer.length++;
  return 0;
}


//INT8 SpiFifoRead (sSpiFifoBuffer_t *fifo, UINT32 *data)
inline INT8 SpiFifoRead (sSpiFifoBuffer_t *fifo, void *data)
{
  if (fifo->bufEmpty)
  {
    return -1;
  }
  fifo->bufFull = 0;
  memcpy(data, fifo->lineBuffer.buffer + (fifo->outIdx * fifo->lenghtOfData), fifo->lenghtOfData);
//  *data = fifo->lineBuffer.buffer[fifo->outIdx];   // Check
  fifo->outIdx = (fifo->outIdx + 1) % fifo->maxBufSize;
  if (fifo->outIdx == fifo->inIdx)
  {
    fifo->bufEmpty = 1;
  }
  fifo->lineBuffer.length--;
  return 0;
}