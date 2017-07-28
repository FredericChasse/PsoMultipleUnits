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
#include "..\..\..\ChinookLib\ChinookLib.X\headers\ChinookLib.h"

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

#define LED1_ON       Port.F.ClearBits  (BIT_0)   
#define LED2_ON       Port.F.ClearBits  (BIT_1)   
#define LED1_OFF      Port.F.SetBits    (BIT_0)     
#define LED2_OFF      Port.F.SetBits    (BIT_1)     
#define LED1_TOGGLE   Port.F.ToggleBits (BIT_0)  
#define LED2_TOGGLE   Port.F.ToggleBits (BIT_1)  

#define U_MATLAB      UART6
#define U_DBG         UART3

#define SW1           PORTEbits.RE5
#define SW2           PORTEbits.RE6
#define SW3           PORTEbits.RE7

#define PI            (3.14159265359f)

#define N_UNITS_TOTAL (15u)

#define ADC_TIMER_PERIOD        (10)
#define ADC_TIMER_SCALE         (SCALE_US)
#define ADC_TIMER_SCALE_FLOAT   (TIMER_SCALE_US)
#define N_SAMPLES_PER_ADC_READ  (456)     // Number of samples / ADC value
#define N_SAMPLES_TO_START      (400)
#define N_SAMPLES_TO_DROP       (200)      // Ignore n first samples
#define N_TOTAL_SAMPLES         (N_SAMPLES_PER_ADC_READ - N_SAMPLES_TO_DROP)

#define SAMPLING_TIME_FLOAT     (ADC_TIMER_SCALE_FLOAT * ADC_TIMER_PERIOD * N_SAMPLES_PER_ADC_READ * N_UNITS_TOTAL)

#endif	/* __SETUP_H__ */