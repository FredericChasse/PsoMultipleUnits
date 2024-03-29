//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Interrupts.h
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


#ifndef __INTERRUPT_H__
#define	__INTERRUPT_H__

#include "Setup.h"

//==============================================================================
// DEFINITIONS
//==============================================================================

/*
 *  ==========================================
 *            Table of interrupts
 *  ==========================================
 * 
 * +--------------------------------------------+
 * | Priority | Subpriority | Interrupt handler |
 * |----------+-------------+-------------------|
 * | 1        | 0           | Timer 5           |
 * | 1        | 1           |                   |
 * | 1        | 2           | Timer 4           |
 * | 1        | 3           |                   |
 * |----------+-------------+-------------------|
 * | 2        | 0           |                   |
 * | 2        | 1           | Timer 2           |
 * | 2        | 2           |                   |
 * | 2        | 3           | Timer 1           |
 * |----------+-------------+-------------------|
 * | 3        | 0           | UART 6            |
 * | 3        | 1           |                   |
 * | 3        | 2           |                   |
 * | 3        | 3           |                   |
 * |----------+-------------+-------------------|
 * | 4        | 0           |                   |
 * | 4        | 1           |                   |
 * | 4        | 2           | UART 3            |
 * | 4        | 3           |                   |
 * |----------+-------------+-------------------|
 * | 5        | 0           |                   |
 * | 5        | 1           |                   |
 * | 5        | 2           | I2C 5             |
 * | 5        | 3           |                   |
 * |----------+-------------+-------------------|
 * | 6        | 0           | SPI 3             |
 * | 6        | 1           |                   |
 * | 6        | 2           |                   |
 * | 6        | 3           | ADC               |
 * |----------+-------------+-------------------|
 * | 7        | 0           |                   |
 * | 7        | 1           |                   |
 * | 7        | 2           |                   |
 * | 7        | 3           | Timer 3           |
 * +----------+-------------+-------------------+
 * 
 */

//===============================================
// Timer interrupts priorities and subpriorities
//===============================================
#define TIMER1_INTERRUPT_PRIORITY       2         // Used in ChinookLib function
#define T1_INTERRUPT_PRIORITY           ipl2auto  // Used in ISR
#define TIMER2_INTERRUPT_PRIORITY       2         // Used in ChinookLib function
#define T2_INTERRUPT_PRIORITY           ipl2auto  // Used in ISR
#define TIMER3_INTERRUPT_PRIORITY       7         // Used in ChinookLib function
#define T3_INTERRUPT_PRIORITY           ipl7auto  // Used in ISR
#define TIMER4_INTERRUPT_PRIORITY       1         // Used in ChinookLib function
#define T4_INTERRUPT_PRIORITY           ipl1auto  // Used in ISR
#define TIMER5_INTERRUPT_PRIORITY       1         // Used in ChinookLib function
#define T5_INTERRUPT_PRIORITY           ipl1auto  // Used in ISR

#define TIMER1_INTERRUPT_SUBPRIORITY    3         // Highest subpriority
#define TIMER2_INTERRUPT_SUBPRIORITY    1
#define TIMER3_INTERRUPT_SUBPRIORITY    3         // Highest subpriority
#define TIMER4_INTERRUPT_SUBPRIORITY    2
#define TIMER5_INTERRUPT_SUBPRIORITY    0         // Lowest subpriority
//===============================================


//===============================================
// UART interrupts priorities and subpriorities
//===============================================
#define UART3_INTERRUPT_PRIORITY        4           // Used in ChinookLib function
#define U3_INTERRUPT_PRIORITY           ipl4auto    // Used in ISR
#define UART6_INTERRUPT_PRIORITY        3           // Used in ChinookLib function
#define U6_INTERRUPT_PRIORITY           ipl3auto    // Used in ISR

#define UART3_INTERRUPT_SUBPRIORITY     2           // Highest subpriority
#define UART6_INTERRUPT_SUBPRIORITY     0           // Lowest subpriority
//===============================================


//===============================================
// SPI interrupts priorities and subpriorities
//===============================================
#define SPI3_INTERRUPT_PRIORITY         6           // Used in ChinookLib function
#define S3_INTERRUPT_PRIORITY           ipl6auto    // Used in ISR

#define SPI3_INTERRUPT_SUBPRIORITY      0           // Highest subpriority
//===============================================


//===============================================
// ADC interrupts priorities and subpriorities
//===============================================
#define ADC_INTERRUPT_PRIORITY          6           // Used in ChinookLib function
#define ADC_INTERRUPT_PRIO              ipl6auto    // Used in ISR

#define ADC_INTERRUPT_SUBPRIORITY       3           // Highest subpriority
//===============================================


//===============================================
// I2C interrupts priorities and subpriorities
//===============================================
#define I2C5_INTERRUPT_PRIORITY          5           // Used in ChinookLib function
#define I2C5_INT_PRIORITY                ipl5auto    // Used in ISR

#define I2C5_INTERRUPT_SUBPRIORITY       2
//===============================================


//==============================================================================
// VARIABLES
//==============================================================================


#endif	/* __INTERRUPT_H__ */

