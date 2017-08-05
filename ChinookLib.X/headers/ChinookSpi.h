
/*
 * File:   ChinookSpi.h
 * Author: Frederic
 *
 * Created on 15 octobre 2014, 17:36
 */

#ifndef __CHINOOK_SPI__
#define	__CHINOOK_SPI__

#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>
#include "ChinookHardwareProfile.h"


//==============================================================================
// General definitions
//==============================================================================


//=========================================
// Type for the SPI channels.
//=========================================
#if defined __32MX795F512L__      // SPI1, 2, 3, 4 only on Chipkit MAX32
typedef enum
{
   SPI1  = SPI_CHANNEL1 - 1       // -1 because SPI_CHANNEL1 == 1 and we want it at 0
  ,SPI2  = SPI_CHANNEL2 - 1       // -1 because SPI_CHANNEL2 == 2 and we want it at 1
  ,SPI3  = SPI_CHANNEL3 - 1       // -1 because SPI_CHANNEL3 == 3 and we want it at 2
  ,SPI4  = SPI_CHANNEL4 - 1       // -1 because SPI_CHANNEL4 == 4 and we want it at 3
} SpiNum_t;
#elif defined __32MX320F128H__    // SPI3, 4 not on this device (Uno32)
typedef enum
{
   SPI1  = SPI_CHANNEL1 - 1       // -1 because SPI_CHANNEL1 == 1 and we want it at 0
  ,SPI2  = SPI_CHANNEL2 - 1       // -1 because SPI_CHANNEL2 == 2 and we want it at 1
} SpiNum_t;
#elif defined __32MX795F512H__    // SPI1 not on this device
typedef enum
{
   SPI2  = SPI_CHANNEL2 - 1       // -1 because SPI_CHANNEL2 == 2 and we want it at 1
  ,SPI3  = SPI_CHANNEL3 - 1       // -1 because SPI_CHANNEL3 == 3 and we want it at 2
  ,SPI4  = SPI_CHANNEL4 - 1       // -1 because SPI_CHANNEL4 == 4 and we want it at 3
} SpiNum_t;
#endif


//=========================================
// Type for the flags of configuration.
//=========================================
#ifndef __32MX320F128H__    // Uno32 has different open flags

typedef enum
{
  // Master or slave configuration
  //===========================================================
   SPI_MASTER_MODE    =   SPI_OPEN_MSTEN    // Set the Master mode
  ,SPI_SLAVE_MODE     =   SPI_OPEN_SLVEN    // Set the Slave mode
  //===========================================================

  // Sampling phase of the data for the master
  //===========================================================
  ,SPI_SAMPLE_END_CLK =   SPI_OPEN_SMP_END  // Master Sample Phase for the input bit at the end of the
                                            // data out time. Otherwise, data is sampled in the middle.
  //===========================================================

  // Data out mode
  //===========================================================
  ,SPI_CLK_IDLE_HIGH_ACT_LOW  = SPI_OPEN_CKP_HIGH // Set the clock polarity to (idle-high, active-low). 
                                                  // Otherwise is (idle-low, active-high).
  ,SPI_DATA_ON_CLK_FEDGE      = SPI_OPEN_CKE_REV  // transmit from active to idle. Otherwise transmit 
                                                  // when clock goes from idle to active.
  //===========================================================
        
  // Enable the use of Slave Select (SS) pin
  //===========================================================
  ,SPI_MASTER_SS      =   SPI_OPEN_MSSEN    // Enable the driving of the Slave Select (SS) output pin by the Master
  ,SPI_SLAVE_SS       =   SPI_OPEN_SSEN     // Enable the SS input pin.
  //===========================================================

  // Lenght of a word
  //===========================================================
  ,SPI_8_BITS_CHAR    =   SPI_OPEN_MODE8    // Set 8 bits/char
  ,SPI_16_BITS_CHAR   =   SPI_OPEN_MODE16   // Set 16 bits/char
  ,SPI_32_BITS_CHAR   =   SPI_OPEN_MODE32   // Set 32 bits/char
  //===========================================================

  // Enhanced buffer (FIFO instead of single word) 
  // configuration. Do not use if you want to use the
  // standard buffer mode
  //===========================================================
  ,SPI_ENHANCED_BUFFER_MODE         =   SPI_OPEN_ENHBUF           // Enable the enhanced buffer mode

  ,SPI_TX_EVENT_BUFFER_NOT_FULL     =   SPI_OPEN_TBE_NOT_FULL     // Tx Buffer event issued when Tx buffer not full (at least one slot empty)
  ,SPI_TX_EVENT_BUFFER_HALF_EMPTY   =   SPI_OPEN_TBE_HALF_EMPTY   // Tx Buffer event issued when Tx buffer >= 1/2 empty
  ,SPI_TX_EVENT_BUFFER_EMPTY        =   SPI_OPEN_TBE_EMPTY        // Tx Buffer event issued when Tx buffer completely empty
  ,SPI_TX_EVENT_BUFFER_SR_EMPTY     =   SPI_OPEN_TBE_SR_EMPTY     /* Tx Buffer event issued when the last character is shifted out of the internal
                                                                     Shift Register and the transmit is complete */

  ,SPI_RX_EVENT_BUFFER_FULL         =   SPI_OPEN_RBF_FULL         // Rx Buffer event issued when RX buffer is full
  ,SPI_RX_EVENT_BUFFER_HALF_FULL    =   SPI_OPEN_RBF_HALF_FULL    // Rx Buffer event issued when RX buffer is >= 1/2 full
  ,SPI_RX_EVENT_BUFFER_NOT_EMPTY    =   SPI_OPEN_RBF_NOT_EMPTY    // Rx Buffer event issued when RX buffer is not empty
  ,SPI_RX_EVENT_BUFFER_EMPTY        =   SPI_OPEN_RBF_EMPTY        // Rx Buffer event issued when RX buffer is empty (the last character in the buffer is read).
  //===========================================================
} SpiOpenFlags_t;

#else     // Uno32

typedef SpiOpenFlags SpiOpenFlags_t;

#endif


//=====================================
// General defs for SPI transmission
//=====================================
#ifndef SPI_LINE_BUFFER_LENGTH
#define SPI_LINE_BUFFER_LENGTH 256
#endif // SPI_LINE_BUFFER_LENGTH
//=====================================


//=========================================
// sUartLineBuffer_t
// Purpose : structure used when receiving
//           data from UART line
//=========================================
typedef union 
{
  union
  {
    UINT8  fifo8Bits [SPI_LINE_BUFFER_LENGTH    ];
    UINT16 fifo16Bits[SPI_LINE_BUFFER_LENGTH / 2];
    UINT32 fifo32Bits[SPI_LINE_BUFFER_LENGTH / 4];
  };
} SpiEnhancedBuffer_t;

//typedef struct sSpiLineBuffer
//{
//  size_t              length;
//  SpiEnhancedBuffer_t buffer;
//} sSpiLineBuffer_t;

typedef struct sSpiLineBuffer
{
  size_t  length;
  void   *buffer;
} sSpiLineBuffer_t;
//=========================================


//=========================================
// sUartFifoBuffer_t
// Purpose : structure used for the FIFO
//           functions.
//=========================================
typedef struct sSpiFifoBuffer
{
  UINT8             inIdx;
  UINT8             outIdx;
  BOOL              bufFull;
  BOOL              bufEmpty;
  size_t            lenghtOfData;
  size_t            maxWordsPerInt;
  UINT16            maxBufSize;
  sSpiLineBuffer_t  lineBuffer;
} sSpiFifoBuffer_t;
//=========================================


//=============================================
// struct sChinookSpiVar
// Purpose : Variables used by SPI interrupts
//=============================================
struct sChinookSpiVar
{
  /**
   * @Description   DO NOT USE. This is the user's TX FIFO buffer. It is used in
   *                interrupts. One for each SPI (spiTxFifo[SPI1] is for SPI1,
   *                spiTxFifo[SPI2] is for SPI2 and so on).
   */
  volatile sSpiFifoBuffer_t spiTxFifo[4];

  /**
   * @Description   DO NOT USE. This is the user's RX FIFO buffer. It is used in
   *                interrupts. One for each SPI (spiRxFifo[SPI1] is for SPI1,
   *                spiRxFifo[SPI2] is for SPI2 and so on).
   */
  volatile sSpiFifoBuffer_t spiRxFifo[4];

  /**
   * @Description   Flag that tells if there is Received data in the RX FIFO. One
   *                for each SPI (oIsRxDataAvailable[SPI1] is for SPI1,
   *                oIsRxDataAvailable[SPI2] is for SPI2 and so on.
   */
  volatile BOOL oIsRxDataAvailable[4];
};
//=============================================


//=========================================
// Public structure of SPI functions
//=========================================
struct sChinookSpi
{

  /**
   * @Prototype   static inline void SpiClose (SpiNum_t numSpi);
   *
   * @Description This function closes the SPI module specified by the argument.
   *
   * @Example     err = Spi.Close(SPI1);\n
   *              This example closes the SPI1.
   *
   * @Param       SpiNum_t numSpi\n
   *              SpiNum_t is an enum which can take the values SPI1, SPI2,
   *              SPI3 and SPI4.
   *
   * @Return      None.
   */
  void (*Close) (SpiNum_t numSpi);

  /**
   * @Prototype   static inline INT8 SpiIsSpiBusy (SpiNum_t numSpi);
   *
   * @Description This function returns the status of the SPI module specified 
   *              by the argument.
   *
   * @Example     err = Spi.IsSpiBusy(SPI1);\n
   *              This example puts the state of the SPI1 in err.
   *
   * @Param       SpiNum_t numSpi\n
   *              SpiNum_t is an enum which can take the values SPI1, SPI2,
   *              SPI3 and SPI4.
   *
   * @Return      -1 on failure (wrong argument)\n
   *              0 if SPI channel IS NOT busy\n
   *              1 if SPI channel IS busy
   */
  INT8 (*IsSpiBusy) (SpiNum_t numSpi);


  /**
   * @Prototype   static inline INT8 SpiConfigInterrupt (SpiNum_t numSpi, UINT8 priority, UINT8 subPriority);
   *
   * @Description This function configures the interrupt of the SPI module specified
   *              by the argument numSpi to the priority and subpriority wanted
   *              WITHOUT ENABLING IT. To enable the interrupt, one must
   *              use the function Spi.EnableTx/RxInterrupts after configuring it.
   *
   * @Example     err = Spi.ConfigInterrupt(SPI1, 1, 3);\n
   *              This example configures the interrupt for SPI1 with a priority
   *              of 1 and a subpriority of 3.
   *
   * @Param       SpiNum_t numSpi\n
   *              SpiNum_t is an enum which can take the values SPI1 (not on the
   *              64 pins PIC32MX795F512H, SPI2, SPI3 and SPI4.
   *
   * @Param       priority is the desired priority of the interrupt to set. Its values have
   *              a range from 0 to 7, seven (7) being the highest priority and
   *              one (1) being the lowest priority. Note that a priority of zero (0)
   *              will disable the interrupt.
   *
   * @Param       subPriority is the desired subpriority of the interrupt to set.
   *              Its values have a range from 0 to 3, three (3) being the highest
   *              priority and zero (0) being the lowest priority.
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : one or more arguments are invalid.
   */
  INT8 (*ConfigInterrupt) (SpiNum_t numSpi, UINT8 priority, UINT8 subPriority);


  /**
   * @Prototype   static inline INT32 SpiOpen (SpiNum_t numSpi, SpiOpenFlags_t oOpenFlags, UINT32 bitRate);
   *
   * @Description This function opens the SPI module specified by the argument numSpi
   *              with the operation mode specified with argument oOpenFlags at the
   *              bitrate specified by argument bitRate.
   *
   * @Example     SpiOpenFlags_t oFlags =   SPI_MASTER_MODE
   *                                      | SPI_MASTER_SS
   *                                      | SPI_16_BITS_CHAR
   *                                      | SPI_ENHANCED_BUFFER_MODE
   *                                      | SPI_TX_EVENT_BUFFER_EMPTY
   *                                      | SPI_RX_EVENT_BUFFER_NOT_EMPTY
   *                                      ;\n\n
   *              err = Spi.Open(SPI1, oFlags, 20e6);\n\n
   *              This example opens the SPI1 in Master Mode. It uses the Slave
   *              Select (SS) pin. The buffer is in enhanced mode. The length of
   *              a word is 16 bits, so the FIFO of the buffer is 128/16 = 8 words
   *              deep. An TxBuffer event is triggered when the TxBuffer is
   *              completely empty. An TxBuffer event is triggered when the TxBuffer
   *              is completely empty.
   *
   * @Example     SpiOpenFlags_t oFlags =   SPI_SLAVE_MODE
   *                                      | SPI_8_BITS_CHAR
   *                                      ;\n\n
   *              err = Spi.Open(SPI1, oFlags, 20e6);\n\n
   *              This example opens the SPI1 in Slave Mode. Since it's not
   *              specified, it doesn't use the Slave Select (SS) pin. Since it's
   *              not specified, the buffer is in standard mode. The length of
   *              a word is 8 bits.
   *
   * @Param       SpiNum_t numSpi\n
   *              SpiNum_t is an enum which can take the values SPI1 (not on the
   *              64 pins PIC32MX795F512H, SPI2, SPI3 and SPI4.
   *
   * @Param       SpiOpenFlags_t oOpenFlags\n
   *              The flags specifying the operating mode of the SPI channel.\n\n
   *              SpiOpenFlags is a enum which can take the values : \n\n
   *              // Master of slave configuration\n
   *              SPI_MASTER_MODE    // Set the Master mode\n
   *              SPI_SLAVE_MODE     // Set the Slave mode\n
   * 
   *              // Sampling phase of the data for the master\n
   *              SPI_SAMPLE_END_CLK // Master Sample Phase for the input bit at the end of the
                                        data out time. Otherwise, data is sampled in the middle.\n
   *
   *              // Data out mode\n
   *              SPI_CLK_IDLE_HIGH_ACT_LOW // Set the clock polarity to (idle-high, active-low). Otherwise is (idle-low, active-high). \n
   *              SPI_DATA_ON_CLK_FEDGE // transmit from active to idle. Otherwise transmit
                                           when clock goes from idle to active.\n
   *
   *              // Enable the use of Slave Select (SS) pin\n
   *              SPI_MASTER_SS      // Enable the driving of the Slave Select (SS) output pin by the Master\n
   *              SPI_SLAVE_SS       // Enable the SS input pin.\n
   *
   *              // Lenght of a word\n
   *              SPI_8_BITS_CHAR    // Set 8 bits/char\n
   *              SPI_16_BITS_CHAR   // Set 16 bits/char\n
   *              SPI_32_BITS_CHAR   // Set 32 bits/char\n
   *
   *              // Enhanced buffer (FIFO instead of single word) configuration. Do not use if you want to use the standard buffer mode\n
   *              SPI_ENHANCED_BUFFER_MODE         // Enable the enhanced buffer mode\n
   *              SPI_TX_EVENT_BUFFER_NOT_FULL     // Tx Buffer event issued when Tx buffer not full (at least one slot empty)\n
   *              SPI_TX_EVENT_BUFFER_HALF_EMPTY   // Tx Buffer event issued when Tx buffer >= 1/2 empty\n
   *              SPI_TX_EVENT_BUFFER_EMPTY        // Tx Buffer event issued when Tx buffer completely empty\n
   *              SPI_TX_EVENT_BUFFER_SR_EMPTY     // Tx Buffer event issued when the last character is shifted out of the internal
   *                                                    Shift Register and the transmit is complete\n
   *
   *              SPI_RX_EVENT_BUFFER_FULL         // Rx Buffer event issued when RX buffer is full\n
   *              SPI_RX_EVENT_BUFFER_HALF_FULL    // Rx Buffer event issued when RX buffer is >= 1/2 full\n
   *              SPI_RX_EVENT_BUFFER_NOT_EMPTY    // Rx Buffer event issued when RX buffer is not empty\n
   *              SPI_RX_EVENT_BUFFER_EMPTY        // Rx Buffer event issued when RX buffer is empty (the last character in the buffer is read).
   *
   * @Param       UINT32 bitRate\n
   *              This is the desired bitrate in Hertz. For example, a desired
   *              bitrate of 20MHz would be "20e6".
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : spiNum is invalid\n
   *                                      bitrate is out of range (bitrate must
   *                                      be smaller than or equal to GetPeripheralClock()/2.
   *                                      GetPeripheralClock is SYS_FREQ/FPBDIV (config bit).
   */
  INT8 (*Open) (SpiNum_t numSpi, SpiOpenFlags_t oOpenFlags, UINT32 bitRate);


  /**
   * @Prototype     static inline void SpiDisableRxInterrupts(SpiNum_t spiNum);
   *
   * @Description   This function disables the RX interrupts for the SPI specified.
   *
   * @Example       Spi.DisableRxInterrupt(SPI2);\n
   *                This example disables RX interrupts for SPI2.
   *
   * @Param         SpiNum_t numSpi\n
   *                SpiNum_t is an enum which can take the values SPI1 (not on the
   *                64 pins PIC32MX795F512H, SPI2, SPI3 and SPI4.
   *
   * @Returns       None.
   */
  void (*DisableRxInterrupts) (SpiNum_t spiNum);


  /**
   * @Prototype     static inline void SpiDisableTxInterrupts(SpiNum_t spiNum);
   *
   * @Description   This function disables the TX interrupts for the SPI specified.
   *
   * @Example       Spi.DisableRxInterrupt(SPI3);\n
   *                This example disables TX interrupts for SPI3.
   *
   * @Param         SpiNum_t numSpi\n
   *                SpiNum_t is an enum which can take the values SPI1, SPI2,
   *                SPI3 and SPI4.
   *
   * @Returns       None.
   */
  void (*DisableTxInterrupts) (SpiNum_t spiNum);


  /**
   * @Prototype     static inline void SpiEnableRxInterrupts(SpiNum_t spiNum);
   *
   * @Description   This function enables the RX interrupts for the SPI specified.
   *
   * @Example       Spi.EnableRxInterrupt(SPI4);\n
   *                This example enables RX interrupts for SPI4.
   *
   * @Param         SpiNum_t numSpi\n
   *                SpiNum_t is an enum which can take the values SPI1 (not on the
   *                64 pins PIC32MX795F512H, SPI2, SPI3 and SPI4.
   *
   * @Returns       None.
   */
  void (*EnableRxInterrupts) (SpiNum_t spiNum);


  /**
   * @Prototype     static inline void SpiEnableTxInterrupts(SpiNum_t spiNum);
   *
   * @Description   This function enables the TX interrupts for the SPI specified.
   *
   * @Example       Spi.EnableTxInterrupt(SPI4);\n
   *                This example enables TX interrupts for SPI4.
   *
   * @Param         SpiNum_t numSpi\n
   *                SpiNum_t is an enum which can take the values SPI1 (not on the
   *                64 pins PIC32MX795F512H, SPI2, SPI3 and SPI4.
   *
   * @Returns       None.
   */
  void (*EnableTxInterrupts) (SpiNum_t spiNum);


  /**
   * @Prototype     static inline void SpiDisableChannel(SpiNum_t spiNum);
   *
   * @Description   This function disables the SPI channel specified.
   *
   * @Example       Spi.DisableChannel(SPI4);\n
   *                This example disables the SPI4.
   *
   * @Param         SpiNum_t numSpi\n
   *                SpiNum_t is an enum which can take the values SPI1 (not on the
   *                64 pins PIC32MX795F512H, SPI2, SPI3 and SPI4.
   *
   * @Returns       None.
   */
  void (*DisableChannel) (SpiNum_t spiNum);


  /**
   * @Prototype     static inline void SpiEnableChannel(SpiNum_t spiNum);
   *
   * @Description   This function enables the SPI channel specified.
   *
   * @Example       Spi.EnableChannel(SPI4);\n
   *                This example enables the SPI4.
   *
   * @Param         SpiNum_t numSpi\n
   *                SpiNum_t is an enum which can take the values SPI1 (not on the
   *                64 pins PIC32MX795F512H, SPI2, SPI3 and SPI4.
   *
   * @Returns       None.
   */
  void (*EnableChannel) (SpiNum_t spiNum);


  /**
   * @Prototype     static inline UINT32 SpiGetCharacter(SpiNum_t numSpi);
   *
   * @Description   This function waits for data to be available on the SPI
   *                channel specified and returns a character.
   *
   * @Example       Spi.GetCharacter(SPI4);\n
   *                This example reads a character on the SPI4.
   *
   * @Param         SpiNum_t numSpi\n
   *                SpiNum_t is an enum which can take the values SPI1 (not on the
   *                64 pins PIC32MX795F512H, SPI2, SPI3 and SPI4.
   *
   * @Returns       UINT32\n
   *                The character read. Depending on the mode of operation, it can
   *                be 8, 16 or 32 bits. Always represented on 32 bits.
   */
  UINT32 (*GetCharacter) (SpiNum_t spiNum);


  /**
   * @Prototype     static inline void SpiSendCharacter (SpiNum_t numSpi, const UINT32 data);
   *
   * @Description   This routine writes a single byte/half word/word to the SPI bus.
   *                It waits for TX buffer empty, so that it doesn't overwrite the
   *                previous untransmitted data.
   *
   * @Example       Spi.SendCharacter(SPI4, 'A');\n
   *                This example sends the character 'A' on the SPI4. In this case,
   *                the word length is 8 bits, but it could be 16 or 32 bits,
   *                depending on the settings put in Spi.Open(SPI_4, ...).
   *
   * @Param         SpiNum_t numSpi\n
   *                SpiNum_t is an enum which can take the values SPI1 (not on the
   *                64 pins PIC32MX795F512H, SPI2, SPI3 and SPI4.
   *
   * @Param         const UINT32 data\n
   *                The character to send.
   *
   * @Returns       None.
   */
  void (*SendCharacter) (SpiNum_t numSpi, const UINT32 data);


  /**
   * @Prototype     static inline UINT32 SpiBlockingTransfer (SpiNum_t numSpi, const UINT32 data);
   *
   * @Description   This function can be used as a blocking read of blocking 
   *                write to the SPI module. This routine writes a single 
   *                byte/half word/word to the SPI bus.
   *                It waits for TX buffer empty, so that it doesn't overwrite the
   *                previous untransmitted data. It then reads the SPI RX buffer 
   *                and returns the contents to the caller.
   *
   * @Example       Spi.BlockingTransfert(SPI4, 'A');\n
   *                This WRITE example sends the character 'A' on the SPI4. In this case,
   *                the word length is 8 bits, but it could be 16 or 32 bits,
   *                depending on the settings put in Spi.Open(SPI_4, ...). Note
   *                that we don't read the return value of the function as it is
   *                used as a write function.
   *
   * @Example       UINT32 value = Spi.BlockingTransfert(SPI4, 0x55);\n
   *                This READ example sends the value 0x55 on the SPI4. In this case,
   *                the word length is 8 bits, but it could be 16 or 32 bits,
   *                depending on the settings put in Spi.Open(SPI_4, ...).
   *
   * @Param         SpiNum_t numSpi\n
   *                SpiNum_t is an enum which can take the values SPI1 (not on the
   *                64 pins PIC32MX795F512H, SPI2, SPI3 and SPI4.
   *
   * @Param         const UINT32 data\n
   *                The character to send.
   *
   * @Returns       None.
   */
  UINT32 (*BlockingTransfer) (SpiNum_t numSpi, const UINT32 data);


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
  INT32 (*GetRxFifoBuffer) (SpiNum_t numSpi, sSpiLineBuffer_t * buffer , BOOL reply);


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
  INT32 (*PutTxFifoBuffer) (SpiNum_t numSpi, sSpiLineBuffer_t * buffer);


  /**
   * @Description   Contains certains variables useful for the SPI. Write a dot
   *                "." after writing "Var" to see a description of the variables
   *                available.
   */
  struct sChinookSpiVar Var;

};


//==============================================================================
// Public functions prototypes
//==============================================================================
//INT8 SpiFifoWrite (sSpiFifoBuffer_t *fifo, UINT32 *data);
//INT8 SpiFifoRead  (sSpiFifoBuffer_t *fifo, UINT32 *data);
inline INT8 SpiFifoWrite (sSpiFifoBuffer_t *fifo, void *data);
inline INT8 SpiFifoRead  (sSpiFifoBuffer_t *fifo, void *data);


#endif	/* __CHINOOK_SPI__ */
