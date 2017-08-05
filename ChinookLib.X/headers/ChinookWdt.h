//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// ChinookLib
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : ChinookWdt.h
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


#ifndef __CHINOOK_WDT__
#define	__CHINOOK_WDT__

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
//typedef enum WdtPostScaler_t
//{
//  WDT_PS_1_1        =   0b00000     // Time-out of 1 ms
// ,WDT_PS_1_2        =   0b00001     // Time-out of 2 ms
// ,WDT_PS_1_4        =   0b00010     // Time-out of 4 ms
// ,WDT_PS_1_8        =   0b00011     // Time-out of 8 ms
// ,WDT_PS_1_16       =   0b00100     // Time-out of 16 ms
// ,WDT_PS_1_32       =   0b00101     // Time-out of 32 ms
// ,WDT_PS_1_64       =   0b00110     // Time-out of 64 ms
// ,WDT_PS_1_128      =   0b00111     // Time-out of 128 ms
// ,WDT_PS_1_256      =   0b01000     // Time-out of 256 ms
// ,WDT_PS_1_512      =   0b01001     // Time-out of 512 ms
// ,WDT_PS_1_1024     =   0b01010     // Time-out of 1.024 s
// ,WDT_PS_1_2048     =   0b01011     // Time-out of 2.048 s
// ,WDT_PS_1_4096     =   0b01100     // Time-out of 4.096 s
// ,WDT_PS_1_8192     =   0b01101     // Time-out of 8.192 s
// ,WDT_PS_1_16384    =   0b01110     // Time-out of 16.384 s
// ,WDT_PS_1_32768    =   0b01111     // Time-out of 32.768 s
// ,WDT_PS_1_65536    =   0b10000     // Time-out of 65.536 s
// ,WDT_PS_1_131072   =   0b10001     // Time-out of 131.072 s
// ,WDT_PS_1_262144   =   0b10010     // Time-out of 262.144 s
// ,WDT_PS_1_524288   =   0b10011     // Time-out of 524.288 s
// ,WDT_PS_1_1045876  =   0b10100     // Time-out of 1048.576 s
//} WdtPostScaler_t;

//==============================================================================
// Public structure declaration
//==============================================================================

struct sChinookWdt
{

  /**
//   * @Prototype     static inline void WdtOpen(WdtPostScaler_t wdtPs, BOOL oEnableWdt);
//   *
//   * @Description   This function sets the WDT at the post-scaler specified, which 
//   *                tells the max counter value of the WDT.
//   *
//   * @Example       Wdt.Open(WDT_PS_1_1, TRUE);
//   *                This example sets the period of the WDT to 1 ms and enables it.
//   *
//   * @Parameter     WdtPostScaler_t wdtPs\n
//   *                Specifies the PostScaler value to use.\n
//   *                Possible values : \n
//   *                WDT_PS_1_1        // Time-out of 1 ms\n
//   *                WDT_PS_1_2        // Time-out of 2 ms\n
//   *                WDT_PS_1_4        // Time-out of 4 ms\n
//   *                WDT_PS_1_8        // Time-out of 8 ms\n
//   *                WDT_PS_1_16       // Time-out of 16 ms\n
//   *                WDT_PS_1_32       // Time-out of 32 ms\n
//   *                WDT_PS_1_64       // Time-out of 64 ms\n
//   *                WDT_PS_1_128      // Time-out of 128 ms\n
//   *                WDT_PS_1_256      // Time-out of 256 ms\n
//   *                WDT_PS_1_512      // Time-out of 512 ms\n
//   *                WDT_PS_1_1024     // Time-out of 1.024 s\n
//   *                WDT_PS_1_2048     // Time-out of 2.048 s\n
//   *                WDT_PS_1_4096     // Time-out of 4.096 s\n
//   *                WDT_PS_1_8192     // Time-out of 8.192 s\n
//   *                WDT_PS_1_16384    // Time-out of 16.384 s\n
//   *                WDT_PS_1_32768    // Time-out of 32.768 s\n
//   *                WDT_PS_1_65536    // Time-out of 65.536 s\n
//   *                WDT_PS_1_131072   // Time-out of 131.072 s\n
//   *                WDT_PS_1_262144   // Time-out of 262.144 s\n
//   *                WDT_PS_1_524288   // Time-out of 524.288 s\n
//   *                WDT_PS_1_1045876  // Time-out of 1048.576 s
//   * 
//   * @Parameter     BOOL oEnableWdt\n
//   *                Enable or disable the WDT.\n
//   *                Possible values : \n
//   *                TRUE : enable the WDT\n
//   *                FALSE : disable the WDT.
//   *
//   * @Returns       None.
//   */
//  void (*Open)  (WdtPostScaler_t wdtPs, BOOL oEnableWdt);
  

  /**
   * @Prototype     static inline void WdtEnable(void);
   * 
   * @Note          IMPORTANT : To change the time-out value of the WDT, the 
   *                configuration bit WDTPS (#pragma config WDTPS) must be adjusted.\n
   *                Possible values : \n
   *                PS1        // Time-out of 1 ms\n
   *                PS2        // Time-out of 2 ms\n
   *                PS4        // Time-out of 4 ms\n
   *                PS8        // Time-out of 8 ms\n
   *                PS16       // Time-out of 16 ms\n
   *                PS32       // Time-out of 32 ms\n
   *                PS64       // Time-out of 64 ms\n
   *                PS128      // Time-out of 128 ms\n
   *                PS256      // Time-out of 256 ms\n
   *                PS512      // Time-out of 512 ms\n
   *                PS1024     // Time-out of 1.024 s\n
   *                PS2048     // Time-out of 2.048 s\n
   *                PS4096     // Time-out of 4.096 s\n
   *                PS8192     // Time-out of 8.192 s\n
   *                PS16384    // Time-out of 16.384 s\n
   *                PS32768    // Time-out of 32.768 s\n
   *                PS65536    // Time-out of 65.536 s\n
   *                PS131072   // Time-out of 131.072 s\n
   *                PS262144   // Time-out of 262.144 s\n
   *                PS524288   // Time-out of 524.288 s\n
   *                PS1045876  // Time-out of 1048.576 s
   *
   * @Description   This function enables the WatchDog Timer.
   *
   * @Example       Wdt.Enable();\n
   *                This example will enable the WDT.
   *
   * @Parameter     None.
   *
   * @Returns       None.
   */
  void (*Enable)  (void);
  

  /**
   * @Prototype     static inline void WdtDisable(void);
   *
   * @Description   This function disables the WatchDog Timer.
   *
   * @Example       Wdt.Disable();\n
   *                This example will disable the WDT.
   *
   * @Parameter     None.
   *
   * @Returns       None.
   */
  void (*Disable)  (void);
  

  /**
   * @Prototype     static inline void WdtClear(void);
   *
   * @Description   This function clears the timer counter for the WDT, preventing
   *                a system reset.
   *
   * @Example       Wdt.Clear();\n
   *                This example clears the counter of the WDT.
   *
   * @Parameter     None.
   *
   * @Returns       None.
   */
  void (*Clear)  (void);
  

  /**
   * @Prototype     static inline void WdtClearEvent(void);
   *
   * @Description   This function clears the event bit that indicates if a system 
   *                reset has occured.
   *
   * @Example       Wdt.ClearEvent();\n
   *                This example clears the event bit of the WDT.
   *
   * @Parameter     None.
   *
   * @Returns       None.
   */
  void (*ClearEvent)  (void);
  

  /**
   * @Prototype     static inline BOOL WdtReadEvent(void);
   *
   * @Description   This function returns the "event occured" bit. If the bit is 
   *                set (TRUE or 1), a system reset has occured. If it's cleared 
   *                (FALSE or 0), no system reset have occured.
   *
   * @Example       BOOL oHasEventOccured = Wdt.ReadEvent();\n
   *                This example returns the event status bit of the WDT.
   *
   * @Parameter     None.
   *
   * @Returns       BOOL\n
   *                If TRUE (1) : a system reset has occured\n
   *                If FALSE (0) : no system reset have occured.
   */
  BOOL (*ReadEvent)  (void);
  

  /**
   * @Prototype     static inline UINT16 WdtReadPostScaler(void);
   *
   * @Description   This function returns the PostScaler value or the WDT.
   *
   * @Example       UINT16 wdtPostScaler = Wdt.ReadPostScaler();\n
   *                This example returns the PostScaler of the WDT.
   *
   * @Parameter     None.
   *
   * @Returns       UINT16\n
   *                The value of the PostScaler.
   */
  UINT16 (*ReadPostScaler)  (void);

};



#endif	/* __CHINOOK_WDT__ */

