/*
 * File:   skadi.cpp
 * Author: Etienne
 *
 *  Created on 5 mars 2013, 20:44
 */

#include "../headers/ChinookSkadi.h"

#include <stdio.h>
#include <string.h>


//==============================================================================
// Private functions prototypes
//==============================================================================
static inline int               SkadiProcessCommand     ( const char * command );
static inline sSkadiCommand_t * SkadiLookupCommand      ( const char * command );
static inline void              SkadiSplitCommandLine   ( const char * command_line, char ** cmd_name, sSkadiArgs_t * skadi_args );
static inline int               SkadiValidateArguments  ( sSkadiCommand_t * sCommand, sSkadiArgs_t * skadi_args );
static inline void              SkadiInit               ( sSkadiCommand_t * external_command_table, size_t len, UartModule_t uartModuleId, BOOL oUseUartInterrupts );
static inline void              SkadiHelpCommand        ( void );
static inline void              SkadiHelpCommandFifo    ( void );
static inline void              SkadiGetCmdMsg          ( void );
static inline void              SkadiGetCmdMsgFifo      ( void );


//==============================================================================
// Public structure of functions for Skadi
//==============================================================================
struct sChinookSkadi Skadi =
{
  .Init           = &SkadiInit
 ,.GetCmdMsg      = &SkadiGetCmdMsg
 ,.GetCmdMsgFifo  = &SkadiGetCmdMsgFifo
};


//==============================================================================
// Variable declarations
//==============================================================================
sUartLineBuffer_t uartLineBuffer =    // Used for Skadi.GetCmdMsg()
{
  .buffer = {0}
 ,.length =  0
};

sSkadiCommand_t skadiInternalCommandTableElements[] =
{
//  { "help", SkadiHelpCommand, 0, "Affiche la liste des commandes disponibles." }
  { "help", SkadiHelpCommandFifo, 0, "Affiche la liste des commandes disponibles." }
};

sSkadiArray_t skadiInternalCommandTable =
{
  .length     = 1
 ,.elements   = skadiInternalCommandTableElements
};

sSkadi_t skadi;


//==============================================================================
// Skadi structure functions declarations
//==============================================================================

static inline void SkadiHelpCommand( void )
{
  int i, j;
  UINT8 bufferDefinedCmds[] = {"\n\r--- Commandes definies dans le projet ---\n\r\0"};
  UINT8 bufferDefaultCmds[] = {"\n\r--- Commandes par defaut (peut etre redefini) ---\n\r\0"};
  UINT8 bufferSpace[]       = {"  \0"};       // Makes the reading prettier
  UINT8 bufferDash[]        = {" - \0"};      // Makes the reading prettier

  Uart.SendDataBuffer(skadi.uartModule, bufferDefinedCmds, sizeof(bufferDefinedCmds));

  for ( i = 0; i < skadi.externalCommandTable.length; i++ )
  {
    Uart.SendDataBuffer(skadi.uartModule, bufferSpace, 2);     // Makes the reading prettier

    j = 0;
    while (skadi.externalCommandTable.elements[i].name[j] != 0)
    {
      Uart.SendDataByte(skadi.uartModule, skadi.externalCommandTable.elements[i].name[j]);
      j++;
    }

    Uart.SendDataBuffer(skadi.uartModule, bufferDash, 3);     // Makes the reading prettier

    j = 0;
    while (skadi.externalCommandTable.elements[i].description[j] != 0)
    {
      Uart.SendDataByte(skadi.uartModule, skadi.externalCommandTable.elements[i].description[j]);
      j++;
    }

    Uart.SendDataByte(skadi.uartModule, '\n');     // Makes the reading prettier
    Uart.SendDataByte(skadi.uartModule, '\r');     // Makes the reading prettier
  }

  Uart.SendDataBuffer(skadi.uartModule, bufferDefaultCmds, sizeof(bufferDefaultCmds));

  for ( i = 0; i < skadiInternalCommandTable.length; i++)
  {
    Uart.SendDataBuffer(skadi.uartModule, bufferSpace, 2);     // Makes the reading prettier

    j = 0;
    while (skadiInternalCommandTable.elements[i].name[j] != 0)
    {
      Uart.SendDataByte(skadi.uartModule, skadiInternalCommandTable.elements[i].name[j]);
      j++;
    }

    Uart.SendDataBuffer(skadi.uartModule, bufferDash, 3);     // Makes the reading prettier

    j = 0;
    while (skadiInternalCommandTable.elements[i].description[j] != 0)
    {
      Uart.SendDataByte(skadi.uartModule, skadiInternalCommandTable.elements[i].description[j]);
      j++;
    }

    UINT8 prettyEnd[] = "\n\r\n--- fin ---\r\n\n\n";
    Uart.SendDataBuffer(skadi.uartModule, prettyEnd, sizeof(prettyEnd));
//    Uart.SendDataByte(skadi.uartModule, '\n');     // Makes the reading prettier
//    Uart.SendDataByte(skadi.uartModule, '\r');     // Makes the reading prettier
//    Uart.SendDataByte(skadi.uartModule, '\n');     // Makes the reading prettier
  }
}


static inline void SkadiGetCmdMsg (void)
{
  BOOL lineState = Uart.GetDataBuffer(skadi.uartModule, &uartLineBuffer, TRUE);

  INT16 err;
  UINT8 errMsgWrongCmd[] = {"\n\rCette fonction n'est pas definie!\n\rEntrez \"help\" pour voir les commandes disponibles\n\r\n\0"};
  UINT8 errMsgWrongArg[] = {"\n\rTrop ou pas assez d'arguments!\n\rEntrez \"help\" pour voir les commandes disponibles\n\r\n\0"};

  if ( lineState == LINE_COMPLETE )
  {
    err = SkadiProcessCommand(uartLineBuffer.buffer);
    uartLineBuffer.length = 0;

    // Check for errors
    if (err == ERROR_WRONG_NUMBER_ARGUMENTS)
    {
      Uart.SendDataBuffer(skadi.uartModule, errMsgWrongArg, sizeof(errMsgWrongArg));
    }

    if (err == ERROR_UNKNOWN_COMMAND)
    {
      Uart.SendDataBuffer(skadi.uartModule, errMsgWrongCmd, sizeof(errMsgWrongCmd));
    }
  }
}

//==============================================================================
// Skadi structure functions declarations
//==============================================================================
static inline void SkadiGetCmdMsgFifo (void)
{
  static sUartLineBuffer_t uartRxMsg =
  {
     .buffer = {0}
    ,.length =  0
  };
  
  sUartLineBuffer_t bufferMsg =
  {
     .buffer = {0}
    ,.length =  0
  };
  
  UINT8 carriageReturn[] = "\n\r\0";
  
  BOOL lineState = LINE_INCOMPLETE;
  
  UINT16 i = 0;
  
  INT16 character;

  INT32 err = Uart.GetRxFifoBuffer(skadi.uartModule, &uartRxMsg, TRUE);
  
  if (err >= 0)
  {
    while( (uartLineBuffer.length < UART_LINE_BUFFER_LENGTH) && (i < uartRxMsg.length) && (lineState == LINE_INCOMPLETE) )
    {
      switch (uartRxMsg.buffer[i])
      {
        case '\r': // Carriage return
          uartLineBuffer.buffer[ uartLineBuffer.length ] = '\0';
          memcpy(&bufferMsg.buffer[0], &carriageReturn[0], sizeof(carriageReturn));
          bufferMsg.length = sizeof(carriageReturn);
          Uart.PutTxFifoBuffer(skadi.uartModule, &bufferMsg);
          lineState = LINE_COMPLETE;
          break;
        case 0x8: // Backspace
          if ( uartLineBuffer.length > 0 )
          {
              uartLineBuffer.length--;
          }
          break;
        default:
          uartLineBuffer.buffer[ uartLineBuffer.length ] = uartRxMsg.buffer[i];
          uartLineBuffer.length++;
      }
      
      i++;
    }
  }


  if ( lineState == LINE_COMPLETE )
  {
    UINT8 errMsgWrongCmd[] = {"\n\rCette fonction n'est pas definie!\n\rEntrez \"help\" pour voir les commandes disponibles\n\r\n\0"};
    UINT8 errMsgWrongArg[] = {"\n\rTrop ou pas assez d'arguments!\n\rEntrez \"help\" pour voir les commandes disponibles\n\r\n\0"};
    
    err = SkadiProcessCommand(uartLineBuffer.buffer);
    uartLineBuffer.length = 0;


    // Check for errors
    if (err == ERROR_WRONG_NUMBER_ARGUMENTS)
    {
      memcpy(&bufferMsg.buffer[0], &errMsgWrongArg[0], sizeof(errMsgWrongArg));
      bufferMsg.length = sizeof(errMsgWrongArg);
      Uart.PutTxFifoBuffer(skadi.uartModule, &bufferMsg);
//      Uart.SendDataBuffer(skadi.uartModule, errMsgWrongArg, sizeof(errMsgWrongArg));
    }

    if (err == ERROR_UNKNOWN_COMMAND)
    {
      memcpy(&bufferMsg.buffer[0], &errMsgWrongCmd[0], sizeof(errMsgWrongCmd));
      bufferMsg.length = sizeof(errMsgWrongCmd);
      Uart.PutTxFifoBuffer(skadi.uartModule, &bufferMsg);
//      Uart.SendDataBuffer(skadi.uartModule, errMsgWrongCmd, sizeof(errMsgWrongCmd));
    }
  }
  else
  {
    return;
  }
}


static inline void SkadiInit( sSkadiCommand_t * extCommandTable, size_t len, UartModule_t uartModule, BOOL oUseUartInterrupts )
{
  skadi.externalCommandTable.length = len;
  skadi.externalCommandTable.elements = extCommandTable;
  skadi.uartModule = uartModule;

  if (oUseUartInterrupts)       // Different functions are used if the user uses the UART with interrupts
  {
    skadiInternalCommandTableElements[1].proc = SkadiHelpCommandFifo;
  }
  else
  {
    skadiInternalCommandTableElements[1].proc = SkadiHelpCommand;
  }
}


//==============================================================================
// Private functions declarations
//==============================================================================
static char * MakeStr( char const *begin, char const *end )
{
  size_t len = end-begin;
  char *ret = malloc( len+1 );
  if ( NULL != ret )
  {
    memcpy(ret, begin, len);
    ret[len]='\0';
  }
  return ret;
}


static inline sSkadiCommand_t* SkadiLookupCommand( const char* commandName )
{
  // Look in the extern command list (if a command is in both list, the extern
  // command is returned.)
  int i;

  for ( i = 0; i < skadi.externalCommandTable.length; i++ )
  {
    if ( !strncmp( commandName, skadi.externalCommandTable.elements[i].name, MAX_CMD_NAME_SIZE ))
    {
      return &( skadi.externalCommandTable.elements[i] );
    }
  }

  // Look in the default command list
  for ( i = 0; i < skadiInternalCommandTable.length; i++ )
  {
    if ( !strncmp( commandName, skadiInternalCommandTable.elements[i].name, MAX_CMD_NAME_SIZE ))
    {
        return &( skadiInternalCommandTable.elements[i] );
    }
  }

  return 0;
}


static inline int SkadiValidateArguments( sSkadiCommand_t * sCommand, sSkadiArgs_t * skadi_args )
{
  return ( sCommand->arity == skadi_args->length ||
      ( sCommand->arity < 0 && abs( sCommand->arity ) <= skadi_args->length ));
}

static inline void SkadiSplitCommandLine( const char * command_line, char ** cmd_name, sSkadiArgs_t * skadi_args )
{
  char const *start=command_line, *end=start;
  skadi_args->length = 0;

  int i;
  for ( i=0; *start && *end && i <= ( MAX_NB_ARGUMENTS + 1 ); i++ )
  {
    while ( NULL != strchr( COMMAND_SEPARATOR, *start ))
      ++start;

    end=start;
    while( *end && NULL == strchr(COMMAND_SEPARATOR, *end ))
      ++end;

    if ( i == 0 ) {
      *cmd_name = MakeStr( start, end );
    }
    else
    {
      skadi_args->length ++;
      skadi_args->elements[ skadi_args->length - 1 ] = MakeStr( start, end );
    }

    start = end + 1;
  }
}


static inline int SkadiProcessCommand( const char* command_line )
{
  char * cmd_name;
  sSkadiArgs_t skadi_args;
  int return_code = ERROR_UNKNOWN_COMMAND;
  int i;

  SkadiSplitCommandLine( command_line, &cmd_name, &skadi_args );
//  sSkadiCommand_t * sCommand = SkadiLookupCommand( skadi, cmd_name );
  sSkadiCommand_t * sCommand = SkadiLookupCommand( cmd_name );

  if ( sCommand ) {
    return_code = ERROR_WRONG_NUMBER_ARGUMENTS;
    int args_valid = SkadiValidateArguments( sCommand, &skadi_args );
    if ( args_valid ) {
//      sCommand->proc( skadi, skadi_args );
      sCommand->proc( &skadi, skadi_args );
      return_code = 1;
    }
  }

  free( cmd_name );
  for ( i = 0; i < skadi_args.length; i++ )
    free( skadi_args.elements[i] );
  skadi_args.length = 0;

  return return_code;
}


static inline void SkadiHelpCommandFifo( void )
{
  int i, j;
  INT32 err = 0;
  UINT8 bufferDefinedCmds[] = {"\n\n\n\r--- Commandes definies dans le projet ---\n\n\r\0"};
  UINT8 bufferDefaultCmds[] = {"\n\n\r--- Commandes par defaut (peut etre redefini) ---\n\n\r\0"};
  UINT8 bufferSpace[]       = {"  \0"};       // Makes the reading prettier
  UINT8 bufferDash[]        = {" - \0"};      // Makes the reading prettier

  sUartLineBuffer_t bufferMsg =
  {
     .buffer = {0}
    ,.length =  0
  };

  memcpy(&bufferMsg.buffer[0], &bufferDefinedCmds[0], sizeof(bufferDefinedCmds));
  bufferMsg.length = sizeof(bufferDefinedCmds);
  do
  {
    err = Uart.PutTxFifoBuffer(skadi.uartModule, &bufferMsg);
  } while (err < 0);

  for ( i = 0; i < skadi.externalCommandTable.length; i++ )
  {
    memcpy(&bufferMsg.buffer[0], &bufferSpace[0], sizeof(bufferSpace));
    bufferMsg.length = sizeof(bufferSpace);
    do
    {
      err = Uart.PutTxFifoBuffer(skadi.uartModule, &bufferMsg);
    } while (err < 0);

    j = 0;
    while (skadi.externalCommandTable.elements[i].name[j] != 0)
    {
      memcpy(&bufferMsg.buffer[0], &skadi.externalCommandTable.elements[i].name[j], sizeof(skadi.externalCommandTable.elements[i].name[j]));
      bufferMsg.length = sizeof(skadi.externalCommandTable.elements[i].name[j]);
      do
      {
        err = Uart.PutTxFifoBuffer(skadi.uartModule, &bufferMsg);
      } while (err < 0);
      j++;
    }

    memcpy(&bufferMsg.buffer[0], &bufferDash[0], sizeof(bufferDash));
    bufferMsg.length = sizeof(bufferDash[0]);
    do
    {
      err = Uart.PutTxFifoBuffer(skadi.uartModule, &bufferMsg);
    } while (err < 0);

    j = 0;
    while (skadi.externalCommandTable.elements[i].description[j] != 0)
    {
      memcpy(&bufferMsg.buffer[0], &skadi.externalCommandTable.elements[i].description[j], sizeof(skadi.externalCommandTable.elements[i].description[j]));
      bufferMsg.length = sizeof(skadi.externalCommandTable.elements[i].description[j]);
      do
      {
        err = Uart.PutTxFifoBuffer(skadi.uartModule, &bufferMsg);
      } while (err < 0);
      j++;
    }

    UINT8 pretty[3] = "\n\r\0";
    memcpy(&bufferMsg.buffer[0], &pretty[0], sizeof(pretty));
    bufferMsg.length = sizeof(pretty);
    do
    {
      err = Uart.PutTxFifoBuffer(skadi.uartModule, &bufferMsg);
    } while (err < 0);
  }

  memcpy(&bufferMsg.buffer[0], &bufferDefaultCmds[0], sizeof(bufferDefaultCmds));
  bufferMsg.length = sizeof(bufferDefaultCmds);
  do
  {
    err = Uart.PutTxFifoBuffer(skadi.uartModule, &bufferMsg);
  } while (err < 0);

  for ( i = 0; i < skadiInternalCommandTable.length; i++)
  {
    memcpy(&bufferMsg.buffer[0], &bufferSpace[0], sizeof(bufferSpace));
    bufferMsg.length = sizeof(bufferSpace);
    do
    {
      err = Uart.PutTxFifoBuffer(skadi.uartModule, &bufferMsg);
    } while (err < 0);

    j = 0;
    while (skadiInternalCommandTable.elements[i].name[j] != 0)
    {
      memcpy(&bufferMsg.buffer[0], &skadiInternalCommandTable.elements[i].name[j], sizeof(skadiInternalCommandTable.elements[i].name[j]));
      bufferMsg.length = sizeof(skadiInternalCommandTable.elements[i].name[j]);
      do
      {
        err = Uart.PutTxFifoBuffer(skadi.uartModule, &bufferMsg);
      } while (err < 0);
      j++;
    }

    memcpy(&bufferMsg.buffer[0], &bufferDash[0], sizeof(bufferDash));
    bufferMsg.length = sizeof(bufferDash);
    do
    {
      err = Uart.PutTxFifoBuffer(skadi.uartModule, &bufferMsg);
    } while (err < 0);

    j = 0;
    while (skadiInternalCommandTable.elements[i].description[j] != 0)
    {
      memcpy(&bufferMsg.buffer[0], &skadiInternalCommandTable.elements[i].description[j], sizeof(skadiInternalCommandTable.elements[i].description[j]));
      bufferMsg.length = sizeof(skadiInternalCommandTable.elements[i].description[j]);
      do
      {
        err = Uart.PutTxFifoBuffer(skadi.uartModule, &bufferMsg);
      } while (err < 0);
      j++;
    }

    UINT8 prettyEnd[] = "\n\r\n--- fin ---\r\n\n\n";

    memcpy(&bufferMsg.buffer[0], &prettyEnd[0], sizeof(prettyEnd));
    bufferMsg.length = sizeof(prettyEnd);
    do
    {
      err = Uart.PutTxFifoBuffer(skadi.uartModule, &bufferMsg);
    } while (err < 0);
  }
}