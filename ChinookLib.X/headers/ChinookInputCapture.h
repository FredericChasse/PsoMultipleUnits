//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// ChinookLib
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : ChinookInputCapture.h
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


#ifndef __CHINOOK_INPUT_CAPTURE__
#define	__CHINOOK_INPUT_CAPTURE__

#include "ChinookHardwareProfile.h"
#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>

#include "ChinookTimer.h"


//==============================================================================
// Macro definitions
//==============================================================================
typedef enum InpCapModule_t
{
   IC1  = 0
  ,IC2  = 1
  ,IC3  = 2
  ,IC4  = 3
  ,IC5  = 4
} InpCapModule_t;

//==============================================================================
// Variable declarations
//==============================================================================
struct sChinookInputCaptureVar
{
  /**
   * @Description   Newest captured values of the timer used in counts (not in time).
   *                Each index is the matrix corresponds to a different Input
   *                Capture module.
   *
   * @Example       UINT32 capture = InputCapture.Var.currentCaptureCountValue[IC2];\n
   *                Read the last captured value of the Input Capture 2.
   */
  volatile UINT32 currentCaptureCountValue[5];


  /**
   * @Description   Previous captured values of the timer used in counts (not in time).
   *                Each index is the matrix corresponds to a different Input
   *                Capture module.
   *
   * @Example       UINT32 capture = InputCapture.Var.previousCaptureCountValue[IC2];\n
   *                Read the previous captured value of the Input Capture 2.
   */
  volatile UINT32 previousCaptureCountValue[5];


  /**
   * @Description   Current value of overflows that occured on the timer used.
   *                Each index is the matrix corresponds to a different Input
   *                Capture module.
   *
   * @Example       UINT32 nOverflow = InputCapture.Var.previousTimerOverflows[IC2];\n
   *                Read the current number of overflows that occured on the 
   *                timer used by Input Capture 2.
   */
  volatile UINT32 currentTimerOverflows[5];


  /**
   * @Description   Previous value of overflows that occured on the timer used.
   *                Each index is the matrix corresponds to a different Input
   *                Capture module.
   *
   * @Example       UINT32 nOverflow = InputCapture.Var.previousTimerOverflows[IC2];\n
   *                Read the previous number of overflows that occured on the
   *                timer used by Input Capture 2.
   */
  volatile UINT32 previousTimerOverflows[5];


  /**
   * @Description   Timer used by the input capture. Each index is the matrix
   *                corresponds to a different Input Capture module.
   */
  TimerNum_t timerUsed[5];
};


//==============================================================================
// Public structure declaration
//==============================================================================


struct sChinookInputCapture
{

  /**
   * @Prototype     static inline INT8 InputCaptureOpen (InpCapModule_t inpCapModuleId, I2cFreq_t i2cFreq);
   *
   * @Description   This function opens the Input Capture module specified at
   *                the configuration specified.
   *
   * @Example       UINT16 config = IC_EVERY_RISE_EDGE | IC_INT_1CAPTURE | IC_TIMER2_SRC | IC_CAP_16BIT | IC_FEDGE_RISE | IC_ON;
   *                err = InputCapture.Open(IC1, config);\n
   *                This example opens the InputCapture 1 with a capture every 
   *                rising edge, an interrupt at every capture, using the 16 bits
   *                Timer2, detecting rising edge first. It also activate the
   *                Input Capture.
   *
   * @Example       UINT16 config = IC_EVERY_RISE_EDGE | IC_INT_1CAPTURE | IC_CAP_32BIT | IC_FEDGE_RISE | IC_ON;
   *                err = InputCapture.Open(IC1, config);\n
   *                This example opens the InputCapture 1 with a capture every
   *                rising edge, an interrupt at every capture, using the 32 bits
   *                Timer23, detecting rising edge first. It also activate the
   *                Input Capture.
   *
   * @Parameter     InpCapModule_t inpCapModuleId\n
   *                Specifies which module this function addresses.\n
   *                Possibles values : \n
   *                IC1, IC2, IC3, IC4, IC5.
   *
   * @Parameter     UINT16 config\n
   *                This parameter is a logic OR operation between different modes
   *                of operation for the input capture. They are presented below.
   *                Note that the modes in each section are mutually exclusive. \n\n
   *                On/Off Control : \n
   *                IC_ON     // Input Capture ON\n
   *                IC_OFF    // Input Capture OFF\n\n
   *                First Edge : \n
   *                IC_FEDGE_RISE   // Capture rising edge first\n
   *                IC_FEDGE_FALL   // Capture falling edge first\n\n
   *                Number of bits of the timer used : \n
   *                IC_CAP_32BIT  // Use a 32 bits timer. Always Timer23\n
   *                IC_CAP_16BIT  // Use a 16 bits timer. Timer2 or Timer3\n\n
   *                Timer select :  // Do not use when operating on 32 bits\n
   *                IC_TIMER2_SRC   // Use Timer2\n
   *                IC_TIMER3_SRC   // Use Timer3\n\n
   *                Captures per interrupt : \n
   *                IC_INT_4CAPTURE // Interrupt on fourth Capture\n
   *                IC_INT_3CAPTURE // Interrupt on third Capture\n
   *                IC_INT_2CAPTURE // Interrupt on second Capture\n
   *                IC_INT_1CAPTURE // Interrupt on first Capture\n\n
   *                Input Capture mode select : \n
   *                IC_SP_EVERY_EDGE      // Specified edge first, then every rising/falling edge\n
   *                IC_EVERY_16_RISE_EDGE // Every 16th rising edge\n
   *                IC_EVERY_4_RISE_EDGE  // Every 4th rising edge\n
   *                IC_EVERY_RISE_EDGE    // Every rising edge\n
   *                IC_EVERY_FALL_EDGE    // Every falling edge\n
   *                IC_EVERY_EDGE         // Every rising/falling edge\n
   *                IC_INPUTCAP_OFF       // Input Capture OFF
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Conditions of failure : wrong argument.
   */
  INT8 (*Open)  (InpCapModule_t inpCapModuleId, UINT16 config);


  /**
   * @Prototype     static inline INT8 InputCaptureGetDirection (InpCapModule_t inpCap1, InpCapModule_t inpCap2, INT64 timeBetweenCaptures, TimerScale_t scale);
   *
   * @Description   This function will do some process between the two Input
   *                Capture module you put in parameters and return the first
   *                of the two in which an event occured. It also assumes that
   *                the two Input Capture modules are dephased by 90 degrees.
   *                The function assumes that the modules use the same Timer.
   *
   * @Example       delay = InputCapture.GetTimeBetweenCaptures(IC2, SCALE_US);\n
   *                err = InputCapture.GetDirection(IC2, IC4, delay, SCALE_US);\n
   *                This example will return the first input capture to read an
   *                event between IC2 and IC4.
   *
   * @Parameter     InpCapModule_t inpCap1, inpCap2\n
   *                Specifies which module this function addresses.\n
   *                Possibles values : \n
   *                IC1, IC2, IC3, IC4, IC5.
   *
   * @Parameter     INT64 timeBetweenCaptures\n
   *                The value returned by InputCapture.GetTimeBetweenCaptures().
   *
   * @Parameter     TimerScale_t scale\n
   *                The timer scale used by timeBetweenCaptures.\n
   *                Possibles values : \n
   *                SCALE_S, SCALE_MS, SCALE_US, SCALE_NS.
   *
   * @Returns       First Input Capture module on success, -1 on failure.\n
   *                Conditions of failure : wrong argument.
   */
  INT8 (*GetDirection) (InpCapModule_t inpCap1, InpCapModule_t inpCap2, INT64 timeBetweenCaptures, TimerScale_t scale);


  /**
   * @Prototype     static inline INT8 InputCaptureClose (InpCapModule_t inpCapModuleId);
   *
   * @Description   This function closes the Input Capture module specified.
   *
   * @Example       err = InputCapture.Close(IC2);\n
   *                This example closes the Input Capture 2.
   *
   * @Parameter     InpCapModule_t inpCapModuleId\n
   *                Specifies which module this function addresses.\n
   *                Possibles values : \n
   *                IC1, IC2, IC3, IC4, IC5.
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Conditions of failure : wrong argument.
   */
  INT8 (*Close) (InpCapModule_t inpCapModuleId);


  /**
   * @Prototype     static inline INT8 InputCaptureConfigInterrupt (InpCapModule_t inpCapModuleId, UINT16 intPriority, UINT16 intSubPriority);
   *
   * @Description   This function configures the Input Capture interrupt of
   *                the module specified WITHOUT enabling it. To enable the
   *                interrupt, use InputCapture.EnableInterrupt function.
   *
   * @Example       err = InputCapture.ConfigInterrupt(IC2, 1, 0);\n
   *                This example configures the interrupt of the Input Capture 2
   *                with a priority of 1 and subpriority of 0.
   *
   * @Parameter     InpCapModule_t inpCapModuleId\n
   *                Specifies which module this function addresses.\n
   *                Possibles values : \n
   *                IC1, IC2, IC3, IC4, IC5.
   *
   * @Parameter     UINT16 intPriority\n
   *                Specifies the priority of the interrupt.\n
   *                Possibles values : \n
   *                0 (interrupt disabled)\n
   *                1 (lowest priority)\n
   *                2-6\n
   *                7 (highest priority)
   *
   * @Parameter     UINT8 intSubPriority\n
   *                subPriority is the desired subpriority of the interrupt to set.
   *                Its values have a range from 0 to 3, three (3) being the highest
   *                priority and zero (0) being the lowest priority.
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Conditions of failure : wrong arguments.
   */
  INT8 (*ConfigInterrupt) (InpCapModule_t inpCapModuleId, UINT16 intPriority, UINT16 intSubPriority);


  /**
   * @Prototype     static inline INT8 InputCaptureDisableInterrupt (InpCapModule_t inpCapModuleId);
   *
   * @Description   This function disables the Input Capture interrupt of
   *                the module specified.
   *
   * @Example       err = InputCapture.DisableInterrupt(IC2);\n
   *                This example disables the interrupt of the Input Capture 2.
   *
   * @Parameter     InpCapModule_t inpCapModuleId\n
   *                Specifies which module this function addresses.\n
   *                Possibles values : \n
   *                IC1, IC2, IC3, IC4, IC5.
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Conditions of failure : wrong arguments.
   */
  INT8 (*DisableInterrupt) (InpCapModule_t inpCapModuleId);


  /**
   * @Prototype     static inline INT8 InputCaptureEnableInterrupt (InpCapModule_t inpCapModuleId);
   *
   * @Description   This function enables the Input Capture interrupt of
   *                the module specified.
   *
   * @Example       err = InputCapture.EnableInterrupt(IC2);\n
   *                This example enables the interrupt of the Input Capture 2.
   *
   * @Parameter     InpCapModule_t inpCapModuleId\n
   *                Specifies which module this function addresses.\n
   *                Possibles values : \n
   *                IC1, IC2, IC3, IC4, IC5.
   *
   * @Returns       0 on success, -1 on failure.\n
   *                Conditions of failure : wrong arguments.
   */
  INT8 (*EnableInterrupt) (InpCapModule_t inpCapModuleId);


  /**
   * @Prototype     static inline INT8 InputCaptureIsCaptureReady (InpCapModule_t inpCapModuleId);
   *
   * @Description   This function returns 0 or 1 is one or more event is captured.
   *
   * @Example       err = InputCapture.IsCaptureReady(IC2);\n
   *                This example returns the state of the buffer.
   *
   * @Parameter     InpCapModule_t inpCapModuleId\n
   *                Specifies which module this function addresses.\n
   *                Possibles values : \n
   *                IC1, IC2, IC3, IC4, IC5.
   *
   * @Returns       1 if there is a captured data\n
   *                0 if there is no capture event\n
   *                -1 on failure.\n
   *                Conditions of failure : wrong arguments.
   */
  INT8 (*IsCaptureReady) (InpCapModule_t inpCapModuleId);


  /**
   * @Prototype     static inline INT32 InputCaptureReadCapture (InpCapModule_t inpCapModuleId);
   *
   * @Description   This function returns the timer value captured in COUNTS 
   *                (NOT in time). One must use InputCapture.IsCaptureReady
   *                before using this function.
   *
   * @Example       InputCapture.ReadCapture(IC2);\n
   *                This example returns the captured timer value associated with
   *                Input Capture module 2.
   *
   * @Parameter     InpCapModule_t inpCapModuleId\n
   *                Specifies which module this function addresses.\n
   *                Possibles values : \n
   *                IC1, IC2, IC3, IC4, IC5.
   *
   * @Returns       INT32. The value of the timer captured\n
   *                -1 on failure.\n
   *                Conditions of failure : wrong arguments.
   */
  INT32 (*ReadCapture) (InpCapModule_t inpCapModuleId);


  /**
   * @Prototype     static inline INT64 InputCaptureGetTimeBetweenCaptures (InpCapModule_t inpCapModuleId, TimerScale_t deltaScale);
   *
   * @Description   This function returns the time difference between the last 2
   *                captures. It is assumed that the variables necessary are
   *                updated in the interrupts. The variables to update are as
   *                follows : \n
   *                InputCapture.Var.previousTimerOverflows[inpCapModuleId]\n
   *                InputCapture.Var.currentTimerOverflows[inpCapModuleId]\n
   *                InputCapture.Var.previousCaptureCountValue[inpCapModuleId]\n
   *                InputCapture.Var.currentCaptureCountValue[inpCapModuleId]
   *
   * @Example       UINT32 captureSpeed = InputCapture.GetTimeBetweenCaptures(IC2);\n
   *                This example returns the time between the last  timer value associated with
   *                Input Capture module 2.
   *
   * @Parameter     InpCapModule_t inpCapModuleId\n
   *                Specifies which module this function addresses.\n
   *                Possibles values : \n
   *                IC1, IC2, IC3, IC4, IC5.
   *
   * @Returns       INT64\n
   *                On success : the value of the timer captured\n
   *                -1 on failure.\n
   *                Conditions of failure : wrong arguments.
   */
  INT64 (*GetTimeBetweenCaptures) (InpCapModule_t inpCapModuleId, TimerScale_t deltaScale);


  /**
   * @Description   Contains certains variables useful for the Input Capture.
   *                Write a dot "." after writing "Var" to see a description of
   *                the variables available.
   */
  struct sChinookInputCaptureVar Var;

};



#endif	/* __CHINOOK_INPUT_CAPTURE__ */

