/*
 * File:   skadi.h
 * Author: Etienne
 *
 * Created on 5 mars 2013, 20:44
 */

#ifndef __SKADI__
#define __SKADI__

#define MAX_CMD_NAME_SIZE             256
#define MAX_CMD_DESCRIPTION_SIZE      256
#define MAX_NB_ARGUMENTS              5
#define COMMAND_SEPARATOR             " "

#define ERROR_WRONG_NUMBER_ARGUMENTS  -1000
#define ERROR_UNKNOWN_COMMAND         -1001

#include <stdlib.h>
#include "ChinookUart.h"
extern struct sChinookUart  Uart;


//==============================================================================
// Type definitions
//==============================================================================

typedef void skadiCommandProc();

typedef struct sSkadiCommand
{
  char *name;                   // The name to be used when calling the command ex: help
  skadiCommandProc *proc;       // The actual function pointer
  int arity;                    // The number of arguments the function takes
  char *description;            // The description (to be used by help)
} sSkadiCommand_t;

typedef struct sSkadiArray
{
  size_t length;
  sSkadiCommand_t * elements;
} sSkadiArray_t;

typedef struct sSkadiArgs
{
  size_t length;
  char* elements[MAX_NB_ARGUMENTS];
} sSkadiArgs_t;


typedef struct sSkadi
{
  sSkadiArray_t externalCommandTable;
  UartModule_t  uartModule;
} sSkadi_t;


//==============================================================================
// Public structure declaration
//==============================================================================

struct sChinookSkadi
{

  /**
   * @Prototype     static inline void SkadiInit ( sSkadiCommand_t * extCommandTable, size_t len, UartModule_t uartModule, BOOL oUseUartInterrupts )
   *
   * @Description   This function initializes Skadi. The user must create a
   *                structure sSkadicommand_t representing the functions he/she
   *                wants to use with skadi. It also assigns a UART module to use
   *                for Skadi. A complete example is given below.
   *
   * @Example       The following example uses the UART1 for Skadi. It uses 2 
   *                functions to interract with Skadi, Led1 and Led2.\n\n
   *                void static Led1(); // Toggle Led 1\n
   *                void static Led2(); // Toggle Led 2\n\n
   *                int main()\n
   *                {\n
   *                  // ..............  //\n
   *                  // Open UART1 code //\n
   *                  // ..............  //\n
   *                  sSkadiCommand_t skadiCommandTable[] =\n
   *                  {\n
   *                     {"Led1", Led1, 1, "Usage : flash Led 1"}\n
   *                    ,{"Led2", Led2, 1, "Usage : flash Led 2"}\n
   *                  };\n\n
   *                  Skadi.Init(skadiCommandTable, sizeof(skadiCommandTable)/sizeof(sSkadiCommand_t), UART1);\n\n
   *                  while(1)\n
   *                  {\n
   *                    Skadi.GetCmdMsg();\n
   *                  }\n
   *                } // end main\n\n
   *
   *                void Led1(sSkadi_t *skadi, sSkadiArgs_t args)\n
   *                {\n
   *                  UINT8 errorMsg[] = {"Cette led n'existe pas!/r/n"};\n
   *                  int led = atoi(args.elements[0]);   // Convert argument to int\n
   *                  if (led == 1)\n
   *                  {\n
   *                    Port.A.ToggleBits(BIT_3);\n
   *                  }\n
   *                  else\n
   *                  {\n
   *                    Uart.SendDataBuffer(UART1, errorMsg, sizeof(errorMsg));\n
   *                  }\n
   *                }\n\n
   *
   *                void Led2(sSkadi_t *skadi, sSkadiArgs_t args)\n
   *                {\n
   *                  UINT8 errorMsg[] = {"Cette led n'existe pas!/r/n"};\n
   *                  int led = atoi(args.elements[0]);   // Convert argument to int\n
   *                  if (led == 2)\n
   *                  {\n
   *                   Port.C.ToggleBits(BIT_1);\n
   *                  }\n
   *                  else\n
   *                  {\n
   *                    Uart.SendDataBuffer(UART1, errorMsg, sizeof(errorMsg));\n
   *                  }\n
   *                }
   *
   * @Argument      sSkadiCommand_t * extCommandTable\n
   *                Address of the structure containing the functions you want
   *                to use. Use the format below : \n
   *                sSkadiCommand_t skadiCommandTable[] =\n
   *                {\n
   *                    {"Function1Name", Function1, nArguments1, "Description1"}\n
   *                   ,{"Function2Name", Function2, nArguments2, "Description2"}\n
   *                   ,{"Function3Name", Function3, nArguments3, "Description3"}\n
   *                };
   * 
   * @Argument      size_t len\n
   *                Number of functions to use. Should be entered as : \n
   *                size_t len = sizeof(skadiCommandTable)/sizeof(sSkadiCommand_t);
   *
   * @Argument      UartModule_t uartModuleId
   *                This specifies which UART Skadi will use. The UART module
   *                used must be initialized beforehand.
   *
   * @Argument      BOOL oUseUartInterrupts
   *                This tells the function if the user is using UART interrupts.
   *                The reason is that different functions are used in each 
   *                situation.\n
   *                Values : \n
   *                TRUE : the user uses UART interrupts\n
   *                FALSE : the user doesn't use UART interrupts
   *
   * @Returns       None.
   */
  void (*Init) (sSkadiCommand_t* external_command_table, size_t len, UartModule_t uartModule, BOOL oUseUartInterrupts);


  /**
   * @Prototype     static inline void SkadiGetCmdMsg (INT32 allo)
   *
   * @Description   This function gets a buffer from the UART specified in
   *                Skadi.Init and, if it is recognized, calls the function read
   *                by the buffer. It should be used in debugging when connecting
   *                the UART module to a  serial command interface (ex : Tera Term).
   *
   * @Example       SkadiGetCmdMsg();\n
   *                This example uses Skadi library.
   *
   * @Returns       None.
   */
  void (*GetCmdMsg) (void);


  /**
   * @Prototype     static inline void SkadiGetCmdMsgFifo (void)
   *
   * @Description   This function gets a buffer from the UART specified in
   *                Skadi.Init and, if it is recognized, calls the function read
   *                by the buffer. It should be used in debugging when connecting
   *                the UART module to a  serial command interface (ex : Tera Term).
   *
   * @Example       SkadiGetCmdMsgFifo();\n
   *                This example uses Skadi library.
   *
   * @Returns       None.
   */
  void (*GetCmdMsgFifo) (void);

};

#endif // __SKADI__


