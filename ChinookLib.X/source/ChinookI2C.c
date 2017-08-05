//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// ChinookLib
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : ChinookI2C.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : TBD.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : TBD.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "..\headers\ChinookI2c.h"


//==============================================================================
// Private function prototypes
//==============================================================================
static inline INT8    I2cOpen                       (I2cModule_t i2cModuleId, I2cFreq_t i2cFreq);
static inline void    I2cClose                      (I2cModule_t i2cModuleId);
static inline INT32   I2cAddDataToFifoReadQueue     (I2cModule_t i2cModuleId, UINT8 * slaveAddPlusRegBuf, size_t sizeOfSlaveBuf, size_t numToRead);
static inline INT32   I2cAddDataToFifoWriteQueue    (I2cModule_t i2cModuleId, UINT8 * buffer, size_t numToWrite, BOOL oPoolSlaveAcknowledge);
static inline INT32   I2cReadRxFifo                 (I2cModule_t i2cModuleId, UINT8 *buffer , size_t numToRead);
static inline INT8    I2cEnableInterrupt            (I2cModule_t i2cModuleId, I2cInterruptType_t interruptType);
static inline INT8    I2cDisableInterrupt           (I2cModule_t i2cModuleId, I2cInterruptType_t interruptType);
static inline INT8    I2cConfigInterrupt            (I2cModule_t i2cModuleId, UINT8  priority         , UINT8 subpriority);
static inline INT8    I2cEepromSendByte             (I2cModule_t i2cModuleId, UINT16 eepromInternalReg, UINT8 data);
static inline INT8    I2cEepromReadByte             (I2cModule_t i2cModuleId, UINT16 eepromInternalReg, UINT8 *pData);
static inline INT8    I2cSlave10BitsAddressSendData (I2cModule_t i2cModuleId, UINT16 slaveAddress, UINT8 *pData   , UINT8 nBytesToSend);
static inline INT8    I2cSlave7BitsAddressSendData  (I2cModule_t i2cModuleId, UINT8  slaveAddress, UINT8 *pData   , UINT8 nBytesToSend);
static inline INT8    I2cSlave10BitsAddressReadData (I2cModule_t i2cModuleId, UINT16 slaveAddress, UINT8 *slaveReg, UINT8  slaveRegSize, UINT8 *pData, UINT8 nBytesToRead);
static inline INT8    I2cSlave7BitsAddressReadData  (I2cModule_t i2cModuleId, UINT8  slaveAddress, UINT8 *slaveReg, UINT8  slaveRegSize, UINT8 *pData, UINT8 nBytesToRead);


//==============================================================================
// I2C internal functions protoypes
//==============================================================================
BOOL StartTransfer    ( BOOL restart  , I2cModule_t i2cModuleId );
BOOL TransmitOneByte  ( UINT8 data    , I2cModule_t i2cModuleId );
void StopTransfer     ( I2cModule_t i2cModuleId );


//==============================================================================
// Public structure of functions for the I2C
//==============================================================================
struct sChinookI2c I2c =
{
   .Open                        = &I2cOpen
  ,.Close                       = &I2cClose
  ,.AddDataToFifoReadQueue      = &I2cAddDataToFifoReadQueue
  ,.AddDataToFifoWriteQueue     = &I2cAddDataToFifoWriteQueue
  ,.ReadRxFifo                  = &I2cReadRxFifo
  ,.ConfigInterrupt             = &I2cConfigInterrupt
  ,.EnableInterrupt             = &I2cEnableInterrupt
  ,.DisableInterrupt            = &I2cDisableInterrupt
  ,.EepromSendByte              = &I2cEepromSendByte
  ,.EepromReadByte              = &I2cEepromReadByte
  ,.Slave10BitsAddressSendData  = &I2cSlave10BitsAddressSendData
  ,.Slave7BitsAddressSendData   = &I2cSlave7BitsAddressSendData
  ,.Slave10BitsAddressReadData  = &I2cSlave10BitsAddressReadData
  ,.Slave7BitsAddressReadData   = &I2cSlave7BitsAddressReadData
  ,.Var.eepromAddress.byte      = 0b10100000          // Address used for all EEPROMs
           
  // FIFO INIT
  ,.Var.i2cReadQueue[0].fifo         = {0}
  ,.Var.i2cReadQueue[0].maxBufSize   = I2C_FIFO_LENGTH
           
  ,.Var.i2cWriteQueue[0].fifo         = {0}
  ,.Var.i2cWriteQueue[0].maxBufSize   = I2C_FIFO_LENGTH
           
  ,.Var.i2cUserFifo[0].fifo           = {0}
  ,.Var.i2cUserFifo[0].maxBufSize     = I2C_FIFO_LENGTH
};


//==============================================================================
// Private functions declarations
//==============================================================================

static inline INT8 I2cOpen (I2cModule_t i2cModuleId, I2cFreq_t i2cFreq)
{

  UINT32 actualClock;

  // Set the I2C baudrate
  actualClock = I2CSetFrequency(i2cModuleId, GetPeripheralClock(), i2cFreq*1000);
  
  if ( abs(actualClock - i2cFreq*1000) > i2cFreq*1000/10 )
  {
    return -1;    // Frequency did not work
  }

  I2CEnable(i2cModuleId, TRUE);     // Enable the I2C bus
  
  // Init flags
  I2c.Var.oRxDataAvailable        [0] = 0;
  I2c.Var.oI2cWriteIsRunning      [0] = 0;
  I2c.Var.oI2cReadIsRunning       [0] = 0;
  I2c.Var.oReadDataInNextInterrupt[0] = 0;
  I2c.Var.oPoolSlaveAcknowledge   [0] = 0;
  I2c.Var.oSecondStopAfterPooling [0] = 0;

  // Init FIFO buffers
  I2c.Var.i2cReadQueue[0].bufEmpty    = 1;
  I2c.Var.i2cReadQueue[0].bufFull     = 0;
  I2c.Var.i2cReadQueue[0].inIdx       = 0;
  I2c.Var.i2cReadQueue[0].outIdx      = 0;
  I2c.Var.i2cReadQueue[0].bufLength   = 0;

  I2c.Var.i2cWriteQueue[0].bufEmpty   = 1;
  I2c.Var.i2cWriteQueue[0].bufFull    = 0;
  I2c.Var.i2cWriteQueue[0].inIdx      = 0;
  I2c.Var.i2cWriteQueue[0].outIdx     = 0;
  I2c.Var.i2cWriteQueue[0].bufLength  = 0;

  I2c.Var.i2cUserFifo[0].bufEmpty     = 1;
  I2c.Var.i2cUserFifo[0].bufFull      = 0;
  I2c.Var.i2cUserFifo[0].inIdx        = 0;
  I2c.Var.i2cUserFifo[0].outIdx       = 0;
  I2c.Var.i2cUserFifo[0].bufLength    = 0;

  return 0;   // EXIT_SUCCESS
}


static inline void I2cClose (I2cModule_t i2cModuleId)
{
  I2CEnable(i2cModuleId, FALSE);    // Disable the I2C bus
}


static inline INT8 I2cConfigInterrupt (I2cModule_t i2cModuleId, UINT8 priority, UINT8 subpriority)
{
#if defined __32MX795F512L__
  switch (i2cModuleId)
  {
    case I2C1     :     INTSetVectorPriority   (INT_I2C_1_VECTOR, priority);
                        INTSetVectorSubPriority(INT_I2C_1_VECTOR, subpriority);
                        break;
    case I2C2     :     INTSetVectorPriority   (INT_I2C_2_VECTOR, priority);
                        INTSetVectorSubPriority(INT_I2C_2_VECTOR, subpriority);
                        break;
    case I2C3     :     INTSetVectorPriority   (INT_I2C_3_VECTOR, priority);
                        INTSetVectorSubPriority(INT_I2C_3_VECTOR, subpriority);
                        break;
    case I2C4     :     INTSetVectorPriority   (INT_I2C_4_VECTOR, priority);
                        INTSetVectorSubPriority(INT_I2C_4_VECTOR, subpriority);
                        break;
    case I2C5     :     INTSetVectorPriority   (INT_I2C_5_VECTOR, priority);
                        INTSetVectorSubPriority(INT_I2C_5_VECTOR, subpriority);
                        break;
    default       :     return -1;  // EXIT_FAILURE
  }
#elif defined __32MX320F128H__
  switch (i2cModuleId)
  {
    case I2C1     :     INTSetVectorPriority   (INT_I2C_1_VECTOR, priority);
                        INTSetVectorSubPriority(INT_I2C_1_VECTOR, subpriority);
                        break;
    case I2C2     :     INTSetVectorPriority   (INT_I2C_2_VECTOR, priority);
                        INTSetVectorSubPriority(INT_I2C_2_VECTOR, subpriority);
                        break;
    default       :     return -1;  // EXIT_FAILURE
  }
#elif defined __32MX795F512H__
  switch (i2cModuleId)
  {
    case I2C1     :     INTSetVectorPriority   (INT_I2C_1_VECTOR, priority);
                        INTSetVectorSubPriority(INT_I2C_1_VECTOR, subpriority);
                        break;
    case I2C3     :     INTSetVectorPriority   (INT_I2C_3_VECTOR, priority);
                        INTSetVectorSubPriority(INT_I2C_3_VECTOR, subpriority);
                        break;
    case I2C4     :     INTSetVectorPriority   (INT_I2C_4_VECTOR, priority);
                        INTSetVectorSubPriority(INT_I2C_4_VECTOR, subpriority);
                        break;
    case I2C5     :     INTSetVectorPriority   (INT_I2C_5_VECTOR, priority);
                        INTSetVectorSubPriority(INT_I2C_5_VECTOR, subpriority);
                        break;
    default       :     return -1;  // EXIT_FAILURE
  }
#endif
  return 0;   // EXIT_SUCCESS
}


static inline INT8 I2cDisableInterrupt (I2cModule_t i2cModuleId, I2cInterruptType_t interruptType)
{
#if defined __32MX795F512L__
  switch (i2cModuleId)
  {
    case I2C1     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C1S);
                                                            INTEnable(INT_I2C1S, INT_DISABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C1M);
                                                            INTEnable(INT_I2C1M, INT_DISABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C1B);
                                                            INTEnable(INT_I2C1B, INT_DISABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    case I2C2     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C2S);
                                                            INTEnable(INT_I2C2S, INT_DISABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C2M);
                                                            INTEnable(INT_I2C2M, INT_DISABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C2B);
                                                            INTEnable(INT_I2C2B, INT_DISABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    case I2C3     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C3S);
                                                            INTEnable(INT_I2C3S, INT_DISABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C3M);
                                                            INTEnable(INT_I2C3M, INT_DISABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C3B);
                                                            INTEnable(INT_I2C3B, INT_DISABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    case I2C4     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C4S);
                                                            INTEnable(INT_I2C4S, INT_DISABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C4M);
                                                            INTEnable(INT_I2C4M, INT_DISABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C4B);
                                                            INTEnable(INT_I2C4B, INT_DISABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    case I2C5     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C5S);
                                                            INTEnable(INT_I2C5S, INT_DISABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C5M);
                                                            INTEnable(INT_I2C5M, INT_DISABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C5B);
                                                            INTEnable(INT_I2C5B, INT_DISABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    default       : return -1;  // EXIT_FAILURE
  }
#elif defined __32MX320F128H__
  switch (i2cModuleId)
  {
    case I2C1     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C1S);
                                                            INTEnable(INT_I2C1S, INT_DISABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C1M);
                                                            INTEnable(INT_I2C1M, INT_DISABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C1B);
                                                            INTEnable(INT_I2C1B, INT_DISABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    case I2C2     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C2S);
                                                            INTEnable(INT_I2C2S, INT_DISABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C2M);
                                                            INTEnable(INT_I2C2M, INT_DISABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C2B);
                                                            INTEnable(INT_I2C2B, INT_DISABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    default       : return -1;  // EXIT_FAILURE
  }
#elif defined __32MX795F512H__
  switch (i2cModuleId)
  {
    case I2C1     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C1S);
                                                            INTEnable(INT_I2C1S, INT_DISABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C1M);
                                                            INTEnable(INT_I2C1M, INT_DISABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C1B);
                                                            INTEnable(INT_I2C1B, INT_DISABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    case I2C3     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C3S);
                                                            INTEnable(INT_I2C3S, INT_DISABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C3M);
                                                            INTEnable(INT_I2C3M, INT_DISABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C3B);
                                                            INTEnable(INT_I2C3B, INT_DISABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    case I2C4     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C4S);
                                                            INTEnable(INT_I2C4S, INT_DISABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C4M);
                                                            INTEnable(INT_I2C4M, INT_DISABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C4B);
                                                            INTEnable(INT_I2C4B, INT_DISABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    case I2C5     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C5S);
                                                            INTEnable(INT_I2C5S, INT_DISABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C5M);
                                                            INTEnable(INT_I2C5M, INT_DISABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C5B);
                                                            INTEnable(INT_I2C5B, INT_DISABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    default       : return -1;  // EXIT_FAILURE
  }
#endif
  return 0;   // EXIT_SUCCESS
}


static inline INT8 I2cEnableInterrupt (I2cModule_t i2cModuleId, I2cInterruptType_t interruptType)
{
#if defined __32MX795F512L__
  switch (i2cModuleId)
  {
    case I2C1     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C1S);
                                                            INTEnable(INT_I2C1S, INT_ENABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C1M);
                                                            INTEnable(INT_I2C1M, INT_ENABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C1B);
                                                            INTEnable(INT_I2C1B, INT_ENABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    case I2C2     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C2S);
                                                            INTEnable(INT_I2C2S, INT_ENABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C2M);
                                                            INTEnable(INT_I2C2M, INT_ENABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C2B);
                                                            INTEnable(INT_I2C2B, INT_ENABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    case I2C3     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C3S);
                                                            INTEnable(INT_I2C3S, INT_ENABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C3M);
                                                            INTEnable(INT_I2C3M, INT_ENABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C3B);
                                                            INTEnable(INT_I2C3B, INT_ENABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    case I2C4     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C4S);
                                                            INTEnable(INT_I2C4S, INT_ENABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C4M);
                                                            INTEnable(INT_I2C4M, INT_ENABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C4B);
                                                            INTEnable(INT_I2C4B, INT_ENABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    case I2C5     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C5S);
                                                            INTEnable(INT_I2C5S, INT_ENABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C5M);
                                                            INTEnable(INT_I2C5M, INT_ENABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C5B);
                                                            INTEnable(INT_I2C5B, INT_ENABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    default       : return -1;  // EXIT_FAILURE
  }
#elif defined __32MX320F128H__
  switch (i2cModuleId)
  {
    case I2C1     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C1S);
                                                            INTEnable(INT_I2C1S, INT_ENABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C1M);
                                                            INTEnable(INT_I2C1M, INT_ENABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C1B);
                                                            INTEnable(INT_I2C1B, INT_ENABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    case I2C2     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C2S);
                                                            INTEnable(INT_I2C2S, INT_ENABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C2M);
                                                            INTEnable(INT_I2C2M, INT_ENABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C2B);
                                                            INTEnable(INT_I2C2B, INT_ENABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    default       : return -1;  // EXIT_FAILURE
  }
#elif defined __32MX795F512H__
  switch (i2cModuleId)
  {
    case I2C1     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C1S);
                                                            INTEnable(INT_I2C1S, INT_ENABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C1M);
                                                            INTEnable(INT_I2C1M, INT_ENABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C1B);
                                                            INTEnable(INT_I2C1B, INT_ENABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    case I2C3     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C3S);
                                                            INTEnable(INT_I2C3S, INT_ENABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C3M);
                                                            INTEnable(INT_I2C3M, INT_ENABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C3B);
                                                            INTEnable(INT_I2C3B, INT_ENABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    case I2C4     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C4S);
                                                            INTEnable(INT_I2C4S, INT_ENABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C4M);
                                                            INTEnable(INT_I2C4M, INT_ENABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C4B);
                                                            INTEnable(INT_I2C4B, INT_ENABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    case I2C5     : switch (interruptType)
                    {
                      case I2C_SLAVE_INTERRUPT          :   INTClearFlag(INT_I2C5S);
                                                            INTEnable(INT_I2C5S, INT_ENABLED);
                                                            break;
                      case I2C_MASTER_INTERRUPT         :   INTClearFlag(INT_I2C5M);
                                                            INTEnable(INT_I2C5M, INT_ENABLED);
                                                            break;
                      case I2C_BUS_COLLISION_INTERRUPT  :   INTClearFlag(INT_I2C5B);
                                                            INTEnable(INT_I2C5B, INT_ENABLED);
                                                            break;
                      default                           :   return -1;  // EXIT_FAILURE
                    }
                    break;

    default       : return -1;  // EXIT_FAILURE
  }
#endif
  return 0;   // EXIT_SUCCESS
}


static inline INT8 I2cEepromSendByte (I2cModule_t i2cModuleId, UINT16 eepromInternalReg, UINT8 data)
{
  UINT8  i2cData[4] = {0}
        ,i          =  0
        ;

  INT8   err        =  0
        ;

  BOOL   Acknowledged
        ;

  I2c.Var.eepromAddress.rw = I2C_WRITE;

  i2cData[0] = I2c.Var.eepromAddress.byte;  // Slave address
  i2cData[1] = eepromInternalReg >> 8;      // HIGH byte of EEPROM internal memory
  i2cData[2] = eepromInternalReg;           // LOW byte of EEPROM internal memory
  i2cData[3] = data;                        // Data to write


  if( !StartTransfer(FALSE, i2cModuleId) )
  {
    return -1;  // Start transfer failed
  }

  // Transmit all data
  while( !(err < 0) && (i < 4) )
  {
    // Transmit a byte
    if (TransmitOneByte(i2cData[i], i2cModuleId))
    {
      // Advance to the next byte
      i++;

      // Verify that the byte was acknowledged
      if(!I2CByteWasAcknowledged(i2cModuleId))
      {
        err = -1;
      }
    }
    else
    {
      err = -1;
    }
  }

  // End the transfer
  StopTransfer(i2cModuleId);
  if (err < 0)
  {
    return err;   // EXIT_FAILURE
  }

  // Wait for EEPROM to complete write process, by polling the ack status.
  Acknowledged = FALSE;
  do
  {
    // Start the transfer to address the EEPROM
    if( !StartTransfer(FALSE, i2cModuleId) )
    {
      return -1;   // EXIT_FAILURE
    }

    // Transmit just the EEPROM's address
    if (TransmitOneByte(I2c.Var.eepromAddress.byte, i2cModuleId))
    {
      // Check to see if the byte was acknowledged
      Acknowledged = I2CByteWasAcknowledged(0);
    }
    else
    {
      err = -1;
    }

    // End the transfer (stop here if an error occured)
    StopTransfer(i2cModuleId);
    if (err < 0)
    {
      return err;   // EXIT_FAILURE
    }

  } while (Acknowledged != TRUE);

  return err;
}


static inline INT8 I2cEepromReadByte (I2cModule_t i2cModuleId, UINT16 eepromInternalReg, UINT8 *pData)
{
  UINT8   i2cData[3] = {0}
        ,i          =  0
        ;

  INT8   err        =  0
        ;

  I2c.Var.eepromAddress.rw = I2C_WRITE;

  i2cData[0] = I2c.Var.eepromAddress.byte;  // Slave address
  i2cData[1] = eepromInternalReg >> 8;      // HIGH byte of EEPROM internal memory
  i2cData[2] = eepromInternalReg;           // LOW byte of EEPROM internal memory


  // Start the transfer to read the EEPROM.
  if( !StartTransfer(FALSE, i2cModuleId) )
  {
    return -1;  // Start transfer failed
  }

  // Address the EEPROM.
   while( !(err < 0) && (i < 3) )
  {
    // Transmit a byte
    if (TransmitOneByte(i2cData[i], i2cModuleId))
    {
      // Advance to the next byte
      i++;
    }
    else
    {
      err = -1;
    }

    // Verify that the byte was acknowledged
    if(!I2CByteWasAcknowledged(i2cModuleId))
    {
      err = -1;
    }
  }

  // Restart and send the EEPROM's internal address to switch to a read transfer
  if( !(err < 0) )
  {
    // Send a Repeated Started condition
    if( !StartTransfer(TRUE, i2cModuleId) )
    {
      return -1;    // EXIT_FAILURE
    }

    // Transmit the address with the READ bit set
    I2c.Var.eepromAddress.rw = I2C_READ;
    if (TransmitOneByte(I2c.Var.eepromAddress.byte, i2cModuleId))
    {
      // Verify that the byte was acknowledged
      if(!I2CByteWasAcknowledged(i2cModuleId))
      {
        err = -1;
      }
    }
    else
    {
      err = -1;
    }
  }

  // Read the data from the desired address
  if( !(err < 0) )
  {
    if(I2CReceiverEnable(i2cModuleId, TRUE) == I2C_RECEIVE_OVERFLOW)
    {
      err = -1;
    }
    else
    {
      while(!I2CReceivedDataIsAvailable(i2cModuleId));
      *pData = I2CGetByte(i2cModuleId);
    }

  }

  // End the transfer (stop here if an error occured)
  StopTransfer(i2cModuleId);

  return err;
}


static inline INT8 I2cSlave10BitsAddressSendData (I2cModule_t i2cModuleId, UINT16 slaveAddress, UINT8 *pData, UINT8 nBytesToSend)
{
  UINT8   i =  0;

  I2C_10_BIT_ADDRESS  slaveAddress10Bits;

  UINT8 slaveAddressSize = 2;

  UINT8 i2cData[slaveAddressSize + nBytesToSend];

  UINT32 nDataToSend;

  INT8   err        =  0
        ;

  BOOL   Acknowledged
        ;

  I2C_FORMAT_10_BIT_ADDRESS(slaveAddress10Bits, slaveAddress, I2C_WRITE);
  i2cData[0] = slaveAddress10Bits.high_bits;
  i2cData[1] = slaveAddress10Bits.low_bits;

  for (i = 0; i < nBytesToSend; i++)
  {
    i2cData[slaveAddressSize + i] = pData[i];
  }

  i = 0;

  nDataToSend = nBytesToSend + slaveAddressSize;

  if( !StartTransfer(FALSE, i2cModuleId) )
  {
    return -1;  // Start transfer failed
  }

  // Transmit all data
  while( !(err < 0) && (i < nDataToSend) )
  {
    // Transmit a byte
    if (TransmitOneByte(i2cData[i], i2cModuleId))
    {
      // Advance to the next byte
      i++;

      // Verify that the byte was acknowledged
      if(!I2CByteWasAcknowledged(i2cModuleId))
      {
        err = -1;
      }
    }
    else
    {
      err = -1;
    }
  }

  // End the transfer
  StopTransfer(i2cModuleId);
  if (err < 0)
  {
    return err;   // EXIT_FAILURE
  }

  // Wait for EEPROM to complete write process, by polling the ack status.
  Acknowledged = FALSE;
  do
  {
    // Start the transfer to address the EEPROM
    if( !StartTransfer(FALSE, i2cModuleId) )
    {
      return -1;   // EXIT_FAILURE
    }

    // Transmit just the EEPROM's address
    if (TransmitOneByte(slaveAddress10Bits.high_bits, i2cModuleId))
    {
      // Check to see if the byte was acknowledged
      Acknowledged = I2CByteWasAcknowledged(i2cModuleId);
    }
    else
    {
      err = -1;
    }
    if (TransmitOneByte(slaveAddress10Bits.low_bits, i2cModuleId))
    {
      // Check to see if the byte was acknowledged
      Acknowledged = I2CByteWasAcknowledged(i2cModuleId);
    }
    else
    {
      err = -1;
    }

    // End the transfer (stop here if an error occured)
    StopTransfer(i2cModuleId);
    if (err < 0)
    {
      return err;   // EXIT_FAILURE
    }

  } while (Acknowledged != TRUE);

  return err;
}


static inline INT8 I2cSlave7BitsAddressSendData (I2cModule_t i2cModuleId, UINT8 slaveAddress, UINT8 *pData, UINT8 nBytesToSend)
{
  UINT8   i =  0;

  I2C_7_BIT_ADDRESS  slaveAddress7Bits;

  UINT8 slaveAddressSize = 1;

  UINT8 i2cData[slaveAddressSize + nBytesToSend];

  UINT32 nDataToSend;

  INT8   err        =  0
        ;

  BOOL   Acknowledged
        ;

  I2C_FORMAT_7_BIT_ADDRESS(slaveAddress7Bits, slaveAddress, I2C_WRITE);
  i2cData[0] = slaveAddress7Bits.byte;

  for (i = 0; i < nBytesToSend; i++)
  {
    i2cData[slaveAddressSize + i] = pData[i];
  }

  i = 0;

  nDataToSend = nBytesToSend + slaveAddressSize;

  if( !StartTransfer(FALSE, i2cModuleId) )
  {
    return -1;  // Start transfer failed
  }

  // Transmit all data
  while( !(err < 0) && (i < nDataToSend) )
  {
    // Transmit a byte
    if (TransmitOneByte(i2cData[i], i2cModuleId))
    {
      // Advance to the next byte
      i++;

      // Verify that the byte was acknowledged
      if(!I2CByteWasAcknowledged(i2cModuleId))
      {
        err = -1;
      }
    }
    else
    {
      err = -1;
    }
  }

  // End the transfer
  StopTransfer(i2cModuleId);
  if (err < 0)
  {
    return err;   // EXIT_FAILURE
  }

  // Wait for EEPROM to complete write process, by polling the ack status.
  Acknowledged = FALSE;
  do
  {
    // Start the transfer to address the EEPROM
    if( !StartTransfer(FALSE, i2cModuleId) )
    {
      return -1;   // EXIT_FAILURE
    }

    // Transmit just the EEPROM's address
    if (TransmitOneByte(slaveAddress7Bits.byte, i2cModuleId))
    {
      // Check to see if the byte was acknowledged
      Acknowledged = I2CByteWasAcknowledged(i2cModuleId);
    }
    else
    {
      err = -1;
    }

    // End the transfer (stop here if an error occured)
    StopTransfer(i2cModuleId);
    if (err < 0)
    {
      return err;   // EXIT_FAILURE
    }

  } while (Acknowledged != TRUE);

  return err;
}


static inline INT8 I2cSlave10BitsAddressReadData (I2cModule_t i2cModuleId, UINT16 slaveAddress, UINT8 *slaveReg, UINT8 slaveRegSize, UINT8 *pData, UINT8 nBytesToRead)
{
  UINT8   i =  0;

  I2C_10_BIT_ADDRESS  slaveAddress10Bits;

  UINT8  slaveAddressSize                 = 2
        ,slaveAddressSizePlusSlaveRegSize = slaveAddressSize + slaveRegSize
        ;

  UINT8 i2cData[slaveRegSize + slaveAddressSize];


  UINT32 nDataToRead;

  INT8   err        =  0
        ;

  I2C_FORMAT_10_BIT_ADDRESS(slaveAddress10Bits, slaveAddress, I2C_WRITE);
  i2cData[0] = slaveAddress10Bits.high_bits;
  i2cData[1] = slaveAddress10Bits.low_bits;
  for (i = 0; i < slaveRegSize; i++)
  {
    i2cData[i + slaveAddressSize] = slaveReg[i];
  }

  i = 0;

  // Start the transfer to read the EEPROM.
  if( !StartTransfer(FALSE, i2cModuleId) )
  {
    return -1;  // Start transfer failed
  }

  // Address the EEPROM.
  while( !(err < 0) && (i < slaveAddressSizePlusSlaveRegSize) )
  {
    // Transmit a byte
    if (TransmitOneByte(i2cData[i], i2cModuleId))
    {
      // Advance to the next byte
      i++;
    }
    else
    {
      err = -1;
    }

    // Verify that the byte was acknowledged
    if(!I2CByteWasAcknowledged(i2cModuleId))
    {
      err = -1;
    }
  }

  // Restart and send the EEPROM's internal address to switch to a read transfer
  if( !(err < 0) )
  {
    // Send a Repeated Started condition
    if( !StartTransfer(TRUE, i2cModuleId) )
    {
      return -1;    // EXIT_FAILURE
    }

    // Transmit the address with the READ bit set
    slaveAddress10Bits.rw = I2C_READ;

    if (TransmitOneByte( slaveAddress10Bits.high_bits, i2cModuleId))
    {
      // Verify that the byte was acknowledged
      if(!I2CByteWasAcknowledged(i2cModuleId))
      {
        err = -1;
      }
    }
    else
    {
      err = -1;
    }

    if ( (TransmitOneByte( slaveAddress10Bits.low_bits, i2cModuleId)) && !(err < 0) )
    {
      // Verify that the byte was acknowledged
      if(!I2CByteWasAcknowledged(i2cModuleId))
      {
        err = -1;
      }
    }
    else
    {
      err = -1;
    }
  }

  i = 0;

  // Read the data from the desired address
  if( !(err < 0) )
  {
    while ( !(err < 0) && (i < nBytesToRead) )
    {
      if(I2CReceiverEnable(i2cModuleId, TRUE) == I2C_RECEIVE_OVERFLOW)
      {
        err = -1;
      }
      else
      {
        while(!I2CReceivedDataIsAvailable(i2cModuleId));
        i++;
        if (i < nBytesToRead)
        {
          I2CAcknowledgeByte(i2cModuleId, TRUE);
          while (!I2CAcknowledgeHasCompleted(i2cModuleId));
        }
        else
        {
          I2CAcknowledgeByte(i2cModuleId, FALSE);
          while (!I2CAcknowledgeHasCompleted(i2cModuleId));
        }

        pData[i-1] = I2CGetByte(i2cModuleId);
      }
    }
  }

  // End the transfer (stop here if an error occured)
  StopTransfer(i2cModuleId);

  return err;
}


static inline INT8 I2cSlave7BitsAddressReadData (I2cModule_t i2cModuleId, UINT8 slaveAddress, UINT8 *slaveReg, UINT8 slaveRegSize, UINT8 *pData, UINT8 nBytesToRead)
{
  UINT8   i =  0;

  I2C_7_BIT_ADDRESS  slaveAddress7Bits;

  UINT8  slaveAddressSize                 = 1
        ,slaveAddressSizePlusSlaveRegSize = slaveAddressSize + slaveRegSize
        ;

  UINT8 i2cData[slaveRegSize + slaveAddressSize];


  UINT32 nDataToRead;

  INT8   err        =  0
        ;

  I2C_FORMAT_7_BIT_ADDRESS(slaveAddress7Bits, slaveAddress, I2C_WRITE);
  i2cData[0] = slaveAddress7Bits.byte;
  for (i = 0; i < slaveRegSize; i++)
  {
    i2cData[i + slaveAddressSize] = slaveReg[i];
  }

  i = 0;

  // Start the transfer to read the EEPROM.
  if( !StartTransfer(FALSE, i2cModuleId) )
  {
    return -1;  // Start transfer failed
  }

  // Address the EEPROM.
  while( !(err < 0) && (i < slaveAddressSizePlusSlaveRegSize) )
  {
    // Transmit a byte
    if (TransmitOneByte(i2cData[i], i2cModuleId))
    {
      // Advance to the next byte
      i++;
    }
    else
    {
      err = -1;
    }

    // Verify that the byte was acknowledged
    if(!I2CByteWasAcknowledged(i2cModuleId))
    {
      err = -1;
    }
  }

  // Restart and send the EEPROM's internal address to switch to a read transfer
  if( !(err < 0) )
  {
    // Send a Repeated Started condition
    if( !StartTransfer(TRUE, i2cModuleId) )
    {
      return -1;    // EXIT_FAILURE
    }

    // Transmit the address with the READ bit set
    slaveAddress7Bits.rw = I2C_READ;

    if (TransmitOneByte( slaveAddress7Bits.byte, i2cModuleId))
    {
      // Verify that the byte was acknowledged
      if(!I2CByteWasAcknowledged(i2cModuleId))
      {
        err = -1;
      }
    }
    else
    {
      err = -1;
    }

  }

  i = 0;

  // Read the data from the desired address
  if( !(err < 0) )
  {
    while ( !(err < 0) && (i < nBytesToRead) )
    {
      if(I2CReceiverEnable(i2cModuleId, TRUE) == I2C_RECEIVE_OVERFLOW)
      {
        err = -1;
      }
      else
      {
        while(!I2CReceivedDataIsAvailable(i2cModuleId));
        i++;
        if (i < nBytesToRead)
        {
          I2CAcknowledgeByte(i2cModuleId, TRUE);
          while (!I2CAcknowledgeHasCompleted(i2cModuleId));
        }
        else
        {
          I2CAcknowledgeByte(i2cModuleId, FALSE);
          while (!I2CAcknowledgeHasCompleted(i2cModuleId));
        }

        pData[i-1] = I2CGetByte(i2cModuleId);
      }
    }
  }

  // End the transfer (stop here if an error occured)
  StopTransfer(i2cModuleId);

  return err;
}

static inline INT32 I2cReadRxFifo (I2cModule_t i2cModuleId, UINT8 *buffer, size_t numToRead)
{
  UINT16 i;
  
  if (numToRead == 0)
  {
    return -1;
  }
  
  if (!I2c.Var.oRxDataAvailable[0])
  {
    return -1;
  }
  
  if (I2c.Var.i2cUserFifo[0].bufEmpty)
  {
    return -1;
  }
  
  if (numToRead > I2c.Var.i2cUserFifo[0].bufLength)
  {
    return -1;
  }
  
  sI2cCmdBuffer_t rxData[numToRead];
  
  //Sensitive Code. Disable temporarily master interrupts
  I2c.DisableInterrupt(i2cModuleId, I2C_MASTER_INTERRUPT);
  for (i = 0; i < numToRead; i++)
  {
    I2cFifoRead ((void *) &I2c.Var.i2cUserFifo[0], &rxData[i]);
  }
  I2c.EnableInterrupt(i2cModuleId, I2C_MASTER_INTERRUPT);
  
  // Copy to user
  for (i = 0; i < numToRead; i++)
  {
    buffer[i] = rxData[i].data;
  }
  
  return i;
}


static inline INT32 I2cAddDataToFifoReadQueue (I2cModule_t i2cModuleId, UINT8 * slaveAddPlusRegBuf, size_t sizeOfSlaveBuf, size_t numToRead)
{

  UINT16 nSpaceAvailable = 0
        ,i = 0
        ,iCmdWritten = 0
        ;
  INT8 err;
  UINT8 dummyByte = 0x00;
  sI2cCmdBuffer_t cmd;
  
  I2C_7_BIT_ADDRESS slaveAddress;
  
  I2cMasterStates_t nextState[] = 
  {
    I2C_MASTER_START_CONDITION
   ,I2C_MASTER_TRANSMIT_DATA
   ,I2C_MASTER_REPEAT_START
   ,I2C_MASTER_TRANSMIT_DATA
   ,I2C_MASTER_RECEIVE_DATA
   ,I2C_MASTER_SEND_NACK
   ,I2C_MASTER_SEND_ACK
   ,I2C_MASTER_STOP_CONDITION
   ,I2C_MASTER_DONE
  };

  if (numToRead == 0)
  {
    return -1;
  }
  
  if (I2c.Var.oI2cWriteIsRunning[0])
  {
    return -1;
  }
  
  if (I2c.Var.i2cReadQueue[0].bufFull)
  {
    return -1;
  }

  nSpaceAvailable = I2c.Var.i2cReadQueue[0].maxBufSize - I2c.Var.i2cReadQueue[0].bufLength;

  if (nSpaceAvailable < (2*numToRead + 8))
  {
    if (!I2c.Var.i2cReadQueue[0].bufEmpty && !I2c.Var.oI2cReadIsRunning[0])
    {
      I2c.Var.oI2cReadIsRunning[0] = 1;
    }
    return -1;
  }
  
  cmd.data  = dummyByte;
  cmd.state = nextState[0];
  err = I2cFifoWrite((void *) &I2c.Var.i2cReadQueue[0], &cmd);   // Start condition
  if (err < 0)
  {
    return -1;
  }  
  else
  {
    iCmdWritten++;
  }
  
  slaveAddress.byte = slaveAddPlusRegBuf[0];
  slaveAddress.rw   = I2C_WRITE;
  slaveAddPlusRegBuf[0] = slaveAddress.byte;

  cmd.state = nextState[1];
  for (i = 0; i < sizeOfSlaveBuf; i++)
  {
    cmd.data = slaveAddPlusRegBuf[i];
    err = I2cFifoWrite((void *) &I2c.Var.i2cReadQueue[0], &cmd);  // Transmit
    if (err < 0)
    {
      for (i = 0; i < iCmdWritten; i++)
      {
        I2cFifoRead((void *) &I2c.Var.i2cReadQueue[0], &cmd);
      }
      return -1;
    }
    else
    {
      iCmdWritten++;
    }
  }

  cmd.data  = dummyByte;
  cmd.state = nextState[2];
  err = I2cFifoWrite((void *) &I2c.Var.i2cReadQueue[0], &cmd);   // Repeat start
  if (err < 0)
  {
    for (i = 0; i < iCmdWritten; i++)
    {
      I2cFifoRead((void *) &I2c.Var.i2cReadQueue[0], &cmd);
    }
    return -1;
  }
  else
  {
    iCmdWritten++;
  }

  slaveAddress.rw = I2C_READ;
  cmd.data = slaveAddress.byte;
  cmd.state = nextState[3];
  err = I2cFifoWrite((void *) &I2c.Var.i2cReadQueue[0], &cmd);   // Transmit 
  if (err < 0)
  {
    for (i = 0; i < iCmdWritten; i++)
    {
      I2cFifoRead((void *) &I2c.Var.i2cReadQueue[0], &cmd);
    }
    return -1;
  }
  else
  {
    iCmdWritten++;
  }

  
  for (i = 0; i < numToRead - 1; i++)
  {
    cmd.state = nextState[4];
    err = I2cFifoWrite((void *) &I2c.Var.i2cReadQueue[0], &cmd);  // Receive
    if (err < 0)
    {
      for (i = 0; i < iCmdWritten; i++)
      {
        I2cFifoRead((void *) &I2c.Var.i2cReadQueue[0], &cmd);
      }
      return -1;
    }
    else
    {
      iCmdWritten++;
    }
    
    cmd.state = nextState[6];
    err = I2cFifoWrite((void *) &I2c.Var.i2cReadQueue[0], &cmd);  // ACK
    if (err < 0)
    {
      for (i = 0; i < iCmdWritten; i++)
      {
        I2cFifoRead((void *) &I2c.Var.i2cReadQueue[0], &cmd);
      }
      return -1;
    }
    else
    {
      iCmdWritten++;
    }
  }
  
  cmd.data = dummyByte;
  cmd.state = nextState[4];
  err = I2cFifoWrite((void *) &I2c.Var.i2cReadQueue[0], &cmd);  // Receive
  if (err < 0)
  {
    for (i = 0; i < iCmdWritten; i++)
    {
      I2cFifoRead((void *) &I2c.Var.i2cReadQueue[0], &cmd);
    }
    return -1;
  }
  else
  {
    iCmdWritten++;
  }
  
  cmd.state = nextState[5];
  err = I2cFifoWrite((void *) &I2c.Var.i2cReadQueue[0], &cmd);  // NACK
  if (err < 0)
  {
    for (i = 0; i < iCmdWritten; i++)
    {
      I2cFifoRead((void *) &I2c.Var.i2cReadQueue[0], &cmd);
    }
    return -1;
  }
  else
  {
    iCmdWritten++;
  }
  
  cmd.state = nextState[7];
  err = I2cFifoWrite((void *) &I2c.Var.i2cReadQueue[0], &cmd);  // Stop condition
  if (err < 0)
  {
    for (i = 0; i < iCmdWritten; i++)
    {
      I2cFifoRead((void *) &I2c.Var.i2cReadQueue[0], &cmd);
    }
    return -1;
  }
  else
  {
    iCmdWritten++;
  }
  
  cmd.state = nextState[8];
  err = I2cFifoWrite((void *) &I2c.Var.i2cReadQueue[0], &cmd);  // Done
  if (err < 0)
  {
    for (i = 0; i < iCmdWritten; i++)
    {
      I2cFifoRead((void *) &I2c.Var.i2cReadQueue[0], &cmd);
    }
    return -1;
  }
  else
  {
    iCmdWritten++;
  }

  INT16 intSource;
  if (!I2c.Var.oI2cReadIsRunning[0])
  {
#if defined __32MX795F512L__
    switch (i2cModuleId)
    {
      case I2C1 :
        intSource = INT_I2C1M;
        break;
      case I2C2 :
        intSource = INT_I2C2M;
        break;
      case I2C3 :
        intSource = INT_I2C3M;
        break;
      case I2C4 :
        intSource = INT_I2C4M;
        break;
      case I2C5 :
        intSource = INT_I2C5M;
        break;
      default :
        break;
    }
#elif defined __32MX795F512H__
    switch (i2cModuleId)
    {
      case I2C1 :
        intSource = INT_I2C1M;
        break;
      case I2C3 :
        intSource = INT_I2C3M;
        break;
      case I2C4 :
        intSource = INT_I2C4M;
        break;
      case I2C5 :
        intSource = INT_I2C5M;
        break;
      default :
        break;
    }
#else
    switch (i2cModuleId)
    {
      case I2C1 :
        intSource = INT_I2C1M;
        break;
      case I2C2 :
        intSource = INT_I2C2M;
        break;
      default :
        break;
    }
#endif

    I2c.Var.oI2cReadIsRunning[0] = 1;
    INTSetFlag(intSource);
  }

  return numToRead;
}


static inline INT32 I2cAddDataToFifoWriteQueue (I2cModule_t i2cModuleId, UINT8 * buffer, size_t numToWrite, BOOL oPoolSlaveAcknowledge)
{

  UINT16 nSpaceAvailable = 0
        ,i = 0
        ,iCmdWritten = 0
        ;
  INT8 err;
  UINT8 dummyByte = 0x00;
  sI2cCmdBuffer_t cmd;
  
  I2C_7_BIT_ADDRESS slaveAddress;
  
  I2cMasterStates_t nextState[] = 
  {
    I2C_MASTER_START_CONDITION
   ,I2C_MASTER_TRANSMIT_DATA
   ,I2C_MASTER_STOP_CONDITION
   ,I2C_MASTER_START_CONDITION
   ,I2C_MASTER_TRANSMIT_DATA
   ,I2C_MASTER_STOP_CONDITION
   ,I2C_MASTER_DONE
  };

  if (numToWrite == 0)
  {
    return -1;
  }
  
  if (I2c.Var.oI2cReadIsRunning[0])
  {
    return -1;
  }

  if (I2c.Var.i2cWriteQueue[0].bufFull)
  {
    return -1;
  }

  nSpaceAvailable = I2c.Var.i2cWriteQueue[0].maxBufSize - I2c.Var.i2cWriteQueue[0].bufLength;

  if (nSpaceAvailable < (numToWrite + 3))
  {
    if (!I2c.Var.i2cWriteQueue[0].bufEmpty && !I2c.Var.oI2cWriteIsRunning[0])
    {
      I2c.Var.oI2cWriteIsRunning[0] = 1;
    }
    return -1;
  }
  
  cmd.data  = dummyByte;
  cmd.state = nextState[0];
  err = I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[0], &cmd);   // Start condition
  if (err < 0)
  {
    for (i = 0; i < iCmdWritten; i++)
    {
      I2cFifoRead((void *) &I2c.Var.i2cWriteQueue[0], &cmd);
    }
    return -1;
  }
  else
  {
    iCmdWritten++;
  }
  
  
  slaveAddress.byte = buffer[0];
  slaveAddress.rw   = I2C_WRITE;
  buffer[0] = slaveAddress.byte;

  cmd.state = nextState[1];
  for (i = 0; i < numToWrite; i++)
  {
    cmd.data = buffer[i];
    err = I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[0], &cmd);
    if (err < 0)
    {
      for (i = 0; i < iCmdWritten; i++)
      {
        I2cFifoRead((void *) &I2c.Var.i2cWriteQueue[0], &cmd);
      }
      return -1;
    }
    else
    {
      iCmdWritten++;
    }
  }
    
  if (oPoolSlaveAcknowledge)
  {  
    cmd.data  = buffer[0];
    cmd.state = nextState[2];
    err = I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[0], &cmd);   // Stop condition
    if (err < 0)
    {
      for (i = 0; i < iCmdWritten; i++)
      {
        I2cFifoRead((void *) &I2c.Var.i2cWriteQueue[0], &cmd);
      }
      return -1;
    }
    else
    {
      iCmdWritten++;
    }
    
    I2c.Var.oPoolSlaveAcknowledge[0] = TRUE;
  }
  else
  {
  
    cmd.data  = dummyByte;
    cmd.state = nextState[2];
    err = I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[0], &cmd);   // Stop condition
    if (err < 0)
    {
      for (i = 0; i < iCmdWritten; i++)
      {
        I2cFifoRead((void *) &I2c.Var.i2cWriteQueue[0], &cmd);
      }
      return -1;
    }
    else
    {
      iCmdWritten++;
    }
    
    cmd.data  = dummyByte;
    cmd.state = nextState[6];
    err = I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[0], &cmd);   // Done
    if (err < 0)
    {
      for (i = 0; i < iCmdWritten; i++)
      {
        I2cFifoRead((void *) &I2c.Var.i2cWriteQueue[0], &cmd);
      }
      return -1;
    }
    else
    {
      iCmdWritten++;
    }
  }

  INT16 intSource;
  
  if (!I2c.Var.oI2cWriteIsRunning[0])
  {
#if defined __32MX795F512L__
    switch (i2cModuleId)
    {
      case I2C1 :
        intSource = INT_I2C1M;
        break;
      case I2C2 :
        intSource = INT_I2C2M;
        break;
      case I2C3 :
        intSource = INT_I2C3M;
        break;
      case I2C4 :
        intSource = INT_I2C4M;
        break;
      case I2C5 :
        intSource = INT_I2C5M;
        break;
      default :
        break;
    }
#elif defined __32MX795F512H__
    switch (i2cModuleId)
    {
      case I2C1 :
        intSource = INT_I2C1M;
        break;
      case I2C3 :
        intSource = INT_I2C3M;
        break;
      case I2C4 :
        intSource = INT_I2C4M;
        break;
      case I2C5 :
        intSource = INT_I2C5M;
        break;
      default :
        break;
    }
#else
    switch (i2cModuleId)
    {
      case I2C1 :
        intSource = INT_I2C1M;
        break;
      case I2C2 :
        intSource = INT_I2C2M;
        break;
      default :
        break;
    }
#endif
    I2c.Var.oI2cWriteIsRunning[0] = 1;
    INTSetFlag(intSource);
  }

  return numToWrite;
}


//==============================================================================
// I2C internal functions
//==============================================================================


/*******************************************************************************
  Function:
    BOOL StartTransfer( BOOL restart )

  Summary:
    Starts (or restarts) a transfer to/from the EEPROM.

  Description:
    This routine starts (or restarts) a transfer to/from the EEPROM, waiting (in
    a blocking loop) until the start (or re-start) condition has completed.

  Precondition:
    The I2C module must have been initialized.

  Parameters:
    restart - If FALSE, send a "Start" condition
            - If TRUE, send a "Restart" condition

  Returns:
    TRUE    - If successful
    FALSE   - If a collision occured during Start signaling

  Example:
    <code>
    StartTransfer(FALSE);
    </code>

  Remarks:
    This is a blocking routine that waits for the bus to be idle and the Start
    (or Restart) signal to complete.
  *****************************************************************************/


BOOL StartTransfer( BOOL restart , I2cModule_t i2cModule )
{
  I2C_STATUS  status;

  // Send the Start (or Restart) signal
  if(restart)
  {
    I2CRepeatStart(i2cModule);
  }
  else
  {
    // Wait for the bus to be idle, then start the transfer
    while( !I2CBusIsIdle(i2cModule) );

    if(I2CStart(i2cModule) != I2C_SUCCESS)
    {
      return FALSE;
    }
  }

  // Wait for the signal to complete
  do
  {
    status = I2CGetStatus(i2cModule);

  } while ( !(status & I2C_START) );

  return TRUE;
}


/*******************************************************************************
  Function:
    BOOL TransmitOneByte( UINT8 data )

  Summary:
    This transmits one byte to the EEPROM.

  Description:
    This transmits one byte to the EEPROM, and reports errors for any bus
    collisions.

  Precondition:
    The transfer must have been previously started.

  Parameters:
    data    - Data byte to transmit

  Returns:
    TRUE    - Data was sent successfully
    FALSE   - A bus collision occured

  Example:
    <code>
    TransmitOneByte(0xAA);
    </code>

  Remarks:
    This is a blocking routine that waits for the transmission to complete.
  *****************************************************************************/

BOOL TransmitOneByte( UINT8 data , I2cModule_t i2cModule )
{
  // Wait for the transmitter to be ready
  while(!I2CTransmitterIsReady(i2cModule));

  // Transmit the byte
  if(I2CSendByte(i2cModule, data) == I2C_MASTER_BUS_COLLISION)
  {
    return FALSE;
  }

  // Wait for the transmission to finish
  while(!I2CTransmissionHasCompleted(i2cModule));

  return TRUE;
}


/*******************************************************************************
  Function:
    void StopTransfer( void )

  Summary:
    Stops a transfer to/from the EEPROM.

  Description:
    This routine Stops a transfer to/from the EEPROM, waiting (in a
    blocking loop) until the Stop condition has completed.

  Precondition:
    The I2C module must have been initialized & a transfer started.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    StopTransfer();
    </code>

  Remarks:
    This is a blocking routine that waits for the Stop signal to complete.
  *****************************************************************************/

void StopTransfer( I2cModule_t i2cModule )
{
  I2C_STATUS  status;

  // Send the Stop signal
  I2CStop(i2cModule);

  // Wait for the signal to complete
  do
  {
    status = I2CGetStatus(i2cModule);

  } while ( !(status & I2C_STOP) );
}


/*******************************************************************************
  Function:
    inline INT8 I2cFifoWrite (sUartFifoBuffer_t *fifo, UINT8 *data, I2cMasterStates_t *cmd)

  Summary:
    Add a byte to the I2C user's FIFO

  Description:
    This function adds a byte to the I2C user's FIFO and updates all variables
    related to it.
  *****************************************************************************/

inline INT8 I2cFifoWrite (sI2cFifoBuffer_t *dest, sI2cCmdBuffer_t *source)
{
  if (dest->bufFull)
  {
    return -1;
  }
  dest->bufEmpty = 0;
  dest->fifo[dest->inIdx].data  = source->data;
  dest->fifo[dest->inIdx].state = source->state;
  dest->inIdx = (dest->inIdx + 1) % dest->maxBufSize;
  if (dest->inIdx == dest->outIdx)
  {
    dest->bufFull = 1;
  }
  dest->bufLength++;
  return 0;
}


/*******************************************************************************
  Function:
    inline INT8 I2cFifoRead (sUartFifoBuffer_t *fifo, UINT8 *data)

  Summary:
    Remove a byte to the I2C user's FIFO

  Description:
    This function removes a byte to the I2C user's FIFO and updates all variables
    related to it.
  *****************************************************************************/

inline INT8 I2cFifoRead (sI2cFifoBuffer_t *source, sI2cCmdBuffer_t *dest)
{
  if (source->bufEmpty)
  {
    return -1;
  }
  source->bufFull = 0;
  dest->data  = source->fifo[source->outIdx].data;
  dest->state = source->fifo[source->outIdx].state;
  source->outIdx = (source->outIdx + 1) % source->maxBufSize;
  if (source->outIdx == source->inIdx)
  {
    source->bufEmpty = 1;
  }
  source->bufLength--;
  return 0;
}