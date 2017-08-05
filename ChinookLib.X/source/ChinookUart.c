#include "..\headers\ChinookUart.h"

//==============================================================================
// Private function prototypes
//==============================================================================
static inline void    UartOpen                (UartModule_t uartModuleId, BaudRate_t BaudRate, UartConfig_t oConfig, UartFifoMode_t oFifoMode, UartLineCtrlMode_t oLineControl);
static inline INT8    UartClose               (UartModule_t uartModuleId);
static inline void    UartSendDataByte        (UartModule_t uartModuleId, const UINT8 data);
static inline void    UartSendDataBuffer      (UartModule_t uartModuleId, const UINT8 *buffer , UINT32 size);
static inline INT16   UartGetDataByte         (UartModule_t uartModuleId);
static inline BOOL    UartGetDataBuffer       (UartModule_t uartModuleId, sUartLineBuffer_t * lineBuffer, BOOL reply);
static inline INT32   UartGetRxFifoBuffer     (UartModule_t uartModuleId, sUartLineBuffer_t * buffer, BOOL reply);
static inline INT32   UartPutTxFifoBuffer     (UartModule_t uartModuleId, sUartLineBuffer_t * buffer);
static inline INT8    UartConfigInterrupt     (UartModule_t uartModuleId, UINT8 priority, UINT8 subPriority);
static inline void    UartEnableRx            (UartModule_t uartModuleId);
static inline void    UartEnableTx            (UartModule_t uartModuleId);
static inline void    UartEnableRxInterrupts  (UartModule_t uartModuleId);
static inline void    UartEnableTxInterrupts  (UartModule_t uartModuleId);
static inline void    UartDisableRx           (UartModule_t uartModuleId);
static inline void    UartDisableTx           (UartModule_t uartModuleId);
static inline void    UartDisableRxInterrupts (UartModule_t uartModuleId);
static inline void    UartDisableTxInterrupts (UartModule_t uartModuleId);


//==============================================================================
// Public structure of functions for the UART
//==============================================================================
struct sChinookUart Uart =
{
   .Open                    = &UartOpen
  ,.Close                   = &UartClose
  ,.ConfigInterrupt         = &UartConfigInterrupt
  ,.DisableRx               = &UartDisableRx
  ,.DisableTx               = &UartDisableTx
  ,.DisableRxInterrupts     = &UartDisableRxInterrupts
  ,.DisableTxInterrupts     = &UartDisableTxInterrupts
  ,.EnableRx                = &UartEnableRx
  ,.EnableTx                = &UartEnableTx
  ,.EnableRxInterrupts      = &UartEnableRxInterrupts
  ,.EnableTxInterrupts      = &UartEnableTxInterrupts
  ,.SendDataByte            = &UartSendDataByte
  ,.GetDataByte             = &UartGetDataByte
  ,.SendDataBuffer          = &UartSendDataBuffer
  ,.GetDataBuffer           = &UartGetDataBuffer
  ,.GetRxFifoBuffer         = &UartGetRxFifoBuffer
  ,.PutTxFifoBuffer         = &UartPutTxFifoBuffer
   
  // FIFO INIT
  ,.Var.uartRxFifo[0].lineBuffer.buffer = {0}
  ,.Var.uartRxFifo[0].maxBufSize        = UART_LINE_BUFFER_LENGTH
  ,.Var.uartRxFifo[1].lineBuffer.buffer = {0}
  ,.Var.uartRxFifo[1].maxBufSize        = UART_LINE_BUFFER_LENGTH
   
  ,.Var.uartTxFifo[0].lineBuffer.buffer = {0}
  ,.Var.uartTxFifo[0].maxBufSize        = UART_LINE_BUFFER_LENGTH
  ,.Var.uartTxFifo[1].lineBuffer.buffer = {0}
  ,.Var.uartTxFifo[1].maxBufSize        = UART_LINE_BUFFER_LENGTH
   
};


//==============================================================================
// Private functions declarations
//==============================================================================

static inline void UartOpen(UartModule_t uartModuleId, BaudRate_t baudRate, UartConfig_t oConfig, UartFifoMode_t oFifoMode, UartLineCtrlMode_t oLineControl)
{
  UINT8 bufIdx;
  if (uartModuleId == UART3)
  {
    bufIdx = 0;
  }
  else if (uartModuleId == UART6)
  {
    bufIdx = 1;
  }
  UARTConfigure(uartModuleId, oConfig);
  UARTSetFifoMode(uartModuleId, oFifoMode);
  UARTSetLineControl(uartModuleId, oLineControl);
  UARTSetDataRate(uartModuleId, GetPeripheralClock(), baudRate);

  // Init FIFO buffers
  Uart.Var.oIsRxDataAvailable[bufIdx]           =  0;

  Uart.Var.uartRxFifo[bufIdx].bufEmpty          =  1;
  Uart.Var.uartRxFifo[bufIdx].bufFull           =  0;
  Uart.Var.uartRxFifo[bufIdx].inIdx             =  0;
  Uart.Var.uartRxFifo[bufIdx].outIdx            =  0;
  Uart.Var.uartRxFifo[bufIdx].lineBuffer.length =  0;

  Uart.Var.uartTxFifo[bufIdx].bufEmpty          =  1;
  Uart.Var.uartTxFifo[bufIdx].bufFull           =  0;
  Uart.Var.uartTxFifo[bufIdx].inIdx             =  0;
  Uart.Var.uartTxFifo[bufIdx].outIdx            =  0;
  Uart.Var.uartTxFifo[bufIdx].lineBuffer.length =  0;
}

static inline INT8 UartClose(UartModule_t uartModuleId)
{
  INT8 err = 0;

#ifndef __32MX320F128H__    // Uno32 doesn't have UART3-6
  switch (uartModuleId)
  {
    case UART1    :   U1MODECLR = (1<<_U1MODE_UARTEN_POSITION);
                      break;
    case UART2    :   U2MODECLR = (1<<_U2MODE_UARTEN_POSITION);
                      break;
    case UART3    :   U3MODECLR = (1<<_U3MODE_UARTEN_POSITION);
                      break;
    case UART4    :   U4MODECLR = (1<<_U4MODE_UARTEN_POSITION);
                      break;
    case UART5    :   U5MODECLR = (1<<_U5MODE_UARTEN_POSITION);
                      break;
    case UART6    :   U6MODECLR = (1<<_U6MODE_UARTEN_POSITION);
                      break;
    default       :   err = -1;   // Wrong argument
                      break;
  }
#else
  switch (uartModuleId)
  {
    case UART1    :   U1MODECLR = (1<<_U1MODE_UARTEN_POSITION);
                      break;
    case UART2    :   U2MODECLR = (1<<_U2MODE_UARTEN_POSITION);
                      break;
    default       :   err = -1;   // Wrong argument
                      break;
  }
#endif
  return err;
}


static inline INT32 UartGetRxFifoBuffer (UartModule_t uartModuleId, sUartLineBuffer_t * buffer, BOOL reply)
{
  UINT8 data;

  UINT16 nDataAvailable, i;

  INT8 err;
  UINT8 bufIdx;
  if (uartModuleId == UART3)
  {
    bufIdx = 0;
  }
  else if (uartModuleId == UART6)
  {
    bufIdx = 1;
  }

  if (Uart.Var.uartRxFifo[bufIdx].bufEmpty)
  {
    return -1;
  }

  Uart.DisableRxInterrupts(uartModuleId);

  nDataAvailable = Uart.Var.uartRxFifo[bufIdx].lineBuffer.length;

  for (i = 0; i < nDataAvailable; i++)
  {
    err = UartFifoRead((void *) &Uart.Var.uartRxFifo[bufIdx], &data);
    if (err < 0)
    {
      break;
    }
    buffer->buffer[i] = data;
    err = i + 1;
  }

  Uart.Var.oIsRxDataAvailable[bufIdx] = 0;

  Uart.EnableRxInterrupts(uartModuleId);

  if (err >= 0)
  {
    buffer->length = err;
  }

  if ( reply && (err >= 0) )
  {
    UartPutTxFifoBuffer (uartModuleId, buffer);
  }

  return err;
}


static inline INT32 UartPutTxFifoBuffer (UartModule_t uartModuleId, sUartLineBuffer_t * buffer)
{
  UINT8 bufIdx;
  if (uartModuleId == UART3)
  {
    bufIdx = 0;
  }
  else if (uartModuleId == UART6)
  {
    bufIdx = 1;
  }

  UINT16 nSpaceAvailable, i;
  INT8 err;

  if (buffer->length == 0)
  {
    return -1;
  }

  Uart.DisableTxInterrupts(uartModuleId);

  if (Uart.Var.uartTxFifo[bufIdx].bufFull)
  {
    Uart.EnableTxInterrupts(uartModuleId);
    return -1;
  }

  nSpaceAvailable = Uart.Var.uartTxFifo[bufIdx].maxBufSize - Uart.Var.uartTxFifo[bufIdx].lineBuffer.length;

  if (nSpaceAvailable < buffer->length)
  {
    if (!Uart.Var.uartTxFifo[bufIdx].bufEmpty)
    {
      Uart.EnableTxInterrupts(uartModuleId);
    }
    return -1;
  }

  for (i = 0; i < buffer->length; i++)
  {
    err = UartFifoWrite((void *) &Uart.Var.uartTxFifo[bufIdx], &buffer->buffer[i]);
    if (err < 0)
    {
      break;
    }
  }

  Uart.EnableTxInterrupts(uartModuleId);

  return i;
}


// *****************************************************************************
// static inline void UartSendDataBuffer(UINT8 uartModuleId, const UINT8 *buffer, UINT32 size)
// *****************************************************************************
static inline void UartSendDataBuffer(UartModule_t uartModuleId, const UINT8 *buffer, UINT32 size)
{
  while(size)
  {
    while(!UARTTransmitterIsReady(uartModuleId));

    UARTSendDataByte(uartModuleId, *buffer);

    buffer++;
    size--;
  }

  while(!UARTTransmissionHasCompleted(uartModuleId));
}

// *****************************************************************************
// static inline void UartSendDataByte(UINT8 uartModuleId, const UINT8 data)
// *****************************************************************************
static inline void UartSendDataByte(UartModule_t uartModuleId, const UINT8 data)
{
  while(!UARTTransmitterIsReady(uartModuleId));

  UARTSendDataByte(uartModuleId, data);

  while(!UARTTransmissionHasCompleted(uartModuleId));
}

// *****************************************************************************
// static inline UINT32GetDataBuffer(UartModule_t uartModuleId, sUartLineBuffer_t * lineBuffer, BOOL reply)
// *****************************************************************************
static inline BOOL UartGetDataBuffer(UartModule_t uartModuleId, sUartLineBuffer_t * lineBuffer, BOOL reply)
{
  INT16 character;
  while(lineBuffer->length < UART_LINE_BUFFER_LENGTH && UARTReceivedDataIsAvailable(uartModuleId))
  {
    character = Uart.GetDataByte(uartModuleId);
    if (character < 0)
    {
      return LINE_INCOMPLETE;   // Timeout has expired
    }

    if ( reply )
    {
      Uart.SendDataByte(uartModuleId, character);
    }

    switch (character)
    {
      case '\r': // Carriage return
        lineBuffer->buffer[ lineBuffer->length ] = '\0';
        Uart.SendDataByte(uartModuleId, '\n');
        return LINE_COMPLETE;
        break;
      case 0x8: // Backspace
        if ( lineBuffer->length > 0 )
        {
            lineBuffer->length--;
        }
        break;
      default:
        lineBuffer->buffer[ lineBuffer->length ] = character;
        lineBuffer->length++;
    }
  }

  return LINE_INCOMPLETE;
}

// *****************************************************************************
// static inline UINT8 UartGetDataByte(UINT8 uartModuleId)
// *****************************************************************************
static inline INT16 UartGetDataByte(UartModule_t uartModuleId)
{
  UINT16 iTimeOut = 0;

  while(!UARTReceivedDataIsAvailable(uartModuleId) && (iTimeOut < 1000))
  {
    iTimeOut++;
  }

  if (iTimeOut >= 1000)
  {
    return -1;
  }
  else
  {
    return UARTGetDataByte(uartModuleId);
  }
}

// *****************************************************************************
// static inline INT8 UartConfigInterrupt(UartModule_t uartModuleId, UINT8 priority, UINT8 subPriority)
// *****************************************************************************
static inline INT8 UartConfigInterrupt(UartModule_t uartModuleId, UINT8 priority, UINT8 subPriority)
{
  INT8 err = 0;

  if ( (priority > 7) || (subPriority > 3) )  // Wrong arguments
  {
    return -1;  // EXIT_FAILURE
  }

  INTEnable(INT_SOURCE_UART_RX(uartModuleId), INT_DISABLED);   // Disable RX interrupts
  INTEnable(INT_SOURCE_UART_TX(uartModuleId), INT_DISABLED);   // Disable TX interrupts

  // Configure UART Interrupt priority and subpriority
  INTSetVectorPriority    (INT_VECTOR_UART (uartModuleId), priority    );
  INTSetVectorSubPriority (INT_VECTOR_UART (uartModuleId), subPriority );
  
  return err;  // EXIT_SUCCESS
}

// *****************************************************************************
// static inline void UartEnableRx(UartModule_t uartModuleId)
// *****************************************************************************
static inline void UartEnableRx(UartModule_t uartModuleId)
{
  UARTEnable(uartModuleId, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX));   // Enable RX
}

// *****************************************************************************
// static inline void UartEnableTx(UartModule_t uartModuleId)
// *****************************************************************************
static inline void UartEnableTx(UartModule_t uartModuleId)
{
  UARTEnable(uartModuleId, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_TX));   // Enable TX
}


// *****************************************************************************
// static inline void UartEnableRxInterrupt(UartModule_t uartModuleId)
// *****************************************************************************
static inline void UartEnableRxInterrupts(UartModule_t uartModuleId)
{
  INTEnable(INT_SOURCE_UART_RX(uartModuleId), INT_ENABLED);   // Enable RX interrupts
}

// *****************************************************************************
// static inline void UartEnableTxInterrupts(UartModule_t uartModuleId)
// *****************************************************************************
static inline void UartEnableTxInterrupts(UartModule_t uartModuleId)
{
  INTEnable(INT_SOURCE_UART_TX(uartModuleId), INT_ENABLED);   // Enable TX interrupts
}

// *****************************************************************************
// static inline void UartDisableRx(UartModule_t uartModuleId)
// *****************************************************************************
static inline void UartDisableRx(UartModule_t uartModuleId)
{
//  UARTEnable(uartModuleId, UART_DISABLE_FLAGS(UART_PERIPHERAL | UART_RX));   // Disable RX
  UARTEnable(uartModuleId, UART_DISABLE_FLAGS(UART_RX));   // Disable RX
}

// *****************************************************************************
// static inline void UartDisableTx(UartModule_t uartModuleId)
// *****************************************************************************
static inline void UartDisableTx(UartModule_t uartModuleId)
{
//  UARTEnable(uartModuleId, UART_DISABLE_FLAGS(UART_PERIPHERAL | UART_TX));   // Disable TX
  UARTEnable(uartModuleId, UART_DISABLE_FLAGS(UART_TX));   // Disable TX
}

// *****************************************************************************
// static inline void UartDisableRxInterrupt(UartModule_t uartModuleId)
// *****************************************************************************
static inline void UartDisableRxInterrupts(UartModule_t uartModuleId)
{
  INTEnable(INT_SOURCE_UART_RX(uartModuleId), INT_DISABLED);   // Disable RX interrupts
}

// *****************************************************************************
// static inline void UartDisableTxInterrupts(UartModule_t uartModuleId)
// *****************************************************************************
static inline void UartDisableTxInterrupts(UartModule_t uartModuleId)
{
  INTEnable(INT_SOURCE_UART_TX(uartModuleId), INT_DISABLED);   // Disable TX interrupts
}


inline INT8 UartFifoWrite(sUartFifoBuffer_t *fifo, UINT8 *data)
{
  if (fifo->bufFull)
  {
    return -1;
  }
  fifo->bufEmpty = 0;
  fifo->lineBuffer.buffer[fifo->inIdx] = *data;
  fifo->inIdx = (fifo->inIdx + 1) % fifo->maxBufSize;
  if (fifo->inIdx == fifo->outIdx)
  {
    fifo->bufFull = 1;
  }
  fifo->lineBuffer.length++;
  return 0;
}


inline INT8 UartFifoRead (sUartFifoBuffer_t *fifo, UINT8 *data)
{
  if (fifo->bufEmpty)
  {
    return -1;
  }
  fifo->bufFull = 0;
  *data = fifo->lineBuffer.buffer[fifo->outIdx];
  fifo->outIdx = (fifo->outIdx + 1) % fifo->maxBufSize;
  if (fifo->outIdx == fifo->inIdx)
  {
    fifo->bufEmpty = 1;
  }
  fifo->lineBuffer.length--;
  return 0;
}