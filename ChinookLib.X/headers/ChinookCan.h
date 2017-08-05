/* UNO32 DOESN'T HAVE CAN */


/**********************************************************************
* Â© 2007 Microchip Technology Inc.
*
* FileName:        CANFunctions.h
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC32
* Compiler:        MPLABÂ® C32
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and
* intellectual property rights in the code accompanying this message and in all
* derivatives hereto.  You may use this code, and any derivatives created by
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY),
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL,
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and
* determining its suitability.  Microchip has no obligation to modify, test,
* certify, or support the code.
*******************************************************************
* * * This file has been modified for use by Club Chinook ETS * * *
*******************************************************************
***********************************************************************/

#ifndef __CHINOOK_CAN__
#define __CHINOOK_CAN__

#ifndef __32MX320F128H__    // Uno32 doesn't have CAN

#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include "plib.h"
#include <xc.h>
#include "ChinookHardwareProfile.h"

#define CAN_TX_CHANNEL CAN_CHANNEL0

#define CAN_BUS_SPEED 250000
//#define CANFIFOSIZE (2 * 8 * CAN_TX_RX_MESSAGE_SIZE_BYTES)

#define CAN1_BRPVAL	0xF			/* CAN speed select - 0x7 for 500kbps, 0xF for 250Kbps, 0x3 for 1Mbps 		*/
#define CAN2_BRPVAL 0xF			/* Use same speed for both CAN modules. Time quanta per bit is set to 10.	*/
								/* See function Can1Init() and Can2Init().*/
typedef enum {TX, RX} CanChannelTypes;

//BYTE Can1MessageFifoArea[CANFIFOSIZE];	/* This is the CAN1 FIFO message area.	*/
// Can2MessageFifoArea[CANFIFOSIZE];	/* This is the CAN2 FIFO message area.	*/

struct sChinookCan
{
  /**
   * @Prototype   static inline void CanInit(CAN_MODULE MODULE_TO_SETUP, BOOL isLoopbackMode)
   *
   * @Description This function initializes specified CAN module. It sets up speed, FIFOs,
   *              filters and interrupts. FIFO0 is set up for TX with 8 message
   *              buffers. FIFO1 is set up for RX with 8 message buffers. Filter 0
   *              is set with Mask 0 for SID 0xCA01 for CAN module 1 and SID 0xCA02 for CAN module 2. Only RXNEMPTY interrupt and
   *              RBIF interrupt is enabled.
   *
   * @Example     BYTE NbChannels = 4; // 4 CAN channels will be used\n
   *              BYTE BufferSize = 8; // 8 buffers per channel will be used\n
   *              BYTE CanMessageFifoArea [ NbChannels * BufferSize * CAN_TX_RX_MESSAGE_SIZE_BYTES ]; \n
   *              Can.Initialize(CAN1, CanMessageFifoArea, NbChannels, BufferSize, FALSE); \n
   *              This example initializes The CAN1 module in normal mode to have 4 channels with a\n
   *              buffer size of 8.
   *
   * @Param       CAN_MODULE CanModule\n
   *              CAN module to setup: CAN 1 or CAN2
   *
   * @Param       BYTE *CanMessageFifoArea\n
   *              Memory buffer for CAN Rx/Tx messages
   *
   * @Param       BYTE NbChannels\n
   *              Number of channels used on the CAN module
   *
   * @Param       Byte BufferSize\n
   *              Size of each channel buffer
   *
   * @Param       BOOL LOOPBACK\n
   *              Setup CAN module in loopback mode
   *
   *
   */
   INT32   (*Initialize)       (CAN_MODULE CanModule, BYTE *CanMessageFifoArea, BYTE NbChannels, BYTE BufferSize, BOOL isLoopbackMode);


  /**
   * @Prototype   static inline void * CanRxMsgProcess(CAN_MODULE CanModule, BOOL is LoopbackMode)
   *
   * @Description This function checks if a message is available to be read in
	 *              specified CAN FIFO1. If a message is available, the function will check
	 *              byte 0 (boolean flag) of the CAN message payload.
   *
   * @Example     UINT32 message = Can.ProcessMessage(CAN1, TRUE);\n
   *              Recieve message in CAN1, in loopback mode
   *
   * @Param       CAN_MODULE CanModule\n
   *              CAN module: CAN 1 or CAN2
   *
   * @Returns     Message buffer
   */
  void *  (*ProcessMessage) (CAN_MODULE CanModule);

  /**
   * @Prototype   static inline void CanSendData(CAN_MODULE CanModule, UINT8 messageSID, UINT64 data);
   *
   * @Description This function will send a CAN message with provided SID. In the payload,
	 *              it sends a 64 bits message. FIFO0 is used for sending the message.
   *
   * @Example     Use Can.SendByteArray instead
   *
   * @Param       CAN_MODULE CanModule\n
   *              CAN module: CAN 1 or CAN2
   *
   * @Param       UINT8 messageSID\n
   *              Desired SID
   *
   * @Param       UINT64 data\n
   *              Data to send
   *
   */
   void   (*SendData)    (CAN_MODULE CanModule, UINT8 messageSID, UINT64 data);

  /**
   * @Prototype   static inline void CanTxSendFloat(CAN_MODULE CanModule, UINT8 messageSID, float data);
   *
   * @Description This function will send a float by CAN message with provided SID. In the payload,
	 *              it sends a 32 bits message. FIFO0 is used for sending the message.
   *
   * @Example     Can.TxSendFloat(CAN1, 0x55, mastAngle);
   *
   * @Param       CAN_MODULE CanModule\n
   *              CAN module: CAN 1 or CAN2
   *
   * @Param       UINT8 messageSID\n
   *              Desired SID
   *
   * @Param       float data\n
   *              Data to send. Must be a float
   *
   */
   void   (*SendFloat)   (CAN_MODULE CanModule, UINT8 messageSID, float data);

   /**
   * @Prototype   static inline void CanSendByte(CAN_MODULE CanModule, UINT8 messageSID, UINT8 data);
   *
   * @Description This function will send a CAN message with provided SID. In the payload,
	 *              it sends one byte. FIFO0 is used for sending the message.
   *
   * @Example     Can.SendByte(CAN1, 0x40, 0x30)
   *
   * @Param       CAN_MODULE CanModule\n
   *              CAN module: CAN 1 or CAN2
   *
   * @Param       UINT8 messageSID\n
   *              Desired SID between 1 and 255
   *
   * @Param       UINT8 data\n
   *              Byte to send
   *
   */
   void   (*SendByte)         (CAN_MODULE CanModule, UINT8 messageSID, UINT8 data);
   
   /**
   * @Prototype   static inline void CanSendByteArray(CAN_MODULE CanModule, UINT8 messageSID, BYTE *dataArray, BYTE arraySize);
   *
   * @Description This function will send a CAN message with provided SID.
   *
   * @Example     BYTE values[8] = { 1,2,3,4,5,6,7,8 };\n
   *              Can.SendByteArray(CAN1, 0x40, value, 8);
   *
   * @Param       CAN_MODULE CanModule\n
   *              CAN module: CAN 1 or CAN2
   *
   * @Param       UINT8 messageSID\n
   *              Desired SID between 1 and 255
   *
   * @Param       BYTE *dataArray\n
   *              Pointer to the array of bytes
   * 
   * @Param       Byte arraySize\n
   *              Size of the byte array between 1 and 8
   *
   */
   void   (*SendByteArray)    (CAN_MODULE CanModule, UINT8 messageSID, BYTE *dataArray, BYTE arraySize);

   
   /**
   * @Prototype   static inline void CanSetChannel(CAN_MODULE CanModule, CAN_CHANNEL channel, UINT bufferSize, CanChannelTypes type);
   *
   * @Description This function will configure a channel for transmission of data (TX)\n
   *              or reception of data (RX).
   *
   * @Example     Can.SetChannel(CAN1, CAN_CHANNEL8, 8, RX);\n
   *              This sets the channel 8 to be an RX channel with a buffer of 8
   *
   * @Param       CAN_MODULE CanModule\n
   *              CAN module: CAN 1 or CAN2
   *
   * @Param       CAN_CHANNEL channel\n
   *              Desired channel, values between CAN_CHANNEL1 and CAN_CHANNEL31. Use CAN_CHANNEL0 for TX.
   *
   * @Param       UINT bufferSize\n
   *              Buffer size for the channel, between 1 and 32 (8 should be fine)
   * 
   * @Param       CanChannelTypes type\n
   *              Type od channel values are RX or TX
   *
   */
   void   (*SetChannel)       (CAN_MODULE CanModule, CAN_CHANNEL channel, UINT bufferSize, CanChannelTypes type);

   /**
   * @Prototype   static inline void CanSetChannelMask(CAN_MODULE CanModule, CAN_CHANNEL channel, CAN_FILTER Filter, UINT32 SID, CAN_FILTER_MASK FilterMask, UINT32 maskbits);
   *
   * @Description This function will configure a channel filter and filter mask;
   *
   * @Example     Can.SetChannel(CAN1, CAN_CHANNEL8, CAN_FILTER0, 0x40, CAN_FILTER_MASK0, 0x7FF);\n
   *              This sets the channel 8 to be associated with CAN_FILTER0,\n
   *              the CAN_FILTER0 to be associated with CAN_FILTER_MASK0,\n
   *              the CAN_FILTER_MASK0 to have a value of 0x7FF.\
   *              This means CAN_CHANNEL8 (RX channel) will accept only CAN messages with SID 0x40.   
   *
   * @Param       CAN_MODULE CanModule\n
   *              CAN module: CAN 1 or CAN2
   *
   * @Param       CAN_CHANNEL channel\n
   *              Desired channel, values between CAN_CHANNEL1 and CAN_CHANNEL31. Use CAN_CHANNEL0 for TX.
   *
   * @Param       CAN_FILTER Filter\n
   *              Filter desired, between CAN_FILTER1 and CAN_FILTER32
   * 
   * @Param       UINT32 SID\n
   *              Desired SID to be associated with the channel.
   * 
   * @Param       CAN_FILTER_MASK FilterMask\n
   *              Desired filter mask, between CAN_FILTER_MASK0 and CAN_FILTER_MASK3\n
   *              For Chinook, you should use 1 filter mask for all your channels and\n
   *              set its value (maskbits) to 0x7FF.
   * 
   * @Param       UINT32 maskbits\n
   *              maskbits to be associated with filter mask. (should be 0x7FF) 
   * 
   * @Return      0 on success, -1 on failure.\n
   *              Reasons of failure : \n
   *              maskbits > 0x7FF\n
   *              SID > 0x7FF\n
   *              Channel is the TX channel (constant CAN_TX_CHANNEL)
   *
   */
   INT32 (*SetChannelMask) (CAN_MODULE CanModule, CAN_CHANNEL channel, CAN_FILTER Filter, UINT32 SID, CAN_FILTER_MASK FilterMask, UINT32 maskbits);
   

  /**
   * @Prototype   static inline void CanConfigInterrupt (CAN_MODULE CanModule, INT_PRIORITY priority, INT_SUB_PRIORITY subpriority);
   *
   * @Description This function configures the interrupt priority and subpriority
   *              for the CAN module passed in argument. It DOES NOT enable the 
   *              CAN interrupts. Use Can.EnableInterrupt().
   *
   * @Example     Can.ConfigInterrupt(CAN1, 4, 0);\n
   *              This example sets the priority of the CAN1 interrupt to 4 and 
   *              its subpriority to 0.
   *
   * @Param       CAN_MODULE CanModule\n
   *              CAN module: CAN 1 or CAN2
   *
   * @Parameter   INT_PRIORITY priority\n
   *              priority is the desired priority of the interrupt to set. Its values have
   *              a range from 0 to 7, seven (7) being the highest priority and
   *              one (1) being the lowest priority. Note that a priority of zero (0)
   *              will disable the interrupt.
   *
   * @Parameter   INT_SUB_PRIORITY subPriority\n
   *              subPriority is the desired subpriority of the interrupt to set.
   *              Its values have a range from 0 to 3, three (3) being the highest
   *              priority and zero (0) being the lowest priority.
   *
   * @Returns     None.
   *
   */
   void (*ConfigInterrupt) (CAN_MODULE CanModule, INT_PRIORITY priority, INT_SUB_PRIORITY subpriority );
   

  /**
   * @Prototype   static inline void CanDisableInterrupt (CAN_MODULE CanModule);
   *
   * @Description This function disables the interrupts for the CAN module passed
   *              in argument.
   *
   * @Example     Can.DisableInterrupt(CAN2);\n
   *              This example disables the interrupts for the CAN 2.
   *
   * @Param       CAN_MODULE CanModule\n
   *              CAN module: CAN 1 or CAN2
   *
   * @Returns     None.
   *
   */
   void (*DisableInterrupt) (CAN_MODULE CanModule);
   

  /**
   * @Prototype   static inline void CanEnableInterrupt (CAN_MODULE CanModule);
   *
   * @Description This function enables the interrupts for the CAN module passed
   *              in argument.
   *
   * @Example     Can.EnableInterrupt(CAN2);\n
   *              This example enables the interrupts for the CAN 2.
   *
   * @Param       CAN_MODULE CanModule\n
   *              CAN module: CAN 1 or CAN2
   *
   * @Returns     None.
   *
   */   
   void (*EnableInterrupt) (CAN_MODULE CanModule);
};

#endif  /* not defined Uno32 */
#endif	/* __CHINOOK_CAN__ */
