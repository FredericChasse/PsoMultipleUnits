//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : HeaderTemplate.h
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This header file gathers the functions used for Skadi.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : Function names can and should be renamed by the user to improve the
//           readability of the code.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#ifndef __SKADI_FUNCTIONS__
#define	__SKADI_FUNCTIONS__

#include "Setup.h"


//==============================================================================
// Public functions prototypes
//==============================================================================

/**************************************************************
 * Function name  : LedToggle
 * Purpose        : Toggle Led 1 or 2 depending on the arguments
 * Arguments      : Received from Skadi functions
 * Returns        : None.
 *************************************************************/
void LedToggle();

/**************************************************************
 * Function name  : SetPwmSkadi
 * Purpose        : Set the duty cycle of a LED
 * Arguments      : 1 : LED num (0 - 15), 2 : duty cycle * 10
 * Returns        : None.
 *************************************************************/
void SetPwmSkadi();

/**************************************************************
 * Function name  : SetAllPwmSkadi
 * Purpose        : Set the duty cycle of all LEDs
 * Arguments      : 1 : duty cycle * 10
 * Returns        : None.
 *************************************************************/
void SetAllPwmSkadi();

/**************************************************************
 * Function name  : SetPotSkadi
 * Purpose        : Set the value of a potentiometer
 * Arguments      : 1 : Pot num (0 - 3), 2 : index (0 - 3), 3 : pot value (0 - 255)
 * Returns        : None.
 *************************************************************/
void SetPotSkadi();

/**************************************************************
 * Function name  : SetAllPotSkadi
 * Purpose        : Set the value of a potentiometer
 * Arguments      : 1 : Pot num (0 - 3), 2 : pot value (0 - 255)
 * Returns        : None.
 *************************************************************/
void SetAllPotSkadi();

/**************************************************************
 * Function name  : ClearScreen
 * Purpose        : Clear the terminal window
 * Arguments      : None.
 * Returns        : None.
 *************************************************************/
void ClearScreen();

//==============================================================================
// Macro definitions
//==============================================================================


//==============================================================================
// Variable declarations
//==============================================================================

#define N_SKADI_CMDS    (6)

/***********************************
 * Table of functions used in Skadi
 **********************************/
extern const sSkadiCommand_t skadiCommandTable[N_SKADI_CMDS];


#endif	/* __SKADI_FUNCTIONS__ */

