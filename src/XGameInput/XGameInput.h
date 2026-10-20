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
#include <SDL3/SDL.h>
#include <unordered_map>
#include <algorithm>
#include <atomic>

#define XGAMEINPUT_PLAYER_MAX               4

#define XGAMEINPUT_GAMEPAD_AXIS_MAX         32767
#define XGAMEINPUT_GAMEPAD_AXIS_MIN         -32768

#define XGAMEINPUT_GAMEPAD_BUTTON_SOUTH            (1u << 0)  
#define XGAMEINPUT_GAMEPAD_BUTTON_EAST             (1u << 1)
#define XGAMEINPUT_GAMEPAD_BUTTON_WEST             (1u << 2)
#define XGAMEINPUT_GAMEPAD_BUTTON_NORTH            (1u << 3)
#define XGAMEINPUT_GAMEPAD_BUTTON_SELECT           (1u << 4)
#define XGAMEINPUT_GAMEPAD_BUTTON_GUIDE            (1u << 5)
#define XGAMEINPUT_GAMEPAD_BUTTON_START            (1u << 6)
#define XGAMEINPUT_GAMEPAD_BUTTON_LEFT_STICK       (1u << 7)
#define XGAMEINPUT_GAMEPAD_BUTTON_RIGHT_STICK      (1u << 8)
#define XGAMEINPUT_GAMEPAD_BUTTON_LEFT_SHOULDER    (1u << 9)
#define XGAMEINPUT_GAMEPAD_BUTTON_RIGHT_SHOULDER   (1u << 10)
#define XGAMEINPUT_GAMEPAD_BUTTON_DPAD_UP          (1u << 11)
#define XGAMEINPUT_GAMEPAD_BUTTON_DPAD_DOWN        (1u << 12)
#define XGAMEINPUT_GAMEPAD_BUTTON_DPAD_LEFT        (1u << 13)
#define XGAMEINPUT_GAMEPAD_BUTTON_DPAD_RIGHT       (1u << 14)
#define XGAMEINPUT_GAMEPAD_BUTTON_RIGHT_PADDLE1    (1u << 15)
#define XGAMEINPUT_GAMEPAD_BUTTON_LEFT_PADDLE1     (1u << 16)
#define XGAMEINPUT_GAMEPAD_BUTTON_RIGHT_PADDLE2    (1u << 17)
#define XGAMEINPUT_GAMEPAD_BUTTON_LEFT_PADDLE2     (1u << 18) 
#define XGAMEINPUT_GAMEPAD_BUTTON_TOUCHPAD         (1u << 19)  
#define XGAMEINPUT_GAMEPAD_BUTTON_MISC1            (1u << 20)
#define XGAMEINPUT_GAMEPAD_BUTTON_MISC2            (1u << 21) 
#define XGAMEINPUT_GAMEPAD_BUTTON_MISC3            (1u << 22)  
#define XGAMEINPUT_GAMEPAD_BUTTON_MISC4            (1u << 23)   
#define XGAMEINPUT_GAMEPAD_BUTTON_MISC5            (1u << 24)   
#define XGAMEINPUT_GAMEPAD_BUTTON_MISC6            (1u << 25)    








/*
#define XINPUT_CAPS_VOICE_SUPPORTED         0x0004
#define XINPUT_CAPS_FFB_SUPPORTED           0x0001
#define XINPUT_CAPS_WIRELESS                0x0002
#define XINPUT_CAPS_PMD_SUPPORTED           0x0008
#define XINPUT_CAPS_NO_NAVIGATION           0x0010


#define XINPUT_FLAG_GAMEPAD                 0x00000001

#define BATTERY_DEVTYPE_GAMEPAD             0x00
#define BATTERY_DEVTYPE_HEADSET             0x01

#define BATTERY_TYPE_DISCONNECTED           0x00
#define BATTERY_TYPE_WIRED                  0x01
#define BATTERY_TYPE_ALKALINE               0x02
#define BATTERY_TYPE_NIMH                   0x03
#define BATTERY_TYPE_UNKNOWN                0xFF

#define BATTERY_LEVEL_EMPTY                 0x00
#define BATTERY_LEVEL_LOW                   0x01
#define BATTERY_LEVEL_MEDIUM                0x02
#define BATTERY_LEVEL_FULL                  0x03
*/

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
} XGAMEINPUT_GAMEPAD_LED, * PXGAMEINPUT_GAMEPAD_LED;




/*

typedef struct _XINPUT_CAPABILITIES
{
    BYTE                Type;
    BYTE                SubType;
    WORD                Flags;
    XINPUT_GAMEPAD      Gamepad;
    XINPUT_VIBRATION    Vibration;
} XINPUT_CAPABILITIES, *PXINPUT_CAPABILITIES;

typedef struct _XINPUT_CAPABILITIES_EX
{
    XINPUT_CAPABILITIES   Capabilities;
    WORD                  VendorId;
    WORD                  ProductId;
    WORD                  ProductVersion;
    WORD                  unk1; //unknown
    DWORD                 unk2; //unknown
} XINPUT_CAPABILITIES_EX, *PXINPUT_CAPABILITIES_EX;

typedef struct _XINPUT_BATTERY_INFORMATION
{
    BYTE    BatteryType;
    BYTE    BatteryLevel;
} XINPUT_BATTERY_INFORMATION, *PXINPUT_BATTERY_INFORMATION;

*/

typedef DWORD(WINAPI* XGameInputGetState_t)(DWORD PlayerIndex, XGAMEINPUT_STATE* pState);
typedef DWORD(WINAPI* XGameInputSetState_t)(DWORD PlayerIndex, const XGAMEINPUT_VIBRATION* pVibration);
typedef DWORD(WINAPI* XGameInputSetLED_t)(DWORD PlayerIndex, const XGAMEINPUT_GAMEPAD_LED* pRGB);

extern "C" {
    DWORD WINAPI XGameInputGetState(DWORD PlayerIndex, XGAMEINPUT_STATE* pState);
    DWORD WINAPI XGameInputSetState(DWORD PlayerIndex, const XGAMEINPUT_VIBRATION* pVibration);
    DWORD WINAPI XGameInputSetLED(DWORD PlayerIndex, const XGAMEINPUT_GAMEPAD_LED* pRGB);
}