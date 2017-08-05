//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// ChinookLib
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : ChinookWdt.c
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

#include "..\headers\ChinookWdt.h"


//==============================================================================
// Private functions prototypes
//==============================================================================
static inline void    WdtClear          (void);
static inline void    WdtClearEvent     (void);
static inline void    WdtDisable        (void);
static inline void    WdtEnable         (void);
//static inline void    WdtOpen           (WdtPostScaler_t wdtPs, BOOL oEnableWdt);
static inline BOOL    WdtReadEvent      (void);
static inline UINT16  WdtReadPostScaler (void);


//==============================================================================
// Variable definitions
//==============================================================================

//==============================================================================
// Public structure of functions for the UART
//==============================================================================
struct sChinookWdt Wdt =
{
   .Clear           = &WdtClear
  ,.ClearEvent      = &WdtClearEvent
  ,.Disable         = &WdtDisable
  ,.Enable          = &WdtEnable
//  ,.Open            = &WdtOpen
  ,.ReadEvent       = &WdtReadEvent
  ,.ReadPostScaler  = &WdtReadPostScaler
};

//==============================================================================
// Private functions declarations
//==============================================================================

//static inline void WdtOpen (WdtPostScaler_t wdtPs, BOOL oEnableWdt)
//{
//  Wdt.Disable();
//  
//  WDTCONbits.WDTPSTA = wdtPs;
//  WDTCONbits.WDTPS = wdtPs;   // Set the postscaler for the WDT
////  #pragma config WDTPS      = PS1024
//  
//  if (oEnableWdt)
//  {
//    Wdt.Enable();
//  }
//  else
//  {
//    Wdt.Disable();
//  }
//}


static inline void WdtDisable (void)
{
  DisableWDT();
}


static inline void WdtEnable (void)
{
  EnableWDT();
}


static inline void WdtClear (void)
{
  ClearWDT();
}


static inline void WdtClearEvent (void)
{
  ClearEventWDT();
}


static inline BOOL WdtReadEvent (void)
{
  return ReadEventWDT();
}


static inline UINT16 WdtReadPostScaler (void)
{
  return ReadPostscalerWDT();
}
