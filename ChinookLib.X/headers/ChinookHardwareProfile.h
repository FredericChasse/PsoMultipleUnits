/*
 * File:   ChinookHardwareProfile.h
 * Author: Frederic
 *
 * Created on 15 octobre 2014, 22:05
 */

#ifndef __CHINOOK_HARDWARE_PROFILE__
#define	__CHINOOK_HARDWARE_PROFILE__

#include <string.h>
#include <stdio.h>

#define SYS_FREQ (80000000L)

#define	GetPeripheralClock()		(SYS_FREQ/(1 << OSCCONbits.PBDIV))
#define	GetInstructionClock()		(SYS_FREQ)
#define	GetSystemClock()        (SYS_FREQ)
#define CORE_TICK_RATE          (SYS_FREQ/2)


#endif	/* __CHINOOK_HARDWARE_PROFILE__ */