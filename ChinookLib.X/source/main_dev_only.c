/* 
 * File:   main.c
 * Author: Frederic
 *
 * Created on 15 octobre 2014, 17:28
 */

#include "..\headers\ChinookLib.h"

#include <xc.h>

// DEVCFG3
// USERID = No Setting
#ifndef __32MX320F128H__    // Uno32 doesn't have these settings
#pragma config FSRSSEL    = PRIORITY_7  // SRS Select (SRS Priority 7)  (SRS = Shadow Registers)
#pragma config FMIIEN     = ON          // Ethernet RMII/MII Enable (MII Enabled)
#pragma config FETHIO     = ON          // Ethernet I/O Pin Select (Default Ethernet I/O)
#pragma config FCANIO     = ON          // CAN I/O Pin Select (Default CAN I/O)
#pragma config FUSBIDIO   = ON          // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO  = ON          // USB VBUS ON Selection (Controlled by USB Module)
#endif

// DEVCFG2
#pragma config FPLLIDIV   = DIV_2       // PLL Input Divider (2x Divider)
#pragma config FPLLMUL    = MUL_20      // PLL Multiplier (20x Multiplier)

#ifndef __32MX320F128H__    // Uno32 doesn't have these settings
#pragma config UPLLIDIV   = DIV_12      // USB PLL Input Divider (12x Divider)
#pragma config UPLLEN     = OFF         // USB PLL Enable (Disabled and Bypassed)
#endif

#pragma config FPLLODIV   = DIV_1       // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
#if defined __32MX795F512L__ || __32MX320F128H__  // Chipkit MAX32 or Uno32
#pragma config FNOSC      = PRIPLL                // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#elif defined __32MX795F512H__                    // Chinook boards
#pragma config FNOSC = FRCPLL                     // Oscillator Selection Bits (Fast RC Osc with PLL)
#endif

#pragma config FSOSCEN    = OFF                   // Secondary Oscillator Enable (Disabled)
#pragma config IESO       = ON                    // Internal/External Switch Over (Enabled)

#if defined __32MX795F512L__ || __32MX320F128H__  // Chipkit MAX32 or Uno32
#pragma config POSCMOD    = HS                    // Primary Oscillator Configuration (HS osc mode)
#elif defined __32MX795F512H__                    // Chinook boards
#pragma config POSCMOD = OFF                      // Primary Oscillator Configuration (Primary osc disabled)
#endif

#pragma config OSCIOFNC   = ON                    // CLKO Output Signal Active on the OSCO Pin (Enabled)
#pragma config FPBDIV     = DIV_8                 // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/8)
#pragma config FCKSM      = CSDCMD                // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS      = PS1048576             // Watchdog Timer Postscaler (1:1048576)
#pragma config FWDTEN     = OFF                   // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))

// DEVCFG0
#pragma config DEBUG      = OFF         // Background Debugger Enable (Debugger is disabled)
#pragma config ICESEL     = ICS_PGx2    // ICE/ICD Comm Channel Select (ICE EMUC2/EMUD2 pins shared with PGC2/PGD2)
#pragma config PWP        = OFF         // Program Flash Write Protect (Disable)
#pragma config BWP        = OFF         // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP         = OFF         // Code Protect (Protection Disabled)

/*
 * 
 */

volatile BOOL  oSpiSend     = 0
              ,oUartSend    = 0
              ,oCapture     = 0
              ,oTimer2Event = 0
              ;

volatile UINT16 CaptureTime = 0;
volatile UINT16 previousCaptureTime = 0;

//sSkadi_t skadi;
void static Led1();
void static Led2();


int main (int argc, char** argv)
{
//==============================================================================
// Following memcopies are used to be able to use the form
// Chinook.LibraryX.FunctionX in case the developper wants to see the available
// functions.
// IMPORTANT NOTE : the variables structures (example : Adc.Var...) WON'T be
//                  updated. Only use Chinook.Lib.Function format for the
//                  functions.
//==============================================================================
  struct sChinook Chinook;
  memcpy( &Chinook.Adc   , &Adc   , sizeof ( struct sChinookAdc   ) );
#ifndef __32MX320F128H__    // Uno32 doesn't CAN
  memcpy( &Chinook.Can   , &Can   , sizeof ( struct sChinookCan   ) );
#endif
  memcpy( &Chinook.I2c   , &I2c   , sizeof ( struct sChinookI2c   ) );
  memcpy( &Chinook.InputCapture , &InputCapture , sizeof ( struct sChinookInputCapture ) );
  memcpy( &Chinook.Port  , &Port  , sizeof ( struct sChinookPort  ) );
  memcpy( &Chinook.Pwm   , &Pwm   , sizeof ( struct sChinookPwm   ) );
  memcpy( &Chinook.Spi   , &Spi   , sizeof ( struct sChinookSpi   ) );
  memcpy( &Chinook.Timer , &Timer , sizeof ( struct sChinookTimer ) );
  memcpy( &Chinook.Uart  , &Uart  , sizeof ( struct sChinookUart  ) );
  memcpy( &Chinook.Skadi , &Skadi , sizeof ( struct sChinookSkadi ) );
  memcpy( &Chinook.Wdt   , &Wdt   , sizeof ( struct sChinookWdt   ) );
//==============================================================================


//==============================================================================
// The next line disables the JTAG for the PIC. If the JTAG is enabled, pins
// RB10-13 can't be used as I/Os. If you want to use the JTAG, comment or
// remove this line.
//==============================================================================
  DDPCONbits.JTAGEN = 0;
//==============================================================================


// Configure the device for maximum performance but do not change the PBDIV
// Given the options, this function will change the flash wait states, RAM
// wait state and enable prefetch cache but will not change the PBDIV.
// The PBDIV value is already set via the pragma FPBDIV option in HardwareProfile.h.
//==============================================================================
  SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
//==============================================================================

  INTDisableInterrupts();

  INT8 err;
//  err = Timer.Open(TIMER_3, 412, SCALE_US);
//  if (err < 0)
//  {
//    Port.C.ToggleBits(BIT_1);
//  }
//  Timer.ConfigInterrupt(TIMER_3, 3, 3);
//  Timer.EnableInterrupt(TIMER_3);

//  err = Timer.Open(TIMER_2, 500, SCALE_MS);
//  if (err < 0)
//  {
//    Port.A.SetBits(BIT_3);
//  }
//  Timer.ConfigInterrupt(TIMER_2, 4, 2);
//  Timer.EnableInterrupt(TIMER_2);

#ifdef __32MX795F512H__
  Port.B.SetPinsDigitalOut(BIT_13 | BIT_12);    // RB12 = DEBUG4 led, RB13 = CAN led
  Port.B.SetBits(BIT_13 | BIT_12);
  Port.C.SetPinsDigitalOut(BIT_14);
  Port.C.ClearBits(BIT_14);
#elif defined __32MX795F512L__
  Port.A.SetPinsDigitalOut(BIT_3);
  Port.A.ClearBits(BIT_3);
  Port.C.SetPinsDigitalOut(BIT_1);
  Port.C.ClearBits(BIT_1);
#define LED4_TOGGLE Port.A.ToggleBits(BIT_3)
#define LED5_TOGGLE Port.C.ToggleBits(BIT_1)
#define LED4_ON Port.A.SetBits(BIT_3)
#define LED5_ON Port.C.SetBits(BIT_1)
#define LED4_OFF Port.A.ClearBits(BIT_3)
#define LED5_OFF Port.C.ClearBits(BIT_1)
#endif

  
  // UART DEBUG

#ifdef __32MX795F512H__
  UartModule_t uartModule = UART6;
#elif defined __32MX795F512L__ || __32MX320F128H__
  UartModule_t uartModule = UART1;
#endif

  UartConfig_t oConfig = UART_ENABLE_PINS_TX_RX_ONLY;
  UartFifoMode_t oFifoMode = UART_INTERRUPT_ON_TX_BUFFER_EMPTY | UART_INTERRUPT_ON_RX_NOT_EMPTY;
  UartLineCtrlMode_t oLineControl = UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1;

  Uart.Open(uartModule, BAUD9600, oConfig, oFifoMode, oLineControl);

  Uart.EnableRx(uartModule);
  Uart.EnableTx(uartModule);
  Uart.ConfigInterrupt(uartModule, 6, 3);
  Uart.EnableRxInterrupts(uartModule);
  Uart.DisableTxInterrupts(uartModule);

  sUartLineBuffer_t buffer =
  {
     .buffer = {0}
    ,.length =  0
  };
  

  // SPI DEBUG

  
//  character = 0xDEADBEEF;
//  
//  for (i = 0; i < Spi.Var.spiRxFifo[SPI2].maxBufSize; i++)
//  {
//    SpiFifoWrite((void *) &Spi.Var.spiRxFifo[SPI2], &character);
//  }
//  
//  character = 0;
//  
//  for (i = 0; i < Spi.Var.spiRxFifo[SPI2].maxBufSize; i++)
//  {
//    SpiFifoRead ((void *) &Spi.Var.spiRxFifo[SPI2], &character);
//    if (character != 0xEF)
//    {
//      LED5_ON;
//    }
//  }
//  
//  
//  UINT32 *pAllo = Spi.Var.spiRxFifo[SPI2].lineBuffer.buffer;
//  UINT32 allo = *pAllo;
//  UINT32 allo = *((UINT32 *) Spi.Var.spiRxFifo[SPI2].lineBuffer.buffer);
//  UINT32 allo = sizeof(UINT32);
  
//  int testicule = SPI2BUF;
  
  SpiOpenFlags_t  oSpiMasterFlags
                 ,oSpiSlaveFlags
                 ;
//  oSpiMasterFlags = SPI_MASTER_MODE | SPI_32_BITS_CHAR;
//  oSpiSlaveFlags  = SPI_SLAVE_MODE  | SPI_32_BITS_CHAR;
  oSpiMasterFlags = SPI_MASTER_MODE | SPI_32_BITS_CHAR | SPI_ENHANCED_BUFFER_MODE | SPI_TX_EVENT_BUFFER_SR_EMPTY | SPI_RX_EVENT_BUFFER_NOT_EMPTY | SPI_SAMPLE_END_CLK;
//  oSpiMasterFlags = SPI_MASTER_MODE | SPI_32_BITS_CHAR | SPI_ENHANCED_BUFFER_MODE | SPI_TX_EVENT_BUFFER_SR_EMPTY | SPI_RX_EVENT_BUFFER_NOT_EMPTY;
  oSpiSlaveFlags  = SPI_SLAVE_MODE  | SPI_32_BITS_CHAR | SPI_ENHANCED_BUFFER_MODE | SPI_TX_EVENT_BUFFER_SR_EMPTY | SPI_RX_EVENT_BUFFER_NOT_EMPTY;
  
  Spi.Open(SPI2, oSpiMasterFlags, 5000000);
//  Spi.Open(SPI2, oSpiSlaveFlags , 5000000);
  
//  Spi.ConfigInterrupt    (SPI2, 4, 2);
//  Spi.EnableRxInterrupts (SPI2);
//  Spi.DisableTxInterrupts(SPI2);
  
  UINT32 character, i;
  UINT8 alloString[] = "\n\rBRAVO CA MARCHE\n\r\0";

  INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
  INTEnableInterrupts();
  
//  SpiEnhancedBuffer_t spiBuffer = {0};
//  sSpiLineBuffer_t pBuffer;
//  pBuffer.buffer = (void *) &spiBuffer.fifo32Bits;
//  pBuffer.length = 0;
  
  while(1)
  {
//        // Master with interrupts
//    if (Uart.Var.oIsRxDataAvailable[UART1])
//    {
//      Uart.GetRxFifoBuffer(UART1, &buffer, TRUE);
//      buffer.length = 0;
//      if (buffer.buffer[0] == 'a')
//      {
//        spiBuffer.fifo32Bits[0] = 0xDEADBEEF;
//        pBuffer.length++;
//        Spi.PutTxFifoBuffer(SPI2, (void *) &pBuffer);
//        LED5_TOGGLE;
//      }
//    }
//    if (Spi.Var.oIsRxDataAvailable[SPI2])
//    {
//      pBuffer.length = 0;
//      spiBuffer.fifo32Bits[0] = 0;
////      character = Spi.GetCharacter(SPI2);
////      character = SpiChnGetC(SPI_CHANNEL2);
//      Spi.GetRxFifoBuffer(SPI2, (void *) &pBuffer, FALSE);
//      character = spiBuffer.fifo32Bits[0];
//    
//      if (character == 0xDEADBEEF)
//      {
//        memcpy(&buffer.buffer[0], &alloString[0], sizeof(alloString));
//        buffer.length = sizeof(alloString);
//        Uart.PutTxFifoBuffer(UART1, &buffer);
//        LED4_TOGGLE;
//      }
//    }
    
    
    
    // Master without interrupts
    if (Uart.Var.oIsRxDataAvailable[UART1])
    {
      Uart.GetRxFifoBuffer(UART1, &buffer, TRUE);
      buffer.length = 0;
      if (buffer.buffer[0] == 'a')
      {
        while(Spi.IsSpiBusy(SPI2));
//        Spi.SendCharacter(SPI2, 0xDEADBEEF);
        SPI2BUF = 0xDEADBEEF;
        LED5_TOGGLE;
      }
    }
    if (!SpiChnRxBuffEmpty(SPI2 + 1))
//    if (!SpiChnRxBuffEmpty(SPI_CHANNEL2))
    {
//      character = Spi.GetCharacter(SPI2);
      character = SPI2BUF;ReadSPI2();
//      character = SpiChnGetC(SPI_CHANNEL2);
    
      if (character == 0xDEADBEEF)
      {
        memcpy(&buffer.buffer[0], &alloString[0], sizeof(alloString));
        buffer.length = sizeof(alloString);
        Uart.PutTxFifoBuffer(UART1, &buffer);
        LED4_TOGGLE;
      }
    }
    
    
//    // Slave
//    while (!SpiChnRxBuffEmpty(SPI2 + 1))
////    while (!SpiChnRxBuffEmpty(SPI_CHANNEL2))
//    {
//      character = Spi.GetCharacter(SPI2);
////      character = SpiChnGetC(SPI_CHANNEL2);
//    
////      if (character == 0xDEADBEEF)
////      {
//        while(Spi.IsSpiBusy(SPI2));
////        while(SpiChnIsBusy(SPI_CHANNEL2));
////        Spi.SendCharacter(SPI2, 0xDEADBEEF);
//        Spi.SendCharacter(SPI2, character);
////        SpiChnPutC(SPI_CHANNEL2, 0xDEADBEEF);
//        LED4_TOGGLE;
////      }
//    }
    
    
//    // UART test
//    if (Uart.Var.oIsRxDataAvailable[UART1])
//    {
//      Uart.GetRxFifoBuffer(UART1, &buffer, TRUE);
//      buffer.length = 0;
//      if (buffer.buffer[0] == 'a')
//      {
//        LED5_TOGGLE;
//      }
//    }
  }
}




//=============================================
// Configure the SPI 2 interrupt handler
//=============================================
void __ISR(_SPI_2_VECTOR, ipl4auto) Spi2InterruptHandler(void)
{
  
  UINT32  i
         ,iMax   // Read/write max 8 bytes/interrupt
         ,data   // used in FifoWrite/Read functions
         ;

	// TX interrupt handling
  //===========================================================

  if ( INTGetEnable ( INT_SOURCE_SPI_TX(SPI2 + 1) ) )               // If TX interrupts enabled
  {
    if ( INTGetFlag ( INT_SOURCE_SPI_TX(SPI2 + 1) ) )               // If TX interrupt occured
    {
      if ( !SpiChnTxBuffFull(SPI2 + 1) && !Spi.Var.spiTxFifo[SPI2].bufEmpty )  // If TX buffer is ready to receive data and the user's TX buffer is not empty
      {
        if (Spi.Var.spiTxFifo[SPI2].lineBuffer.length < Spi.Var.spiTxFifo[SPI2].maxWordsPerInt)     // Write max 4/8/16 bytes/interrupt
        {
          iMax = Spi.Var.spiTxFifo[SPI2].lineBuffer.length;
        }
        else
        {
          iMax = Spi.Var.spiTxFifo[SPI2].maxWordsPerInt;
        }

        for (i = 0; i < iMax; i++)
        {
          SpiFifoRead((void *) &Spi.Var.spiTxFifo[SPI2], &data);  // Copy from user
          SPI2BUF = data;                                         // Put data in PIC32's TX buffer
        }
      }

      if (Spi.Var.spiTxFifo[SPI2].bufEmpty)                    // If User's TX buffer is empty
      {
        Spi.DisableTxInterrupts(SPI2);                          // Disable TX interrupts
      }

//      LED4_TOGGLE;

      INTClearFlag(INT_SOURCE_SPI_TX(SPI2 + 1));                    // Clear the TX interrupt Flag
    }
  }
  //===========================================================
  

	// RX interrupt handling
  //===========================================================
  if ( INTGetEnable ( INT_SOURCE_SPI_RX(SPI2 + 1) ) )               // If RX interrupts enabled
  {
    if ( INTGetFlag ( INT_SOURCE_SPI_RX(SPI2 + 1) ) )               // If RX interrupt occured
    {
      i = 0;
      iMax = Spi.Var.spiRxFifo[SPI2].maxWordsPerInt;            // Read max 4/8/16 words/interrupt
      while (  !SpiChnRxBuffEmpty(SPI2 + 1)                     // While RX data available
            && !Spi.Var.spiRxFifo[SPI2].bufFull                 // and user's RX buffer not full
            && (i < iMax)                                       // and under 4/8/16 words read
            )
      { // while ^
        data = SPI2BUF;
        if ( SpiFifoWrite((void *) &Spi.Var.spiRxFifo[SPI2], &data) < 0 ) // If copy to user did not work
        {
          break;                                                  // Exit while loop
        }
        i++;
      } // end while

      if (!Spi.Var.spiRxFifo[SPI2].bufEmpty)                   // If there is data in the user's RX buffer
      {
        Spi.Var.oIsRxDataAvailable[SPI2] = 1;                   // Set according flag
      }

//      LED5_TOGGLE;

      INTClearFlag (INT_SOURCE_SPI_RX(SPI2 + 1) );                  // Clear the RX interrupt Flag

    }
	}
  //===========================================================
}



//=============================================
// Configure the UART 1 interrupt handler
//=============================================
void __ISR(_UART_1_VECTOR, ipl6auto) Uart1InterruptHandler(void)
{
  UINT8  i
        ,iMax   // Read/write max 8 bytes/interrupt
        ,data   // used in FifoWrite/Read functions
        ;

	// TX interrupt handling
  //===========================================================

  if ( INTGetEnable ( INT_SOURCE_UART_TX(UART1) ) )               // If TX interrupts enabled
  {
    if ( INTGetFlag ( INT_SOURCE_UART_TX(UART1) ) )               // If TX interrupt occured
    {
      if ( UARTTransmitterIsReady(UART1) && !Uart.Var.uartTxFifo[UART1].bufEmpty )  // If TX buffer is ready to receive data and the user's TX buffer is not empty
      {
        if (Uart.Var.uartTxFifo[UART1].lineBuffer.length < 8)     // Write max 8 bytes/interrupt
        {
          iMax = Uart.Var.uartTxFifo[UART1].lineBuffer.length;
        }
        else
        {
          iMax = 8;
        }

        for (i = 0; i < iMax; i++)
        {
          UartFifoRead((void *) &Uart.Var.uartTxFifo[UART1], &data);  // Copy from user
          U1TXREG = data;                                         // Put data in PIC32's TX buffer
        }
      }

      if (Uart.Var.uartTxFifo[UART1].bufEmpty)                    // If User's TX buffer is empty
      {
        Uart.DisableTxInterrupts(UART1);                          // Disable TX interrupts
      }

//      LED4_TOGGLE;

      INTClearFlag(INT_SOURCE_UART_TX(UART1));                    // Clear the TX interrupt Flag
    }
  }
  //===========================================================
  

	// RX interrupt handling
  //===========================================================
  if ( INTGetEnable ( INT_SOURCE_UART_RX(UART1) ) )               // If RX interrupts enabled
  {
    if ( INTGetFlag ( INT_SOURCE_UART_RX(UART1) ) )               // If RX interrupt occured
    {
      i = 0;
      iMax = 8;                                                   // Read max 8 bytes/interrupt
      while (   UARTReceivedDataIsAvailable(UART1)                // While RX data available
            && !Uart.Var.uartRxFifo[UART1].bufFull                // and user's RX buffer not full
            && (i < iMax)                                         // and under 8 bytes read
            )
      { // while ^
        data = UARTGetDataByte(UART1);
        if ( UartFifoWrite((void *) &Uart.Var.uartRxFifo[UART1], &data) < 0 ) // If copy to user did not work
        {
          break;                                                  // Exit while loop
        }
        i++;
      } // end while

      if (!Uart.Var.uartRxFifo[UART1].bufEmpty)                   // If there is data in the user's RX buffer
      {
        Uart.Var.oIsRxDataAvailable[UART1] = 1;                   // Set according flag
      }

//      LED5_TOGGLE;

      INTClearFlag (INT_SOURCE_UART_RX(UART1) );                  // Clear the RX interrupt Flag

    }
	}
  //===========================================================
}


//=============================================
// Configure the Timer 3 interrupt handler
//=============================================
void __ISR(_TIMER_3_VECTOR, ipl3auto) Timer3InterruptHandler(void)
{
  mT3ClearIntFlag();
}


//=============================================
// Configure the Timer 2 interrupt handler
//=============================================
void __ISR(_TIMER_2_VECTOR, ipl4auto) Timer2InterruptHandler(void)
{
  Timer.Var.nOverflows[1]++;
  
  oTimer2Event = 1;
  
  mT2ClearIntFlag();
}


//=============================================
// Configure the ADC interrupt handler
//=============================================
void __ISR(_ADC_VECTOR, ipl3auto) AdcInterruptHandler(void)
{
//  float ftest[16];
  int i = 0;

  Adc.Read();

  oUartSend = 1;
  INTClearFlag(INT_AD1);    // Clear the ADC conversion done interrupt Flag
}


// Configure the input capture interrupt handler
void __ISR(_INPUT_CAPTURE_1_VECTOR, ipl4auto) InputCaptureInterruptHandler(void)
{

  // Wait for capture data to be ready
  while(!(InputCapture.IsCaptureReady(IC1)));

  InputCapture.Var.previousTimerOverflows[IC1] = InputCapture.Var.currentTimerOverflows[IC1];
  InputCapture.Var.currentTimerOverflows[IC1] = Timer.Var.nOverflows[1];  // Get the number of overflows for Timer2

  // Store the timer value of the capture event into CaptureTime variable
  InputCapture.Var.previousCaptureCountValue[IC1] = InputCapture.Var.currentCaptureCountValue[IC1];
  InputCapture.Var.currentCaptureCountValue [IC1] = InputCapture.ReadCapture(IC1);
  oCapture = 1;

//  Port.A.ToggleBits(BIT_3);

  // Clear the interrupt flag
  INTClearFlag(INT_IC1);
}

//=============================================
// Configure the SPI 1 interrupt handler
//=============================================
//void __ISR(_SPI_1_VECTOR, ipl2auto) Spi1InterruptHandler(void)
//{
//	// RX interrupt handling
//  //===========================================================
//	if(INTGetFlag(INT_SOURCE_SPI_RX(SPI1)))
//	{
//    UINT32 spiData = Spi.GetCharacter(SPI1);
//    if (spiData == 0xDEADBEEF)
//    {
//      Port.A.ToggleBits(BIT_3);
//    }
//    else
//    {
//      Port.C.ToggleBits(BIT_1);
//    }
//    INTClearFlag(INT_SOURCE_SPI_RX(SPI1));    // Clear the RX interrupt Flag
//	}
//  //===========================================================
//
//
//	// TX interrupt handling
//  //===========================================================
//	if ( INTGetFlag(INT_SOURCE_SPI_TX(SPI1)) )
//	{
//    INTClearFlag(INT_SOURCE_SPI_TX(SPI1));    // Clear the TX interrupt Flag
//	}
//  //===========================================================
//}


void Led1(sSkadi_t *skadi, sSkadiArgs_t args)
{
#ifdef __32MX795F512H__
  Port.B.ToggleBits(BIT_12);    // RB12 = DEBUG4 led
#elif defined __32MX795F512L__
  Port.A.ToggleBits(BIT_3);
#endif
}

void Led2(sSkadi_t *skadi, sSkadiArgs_t args)
{
#ifdef __32MX795F512H__
  Port.B.ToggleBits(BIT_13);    // RB13 = CAN led
#elif defined __32MX795F512L__
  Port.C.ToggleBits(BIT_1);
#endif
}

//void Led1(sSkadi_t *skadi, sSkadiArgs_t args)
//{
//  UINT8 errorMsg[] = {"Cette led n'existe pas!\r\n"};
//  int led = atoi(args.elements[0]);
//  if (led == 1)
//  {
//    Port.A.ToggleBits(BIT_3);
//  }
//  else
//  {
//    Uart.SendDataBuffer(UART1, errorMsg, sizeof(errorMsg));
////    printf("Cette led n'existe pas!\r\n");
//  }
//}
//
//void Led2(sSkadi_t *skadi, sSkadiArgs_t args)
//{
//  UINT8 errorMsg[] = {"Cette led n'existe pas!\r\n"};
//  int led = atoi(args.elements[0]);
//  if (led == 2)
//  {
//    Port.C.ToggleBits(BIT_1);
//  }
//  else
//  {
//    Uart.SendDataBuffer(UART1, errorMsg, sizeof(errorMsg));
////    printf("Cette led n'existe pas!\r\n");
//  }
//}



  // DEBUG WDT



//  if (Wdt.ReadEvent())
//  {
//    // A WDT event did occur
//    Wdt.Disable();
//
//    // Clear the WDT event flag so a subsequent event can set the event bit
//    Wdt.ClearEvent();
//
//    // Blink left-most LED ON/OFF forever to indicate that we have seen WDT timeout reset.
//    while (1)
//    {
//
//      if (oTimer2Event)
//      {
//        oTimer2Event = 0;
//
//        Port.A.ToggleBits(BIT_3);
//      }
//    }
//  }
//
////  Wdt.Open(WDT_PS_1_1024, TRUE);
////  WDTCONbits.SWDTPS = WDT_PS_1_1024;
////  WDTCONbits.WDTPSTA = WDT_PS_1_1024;
////  WDTCONbits.WDTPS = WDT_PS_1_1024;
////  DEVCFG1bits.WDTPS = PS1048576;
////  DEVCFG1bits.WDTPS = WDT_PS_1_1024;
//  Wdt.Enable();
//
//  UINT16 test = Wdt.ReadPostScaler();
//
//
//  INT16 byte = 0;
//
//  while (1)
//  {
//    if (oTimer2Event)
//    {
//      oTimer2Event = 0;
//      Port.C.ToggleBits(BIT_1);
//      Wdt.Clear();
//    }
//
//    byte = Uart.GetDataByte(uartModule);
//    if (byte > 0)
//    {
//      Uart.SendDataByte(uartModule, byte);
////      UINT32 i = 1024*100;
////      while(i--);
//      while(1);
////      Port.C.ToggleBits(BIT_1);
//    }
//  }



    /* TEST INPUT CAPTURE */

//  // Setup Timer 23 (timer 2 + timer 3, 32bit time)
//  // Use PBCLK as source, FPB/256, Timer running at (10E6)/256 = 39KHz
////  OpenTimer23(T23_sourceON | T23_SOURCE_INT | T23_PS_1_256 | T23_32BIT_MODE_ON, PERIOD);
////  Timer.Open(TIMER_2, 500, SCALE_US);
//
//  // Enable Input Capture Module 1
//  // - Capture every rising edge
//  // - Interrupt on first capture
//  // - Use Timer 23 source
//  // - Capture rising edge first
////  OpenCapture1(IC_EVERY_RISE_EDGE | IC_INT_1CAPTURE | IC_CAP_32BIT | IC_FEDGE_RISE | IC_ON);
////  OpenCapture1(IC_EVERY_RISE_EDGE | IC_INT_1CAPTURE | IC_TIMER2_SRC | IC_CAP_16BIT | IC_FEDGE_RISE | IC_ON);
//  UINT16 config = IC_EVERY_RISE_EDGE | IC_INT_1CAPTURE | IC_TIMER2_SRC | IC_CAP_16BIT | IC_FEDGE_RISE | IC_ON;
//  InputCapture.Open(IC1, config);
//
//  // Set up the input capture 1 interrupt with a priority of 2, subpriority of 0
////  ConfigIntCapture1(IC_INT_PRIOR_2 | IC_INT_SUB_PRIOR_0 | IC_INT_OFF);
//  InputCapture.ConfigInterrupt(IC1, 4, 0);
////  int dummyRead = IC1BUF;
////  EnableIntIC1;
//  InputCapture.EnableInterrupt(IC1);
////  INTEnable(INT_IC1, INT_ENABLED);
////  INTSetVectorPriority(INT_INPUT_CAPTURE_1_VECTOR, INT_PRIORITY_LEVEL_2);
////  INTSetVectorSubPriority(INT_INPUT_CAPTURE_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
//
//  Pwm.Open(PWM_1);
//
//  UINT16 captureSpeed = 0;
//  // Enable multi-vector interrupts
//  INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
//  INTEnableInterrupts();
//
//  UINT16 timer2Period = Timer.ReadPeriodInCounts(TIMER_2);
//  UINT32 previousOverflows = 0;
//  UINT32 currentOverflows = 0;
//  while(1)
//  {
////      Uart.GetDataBuffer(UART1, msg, 1);
//    msg[0] = Uart.GetDataByte(uartModule);
//
//    if (msg[0] == 'e')
//    {
//      Uart.SendDataByte(uartModule,msg[0]);
//      Port.A.ToggleBits(BIT_3);
//      msg[0] = 0;
//    }
//
//    // Waiting for input capture event
//    if (oCapture)
//    {
////      currentOverflows = InputCapture.Var.currentTimerOverflows[IC1];
////      previousOverflows = InputCapture.Var.previousTimerOverflows[IC1];
////      CaptureTime = InputCapture.Var.currentCaptureCountValue[IC1];
////      previousCaptureTime = InputCapture.Var.previousCaptureCountValue[IC1];
////
//////      UINT32 offset = timer2Period*(InputCapture.Var.currentTimerOverflows - InputCapture.Var.currentTimerOverflows);
////      UINT32 offset = timer2Period*(currentOverflows - previousOverflows);
////
////      UINT16 timerPeriodinTime = Timer.ReadPeriodInTime(TIMER_2);
////
//////      captureSpeed = ((float)(offset + CaptureTime - previousCaptureTime)/timer2Period) * 500;
////      captureSpeed = ((float)(offset + CaptureTime - previousCaptureTime)/timer2Period) * timerPeriodinTime;
//
//      captureSpeed = InputCapture.GetTimeBetweenCaptures(IC1, SCALE_US);
//      if (captureSpeed > 500)
//      {
//        Port.A.ToggleBits(BIT_3);
//      }
//
////      captureSpeed = ((float)(timer2Period*(InputCapture.Var.currentCaptureCountValue - InputCapture.Var.previousCaptureCountValue) + CaptureTime - previousCaptureTime)/timer2Period) * 500;
////      if ( (CaptureTime - previousCaptureTime) < 0 )
////      {
////        captureSpeed = ((float)(timer2Period + CaptureTime - previousCaptureTime)/timer2Period) * 500;
////      }
////      else
////      {
////        captureSpeed = ((float)(CaptureTime - previousCaptureTime)/timer2Period) * 500;
////      }
//      UINT8 size = sprintf(msg, "%d", captureSpeed);
////      Uart.SendDataByte(UART1, msg[0]);
//      Uart.SendDataBuffer(uartModule, msg, size);
//      Uart.SendDataByte(uartModule, '\n');
//      Uart.SendDataByte(uartModule, '\r');
//      msg[0] = 0;
//      oCapture = 0;
//    }
//  }
//}



//  //    // SKADI TEST
//
//  sSkadiCommand_t skadiCommandTable[] =
//  {
//     {"Led1", Led1, 1, "Usage : flash Led 1"}
//    ,{"Led2", Led2, 1, "Usage : flash Led 2"}
//  };
//  size_t length = sizeof(skadiCommandTable)/sizeof(sSkadiCommand_t);
////  Skadi.Init(skadiCommandTable, 2);
//  Skadi.Init(skadiCommandTable, length, uartModule, TRUE);
////  Uart.Get
//  while(1)
//  {
////    Skadi.GetCmdMsg();
//    if ( Uart.Var.oIsRxDataAvailable[UART1] )
//    {
//      Skadi.GetCmdMsgFifo();
//    }
//  }



//    // UART TEST

//  sUartLineBuffer_t buffer =
//  {
//     .buffer = {0}
//    ,.length =  0
//  };
//
//  UINT8 iTxBuffer;
//  while(1)
//  {
//    if (Uart.Var.oIsRxDataAvailable[0])
//    {
//////      memcpy(&buffer.buffer[0], (const void*) &Uart.Var.uartRxFifo[0].buffer[0], Uart.Var.uartRxFifo[0].length * sizeof(UINT8));
//////      buffer.length = Uart.Var.uartRxFifo[0].length;
//////      Uart.Var.uartRxFifo[0].length = 0;
//////      Uart.Var.oIsRxDataAvailable[0] = 0;
//////      Uart.GetRxFifoBuffer(UART1, &buffer, FALSE);
//      Uart.GetRxFifoBuffer(UART1, &buffer, TRUE);
//
//////      Uart.DisableTxInterrupts(UART1);
//////      iTxBuffer = Uart.Var.uartTxFifo[0].length;
//////      memcpy( (void *) &Uart.Var.uartTxFifo[0].buffer[iTxBuffer], &buffer.buffer[0], buffer.length * sizeof(UINT8));
//////      Uart.Var.uartTxFifo[0].length += buffer.length;
//////      Uart.EnableTxInterrupts(UART1);
//      UINT16 i = 0;
//      buffer.length = 0;
//      for (i = 0; i < UART_LINE_BUFFER_LENGTH; i++)
//      {
//        if ((i==78) || (i==78*2) || (i==78*3))
//        {
//          buffer.buffer[i] = '\n';
//        }
//        else if ((i==78+1) || (i==78*2+1) || (i==78*3+1))
//        {
//          buffer.buffer[i] = '\r';
//        }
//        else if (i==254)
//        {
//          buffer.buffer[i] = '\n';
//        }
//        else if (i==255)
//        {
//          buffer.buffer[i] = '\r';
//        }
//        else
//        {
//          buffer.buffer[i] = 'a';
//        }
//        buffer.length++;
//      }
//      Uart.PutTxFifoBuffer(UART1, &buffer);
////      Uart.SendDataBuffer(UART1, buffer.buffer, buffer.length);
//
//      buffer.length = 0;
//    }
//  }