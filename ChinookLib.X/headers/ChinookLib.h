/* 
 * File:   ChinookLib.h
 * Author: Frederic
 *
 * Created on 15 octobre 2014, 22:05
 */

#ifndef __CHINOOKLIB__
#define	__CHINOOKLIB__

#include "ChinookHardwareProfile.h"

#include "ChinookAdc.h"
#include "ChinookCan.h"
#include "ChinookI2c.h"
//#include "ChinookInputCapture.h"
#include "ChinookPorts.h"
//#include "ChinookPwm.h"
#include "ChinookSkadi.h"
#include "ChinookSpi.h"
#include "ChinookTimer.h"
#include "ChinookUart.h"
#include "ChinookWdt.h"

extern struct sChinookAdc   Adc;

//#ifndef __32MX320F128H__    // Uno32 doesn't CAN
//extern struct sChinookCan   Can;
//#endif
extern struct sChinookI2c   I2c;
//extern struct sChinookInputCapture   InputCapture;
extern struct sChinookPort  Port;
//extern struct sChinookPwm   Pwm;
extern struct sChinookSkadi Skadi;
extern struct sChinookSpi   Spi;
extern struct sChinookTimer Timer;
extern struct sChinookUart  Uart;
extern struct sChinookWdt   Wdt;


#ifndef __32MX320F128H__    // Uno32 doesn't CAN
struct sChinook
{
  struct sChinookAdc   Adc;
//  struct sChinookCan   Can;
  struct sChinookI2c   I2c;
//  struct sChinookInputCapture   InputCapture;
  struct sChinookPort  Port;
//  struct sChinookPwm   Pwm;
  struct sChinookSkadi Skadi;
  struct sChinookSpi   Spi;
  struct sChinookTimer Timer;
  struct sChinookUart  Uart;
  struct sChinookWdt   Wdt;
};
#else
struct sChinook
{
  struct sChinookAdc   Adc;
  struct sChinookI2c   I2c;
//  struct sChinookInputCapture   InputCapture;
  struct sChinookPort  Port;
//  struct sChinookPwm   Pwm;
  struct sChinookSkadi Skadi;
  struct sChinookSpi   Spi;
  struct sChinookTimer Timer;
  struct sChinookUart  Uart;
  struct sChinookWdt   Wdt;
};
#endif


#endif	/* __CHINOOKLIB__ */

