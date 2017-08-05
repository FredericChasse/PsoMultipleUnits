/**********************************************************************
* © 2007 Microchip Technology Inc.
*
* FileName:        ChinookSteeringWheelUtils.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC32
* Compiler:        MPLAB® XC32
*
************************************************************************/
#include "..\headers\ChinookSteeringWheelUtils.h"

#ifndef __32MX320F128H__    // Uno32 doesn't have CAN


void registerSystemForCalibration(int p_System, void (*p_CalibrationCallback)(void))
{
    if(m_iCurSystem == -1)
    {
        m_iCurSystem = 0;
        m_iActivateCalibration = 0;
        m_iUserSystem = p_System;
        m_CalibrationCallback = p_CalibrationCallback;
    }
}

void steeringWheelCommandReceived(UINT16 canMessage)
{
    if(isCalibrationCommand(canMessage))
    {        
        m_iCurSystem++;
        
        if(m_iCurSystem >= MAX_CALIB)
        {
            m_iCurSystem = NO_CALIB;
        }
    }
    if(isCalibrationCommand())
    {
        if(m_iCurSystem == m_iUserSystem)
        {        
            m_CalibrationCallback();
        }
    }
}

int isCalibrationToggleCommand(UINT16 canMessage)
{
    return isCommand(canMessage, CALIB_TOGGLE_BUTTON);
}

int isCalibrationCommand(UINT16 canMessage)
{
   return isCommand(canMessage, CALIB_ENGAGE_BUTTON);
}

int isCommand(UINT16 canMessage, UINT16 button)
{
    return (canMessage & button) == button;
}

#endif
