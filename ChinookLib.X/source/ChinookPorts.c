#include "..\headers\ChinookPorts.h"


//==============================================================================
// Private functions prototypes for the I/0 Ports
//==============================================================================
static inline INT8 PortASetBits(UINT16 bits);
static inline INT8 PortBSetBits(UINT16 bits);
static inline INT8 PortCSetBits(UINT16 bits);
static inline INT8 PortDSetBits(UINT16 bits);
static inline INT8 PortESetBits(UINT16 bits);
static inline INT8 PortFSetBits(UINT16 bits);
static inline INT8 PortGSetBits(UINT16 bits);

static inline INT8 PortAClearBits(UINT16 bits);
static inline INT8 PortBClearBits(UINT16 bits);
static inline INT8 PortCClearBits(UINT16 bits);
static inline INT8 PortDClearBits(UINT16 bits);
static inline INT8 PortEClearBits(UINT16 bits);
static inline INT8 PortFClearBits(UINT16 bits);
static inline INT8 PortGClearBits(UINT16 bits);

static inline INT8 PortAToggleBits(UINT16 bits);
static inline INT8 PortBToggleBits(UINT16 bits);
static inline INT8 PortCToggleBits(UINT16 bits);
static inline INT8 PortDToggleBits(UINT16 bits);
static inline INT8 PortEToggleBits(UINT16 bits);
static inline INT8 PortFToggleBits(UINT16 bits);
static inline INT8 PortGToggleBits(UINT16 bits);

static inline INT8 PortASetPinsDigitalOut(UINT16 bits);
static inline INT8 PortBSetPinsDigitalOut(UINT16 bits);
static inline INT8 PortCSetPinsDigitalOut(UINT16 bits);
static inline INT8 PortDSetPinsDigitalOut(UINT16 bits);
static inline INT8 PortESetPinsDigitalOut(UINT16 bits);
static inline INT8 PortFSetPinsDigitalOut(UINT16 bits);
static inline INT8 PortGSetPinsDigitalOut(UINT16 bits);

static inline INT8 PortASetPinsDigitalIn(UINT16 bits);
static inline INT8 PortBSetPinsDigitalIn(UINT16 bits);
static inline INT8 PortCSetPinsDigitalIn(UINT16 bits);
static inline INT8 PortDSetPinsDigitalIn(UINT16 bits);
static inline INT8 PortESetPinsDigitalIn(UINT16 bits);
static inline INT8 PortFSetPinsDigitalIn(UINT16 bits);
static inline INT8 PortGSetPinsDigitalIn(UINT16 bits);

static inline INT8 PortACloseBits(UINT16 bits);
static inline INT8 PortBCloseBits(UINT16 bits);
static inline INT8 PortCCloseBits(UINT16 bits);
static inline INT8 PortDCloseBits(UINT16 bits);
static inline INT8 PortECloseBits(UINT16 bits);
static inline INT8 PortFCloseBits(UINT16 bits);
static inline INT8 PortGCloseBits(UINT16 bits);

static inline INT8 PortASetPinsAnalogIn(UINT16 bits);   // Invalid, returns -1
static inline INT8 PortBSetPinsAnalogIn(UINT16 bits);
static inline INT8 PortCSetPinsAnalogIn(UINT16 bits);   // Invalid, returns -1
static inline INT8 PortDSetPinsAnalogIn(UINT16 bits);   // Invalid, returns -1
static inline INT8 PortESetPinsAnalogIn(UINT16 bits);   // Invalid, returns -1
static inline INT8 PortFSetPinsAnalogIn(UINT16 bits);   // Invalid, returns -1
static inline INT8 PortGSetPinsAnalogIn(UINT16 bits);   // Invalid, returns -1

static inline UINT16 PortAReadPort(void);
static inline UINT16 PortBReadPort(void);
static inline UINT16 PortCReadPort(void);
static inline UINT16 PortDReadPort(void);
static inline UINT16 PortEReadPort(void);
static inline UINT16 PortFReadPort(void);
static inline UINT16 PortGReadPort(void);

static inline INT32 PortAReadBits(UINT16 bits);
static inline INT32 PortBReadBits(UINT16 bits);
static inline INT32 PortCReadBits(UINT16 bits);
static inline INT32 PortDReadBits(UINT16 bits);
static inline INT32 PortEReadBits(UINT16 bits);
static inline INT32 PortFReadBits(UINT16 bits);
static inline INT32 PortGReadBits(UINT16 bits);

static inline void PortAWritePort(UINT16 bits);
static inline void PortBWritePort(UINT16 bits);
static inline void PortCWritePort(UINT16 bits);
static inline void PortDWritePort(UINT16 bits);
static inline void PortEWritePort(UINT16 bits);
static inline void PortFWritePort(UINT16 bits);
static inline void PortGWritePort(UINT16 bits);


//==============================================================================
// Public structure of functions for the I/0 Ports
//==============================================================================
struct sChinookPort Port =
{

   .A.ClearBits           = &PortAClearBits
  ,.B.ClearBits           = &PortBClearBits
  ,.C.ClearBits           = &PortCClearBits
  ,.D.ClearBits           = &PortDClearBits
  ,.E.ClearBits           = &PortEClearBits
  ,.F.ClearBits           = &PortFClearBits
  ,.G.ClearBits           = &PortGClearBits

  ,.A.CloseBits           = &PortACloseBits
  ,.B.CloseBits           = &PortBCloseBits
  ,.C.CloseBits           = &PortCCloseBits
  ,.D.CloseBits           = &PortDCloseBits
  ,.E.CloseBits           = &PortECloseBits
  ,.F.CloseBits           = &PortFCloseBits
  ,.G.CloseBits           = &PortGCloseBits

  ,.A.ReadPort            = &PortAReadPort
  ,.B.ReadPort            = &PortBReadPort
  ,.C.ReadPort            = &PortCReadPort
  ,.D.ReadPort            = &PortDReadPort
  ,.E.ReadPort            = &PortEReadPort
  ,.F.ReadPort            = &PortFReadPort
  ,.G.ReadPort            = &PortGReadPort

  ,.A.ReadBits            = &PortAReadBits
  ,.B.ReadBits            = &PortBReadBits
  ,.C.ReadBits            = &PortCReadBits
  ,.D.ReadBits            = &PortDReadBits
  ,.E.ReadBits            = &PortEReadBits
  ,.F.ReadBits            = &PortFReadBits
  ,.G.ReadBits            = &PortGReadBits

  ,.A.SetBits             = &PortASetBits
  ,.B.SetBits             = &PortBSetBits
  ,.C.SetBits             = &PortCSetBits
  ,.D.SetBits             = &PortDSetBits
  ,.E.SetBits             = &PortESetBits
  ,.F.SetBits             = &PortFSetBits
  ,.G.SetBits             = &PortGSetBits

  ,.A.SetPinsAnalogIn     = &PortASetPinsAnalogIn
  ,.B.SetPinsAnalogIn     = &PortBSetPinsAnalogIn
  ,.C.SetPinsAnalogIn     = &PortCSetPinsAnalogIn
  ,.D.SetPinsAnalogIn     = &PortDSetPinsAnalogIn
  ,.E.SetPinsAnalogIn     = &PortESetPinsAnalogIn
  ,.F.SetPinsAnalogIn     = &PortFSetPinsAnalogIn
  ,.G.SetPinsAnalogIn     = &PortGSetPinsAnalogIn

  ,.A.SetPinsDigitalOut   = &PortASetPinsDigitalOut
  ,.B.SetPinsDigitalOut   = &PortBSetPinsDigitalOut
  ,.C.SetPinsDigitalOut   = &PortCSetPinsDigitalOut
  ,.D.SetPinsDigitalOut   = &PortDSetPinsDigitalOut
  ,.E.SetPinsDigitalOut   = &PortESetPinsDigitalOut
  ,.F.SetPinsDigitalOut   = &PortFSetPinsDigitalOut
  ,.G.SetPinsDigitalOut   = &PortGSetPinsDigitalOut

  ,.A.SetPinsDigitalIn    = &PortASetPinsDigitalIn
  ,.B.SetPinsDigitalIn    = &PortBSetPinsDigitalIn
  ,.C.SetPinsDigitalIn    = &PortCSetPinsDigitalIn
  ,.D.SetPinsDigitalIn    = &PortDSetPinsDigitalIn
  ,.E.SetPinsDigitalIn    = &PortESetPinsDigitalIn
  ,.F.SetPinsDigitalIn    = &PortFSetPinsDigitalIn
  ,.G.SetPinsDigitalIn    = &PortGSetPinsDigitalIn

  ,.A.ToggleBits          = &PortAToggleBits
  ,.B.ToggleBits          = &PortBToggleBits
  ,.C.ToggleBits          = &PortCToggleBits
  ,.D.ToggleBits          = &PortDToggleBits
  ,.E.ToggleBits          = &PortEToggleBits
  ,.F.ToggleBits          = &PortFToggleBits
  ,.G.ToggleBits          = &PortGToggleBits

  ,.A.WritePort           = &PortAWritePort
  ,.B.WritePort           = &PortBWritePort
  ,.C.WritePort           = &PortCWritePort
  ,.D.WritePort           = &PortDWritePort
  ,.E.WritePort           = &PortEWritePort
  ,.F.WritePort           = &PortFWritePort
  ,.G.WritePort           = &PortGWritePort

};

//==============================================================================
// Declarations of private functions
//==============================================================================

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// PortXSetBits
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
static inline INT8 PortASetBits(UINT16 bits)
{
#if defined __32MX795F512L__   // Port A only on this device
  if (  (bits & BIT_8 )  // RA8, RA11-13 non existent
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13))
  {
    return -1;  // bits invalid
  }

  mPORTASetBits(bits);

  return 0;     // EXIT_SUCCESS
#elif defined __32MX795F512H__ || __32MX320F128H__   // Port A not on these devices
  return -1;    // EXIT_FAILURE
#endif
}


static inline INT8 PortBSetBits(UINT16 bits)
{
  mPORTBSetBits(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortCSetBits(UINT16 bits)
{
#if defined __32MX795F512L__   // RC1-4 only on those device
  if (  (bits & BIT_0 )  // RC0, RC5-11 non existent
     || (bits & BIT_5 )
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_0 )  // RC0-11 non existent
     || (bits & BIT_1 )
     || (bits & BIT_2 )
     || (bits & BIT_3 )
     || (bits & BIT_4 )
     || (bits & BIT_5 )
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#endif


  mPORTCSetBits(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortDSetBits(UINT16 bits)
{
#if defined __32MX795F512L__   // RD12-15 only on those device
  mPORTDSetBits(bits);
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
  mPORTDSetBits(bits);
#endif

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortESetBits(UINT16 bits)
{
#if defined __32MX795F512L__   // RE8-9 only on those device
  if (  (bits & BIT_10)  // RE10-15 non existent
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_8)    // RE8-15 non existent
     || (bits & BIT_9)
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  mPORTESetBits(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortFSetBits(UINT16 bits)
{
#ifdef __32MX795F512L__   // RF2, 8, 12-13 only on this device
  if (  (bits & BIT_6 )  // RF6-7, RF9-11, RF14-15 non existent
     || (bits & BIT_7 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX320F128H__  // RF2, 6, only on Chipkit Uno32
  if (  (bits & BIT_7 )  // RF9-11, RF14-15 non existent
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__   // RF2, 8, 12-13 not on this device
  if (  (bits & BIT_2 )  // RF2, RF6-15 non existent
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  mPORTFSetBits(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortGSetBits(UINT16 bits)
{
#if defined __32MX795F512L__   // RG0-1, RG12-15 only on those device
  if (  (bits & BIT_2 )  // RG2-3 input only and RG4-5, RG10-11 non existent
     || (bits & BIT_3 )
     || (bits & BIT_4 )
     || (bits & BIT_5 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__  // RG0-1, RG12-15 not on those device
  if (  (bits & BIT_0 )  // RG2-3 input only and RG0-1, RG4-5, RG10-15 non existent
     || (bits & BIT_1 )
     || (bits & BIT_2 )
     || (bits & BIT_3 )
     || (bits & BIT_4 )
     || (bits & BIT_5 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  mPORTGSetBits(bits);

  return 0;     // EXIT_SUCCESS
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// PortXClearBits
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
static inline INT8 PortAClearBits(UINT16 bits)
{
#if defined __32MX795F512L__  // Port A only on those device
  if (  // RA8, RA11-13 non existent
        (bits & BIT_8 )
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13))
  {
    return -1;  // bits invalid
  }

  mPORTAClearBits(bits);

  return 0;     // EXIT_SUCCESS
#elif defined __32MX795F512H__ || __32MX320F128H__   // Port A not on these devices
  return -1;    // EXIT_FAILURE
#endif
}

static inline INT8 PortBClearBits(UINT16 bits)
{
  mPORTBClearBits(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortCClearBits(UINT16 bits)
{
#if defined __32MX795F512L__    // RC1-4 only on those device
  if (  (bits & BIT_0 )  // RC0, RC5-11 non existent
     || (bits & BIT_5 )
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_0 )  // RC0-11 non existent
     || (bits & BIT_1 )
     || (bits & BIT_2 )
     || (bits & BIT_3 )
     || (bits & BIT_4 )
     || (bits & BIT_5 )
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#endif


  mPORTCClearBits(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortDClearBits(UINT16 bits)
{
#if defined __32MX795F512L__   // RD12-15 only on those device
  mPORTDClearBits(bits);
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
  mPORTDClearBits(bits);
#endif


  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortEClearBits(UINT16 bits)
{
#if defined __32MX795F512L__   // RE8-9 only on those device
  if (  (bits & BIT_10)  // RE10-15 non existent
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_8)    // RE8-15 non existent
     || (bits & BIT_9)
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  mPORTEClearBits(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortFClearBits(UINT16 bits)
{
#ifdef __32MX795F512L__   // RF2, 8, 12-13 only on this device
  if (  (bits & BIT_6 )  // RF6-7, RF9-11, RF14-15 non existent
     || (bits & BIT_7 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX320F128H__  // RF2, 6, only on Chipkit Uno32
  if (  (bits & BIT_7 )  // RF9-11, RF14-15 non existent
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__   // RF2, 8, 12-13 not on this device
  if (  (bits & BIT_2 )  // RF2, RF6-15 non existent
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif
  mPORTFClearBits(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortGClearBits(UINT16 bits)
{
#if defined __32MX795F512L__   // RG0-1, RG12-15 only on this device
  if (  (bits & BIT_2 )  // RG2-3 input only and RG4-5, RG10-11 non existent
     || (bits & BIT_3 )
     || (bits & BIT_4 )
     || (bits & BIT_5 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__  // RG0-1, RG12-15 not on those device
  if (  (bits & BIT_0 )  // RG2-3 input only and RG0-1, RG4-5, RG10-15 non existent
     || (bits & BIT_1 )
     || (bits & BIT_2 )
     || (bits & BIT_3 )
     || (bits & BIT_4 )
     || (bits & BIT_5 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  mPORTGClearBits(bits);

  return 0;     // EXIT_SUCCESS
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// PortXToggleBits
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
static inline INT8 PortAToggleBits(UINT16 bits)
{
#if defined __32MX795F512L__   // Port A only on this device
  if (  // RA8, RA11-13 non existent
        (bits & BIT_8 )
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13))
  {
    return -1;  // bits invalid
  }

  mPORTAToggleBits(bits);

  return 0;     // EXIT_SUCCESS
#elif defined __32MX795F512H__ || __32MX320F128H__   // Port A not on these devices
  return -1;    // EXIT_FAILURE
#endif
}

static inline INT8 PortBToggleBits(UINT16 bits)
{
  mPORTBToggleBits(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortCToggleBits(UINT16 bits)
{
#if defined __32MX795F512L__   // RC1-4 only on this device
  if (  (bits & BIT_0 )  // RC0, RC5-11 non existent
     || (bits & BIT_5 )
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_0 )  // RC0-11 non existent
     || (bits & BIT_1 )
     || (bits & BIT_2 )
     || (bits & BIT_3 )
     || (bits & BIT_4 )
     || (bits & BIT_5 )
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#endif


  mPORTCToggleBits(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortDToggleBits(UINT16 bits)
{
#if defined __32MX795F512L__   // RD12-15 only on this device
  mPORTDToggleBits(bits);
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
  mPORTDToggleBits(bits);
#endif


  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortEToggleBits(UINT16 bits)
{
#if defined __32MX795F512L__   // RE8-9 only on this devic
  if (  (bits & BIT_10)  // RE10-15 non existent
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_8)    // RE8-15 non existent
     || (bits & BIT_9)
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  mPORTEToggleBits(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortFToggleBits(UINT16 bits)
{
#ifdef __32MX795F512L__   // RF2, 8, 12-13 only on this device
  if (  (bits & BIT_6 )  // RF6-7, RF9-11, RF14-15 non existent
     || (bits & BIT_7 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX320F128H__  // RF2, 6, only on Chipkit Uno32
  if (  (bits & BIT_7 )  // RF9-11, RF14-15 non existent
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__   // RF2, 8, 12-13 not on this device
  if (  (bits & BIT_2 )  // RF2, RF6-15 non existent
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  mPORTFToggleBits(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortGToggleBits(UINT16 bits)
{
#if defined __32MX795F512L__   // RG0-1, RG12-15 only on this device
  if (  (bits & BIT_2 )  // RG2-3 input only and RG4-5, RG10-11 non existent
     || (bits & BIT_3 )
     || (bits & BIT_4 )
     || (bits & BIT_5 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__  // RG0-1, RG12-15 not on those device
  if (  (bits & BIT_0 )  // RG2-3 input only and RG0-1, RG4-5, RG10-15 non existent
     || (bits & BIT_1 )
     || (bits & BIT_2 )
     || (bits & BIT_3 )
     || (bits & BIT_4 )
     || (bits & BIT_5 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  mPORTGToggleBits(bits);

  return 0;     // EXIT_SUCCESS
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// PortXSetPinsDigitalOut
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
static inline INT8 PortASetPinsDigitalOut(UINT16 bits)
{
#if defined __32MX795F512L__   // Port A only on this device
  if (  // RA8, RA11-13 non existent
        (bits & BIT_8 )
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13))
  {
    return -1;  // bits invalid
  }

  mPORTASetPinsDigitalOut(bits);

  return 0;     // EXIT_SUCCESS
#elif defined __32MX795F512H__ || __32MX320F128H__   // Port A not on these devices
  return -1;    // EXIT_FAILURE
#endif
}

static inline INT8 PortBSetPinsDigitalOut(UINT16 bits)
{
  mPORTBSetPinsDigitalOut(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortCSetPinsDigitalOut(UINT16 bits)
{
#if defined __32MX795F512L__   // RC1-4 only on this device
  if (  (bits & BIT_0 )  // RC0, RC5-11 non existent
     || (bits & BIT_5 )
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_0 )  // RC0-11 non existent
     || (bits & BIT_1 )
     || (bits & BIT_2 )
     || (bits & BIT_3 )
     || (bits & BIT_4 )
     || (bits & BIT_5 )
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#endif


  mPORTCSetPinsDigitalOut(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortDSetPinsDigitalOut(UINT16 bits)
{
#if defined __32MX795F512L__   // RD12-15 only on this device
  mPORTDSetPinsDigitalOut(bits);
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
  mPORTDSetPinsDigitalOut(bits);
#endif


  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortESetPinsDigitalOut(UINT16 bits)
{
#if defined __32MX795F512L__   // RE8-9 only on this devic
  if (  (bits & BIT_10)  // RE10-15 non existent
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_8)    // RE8-15 non existent
     || (bits & BIT_9)
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  mPORTESetPinsDigitalOut(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortFSetPinsDigitalOut(UINT16 bits)
{
#ifdef __32MX795F512L__   // RF2, 8, 12-13 only on this device
  if (  (bits & BIT_6 )  // RF6-7, RF9-11, RF14-15 non existent
     || (bits & BIT_7 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX320F128H__  // RF2, 6, only on Chipkit Uno32
  if (  (bits & BIT_7 )  // RF9-11, RF14-15 non existent
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__   // RF2, 8, 12-13 not on this device
  if (  (bits & BIT_2 )  // RF2, RF6-15 non existent
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  mPORTFSetPinsDigitalOut(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortGSetPinsDigitalOut(UINT16 bits)
{
#if defined __32MX795F512L__   // RG0-1, RG12-15 only on this device
  if (  (bits & BIT_2 )  // RG2-3 input only and RG4-5, RG10-11 non existent
     || (bits & BIT_3 )
     || (bits & BIT_4 )
     || (bits & BIT_5 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__  // RG0-1, RG12-15 not on those device
  if (  (bits & BIT_0 )  // RG2-3 input only and RG0-1, RG4-5, RG10-15 non existent
     || (bits & BIT_1 )
     || (bits & BIT_2 )
     || (bits & BIT_3 )
     || (bits & BIT_4 )
     || (bits & BIT_5 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  mPORTGSetPinsDigitalOut(bits);

  return 0;     // EXIT_SUCCESS
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// PortXSetPinsDigitalIn
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
static inline INT8 PortASetPinsDigitalIn(UINT16 bits)
{
#if defined __32MX795F512L__   // Port A only on this device
  if (  // RA8, RA11-13 non existent
        (bits & BIT_8 )
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13))
  {
    return -1;  // bits invalid
  }

  mPORTASetPinsDigitalIn(bits);

  return 0;     // EXIT_SUCCESS
#elif defined __32MX795F512H__ || __32MX320F128H__   // Port A not on these devices
  return -1;    // EXIT_FAILURE
#endif
}

static inline INT8 PortBSetPinsDigitalIn(UINT16 bits)
{
  mPORTBSetPinsDigitalIn(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortCSetPinsDigitalIn(UINT16 bits)
{
#if defined __32MX795F512L__   // RC1-4 only on this device
  if (  (bits & BIT_0 )  // RC0, RC5-11 non existent
     || (bits & BIT_5 )
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_0 )  // RC0-11 non existent
     || (bits & BIT_1 )
     || (bits & BIT_2 )
     || (bits & BIT_3 )
     || (bits & BIT_4 )
     || (bits & BIT_5 )
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#endif


  mPORTCSetPinsDigitalIn(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortDSetPinsDigitalIn(UINT16 bits)
{
#if defined __32MX795F512L__   // RD12-15 only on this device
  mPORTDSetPinsDigitalIn(bits);
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
  mPORTDSetPinsDigitalIn(bits);
#endif


  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortESetPinsDigitalIn(UINT16 bits)
{
#if defined __32MX795F512L__   // RE8-9 only on this devic
  if (  (bits & BIT_10)  // RE10-15 non existent
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_8)    // RE8-15 non existent
     || (bits & BIT_9)
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  mPORTESetPinsDigitalIn(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortFSetPinsDigitalIn(UINT16 bits)
{
#ifdef __32MX795F512L__   // RF2, 8, 12-13 only on this device
  if (  (bits & BIT_6 )  // RF6-7, RF9-11, RF14-15 non existent
     || (bits & BIT_7 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX320F128H__  // RF2, 6, only on Chipkit Uno32
  if (  (bits & BIT_7 )  // RF9-11, RF14-15 non existent
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__   // RF2, 8, 12-13 not on this device
  if (  (bits & BIT_2 )  // RF2, RF6-15 non existent
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  mPORTFSetPinsDigitalIn(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortGSetPinsDigitalIn(UINT16 bits)
{
#if defined __32MX795F512L__   // RG0-1, RG12-15 only on this device
  if (  (bits & BIT_4 )  // RG4-5, RG10-11 non existent
     || (bits & BIT_5 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__  // RG0-1, RG12-15 not on those device
  if (  (bits & BIT_0 )  // RG0-1, RG4-5, RG10-15 non existent
     || (bits & BIT_1 )
     || (bits & BIT_4 )
     || (bits & BIT_5 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  mPORTGSetPinsDigitalIn(bits);

  return 0;     // EXIT_SUCCESS
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// PortXCloseBits
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
static inline INT8 PortACloseBits(UINT16 bits)
{
#if defined __32MX795F512L__   // Port A only on this device
  if (  // RA8, RA11-13 non existent
        (bits & BIT_8 )
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13))
  {
    return -1;  // bits invalid
  }

  mPORTACloseBits(bits);

  return 0;     // EXIT_SUCCESS
#elif defined __32MX795F512H__ || __32MX320F128H__   // Port A not on these devices
  return -1;    // EXIT_FAILURE
#endif
}

static inline INT8 PortBCloseBits(UINT16 bits)
{
  mPORTBCloseBits(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortCCloseBits(UINT16 bits)
{
#if defined __32MX795F512L__  // RC1-4 only on this device
  if (  (bits & BIT_0 )  // RC0, RC5-11 non existent
     || (bits & BIT_5 )
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_0 )  // RC0-11 non existent
     || (bits & BIT_1 )
     || (bits & BIT_2 )
     || (bits & BIT_3 )
     || (bits & BIT_4 )
     || (bits & BIT_5 )
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#endif


  mPORTCCloseBits(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortDCloseBits(UINT16 bits)
{
#if defined __32MX795F512L__   // RD12-15 only on this device
  mPORTDCloseBits(bits);
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
  mPORTDCloseBits(bits);
#endif


  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortECloseBits(UINT16 bits)
{
#if defined __32MX795F512L__   // RE8-9 only on this devic
  if (  (bits & BIT_10)  // RE10-15 non existent
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_8)    // RE8-15 non existent
     || (bits & BIT_9)
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  mPORTECloseBits(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortFCloseBits(UINT16 bits)
{
#ifdef __32MX795F512L__   // RF2, 8, 12-13 only on this device
  if (  (bits & BIT_6 )  // RF6-7, RF9-11, RF14-15 non existent
     || (bits & BIT_7 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX320F128H__  // RF2, 6, only on Chipkit Uno32
  if (  (bits & BIT_7 )  // RF9-11, RF14-15 non existent
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__   // RF2, 8, 12-13 not on this device
  if (  (bits & BIT_2 )  // RF2, RF6-15 non existent
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  mPORTFCloseBits(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortGCloseBits(UINT16 bits)
{
#if defined __32MX795F512L__   // RG0-1, RG12-15 only on this device
  if (  (bits & BIT_4 )  // RG4-5, RG10-11 non existent
     || (bits & BIT_5 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__  // RG0-1, RG12-15 not on those device
  if (  (bits & BIT_0 )  // RG0-1, RG4-5, RG10-15 non existent
     || (bits & BIT_1 )
     || (bits & BIT_4 )
     || (bits & BIT_5 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  mPORTGCloseBits(bits);

  return 0;     // EXIT_SUCCESS
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// PortXSetPinsAnalogIn
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
static inline INT8 PortASetPinsAnalogIn(UINT16 bits)
{
  return -1;  // The function is only available for PORT B
}

static inline INT8 PortBSetPinsAnalogIn(UINT16 bits)
{
  mPORTBSetPinsAnalogIn(bits);

  return 0;     // EXIT_SUCCESS
}
static inline INT8 PortCSetPinsAnalogIn(UINT16 bits)
{
  return -1;  // The function is only available for PORT B
}

static inline INT8 PortDSetPinsAnalogIn(UINT16 bits)
{
  return -1;  // The function is only available for PORT B
}

static inline INT8 PortESetPinsAnalogIn(UINT16 bits)
{
  return -1;  // The function is only available for PORT B
}

static inline INT8 PortFSetPinsAnalogIn(UINT16 bits)
{
  return -1;  // The function is only available for PORT B
}

static inline INT8 PortGSetPinsAnalogIn(UINT16 bits)
{
  return -1;  // The function is only available for PORT B
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// PortXReadPort
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
static inline UINT16 PortAReadPort(void)
{
#if defined __32MX795F512L__   // Port A only on this device
  return PORTRead(IOPORT_A);
#elif defined __32MX795F512H__ || __32MX320F128H__   // Port A not on these devices
  return -1;    // EXIT_FAILURE
#endif
}


static inline UINT16 PortBReadPort(void)
{
  return PORTRead(IOPORT_B);
}
static inline UINT16 PortCReadPort(void)
{
  return PORTRead(IOPORT_C);
}

static inline UINT16 PortDReadPort(void)
{
  return PORTRead(IOPORT_D);
}

static inline UINT16 PortEReadPort(void)
{
  return PORTRead(IOPORT_E);
}

static inline UINT16 PortFReadPort(void)
{
  return PORTRead(IOPORT_F);
}

static inline UINT16 PortGReadPort(void)
{
  return PORTRead(IOPORT_G);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// PortXReadBits
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
static inline INT32 PortAReadBits(UINT16 bits)
{
#if defined __32MX795F512L__   // Port A only on this device
  if (  // RA8, RA11-13 non existent
        (bits & BIT_8 )
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13))
  {
    return -1;  // bits invalid
  }

  return PORTReadBits(IOPORT_A, bits);

  return 0;     // EXIT_SUCCESS
#elif defined __32MX795F512H__ || __32MX320F128H__   // Port A not on these devices
  return -1;    // EXIT_FAILURE
#endif
}

static inline INT32 PortBReadBits(UINT16 bits)
{
  return PORTReadBits(IOPORT_B, bits);
}

static inline INT32 PortCReadBits(UINT16 bits)
{
#if defined __32MX795F512L__     // RC1-4 only on this device
  if (  (bits & BIT_0 )  // RC0, RC5-11 non existent
     || (bits & BIT_5 )
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_0 )  // RC0-11 non existent
     || (bits & BIT_1 )
     || (bits & BIT_2 )
     || (bits & BIT_3 )
     || (bits & BIT_4 )
     || (bits & BIT_5 )
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#endif

  return PORTReadBits(IOPORT_C, bits);
}

static inline INT32 PortDReadBits(UINT16 bits)
{
#if defined __32MX795F512L__   // RD12-15 only on this device
  return PORTReadBits(IOPORT_D, bits);
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
  return PORTReadBits(IOPORT_D, bits);
#endif
}

static inline INT32 PortEReadBits(UINT16 bits)
{
#if defined __32MX795F512L__   // RE8-9 only on this devic
  if (  (bits & BIT_10)  // RE10-15 non existent
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__
  if (  (bits & BIT_8)    // RE8-15 non existent
     || (bits & BIT_9)
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  return PORTReadBits(IOPORT_E, bits);
}
static inline INT32 PortFReadBits(UINT16 bits)
{
#ifdef __32MX795F512L__   // RF2, 8, 12-13 only on this device
  if (  (bits & BIT_6 )  // RF6-7, RF9-11, RF14-15 non existent
     || (bits & BIT_7 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX320F128H__  // RF2, 6, only on Chipkit Uno32
  if (  (bits & BIT_7 )  // RF9-11, RF14-15 non existent
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__   // RF2, 8, 12-13 not on this device
  if (  (bits & BIT_2 )  // RF2, RF6-15 non existent
     || (bits & BIT_6 )
     || (bits & BIT_7 )
     || (bits & BIT_8 )
     || (bits & BIT_9 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  return PORTReadBits(IOPORT_F, bits);
}
static inline INT32 PortGReadBits(UINT16 bits)
{
#if defined __32MX795F512L__   // RG0-1, RG12-15 only on this device
  if (  (bits & BIT_4 )  // RG4-5, RG10-11 non existent
     || (bits & BIT_5 )
     || (bits & BIT_10)
     || (bits & BIT_11))
  {
    return -1;  // bits invalid
  }
#elif defined __32MX795F512H__ || __32MX320F128H__  // RG0-1, RG12-15 not on those device
  if (  (bits & BIT_0 )  // RG0-1, RG4-5, RG10-15 non existent
     || (bits & BIT_1 )
     || (bits & BIT_4 )
     || (bits & BIT_5 )
     || (bits & BIT_10)
     || (bits & BIT_11)
     || (bits & BIT_12)
     || (bits & BIT_13)
     || (bits & BIT_14)
     || (bits & BIT_15))
  {
    return -1;  // bits invalid
  }
#endif

  return PORTReadBits(IOPORT_G, bits);

  return 0;     // EXIT_SUCCESS
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// PortXWritePort
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
static inline void PortAWritePort(UINT16 bits)
{
#if defined __32MX795F512L__   // Port A only on this device
  mPORTAWrite(bits);
#elif defined __32MX795F512H__ || __32MX320F128H__   // Port A not on these devices
  return;     // EXIT_FAILURE
#endif
}

static inline void PortBWritePort(UINT16 bits)
{
  mPORTBWrite(bits);
}
static inline void PortCWritePort(UINT16 bits)
{
  mPORTCWrite(bits);
}
static inline void PortDWritePort(UINT16 bits)
{
  mPORTDWrite(bits);
}
static inline void PortEWritePort(UINT16 bits)
{
  mPORTEWrite(bits);
}
static inline void PortFWritePort(UINT16 bits)
{
  mPORTFWrite(bits);
}
static inline void PortGWritePort(UINT16 bits)
{
  mPORTGWrite(bits);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%