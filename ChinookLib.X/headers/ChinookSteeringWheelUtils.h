/* UNO32 DOESN'T HAVE CAN */


/**********************************************************************
* enhen this ma shit
***********************************************************************/

#ifndef __CHINOOK_STEERING_WHEEL_UTILS__
#define __CHINOOK_STEERING_WHEEL_UTILS__

#ifndef __32MX320F128H__    // Uno32 doesn't have CAN

enum CALIB_MODES
{
    NO_CALIB = 0,
    MAST_CALIB,
    WEATHERCOCK_CALIB,
    GEAR_CALIB,
    PITCH_CALIB,
    
    //keep this one last
    MAX_CALIB
};


#define CALIB_ENGAGE_BUTTON     0b000000000001
#define PITCH_MINUS_BUTTON      0b000000000010
#define HORN_BUTTON             0b000000000100
#define DISPLAY_RIGHT_BUTTON    0b000000001000
#define DISPLAY_LEFT_BUTTON     0b000000010000
#define MAST_MODE_BUTTON        0b000000100000
#define CALIB_TOGGLE_BUTTON     0b000001000000
//#define MAST ORIENTATION 8
#define AEROBRAKE_BUTTON        0b000010000000
#define PITCH_PLUS_BUTTON       0b000100000000
#define AQUISITION_BUTTON       0b001000000000
#define GEAR_DOWN_BUTTON        0b010000000000
#define GEAR_UP_BUTTON          0b100000000000

int m_iCurSystem = -1;
int m_iUserSystem;
int m_iActivateCalibration;
void (*m_CalibrationCallback)(void);

void registerSystemForCalibration(int p_System, void* p_CalibrationCallback);

void steeringWheelCommandReceived(UINT16 canMessage);

int isCalibrationCommand();

int isCalibrationToggleCommand(UINT16 canMessage);

int isCalibrationCommand(UINT16 canMessage);

int isCommand(UINT16 canMessage, UINT16 button);

#endif  /* not defined Uno32 */
#endif	/* __CHINOOK_STEERING_WHEEL_UTILS__ */
