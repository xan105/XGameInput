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

#include "XGameInput.h"
#include <SDL3/SDL.h>
#include <unordered_map>
#include <atomic>

extern HANDLE InputFusion_SDL_Init_Wait;

namespace {

    constexpr auto INPUTFUSION_SDL_EVENT_LOOP_DELAY = 10;

    const std::unordered_map<SDL_GamepadButton, DWORD> BUTTONS = {
        { SDL_GAMEPAD_BUTTON_SOUTH, XGAMEINPUT_BUTTON_SOUTH },
        { SDL_GAMEPAD_BUTTON_EAST, XGAMEINPUT_BUTTON_EAST },
        { SDL_GAMEPAD_BUTTON_WEST, XGAMEINPUT_BUTTON_WEST },
        { SDL_GAMEPAD_BUTTON_NORTH, XGAMEINPUT_BUTTON_NORTH },
        { SDL_GAMEPAD_BUTTON_BACK, XGAMEINPUT_BUTTON_SELECT },
        { SDL_GAMEPAD_BUTTON_GUIDE, XGAMEINPUT_BUTTON_GUIDE },
        { SDL_GAMEPAD_BUTTON_START, XGAMEINPUT_BUTTON_START },
        { SDL_GAMEPAD_BUTTON_LEFT_STICK, XGAMEINPUT_BUTTON_LEFT_STICK },
        { SDL_GAMEPAD_BUTTON_RIGHT_STICK, XGAMEINPUT_BUTTON_RIGHT_STICK },
        { SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, XGAMEINPUT_BUTTON_LEFT_SHOULDER },
        { SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, XGAMEINPUT_BUTTON_RIGHT_SHOULDER },
        { SDL_GAMEPAD_BUTTON_DPAD_UP, XGAMEINPUT_BUTTON_DPAD_UP },
        { SDL_GAMEPAD_BUTTON_DPAD_DOWN, XGAMEINPUT_BUTTON_DPAD_DOWN },
        { SDL_GAMEPAD_BUTTON_DPAD_LEFT, XGAMEINPUT_BUTTON_DPAD_LEFT },
        { SDL_GAMEPAD_BUTTON_DPAD_RIGHT, XGAMEINPUT_BUTTON_DPAD_RIGHT },
        { SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1, XGAMEINPUT_BUTTON_RIGHT_PADDLE1 },
        { SDL_GAMEPAD_BUTTON_LEFT_PADDLE1, XGAMEINPUT_BUTTON_LEFT_PADDLE1 },
        { SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2, XGAMEINPUT_BUTTON_RIGHT_PADDLE2 },
        { SDL_GAMEPAD_BUTTON_LEFT_PADDLE2, XGAMEINPUT_BUTTON_LEFT_PADDLE2 },
        { SDL_GAMEPAD_BUTTON_TOUCHPAD, XGAMEINPUT_BUTTON_TOUCHPAD },
        { SDL_GAMEPAD_BUTTON_MISC1, XGAMEINPUT_BUTTON_MISC1 },
        { SDL_GAMEPAD_BUTTON_MISC2, XGAMEINPUT_BUTTON_MISC2 },
        { SDL_GAMEPAD_BUTTON_MISC3, XGAMEINPUT_BUTTON_MISC3 },
        { SDL_GAMEPAD_BUTTON_MISC4, XGAMEINPUT_BUTTON_MISC4 },
        { SDL_GAMEPAD_BUTTON_MISC5, XGAMEINPUT_BUTTON_MISC5 },
        { SDL_GAMEPAD_BUTTON_MISC6, XGAMEINPUT_BUTTON_MISC6 }
    };

    const std::vector<SDL_GamepadAxis> AXIS = {
        SDL_GAMEPAD_AXIS_LEFTX,
        SDL_GAMEPAD_AXIS_LEFTY,
        SDL_GAMEPAD_AXIS_RIGHTX,
        SDL_GAMEPAD_AXIS_RIGHTY,
        SDL_GAMEPAD_AXIS_LEFT_TRIGGER,
        SDL_GAMEPAD_AXIS_RIGHT_TRIGGER
    };
}

extern "C" {

    DWORD WINAPI XGameInputGetState(DWORD PlayerIndex, XGAMEINPUT_STATE* pState) {

        static std::atomic<DWORD> PacketNumber[XGAMEINPUT_PLAYER_MAX] = {};

        if (PlayerIndex >= XGAMEINPUT_PLAYER_MAX || pState == nullptr) {
            return ERROR_INVALID_PARAMETER;
        }

        if (!(SDL_WasInit(SDL_INIT_GAMEPAD) & SDL_INIT_GAMEPAD)) {
            if (InputFusion_SDL_Init_Wait) {
                WaitForSingleObject(InputFusion_SDL_Init_Wait, INFINITE);
                SDL_Delay(INPUTFUSION_SDL_EVENT_LOOP_DELAY);
            }
        }

        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(PlayerIndex);
        if (gamepad == nullptr) {
            return ERROR_DEVICE_NOT_CONNECTED;
        }

        ZeroMemory(pState, sizeof(XGAMEINPUT_STATE));
        pState->PacketNumber = ++PacketNumber[PlayerIndex];

        SDL_UpdateGamepads();

        for (const auto& [sdl_button, button] : BUTTONS) {
            if (SDL_GetGamepadButton(gamepad, sdl_button)) {
                pState->Gamepad.Buttons |= button;
            }
        }

        for (const auto& sdl_axis : AXIS) {
            SHORT value = SDL_GetGamepadAxis(gamepad, sdl_axis);
            if (sdl_axis == SDL_GAMEPAD_AXIS_LEFTX) {
                pState->Gamepad.StickLeftX = value;
            }
            else if (sdl_axis == SDL_GAMEPAD_AXIS_LEFTY) {
                pState->Gamepad.StickLeftY = value;
            }
            else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHTX) {
                pState->Gamepad.StickRightX = value;
            }
            else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHTY) {
                pState->Gamepad.StickRightY = value;
            }
            else if (sdl_axis == SDL_GAMEPAD_AXIS_LEFT_TRIGGER) {
                pState->Gamepad.TriggerLeft = value;
            }
            else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) {
                pState->Gamepad.TriggerRight = value;
            }
        }

        return ERROR_SUCCESS;
    }

    DWORD WINAPI XGameInputSetState(DWORD PlayerIndex, const XGAMEINPUT_VIBRATION* pVibration) {
        if (PlayerIndex >= XGAMEINPUT_PLAYER_MAX || pVibration == nullptr) {
            return ERROR_INVALID_PARAMETER;
        }

        if (!(SDL_WasInit(SDL_INIT_GAMEPAD) & SDL_INIT_GAMEPAD)) {
            if (InputFusion_SDL_Init_Wait) {
                WaitForSingleObject(InputFusion_SDL_Init_Wait, INFINITE);
                SDL_Delay(INPUTFUSION_SDL_EVENT_LOOP_DELAY);
            }
        }

        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(PlayerIndex);
        if (gamepad == nullptr) {
            return ERROR_DEVICE_NOT_CONNECTED;
        }

        SDL_RumbleGamepad(
            gamepad,
            pVibration->LeftMotorSpeed,
            pVibration->RightMotorSpeed,
            0
        );
        SDL_RumbleGamepadTriggers(
            gamepad,
            pVibration->LeftTriggerSpeed,
            pVibration->RightTriggerSpeed,
            0
        );
        SDL_UpdateGamepads();

        return ERROR_SUCCESS;
    }

    DWORD WINAPI XGameInputSetLED(DWORD PlayerIndex, const XGAMEINPUT_GAMEPAD_LED* pRGB) {
        if (PlayerIndex >= XGAMEINPUT_PLAYER_MAX || pRGB == nullptr) {
            return ERROR_INVALID_PARAMETER;
        }

        if (!(SDL_WasInit(SDL_INIT_GAMEPAD) & SDL_INIT_GAMEPAD)) {
            if (InputFusion_SDL_Init_Wait) {
                WaitForSingleObject(InputFusion_SDL_Init_Wait, INFINITE);
                SDL_Delay(INPUTFUSION_SDL_EVENT_LOOP_DELAY);
            }
        }

        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(PlayerIndex);
        if (gamepad == nullptr) {
            return ERROR_DEVICE_NOT_CONNECTED;
        }

        SDL_SetGamepadLED(gamepad, pRGB->Red, pRGB->Green, pRGB->Blue);

        return ERROR_SUCCESS;
    }

    DWORD WINAPI XGameInputGetBatteryInfo(DWORD PlayerIndex, XGAMEINPUT_BATTERY_INFORMATION* pBatteryInfo) {
        if (PlayerIndex >= XGAMEINPUT_PLAYER_MAX || pBatteryInfo == nullptr) {
            return ERROR_INVALID_PARAMETER;
        }

        if (!(SDL_WasInit(SDL_INIT_GAMEPAD) & SDL_INIT_GAMEPAD)) {
            if (InputFusion_SDL_Init_Wait) {
                WaitForSingleObject(InputFusion_SDL_Init_Wait, INFINITE);
                SDL_Delay(INPUTFUSION_SDL_EVENT_LOOP_DELAY);
            }
        }

        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(PlayerIndex);
        if (gamepad == nullptr) {
            return ERROR_DEVICE_NOT_CONNECTED;
        }

        ZeroMemory(pBatteryInfo, sizeof(XGAMEINPUT_BATTERY_INFORMATION));
        SDL_UpdateGamepads();

        int percent = -1;
        SDL_PowerState PowerState = SDL_GetGamepadPowerInfo(gamepad, &percent);
        switch (PowerState) {
            case SDL_POWERSTATE_CHARGED:
                pBatteryInfo->State = XGAMEINPUT_BATTERY_STATE_CHARGED;
                break;
            case SDL_POWERSTATE_CHARGING:
                pBatteryInfo->State = XGAMEINPUT_BATTERY_STATE_CHARGING;
                break;
            case SDL_POWERSTATE_ON_BATTERY:
                pBatteryInfo->State = XGAMEINPUT_BATTERY_STATE_DISCHARGING;
                break;
            default:
                pBatteryInfo->State = XGAMEINPUT_BATTERY_STATE_UNKNOWN;
        }
        pBatteryInfo->Percent = percent;

        return ERROR_SUCCESS;
    }

    DWORD WINAPI XGameInputGetCapabilities(DWORD PlayerIndex, XGAMEINPUT_CAPABILITIES* pCapabilities) {

        if (PlayerIndex >= XGAMEINPUT_PLAYER_MAX || pCapabilities == nullptr) {
            return ERROR_INVALID_PARAMETER;
        }

        if (!(SDL_WasInit(SDL_INIT_GAMEPAD) & SDL_INIT_GAMEPAD)) {
            if (InputFusion_SDL_Init_Wait) {
                WaitForSingleObject(InputFusion_SDL_Init_Wait, INFINITE);
                SDL_Delay(INPUTFUSION_SDL_EVENT_LOOP_DELAY);
            }
        }

        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(PlayerIndex);
        if (gamepad == nullptr) {
            return ERROR_DEVICE_NOT_CONNECTED;
        }

        ZeroMemory(pCapabilities, sizeof(XGAMEINPUT_CAPABILITIES));
        SDL_UpdateGamepads();

        pCapabilities->VendorId = SDL_GetGamepadVendor(gamepad);
        pCapabilities->ProductId = SDL_GetGamepadProduct(gamepad);
        pCapabilities->ProductVersion = SDL_GetGamepadProductVersion(gamepad);

        SDL_GamepadType type = SDL_GetGamepadType(gamepad);
        switch (type) {
            case SDL_GAMEPAD_TYPE_XBOX360:
                pCapabilities->Type = XGAMEINPUT_GAMEPAD_TYPE_XBOX360;
                break;
            case SDL_GAMEPAD_TYPE_XBOXONE:
                pCapabilities->Type = XGAMEINPUT_GAMEPAD_TYPE_XBOXONE;
                break;
            case SDL_GAMEPAD_TYPE_PS3:
                pCapabilities->Type = XGAMEINPUT_GAMEPAD_TYPE_PS3;
                break;
            case SDL_GAMEPAD_TYPE_PS4:
                pCapabilities->Type = XGAMEINPUT_GAMEPAD_TYPE_PS4;
                break;
            case SDL_GAMEPAD_TYPE_PS5:
                pCapabilities->Type = XGAMEINPUT_GAMEPAD_TYPE_PS5;
                break;
            case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO:
                pCapabilities->Type = XGAMEINPUT_GAMEPAD_TYPE_SWITCH_PRO;
                break;
            // SDL 3.6
            /*case SDL_GAMEPAD_TYPE_STEAM:
                    pCapabilities->Type = XGAMEINPUT_GAMEPAD_TYPE_STEAM;*/
                break;
            default:
                pCapabilities->Type = XGAMEINPUT_GAMEPAD_TYPE_UNKNOWN;
        }

        SDL_JoystickConnectionState ConnectionState = SDL_GetGamepadConnectionState(gamepad);
        if (ConnectionState == SDL_JOYSTICK_CONNECTION_WIRELESS) {
            pCapabilities->Flags |= XGAMEINPUT_IS_WIRELESS;
        }

        SDL_PropertiesID Properties = SDL_GetGamepadProperties(gamepad);
        if (SDL_GetBooleanProperty(Properties, SDL_PROP_GAMEPAD_CAP_RUMBLE_BOOLEAN, false)) {
            pCapabilities->Flags |= XGAMEINPUT_HAS_RUMBLE;
        }
        if (SDL_GetBooleanProperty(Properties, SDL_PROP_GAMEPAD_CAP_TRIGGER_RUMBLE_BOOLEAN, false)) {
            pCapabilities->Flags |= XGAMEINPUT_HAS_TRIGGER_RUMBLE;
        }
        if (SDL_GetBooleanProperty(Properties, SDL_PROP_GAMEPAD_CAP_RGB_LED_BOOLEAN, false) ||
            SDL_GetBooleanProperty(Properties, SDL_PROP_GAMEPAD_CAP_MONO_LED_BOOLEAN, false))
        {
            pCapabilities->Flags |= XGAMEINPUT_HAS_LED;
        }
        if (SDL_GetBooleanProperty(Properties, SDL_PROP_GAMEPAD_CAP_PLAYER_LED_BOOLEAN, false))
        {
            pCapabilities->Flags |= XGAMEINPUT_HAS_PLAYER_LED;
        }

        int nbrTouchPads = SDL_GetNumGamepadTouchpads(gamepad);
        if (nbrTouchPads > 0) {
            pCapabilities->Flags |= XGAMEINPUT_HAS_TOUCHPAD;
            pCapabilities->nbrTouchPads = nbrTouchPads;
        }

        if (SDL_GamepadHasSensor(gamepad, SDL_SENSOR_ACCEL) ||
            SDL_GamepadHasSensor(gamepad, SDL_SENSOR_ACCEL_L) ||
            SDL_GamepadHasSensor(gamepad, SDL_SENSOR_ACCEL_R)) 
        {
            pCapabilities->Flags |= XGAMEINPUT_HAS_ACCEL;
        }
        if (SDL_GamepadHasSensor(gamepad, SDL_SENSOR_GYRO) ||
            SDL_GamepadHasSensor(gamepad, SDL_SENSOR_GYRO_L) ||
            SDL_GamepadHasSensor(gamepad, SDL_SENSOR_GYRO_R)) 
        {
            pCapabilities->Flags |= XGAMEINPUT_HAS_GYRO;
        }

        // SDL 3.6
        /*if (SDL_GamepadHasCapSense(gamepad, SDL_GAMEPAD_CAPSENSE_LEFT_STICK) ||
            SDL_GamepadHasCapSense(gamepad, SDL_GAMEPAD_CAPSENSE_RIGHT_STICK)) 
        {
            pCapabilities->Flags |= XGAMEINPUT_HAS_CAPSENSE_STICK;
        }*/
        /*if (SDL_GamepadHasCapSense(gamepad, SDL_GAMEPAD_CAPSENSE_LEFT_GRIP) ||
            SDL_GamepadHasCapSense(gamepad, SDL_GAMEPAD_CAPSENSE_RIGHT_GRIP))
        {
            pCapabilities->Flags |= XGAMEINPUT_HAS_CAPSENSE_GRIP;
        }*/

        for (const auto& [sdl_button, button] : BUTTONS) {
            if (SDL_GamepadHasButton(gamepad, sdl_button)) {
                ++pCapabilities->nbrButtons;
            }
        }

        return ERROR_SUCCESS;
    }


    /*
    XGameInputGetTouchpadState() {

    }

    XGameInputSetTriggerState() {
        //dual sense sdl raw packet send ?

        */
}