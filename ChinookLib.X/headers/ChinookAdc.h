//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// ChinookLib
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : ChinookAdc.h
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


#ifndef __CHINOOK_ADC__
#define	__CHINOOK_ADC__

#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>
#include "ChinookHardwareProfile.h"


//==============================================================================
// Public functions prototypes
//==============================================================================



//==============================================================================
// Macro definitions
//==============================================================================



//==============================================================================
// Variable declarations
//==============================================================================
struct sChinookAdcVar
{
  /**
   * @Description   Number of enabled channels for the ADC.
   */
  UINT8   numChannels;


  /**
   * @Description   Calibration values obtained at the opening of the ADC.
   */
  UINT32  calibValues[16];


  /**
   * @Description   Which channels are enabled.
   */
  UINT32  enabledChannels[16];


  /**
   * @Description   Operating mode of the ADC. Possibles values : \n
   *                ADC_CLK_AUTO    // Internal counter ends sampling and starts conversion\n
   *                ADC_CLK_TMR     // Timer3 or Timer23 compare ends sampling and starts conversion\n
   *                ADC_CLK_INT0    // Active transition on INT0 ends sampling and starts conversion\n
   *                ADC_CLK_MANUAL  // Manual sampling
   */
  UINT32  operatingMode;


  /**
   * @Description   Values obtained from read functions. This variable is volatile.
   */
  volatile UINT16  adcReadValues[16];
};

//==============================================================================
// Public structure declaration
//==============================================================================
struct sChinookAdc
{

  /**
   * @Prototype     static inline INT8 AdcOpen(UINT32 samplingClk, UINT32 configHardware, UINT32 configPort, UINT32 configScan);
   *
   * @Description   This function opens the 10 bits ADC with the configuration
   *                stated by the arguments. Some of the configurable parameters
   *                were set by the developpers of ChinookLib to decrease the 
   *                complexity of the function. If you judge that the parameters
   *                set should be configurable by the user, please tell the 
   *                developpers of ChinookLib.\n\n
   *                NON CONFIGURABLE PARAMETERS\n
   *                - Data format in 16 bits integer;\n
   *                - ADC works in auto sampling mode when not in manual mode;\n
   *                - ADC works in scan mode when not in manual mode;\n
   *                - Buffer configured as one 16-word buffer;\n
   *                - ADC uses one MUX;\n
   *                - A calibration is done at the beginning of the function and
   *                  the offset read are put in Adc.Var.calibValues.
   *                - ADC uses internal RC clock and a sampling time of 12 TADs.\n\n
   *                For more information on ADC config, please refer to the definition of
   *                functions OpenADC10() and SetChanADC10() located in adc10.h
   *                and see Microchip's documentation on 10 bits Analog to Digital
   *                Converter.
   *
   * @Example       UINT32 samplingClk = ADC_CLK_TMR;   // Timer3 used for sampling
   *                UINT32 configHardware =   ADC_VREF_AVDD_AVSS // Vref+ is AVdd and Vref- is AVss\n
   *                                        | ADC_SAMPLES_PER_INT_16; // 16 samples/interrupt\n
   *                UINT32 configPort = ENABLE_ALL_ANA; // Enable AN0-AN15 in analog mode\n
   *                UINT32 configScan = 0; // Scan all ANs\n
   *                err = Adc.Open(samplingClk, configHardware, configPort, configScan);\n
   *                This example opens the ADC with the specified parameters.
   *
   * @Parameter     UINT32 samplingClk\n
   *                Specifies the way sampling will be done.\n
   *                Possibles values : \n
   *                ADC_CLK_AUTO    // Internal counter ends sampling and starts conversion\n
   *                ADC_CLK_TMR     // Timer3 or Timer23 compare ends sampling and starts conversion\n
   *                ADC_CLK_INT0    // Active transition on INT0 ends sampling and starts conversion\n
   *                ADC_CLK_MANUAL  // Manual sampling. In this mode, configHardware MUST include ADC_SAMPLES_PER_INT_1
   *
   * @Parameter     UINT32 configHardware\n
   *                Configure the Vrefs, calib and samples/interrupt. Each value
   *                must be separated by a bitwise OR operator |\n\n
   *                Voltage reference - Values are mutually exclusive\n
   *                ADC_VREF_AVDD_AVSS  // Vref+ is AVdd and Vref- is AVss\n
   *                ADC_VREF_EXT_AVSS   // Vref+ external and Vref- is AVss\n
   *                ADC_VREF_AVDD_EXT   // Vref+ AVdd and Vref- external\n
   *                ADC_VREF_EXT_EXT    // Both Vref+ and Vref- are external\n\n
   *                Offset calibration values - Values are mutually exclusive\n
   *                Number of samples between interrupts - Values are mutually exclusive\n
   *                ADC_SAMPLES_PER_INT_1   // A/D Interrupt at the completion of conversion for each sample\n
   *                ...\n
   *                ADC_SAMPLES_PER_INT_16  // A/D interrupt at the completion of conversion for each 16th sample
   *
   * @Parameter     UINT32 configPort\n
   *                Decide which channels to sample. Each value must be separated
   *                by a bitwise OR operator |. Values are mutually exclusive\n\n
   *                ENABLE_AN0_ANA    // Enable AN0 in analog mode\n
   *                ...\n
   *                ENABLE_AN15_ANA   // Enable AN15 in analog mode\n
   *                ENABLE_ALL_ANA    // Enable AN0-AN15 in analog mode
   *
   * @Parameter     UINT32 configScan\n
   *                Channels to skip during auto scan mode (automatically SKIP_SCAN_ALL
   *                when in manual mode - Values are mutually exclusive. Each
   *                value must be separated by a bitwise OR operator |.\n\n
   *                SKIP_SCAN_AN0   // AN0 disabled for Input Scan\n
   *                ...\n
   *                SKIP_SCAN_AN15  // AN15 disabled for Input Scan\n
   *                SKIP_SCAN_ALL   // Disable AN0-AN15 for Input Scan\n
   *                0               // Don't skip any channel
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Conditions of failure : Wrong arguments.
   */
  INT8 (*Open)  (UINT32 samplingClk, UINT32 configHardware, UINT32 configPort, UINT32 configScan);


  /**
   * @Prototype     static inline void AdcClose (void);
   *
   * @Description   This function closes the ADC.
   *
   * @Example       Adc.Close();\n
   *                This example close the ADC.
   *
   * @Parameters    None.
   *
   * @Returns       None.
   */
  void (*Close) (void);


  /**
   * @Prototype     static inline UINT8 AdcConfigInterrupt(UINT8 priority, UINT8 subPriority);
   *
   * @Description   This function is only available when ADC is not operating in
   *                manual mode. This function configures the ADC interrupt at the
   *                priority and subpriority needed. It does not enables the
   *                interrupts. To enable them, use Adc.EnableInterrupts();
   *
   * @Example       err = Adc.ConfigInterrupt(7, 1);\n
   *                This example configures ADC interrupts with a
   *                priority of 7 (highest) and subpriority of 1.
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
   *                Reasons of failure : wrong arguments, ADC is is manual mode.
   */
  INT8 (*ConfigInterrupt) (UINT8 priority, UINT8 subPriority);


  /**
   * @Prototype     static inline INT8 AdcEnableInterrupts(void);
   *
   * @Description   This function is only available when ADC is not operating in
   *                manual mode. This function enables the ADC interrupts.
   *
   * @Example       err = Adc.EnableInterrupt();\n
   *                This example enables ADC interrupts.
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Reasons of failure : ADC is is manual mode.
   */
  INT8 (*EnableInterrupts) (void);


  /**
   * @Prototype     static inline INT8 AdcDisableInterrupts(void);
   *
   * @Description   This function is only available when ADC is not operating in
   *                manual mode. This function disables the ADC interrupts.
   *
   * @Example       err = Adc.DisableInterrupt();\n
   *                This example disables ADC interrupts.
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Reasons of failure : ADC is is manual mode.
   */
  INT8 (*DisableInterrupts) (void);


  /**
   * @Prototype     static inline INT8 AdcManualRead(UINT8 channel);
   *
   * @Description   This function performs a manual read on the ADC on the channel
   *                specified. It puts the read value in Adc.Var.adcReadValues[channel].
   *                This function can only be used when operating in manual mode.
   *
   * @Example       err = Adc.ManualRead(8);\n
   *                This example performs a sampling and conversion on AN8.
   *
   * @Parameter     UINT8 channel\n
   *                The ADC channel from which the user wants to read.
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Reasons of failure : wrong arguments or ADC not in manual mode.
   */
  INT8 (*ManualRead) (UINT8 channel);


  /**
   * @Prototype     static inline INT8 AdcRead(UINT8 channel);
   *
   * @Description   This function performs a read on the ADC on all of the
   *                enabled channels. It puts each reading in
   *                Adc.Var.adcReadValues[channel].
   *                This function can only be used when NOT operating in manual mode.
   *                It can be used in interrupts.
   *
   * @Example       err = Adc.Read();\n
   *                This example reads the available values in ADC. If the enabled
   *                channels were AN2, AN5 and AN13, then the adcReadValues filled
   *                will be adcReadValues[2], adcReadValues[5] and adcReadValues[13].
   *
   * @Parameters    None.
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Reasons of failure : ADC in manual mode.
   */
  INT8 (*Read) (void);


  /**
   * @Description   Contains certains variables useful for the ADC. Write a dot
   *                "." after writing "Var" to see a description of the variables
   *                available.
   */
  struct sChinookAdcVar Var;

};

#endif	/* __CHINOOK_ADC__ */

