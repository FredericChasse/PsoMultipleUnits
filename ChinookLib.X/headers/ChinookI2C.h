//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// ChinookLib
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : ChinookI2c.h
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : TBD.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : UNO 32 ONLY HAVE I2C1 and 2, CHINOOK BOARDS DONT HAVE I2C2
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#ifndef __CHINOOK_I2C__
#define	__CHINOOK_I2C__

#include "ChinookHardwareProfile.h"
#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>


//==============================================================================
// Macro definitions
//==============================================================================

//=========================================
// I2cModule_t
// Purpose : Tells the function which 
//           I2C module to interract with
//=========================================
typedef I2C_MODULE I2cModule_t;


//=========================================
// I2cFreq_t
// Purpose : Tells the function the rate at
//           which to set the I2C
//=========================================
typedef enum
{
   I2C_FREQ_100K   =   100
  ,I2C_FREQ_400K   =   400
} I2cFreq_t;


//=========================================
// I2cInterruptType_t
// Purpose : Used by the interrupt functions
//=========================================
typedef enum
{
   I2C_SLAVE_INTERRUPT
  ,I2C_MASTER_INTERRUPT
  ,I2C_BUS_COLLISION_INTERRUPT
} I2cInterruptType_t;


//=========================================
// I2cEepromInternalRegister_t
// Purpose : Useful for the user for 
//           manipulating the EEPROM
//=========================================
typedef union
{
  struct
  {
    UINT16     byteIndex  :   6   // Specific byte of a page. There are 64 bytes/page
              ,pageIndex  :   9   // Specific page of the memory. There are 512 pages
              ,           :   1   // Don't care
              ;
  } index;
  
  struct
  {
    UINT16     lowByte    :   8   // Low Byte of the address
              ,highByte   :   8   // High byte of the address
              ;
    UINT16     fullAdress         // Full address
              ;
  } address;
  
} I2cEepromInternalRegister_t;


//=========================================
// I2cPollFlag_t
// Purpose : Used by I2c.AddDataToFifoWriteQueue
//=========================================
typedef enum
{
   DO_POLL    = TRUE
  ,DONT_POLL  = FALSE
} I2cPollFlag_t;


//=========================================
// I2cSlaveAddressTypeFlag_t
// Purpose : Tells the type of slave. Not
//           implemented.
//=========================================
typedef enum
{
   SLAVE_ADDRESS_7_BITS
  ,SLAVE_ADDRESS_10_BITS
} I2cSlaveAddressTypeFlag_t;


//=========================================
// I2cMasterStates_t
// Purpose : States that can occur during
//           I2C interrupts.
//=========================================
typedef enum
{
   I2C_MASTER_START_CONDITION
  ,I2C_MASTER_REPEAT_START
  ,I2C_MASTER_STOP_CONDITION
  ,I2C_MASTER_RECEIVE_DATA
  ,I2C_MASTER_SEND_ACK
  ,I2C_MASTER_SEND_NACK
  ,I2C_MASTER_TRANSMIT_DATA
  ,I2C_MASTER_SLAVE_SENT_STOP
  ,I2C_CMD_ERROR
  ,I2C_MASTER_DONE
} I2cMasterStates_t;


//=========================================
// I2C_FIFO_LENGTH
// Purpose : Defines the length of the 
//           I2C FIFOs.
//=========================================
#define I2C_FIFO_LENGTH  256


//=========================================
// sI2cCmdBuffer_t
// Purpose : Used by the I2C interrupts
//=========================================
typedef struct sI2cCmdBuffer
{
  I2cMasterStates_t state;
  UINT8             data;
} sI2cCmdBuffer_t;


//=========================================
// sI2cFifoBuffer_t
// Purpose : structure used for the FIFO
//           functions.
//=========================================
typedef struct sI2cFifoBuffer
{
  UINT8             inIdx;
  UINT8             outIdx;
  BOOL              bufFull;
  BOOL              bufEmpty;
  UINT16            bufLength;
  const UINT16      maxBufSize;
  sI2cCmdBuffer_t   fifo[I2C_FIFO_LENGTH];
} sI2cFifoBuffer_t;
//=========================================


//==============================================================================
// Variable declarations
//==============================================================================
struct sChinookI2cVar
{
  /**
   * @Description   Address of the EEPROM. Analogically set at 0x1010 000r, where
   *                r is read of write (1 or 0).
   */
  I2C_7_BIT_ADDRESS eepromAddress;
  
  /**
   * @Description   DO NOT USE. This is the write queue used by the interrupt 
   *                functions. One for each I2C (i2cWriteQueue[I2C1] is for I2C1,
   *                i2cWriteQueue[I2C2] is for I2C2 and so on).
   */
  volatile sI2cFifoBuffer_t i2cWriteQueue[1];
  
  /**
   * @Description   DO NOT USE. This is the read queue used by the interrupt 
   *                functions. One for each I2C (i2cWriteQueue[I2C1] is for I2C1,
   *                i2cWriteQueue[I2C2] is for I2C2 and so on).
   */
  volatile sI2cFifoBuffer_t i2cReadQueue[1];
  
  /**
   * @Description   DO NOT USE. This is the user's RX FIFO buffer. It is used in
   *                interrupts and by the function I2c.ReadRxFifo(). One for each 
   *                I2C (i2cWriteQueue[I2C1] is for I2C1, i2cWriteQueue[I2C2] 
   *                is for I2C2 and so on).
   */
  volatile sI2cFifoBuffer_t i2cUserFifo[1];
  
  /**
   * @Description   DO NOT USE. Flag that tells the I2C interrupt to read its 
   *                receive buffer.
   */
  volatile BOOL oReadDataInNextInterrupt[1];

  /**
   * @Description   Flag that tells if there is Received data in the RX FIFO.
   *                One for each I2C (oRxDataAvailable[I2C1] is for I2C1, 
   *                oRxDataAvailable[I2C2] is for I2C2 and so on).
   */
  volatile BOOL oRxDataAvailable[1];

  /**
   * @Description   Flag that tells if the I2C module is writing. One 
   *                for each I2C (oI2cIsRunning[I2C1] is for I2C1, 
   *                oI2cIsRunning[I2C2] is for I2C2 and so on).
   */
  volatile BOOL oI2cWriteIsRunning[1];

  /**
   * @Description   Flag that tells if the I2C module is reading. One 
   *                for each I2C (oI2cIsRunning[I2C1] is for I2C1, 
   *                oI2cIsRunning[I2C2] is for I2C2 and so on).
   */
  volatile BOOL oI2cReadIsRunning[1];

  /**
   * @Description   DO NOT USE. Flag that tells the I2C interrupt to perform a 
   *                slave polling during a write procedure.
   */
  volatile BOOL oPoolSlaveAcknowledge[1];

  /**
   * @Description   DO NOT USE. Flag that tells the I2C interrupt the point at 
   *                which it is during a slave polling procedure.
   */
  volatile BOOL oSecondStopAfterPooling[1];
};


//==============================================================================
// Public structure declaration
//==============================================================================


struct sChinookI2c
{

  /**
   * @Prototype     static inline INT8 I2cOpen (I2cModule_t i2cModuleId, I2cFreq_t i2cFreq);
   *
   * @Description   This function opens the I2C module specified at the frequency
   *                specified.
   *
   * @Example       I2c.Open(I2C4, I2C_FREQ_400K);\n
   *                This example opens the i2C4 at a frequency of 400 kHz.
   *
   * @Parameter     I2cModule_t i2cModuleId\n
   *                Specifies which I2C to open.\n
   *                Possibles values : \n
   *                I2C1, I2C2, I2C3, I2C4, I2C5.\n
   *                If using Uno32, only I2Cs available are I2C1 and I2C2. If
   *                using the PIC32MX795F512H (the one on the Chinook boards),
   *                I2C2 is not available.
   *
   * @Parameter     I2cFreq_t i2cFreq\n
   *                Specifies the needed frequency for the I2C specified.\n
   *                Possible values : I2C_FREQ_100K and I2C_FREQ_400K. The PIC
   *                I2C peripheral only supports those frequencies.
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Conditions of failure : the frequency adjustment of the I2C
   *                module did not work.
   */
  INT8 (*Open)  (I2cModule_t i2cModuleId, I2cFreq_t i2cFreq);


  /**
   * @Prototype     static inline INT8 I2cConfigInterrupt (I2cModule_t i2cModuleId, UINT8 priority, UINT8 subpriority);
   *
   * @Description   This function configures the interrupts at the
   *                priority and subpriority needed. It does not enables the
   *                interrupts. To enable them, use I2c.EnableInterrupt();
   *
   * @Example       I2c.ConfigInterrupt(I2C4, 4, 2);\n
   *                This example configures the interrupts of the I2C4 at a
   *                priority of 4 and a subpriority of 2.
   *
   * @Parameter     I2cModule_t i2cModuleId\n
   *                Specifies which I2C to open.\n
   *                Possibles values : \n
   *                I2C1, I2C2, I2C3, I2C4, I2C5.\n
   *                If using Uno32, only I2Cs available are I2C1 and I2C2. If
   *                using the PIC32MX795F512H (the one on the Chinook boards),
   *                I2C2 is not available.
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
  INT8 (*ConfigInterrupt) (I2cModule_t i2cModuleId, UINT8 priority, UINT8 subpriority);


  /**
   * @Prototype     static inline INT8 I2cEnableInterrupt (I2cModule_t i2cModuleId, I2cInterruptType_t interruptType);
   *
   * @Description   This function enables the interrupt specified for the I2C specified.
   *
   * @Example       I2c.EnableInterrupt(I2C1, I2C_BUS_COLLISION_INTERRUPT);\n
   *                This example enables the bus collision interrupts for the I2C1.
   *
   * @Parameter     I2cModule_t i2cModuleId\n
   *                Specifies which I2C to open.\n
   *                Possibles values : \n
   *                I2C1, I2C2, I2C3, I2C4, I2C5.\n
   *                If using Uno32, only I2Cs available are I2C1 and I2C2. If
   *                using the PIC32MX795F512H (the one on the Chinook boards),
   *                I2C2 is not available.
   *
   * @Parameter     I2cInterruptType_t interruptType\n
   *                Typedef that contains the interrupt source you want to activate.\n
   *                Possible values : \n
   *                I2C_SLAVE_INTERRUPT         // Enable slave interrupts\n
   *                I2C_MASTER_INTERRUPT        // Enable master interrupts\n
   *                I2C_BUS_COLLISION_INTERRUPT // Enable bus collision interrupts
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Reasons of failure : wrong arguments.
   */
  INT8 (*EnableInterrupt) (I2cModule_t i2cModuleId, I2cInterruptType_t interruptType);


  /**
   * @Prototype     static inline INT8 I2cDisableInterrupt (I2cModule_t i2cModuleId, I2cInterruptType_t interruptType);
   *
   * @Description   This function disables the interrupt specified for the I2C specified.
   *
   * @Example       I2c.DisableInterrupt(I2C1, I2C_BUS_COLLISION_INTERRUPT);\n
   *                This example disables the bus collision interrupts for the I2C1.
   *
   * @Parameter     I2cModule_t i2cModuleId\n
   *                Specifies which I2C to open.\n
   *                Possibles values : \n
   *                I2C1, I2C2, I2C3, I2C4, I2C5.\n
   *                If using Uno32, only I2Cs available are I2C1 and I2C2. If
   *                using the PIC32MX795F512H (the one on the Chinook boards),
   *                I2C2 is not available.
   *
   * @Parameter     I2cInterruptType_t interruptType\n
   *                Typedef that contains the interrupt source you want to deactivate.\n
   *                Possible values : \n
   *                I2C_SLAVE_INTERRUPT         // Enable slave interrupts\n
   *                I2C_MASTER_INTERRUPT        // Enable master interrupts\n
   *                I2C_BUS_COLLISION_INTERRUPT // Enable bus collision interrupts
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Reasons of failure : wrong arguments.
   */
  INT8 (*DisableInterrupt) (I2cModule_t i2cModuleId, I2cInterruptType_t interruptType);


  /**
   * @Prototype     static inline void I2cClose (I2cModule_t i2cModuleId);
   *
   * @Description   This function closes the I2C specified.
   *
   * @Example       I2c.Close(I2C4);\n
   *                This example closes the I2C4.
   *
   * @Parameter     I2cModule_t i2cModuleId\n
   *                Specifies which I2C to open.\n
   *                Possibles values : \n
   *                I2C1, I2C2, I2C3, I2C4, I2C5.\n
   *                If using Uno32, only I2Cs available are I2C1 and I2C2. If
   *                using the PIC32MX795F512H (the one on the Chinook boards),
   *                I2C2 is not available.
   *
   * @Returns       None.
   */
  void (*Close) (I2cModule_t i2cModuleId);


  /**
   * @Prototype     static inline INT8 I2cEepromSendByte (I2cModule_t i2cModuleId, UINT16 eepromInternalReg, UINT8 data)
   *
   * @Description   This is a blocking function that sends a single byte to the
   *                I2C module specified at the EEPROM internal register
   *                specified. It blocks when waiting for the slave to be able to
   *                receive further instructions after its write process. The
   *                following list describes the steps used to achieve this.\n
   *                1 - Format the address as WRITE (0b10100000);\n
   *                2 - Send a START condition. On failure, the
   *                function returns -1;\n
   *                3 - Send the slave address with a WRITE condition (0b10100000)
   *                and wait for ACK from the slave. If no ACK was received, return
   *                -1;\n
   *                4 - Send the HIGH byte of the 16 bits internal register. Wait
   *                for ACK. If no ACK was received, return -1;\n
   *                5 - Send the LOW byte of the 16 bits internal register. Wait 
   *                for ACK. If no ACK was received, return -1;\n
   *                6 - Send the data specified in the function parameters. Wait
   *                for ACK. If no ACK was received, return -1;\n
   *                7 - Send a STOP condition;\n
   *                The following steps are done in order to wait for the slave to
   *                be ready to receive further instructions.\n
   *                8 - Send a START condition. On failure, the
   *                function returns -1;\n
   *                9 - Send a dummy write with the address of the slave.\n
   *                10 - Check is ACK was received;\n
   *                11 - Send a STOP condition.
   *                If the master received a NACK instead of an ACK, redo steps
   *                8 to 11. This is the blocking part of the function as it waits
   *                for the slave to finish writing to its internal register.
   *
   * @Example       I2c.EepromSendByte(I2C4, 0x0540, 0xAA);\n
   *                This example writes 0xAA at the internal address 0x0540 of the
   *                EEPROM.
   *
   * @Parameter     I2cModule_t i2cModuleId\n
   *                Specifies which I2C to open.\n
   *                Possibles values : \n
   *                I2C1, I2C2, I2C3, I2C4, I2C5.\n
   *                If using Uno32, only I2Cs available are I2C1 and I2C2. If
   *                using the PIC32MX795F512H (the one on the Chinook boards),
   *                I2C2 is not available.
   *
   * @Parameter     UINT16 eepromInternalReg\n
   *                Internal register of the EEPROM to write to.
   *
   * @Parameter     UINT8 data\n
   *                Data to write.
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Conditions of failure : one of the step did not return an
   *                ACKNOWLEDGE.
   */
  INT8 (*EepromSendByte) (I2cModule_t i2cModuleId, UINT16 eepromInternalAdress, UINT8 data);


  /**
   * @Prototype     static inline INT8 I2cEepromReadByte (I2cModule_t i2cModuleId, UINT16 eepromInternalReg, UINT8 *pData)
   *
   * @Description   This function reads a single byte from the I2C module
   *                specified at the EEPROM internal register specified. The
   *                following list describes the steps used to achieve this.\n
   *                1 - Format the address as WRITE (0b10100000);\n
   *                2 - Send a START condition. On failure, the
   *                function returns -1;\n
   *                3 - Send the slave address with a WRITE condition (0b10100000)
   *                and wait for ACK from the slave. This is refered to as a
   *                "dummy write" and is done in order to tell the slave the
   *                register to read from. If no ACK was received, return -1;\n
   *                4 - Send the HIGH byte of the 16 bits internal register. Wait
   *                for ACK. If no ACK was received, return -1;\n
   *                5 - Send the LOW byte of the 16 bits internal register. Wait
   *                for ACK. If no ACK was received, return -1;\n
   *                6 - Send a RESTART condition. On failure, the
   *                function returns -1;\n
   *                7 - Format the address as READ (0b10100001) and send this
   *                address to the slave. If no ACK was received, return -1;\n
   *                8 - Enable the master to receive data on its bus; On failure,
   *                the function returns -1;\n
   *                9 - Wait for data to be available to read;\n
   *                10 - Place read data at the address set by pData;\n
   *                11 - Send a STOP condition.
   *
   * @Example       UINT8 byteToRead = 0;\n
   *                INT8 err = I2c.EepromReadByte(I2C4, 0x0540, *byteToRead);\n
   *                if (err < 0)\n
   *                {\n
   *                  LED_ERROR_ON;\n 
   *                }\n
   *                else\n 
   *                {\n
   *                  if (byteToRead == byteSent)\n
   *                  {\n
   *                    LED_DEBUG0_ON;\n
   *                  }\n
   *                }\n
   *                This example reads the internal address 0x0540 of the
   *                EEPROM and stores its content in byteToRead. It then tests 
   *                the result of the operation.
   *
   * @Parameter     I2cModule_t i2cModuleId\n
   *                Specifies which I2C to open.\n
   *                Possibles values : \n
   *                I2C1, I2C2, I2C3, I2C4, I2C5.\n
   *                If using Uno32, only I2Cs available are I2C1 and I2C2. If
   *                using the PIC32MX795F512H (the one on the Chinook boards),
   *                I2C2 is not available.
   *
   * @Parameter     UINT16 eepromInternalReg\n
   *                Internal register of the EEPROM to read from.
   *
   * @Parameter     UINT8 *pData\n
   *                Pointer to location to write the data to. The data read will
   *                be written at this address.
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Conditions of failure : one of the step did not return an
   *                ACKNOWLEDGE or an error occured during the process.
   */
  INT8 (*EepromReadByte) (I2cModule_t i2cModuleId, UINT16 eepromInternalAdress, UINT8 *pData);


  /**
   * @Note          This function was not tested due to not having the need nor 
   *                the material for 10 bits address. Its functionality is very
   *                similar to its 7 bits cousin, but bugs may be present. 
   *                Please report any bugs that you may find.
   *
   * @Prototype     static inline INT8 I2cSlave10BitsAddressSendData (I2cModule_t i2cModuleId, UINT16 slaveAddress, UINT8 *pData, UINT8 nBytesToSend);
   *
   * @Description   This is a blocking function that sends nBytesToSend bytes to
   *                the I2C module specified. It blocks when waiting for the
   *                slave to be able to receive further instructions after its
   *                write process. The following list describes the steps used
   *                to achieve this. The buffer pData must contain the internal
   *                register to write to.\n
   *                1 - Format the address as WRITE;\n
   *                2 - Send a START condition. On failure, the
   *                function returns -1;\n
   *                3 - Send the slave address with a WRITE condition
   *                and wait for ACK from the slave. If no ACK was received, return
   *                -1;\n
   *                4 - Send pData[i] Wait for ACK. If no ACK was received,
   *                return -1;\n
   *                5 - Repeat step 4 for nBytesToSend iterations;
   *                6 - Send a STOP condition;\n
   *                The following steps are done in order to wait for the slave
   *                to be ready to receive further instructions.\n
   *                7 - Send a START condition. On failure, the
   *                function returns -1;\n
   *                8 - Send a dummy write with the address of the slave.\n
   *                9 - Check is ACK was received;\n
   *                10 - Send a STOP condition.
   *                If the master received a NACK instead of an ACK, redo steps
   *                7 to 10. This is the blocking part of the function as it waits
   *                for the slave to finish writing to its internal register.
   *
   * @Example       UINT8  nBytesToWrite = 5\n
   *                      ,i2cData[5] = { 0xC0, 0x50, 0xDE, 0xAD, 0xBB }\n
   *                      ;\n
   *                UINT16 slaveAddress = 0b1010000101  // EEPROM address\n
   *                if ( I2c.Slave7BitsAddressSendData(I2C4, slaveAddress, i2cData, nBytesToWrite) < 0 )\n
   *                {\n
   *                  Port.B.ClearBits(BIT_13); // Error LED\n
   *                  while(1);\n
   *                }\n\n
   *                This example writes 5 bytes to the slave at the address
   *                0b1010000101. The first two bytes could be, depending on the
   *                slave, the HIGH byte and the LOW byte of the internal
   *                register to start writing to. The slave writes the next data
   *                to the following register.
   *
   * @Parameter     I2cModule_t i2cModuleId\n
   *                Specifies which I2C to open.\n
   *                Possibles values : \n
   *                I2C1, I2C2, I2C3, I2C4, I2C5.\n
   *                If using Uno32, only I2Cs available are I2C1 and I2C2. If
   *                using the PIC32MX795F512H (the one on the Chinook boards),
   *                I2C2 is not available.
   *
   * @Parameter     UINT16 slaveAddress\n
   *                Address of the slave. Must be 10 bits. DO NOT INCLUDE THE
   *                READ/WRITE BIT NOR THE 5 MSB. THE MSB ARE CONSTANT AND ARE
   *                SET BY THE FUNCTION.
   *
   * @Parameter     UINT8 *pData\n
   *                Pointer to the buffer that contains the bytes to send. It
   *                must include the internal register to write to. For example,
   *                if you want to write to the register 0xFADE of the slave, then
   *                pData[0] = 0xFA and pData[1] = 0xDE.
   *
   * @Parameter     UINT8 nBytesToWrite\n
   *                How many elements to write.
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Conditions of failure : one of the step did not return an
   *                ACKNOWLEDGE or an error occured during the process.
   */
  INT8 (*Slave10BitsAddressSendData) (I2cModule_t i2cModuleId, UINT16 slaveAddress, UINT8 *pData, UINT8 nBytesToSend);


  /**
   * @Prototype     static inline INT8 I2cSlave7BitsAddressSendData (I2cModule_t i2cModuleId, UINT8 slaveAddress, UINT8 *pData, UINT8 nBytesToSend);
   *
   * @Description   This is a blocking function that sends nBytesToSend bytes to
   *                the I2C module specified. It blocks when waiting for the
   *                slave to be able to receive further instructions after its
   *                write process. The following list describes the steps used
   *                to achieve this. The buffer pData must contain the internal
   *                register to write to.\n
   *                1 - Format the address as WRITE;\n
   *                2 - Send a START condition. On failure, the
   *                function returns -1;\n
   *                3 - Send the slave address with a WRITE condition
   *                and wait for ACK from the slave. If no ACK was received, return
   *                -1;\n
   *                4 - Send pData[i] Wait for ACK. If no ACK was received,
   *                return -1;\n
   *                5 - Repeat step 4 for nBytesToSend iterations;
   *                6 - Send a STOP condition;\n
   *                The following steps are done in order to wait for the slave
   *                to be ready to receive further instructions.\n
   *                7 - Send a START condition. On failure, the
   *                function returns -1;\n
   *                8 - Send a dummy write with the address of the slave.\n
   *                9 - Check is ACK was received;\n
   *                10 - Send a STOP condition.
   *                If the master received a NACK instead of an ACK, redo steps
   *                7 to 10. This is the blocking part of the function as it waits
   *                for the slave to finish writing to its internal register.
   *
   * @Example       UINT8  nBytesToWrite = 5\n
   *                      ,i2cData[5] = { 0xC0, 0x50, 0xDE, 0xAD, 0xBB }\n
   *                      ,slaveAddress = 0b1010000  // EEPROM address\n
   *                      ;\n
   *                if ( I2c.Slave7BitsAddressSendData(I2C4, slaveAddress, i2cData, nBytesToWrite) < 0 )\n
   *                {\n
   *                  Port.B.ClearBits(BIT_13); // Error LED\n
   *                  while(1);\n
   *                }\n\n
   *                This example writes 5 bytes to the slave. The first two bytes
   *                could be, depending on the slave, the HIGH byte and the LOW
   *                byte of the internal register to start writing to. The slave
   *                writes the next data to the following register.
   *
   * @Parameter     I2cModule_t i2cModuleId\n
   *                Specifies which I2C to open.\n
   *                Possibles values : \n
   *                I2C1, I2C2, I2C3, I2C4, I2C5.\n
   *                If using Uno32, only I2Cs available are I2C1 and I2C2. If
   *                using the PIC32MX795F512H (the one on the Chinook boards),
   *                I2C2 is not available.
   *
   * @Parameter     UINT8 slaveAddress\n
   *                Address of the slave. DO NOT INCLUDE THE READ/WRITE BIT. This
   *                must be 7 bits.
   *
   * @Parameter     UINT8 *pData\n
   *                Pointer to the buffer that contains the bytes to send. It
   *                must include the internal register to write to. For example,
   *                if you want to write to the register 0xFADE of the slave, then
   *                pData[0] = 0xFA and pData[1] = 0xDE.
   *
   * @Parameter     UINT8 nBytesToWrite\n
   *                How many elements to write.
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Conditions of failure : one of the step did not return an
   *                ACKNOWLEDGE or an error occured during the process.
   */
  INT8 (*Slave7BitsAddressSendData) (I2cModule_t i2cModuleId, UINT8 slaveAddress, UINT8 *pData, UINT8 nBytesToSend);


  /**
   * @Note          This function was not tested due to not having the need nor
   *                the material for 10 bits address. Its functionality is very
   *                similar to its 7 bits cousin, but bugs may be present.
   *                Please report any bugs that you may find.
   *
   * @Prototype     static inline INT8 I2cSlave10BitsAddressReadData (I2cModule_t i2cModuleId, UINT16 slaveAddress, UINT8 *slaveReg, UINT8  slaveRegSize, UINT8 *pData, UINT8 nBytesToRead);
   *
   * @Description   This function reads nBytesToRead bytes from the I2C module
   *                specified at the EEPROM internal register specified. The
   *                following list describes the steps used to achieve this.\n
   *                1 - Format the address as a 10 bits address with WRITE;\n
   *                2 - Send a START condition. On failure, the
   *                function returns -1;\n
   *                3 - Send the slave address (HIGH byte first, then LOW byte)
   *                with a WRITE condition and wait for ACK from the slave.
   *                This is refered to as a "dummy write" and is done in order
   *                to tell the slave the register to read from. If no ACK was
   *                received, return -1;\n
   *                4 - Send the HIGH byte of the register specified by slaveReg[0].
   *                Wait for ACK. If no ACK was received, return -1. Repeat for
   *                subsequent elements of slaveReg for slaveRegSize iterations;\n
   *                5 - Send a RESTART condition. On failure, the
   *                function returns -1;\n
   *                6 - Format the address as a 10 bits address with READ and send this
   *                address to the slave. If no ACK was received, return -1;\n
   *                7 - Enable the master to receive data on its bus; On failure,
   *                the function returns -1;\n
   *                8 - Wait for data to be available to read;\n
   *                9 - If there are more data to read, send an ACK. Else, send
   *                a NACK to the slave. Wait for the ACK/NACK to complete;\n
   *                10 - Read data and place it at the address set by pData[i].
   *                If there is an ACK from the master, the slave will send
   *                for the next transfer the data contained at its following
   *                register;\n
   *                11 - Repeat steps 7 to 10 for nBytesToRead iterations;\n
   *                11 - Send a STOP condition.
   *
   * @Example       UINT8  nBytesToRead = 5\n
   *                      ,i2cData[nBytesToRead]\n
   *                      ,slaveRegSize = 2\n
   *                      ,slaveReg[2] = { 0x50, 0xC0 } // Slave internal register\n
   *                      ;\n
   *                UINT16 slaveAddress = 0b1011010000;  // EEPROM address\n
   *                if ( I2c.Slave10BitsAddressReadData(I2C4, slaveAddress, slaveReg, slaveRegSize, i2cbyte, nBytesToRead) < 0 )\n
   *                {\n
   *                  Port.B.ClearBits(BIT_13); // Error LED\n
   *                  while(1);\n
   *                }\n\n
   *                This example reads 5 bytes from the slave with the address
   *                0b1011010000 starting at the register 0x50C0 and puts them
   *                in i2cByte.
   *
   * @Parameter     I2cModule_t i2cModuleId\n
   *                Specifies which I2C to open.\n
   *                Possibles values : \n
   *                I2C1, I2C2, I2C3, I2C4, I2C5.\n
   *                If using Uno32, only I2Cs available are I2C1 and I2C2. If
   *                using the PIC32MX795F512H (the one on the Chinook boards),
   *                I2C2 is not available.
   *
   * @Parameter     UINT16 slaveAddress\n
   *                Address of the slave. Must be 10 bits. DO NOT INCLUDE THE
   *                READ/WRITE BIT NOR THE 5 MSB. THE MSB ARE CONSTANT AND ARE
   *                SET BY THE FUNCTION.
   *
   * @Parameter     UINT8 *slaveReg\n
   *                Table used for the internal register of the EEPROM to read from.
   *                Must be formated as HIGHEST to LOWEST byte. Example : \n
   *                Address = 0xC050FE\n
   *                UINT8 buffer = {0xC0, 0x50, 0xFE};
   *
   * @Parameter     UINT8  slaveRegSize\n
   *                Tells the function how many bytes the internal register of the
   *                slave has (i.e.: how many elements in slaveReg).
   *
   * @Parameter     UINT8 *pData\n
   *                Pointer to location to write the data to. The data read will
   *                be written at this address. If there is more than one byte
   *                to read, pData must point to a buffer.
   *
   * @Parameter     UINT32 nBytesToRead\n
   *                How many elements to read.
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Conditions of failure : one of the step did not return an
   *                ACKNOWLEDGE or an error occured during the process.
   */
  INT8 (*Slave10BitsAddressReadData) (I2cModule_t i2cModuleId, UINT16 slaveAddress, UINT8 *slaveReg, UINT8  slaveRegSize, UINT8 *pData, UINT8 nBytesToRead);


  /**
   * @Prototype     static inline INT8 I2cSlave7BitsAddressReadData (I2cModule_t i2cModuleId, UINT8 slaveAddress, UINT8 *slaveReg, UINT8  slaveRegSize, UINT8 *pData, UINT8 nBytesToRead);
   *
   * @Description   This function reads nBytesToRead bytes from the I2C module
   *                specified at the EEPROM internal register specified. The
   *                following list describes the steps used to achieve this.\n
   *                1 - Format the address as WRITE;\n
   *                2 - Send a START condition. On failure, the
   *                function returns -1;\n
   *                3 - Send the slave address with a WRITE condition
   *                and wait for ACK from the slave. This is refered to as a
   *                "dummy write" and is done in order to tell the slave the
   *                register to read from. If no ACK was received, return -1;\n
   *                4 - Send the HIGH byte of the register specified by slaveReg[0].
   *                Wait for ACK. If no ACK was received, return -1. Repeat for
   *                subsequent elements of slaveReg for slaveRegSize iterations;\n
   *                5 - Send a RESTART condition. On failure, the
   *                function returns -1;\n
   *                6 - Format the address as READ and send this
   *                address to the slave. If no ACK was received, return -1;\n
   *                7 - Enable the master to receive data on its bus; On failure,
   *                the function returns -1;\n
   *                8 - Wait for data to be available to read;\n
   *                9 - If there are more data to read, send an ACK. Else, send
   *                a NACK to the slave. Wait for the ACK/NACK to complete;\n
   *                10 - Read data and place it at the address set by pData[i].
   *                If there is an ACK from the master, the slave will send
   *                for the next transfer the data contained at its following 
   *                register;\n
   *                11 - Repeat steps 7 to 10 for nBytesToRead iterations;\n
   *                11 - Send a STOP condition.
   *
   * @Example       UINT8  nBytesToRead = 5\n
   *                      ,i2cData[nBytesToRead]\n
   *                      ,slaveRegSize = 2\n
   *                      ,slaveReg[2] = { 0x50, 0xC0 } // Slave internal register\n
   *                      ,slaveAddress = 0b1010000  // EEPROM address\n
   *                      ;\n
   *                if ( I2c.Slave7BitsAddressReadData(I2C4, slaveAddress, slaveReg, slaveRegSize, i2cbyte, nBytesToRead) < 0 )\n
   *                {\n
   *                  Port.B.ClearBits(BIT_13); // Error LED\n
   *                  while(1);\n
   *                }\n\n
   *                This example reads 5 bytes from the slave with the address
   *                0b1010000 starting at the register 0x50C0 and puts them in
   *                i2cByte.
   *
   * @Parameter     I2cModule_t i2cModuleId\n
   *                Specifies which I2C to open.\n
   *                Possibles values : \n
   *                I2C1, I2C2, I2C3, I2C4, I2C5.\n
   *                If using Uno32, only I2Cs available are I2C1 and I2C2. If
   *                using the PIC32MX795F512H (the one on the Chinook boards),
   *                I2C2 is not available.
   *
   * @Parameter     UINT8 slaveAddress\n
   *                Address of the slave. DO NOT INCLUDE THE READ/WRITE BIT. This
   *                must be 7 bits.
   *
   * @Parameter     UINT8 *slaveReg\n
   *                Table used for the internal register of the EEPROM to read from.
   *                Must be formated as HIGHEST to LOWEST byte. Example : \n
   *                Address = 0xC050FE\n
   *                UINT8 buffer = {0xC0, 0x50, 0xFE};
   *
   * @Parameter     UINT8  slaveRegSize\n
   *                Tells the function how many bytes the internal register of the
   *                slave has (i.e.: how many elements in slaveReg).
   *
   * @Parameter     UINT8 *pData\n
   *                Pointer to location to write the data to. The data read will
   *                be written at this address. If there is more than one byte
   *                to read, pData must point to a buffer.
   *
   * @Parameter     UINT32 nBytesToRead\n
   *                How many elements to read.
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Conditions of failure : one of the step did not return an
   *                ACKNOWLEDGE or an error occured during the process.
   */
  INT8 (*Slave7BitsAddressReadData) (I2cModule_t i2cModuleId, UINT8 slaveAddress, UINT8 *slaveReg, UINT8  slaveRegSize, UINT8 *pData, UINT8 nBytesToRead);


  /**
   * @Prototype     static inline INT32 I2cAddDataToFifoReadQueue (I2cModule_t i2cModuleId, UINT8 * slaveAddPlusRegBuf, size_t sizeOfSlaveBuf, size_t numToRead);
   *
   * @Description   This function is useful when using I2C interrupts. It should
   *                be used when you intend to read from your I2C Slave. In 
   *                addition, the function I2c.I2cAddDataToFifoWriteQueue() and 
   *                I2c.ReadRxFifo().
   *
   * @Example       UINT8 slaveAddPlusRegBuf[3];\n
   *                slaveAddPlusRegBuf[0] = I2c.Var.eepromAddress.byte;\n
   *                slaveAddPlusRegBuf[1] = 0x05;\n
   *                slaveAddPlusRegBuf[2] = 0x50;\n\n
   *                // ... Write sequence ... //\n\n
   *                while(I2c.Var.oI2cWriteIsRunning[I2C1]);  // Wait for any I2C1 write sequence to end\n
   *                I2c.AddDataToFifoReadQueue(I2C1, &slaveAddPlusRegBuf[0], sizeof(slaveAddPlusRegBuf), 5);\n\n
   *                while(I2c.Var.oI2cReadIsRunning[I2C1]);  // Wait for the read sequence to end\n
   *                UINT8 readBuffer[10];\n
   *                I2c.ReadRxFifo(I2C1, &readBuffer[0], 5);\n\n                
   *                
   *                This example adds a read sequence to the I2C1 interrupt. The
   *                I2C1 will read 5 bytes from the EEPROM (in this example, the
   *                slave is the EEPROM) starting at the register 0x0540. The
   *                example then waits for the read sequence to end and reads the
   *                data from the FIFO.
   *
   * @Parameter     I2cModule_t i2cModuleId\n
   *                Specifies which I2C to open.\n
   *                Possibles values : \n
   *                I2C1, I2C2, I2C3, I2C4, I2C5.\n
   *                If using Uno32, only I2Cs available are I2C1 and I2C2. If
   *                using the PIC32MX795F512H (the one on the Chinook boards),
   *                I2C2 is not available.
   *
   * @Parameter     UINT8 * slaveAddPlusRegBuf\n
   *                Contains the slave address PLUS the first register in which
   *                you want to read from. For example, if you want to read the
   *                EEPROM register 0x0540, you buffer should look like : \n
   *                UINT8 slaveAddPlusRegBuf[3];\n
   *                slaveAddPlusRegBuf[0] = I2c.Var.eepromAddress.byte;\n
   *                slaveAddPlusRegBuf[1] = 0x05;\n
   *                slaveAddPlusRegBuf[2] = 0x50;\n
   *                If the register in your slave is different than 2 bytes (like
   *                the EEPROM), then you should adapt your buffer accordingly. 
   *                Normally, the HIGH bytes are transfered first (but you should
   *                still refer to your slave's datasheet).\n\n
   *                This section explains the way that the EEPROM memory is 
   *                structured. \n
   *                The EEPROM has 512 pages of 64 bytes each. You 
   *                can only read or write from a page at a time. The register 
   *                address is formated as follows : \n
   *                1 don't care, 9 bits for the page index, 6 bits for the byte
   *                index.\n
   *                If you want to address a certain byte of a page of the EEPROM 
   *                of a single byte from a page, you can use the type 
   *                I2cEepromInternalRegister_t. It's a union that lets you 
   *                manipulate either the page index, the byte index, the HIGH 
   *                or LOW bytes of the register or the full 16 bits register 
   *                address. Here is an example : \n
   *                I2cEepromInternalRegister_t slaveRegister;\n
   *                slaveRegister.pageIndex = 0b000011100;\n
   *                slaveRegister.byteIndex = 0b110011;\n
   *                UINT8 slaveAddPlusRegBuf[3];\n
   *                slaveAddPlusRegBuf[0] = I2c.Var.eepromAddress.byte;\n
   *                slaveAddPlusRegBuf[1] = slaveRegister.highByte;\n
   *                slaveAddPlusRegBuf[2] = slaveRegister.lowByte;
   *                
   *
   * @Parameter     size_t sizeOfSlaveBuf\n
   *                Size in bytes of the slaveAddPlusRegBuf. It's the size in bytes
   *                of the slave address + the register length.
   *
   * @Parameter     size_t numToRead\n
   *                number of bytes to read.
   *
   * @Returns       Number of bytes read on success, -1 on failure.\n
   *                Reasons of failure : \n
   *                - numToRead == 0;\n
   *                - A write process in currently running (I2c.Var.oI2cWriteIsRunning[i2cModuleId] == 1);\n
   *                - ReadQueue buffer is full;\n
   *                - Space available in ReadQueue buffer < (2*numToRead + 8) (overhead);\n
   *                - Could not write properly to the ReadQueue buffer.
   */
  INT32 (*AddDataToFifoReadQueue) (I2cModule_t i2cModuleId, UINT8 * slaveAddPlusRegBuf, size_t sizeOfSlaveBuf, size_t numToRead);


  /**
   * @Prototype     static inline INT32 I2cAddDataToFifoWriteQueue (I2cModule_t i2cModuleId, UINT8 * buffer, size_t numToWrite, BOOL oPoolSlaveAcknowledge);
   *
   * @Description   This function is useful when using I2C interrupts. It should
   *                be used when you intend to write to your I2C Slave. In 
   *                addition, the function I2c.I2cAddDataToFifoReadQueue() and 
   *                I2c.ReadRxFifo(). The function can, according to the flag
   *                oPoolSlaveAcknowledge, perform a slave polling procedure after
   *                the writing process. A slave polling procedure ensures that 
   *                the slave is ready to accept more data or send data. If you
   *                judge that the time between you I2C reads and writes is sufficient
   *                for the slave to be ready, you can skip the slave polling.
   *
   * @Example       UINT8 dataBuffer[10];\n
   *                dataBuffer[0] = I2c.Var.eepromAddress.byte;\n
   *                dataBuffer[1] = 0x05;\n
   *                dataBuffer[2] = 0x50;\n
   *                dataBuffer[3] = 0xFF;\n
   *                // ... data to write ... //\n
   *                dataBuffer[9] = 0xCC;\n
   *                while(I2c.Var.oI2cReadIsRunning[I2C1]);  // Wait for any I2C1 read sequence to end\n
   *                I2c.AddDataToFifoWriteQueue(I2C1, &dataBuffer[0], sizeof(dataBuffer), TRUE);\n\n
   *                while(I2c.Var.oI2cWriteIsRunning[I2C1]);  // Wait for any I2C1 write sequence to end\n
   *                I2c.AddDataToFifoReadQueue(I2C1, &dataBuffer[0], 3, 7);\n\n
   *                while(I2c.Var.oI2cReadIsRunning[I2C1]);  // Wait for the read sequence to end\n
   *                UINT8 readBuffer[10];\n
   *                I2c.ReadRxFifo(I2C1, &readBuffer[0], 7);\n\n                
   *                
   *                This example adds a write sequence to the I2C1 interrupt. The
   *                I2C1 will write a byte for the slave address (in this case, 
   *                the EEPROM), the two bytes of the register to write to and 
   *                seven data bytes. The I2C1 will perform a slave polling procedure.
   *                Then, the example reads the same data from the slave.
   *
   * @Parameter     I2cModule_t i2cModuleId\n
   *                Specifies which I2C to open.\n
   *                Possibles values : \n
   *                I2C1, I2C2, I2C3, I2C4, I2C5.\n
   *                If using Uno32, only I2Cs available are I2C1 and I2C2. If
   *                using the PIC32MX795F512H (the one on the Chinook boards),
   *                I2C2 is not available.
   *
   * @Parameter     UINT8 * buffer\n
   *                Contains the slave address, the first register in which
   *                you want to write to and the data you want to write. For 
   *                example, if you want to write to the
   *                EEPROM register 0x0540, you buffer should look like : \n
   *                UINT8 dataBuffer[10];\n
   *                dataBuffer[0] = I2c.Var.eepromAddress.byte;\n
   *                dataBuffer[1] = 0x05;\n
   *                dataBuffer[2] = 0x50;\n
   *                // ...Rest of data... //\n\n
   *                If the register in your slave is different than 2 bytes (like
   *                the EEPROM), then you should adapt your buffer accordingly. 
   *                Normally, the HIGH bytes are transfered first (but you should
   *                still refer to your slave's datasheet).\n\n
   *                This section explains the way that the EEPROM memory is 
   *                structured. \n
   *                The EEPROM has 512 pages of 64 bytes each. You 
   *                can only read or write from a page at a time. The register 
   *                address is formated as follows : \n
   *                1 don't care, 9 bits for the page index, 6 bits for the byte
   *                index.\n
   *                If you want to address a certain byte of a page of the EEPROM 
   *                of a single byte from a page, you can use the type 
   *                I2cEepromInternalRegister_t. It's a union that lets you 
   *                manipulate either the page index, the byte index, the HIGH 
   *                or LOW bytes of the register or the full 16 bits register 
   *                address. Here is an example : \n
   *                I2cEepromInternalRegister_t slaveRegister;\n
   *                slaveRegister.pageIndex = 0b000011100;\n
   *                slaveRegister.byteIndex = 0b110011;\n
   *                UINT8 dataBuffer[10];\n
   *                dataBuffer[0] = I2c.Var.eepromAddress.byte;\n
   *                dataBuffer[1] = slaveRegister.highByte;\n
   *                dataBuffer[2] = slaveRegister.lowByte;\n
   *                // ...Rest of data... //
   *                
   *
   * @Parameter     size_t numToWrite\n
   *                Size in bytes of the buffer to write. It's the size in bytes
   *                of the slave address + the register length + the number of 
   *                data to write.
   *
   * @Parameter     BOOL oPoolSlaveAcknowledge\n
   *                Perform or not a slave acknowledge procedure.\n
   *                TRUE : perform the procedure\n
   *                FALSE : don't perform the procedure.
   *
   * @Returns       Number of bytes written on success, -1 on failure.\n
   *                Reasons of failure : \n
   *                - numToWrite == 0;\n
   *                - A read process in currently running (I2c.Var.oI2cReadIsRunning[i2cModuleId] == 1);\n
   *                - WriteQueue buffer is full;\n
   *                - Space available in ReadQueue buffer < (numToRead + 3) (overhead);\n
   *                - Could not write properly to the ReadQueue buffer.
   */
  INT32 (*AddDataToFifoWriteQueue) (I2cModule_t i2cModuleId, UINT8 * buffer, size_t numToWrite, BOOL oPoolSlaveAcknowledge);


  /**
   * @Prototype     static inline INT32 I2cReadRxFifo (I2cModule_t i2cModuleId, UINT8 *buffer, size_t numToRead);
   *
   * @Description   This function is useful when using I2C interrupts. It should
   *                be used when you have just performed a reading process with 
   *                the I2C. In addition, the function I2c.I2cAddDataToFifoReadQueue() 
   *                and I2c.I2cAddDataToFifoWriteQueue().
   *
   * @Example       // ...Reading and writing procedure... //\n
   *                while(I2c.Var.oI2cWriteIsRunning[I2C1]);  // Wait for any I2C1 write sequence to end\n
   *                while(I2c.Var.oI2cReadIsRunning[I2C1]);   // Wait for any I2C1 read sequence to end\n
   *                while(!I2c.Var.oRxDataAvailable[I2C1]);   // Wait for RX data to be available\n
   *                UINT8 buffer[30];
   *                I2c.ReadRxFifo(I2C1, &buffer[0], 10);\n\n             
   *                
   *                This example reads 10 bytes from the I2C slave. It is performed
   *                After the function I2c.AddDataToFifoReadQueue();
   *
   * @Parameter     I2cModule_t i2cModuleId\n
   *                Specifies which I2C to open.\n
   *                Possibles values : \n
   *                I2C1, I2C2, I2C3, I2C4, I2C5.\n
   *                If using Uno32, only I2Cs available are I2C1 and I2C2. If
   *                using the PIC32MX795F512H (the one on the Chinook boards),
   *                I2C2 is not available.
   *
   * @Parameter     UINT8 * buffer\n
   *                The buffer in which the data from the I2C slave will be put.
   *
   * @Parameter     size_t numToRead\n
   *                Size in bytes you want to read.
   *
   * @Returns       Number of bytes written in *buffer on success, -1 on failure.\n
   *                Reasons of failure : \n
   *                - numToRead == 0;\n
   *                - There is not data to be read\n
   *                - numToRead is greater than the data available
   */
  INT32 (*ReadRxFifo) (I2cModule_t i2cModuleId, UINT8 *buffer, size_t numToRead);


  /**
   * @Description   Contains certains variables useful for the I2C. Write a dot
   *                "." after writing "Var" to see a description of the variables
   *                available.
   */
  struct sChinookI2cVar Var;

};


//==============================================================================
// Public functions prototypes
//==============================================================================

//=========================================
// I2cFifoWrite
// Purpose : Write data to an I2C FIFO
//=========================================
inline INT8 I2cFifoWrite (sI2cFifoBuffer_t *dest, sI2cCmdBuffer_t *source);

//=========================================
// I2cFifoRead
// Purpose : Read data from an I2C FIFO
//=========================================
inline INT8 I2cFifoRead (sI2cFifoBuffer_t *source, sI2cCmdBuffer_t *dest);



#endif	/* __CHINOOK_I2C__ */

