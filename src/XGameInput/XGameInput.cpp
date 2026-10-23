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
#include "../InputFusion.h"
#include "../Gamepad/dualsense.h"

namespace {

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

    DWORD WINAPI XGameInputGetState(DWORD PlayerIndex, XGAMEINPUT_GAMEPAD* pGamepad) {

        if (PlayerIndex >= XGAMEINPUT_PLAYER_MAX || pGamepad == nullptr) {
            return ERROR_INVALID_PARAMETER;
        }

        InputFusion_SDL_Wait_ready();

        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(PlayerIndex);
        if (gamepad == nullptr) {
            return ERROR_DEVICE_NOT_CONNECTED;
        }

        ZeroMemory(pGamepad, sizeof(XGAMEINPUT_GAMEPAD));

        SDL_UpdateGamepads();

        for (const auto& [sdl_button, button] : BUTTONS) {
            if (SDL_GetGamepadButton(gamepad, sdl_button)) {
                pGamepad->Buttons |= button;
            }
        }

        for (const auto& sdl_axis : AXIS) {
            SHORT value = SDL_GetGamepadAxis(gamepad, sdl_axis);
            if (sdl_axis == SDL_GAMEPAD_AXIS_LEFTX) {
                pGamepad->StickLeftX = value;
            }
            else if (sdl_axis == SDL_GAMEPAD_AXIS_LEFTY) {
                pGamepad->StickLeftY = value;
            }
            else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHTX) {
                pGamepad->StickRightX = value;
            }
            else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHTY) {
                pGamepad->StickRightY = value;
            }
            else if (sdl_axis == SDL_GAMEPAD_AXIS_LEFT_TRIGGER) {
                pGamepad->TriggerLeft = value;
            }
            else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) {
                pGamepad->TriggerRight = value;
            }
        }

        return ERROR_SUCCESS;
    }

    DWORD WINAPI XGameInputSetState(DWORD PlayerIndex, const XGAMEINPUT_VIBRATION* pVibration) {
        if (PlayerIndex >= XGAMEINPUT_PLAYER_MAX || pVibration == nullptr) {
            return ERROR_INVALID_PARAMETER;
        }

        InputFusion_SDL_Wait_ready();

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

        InputFusion_SDL_Wait_ready();

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

        InputFusion_SDL_Wait_ready();

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

        InputFusion_SDL_Wait_ready();

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

    DWORD WINAPI XGameInputGetSteamHandle(DWORD PlayerIndex, UINT64* pInputHandle) {
    
        if (PlayerIndex >= XGAMEINPUT_PLAYER_MAX || pInputHandle == nullptr) {
            return ERROR_INVALID_PARAMETER;
        }

        InputFusion_SDL_Wait_ready();

        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(PlayerIndex);
        if (gamepad == nullptr) {
            return ERROR_DEVICE_NOT_CONNECTED;
        }

        *pInputHandle = SDL_GetGamepadSteamHandle(gamepad);

        return ERROR_SUCCESS;
    }

    DWORD WINAPI XGameInputGetTouchpadState(DWORD PlayerIndex, WORD TouchPadIndex, XGAMEINPUT_TOUCHPAD_STATE* pTouchPad) {

        if (PlayerIndex >= XGAMEINPUT_PLAYER_MAX || pTouchPad == nullptr) {
            return ERROR_INVALID_PARAMETER;
        }

        InputFusion_SDL_Wait_ready();

        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(PlayerIndex);
        if (gamepad == nullptr) {
            return ERROR_DEVICE_NOT_CONNECTED;
        }

        int fingers = SDL_GetNumGamepadTouchpadFingers(gamepad, static_cast<int>(TouchPadIndex));

        if (fingers < 0) {
            fingers = 0;
        }

        if (fingers > XGAMEINPUT_TOUCHPAD_MAX_FINGERS) {
            fingers = XGAMEINPUT_TOUCHPAD_MAX_FINGERS;
        }

        ZeroMemory(pTouchPad, sizeof(XGAMEINPUT_TOUCHPAD_STATE));
        pTouchPad->FingerCount = 0;

        for (int finger=0; finger < fingers; finger++ ) 
        {
            XGAMEINPUT_TOUCHPAD_FINGER* state = &pTouchPad->Fingers[finger];

            if (SDL_GetGamepadTouchpadFinger(
                gamepad,
                static_cast<int>(TouchPadIndex),
                finger,
                &state->Down,
                &state->X,
                &state->Y,
                &state->Pressure
            )) {
                ++pTouchPad->FingerCount;
            } else {
                state->Down = false;
                state->X = 0.0f;
                state->Y = 0.0f;
                state->Pressure = 0.0f;
            }
        }

        return ERROR_SUCCESS;
    }

    // NB: This code is at the moment still untested on real hardware as I unforunately do not yet own a DS5 controller.
    DWORD WINAPI XGameInputSetTriggerEffect(DWORD PlayerIndex, XGAMEINPUT_TRIGGER_EFFECT trigger, XGAMEINPUT_TRIGGER_EFFECT_MODE mode, const XGAMEINPUT_TRIGGER_EFFECT_PARAM* pEffectParam) {
      
      if (PlayerIndex >= XGAMEINPUT_PLAYER_MAX) {
        return ERROR_INVALID_PARAMETER;
      }
      
      if (mode != XGAMEINPUT_TRIGGER_EFFECT_MODE_OFF && pEffectParam == nullptr) {
          return ERROR_INVALID_PARAMETER;
      }

      uint8_t trigger_mask = DS5_TRIGGER_EFFECT_BOTH;
      switch (trigger) {
        case XGAMEINPUT_TRIGGER_EFFECT_BOTH: {
          trigger_mask = DS5_TRIGGER_EFFECT_BOTH;
          break;
        }
        case XGAMEINPUT_TRIGGER_EFFECT_LEFT: {
          trigger_mask = DS5_TRIGGER_EFFECT_LEFT;
          break;
        }
        case XGAMEINPUT_TRIGGER_EFFECT_RIGHT: {
          trigger_mask = DS5_TRIGGER_EFFECT_RIGHT;
          break;
        }
        default: {
          return ERROR_INVALID_PARAMETER;
        }
      }

      uint8_t effect[DS5_TRIGGER_EFFECT_SIZE] = { 0 };
      switch (mode) {
        case XGAMEINPUT_TRIGGER_EFFECT_MODE_OFF: {
            memcpy(effect, dualsense::DS5_TRIGGER_EFFECT_OFF, DS5_TRIGGER_EFFECT_SIZE);
            break;
        }
        case XGAMEINPUT_TRIGGER_EFFECT_MODE_FEEDBACK: {
            if (!dualsense::CreateFeedbackEffect(effect, pEffectParam->position, pEffectParam->strength)) {
              return ERROR_INVALID_PARAMETER;
            }
            break;
        }
        case XGAMEINPUT_TRIGGER_EFFECT_MODE_WEAPON: {
            if (!dualsense::CreateWeaponEffect(effect, pEffectParam->start, pEffectParam->end, pEffectParam->strength)) {
                return ERROR_INVALID_PARAMETER;
            }
          break;
        }
        case XGAMEINPUT_TRIGGER_EFFECT_MODE_VIBRATION: {
            if (!dualsense::CreateVibrationEffect(effect, pEffectParam->position, pEffectParam->amplitude, pEffectParam->frequency)){
                return ERROR_INVALID_PARAMETER;
            }
            break;
        }
        default: {
          return ERROR_INVALID_PARAMETER;
        }
      }
      
      InputFusion_SDL_Wait_ready();
      
      SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(PlayerIndex);
      if (gamepad == nullptr) {
        return ERROR_DEVICE_NOT_CONNECTED;
      }
      
      dualsense::SetTriggerEffect(gamepad, trigger_mask, effect);
      
      return ERROR_SUCCESS;
    }
}