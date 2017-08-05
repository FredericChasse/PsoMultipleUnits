/* 
 * File:   ChinookPorts.h
 * Author: Frederic
 *
 * Created on 15 octobre 2014, 17:36
 */

#ifndef __CHINOOK_PORTS__
#define	__CHINOOK_PORTS__

#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>
#include "ChinookHardwareProfile.h"


/*
 * Public structure declaration
 */
struct sPort
{

  /**
   * @Prototype   static inline INT8 PortXSetBits(UINT16 bits);
   *
   * @Description This function sets the bits specified in Argument 1 to HIGH
   *
   * @Example     err = Port.A.SetBits( BIT_0 | BIT_2 | BIT_7 );\n
   *              This example sets the bits 0, 2 and 7 of port A to HIGH
   *
   * @Param       UINT16 bits\n
   *              Bits to set\n
   *              Invalid bits are :\n
   *              RA8, RA11-13 non existent\n
   *              RC0, RC5-11 non existent
   *              RE10-15 non existent
   *              RF6-7, RF9-11, RF14-15 non existent
   *              RG4-5, RG10-11 non existent
   *              RG2-3 input only
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : The bits of the Port used do not exist.
   */
  INT8 (*SetBits)     (UINT16 bits);


  /**
   * @Prototype   static inline INT8 PortXClearBits(UINT16 bits);
   *
   * @Description This function clears the bits specified in Argument 1
   *
   * @Example     err = Port.A.ClearBits( BIT_0 | BIT_2 | BIT_7 );\n
   *              This example clears the bits 0, 2 and 7 of port A
   *
   * @Param       UINT16 bits\n
   *              Bits to clear\n
   *              Invalid bits are :\n
   *              RA8, RA11-13 non existent\n
   *              RC0, RC5-11 non existent
   *              RE10-15 non existent
   *              RF6-7, RF9-11, RF14-15 non existent
   *              RG4-5, RG10-11 non existent
   *              RG2-3 input only
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : The bits of the Port used do not exist.
   */
  INT8 (*ClearBits)   (UINT16 bits);


  /**
   * @Prototype   static inline INT8 PortXToggleBits(UINT16 bits);
   *
   * @Description This function toggles the bits specified in Argument 1
   *
   * @Example     err = Port.A.ToggleBits( BIT_0 | BIT_2 | BIT_7 );\n
   *              This example toggles the bits 0, 2 and 7 of port A
   *
   * @Param       UINT16 bits\n
   *              Bits to toggle\n
   *              Invalid bits are :\n
   *              RA8, RA11-13 non existent\n
   *              RC0, RC5-11 non existent
   *              RE10-15 non existent
   *              RF6-7, RF9-11, RF14-15 non existent
   *              RG4-5, RG10-11 non existent
   *              RG2-3 input only
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : The bits of the Port used do not exist.
   */
  INT8 (*ToggleBits)  (UINT16 bits);
  
  
  /**
   * @Prototype   static inline INT8 PortXSetPinsDigitalOut(UINT16 bits);
   * 
   * @Description This function sets the pins specified in Argument 1 as 
   *              digital outputs
   * 
   * @Example     err = Port.A.SetPinsDigitalOut( BIT_0 | BIT_2 | BIT_7 );\n
   *              This example sets the bits 0, 2 and 7 of port A to
   *              digital outputs
   * 
   * @Param       UINT16 bits\n
   *              Bits to set as digital outputs\n
   *              Invalid bits are :\n
   *              RA8, RA11-13 non existent\n
   *              RC0, RC5-11 non existent
   *              RE10-15 non existent
   *              RF6-7, RF9-11, RF14-15 non existent
   *              RG4-5, RG10-11 non existent
   *              RG2-3 input only
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : The bits of the Port used do not exist.
   */
  INT8 (*SetPinsDigitalOut) (UINT16 bits);


  /**
   * @Prototype   static inline INT8 PortXSetPinsDigitalIn(UINT16 bits);
   *
   * @Description This function sets the pins specified in Argument 1 as
   *              digital inputs
   *
   * @Example     err = Port.A.SetPinsDigitalIn( BIT_0 | BIT_2 | BIT_7 );\n
   *              This example sets the bits 0, 2 and 7 of port A to
   *              digital inputs
   *
   * @Param       UINT16 bits\n
   *              Bits to set as digital inputs\n
   *              Invalid bits are :\n
   *              RA8, RA11-13 non existent\n
   *              RC0, RC5-11 non existent
   *              RE10-15 non existent
   *              RF6-7, RF9-11, RF14-15 non existent
   *              RG4-5, RG10-11 non existent
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : The bits of the Port used do not exist.
   */
  INT8 (*SetPinsDigitalIn) (UINT16 bits);


  /**
   * @Prototype   static inline INT8 PortXCloseBits(UINT16 bits);
   *
   * @Description This function sets the pins specified in Argument 1 as
   *              inputs and clears the corresponding register
   *
   * @Example     err = Port.A.CloseBits( BIT_0 | BIT_2 | BIT_7 );\n
   *              This example closes the bits 0, 2 and 7 of port A
   *
   * @Param       UINT16 bits\n
   *              Bits to close\n
   *              Invalid bits are :\n
   *              RA8, RA11-13 non existent\n
   *              RC0, RC5-11 non existent
   *              RE10-15 non existent
   *              RF6-7, RF9-11, RF14-15 non existent
   *              RG4-5, RG10-11 non existent
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : The bits of the Port used do not exist.
   */
  INT8 (*CloseBits) (UINT16 bits);


  /**
   * @Note        THIS FUNCTION IS ONLY AVAILABLE FOR PORT B. ALL OTHER PORTS
   *              WILL RETURN -1!
   *
   * @Prototype   static inline INT8 PortXSetPinsAnalogIn(UINT16 bits);
   *
   * @Description This function sets the pins specified in Argument 1 as
   *              analog inputs for the ADC
   *
   * @Example     err = Port.B.SetPinsAnalogIn( BIT_0 | BIT_2 | BIT_15 );\n
   *              This example set the bits 0, 2 and 15 of port B as analog
   *              inputs.
   *
   * @Param       UINT16 bits\n
   *              Bits to close\n
   *              All ports are invalid except for PORT B
   *
   * @Return      INT8\n
   *              On success : 0\n
   *              On failure : -1\n
   *              Conditions of failure : The bits of the Port used do not exist.
   */
  INT8 (*SetPinsAnalogIn) (UINT16 bits);


  /**
   * @Prototype   static inline UINT16 PortXReadPort(void);
   *
   * @Description This function reads the value of the Port specified
   *
   * @Example     portA = Port.A.ReadPort();\n
   *              This example reads the value of Port A
   *
   * @Param       None.
   *
   * @Return      UINT16\n
   *              Value of the Port specified
   */
  UINT16 (*ReadPort) (void);


  /**
   * @Prototype   static inline INT32 PortXReadBits(UINT16 bits);
   *
   * @Description This function reads the pins specified in Argument 1
   *
   * @Example     err = Port.A.ReadBits( BIT_0 | BIT_2 | BIT_7 );\n
   *              This example reads the bits 0, 2 and 7 of port A. The result will
   *              have the following format : \n
   *              err : 0bxxxxxxxxBxxxxBxB\n
   *              where B is the value of the bit.
   *
   * @Param       UINT16 bits\n
   *              Bits to read\n
   *              Invalid bits are :\n
   *              RA8, RA11-13 non existent\n
   *              RC0, RC5-11 non existent
   *              RE10-15 non existent
   *              RF6-7, RF9-11, RF14-15 non existent
   *              RG4-5, RG10-11 non existent
   *
   * @Return      INT32\n
   *              On success : The values needed presented on 16 bits\n
   *              On failure : -1\n
   *              Conditions of failure : The bits of the Port used do not exist.
   */
  INT32 (*ReadBits) (UINT16 bits);


  /**
   * @Prototype   static inline void PortXWritePort(UINT16 bits);
   *
   * @Description This function reads the pins specified in Argument 1
   *
   * @Example     Port.A.WritePort( 0x0055 );\n
   *              This example writes the value 0x0055 to port A
   *
   * @Param       UINT16 bits\n
   *              Word to write\n
   *
   * @Return      None.
   */
  void (*WritePort) (UINT16 bits);

};

struct sChinookPort
{
  /**
   * @Note    Port A only available on PIC32MX795F512L
   */
  struct sPort A;
  struct sPort B;
  struct sPort C;
  struct sPort D;
  struct sPort E;
  struct sPort F;
  struct sPort G;
};



#endif	/* __CHINOOK_PORTS__ */

