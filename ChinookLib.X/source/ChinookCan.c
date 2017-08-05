/**********************************************************************
* © 2007 Microchip Technology Inc.
*
* FileName:        ChinookCAN.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC32
* Compiler:        MPLAB® XC32
*
************************************************************************/
#include "..\headers\ChinookCan.h"



#ifndef __32MX320F128H__    // Uno32 doesn't have CAN


//==============================================================================
// Private functions prototypes
//==============================================================================
static inline INT32   CanInit             (CAN_MODULE CanModule, BYTE *CanMessageFifoArea, BYTE NbChannels, BYTE BufferSize, BOOL isLoopbackMode);
static inline void    CanTxSendMsg        (CAN_MODULE CanModule, UINT8 messageSID, UINT64 data);
static inline void    CanTxSendByte       (CAN_MODULE CanModule, UINT8 messageSID, UINT8 data);
static inline void    CanTxSendFloat    (CAN_MODULE CanModule, UINT8 messageSID, float data);
static inline void    CanTxSendByteArray  (CAN_MODULE CanModule, UINT8 messageSID, BYTE *dataArray, BYTE arraySize);
static inline void    CanSetChannel       (CAN_MODULE CanModule, CAN_CHANNEL channel, UINT bufferSize, CanChannelTypes type);
static inline INT32   CanSetChannelMask   (CAN_MODULE CanModule, CAN_CHANNEL channel, CAN_FILTER Filter, UINT32 SID, CAN_FILTER_MASK FilterMask, UINT32 maskbits);
static inline void    CanConfigInterrupt  (CAN_MODULE CanModule, INT_PRIORITY priority, INT_SUB_PRIORITY subpriority );
static inline void    CanDisableInterrupt (CAN_MODULE CanModule);
static inline void    CanEnableInterrupt  (CAN_MODULE CanModule);
static inline void  * CanRxMsgProcess     (CAN_MODULE CanModule);



//==============================================================================
// Public structure of functions for the CAN
//==============================================================================
struct sChinookCan Can =
{
   .Initialize            = &CanInit
  ,.ProcessMessage        = &CanRxMsgProcess
  ,.SendData              = &CanTxSendMsg
  ,.SendFloat             = &CanTxSendFloat
  ,.SendByte              = &CanTxSendByte
  ,.SendByteArray         = &CanTxSendByteArray
  ,.SetChannel            = &CanSetChannel
  ,.SetChannelMask        = &CanSetChannelMask
  ,.EnableInterrupt       = &CanEnableInterrupt
  ,.DisableInterrupt      = &CanDisableInterrupt
  ,.ConfigInterrupt       = &CanConfigInterrupt
};


//==============================================================================
// Variables
//==============================================================================
/* isCan1MsgReceived is true if CAN1 channel 1 received
 * a message. This flag is updated in the CAN1 ISR. */
static volatile BYTE CanBufferSize = 0;
static volatile UINT32 CanFifoSize = 0;

/* isCan2MsgReceived is true if CAN2 channel 1 received
 * a message. This flag is updated in the CAN2 ISR. */
static volatile BOOL isCan2MsgReceived = FALSE;


/*******************************************************************************
 *******************                                       *********************
 ****************       PRIVATE FUNCTION DECLARATIONS         ******************
 *******************                                       *********************
 *******************************************************************************/

static inline INT32 CanInit(CAN_MODULE CanModule, BYTE *CanMessageFifoArea, BYTE NbChannels, BYTE BufferSize, BOOL isLoopbackMode)
{
  if(NbChannels > 32 || BufferSize > 32)
  {
    return -1; //non pas plus que 32 channels et pas plus que 32 de bufferSize
  }
  
	CAN_BIT_CONFIG canBitConfig;

	/* This function will intialize
	 * CAN1 module. */

	/* Step 1: Switch the CAN module
	 * ON and switch it to Configuration
	 * mode. Wait till the switch is
	 * complete */

	CANEnableModule(CanModule,TRUE);

	CANSetOperatingMode(CanModule, CAN_CONFIGURATION);
	while(CANGetOperatingMode(CanModule) != CAN_CONFIGURATION);



	/* Step 2: Configure the CAN Module Clock. The
	 * CAN_BIT_CONFIG data structure is used
	 * for this purpose. The propagation segment,
	 * phase segment 1 and phase segment 2
	 * are configured to have 3TQ. The
   * CANSetSpeed function sets the baud.*/

	canBitConfig.phaseSeg2Tq            = CAN_BIT_3TQ;
	canBitConfig.phaseSeg1Tq            = CAN_BIT_3TQ;
	canBitConfig.propagationSegTq       = CAN_BIT_3TQ;
	canBitConfig.phaseSeg2TimeSelect    = TRUE;
	canBitConfig.sample3Time            = TRUE;
  canBitConfig.syncJumpWidth          = CAN_BIT_1TQ;

  CANSetSpeed(CanModule,&canBitConfig, SYS_FREQ, CAN_BUS_SPEED);

/* Step 3: Assign the buffer area to the
   * CAN module.
   */

  CanBufferSize = BufferSize;
  CanFifoSize = (NbChannels * CanBufferSize * CAN_TX_RX_MESSAGE_SIZE_BYTES);
  //CanMessageFifoArea [NbChannels * CanBufferSize * CAN_TX_RX_MESSAGE_SIZE_BYTES];

  CANAssignMemoryBuffer(CanModule, CanMessageFifoArea, CanFifoSize);

/* Step 4: Configure channel 0 for TX and size of
   * 8 message buffers with RTR disabled and low medium
   * priority. Configure channel 1 for RX and size
   * of 8 message buffers and receive the full message.
   */
  CANConfigureChannelForTx(CanModule, CAN_TX_CHANNEL, 8, CAN_TX_RTR_DISABLED, CAN_LOW_MEDIUM_PRIORITY);
//  CANConfigureChannelForRx(CanModule, CAN_CHANNEL1, 8, CAN_RX_FULL_RECEIVE);

/* Step 5: Configure filters and mask. Configure
   * filter 0 to accept SID messages with ID 0x201.
   * Configure filter mask 0 to compare all the ID
   * bits and to filter by the ID type specified in
   * the filter configuration. Messages accepted by
   * filter 0 should be stored in channel 1. */

    //This is now done using the CanSetChannelMask method
  
//  if(CanModule == CAN1)
//  {
//     /* CANConfigureFilter      (CAN1, CAN_FILTER0, 0xc1, CAN_SID);
//      CANConfigureFilterMask  (CAN1, CAN_FILTER_MASK0, 0x00, CAN_SID, CAN_FILTER_MASK_IDE_TYPE);
//      CANLinkFilterToChannel  (CAN1, CAN_FILTER0, CAN_FILTER_MASK0, CAN_CHANNEL1);
//      CANEnableFilter         (CAN1, CAN_FILTER0, TRUE);*/
//    
//  }
//  else if(CanModule == CAN2)
//  {
//    CANConfigureFilter      (CAN2, CAN_FILTER0, 0xC2, CAN_SID);
//    CANConfigureFilterMask  (CAN2, CAN_FILTER_MASK0, 0x00, CAN_SID, CAN_FILTER_MASK_IDE_TYPE);
//    CANLinkFilterToChannel  (CAN2, CAN_FILTER0, CAN_FILTER_MASK0, CAN_CHANNEL1);
//    CANEnableFilter         (CAN2, CAN_FILTER0, TRUE);
//  }

/* Step 6: Enable interrupt and events. Enable the receive
   * channel not empty event (channel event) and the receive
   * channel event (module event).
   * The interrrupt peripheral library is used to enable
   * the CAN interrupt to the CPU. */

  //CANEnableChannelEvent(CanModule, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
  //CANEnableModuleEvent (CanModule, CAN_RX_EVENT, TRUE);

  /* These functions are from interrupt peripheral
   * library. */

//  if(CanModule == CAN1)
//  {
//    INTEnable(INT_CAN1, INT_ENABLED);
//    INTSetVectorPriority(INT_CAN_1_VECTOR, INT_PRIORITY_LEVEL_4);
//    INTSetVectorSubPriority(INT_CAN_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
//  }
//  else if(CanModule == CAN2)
//  {
//    INTEnable(INT_CAN2, INT_ENABLED);
//    INTSetVectorPriority(INT_CAN_2_VECTOR, INT_PRIORITY_LEVEL_4);
//    INTSetVectorSubPriority(INT_CAN_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
//  }

	/* Step 7: Switch the CAN mode
	 * to normal mode. */
  if(isLoopbackMode)
  {
    CANSetOperatingMode(CanModule, CAN_LOOPBACK);
    while(CANGetOperatingMode(CanModule) != CAN_LOOPBACK);
  }
  else
  {
    CANSetOperatingMode(CanModule, CAN_NORMAL_OPERATION);
    while(CANGetOperatingMode(CanModule) != CAN_NORMAL_OPERATION);
  }
}


static inline void CanTxSendFloat(CAN_MODULE CanModule, UINT8 messageSID, float data)
{
  CANTxMessageBuffer * message;
  /* Get a pointer to the next buffer in the channel
   * check if the returned value is null. */

  message = CANGetTxMessageBuffer(CanModule,CAN_TX_CHANNEL);

  if(message != NULL)
  {

    /* Form a Standard ID CAN message.
     * Start by clearing the buffer.
     * Send message to CAN2.
     * IDE = 0 means Standard ID message.
     * Send one byte of data.
     * This is the payload.					*/


    message->messageWord[0] = 0;
    message->messageWord[1] = 0;
    message->messageWord[2] = 0;
    message->messageWord[3] = 0;

    message->msgSID.SID 	= messageSID;
    message->msgEID.IDE 	= 0;
    message->msgEID.DLC 	= 4;
//    memcpy((void *) &message->data, (void *) &data, sizeof ( message->data));
    memcpy((void *) &message->data[0], (void *) &data, 4);    // 4 bytes in a float


    /* This function lets the CAN module
     * know that the message processing is done
     * and message is ready to be processed. */

    CANUpdateChannel(CanModule,CAN_TX_CHANNEL);

    /* Direct the CAN module to flush the
     * TX channel. This will send any pending
     * message in the TX channel. */

    CANFlushTxChannel(CanModule,CAN_TX_CHANNEL);
  }
}


static inline void * CanRxMsgProcess(CAN_MODULE CanModule)
{
	/* This function will check if a CAN
	 * message is available in CAN1 channel 1.
     * If so , the message is read. Byte 0 of
     * the received message will indicate if
	 * LED6 should be switched ON or OFF. */

	void *message;


  message = CANGetRxMessage(CanModule,CAN_CHANNEL1);

  /* Call the CANUpdateChannel() function to let
   * CAN 1 module know that the message processing
   * is done. Enable the receive channale not empty event
   * so that the CAN module generates an interrupt when
   * the event occurs the next time.*/


  CANUpdateChannel(CanModule, CAN_CHANNEL1);
  CANEnableChannelEvent(CanModule, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);

  return message;
}


static inline void CanTxSendMsg(CAN_MODULE CanModule, UINT8 messageSID, UINT64 data)
{
 /*
  * This function will send a message to
  * CAN2 with SID 202. The data payload
  * size is 1 byte. The value of the LED5Status
  * will be toggled and then sent as
  * the payload. CAN1 Channel 0 is used
  * to send the message
  */

  CANTxMessageBuffer * message;
  /* Get a pointer to the next buffer in the channel
   * check if the returned value is null. */

  message = CANGetTxMessageBuffer(CanModule,CAN_TX_CHANNEL);

  if(message != NULL)
  {

    /* Form a Standard ID CAN message.
     * Start by clearing the buffer.
     * Send message to CAN2.
     * IDE = 0 means Standard ID message.
     * Send one byte of data.
     * This is the payload.					*/


    message->messageWord[0] = 0;
    message->messageWord[1] = 0;
    message->messageWord[2] = 0;
    message->messageWord[3] = 0;

    message->msgSID.SID 	= messageSID;
    message->msgEID.IDE 	= 0;
    message->msgEID.DLC 	= 8;
    memcpy(&message->data, &data, sizeof ( message->data));


    /* This function lets the CAN module
     * know that the message processing is done
     * and message is ready to be processed. */

    CANUpdateChannel(CanModule,CAN_TX_CHANNEL);

    /* Direct the CAN module to flush the
     * TX channel. This will send any pending
     * message in the TX channel. */

    CANFlushTxChannel(CanModule,CAN_TX_CHANNEL);
  }
}


static inline void CanTxSendByte(CAN_MODULE CanModule, UINT8 messageSID, UINT8 data)
{
 /*
  * This function will send a message to
  * CAN2 with SID 202. The data payload
  * size is 1 byte. The value of the LED5Status
  * will be toggled and then sent as
  * the payload. CAN1 Channel 0 is used
  * to send the message
  */

  CANTxMessageBuffer * message;
  /* Get a pointer to the next buffer in the channel
   * check if the returned value is null. */

  message = CANGetTxMessageBuffer(CanModule,CAN_TX_CHANNEL);

  if(message != NULL)
  {

   /* Form a Standard ID CAN message.
    * Start by clearing the buffer.
    * Send message to CAN2.
    * IDE = 0 means Standard ID message.
    * Send one byte of data.
    * This is the payload.					*/


    message->messageWord[0] = 0;
    message->messageWord[1] = 0;
    message->messageWord[2] = 0;
    message->messageWord[3] = 0;

    message->msgSID.SID 	= messageSID;
    message->msgEID.IDE 	= 0;
    message->msgEID.DLC 	= 1;
    message->data[0]      = data;


   /* This function lets the CAN module
    * know that the message processing is done
    * and message is ready to be processed. */

    CANUpdateChannel(CanModule,CAN_TX_CHANNEL);

   /* Direct the CAN module to flush the
    * TX channel. This will send any pending
    * message in the TX channel. */

    CANFlushTxChannel(CanModule,CAN_TX_CHANNEL);
  }
}


static inline void    CanTxSendByteArray    (CAN_MODULE CanModule, UINT8 messageSID, BYTE *dataArray, BYTE arraySize)
{
  CANTxMessageBuffer * message;
  message = CANGetTxMessageBuffer(CanModule,CAN_TX_CHANNEL);

  if(message != NULL)
  {
    message->messageWord[0] = 0;
    message->messageWord[1] = 0;
    message->messageWord[2] = 0;
    message->messageWord[3] = 0;

    message->msgSID.SID 	= messageSID;
    message->msgEID.IDE 	= 0;
    message->msgEID.DLC 	= arraySize;
    memcpy(&message->data, dataArray, arraySize);


   /* This function lets the CAN module
    * know that the message processing is done
    * and message is ready to be processed. */

    CANUpdateChannel(CanModule,CAN_TX_CHANNEL);

   /* Direct the CAN module to flush the
    * TX channel. This will send any pending
    * message in the TX channel. */

    CANFlushTxChannel(CanModule,CAN_TX_CHANNEL);
//    message->msgEID.DLC 	= sizeof(dataArray) / sizeof(dataArray[0]);
//    memcpy(&message->data, dataArray, sizeof ( message->data));
  }
}


static inline void CanSetChannel(CAN_MODULE CanModule, CAN_CHANNEL channel, UINT bufferSize, CanChannelTypes type)
{

  CANSetOperatingMode(CanModule, CAN_CONFIGURATION);
	while(CANGetOperatingMode(CanModule) != CAN_CONFIGURATION);

    if( type == TX )
    {
      CANConfigureChannelForTx(CanModule, channel, bufferSize, CAN_TX_RTR_DISABLED, CAN_LOW_MEDIUM_PRIORITY);
    }
    else if( type == RX)
    {
      CANConfigureChannelForRx(CanModule, channel, bufferSize, CAN_RX_FULL_RECEIVE);
    }

  CANSetOperatingMode(CanModule, CAN_NORMAL_OPERATION);
  while(CANGetOperatingMode(CanModule) != CAN_NORMAL_OPERATION);
}


static inline INT32 CanSetChannelMask(CAN_MODULE CanModule, CAN_CHANNEL channel, CAN_FILTER Filter, UINT32 SID, CAN_FILTER_MASK FilterMask, UINT32 maskbits)
{
  if(maskbits > 0x7FF || SID > 0x7FF)
  {
    return -1; //bad maskbits or sid, read doc of CANConfigureFilterMask or CANConfigureFilter in CAN.h
  }
  
  if (channel == CAN_TX_CHANNEL)
  {
    return -1;
  }

  CANSetOperatingMode(CanModule, CAN_CONFIGURATION);
  while(CANGetOperatingMode(CanModule) != CAN_CONFIGURATION);

  CANConfigureFilter      (CanModule, Filter, SID, CAN_SID);
  CANConfigureFilterMask  (CanModule, FilterMask, maskbits, CAN_SID, CAN_FILTER_MASK_IDE_TYPE);
  CANLinkFilterToChannel  (CanModule, Filter, FilterMask, channel);
  CANEnableFilter         (CanModule, Filter, TRUE);

  CANEnableChannelEvent(CanModule, channel, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
  CANEnableModuleEvent (CanModule, CAN_RX_EVENT, TRUE);

  CANSetOperatingMode(CanModule, CAN_NORMAL_OPERATION);
  while(CANGetOperatingMode(CanModule) != CAN_NORMAL_OPERATION);
  
  return 0;
}


static inline void CanConfigInterrupt(CAN_MODULE CanModule, INT_PRIORITY priority, INT_SUB_PRIORITY subpriority )
{
  CANSetOperatingMode(CanModule, CAN_CONFIGURATION);
	while(CANGetOperatingMode(CanModule) != CAN_CONFIGURATION);
  
  if(CanModule == CAN1)
  {
    INTEnable(INT_CAN1, INT_DISABLED);  
    INTSetVectorPriority(INT_CAN_1_VECTOR, priority);
    INTSetVectorSubPriority(INT_CAN_1_VECTOR, subpriority);
  }
  else if(CanModule == CAN2)
  {
    INTEnable(INT_CAN2, INT_DISABLED);  
    INTSetVectorPriority(INT_CAN_2_VECTOR, priority);
    INTSetVectorSubPriority(INT_CAN_2_VECTOR, subpriority);
  }
  
  CANSetOperatingMode(CanModule, CAN_NORMAL_OPERATION);
  while(CANGetOperatingMode(CanModule) != CAN_NORMAL_OPERATION);
}


static inline void CanEnableInterrupt(CAN_MODULE CanModule)
{
  CANSetOperatingMode(CanModule, CAN_CONFIGURATION);
	while(CANGetOperatingMode(CanModule) != CAN_CONFIGURATION);
  
  if(CanModule == CAN1)
  {
    INTEnable(INT_CAN1, INT_ENABLED);      
  }
  else if(CanModule == CAN2)
  {
    INTEnable(INT_CAN2, INT_ENABLED);      
  }
  
  CANSetOperatingMode(CanModule, CAN_NORMAL_OPERATION);
  while(CANGetOperatingMode(CanModule) != CAN_NORMAL_OPERATION);
}


static inline void CanDisableInterrupt(CAN_MODULE CanModule)
{
  CANSetOperatingMode(CanModule, CAN_CONFIGURATION);
	while(CANGetOperatingMode(CanModule) != CAN_CONFIGURATION);
  
  if(CanModule == CAN1)
  {
    INTEnable(INT_CAN1, INT_DISABLED);      
  }
  else if(CanModule == CAN2)
  {
    INTEnable(INT_CAN2, INT_DISABLED);      
  }
  
  CANSetOperatingMode(CanModule, CAN_NORMAL_OPERATION);
  while(CANGetOperatingMode(CanModule) != CAN_NORMAL_OPERATION);
}

#endif
