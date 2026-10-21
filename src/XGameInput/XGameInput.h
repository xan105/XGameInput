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

#define XGAMEINPUT_PLAYER_MAX                      8
#define XGAMEINPUT_TOUCHPAD_MAX_FINGERS            5

#define XGAMEINPUT_GAMEPAD_AXIS_MAX                32767
#define XGAMEINPUT_GAMEPAD_AXIS_MIN                -32768
#define XGAMEINPUT_GAMEPAD_TRIGGER_MAX             32767
#define XGAMEINPUT_GAMEPAD_TRIGGER_MIN             0

#define XGAMEINPUT_GAMEPAD_LEFT_STICK_DEADZONE     7849
#define XGAMEINPUT_GAMEPAD_RIGHT_STICK_DEADZONE    8689
#define XGAMEINPUT_GAMEPAD_TRIGGER_THRESHOLD       3855

#define XGAMEINPUT_BUTTON_SOUTH                   (1u << 0)     // Bottom face button (e.g. Xbox A button)
#define XGAMEINPUT_BUTTON_EAST                    (1u << 1)     // Right face button (e.g. Xbox B button)
#define XGAMEINPUT_BUTTON_WEST                    (1u << 2)     // Left face button (e.g. Xbox X button)
#define XGAMEINPUT_BUTTON_NORTH                   (1u << 3)     // Top face button (e.g. Xbox Y button)
#define XGAMEINPUT_BUTTON_SELECT                  (1u << 4)     // "Back" button
#define XGAMEINPUT_BUTTON_GUIDE                   (1u << 5)
#define XGAMEINPUT_BUTTON_START                   (1u << 6)
#define XGAMEINPUT_BUTTON_LEFT_STICK              (1u << 7)
#define XGAMEINPUT_BUTTON_RIGHT_STICK             (1u << 8)
#define XGAMEINPUT_BUTTON_LEFT_SHOULDER           (1u << 9)
#define XGAMEINPUT_BUTTON_RIGHT_SHOULDER          (1u << 10)
#define XGAMEINPUT_BUTTON_DPAD_UP                 (1u << 11)
#define XGAMEINPUT_BUTTON_DPAD_DOWN               (1u << 12)
#define XGAMEINPUT_BUTTON_DPAD_LEFT               (1u << 13)
#define XGAMEINPUT_BUTTON_DPAD_RIGHT              (1u << 14)
#define XGAMEINPUT_BUTTON_RIGHT_PADDLE1           (1u << 15)    // Upper or primary paddle, under your right hand (e.g. Xbox Elite paddle P1, DualSense Edge RB button, Right Joy-Con SR button, Steam Controller R4 button)
#define XGAMEINPUT_BUTTON_LEFT_PADDLE1            (1u << 16)    // Upper or primary paddle, under your left hand (e.g. Xbox Elite paddle P3, DualSense Edge LB button, Left Joy-Con SL button, Steam Controller L4 button)
#define XGAMEINPUT_BUTTON_RIGHT_PADDLE2           (1u << 17)    // Lower or secondary paddle, under your right hand (e.g. Xbox Elite paddle P2, DualSense Edge right Fn button, Right Joy-Con SL button, Steam Controller R5 button)
#define XGAMEINPUT_BUTTON_LEFT_PADDLE2            (1u << 18)    // Lower or secondary paddle, under your left hand (e.g. Xbox Elite paddle P4, DualSense Edge left Fn button, Left Joy-Con SR button, Steam Controller L5 button)
#define XGAMEINPUT_BUTTON_TOUCHPAD                (1u << 19)    // PS4/PS5 touchpad button
#define XGAMEINPUT_BUTTON_MISC1                   (1u << 20)    // Additional button (e.g. Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Steam Controller QAM button, Amazon Luna microphone button, Google Stadia capture button)
#define XGAMEINPUT_BUTTON_MISC2                   (1u << 21)    // Additional button
#define XGAMEINPUT_BUTTON_MISC3                   (1u << 22)    // Additional button (e.g. Nintendo GameCube left trigger click)
#define XGAMEINPUT_BUTTON_MISC4                   (1u << 23)    // Additional button (e.g. Nintendo GameCube right trigger click)
#define XGAMEINPUT_BUTTON_MISC5                   (1u << 24)    // Additional button
#define XGAMEINPUT_BUTTON_MISC6                   (1u << 25)    // Additional button

#define XGAMEINPUT_IS_WIRELESS                    (1u << 0)
#define XGAMEINPUT_HAS_RUMBLE                     (1u << 1)
#define XGAMEINPUT_HAS_TRIGGER_RUMBLE             (1u << 2)
#define XGAMEINPUT_HAS_LED                        (1u << 3)
#define XGAMEINPUT_HAS_PLAYER_LED                 (1u << 5)
#define XGAMEINPUT_HAS_TOUCHPAD                   (1u << 5)
#define XGAMEINPUT_HAS_ACCEL                      (1u << 6)
#define XGAMEINPUT_HAS_GYRO                       (1u << 7)
#define XGAMEINPUT_HAS_CAPSENSE_STICK             (1u << 8)     // Capacitive sensor: activated by touching the top of an analog stick
#define XGAMEINPUT_HAS_CAPSENSE_GRIP              (1u << 9)     // Capacitive sensor: activated by gripping the handle of the controller (e.g. Steam controller Grip sense)

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

typedef struct _XGAMEINPUT_GAMEPAD
{
    DWORD   Buttons;
    SHORT   TriggerLeft;
    SHORT   TriggerRight;
    SHORT   StickLeftX;
    SHORT   StickLeftY;
    SHORT   StickRightX;
    SHORT   StickRightY;
} XGAMEINPUT_GAMEPAD, *PXGAMEINPUT_GAMEPAD;

typedef struct _XGAMEINPUT_STATE
{
    DWORD               PacketNumber;
    XGAMEINPUT_GAMEPAD  Gamepad;
} XGAMEINPUT_STATE, *PXGAMEINPUT_STATE;

typedef struct _XGAMEINPUT_VIBRATION
{
    WORD    LeftMotorSpeed;
    WORD    RightMotorSpeed;
    WORD    LeftTriggerSpeed;
    WORD    RightTriggerSpeed;
} XGAMEINPUT_VIBRATION, *PXGAMEINPUT_VIBRATION;

typedef struct _XGAMEINPUT_GAMEPAD_LED
{
    BYTE    Red;
    BYTE    Green;
    BYTE    Blue;
} XGAMEINPUT_GAMEPAD_LED, *PXGAMEINPUT_GAMEPAD_LED;

typedef struct _XGAMEINPUT_BATTERY_INFORMATION {
  XGAMEINPUT_BATTERY_STATE  State;
  BYTE                      Percent;
} XGAMEINPUT_BATTERY_INFORMATION, *PXGAMEINPUT_BATTERY_INFORMATION;

typedef struct _XGAMEINPUT_CAPABILITIES
{
    XGAMEINPUT_GAMEPAD_TYPE Type;
    WORD                    VendorId;
    WORD                    ProductId;
    WORD                    ProductVersion;
    WORD                    Flags;
    DWORD                   nbrButtons;
    WORD                    nbrTouchPads;
} XGAMEINPUT_CAPABILITIES, *PXGAMEINPUT_CAPABILITIES;

typedef struct _XGAMEINPUT_TOUCHPAD_FINGER
{
    bool Down;
    float X;
    float Y;
    float Pressure;
} XGAMEINPUT_TOUCHPAD_FINGER, *PXGAMEINPUT_TOUCHPAD_FINGER;

typedef struct _XGAMEINPUT_TOUCHPAD_STATE
{
    WORD                        FingerCount;
    XGAMEINPUT_TOUCHPAD_FINGER  Fingers[XGAMEINPUT_TOUCHPAD_MAX_FINGERS];
} XGAMEINPUT_TOUCHPAD_STATE, *PXGAMEINPUT_TOUCHPAD_STATE;

typedef DWORD(WINAPI* XGameInputGetState_t)(DWORD PlayerIndex, XGAMEINPUT_STATE* pState);
typedef DWORD(WINAPI* XGameInputSetState_t)(DWORD PlayerIndex, const XGAMEINPUT_VIBRATION* pVibration);
typedef DWORD(WINAPI* XGameInputSetLED_t)(DWORD PlayerIndex, const XGAMEINPUT_GAMEPAD_LED* pRGB);
typedef DWORD(WINAPI* XGameInputGetBatteryInfo_t)(DWORD PlayerIndex, XGAMEINPUT_BATTERY_INFORMATION *pBatteryInfo);
typedef DWORD(WINAPI* XGameInputGetCapabilities_t)(DWORD PlayerIndex, XGAMEINPUT_CAPABILITIES* pCapabilities);
typedef DWORD(WINAPI* XGameInputGetSteamHandle_t)(DWORD PlayerIndex, UINT64* pInputHandle);
typedef DWORD(WINAPI* XGameInputGetTouchpadState_t)(DWORD PlayerIndex, WORD TouchPadIndex, XGAMEINPUT_TOUCHPAD_STATE* pTouchPadState);

#ifdef __cplusplus
extern "C" {
#endif
    DWORD WINAPI XGameInputGetState(DWORD PlayerIndex, XGAMEINPUT_STATE* pState);
    DWORD WINAPI XGameInputSetState(DWORD PlayerIndex, const XGAMEINPUT_VIBRATION* pVibration);
    DWORD WINAPI XGameInputSetLED(DWORD PlayerIndex, const XGAMEINPUT_GAMEPAD_LED* pRGB);
    DWORD WINAPI XGameInputGetBatteryInfo(DWORD PlayerIndex, XGAMEINPUT_BATTERY_INFORMATION *pBatteryInfo);
    DWORD WINAPI XGameInputGetCapabilities(DWORD PlayerIndex, XGAMEINPUT_CAPABILITIES* pCapabilities);
    DWORD WINAPI XGameInputGetSteamHandle(DWORD PlayerIndex, UINT64* pInputHandle);
    DWORD WINAPI XGameInputGetTouchpadState(DWORD PlayerIndex, WORD TouchPadIndex, XGAMEINPUT_TOUCHPAD_STATE* pTouchPadState);
#ifdef __cplusplus
}
#endif