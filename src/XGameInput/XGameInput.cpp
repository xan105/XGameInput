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

extern HANDLE InputFusion_SDL_Init_Wait;

namespace {

    constexpr auto INPUTFUSION_SDL_EVENT_LOOP_DELAY = 10;

    const std::unordered_map<SDL_GamepadButton, DWORD> BUTTONS = {
        { SDL_GAMEPAD_BUTTON_SOUTH, XGAMEINPUT_GAMEPAD_BUTTON_SOUTH },
        { SDL_GAMEPAD_BUTTON_EAST, XGAMEINPUT_GAMEPAD_BUTTON_EAST },
        { SDL_GAMEPAD_BUTTON_WEST, XGAMEINPUT_GAMEPAD_BUTTON_WEST },
        { SDL_GAMEPAD_BUTTON_NORTH, XGAMEINPUT_GAMEPAD_BUTTON_NORTH },
        { SDL_GAMEPAD_BUTTON_BACK, XGAMEINPUT_GAMEPAD_BUTTON_SELECT },
        { SDL_GAMEPAD_BUTTON_GUIDE, XGAMEINPUT_GAMEPAD_BUTTON_GUIDE },
        { SDL_GAMEPAD_BUTTON_START, XGAMEINPUT_GAMEPAD_BUTTON_START },
        { SDL_GAMEPAD_BUTTON_LEFT_STICK, XGAMEINPUT_GAMEPAD_BUTTON_LEFT_STICK },
        { SDL_GAMEPAD_BUTTON_RIGHT_STICK, XGAMEINPUT_GAMEPAD_BUTTON_RIGHT_STICK },
        { SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, XGAMEINPUT_GAMEPAD_BUTTON_LEFT_SHOULDER },
        { SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, XGAMEINPUT_GAMEPAD_BUTTON_RIGHT_SHOULDER },
        { SDL_GAMEPAD_BUTTON_DPAD_UP, XGAMEINPUT_GAMEPAD_BUTTON_DPAD_UP },
        { SDL_GAMEPAD_BUTTON_DPAD_DOWN, XGAMEINPUT_GAMEPAD_BUTTON_DPAD_DOWN },
        { SDL_GAMEPAD_BUTTON_DPAD_LEFT, XGAMEINPUT_GAMEPAD_BUTTON_DPAD_LEFT },
        { SDL_GAMEPAD_BUTTON_DPAD_RIGHT, XGAMEINPUT_GAMEPAD_BUTTON_DPAD_RIGHT },
        { SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1, XGAMEINPUT_GAMEPAD_BUTTON_RIGHT_PADDLE1 },
        { SDL_GAMEPAD_BUTTON_LEFT_PADDLE1, XGAMEINPUT_GAMEPAD_BUTTON_LEFT_PADDLE1 },
        { SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2, XGAMEINPUT_GAMEPAD_BUTTON_RIGHT_PADDLE2 },
        { SDL_GAMEPAD_BUTTON_LEFT_PADDLE2, XGAMEINPUT_GAMEPAD_BUTTON_LEFT_PADDLE2 },
        { SDL_GAMEPAD_BUTTON_TOUCHPAD, XGAMEINPUT_GAMEPAD_BUTTON_TOUCHPAD },
        { SDL_GAMEPAD_BUTTON_MISC1, XGAMEINPUT_GAMEPAD_BUTTON_MISC1 },
        { SDL_GAMEPAD_BUTTON_MISC2, XGAMEINPUT_GAMEPAD_BUTTON_MISC2 },
        { SDL_GAMEPAD_BUTTON_MISC3, XGAMEINPUT_GAMEPAD_BUTTON_MISC3 },
        { SDL_GAMEPAD_BUTTON_MISC4, XGAMEINPUT_GAMEPAD_BUTTON_MISC4 },
        { SDL_GAMEPAD_BUTTON_MISC5, XGAMEINPUT_GAMEPAD_BUTTON_MISC5 },
        { SDL_GAMEPAD_BUTTON_MISC6, XGAMEINPUT_GAMEPAD_BUTTON_MISC6 }
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

        static std::atomic<DWORD> dwPacketNumber[XGAMEINPUT_PLAYER_MAX] = {};

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
        pState->PacketNumber = ++dwPacketNumber[PlayerIndex];

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




    




    /*

    DWORD WINAPI XGameInputGetCapabilities(DWORD dwUserIndex, XINPUT_CAPABILITIES_EX* pCapabilities) {
        
        if (dwUserIndex >= XUSER_MAX_COUNT || pCapabilities == nullptr) {
            return ERROR_INVALID_PARAMETER;
        }

        if (!(SDL_WasInit(SDL_INIT_GAMEPAD) & SDL_INIT_GAMEPAD)) {
            if (InputFusion_SDL_Init_Wait) {
                WaitForSingleObject(InputFusion_SDL_Init_Wait, INFINITE);
                SDL_Delay(10); // SDL event loop
            }
        }

        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(dwUserIndex);
        if (gamepad == nullptr) {
            return ERROR_DEVICE_NOT_CONNECTED;
        }

        ZeroMemory(pCapabilities, sizeof(XINPUT_CAPABILITIES_EX));
        pCapabilities->VendorId = SDL_GetGamepadVendor(gamepad);
        pCapabilities->ProductId = SDL_GetGamepadProduct(gamepad);
        pCapabilities->ProductVersion = SDL_GetGamepadProductVersion(gamepad);
        pCapabilities->Capabilities.Type = XINPUT_DEVTYPE_GAMEPAD;
        pCapabilities->Capabilities.SubType = XINPUT_DEVSUBTYPE_GAMEPAD;


        Capabilities
            XGAMEINPUT_GAMEPAD_TYPE Type
            WORD VendorId
            WORD ProductId
            WORD ProductVersion
            WORD Flags  XGAMEINPUT_GAMEPAD_WIRELESS XGAMEINPUT_GAMEPAD_RUMBLE XGAMEINPUT_GAMEPAD_TOUCHPAD XGAMEINPUT_GAMEPAD_GYRO XGAMEINPUT_GAMEPAD_ACCEL XGAMEINPUT_GAMEPAD_CAPSENSE
            DWORD dwButtons   SDL_GamepadHasButton() / loop

            float BatteryLevel  0.0f 0% -> 1.0f 100% (if wired == 1.0f)
            SDL_PowerState SDL_GetGamepadPowerInfo(SDL_Gamepad * gamepad, int* percent);


        SDL_UpdateGamepads();

        SDL_GamepadType type = SDL_GetGamepadType(gamepad);
        if (type == SDL_GAMEPAD_TYPE_PS4 || type == SDL_GAMEPAD_TYPE_PS5 || type == SDL_GAMEPAD_TYPE_XBOXONE) {
            pCapabilities->Capabilities.Flags |= XINPUT_CAPS_VOICE_SUPPORTED;
        }

        if (type == SDL_GAMEPAD_TYPE_XBOX360 || type == SDL_GAMEPAD_TYPE_XBOXONE) {
            pCapabilities->Capabilities.Flags |= XINPUT_CAPS_PMD_SUPPORTED;
        }

        SDL_JoystickConnectionState connectionState = SDL_GetGamepadConnectionState(gamepad);
        if (connectionState == SDL_JOYSTICK_CONNECTION_WIRELESS) {
            pCapabilities->Capabilities.Flags |= XINPUT_CAPS_WIRELESS;
        }

        SDL_PropertiesID properties = SDL_GetGamepadProperties(gamepad);
        if (SDL_GetBooleanProperty(properties, SDL_PROP_GAMEPAD_CAP_RUMBLE_BOOLEAN, false)) {
            pCapabilities->Capabilities.Flags |= XINPUT_CAPS_FFB_SUPPORTED;
        }

        for (const auto& [sdl_button, xinput_button] : BUTTONS) {
            if (SDL_GamepadHasButton(gamepad, sdl_button)) {
                pCapabilities->Capabilities.Gamepad.wButtons |= xinput_button;
            }
            else if (sdl_button == SDL_GAMEPAD_BUTTON_START ||
                sdl_button == SDL_GAMEPAD_BUTTON_BACK ||
                sdl_button == SDL_GAMEPAD_BUTTON_DPAD_UP ||
                sdl_button == SDL_GAMEPAD_BUTTON_DPAD_DOWN ||
                sdl_button == SDL_GAMEPAD_BUTTON_DPAD_LEFT ||
                sdl_button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT) {
                pCapabilities->Capabilities.Flags |= XINPUT_CAPS_NO_NAVIGATION;
            }
        }

        return ERROR_SUCCESS;
    }

    */

    /*
    XGameInputGetTouchpadState() {

    }

    XGameInputSetTriggerState() {
        //dual sense sdl raw packet send ?
    }
    */


}