//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Interrupts.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file declares all interrupt subroutines used.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : All priorities must be set and verified by the developper. All
//           priorities and subpriorities used are there as example. Also, note
//           that interrupt can be defined as iplnauto, iplnsrs and iplnsoft,
//           where n can be from 0 to 7. 7 is the highest priority, while 1 is
//           the lowest. A priority of 0 will disable the interrupt.
//           Subpriorities can go from (lowest to highest) 0 to 3.
//           SRS mode uses the shadow registers, soft uses the software
//           registers and auto is an automatic assignment.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "..\headers\Interrupts.h"
#include "..\headers\Potentiometer.h"

volatile BOOL  oAdcReady    = 0
              ,oTimer3Ready = 0
              ,oAcqOngoing  = 0
              ;
volatile UINT16 adcValues[15];

extern volatile UINT16 nSamples;
extern volatile UINT16 cellVoltRaw[N_SAMPLES_PER_ADC_READ][N_UNITS_TOTAL];
extern volatile UINT32 cellVoltRawMean[N_UNITS_TOTAL];
volatile UINT32 cellVoltRawMeanTemp[N_UNITS_TOTAL];


/*******************************************************************************
 ***********************                               *************************
 ********************           TIMER INTERRUPTS          **********************
 ***********************                               *************************
 ******************************************************************************/

//=============================================
// Configure the Timer 1 interrupt handler
//=============================================
void __ISR(_TIMER_1_VECTOR, T1_INTERRUPT_PRIORITY) Timer1InterruptHandler(void)
{
  // Increment the number of overflows from this timer. Used primarily by Input Capture
  Timer.Var.nOverflows[0]++;
  
  LED2_TOGGLE();
  
  mT1ClearIntFlag();
}

//=============================================
// Configure the Timer 2 interrupt handler
//=============================================
void __ISR(_TIMER_2_VECTOR, T2_INTERRUPT_PRIORITY) Timer2InterruptHandler(void)
{
  // Increment the number of overflows from this timer. Used primarily by Input Capture
  Timer.Var.nOverflows[1]++;

  mT2ClearIntFlag();
}

//=============================================
// Configure the Timer 3 interrupt handler
//=============================================
volatile INT64 time_ns;
volatile BOOL oDbgToggle = 0;
void __ISR(_TIMER_3_VECTOR, T3_INTERRUPT_PRIORITY) Timer3InterruptHandler(void)
{
  oTimer3Ready = 1;

  // Increment the number of overflows from this timer. Used primarily by Input Capture
  Timer.Var.nOverflows[2]++;

  mT3ClearIntFlag();
}

//=============================================
// Configure the Timer 4 interrupt handler
//=============================================
void __ISR(_TIMER_4_VECTOR, T4_INTERRUPT_PRIORITY) Timer4InterruptHandler(void)
{
  // Increment the number of overflows from this timer. Used primarily by Input Capture
  Timer.Var.nOverflows[3]++;

  mT4ClearIntFlag();
  
  const UINT8 nToggles = 20;
  static toggleCount = 0;
  LED1_TOGGLE();
  if (++toggleCount == nToggles)
  {
    Timer.DisableInterrupt(TIMER_4);
  }
}

//=============================================
// Configure the Timer 5 interrupt handler
//=============================================
void __ISR(_TIMER_5_VECTOR, T5_INTERRUPT_PRIORITY) Timer5InterruptHandler(void)
{
  // Increment the number of overflows from this timer. Used primarily by Input Capture
  Timer.Var.nOverflows[4]++;

  mT5ClearIntFlag();
  
  const UINT8 nToggles = 20;
  static toggleCount = 0;
  LED2_TOGGLE();
  if (++toggleCount == nToggles)
  {
    Timer.DisableInterrupt(TIMER_5);
  }
}


/*******************************************************************************
 ***********************                               *************************
 ********************           UART INTERRUPTS           **********************
 ***********************                               *************************
 *******************************************************************************/


//=============================================
// Configure the UART 3 interrupt handler
//=============================================

void __ISR(_UART_3_VECTOR, U3_INTERRUPT_PRIORITY) Uart3InterruptHandler(void)
{
  UINT8  i
        ,iMax   // Read/write max 8 bytes/interrupt
        ,data   // used in UartFifoWrite/Read functions
        ;

	// TX interrupt handling
  //===========================================================

  if ( INTGetEnable ( INT_SOURCE_UART_TX(UART3) ) )               // If TX interrupts enabled
  {
    if ( INTGetFlag ( INT_SOURCE_UART_TX(UART3) ) )               // If TX interrupt occured
    {
      if ( UARTTransmitterIsReady(UART3) && !Uart.Var.uartTxFifo[0].bufEmpty )  // If TX buffer is ready to receive data and the user's TX buffer is not empty
      {
        if (Uart.Var.uartTxFifo[0].lineBuffer.length < 8)     // Write max 8 bytes/interrupt
        {
          iMax = Uart.Var.uartTxFifo[0].lineBuffer.length;
        }
        else
        {
          iMax = 8;
        }

        for (i = 0; i < iMax; i++)
        {
          UartFifoRead((void *) &Uart.Var.uartTxFifo[0], &data);  // Copy from user
          U3TXREG = data;                                         // Put data in PIC32's TX buffer
        }
      }

      if (Uart.Var.uartTxFifo[0].bufEmpty)                    // If User's TX buffer is empty
      {
        Uart.DisableTxInterrupts(UART3);                          // Disable TX interrupts
      }

      INTClearFlag(INT_SOURCE_UART_TX(UART3));                    // Clear the TX interrupt Flag
    }
  }
  //===========================================================
  

	// RX interrupt handling
  //===========================================================
  if ( INTGetEnable ( INT_SOURCE_UART_RX(UART3) ) )               // If RX interrupts enabled
  {
    if ( INTGetFlag ( INT_SOURCE_UART_RX(UART3) ) )               // If RX interrupt occured
    {
      i = 0;
      iMax = 8;                                                   // Read max 8 bytes/interrupt
      while (   UARTReceivedDataIsAvailable(UART3)                // While RX data available
            && !Uart.Var.uartRxFifo[0].bufFull                // and user's RX buffer not full
            && (i < iMax)                                         // and under 8 bytes read
            )
      { // while ^
        data = UARTGetDataByte(UART3);                            // Get data for PIC32's RX FIFO buffer and copy it to user (next line)
        if ( UartFifoWrite((void *) &Uart.Var.uartRxFifo[0], &data) < 0 ) // If copy to user did not work
        {
          break;                                                  // Exit while loop
        }
        i++;
      } // end while

      if (!Uart.Var.uartRxFifo[0].bufEmpty)                   // If there is data in the user's RX buffer
      {
        Uart.Var.oIsRxDataAvailable[0] = 1;                   // Set according flag
      }

      INTClearFlag (INT_SOURCE_UART_RX(UART3) );                  // Clear the RX interrupt Flag

    }
	}
  //===========================================================
}
//=============================================


//=============================================
// Configure the UART 6 interrupt handler
//=============================================

void __ISR(_UART_6_VECTOR, U6_INTERRUPT_PRIORITY) Uart6InterruptHandler(void)
{
  UINT8  i
        ,iMax   // Read/write max 8 bytes/interrupt
        ,data   // used in UartFifoWrite/Read functions
        ;

	// TX interrupt handling
  //===========================================================

  if ( INTGetEnable ( INT_SOURCE_UART_TX(UART6) ) )               // If TX interrupts enabled
  {
    if ( INTGetFlag ( INT_SOURCE_UART_TX(UART6) ) )               // If TX interrupt occured
    {
      if ( UARTTransmitterIsReady(UART6) && !Uart.Var.uartTxFifo[1].bufEmpty )  // If TX buffer is ready to receive data and the user's TX buffer is not empty
      {
        if (Uart.Var.uartTxFifo[1].lineBuffer.length < 8)     // Write max 8 bytes/interrupt
        {
          iMax = Uart.Var.uartTxFifo[1].lineBuffer.length;
        }
        else
        {
          iMax = 8;
        }

        for (i = 0; i < iMax; i++)
        {
          UartFifoRead((void *) &Uart.Var.uartTxFifo[1], &data);  // Copy from user
          U6TXREG = data;                                         // Put data in PIC32's TX buffer
        }
      }

      if (Uart.Var.uartTxFifo[1].bufEmpty)                    // If User's TX buffer is empty
      {
        Uart.DisableTxInterrupts(UART6);                          // Disable TX interrupts
      }

      INTClearFlag(INT_SOURCE_UART_TX(UART6));                    // Clear the TX interrupt Flag
    }
  }
  //===========================================================
  

	// RX interrupt handling
  //===========================================================
  if ( INTGetEnable ( INT_SOURCE_UART_RX(UART6) ) )               // If RX interrupts enabled
  {
    if ( INTGetFlag ( INT_SOURCE_UART_RX(UART6) ) )               // If RX interrupt occured
    {
      i = 0;
      iMax = 8;                                                   // Read max 8 bytes/interrupt
      while (   UARTReceivedDataIsAvailable(UART6)                // While RX data available
            && !Uart.Var.uartRxFifo[1].bufFull                // and user's RX buffer not full
            && (i < iMax)                                         // and under 8 bytes read
            )
      { // while ^
        data = UARTGetDataByte(UART6);                            // Get data for PIC32's RX FIFO buffer and copy it to user (next line)
        if ( UartFifoWrite((void *) &Uart.Var.uartRxFifo[1], &data) < 0 ) // If copy to user did not work
        {
          break;                                                  // Exit while loop
        }
        i++;
      } // end while

      if (!Uart.Var.uartRxFifo[1].bufEmpty)                   // If there is data in the user's RX buffer
      {
        Uart.Var.oIsRxDataAvailable[1] = 1;                   // Set according flag
      }

      INTClearFlag (INT_SOURCE_UART_RX(UART6) );                  // Clear the RX interrupt Flag

    }
	}
  //===========================================================
}
//=============================================


/*******************************************************************************
 ***********************                               *************************
 ********************           ADC INTERRUPT             **********************
 ***********************                               *************************
 *******************************************************************************/

//=============================================
// Configure the ADC interrupt handler
//=============================================
void __ISR(_ADC_VECTOR, ADC_INTERRUPT_PRIO) AdcInterruptHandler(void)
{
  const UINT8 memcpySize = N_UNITS_TOTAL * 4;
  
  oAcqOngoing = 1;
  
  cellVoltRaw[nSamples][ 0] = ReadADC10( 0);
  cellVoltRaw[nSamples][ 1] = ReadADC10( 1);
  cellVoltRaw[nSamples][ 2] = ReadADC10( 2);
  cellVoltRaw[nSamples][ 3] = ReadADC10( 3);
  cellVoltRaw[nSamples][ 4] = ReadADC10( 4);
  cellVoltRaw[nSamples][ 5] = ReadADC10( 5);
  cellVoltRaw[nSamples][ 6] = ReadADC10( 6);
  cellVoltRaw[nSamples][ 7] = ReadADC10( 7);
  cellVoltRaw[nSamples][ 8] = ReadADC10( 8);
  cellVoltRaw[nSamples][ 9] = ReadADC10( 9);
  cellVoltRaw[nSamples][10] = ReadADC10(10);
  cellVoltRaw[nSamples][11] = ReadADC10(11);
  cellVoltRaw[nSamples][12] = ReadADC10(12);
  cellVoltRaw[nSamples][13] = ReadADC10(13);
  cellVoltRaw[nSamples][14] = ReadADC10(14);
//  cellVoltRawMeanTemp[ 0] += ReadADC10( 0);
//  cellVoltRawMeanTemp[ 1] += ReadADC10( 1);
//  cellVoltRawMeanTemp[ 2] += ReadADC10( 2);
//  cellVoltRawMeanTemp[ 3] += ReadADC10( 3);
//  cellVoltRawMeanTemp[ 4] += ReadADC10( 4);
//  cellVoltRawMeanTemp[ 5] += ReadADC10( 5);
//  cellVoltRawMeanTemp[ 6] += ReadADC10( 6);
//  cellVoltRawMeanTemp[ 7] += ReadADC10( 7);
//  cellVoltRawMeanTemp[ 8] += ReadADC10( 8);
//  cellVoltRawMeanTemp[ 9] += ReadADC10( 9);
//  cellVoltRawMeanTemp[10] += ReadADC10(10);
//  cellVoltRawMeanTemp[11] += ReadADC10(11);
//  cellVoltRawMeanTemp[12] += ReadADC10(12);
//  cellVoltRawMeanTemp[13] += ReadADC10(13);
//  cellVoltRawMeanTemp[14] += ReadADC10(14);
  
  INTClearFlag(INT_AD1);    // Clear the ADC conversion done interrupt Flag
  
  if (++nSamples >= N_SAMPLES_PER_ADC_READ)
  {
//    DBG1_TOGGLE();
    nSamples = 0;
    oAdcReady = 1;
//    memcpy((void *) cellVoltRawMean, (void *) cellVoltRawMeanTemp, memcpySize);
//    memset((void *) cellVoltRawMeanTemp, 0, memcpySize);
  }
  oAcqOngoing = 0;
}
//=============================================


/*******************************************************************************
 ***********************                               *************************
 ********************           I2C INTERRUPTS            **********************
 ***********************                               *************************
 *******************************************************************************/

//=============================================
// Configure the I2C5 interrupt handler
//=============================================
void __ISR(_I2C_5_VECTOR, I2C5_INT_PRIORITY) I2c5InterruptHandler(void)
{
//  if (INTGetFlag(INT_I2C5S))  // Slave interrupt
//  {
//    INTClearFlag(INT_I2C5S);
//  }
  
//  if (INTGetFlag(INT_I2C5B))  //Bus Collision interrupt
//  {
//    INTClearFlag(INT_I2C5B);
//  }
  
  sI2cCmdBuffer_t masterData;   // Used for keeping track of the current state
  
  if (INTGetFlag(INT_I2C5M))  // Master interrupt
  {
    INTClearFlag(INT_I2C5M);

    if (I2c.Var.oReadDataInNextInterrupt[0])     // If a read was started last interrupt
    {
      masterData.data  = I2C5RCV;                   // Read from I2C buffer
      masterData.state = I2C_MASTER_RECEIVE_DATA;   // Dummy write
      I2cFifoWrite((void *) &I2c.Var.i2cUserFifo[0], &masterData);   // Copy to user
      I2c.Var.oRxDataAvailable[0] = 1;
      I2c.Var.oReadDataInNextInterrupt[0] = 0;
    }
    
    if (I2c.Var.oI2cWriteIsRunning[0])   // Writing procedure
    {
      I2cFifoRead((void *) &I2c.Var.i2cWriteQueue[0], &masterData);  // Get current state
      switch (masterData.state)
      {
      //======================================================  
        case I2C_MASTER_RECEIVE_DATA : 
          I2C5CONbits.RCEN = 1;                         // Receive byte sequence
          I2c.Var.oReadDataInNextInterrupt[0] = 1;   // Flag for the next interrupt to read the RX buffer
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_START_CONDITION : 
          I2C5CONbits.SEN = 1;                          // Start condition sequence
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_STOP_CONDITION : 
          I2C5CONbits.PEN = 1;                          // Stop condition sequence
          
          if (I2c.Var.oPoolSlaveAcknowledge[0])      // If the user wanted to poll
          {
            if (!I2c.Var.oSecondStopAfterPooling[0]) // If this is the first stop after writing all the data
            {                                           // Add the next three states required for polling
              masterData.state = I2C_MASTER_START_CONDITION;
              I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[0], &masterData);
              
              masterData.state = I2C_MASTER_TRANSMIT_DATA;
              I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[0], &masterData);
              
              masterData.state = I2C_MASTER_STOP_CONDITION;
              I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[0], &masterData);
              
              I2c.Var.oSecondStopAfterPooling[0] = 1;
            }
            else                                        // If the first stop has passed
            {
              if (!I2CByteWasAcknowledged(0))        // If slave sent NACK
              {                                         // Redo the 3 states for the polling
                masterData.state = I2C_MASTER_START_CONDITION;
                I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[0], &masterData);

                masterData.state = I2C_MASTER_TRANSMIT_DATA;
                I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[0], &masterData);

                masterData.state = I2C_MASTER_STOP_CONDITION;
                I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[0], &masterData);
              }
              else                                      // If slave sent ACK
              {                                         // End the communication with the slave
                masterData.state = I2C_MASTER_DONE;
                I2cFifoWrite((void *) &I2c.Var.i2cWriteQueue[0], &masterData);
                I2c.Var.oSecondStopAfterPooling[0] = 0;
              }
            }
          }
          break;
      //====================================================== 

      //======================================================    
        case I2C_MASTER_TRANSMIT_DATA : 
          I2C5TRN = masterData.data;      // Put data in TX buffer
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_DONE : 
          if (I2c.Var.i2cWriteQueue[0].bufEmpty)     // If there is nothing more to send
          {
            I2c.Var.oI2cWriteIsRunning[0] = 0;       // Turn off writing process
          }
          else                                          // If there is still data in the WriteQueue
          {
            INTSetFlag(INT_I2C5M);                      // Start another writing process
          }
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_REPEAT_START : 
          I2C5CONbits.RSEN = 1;   //repeated start condition sequence
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_SLAVE_SENT_STOP :   // Not used
//          LED_ERROR_ON;
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_SEND_ACK : 
          I2C5CONbits.ACKDT = 0;  //ACK
          I2C5CONbits.ACKEN = 1;  //Send ACK sequence
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_SEND_NACK : 
          I2C5CONbits.ACKDT = 1;  //NACK
          I2C5CONbits.ACKEN = 1;  //Send NACK sequence
          break;
      //====================================================== 

      //======================================================  
        case I2C_CMD_ERROR :      // Not used
//          LED_ERROR_ON;
          break;
      //====================================================== 

      //======================================================  
        default : 
          break;
      //======================================================  
      } // end switch (master.state)
    } // end if (writing sequence)
    
    

    if (I2c.Var.oI2cReadIsRunning[0])    // Reading procedure
    {
      I2cFifoRead((void *) &I2c.Var.i2cReadQueue[0], &masterData);   // Get current state
      switch (masterData.state)
      {
      //======================================================  
        case I2C_MASTER_RECEIVE_DATA : 
          I2C5CONbits.RCEN = 1;             // Receive byte sequence
          I2c.Var.oReadDataInNextInterrupt[0] = 1;
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_START_CONDITION : 
          I2C5CONbits.SEN = 1;              // Start condition sequence
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_REPEAT_START : 
          I2C5CONbits.RSEN = 1;             // Repeated start condition sequence
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_STOP_CONDITION : 
          I2C5CONbits.PEN = 1;              // Stop condition
          break;
      //====================================================== 

      //======================================================    
        case I2C_MASTER_TRANSMIT_DATA : 
          I2C5TRN = masterData.data;        // Put data in TX buffer
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_DONE : 
          if (I2c.Var.i2cReadQueue[0].bufEmpty)    // If there is nothing more to send
          {
            I2c.Var.oI2cReadIsRunning[0] = 0;      // Turn off reading process
          }
          else                                        // If there is still data in the ReadQueue
          {
            INTSetFlag(INT_I2C5M);                    // Start another reading process
          }
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_SLAVE_SENT_STOP :   // Not used
//          LED_ERROR_ON;
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_SEND_ACK : 
          I2C5CONbits.ACKDT = 0;  // ACK
          I2C5CONbits.ACKEN = 1;  // Send ACK sequence
          break;
      //====================================================== 

      //======================================================  
        case I2C_MASTER_SEND_NACK : 
          I2C5CONbits.ACKDT = 1;  // NACK
          I2C5CONbits.ACKEN = 1;  // Send NACK sequence
          break;
      //====================================================== 

      //======================================================  
        case I2C_CMD_ERROR :      // Not used
//          LED_ERROR_ON;
          break;
      //======================================================

      //======================================================  
        default : 
          break;
      //======================================================  
      } // end switch (master.state)
    } // end if (read sequence)
  }  // end if (master interrupt)
}   // end interrupt
//=============================================
