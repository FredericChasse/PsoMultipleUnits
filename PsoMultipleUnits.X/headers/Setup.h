//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Setup.h
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the setup header for the system. It contains function
//           prototypes as well as macros and the public structure of
//           ChinookLib functions.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : For ChinookLib to be useable, it must be cloned on your hard
//           drive so the path
//               "..\..\..\ChinookLib\ChinookLib.X\headers\ChinookLib.h"
//           references an existing file.
//
//           Function names can and should be renamed by the user to improve the
//           readability of the code.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#ifndef __SETUP_H__
#define	__SETUP_H__

//%%%%%%%%%%%%%%%%%%%%%%%%%
// ChinookLib files
//%%%%%%%%%%%%%%%%%%%%%%%%%
#include "..\..\ChinookLib.X\headers\ChinookLib.h"

//==============================================================================
// ChinookLib functions
//==============================================================================
struct sChinook Chinook;            // Contains all ChinookLib functions


//==============================================================================
// Skadi functions
//==============================================================================
//extern const size_t lengthSkadiCommandTable;
//extern sSkadiCommand_t skadiCommandTable[];

//==============================================================================
// State Machine public functions prototypes
//==============================================================================
void InitTimer        (void);
void InitPorts        (void);
void InitSkadi        (void);
void InitSpi          (void);
void InitUart         (void);
void InitI2c          (void);
void InitWdt          (void);
void InitAdc          (void);
void StartInterrupts  (void);


//==============================================================================
// Macro definitions
//==============================================================================

#define LED1_ON()       Port.F.ClearBits  (BIT_0)   
#define LED2_ON()       Port.F.ClearBits  (BIT_1)   
#define LED1_OFF()      Port.F.SetBits    (BIT_0)     
#define LED2_OFF()      Port.F.SetBits    (BIT_1)     
#define LED1_TOGGLE()   Port.F.ToggleBits (BIT_0)  
#define LED2_TOGGLE()   Port.F.ToggleBits (BIT_1)  

#define DBG0_ON()       Port.C.SetBits(BIT_13)    // GPIO0 RC13
#define DBG1_ON()       Port.C.SetBits(BIT_14)    // GPIO1 RC14
#define DBG2_ON()       Port.E.SetBits(BIT_4)     // GPIO2 RE4
#define DBG0_OFF()      Port.C.ClearBits(BIT_13)  // GPIO0 RC13
#define DBG1_OFF()      Port.C.ClearBits(BIT_14)  // GPIO1 RC14
#define DBG2_OFF()      Port.E.ClearBits(BIT_4)   // GPIO2 RE4

#define U_MATLAB      UART6
#define U_DBG         UART3

#ifdef ONLY_2_UART_BUFFERS
#define U_MATLAB_IDX  (1)
#define U_DBG_IDX     (0)
#else
#define U_MATLAB_IDX  (U_MATLAB)
#define U_DBG_IDX     (U_DBG)
#endif

#define SW1           PORTEbits.RE5
#define SW2           PORTEbits.RE6
#define SW3           PORTEbits.RE7

#define PI            (3.14159265359f)

#define N_UNITS_TOTAL (12u)

#define ADC_TIMER_PERIOD          (100)
#define ADC_TIMER_SCALE           (SCALE_MS)
#define ADC_TIMER_SCALE_FLOAT     (TIMER_SCALE_MS)
#define N_SAMPLES_PER_ADC_READ    (10)     // Number of samples / ADC value
#define N_SAMPLES_TO_DROP         (0)      // Ignore n first samples
#define N_TOTAL_SAMPLES           (N_SAMPLES_PER_ADC_READ - N_SAMPLES_TO_DROP)

//#define N_LARGE_SAMPLES_FOR_ALGO  (1200)    // 12 * 0.025 * 10 = 3 seconds; 3 * 1200 = 3600 seconds = 1 hour
#define N_LARGE_SAMPLES_FOR_ALGO  (225)    // 12 * 0.025 * 10 = 3 seconds; 3 * 4 = 12 seconds

#define SAMPLING_TIME_FLOAT       (ADC_TIMER_SCALE_FLOAT * ADC_TIMER_PERIOD * N_SAMPLES_PER_ADC_READ * N_UNITS_TOTAL)
#define LARGE_SAMPLING_TIME_FLOAT (ADC_TIMER_SCALE_FLOAT * ADC_TIMER_PERIOD * N_SAMPLES_PER_ADC_READ * N_UNITS_TOTAL * N_LARGE_SAMPLES_FOR_ALGO)

#endif	/* __SETUP_H__ */