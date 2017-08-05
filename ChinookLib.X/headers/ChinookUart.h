/* 
 * File:   ChinookUart.h
 * Author: Frederic
 *
 * Created on 15 octobre 2014, 17:37
 */

#ifndef __CHINOOK_UART__
#define	__CHINOOK_UART__

#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>
#include "ChinookHardwareProfile.h"

#define ONLY_2_UART_BUFFERS


//==============================================================================
// General definitions
//==============================================================================

//=====================================
// General defs for UART transmission
//=====================================
#ifndef UART_LINE_BUFFER_LENGTH
#define UART_LINE_BUFFER_LENGTH 256
#endif // UART_LINE_BUFFER_LENGTH

#ifndef LINE_COMPLETE
#define LINE_COMPLETE   TRUE
#endif

#ifndef LINE_INCOMPLETE
#define LINE_INCOMPLETE FALSE
#endif
//=====================================


//=====================================
// BaudRate_t
// Purpose : set at which Baud rate
//           the UARTs will transmit.
//=====================================
typedef enum BaudRate_t
{
   BAUD50     =       50
  ,BAUD75     =       75
  ,BAUD110    =      110
  ,BAUD134    =      134
  ,BAUD150    =      150
  ,BAUD200    =      200
  ,BAUD300    =      300
  ,BAUD600    =      600
  ,BAUD1200   =     1200
  ,BAUD1800   =     1800
  ,BAUD2400   =     2400
  ,BAUD4800   =     4800
  ,BAUD9600   =     9600
  ,BAUD19200  =    19200
  ,BAUD38400  =    38400
  ,BAUD57600  =    57600
  ,BAUD115200 =   115200
} BaudRate_t;
//=====================================


//=====================================
// UartModule_t
// Purpose : set at which Baud rate
//           the UARTs will transmit.
//=====================================
typedef UART_MODULE UartModule_t;
//=====================================


//=====================================
// UartFifoMode_t
// Purpose : set the FIFO mode of the
//           UART
//=====================================
typedef UART_FIFO_MODE UartFifoMode_t;
//=====================================


//=====================================
// UartLineControlMode_t
// Purpose : set the data bit count, parity
//           and others
//=====================================
typedef UART_LINE_CONTROL_MODE UartLineCtrlMode_t;
//=====================================


//=====================================
// UartConfig_t
// Purpose : different UART configs
//=====================================
typedef UART_CONFIGURATION UartConfig_t;
//=====================================


//=========================================
// sUartLineBuffer_t
// Purpose : structure used when receiving
//           data from UART line
//=========================================
typedef struct sUartLineBuffer
{
  size_t length;
  UINT8 buffer [UART_LINE_BUFFER_LENGTH];
} sUartLineBuffer_t;
//=========================================


//=========================================
// sUartFifoBuffer_t
// Purpose : structure used for the FIFO
//           functions.
//=========================================
typedef struct sUartFifoBuffer
{
  UINT8             inIdx;
  UINT8             outIdx;
  BOOL              bufFull;
  BOOL              bufEmpty;
  const UINT16      maxBufSize;
  sUartLineBuffer_t lineBuffer;
} sUartFifoBuffer_t;
//=========================================


//=============================================
// struct sChinookUartVar
// Purpose : Variables used by UART interrupts
//=============================================
struct sChinookUartVar
{
  /**
   * @Description   DO NOT USE. This is the user's TX FIFO buffer. It is used in
   *                interrupts. One for each UART (uartTxFifo[UART1] is for UART1,
   *                uartTxFifo[UART2] is for UART2 and so on).
   */
//  volatile sUartLineBuffer_t uartTxFifo[6];
  volatile sUartFifoBuffer_t uartTxFifo[2];

  /**
   * @Description   DO NOT USE. This is the user's RX FIFO buffer. It is used in
   *                interrupts. One for each UART (uartRxFifo[UART1] is for UART1,
   *                uartRxFifo[UART2] is for UART2 and so on).
   */
//  volatile sUartLineBuffer_t uartRxFifo[6];
  volatile sUartFifoBuffer_t uartRxFifo[2];

  /**
   * @Description   Flag that tells if there is Received data in the RX FIFO. One
   *                for each UART (oIsRxDataAvailable[UART1] is for UART1,
   *                oIsRxDataAvailable[UART2] is for UART2 and so on.
   */
  volatile BOOL oIsRxDataAvailable[2];
};
//=============================================


//==============================================================================
// Public structure declaration
//==============================================================================

struct sChinookUart
{

  /**
   * @Prototype     static inline void UartOpen(UartModule_t uartModuleId, BaudRate_t baudRate, UartConfig_t oConfig, UartFifoMode_t oFifoMode, UartLineCtrlMode_t oLineControl);
   *
   * @Description   This function opens the UART with TX/RX enabled, 8 bits data,
   *                no parity and 1 stop bit. The Baud rate is specified by the
   *                user. IT DOES NOT ENABLE THE TX OR RX. For enabling the TX
   *                and RX, you must use Uart.EnableRx and Uart.EnableTx
   *
   * @Example       UartModule_t uartModule = UART1;\n
   *                BaudRate_t baudRate = BAUD9600;\n
   *                UartConfig_t oConfig = UART_ENABLE_PINS_TX_RX_ONLY;\n
   *                UartFifoMode_t oFifoMode = UART_INTERRUPT_ON_TX_DONE | UART_INTERRUPT_ON_RX_3_QUARTER_FULL;\n
   *                UartLineCtrlMode_t oLineControl = UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1;\n
   *                Uart.Open(uartModule, baudRate, oConfig, oFifoMode, oLineControl);\n
   *                Uart.EnableRx(uartModule);\n
   *                Uart.EnableTx(uartModule);\n\n
   *                This example opens the UART1 at a Baud rate of 9600 BAUD.
   *                The TX and RX pins are enabled, a TX interrupt will occur when
   *                a transfer is done, a RX interrupt will occur when the RX FIFO 
   *                is 3/4 full. The line operates on 8 bits data, no parity and 
   *                one stop bit. The RX and TX channels are enabled by the last
   *                2 lines.
   *
   * @Parameter     UART_MODULE uartModuleId\n
   *                Specifies which UART to open.\n
   *                Possibles values : \n
   *                UART1, UART2, UART3, UART4, UART5, UART6. If using Uno32, only
   *                UARTs available are UART1 and UART2.
   *
   * @Parameter     BaudRate_t BaudRate\n
   *                Specifies the needed Baud rate for the UART specified.\n
   *                Possible values : See typedef BaudRate_t
   *
   * @Parameter     UartConfig_t oConfig\n
   *                Specifies the configuration flags for the UART specified.\n
   *                Possible values : See definition of UART_CONFIGURATION.
   *
   * @Parameter     UartFifoMode_t oFifoMode\n
   *                Specifies the condition of RX/TX interrupts for the UART specified.\n
   *                Possible values : See definition of UART_FIFO_MODE.
   *
   * @Parameter     UartLineCtrlMode_t oLineControl\n
   *                Specifies the data configuration (number of bits, the parity and  
   *                number of stop bits) for the UART specified.\n
   *                Possible values : See definition of UART_LINE_CONTROL_MODE.
   *
   * @Returns       None.
   */
  void (*Open)  (UartModule_t uartModuleId, BaudRate_t baudRate, UartConfig_t oConfig, UartFifoMode_t oFifoMode, UartLineCtrlMode_t oLineControl);


  /**
   * @Prototype     static inline INT8 UartClose(UartModule_t uartModuleId);
   *
   * @Description   This function closes the UART specified.
   *
   * @Example       Uart.Close(UART6);\n
   *                This example close the UART6.
   *
   * @Parameter     UartModule_t uartModuleId\n
   *                Specifies which UART to close.\n
   *                Possibles values : \n
   *                UART1, UART2, UART3, UART4, UART5, UART6.
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Conditions of failure : Wrong arguments.
   */
  INT8 (*Close) (UartModule_t UartModuleId);


  /**
   * @Prototype   static inline void SendDataBuffer(UartModule_t uartModuleId, const UINT8 *buffer, UINT32 size);
   *
   * @Description This function sends (size) bytes of the buffer to the UART specified.
   *
   * @Example     Uart.SendDataBuffer(UART2, &txBuffer, 12);\n
   *              This example sends 12 bytes of txBuffer via UART 2.
   *
   * @Parameter   UartModule_t uartModuleId\n
   *              Specifies which UART to close.\n
   *              Possibles values : \n
   *              UART1, UART2, UART3, UART4, UART5, UART6.
   *
   * @Parameter   const UINT8 *buffer\n
   *              Buffer containing the data to send.
   *
   * @Parameter   UINT32 size\n
   *              Size in bytes to send.
   */
  void (*SendDataBuffer) (UartModule_t uartModuleId, const UINT8 *buffer, UINT32 size);


  /**
   * @Prototype   static inline void SendDataByte(UartModule_t uartModuleId, const UINT8 data);
   *
   * @Description This function sends a single byte to the UART module specified.
   *
   * @Example     Uart.SendDataByte(UART2, 0x23);\n
   *              This example sends the data 0x23 via UART 2.
   *
   * @Parameter   UartModule_t uartModuleId\n
   *              Specifies which UART to close.\n
   *              Possibles values : \n
   *              UART1, UART2, UART3, UART4, UART5, UART6.
   *
   * @Parameter   const UINT8 data\n
   *              Data byte to send.
   */
  void (*SendDataByte) (UartModule_t uartModuleId, const UINT8 data);


  /**
   * @Prototype   static inline BOOL UartGetDataBuffer (UINT8 uartModuleId, sUartLineBuffer_t * lineBuffer, BOOL reply);
   *
   * @Description This function reads and accumulates up to UART_LINE_BUFFER_LENGTH
   *              bytes into the lineBuffer structure. The function exit if
   *              there are no data to read or if an end of line char is met.
   *
   * @Example     sUartLineBuffer_t lineBuffer = \n
   *              {\n
   *                .length =  0\n
   *               ,.buffer = {0}\n
   *              };\n
   *              BOOL lineStatus = LINE_INCOMPLETE;\n
   *              lineStatus = Uart.GetDataBuffer(UART2, &lineBuffer, TRUE);\n\n
   *              This example will get a message from UART2 and exit when an 
   *              EOL character is received or after a timeout. The message received
   *              will be put in lineBuffer.buffer and the number of characters 
   *              put in lineBuffer.length. The 3rd parameter is set as TRUE, so 
   *              the function will echo everything it receives.
   *
   * @Parameter   UartModule_t uartModuleId\n
   *              Specifies which UART to read.\n
   *              Possibles values : \n
   *              UART1, UART2, UART3, UART4, UART5, UART6.
   *
   * @Parameter   sUartLineBuffer_t * lineBuffer\n
   *              Structure in which the received data and its length will be put.
   *              Contains the buffer and its length.
   *
   * @Parameter   BOOL reply\n
   *              Tells if the function must echo what it has received.\n
   *              Possibles values : \n
   *              TRUE : the function will echo what it receives\n
   *              FALSE : the function won't echo.
   *
   * @Returns     The line status (LINE_COMPLETE or LINE_INCOMPLETE).\n
   *              LINE_COMPLETE : an "End of File" character has been received.\n
   *              LINE_INCOMPLETE : the timeout has expired or the number of 
   *              bytes received is greater than UART_LINE_BUFFER_LENGTH.
   */
  BOOL (*GetDataBuffer) (UartModule_t uartModuleId, sUartLineBuffer_t * lineBuffer, BOOL reply);


  /**
   * @Prototype     static inline INT16 GetDataByte(UartModule_t uartModuleId);
   *
   * @Description   This function reads a single byte from the UART specified 
   *                and returns it. It waits for data to be in the RX buffer. If,
   *                after 1000 iterations, there is no data, the function returns
   *                -1.
   *
   * @Example       INT16 data = Uart.GetDataByte(UART2);\n
   *                if (data < 0)\n
   *                {\n
   *                  LED_ERROR = ON;\n
   *                }\n
   *                This example reads a byte from UART 2 and returns it. If the
   *                data returned is -1, light LEd_ERROR.
   *
   * @Parameter     UartModule_t uartModuleId\n
   *                Specifies which UART to close.\n
   *                Possibles values : \n
   *                UART1, UART2, UART3, UART4, UART5, UART6.
   *
   * @Returns       On success : Byte read\n
   *                On failure : -1\n
   *                Reasons of failure : timeout of 1000 iterations has expired.
   */
  INT16 (*GetDataByte) (UartModule_t uartModuleId);


  /**
   * @Prototype     static inline UINT8 UartConfigInterrupt(UartModule_t uartModuleId, UINT8 priority, UINT8 subPriority);
   *
   * @Description   This function configures the TX and RX interrupts at the
   *                priority and subpriority needed. It does not enables the
   *                interrupts. To enable them, use Uart.EnableTx/RxInterrupts();
   *
   * @Example       Uart.ConfigInterrupt(UART3, 7, 1);\n
   *                This example configures TX and RX interrupts for UART3 with a
   *                priority of 7 (highest) and subpriority of 1.
   *
   * @Parameter     UartModule_t uartModuleId\n
   *                Specifies which UART to configure the interrupts.\n
   *                Possibles values : \n
   *                UART1, UART2, UART3, UART4, UART5, UART6.
   *
   * @Parameter     UINT8 priority\n
   *                priority is the desired priority of the interrupt to set. Its values have
   *                a range from 0 to 7, seven (7) being the highest priority and
   *                one (1) being the lowest priority. Note that a priority of zero (0)
   *                will disable the interrupt.
   *
   * @Parameter     UINT8 subPriority\n
   *                subPriority is the desired subpriority of the interrupt to set.
   *                Its values have a range from 0 to 3, three (3) being the highest
   *                priority and zero (0) being the lowest priority.
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Reasons of failure : wrong arguments.
   */
  INT8 (*ConfigInterrupt) (UartModule_t uartModuleId, UINT8 priority, UINT8 subPriority);


  /**
   * @Prototype     static inline void UartEnableRx(UartModule_t uartModuleId);
   *
   * @Description   This function enables the RX for the UART specified.
   *
   * @Example       Uart.EnableRx(UART3);\n
   *                This example disables RX for UART3.
   *
   * @Parameter     UartModule_t uartModuleId\n
   *                Specifies which UART to close.\n
   *                Possibles values : \n
   *                UART1, UART2, UART3, UART4, UART5, UART6.
   *
   * @Returns       None.
   */
  void (*EnableRx) (UartModule_t uartModuleId);


  /**
   * @Prototype     static inline void UartEnableTx(UartModule_t uartModuleId);
   *
   * @Description   This function enables the TX for the UART specified.
   *
   * @Example       Uart.EnableTx(UART3);\n
   *                This example disables TX for UART3.
   *
   * @Parameter     UartModule_t uartModuleId\n
   *                Specifies which UART to close.\n
   *                Possibles values : \n
   *                UART1, UART2, UART3, UART4, UART5, UART6.
   *
   * @Returns       None.
   */
  void (*EnableTx) (UartModule_t uartModuleId);


  /**
   * @Prototype     static inline void UartEnableRxInterrupts(UartModule_t uartModuleId);
   *
   * @Description   This function enables the RX interrupts for the UART specified.
   *
   * @Example       Uart.EnableRxInterrupt(UART3);\n
   *                This example enables RX interrupts for UART3.
   *
   * @Parameter     UartModule_t uartModuleId\n
   *                Specifies which UART to close.\n
   *                Possibles values : \n
   *                UART1, UART2, UART3, UART4, UART5, UART6.
   *
   * @Returns       None.
   */
  void (*EnableRxInterrupts) (UartModule_t uartModuleId);

  
  /**
   * @Prototype     static inline void UartEnableTxInterrupts(UartModule_t uartModuleId);
   *
   * @Description   This function enables the TX interrupts for the UART specified.
   *
   * @Example       Uart.EnableTxInterrupt(UART3);\n
   *                This example enables TX interrupts for UART3.
   *
   * @Parameter     UartModule_t uartModuleId\n
   *                Specifies which UART to close.\n
   *                Possibles values : \n
   *                UART1, UART2, UART3, UART4, UART5, UART6.
   *
   * @Returns       None.
   */
  void (*EnableTxInterrupts) (UartModule_t uartModuleId);


  /**
   * @Prototype     static inline void UartDisableRx(UartModule_t uartModuleId);
   *
   * @Description   This function disables the RX for the UART specified.
   *
   * @Example       Uart.DisableRx(UART3);\n
   *                This example disables RX for UART3.
   *
   * @Parameter     UartModule_t uartModuleId\n
   *                Specifies which UART to close.\n
   *                Possibles values : \n
   *                UART1, UART2, UART3, UART4, UART5, UART6.
   *
   * @Returns       None.
   */
  void (*DisableRx) (UartModule_t uartModuleId);


  /**
   * @Prototype     static inline void UartDisableTx(UartModule_t uartModuleId);
   *
   * @Description   This function disables the TX for the UART specified.
   *
   * @Example       Uart.DisableTx(UART3);\n
   *                This example disables TX for UART3.
   *
   * @Parameter     UartModule_t uartModuleId\n
   *                Specifies which UART to close.\n
   *                Possibles values : \n
   *                UART1, UART2, UART3, UART4, UART5, UART6.
   *
   * @Returns       None.
   */
  void (*DisableTx) (UartModule_t uartModuleId);


  /**
   * @Prototype     static inline void UartDisableRxInterrupts(UartModule_t uartModuleId);
   *
   * @Description   This function disables the RX interrupts for the UART specified.
   *
   * @Example       Uart.DisableRxInterrupt(UART3);\n
   *                This example disables RX interrupts for UART3.
   *
   * @Parameter     UartModule_t uartModuleId\n
   *                Specifies which UART to close.\n
   *                Possibles values : \n
   *                UART1, UART2, UART3, UART4, UART5, UART6.
   *
   * @Returns       None.
   */
  void (*DisableRxInterrupts) (UartModule_t uartModuleId);


  /**
   * @Prototype     static inline void UartDisableTxInterrupts(UartModule_t uartModuleId);
   *
   * @Description   This function disables the TX interrupts for the UART specified.
   *
   * @Example       Uart.DisableTxInterrupt(UART3);\n
   *                This example disables TX interrupts for UART3.
   *
   * @Parameter     UartModule_t uartModuleId\n
   *                Specifies which UART to close.\n
   *                Possibles values : \n
   *                UART1, UART2, UART3, UART4, UART5, UART6.
   *
   * @Returns       None.
   */
  void (*DisableTxInterrupts) (UartModule_t uartModuleId);


  /**
   * @Prototype   static inline INT32 UartGetRxFifoBuffer (UartModule_t uartModuleId, sUartLineBuffer_t * buffer, BOOL reply);
   *
   * @Description This function will put the contents of the user's RX FIFO buffer
   *              into the *buffer passed in argument. During the reading of the
   *              RX FIFO buffer, the UART RX interrupts are disabled and reenabled
   *              afterwards. If, at the end of the process, the RX FIFO buffer
   *              is empty, the function clears the flag
   *              Uart.Var.oIsRxDataAvailable[uartModuleId].
   *
   * @Example     sUartLineBuffer_t buffer =\n
   *              {\n
   *               .buffer = {0}\n
   *              ,.length =  0\n
   *              };\n
   *              INT32 err;\n
   *              err = Uart.GetRxFifoBuffer(UART2, &buffer, FALSE);\n
   *              This example reads the content of the RX FIFO and puts it in
   *              buffer. The function won't echo what it has received.
   *
   * @Parameter   UartModule_t uartModuleId\n
   *              Specifies which UART to close.\n
   *              Possibles values : \n
   *              UART1, UART2, UART3, UART4, UART5, UART6.
   *
   * @Parameter   sUartLineBuffer_t * lineBuffer\n
   *              Structure in which the received data and its length will be put.
   *              Contains the buffer and its length.
   *
   * @Parameter   BOOL reply\n
   *              Tells if the function must echo what it has received.\n
   *              Possibles values : \n
   *              TRUE : the function will echo what it receives\n
   *              FALSE : the function won't echo.
   *
   * @Parameter   INT32\n
   *              On success : the number of bytes read\n
   *              On failure : -1.\n
   *              Reasons of failure : RX FIFO buffer was empty.
   */
  INT32 (*GetRxFifoBuffer) (UartModule_t uartModuleId, sUartLineBuffer_t * buffer, BOOL reply);


  /**
   * @Prototype   static inline INT32 UartPutTxFifoBuffer (UartModule_t uartModuleId, sUartLineBuffer_t * buffer);
   *
   * @Description This function will put the contents of the *buffer passed in
   *              argument into the user's TX FIFO buffer . During the writing
   *              of the TX FIFO buffer, the UART TX interrupts are disabled and
   *              enabled afterwards. This allows the interrupts to write to the
   *              UxTXREG by sequences of 8 bytes (which is the size of the
   *              PIC32 FIFO).
   *
   * @Example     sUartLineBuffer_t buffer =\n
   *              {\n
   *               .buffer = {0}\n
   *              ,.length =  0\n
   *              };\n
   *              // ...\n
   *              // code to fill local buffer\n
   *              // ...\n
   *              INT32 err;\n
   *              err = Uart.PutTxFifoBuffer(UART2, &buffer);\n
   *              This example will put the content of the local buffer in TX
   *              FIFO buffer (if there is enough space).
   *
   * @Parameter   UartModule_t uartModuleId\n
   *              Specifies which UART to close.\n
   *              Possibles values : \n
   *              UART1, UART2, UART3, UART4, UART5, UART6.
   *
   * @Parameter   sUartLineBuffer_t * lineBuffer\n
   *              Structure in which the received data and its length will be put.
   *              Contains the buffer and its length.
   *
   * @Parameter   INT32\n
   *              On success : the number of bytes written\n
   *              On failure : -1.\n
   *              Reasons of failure : there was not enough space in TX FIFO 
   *              buffer OR the buffer passed in argument was empty.
   */
  INT32 (*PutTxFifoBuffer) (UartModule_t uartModuleId, sUartLineBuffer_t * buffer);


  /**
   * @Description   Contains certains variables useful for the UART. Write a dot
   *                "." after writing "Var" to see a description of the variables
   *                available.
   */
  struct sChinookUartVar Var;

};


//==============================================================================
// Public functions prototypes
//==============================================================================
inline INT8 UartFifoWrite (sUartFifoBuffer_t *fifo, UINT8 *data);
inline INT8 UartFifoRead  (sUartFifoBuffer_t *fifo, UINT8 *data);


#endif	/* __CHINOOK_UART__ */

