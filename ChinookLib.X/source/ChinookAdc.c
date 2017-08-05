//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// ChinookLib
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : ChinookAdc.c
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

#include "..\headers\ChinookAdc.h"


//==============================================================================
// Private functions prototypes
//==============================================================================
static inline INT8  AdcOpen               (UINT32 samplingClk, UINT32 configHardware, UINT32 configPort, UINT32 configScan);
static inline void  AdcClose              (void);
static inline INT8  AdcConfigInterrupt    (UINT8 priority, UINT8 subPriority);
static inline INT8  AdcEnableInterrupts   (void);
static inline INT8  AdcDisableInterrupts  (void);
static inline INT8  AdcManualRead         (UINT8 channel);
static inline INT8  AdcRead               (void);


//==============================================================================
// Public structure of functions for the ADC
//==============================================================================
struct sChinookAdc Adc =
{
   .Open                = &AdcOpen
  ,.Close               = &AdcClose
  ,.ConfigInterrupt     = &AdcConfigInterrupt
  ,.DisableInterrupts   = &AdcDisableInterrupts
  ,.EnableInterrupts    = &AdcEnableInterrupts
  ,.ManualRead          = &AdcManualRead
  ,.Read                = &AdcRead
  ,.Var.calibValues     = {0}
  ,.Var.adcReadValues   = {0}
  ,.Var.enabledChannels = {0}
  ,.Var.numChannels     =  0
  ,.Var.operatingMode   =  0
};

/*******************************************************************************
 *******************                                       *********************
 ****************       PRIVATE FUNCTION DECLARATIONS         ******************
 *******************                                       *********************
 *******************************************************************************/

static inline INT8 AdcOpen(UINT32 samplingClk, UINT32 configHardware, UINT32 configPort, UINT32 configScan)   // WITHOUT CALIB
{
  //============================================================================
  // Configuration of the ADC
  //============================================================================
  Adc.Close();    // Ensure the ADC is off before setting the configuration

  if (samplingClk == ADC_CLK_MANUAL)
  {
    samplingClk     |= (ADC_FORMAT_INTG | ADC_AUTO_SAMPLING_OFF);
    configHardware  |= (ADC_OFFSET_CAL_DISABLE | ADC_SCAN_OFF | ADC_ALT_BUF_OFF | ADC_ALT_INPUT_OFF);
    configScan       = SKIP_SCAN_ALL;
  }
  else
  {
    samplingClk     |= (ADC_FORMAT_INTG | ADC_AUTO_SAMPLING_ON);
    configHardware  |= (ADC_OFFSET_CAL_DISABLE | ADC_SCAN_ON | ADC_ALT_BUF_OFF | ADC_ALT_INPUT_OFF);
  }
  
  UINT32 config3 = ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_12;

  // Configure and open ADC
  //============================================================================
  OpenADC10( samplingClk, configHardware, config3, configPort, configScan );
  EnableADC10(); // Enable the ADC
  //============================================================================


  // Set the ADC variables
  //============================================================================
  // If ANx is enabled, increase Var.numChannels and put the number of the
  // channel in Var.enabledChannels[i]
  int i = 0;
  if (configPort & ENABLE_AN0_ANA )                        // If ANx is enabled
  {
    Adc.Var.numChannels++;                                 // Increase Var.numChannels
    Adc.Var.enabledChannels[i] = 0;                        // Put the number of the channel in Var.enabledChannels[i]
    i++;
    Adc.Var.adcReadValues[0] = 0;                          // Reset adcReadValues
  }
  if (configPort & ENABLE_AN1_ANA )                        // If ANx is enabled
  {
    Adc.Var.numChannels++;                                 // Increase Var.numChannels
    Adc.Var.enabledChannels[i] = 1;                        // Put the number of the channel in Var.enabledChannels[i]
    i++;
    Adc.Var.adcReadValues[1] = 0;                          // Reset adcReadValues
  }
  if (configPort & ENABLE_AN2_ANA )                        // If ANx is enabled
  {
    Adc.Var.numChannels++;                                 // Increase Var.numChannels
    Adc.Var.enabledChannels[i] = 2;                        // Put the number of the channel in Var.enabledChannels[i]
    i++;
    Adc.Var.adcReadValues[2] = 0;                          // Reset adcReadValues
  }
  if (configPort & ENABLE_AN3_ANA )                        // If ANx is enabled
  {
    Adc.Var.numChannels++;                                 // Increase Var.numChannels
    Adc.Var.enabledChannels[i] = 3;                        // Put the number of the channel in Var.enabledChannels[i]
    i++;
    Adc.Var.adcReadValues[3] = 0;                          // Reset adcReadValues
  }
  if (configPort & ENABLE_AN4_ANA )                        // If ANx is enabled
  {
    Adc.Var.numChannels++;                                 // Increase Var.numChannels
    Adc.Var.enabledChannels[i] = 4;                        // Put the number of the channel in Var.enabledChannels[i]
    i++;
    Adc.Var.adcReadValues[4] = 0;                          // Reset adcReadValues
  }
  if (configPort & ENABLE_AN5_ANA )                        // If ANx is enabled
  {
    Adc.Var.numChannels++;                                 // Increase Var.numChannels
    Adc.Var.enabledChannels[i] = 5;                        // Put the number of the channel in Var.enabledChannels[i]
    i++;
    Adc.Var.adcReadValues[5] = 0;                          // Reset adcReadValues
  }
  if (configPort & ENABLE_AN6_ANA )                        // If ANx is enabled
  {
    Adc.Var.numChannels++;                                 // Increase Var.numChannels
    Adc.Var.enabledChannels[i] = 6;                        // Put the number of the channel in Var.enabledChannels[i]
    i++;
    Adc.Var.adcReadValues[6] = 0;                          // Reset adcReadValues
  }
  if (configPort & ENABLE_AN7_ANA )                        // If ANx is enabled
  {
    Adc.Var.numChannels++;                                 // Increase Var.numChannels
    Adc.Var.enabledChannels[i] = 7;                        // Put the number of the channel in Var.enabledChannels[i]
    i++;
    Adc.Var.adcReadValues[7] = 0;                          // Reset adcReadValues
  }
  if (configPort & ENABLE_AN8_ANA )                        // If ANx is enabled
  {
    Adc.Var.numChannels++;                                 // Increase Var.numChannels
    Adc.Var.enabledChannels[i] = 8;                        // Put the number of the channel in Var.enabledChannels[i]
    i++;
    Adc.Var.adcReadValues[8] = 0;                          // Reset adcReadValues
  }
  if (configPort & ENABLE_AN9_ANA )                        // If ANx is enabled
  {
    Adc.Var.numChannels++;                                 // Increase Var.numChannels
    Adc.Var.enabledChannels[i] = 9;                        // Put the number of the channel in Var.enabledChannels[i]
    i++;
    Adc.Var.adcReadValues[9] = 0;                          // Reset adcReadValues
  }
  if (configPort & ENABLE_AN10_ANA)                        // If ANx is enabled
  {
    Adc.Var.numChannels++;                                 // Increase Var.numChannels
    Adc.Var.enabledChannels[i] = 10;                       // Put the number of the channel in Var.enabledChannels[i]
    i++;
    Adc.Var.adcReadValues[10] = 0;                         // Reset adcReadValues
  }
  if (configPort & ENABLE_AN11_ANA)                        // If ANx is enabled
  {
    Adc.Var.numChannels++;                                 // Increase Var.numChannels
    Adc.Var.enabledChannels[i] = 11;                       // Put the number of the channel in Var.enabledChannels[i]
    i++;
    Adc.Var.adcReadValues[11] = 0;                         // Reset adcReadValues
  }
  if (configPort & ENABLE_AN12_ANA)                        // If ANx is enabled
  {
    Adc.Var.numChannels++;                                 // Increase Var.numChannels
    Adc.Var.enabledChannels[i] = 12;                       // Put the number of the channel in Var.enabledChannels[i]
    i++;
    Adc.Var.adcReadValues[12] = 0;                         // Reset adcReadValues
  }
  if (configPort & ENABLE_AN13_ANA)                        // If ANx is enabled
  {
    Adc.Var.numChannels++;                                 // Increase Var.numChannels
    Adc.Var.enabledChannels[i] = 13;                       // Put the number of the channel in Var.enabledChannels[i]
    i++;
    Adc.Var.adcReadValues[13] = 0;                         // Reset adcReadValues
  }
  if (configPort & ENABLE_AN14_ANA)                        // If ANx is enabled
  {
    Adc.Var.numChannels++;                                 // Increase Var.numChannels
    Adc.Var.enabledChannels[i] = 14;                       // Put the number of the channel in Var.enabledChannels[i]
    i++;
    Adc.Var.adcReadValues[14] = 0;                         // Reset adcReadValues
  }
  if (configPort & ENABLE_AN15_ANA)                        // If ANx is enabled
  {
    Adc.Var.numChannels++;                                 // Increase Var.numChannels
    Adc.Var.enabledChannels[i] = 15;                       // Put the number of the channel in Var.enabledChannels[i]
    Adc.Var.adcReadValues[15] = 0;                         // Reset adcReadValues
  }

  Adc.Var.operatingMode = (samplingClk & _AD1CON1_SSRC_MASK);
  //============================================================================
  
  return 0;
}
                                                                                   
// WITH CALIB
                                                                                   
//static inline INT8 AdcOpen(UINT32 samplingClk, UINT32 configHardware, UINT32 configPort, UINT32 configScan)
//{
//  //============================================================================
//  // Calibration of the ADC for the channels needed
//  //============================================================================
//  Adc.Close();    // Ensure the ADC is off before setting the configuration
//
//  UINT32  config1 = ADC_FORMAT_INTG | ADC_CLK_MANUAL | ADC_AUTO_SAMPLING_OFF
//         ,config2 = ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_ENABLE | ADC_SCAN_OFF | ADC_SAMPLES_PER_INT_1 | ADC_ALT_BUF_OFF | ADC_ALT_INPUT_OFF
//         ,config3 = ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_12
//         ,config4 = configPort
//         ,config5 = SKIP_SCAN_ALL
//         ;
//
//  // Configure and open ADC
//  //============================================================================
//  OpenADC10( config1, config2, config3, config3, config5 );
//  EnableADC10(); // Enable the ADC
//  //============================================================================
//
//
//  // Set the ADC variables
//  //============================================================================
//  // If ANx is enabled, increase Var.numChannels and put the number of the
//  // channel in Var.enabledChannels[i]
//  int i = 0;
//  if (configPort & ENABLE_AN0_ANA )                        // If ANx is enabled
//  {
//    Adc.Var.numChannels++;                                 // Increase Var.numChannels
//    Adc.Var.enabledChannels[i] = 0;                        // Put the number of the channel in Var.enabledChannels[i]
//    i++;
//    Adc.ManualRead(0);                                     // Read the offset on ANx
//    Adc.Var.calibValues[0]   = Adc.Var.adcReadValues[0];   // Put the offset value in Adc.Var.calibValues[x]
//    Adc.Var.adcReadValues[0] = 0;                          // Reset adcReadValues
//  }
//  if (configPort & ENABLE_AN1_ANA )                        // If ANx is enabled
//  {
//    Adc.Var.numChannels++;                                 // Increase Var.numChannels
//    Adc.Var.enabledChannels[i] = 1;                        // Put the number of the channel in Var.enabledChannels[i]
//    i++;
//    Adc.ManualRead(1);                                     // Read the offset on ANx
//    Adc.Var.calibValues[1]   = Adc.Var.adcReadValues[1];   // Put the offset value in Adc.Var.calibValues[x]
//    Adc.Var.adcReadValues[1] = 0;                          // Reset adcReadValues
//  }
//  if (configPort & ENABLE_AN2_ANA )                        // If ANx is enabled
//  {
//    Adc.Var.numChannels++;                                 // Increase Var.numChannels
//    Adc.Var.enabledChannels[i] = 2;                        // Put the number of the channel in Var.enabledChannels[i]
//    i++;
//    Adc.ManualRead(2);                                     // Read the offset on ANx
//    Adc.Var.calibValues[2]   = Adc.Var.adcReadValues[2];   // Put the offset value in Adc.Var.calibValues[x]
//    Adc.Var.adcReadValues[2] = 0;                          // Reset adcReadValues
//  }
//  if (configPort & ENABLE_AN3_ANA )                        // If ANx is enabled
//  {
//    Adc.Var.numChannels++;                                 // Increase Var.numChannels
//    Adc.Var.enabledChannels[i] = 3;                        // Put the number of the channel in Var.enabledChannels[i]
//    i++;
//    Adc.ManualRead(3);                                     // Read the offset on ANx
//    Adc.Var.calibValues[3]   = Adc.Var.adcReadValues[3];   // Put the offset value in Adc.Var.calibValues[x]
//    Adc.Var.adcReadValues[3] = 0;                          // Reset adcReadValues
//  }
//  if (configPort & ENABLE_AN4_ANA )                        // If ANx is enabled
//  {
//    Adc.Var.numChannels++;                                 // Increase Var.numChannels
//    Adc.Var.enabledChannels[i] = 4;                        // Put the number of the channel in Var.enabledChannels[i]
//    i++;
//    Adc.ManualRead(4);                                     // Read the offset on ANx
//    Adc.Var.calibValues[4]   = Adc.Var.adcReadValues[4];   // Put the offset value in Adc.Var.calibValues[x]
//    Adc.Var.adcReadValues[4] = 0;                          // Reset adcReadValues
//  }
//  if (configPort & ENABLE_AN5_ANA )                        // If ANx is enabled
//  {
//    Adc.Var.numChannels++;                                 // Increase Var.numChannels
//    Adc.Var.enabledChannels[i] = 5;                        // Put the number of the channel in Var.enabledChannels[i]
//    i++;
//    Adc.ManualRead(5);                                     // Read the offset on ANx
//    Adc.Var.calibValues[5]   = Adc.Var.adcReadValues[5];   // Put the offset value in Adc.Var.calibValues[x]
//    Adc.Var.adcReadValues[5] = 0;                          // Reset adcReadValues
//  }
//  if (configPort & ENABLE_AN6_ANA )                        // If ANx is enabled
//  {
//    Adc.Var.numChannels++;                                 // Increase Var.numChannels
//    Adc.Var.enabledChannels[i] = 6;                        // Put the number of the channel in Var.enabledChannels[i]
//    i++;
//    Adc.ManualRead(6);                                     // Read the offset on ANx
//    Adc.Var.calibValues[6]   = Adc.Var.adcReadValues[6];   // Put the offset value in Adc.Var.calibValues[x]
//    Adc.Var.adcReadValues[6] = 0;                          // Reset adcReadValues
//  }
//  if (configPort & ENABLE_AN7_ANA )                        // If ANx is enabled
//  {
//    Adc.Var.numChannels++;                                 // Increase Var.numChannels
//    Adc.Var.enabledChannels[i] = 7;                        // Put the number of the channel in Var.enabledChannels[i]
//    i++;
//    Adc.ManualRead(7);                                     // Read the offset on ANx
//    Adc.Var.calibValues[7]   = Adc.Var.adcReadValues[7];   // Put the offset value in Adc.Var.calibValues[x]
//    Adc.Var.adcReadValues[7] = 0;                          // Reset adcReadValues
//  }
//  if (configPort & ENABLE_AN8_ANA )                        // If ANx is enabled
//  {
//    Adc.Var.numChannels++;                                 // Increase Var.numChannels
//    Adc.Var.enabledChannels[i] = 8;                        // Put the number of the channel in Var.enabledChannels[i]
//    i++;
//    Adc.ManualRead(8);                                     // Read the offset on ANx
//    Adc.Var.calibValues[8]   = Adc.Var.adcReadValues[8];   // Put the offset value in Adc.Var.calibValues[x]
//    Adc.Var.adcReadValues[8] = 0;                          // Reset adcReadValues
//  }
//  if (configPort & ENABLE_AN9_ANA )                        // If ANx is enabled
//  {
//    Adc.Var.numChannels++;                                 // Increase Var.numChannels
//    Adc.Var.enabledChannels[i] = 9;                        // Put the number of the channel in Var.enabledChannels[i]
//    i++;
//    Adc.ManualRead(9);                                     // Read the offset on ANx
//    Adc.Var.calibValues[9]   = Adc.Var.adcReadValues[9];   // Put the offset value in Adc.Var.calibValues[x]
//    Adc.Var.adcReadValues[9] = 0;                          // Reset adcReadValues
//  }
//  if (configPort & ENABLE_AN10_ANA)                        // If ANx is enabled
//  {
//    Adc.Var.numChannels++;                                 // Increase Var.numChannels
//    Adc.Var.enabledChannels[i] = 10;                       // Put the number of the channel in Var.enabledChannels[i]
//    i++;
//    Adc.ManualRead(10);                                    // Read the offset on ANx
//    Adc.Var.calibValues[10]   = Adc.Var.adcReadValues[10]; // Put the offset value in Adc.Var.calibValues[x]
//    Adc.Var.adcReadValues[10] = 0;                         // Reset adcReadValues
//  }
//  if (configPort & ENABLE_AN11_ANA)                        // If ANx is enabled
//  {
//    Adc.Var.numChannels++;                                 // Increase Var.numChannels
//    Adc.Var.enabledChannels[i] = 11;                       // Put the number of the channel in Var.enabledChannels[i]
//    i++;
//    Adc.ManualRead(11);                                    // Read the offset on ANx
//    Adc.Var.calibValues[11]   = Adc.Var.adcReadValues[11]; // Put the offset value in Adc.Var.calibValues[x]
//    Adc.Var.adcReadValues[11] = 0;                         // Reset adcReadValues
//  }
//  if (configPort & ENABLE_AN12_ANA)                        // If ANx is enabled
//  {
//    Adc.Var.numChannels++;                                 // Increase Var.numChannels
//    Adc.Var.enabledChannels[i] = 12;                       // Put the number of the channel in Var.enabledChannels[i]
//    i++;
//    Adc.ManualRead(12);                                    // Read the offset on ANx
//    Adc.Var.calibValues[12]   = Adc.Var.adcReadValues[12]; // Put the offset value in Adc.Var.calibValues[x]
//    Adc.Var.adcReadValues[12] = 0;                         // Reset adcReadValues
//  }
//  if (configPort & ENABLE_AN13_ANA)                        // If ANx is enabled
//  {
//    Adc.Var.numChannels++;                                 // Increase Var.numChannels
//    Adc.Var.enabledChannels[i] = 13;                       // Put the number of the channel in Var.enabledChannels[i]
//    i++;
//    Adc.ManualRead(13);                                    // Read the offset on ANx
//    Adc.Var.calibValues[13]   = Adc.Var.adcReadValues[13]; // Put the offset value in Adc.Var.calibValues[x]
//    Adc.Var.adcReadValues[13] = 0;                         // Reset adcReadValues
//  }
//  if (configPort & ENABLE_AN14_ANA)                        // If ANx is enabled
//  {
//    Adc.Var.numChannels++;                                 // Increase Var.numChannels
//    Adc.Var.enabledChannels[i] = 14;                       // Put the number of the channel in Var.enabledChannels[i]
//    i++;
//    Adc.ManualRead(14);                                    // Read the offset on ANx
//    Adc.Var.calibValues[14]   = Adc.Var.adcReadValues[14]; // Put the offset value in Adc.Var.calibValues[x]
//    Adc.Var.adcReadValues[14] = 0;                         // Reset adcReadValues
//  }
//  if (configPort & ENABLE_AN15_ANA)                        // If ANx is enabled
//  {
//    Adc.Var.numChannels++;                                 // Increase Var.numChannels
//    Adc.Var.enabledChannels[i] = 15;                       // Put the number of the channel in Var.enabledChannels[i]
//    Adc.ManualRead(15);                                    // Read the offset on ANx
//    Adc.Var.calibValues[15]   = Adc.Var.adcReadValues[15]; // Put the offset value in Adc.Var.calibValues[x]
//    Adc.Var.adcReadValues[15] = 0;                         // Reset adcReadValues
//  }
//
//  Adc.Var.operatingMode = (samplingClk & _AD1CON1_SSRC_MASK);
//  //============================================================================
//
//
//  //============================================================================
//  // Configuration of the ADC
//  //============================================================================
//  Adc.Close();    // Ensure the ADC is off before setting the configuration
//
//  if (samplingClk == ADC_CLK_MANUAL)
//  {
//    samplingClk     |= (ADC_FORMAT_INTG | ADC_AUTO_SAMPLING_OFF);
//    configHardware  |= (ADC_OFFSET_CAL_DISABLE | ADC_SCAN_OFF | ADC_ALT_BUF_OFF | ADC_ALT_INPUT_OFF);
//    configScan       = SKIP_SCAN_ALL;
//  }
//  else
//  {
//    samplingClk     |= (ADC_FORMAT_INTG | ADC_AUTO_SAMPLING_ON);
//    configHardware  |= (ADC_OFFSET_CAL_DISABLE | ADC_SCAN_ON | ADC_ALT_BUF_OFF | ADC_ALT_INPUT_OFF);
//  }
//  
////  UINT32 config3 = ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_12;
//
//  // Configure and open ADC
//  //============================================================================
//  OpenADC10( samplingClk, configHardware, config3, configPort, configScan );
//  EnableADC10(); // Enable the ADC
//  //============================================================================
//
//
////  // Set the ADC variables
////  //============================================================================
////  i = 0;
////  // If ANx is enabled, increase Var.numChannels and put the number of the
////  // channel in Var.enabledChannels[i]
////  if (configPort & ENABLE_AN0_ANA )   { Adc.Var.numChannels++; Adc.Var.enabledChannels[i] = 0 ; i++; }
////  if (configPort & ENABLE_AN1_ANA )   { Adc.Var.numChannels++; Adc.Var.enabledChannels[i] = 1 ; i++; }
////  if (configPort & ENABLE_AN2_ANA )   { Adc.Var.numChannels++; Adc.Var.enabledChannels[i] = 2 ; i++; }
////  if (configPort & ENABLE_AN3_ANA )   { Adc.Var.numChannels++; Adc.Var.enabledChannels[i] = 3 ; i++; }
////  if (configPort & ENABLE_AN4_ANA )   { Adc.Var.numChannels++; Adc.Var.enabledChannels[i] = 4 ; i++; }
////  if (configPort & ENABLE_AN5_ANA )   { Adc.Var.numChannels++; Adc.Var.enabledChannels[i] = 5 ; i++; }
////  if (configPort & ENABLE_AN6_ANA )   { Adc.Var.numChannels++; Adc.Var.enabledChannels[i] = 6 ; i++; }
////  if (configPort & ENABLE_AN7_ANA )   { Adc.Var.numChannels++; Adc.Var.enabledChannels[i] = 7 ; i++; }
////  if (configPort & ENABLE_AN8_ANA )   { Adc.Var.numChannels++; Adc.Var.enabledChannels[i] = 8 ; i++; }
////  if (configPort & ENABLE_AN9_ANA )   { Adc.Var.numChannels++; Adc.Var.enabledChannels[i] = 9 ; i++; }
////  if (configPort & ENABLE_AN10_ANA)   { Adc.Var.numChannels++; Adc.Var.enabledChannels[i] = 10; i++; }
////  if (configPort & ENABLE_AN11_ANA)   { Adc.Var.numChannels++; Adc.Var.enabledChannels[i] = 11; i++; }
////  if (configPort & ENABLE_AN12_ANA)   { Adc.Var.numChannels++; Adc.Var.enabledChannels[i] = 12; i++; }
////  if (configPort & ENABLE_AN13_ANA)   { Adc.Var.numChannels++; Adc.Var.enabledChannels[i] = 13; i++; }
////  if (configPort & ENABLE_AN14_ANA)   { Adc.Var.numChannels++; Adc.Var.enabledChannels[i] = 14; i++; }
////  if (configPort & ENABLE_AN15_ANA)   { Adc.Var.numChannels++; Adc.Var.enabledChannels[i] = 15;      }
////
////  Adc.Var.operatingMode = (samplingClk & _AD1CON1_SSRC_MASK);
////  //============================================================================
//  
//  return 0;
//}


static inline void AdcClose(void)
{
  CloseADC10();
}


static inline INT8 AdcConfigInterrupt(UINT8 priority, UINT8 subPriority)
{
  INT8 err = 0;
  
  if (Adc.Var.operatingMode == ADC_CLK_MANUAL)
  {
    return -1;  // Wrong operating mode
  }

  if ( (priority > 7) || (subPriority > 3) )  // Wrong arguments
  {
    return -1;  // EXIT_FAILURE
  }

  switch (subPriority)
  {
    case 0  :   subPriority = ADC_INT_SUB_PRI_0;
                break;
    case 1  :   subPriority = ADC_INT_SUB_PRI_1;
                break;
    case 2  :   subPriority = ADC_INT_SUB_PRI_2;
                break;
    case 3  :   subPriority = ADC_INT_SUB_PRI_3;
                break;
    default :   subPriority = ADC_INT_SUB_PRI_0;
                break;
  }

  ConfigIntADC10(ADC_INT_OFF | priority | subPriority);   // Configure the ADC interrupts without enabling them

  return err;  // EXIT_SUCCESS
}


static inline INT8 AdcEnableInterrupts(void)
{
  if (Adc.Var.operatingMode == ADC_CLK_MANUAL)
  {
    return -1;  // Wrong operating mode
  }
  mAD1IntEnable(1);   // Enable ADC interrupts
  return 0;
}


static inline INT8 AdcDisableInterrupts(void)
{
  if (Adc.Var.operatingMode == ADC_CLK_MANUAL)
  {
    return -1;  // Wrong operating mode
  }
  mAD1IntEnable(0);   // Disable ADC interrupts
  return 0;
}


static inline INT8 AdcManualRead(UINT8 channel)
{
  if (Adc.Var.operatingMode != ADC_CLK_MANUAL)
  {
    return -1;  // Wrong function
  }
  if (channel > 15)
  {
    return -1;  // Wrong argument
  }

  UINT32 adcPosSample = 0;

  // Choose which channel to sample
  //================================================================
  if      (channel == 0 )  adcPosSample = ADC_CH0_POS_SAMPLEA_AN0;
  else if (channel == 1 )  adcPosSample = ADC_CH0_POS_SAMPLEA_AN1;
  else if (channel == 2 )  adcPosSample = ADC_CH0_POS_SAMPLEA_AN2;
  else if (channel == 3 )  adcPosSample = ADC_CH0_POS_SAMPLEA_AN3;
  else if (channel == 4 )  adcPosSample = ADC_CH0_POS_SAMPLEA_AN4;
  else if (channel == 5 )  adcPosSample = ADC_CH0_POS_SAMPLEA_AN5;
  else if (channel == 6 )  adcPosSample = ADC_CH0_POS_SAMPLEA_AN6;
  else if (channel == 7 )  adcPosSample = ADC_CH0_POS_SAMPLEA_AN7;
  else if (channel == 8 )  adcPosSample = ADC_CH0_POS_SAMPLEA_AN8;
  else if (channel == 9 )  adcPosSample = ADC_CH0_POS_SAMPLEA_AN9;
  else if (channel == 10)  adcPosSample = ADC_CH0_POS_SAMPLEA_AN10;
  else if (channel == 11)  adcPosSample = ADC_CH0_POS_SAMPLEA_AN11;
  else if (channel == 12)  adcPosSample = ADC_CH0_POS_SAMPLEA_AN12;
  else if (channel == 13)  adcPosSample = ADC_CH0_POS_SAMPLEA_AN13;
  else if (channel == 14)  adcPosSample = ADC_CH0_POS_SAMPLEA_AN14;
  else if (channel == 15)  adcPosSample = ADC_CH0_POS_SAMPLEA_AN15;
  //================================================================

  SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF | adcPosSample);

  // First sample and conversion
  //================================================================
  AcquireADC10();
  UINT16 lp = 0;
  while (lp != 1000)    // Time required for sampling
  {
    lp++;
  }
  ConvertADC10();
  while(BusyADC10());   // Wait for conversion done
  //================================================================

  // Second sample and conversion for stabilization of the reading
  //================================================================
  AcquireADC10();
  lp = 0;
  while (lp != 1000)    // Time required for sampling
  {
    lp++;
  }
  ConvertADC10();   // Wait for conversion done
  while(BusyADC10());
  //================================================================

  Adc.Var.adcReadValues[channel] = ReadADC10(0);

  return 0;   // EXIT_SUCCESS
}


static inline INT8 AdcRead(void)
{
  if (Adc.Var.operatingMode == ADC_CLK_MANUAL)
  {
    return -1;  // Wrong function
  }


  int i = 0;

  // Following code reads the ADC buffer and puts each single value in
  // Adc.Var.adcReadValues[channel]. For example, the reading on AN2 will be put
  // in Adc.Var.adcReadValues[2].
  //============================================================================
  for (i = 0; i<Adc.Var.numChannels; i++)
  {
    Adc.Var.adcReadValues[ Adc.Var.enabledChannels[i] ] = ReadADC10(i);
  }
  //============================================================================

  return 0;   // EXIT_SUCCESS
}