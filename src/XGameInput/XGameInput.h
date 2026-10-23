/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.

This file is based upon the original source code of
[InputFusion](https://github.com/xan105/InputFusion)
distributed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3.

Original source: https://github.com/xan105/InputFusion.git
Mirror: https://download.xan105.com/InputFusion/Source%20code.zip
*/

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#ifndef __cplusplus
    #include <stdbool.h>
#endif

#define XGAMEINPUT_PLAYER_MAX                      8            // Maximum number of supported controllers.
#define XGAMEINPUT_TOUCHPAD_MAX_FINGERS            5            // Maximum supported simultaneous fingers on a touchpad.
#define XGAMEINPUT_MOTION_SENSORS_AXIS             3            // X,Y,Z

#define XGAMEINPUT_GAMEPAD_AXIS_MAX                32767        // The largest value an axis can report.
#define XGAMEINPUT_GAMEPAD_AXIS_MIN                -32768       // The smallest value an axis can report.
#define XGAMEINPUT_GAMEPAD_TRIGGER_MAX             32767        // Triggers when fully pressed.
#define XGAMEINPUT_GAMEPAD_TRIGGER_MIN             0            // Triggers when released.

#define XGAMEINPUT_GAMEPAD_LEFT_STICK_DEADZONE     7849         // (Optional) You may define your own dead zones for the sticks or you may use the provided deadzones values.
#define XGAMEINPUT_GAMEPAD_RIGHT_STICK_DEADZONE    8689         // (Optional) You may define your own dead zones for the sticks or you may use the provided deadzones values.
#define XGAMEINPUT_GAMEPAD_TRIGGER_THRESHOLD       3855         // (Optional) Value which a Trigger must be greater than to register as pressed. You may define your own value or you may use the provided value.

#define XGAMEINPUT_BUTTON_SOUTH                   (1u << 0)     // Bottom face button (e.g. Xbox A button, Playstation 🗙).
#define XGAMEINPUT_BUTTON_EAST                    (1u << 1)     // Right face button (e.g. Xbox B button, Playstation ●).
#define XGAMEINPUT_BUTTON_WEST                    (1u << 2)     // Left face button (e.g. Xbox X button, Playstation ■).
#define XGAMEINPUT_BUTTON_NORTH                   (1u << 3)     // Top face button (e.g. Xbox Y button, Playstation ▲).
#define XGAMEINPUT_BUTTON_SELECT                  (1u << 4)     // Also known as the "Back" button.
#define XGAMEINPUT_BUTTON_GUIDE                   (1u << 5)     // The large, central button located in the middle of your controller. Often referred to as the Xbox button, the PlayStation/PS button, or the Steam button.
#define XGAMEINPUT_BUTTON_START                   (1u << 6)
#define XGAMEINPUT_BUTTON_LEFT_STICK              (1u << 7)     // Also known as "L3" on Playstation controller.
#define XGAMEINPUT_BUTTON_RIGHT_STICK             (1u << 8)     // Also known as "R3" on Playstation controller.
#define XGAMEINPUT_BUTTON_LEFT_SHOULDER           (1u << 9)     // Also known as "L1" on Playstation controller.
#define XGAMEINPUT_BUTTON_RIGHT_SHOULDER          (1u << 10)    // Also known as "L2" on Playstation controller.
#define XGAMEINPUT_BUTTON_DPAD_UP                 (1u << 11)
#define XGAMEINPUT_BUTTON_DPAD_DOWN               (1u << 12)
#define XGAMEINPUT_BUTTON_DPAD_LEFT               (1u << 13)
#define XGAMEINPUT_BUTTON_DPAD_RIGHT              (1u << 14)
#define XGAMEINPUT_BUTTON_RIGHT_PADDLE1           (1u << 15)    // Upper or primary paddle, under your right hand (e.g. Xbox Elite paddle P1, DualSense Edge RB button, Right Joy-Con SR button, Steam Controller R4 button).
#define XGAMEINPUT_BUTTON_LEFT_PADDLE1            (1u << 16)    // Upper or primary paddle, under your left hand (e.g. Xbox Elite paddle P3, DualSense Edge LB button, Left Joy-Con SL button, Steam Controller L4 button).
#define XGAMEINPUT_BUTTON_RIGHT_PADDLE2           (1u << 17)    // Lower or secondary paddle, under your right hand (e.g. Xbox Elite paddle P2, DualSense Edge right Fn button, Right Joy-Con SL button, Steam Controller R5 button).
#define XGAMEINPUT_BUTTON_LEFT_PADDLE2            (1u << 18)    // Lower or secondary paddle, under your left hand (e.g. Xbox Elite paddle P4, DualSense Edge left Fn button, Left Joy-Con SR button, Steam Controller L5 button).
#define XGAMEINPUT_BUTTON_TOUCHPAD                (1u << 19)    // PS4/PS5 touchpad button.
#define XGAMEINPUT_BUTTON_MISC1                   (1u << 20)    // Additional button (e.g. Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Steam Controller QAM button, Amazon Luna microphone button, Google Stadia capture button).
#define XGAMEINPUT_BUTTON_MISC2                   (1u << 21)    // Additional button.
#define XGAMEINPUT_BUTTON_MISC3                   (1u << 22)    // Additional button (e.g. Nintendo GameCube left trigger click).
#define XGAMEINPUT_BUTTON_MISC4                   (1u << 23)    // Additional button (e.g. Nintendo GameCube right trigger click).
#define XGAMEINPUT_BUTTON_MISC5                   (1u << 24)    // Additional button.
#define XGAMEINPUT_BUTTON_MISC6                   (1u << 25)    // Additional button.

#define XGAMEINPUT_IS_WIRELESS                    (1u << 0)     // The gamepad is connected wirelessly.
#define XGAMEINPUT_HAS_RUMBLE                     (1u << 1)     // The gamepad is equipped with vibration motors.
#define XGAMEINPUT_HAS_TRIGGER_RUMBLE             (1u << 2)     // The gamepad is equipped with impulse triggers.
#define XGAMEINPUT_HAS_LED                        (1u << 3)     // The gamepad is equipped with a programmable LED (e.g. the light on the back of a PlayStation 4's DualShock 4 controller).
#define XGAMEINPUT_HAS_PLAYER_LED                 (1u << 5)     // The gamepad is equipped with a LED to indicate the player number.
#define XGAMEINPUT_HAS_TOUCHPAD                   (1u << 6)     // The gamepad is equipped with one or more touchpad.
#define XGAMEINPUT_HAS_ACCEL                      (1u << 7)     // The gamepad is equipped with one or more accelerometer.
#define XGAMEINPUT_HAS_GYRO                       (1u << 8)     // The gamepad is equipped with one or more gyroscope.
#define XGAMEINPUT_HAS_CAPSENSE_STICK             (1u << 9)     // The gamepad is equipped with one or more capacitive sensor activated by touching the top of an analog stick.
#define XGAMEINPUT_HAS_CAPSENSE_GRIP              (1u << 10)    // The gamepad is equipped with one or more capacitive sensor activated by gripping the handle of the controller (e.g. Steam controller Grip sense).

typedef enum _XGAMEINPUT_GAMEPAD_TYPE
{
    XGAMEINPUT_GAMEPAD_TYPE_UNKNOWN = 0,
    XGAMEINPUT_GAMEPAD_TYPE_XBOX360,
    XGAMEINPUT_GAMEPAD_TYPE_XBOXONE,
    XGAMEINPUT_GAMEPAD_TYPE_PS3,
    XGAMEINPUT_GAMEPAD_TYPE_PS4,
    XGAMEINPUT_GAMEPAD_TYPE_PS5,
    XGAMEINPUT_GAMEPAD_TYPE_SWITCH_PRO,
    XGAMEINPUT_GAMEPAD_TYPE_STEAM
} XGAMEINPUT_GAMEPAD_TYPE;

typedef enum _XGAMEINPUT_BATTERY_STATE
{
    XGAMEINPUT_BATTERY_STATE_UNKNOWN = 0,
    XGAMEINPUT_BATTERY_STATE_CHARGED,       
    XGAMEINPUT_BATTERY_STATE_CHARGING,
    XGAMEINPUT_BATTERY_STATE_DISCHARGING
} XGAMEINPUT_BATTERY_STATE;

typedef struct _XGAMEINPUT_GAMEPAD                              // Represents the current state of a controller (button press, axis and trigger values).
{
    DWORD   Buttons;                                            // Bitmask of the device digital buttons. A set bit indicates that the corresponding button is pressed.
    SHORT   TriggerLeft;                                        // The current value of the left trigger. The value is between 0 (released) and 32767 (fully pressed). 
                                                                // (Optional) The constant `XGAMEINPUT_GAMEPAD_TRIGGER_THRESHOLD` may be used as the value which a Trigger must be greater than to register as pressed.
    SHORT   TriggerRight;                                       // The current value of the right trigger. The value is between 0 (released) and 32767 (fully pressed). 
                                                                // (Optional) The constant `XGAMEINPUT_GAMEPAD_TRIGGER_THRESHOLD` may be used as the value which a Trigger must be greater than to register as pressed.
    SHORT   StickLeftX;                                         // Left stick X-axis value. The value is between -32768 (left) and 32767 (right). A value of 0 is centered. 
                                                                // (Optional) The constants `XGAMEINPUT_GAMEPAD_LEFT_STICK_DEADZONE` or `XGAMEINPUT_GAMEPAD_RIGHT_STICK_DEADZONE` may be used as a positive and negative value to filter a stick input.
    SHORT   StickLeftY;                                         // Left stick Y-axis value. The value is between -32768 (up) and 32767 (down). A value of 0 is centered. 
                                                                // (Optional) The constants `XGAMEINPUT_GAMEPAD_LEFT_STICK_DEADZONE` or `XGAMEINPUT_GAMEPAD_RIGHT_STICK_DEADZONE` may be used as a positive and negative value to filter a stick input.
    SHORT   StickRightX;                                        // Right stick X-axis value. The value is between -32768 (left) and 32767 (right). A value of 0 is centered. 
                                                                // (Optional) The constants `XGAMEINPUT_GAMEPAD_LEFT_STICK_DEADZONE` or `XGAMEINPUT_GAMEPAD_RIGHT_STICK_DEADZONE` may be used as a positive and negative value to filter a stick input.   
    SHORT   StickRightY;                                        // Right stick Y-axis value. The value is between -32768 (up) and 32767 (down). A value of 0 is centered. 
                                                                // (Optional) The constants `XGAMEINPUT_GAMEPAD_LEFT_STICK_DEADZONE` or `XGAMEINPUT_GAMEPAD_RIGHT_STICK_DEADZONE` may be used as a positive and negative value to filter a stick input.
} XGAMEINPUT_GAMEPAD, *PXGAMEINPUT_GAMEPAD;

typedef struct _XGAMEINPUT_VIBRATION                            // Specifies motor speed levels of a controller.
{
    WORD    LeftMotorSpeed;                                     // Speed of the left motor. Valid values range from 0 through 65535. Zero signifies that the motor is not used at all, and 65535 signifies that the motor is used at 100 percent.
    WORD    RightMotorSpeed;                                    // Speed of the right motor. Valid values range from 0 through 65535. Zero signifies that the motor is not used at all, and 65535 signifies that the motor is used at 100 percent.
    WORD    LeftTriggerSpeed;                                   // Speed of the left trigger motor. Valid values range from 0 through 65535. Zero signifies that the motor is not used at all, and 65535 signifies that the motor is used at 100 percent.
    WORD    RightTriggerSpeed;                                  // Speed of the right trigger motor. Valid values range from 0 through 65535. Zero signifies that the motor is not used at all, and 65535 signifies that the motor is used at 100 percent.
} XGAMEINPUT_VIBRATION, *PXGAMEINPUT_VIBRATION;

typedef struct _XGAMEINPUT_GAMEPAD_LED                          // Specifies a gamepad's LED color. For gamepads with a single color LED, the maximum of the RGB values will be used as the LED brightness.
{
    BYTE    Red;                                                // The intensity of the red LED.
    BYTE    Green;                                              // The intensity of the green LED.
    BYTE    Blue;                                               // The intensity of the blue LED.
} XGAMEINPUT_GAMEPAD_LED, *PXGAMEINPUT_GAMEPAD_LED;

typedef struct _XGAMEINPUT_BATTERY_INFORMATION {                // Contains information on battery and charge state.
  XGAMEINPUT_BATTERY_STATE  State;                              // The current battery state.
  BYTE                      Percent;                            // The percentage of battery life left, between 0 and 100. -1 signifies that it was impossible to determine a value or there is no battery.
} XGAMEINPUT_BATTERY_INFORMATION, *PXGAMEINPUT_BATTERY_INFORMATION;

typedef struct _XGAMEINPUT_CAPABILITIES                         // Describes the capabilities of a controller.
{
    XGAMEINPUT_GAMEPAD_TYPE Type;
    WORD                    VendorId;                           // Identification number of the manufacturer or company behind the equipment.
    WORD                    ProductId;                          // Identification number defined by the manufacturer itself, it identifies a specific model or variant of its product.
    WORD                    ProductVersion;                     // Indicates the hardware or firmware revision of the device.
    WORD                    Flags;                              // Features of the controller.
    DWORD                   nbrButtons;                         // How many supported buttons are available on the device.
    WORD                    nbrTouchPads;                       // How many touchpads are available on the device.
} XGAMEINPUT_CAPABILITIES, *PXGAMEINPUT_CAPABILITIES;

typedef struct _XGAMEINPUT_TOUCHPAD_FINGER                      // Represents the current state of a finger on a touchpad.
{
    bool Down;                                                  // True if the finger is down, false otherwise.
    float X;                                                    // X position, normalized 0 to 1, with the origin in the upper left.
    float Y;                                                    // Y position, normalized 0 to 1, with the origin in the upper left.
    float Pressure;                                             // Pressure value.
} XGAMEINPUT_TOUCHPAD_FINGER, *PXGAMEINPUT_TOUCHPAD_FINGER;

typedef struct _XGAMEINPUT_TOUCHPAD                             // Represents the current state of a touchpad.
{
    WORD                        FingerCount;                              // The count represents the number of successfully queried finger slots, not the number of fingers currently touching the pad. Each slot's `Down` field tells you whether that finger is touching it.
    XGAMEINPUT_TOUCHPAD_FINGER  Fingers[XGAMEINPUT_TOUCHPAD_MAX_FINGERS]; // Contains the current state of fingers of a touchpad.
} XGAMEINPUT_TOUCHPAD, *PXGAMEINPUT_TOUCHPAD;

typedef struct _XGAMEINPUT_MOTION_SENSORS
{
  float Accel[XGAMEINPUT_MOTION_SENSORS_AXIS];                  // Accelerometer (X,Y,Z).
  float AccelRate;                                              // Accelerometer data rate (number of events per second).
  float AccelLeft[XGAMEINPUT_MOTION_SENSORS_AXIS];              // Accelerometer for left Joy-Con controller and Wii nunchuk (X,Y,Z).
  float AccelLeftRate;                                          // Accelerometer for left Joy-Con controller and Wii nunchuk data rate (number of events per second).
  float AccelRight[XGAMEINPUT_MOTION_SENSORS_AXIS];             // Accelerometer for right Joy-Con controller (X,Y,Z).
  float AccelRightRate;                                         // Accelerometer for right Joy-Con controller data rate (number of events per second).
  float Gyro[XGAMEINPUT_MOTION_SENSORS_AXIS];                   // Gyroscope (X,Y,Z).
  float GyroRate;                                               // Gyroscope data rate (number of events per second).
  float GyroLeft[XGAMEINPUT_MOTION_SENSORS_AXIS];               // Gyroscope for left Joy-Con controller (X,Y,Z).
  float GyroLeftRate;                                           // Gyroscope for left Joy-Con controller data rate (number of events per second).
  float GyroRight[XGAMEINPUT_MOTION_SENSORS_AXIS];              // Gyroscope for right Joy-Con controller (X,Y,Z).
  float GyroRightRate;                                          // Gyroscope for right Joy-Con controller data rate (number of events per second).
} XGAMEINPUT_MOTION_SENSORS, *PXGAMEINPUT_MOTION_SENSORS;

// Dualsense (DS5)
typedef enum _XGAMEINPUT_TRIGGER_EFFECT {
  XGAMEINPUT_TRIGGER_EFFECT_BOTH = 0,
  XGAMEINPUT_TRIGGER_EFFECT_LEFT,
  XGAMEINPUT_TRIGGER_EFFECT_RIGHT,
} XGAMEINPUT_TRIGGER_EFFECT;

typedef enum _XGAMEINPUT_TRIGGER_EFFECT_MODE{                   // Offically recognized modes
  XGAMEINPUT_TRIGGER_EFFECT_MODE_OFF = 0,     
  XGAMEINPUT_TRIGGER_EFFECT_MODE_FEEDBACK,                      // Constant resistance from a zone to the end of the travel. The simplest "heavy trigger".
  XGAMEINPUT_TRIGGER_EFFECT_MODE_WEAPON,                        // Resistance between two zones that gives way with a click, like a real gun trigger.
  XGAMEINPUT_TRIGGER_EFFECT_MODE_VIBRATION                      // The trigger vibrates from a zone to the end of the travel.
} XGAMEINPUT_TRIGGER_EFFECT_MODE;

typedef struct _XGAMEINPUT_TRIGGER_EFFECT_PARAM {
  BYTE position;	                                            // Zone (position)	[0~9]: The trigger travel split into 10 zones. 0 is released, 9 is fully pressed. 
  BYTE strength;	                                            // Resistance force [0~8]: 1 is barely there, 8 is the strongest the motor can push. 0 is same as Off mode.
  BYTE amplitude;                                               // Amplitude	[0~8]: Vibration intensity. 0 is same as Off mode.
  BYTE frequency;                                               // Frequency	[0~255]: Vibration frequency in Hz. 0 is same as Off mode.
  BYTE start;                                                   // Position [2~7] where the stiffness of trigger start changing.
  BYTE end;                                                     // Position [start+1~8] where the stiffness of trigger finish changing
} XGAMEINPUT_TRIGGER_EFFECT_PARAM;

// Common
typedef DWORD(WINAPI* XGameInputGetState_t)(DWORD PlayerIndex, XGAMEINPUT_GAMEPAD* pGamepad);
typedef DWORD(WINAPI* XGameInputSetState_t)(DWORD PlayerIndex, const XGAMEINPUT_VIBRATION* pVibration);
typedef DWORD(WINAPI* XGameInputGetCapabilities_t)(DWORD PlayerIndex, XGAMEINPUT_CAPABILITIES* pCapabilities);
typedef DWORD(WINAPI* XGameInputGetBatteryInfo_t)(DWORD PlayerIndex, XGAMEINPUT_BATTERY_INFORMATION *pBatteryInfo);
// Steam Input
typedef DWORD(WINAPI* XGameInputGetSteamHandle_t)(DWORD PlayerIndex, UINT64* pInputHandle);
// Hardware specific
typedef DWORD(WINAPI* XGameInputSetLED_t)(DWORD PlayerIndex, const XGAMEINPUT_GAMEPAD_LED* pRGB);
typedef DWORD(WINAPI* XGameInputGetTouchpad_t)(DWORD PlayerIndex, WORD TouchPadIndex, XGAMEINPUT_TOUCHPAD* pTouchPad);
typedef DWORD(WINAPI* XGameInputGetMotionSensors_t)(DWORD PlayerIndex, XGAMEINPUT_MOTION_SENSORS* pSensors);
typedef DWORD(WINAPI* XGameInputSetTriggerEffect_t)(
    DWORD PlayerIndex,
    XGAMEINPUT_TRIGGER_EFFECT trigger,
    XGAMEINPUT_TRIGGER_EFFECT_MODE mode,
    const XGAMEINPUT_TRIGGER_EFFECT_PARAM* pEffectParam);

#ifdef __cplusplus
extern "C" {
#endif
    // Common
    DWORD WINAPI XGameInputGetState(DWORD PlayerIndex, XGAMEINPUT_GAMEPAD* pGamepad);                                           // Retrieves the current state of the specified controller.
    DWORD WINAPI XGameInputSetState(DWORD PlayerIndex, const XGAMEINPUT_VIBRATION* pVibration);                                 // Sends data to the specified controller. This function is used to activate vibration motors.
    DWORD WINAPI XGameInputGetCapabilities(DWORD PlayerIndex, XGAMEINPUT_CAPABILITIES* pCapabilities);                          // Retrieves the capabilities and features of the specified controller.
    DWORD WINAPI XGameInputGetBatteryInfo(DWORD PlayerIndex, XGAMEINPUT_BATTERY_INFORMATION *pBatteryInfo);                     // Retrieves information on battery and charge state of the specified controller.
    
    // Steam Input
    DWORD WINAPI XGameInputGetSteamHandle(DWORD PlayerIndex, UINT64* pInputHandle);                                             // Returns an `InputHandle_t` for the specified controller that can be used with the Steam Input API.
    
    // Hardware specific
    DWORD WINAPI XGameInputSetLED(DWORD PlayerIndex, const XGAMEINPUT_GAMEPAD_LED* pRGB);                                       // Update the specified controller's LED color.
    DWORD WINAPI XGameInputGetTouchpad(DWORD PlayerIndex, WORD TouchPadIndex, XGAMEINPUT_TOUCHPAD* pTouchPad);                  // Retrieves the current state of the specified touchpad of the specified controller.
    DWORD WINAPI XGameInputGetMotionSensors(DWORD PlayerIndex, XGAMEINPUT_MOTION_SENSORS* pSensors);                            // Retrieves the current state of the motions sensors of the specified controller.
    DWORD WINAPI XGameInputSetTriggerEffect(
        DWORD PlayerIndex,
        XGAMEINPUT_TRIGGER_EFFECT trigger,
        XGAMEINPUT_TRIGGER_EFFECT_MODE mode,
        const XGAMEINPUT_TRIGGER_EFFECT_PARAM* pEffectParam);                                                                   // Dualsense (DS5) only | Send a trigger effect to the specified trigger on the specified controller.   
                                                                                                                                // pEffectParam can be null when mode is off.
#ifdef __cplusplus
}
#endif