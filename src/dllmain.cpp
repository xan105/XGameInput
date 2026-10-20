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

#include "dllmain.h"
#include "version.h"

HANDLE InputFusion_SDL_Init_Wait = nullptr;
HANDLE InputFusion_SDL_Quit_Wait = nullptr;
std::atomic<bool> InputFusion_SDL_Event_Loop(true);

namespace {

    constexpr auto INPUTFUSION_SDL_EVENT_LOOP_DELAY = 10;

    void InputFusion_SDL_SetHints() {
        SDL_SetHint(SDL_HINT_JOYSTICK_DIRECTINPUT, "1");                    // Dinput8
        SDL_SetHint(SDL_HINT_XINPUT_ENABLED, "1");                          // Xinput
        SDL_SetHint(SDL_HINT_JOYSTICK_WGI, "1");                            // Windows.Gaming.Input (WinRT)
        SDL_SetHint(SDL_HINT_JOYSTICK_GAMEINPUT, "1");                      // GameInput
        SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI, "1");                         // HID
            SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS3, "1");
            SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS4, "1");
            SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS5, "1");
            SDL_SetHint(SDL_HINT_JOYSTICK_ENHANCED_REPORTS, "1");
        SDL_SetHint(SDL_HINT_JOYSTICK_RAWINPUT, "0");                       // RAW(Xbox controllers) | Disabled for now cf : https://github.com/libsdl-org/SDL/issues/13047#issuecomment-2913284199
            SDL_SetHint(SDL_HINT_JOYSTICK_RAWINPUT_CORRELATE_XINPUT, "1");  // Raw input pulls data from WGI/XInput providing better support for Xbox controllers                                                               
            SDL_SetHint(SDL_HINT_JOYSTICK_THREAD, "1");                     // Xbox controllers raw input (SDL is not running in the main thread)

        // https://github.com/libsdl-org/SDL/pull/14353
        // Should be merged in SDL3 3.6
        // SDL_SetHint(SDL_HINT_JOYSTICK_DSU, "1");                         // DualShock UDP (DS4Windows, BetterJoy, etc.)
    }

    bool InputFusion_SDL_Init() {
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, VER_PRODUCTNAME_STR);
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, VER_FILEVERSION_STR);
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, VER_COMPANYNAME_STR);
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, VER_LEGALCOPYRIGHT_STR);
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, VER_COMPANYDOMAIN_STR);
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING, "com.xan105.XGameInput");

        if (!SDL_Init(SDL_INIT_GAMEPAD)) {
            if (InputFusion_SDL_Init_Wait) {
                SetEvent(InputFusion_SDL_Init_Wait);
            }
            return false;
        }

        if (InputFusion_SDL_Init_Wait) {
            SetEvent(InputFusion_SDL_Init_Wait);
        }
        return true;
    }

    void InputFusion_SDL_Quit() {
        SDL_Quit();
        if (InputFusion_SDL_Quit_Wait) {
            SetEvent(InputFusion_SDL_Quit_Wait);
        }
        if (InputFusion_SDL_Init_Wait) {
            CloseHandle(InputFusion_SDL_Init_Wait);
            InputFusion_SDL_Init_Wait = nullptr;
        }
    }

    void InputFusion_SDL_CloseGamepads() {
        int count;

        SDL_JoystickID* gamepads = SDL_GetGamepads(&count);
        if (gamepads == nullptr) return;

        for (int i = 0; i < count; i++) {
            SDL_JoystickID id = gamepads[i];
            SDL_Gamepad* gamepad = SDL_GetGamepadFromID(id);
            if (gamepad == nullptr) continue;
            SDL_SetGamepadPlayerIndex(gamepad, -1); //turn off player light
            if (SDL_GetGamepadType(gamepad) == SDL_GAMEPAD_TYPE_PS4) {
                SDL_SetGamepadLED(gamepad, 10, 20, 20); //Not the exact default color (no player) but close enough
            }
            SDL_CloseGamepad(gamepad);
        }
        SDL_free(gamepads);
    }

    void InputFusion_SDL_EventLoop() {
        SDL_Event event;
        while (InputFusion_SDL_Event_Loop) {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_EVENT_GAMEPAD_ADDED: {
                        SDL_JoystickID id = event.gdevice.which;
                        SDL_Gamepad* gamepad = SDL_OpenGamepad(id);
                        if (gamepad != nullptr) {
                            // Workaround for https://github.com/libsdl-org/SDL/issues/16261
                            SDL_GamepadType type = SDL_GetGamepadType(gamepad);
                            if (type == SDL_GAMEPAD_TYPE_PS4) {
                                int index = SDL_GetGamepadPlayerIndex(gamepad);
                                if (index == 0) {
                                    SDL_SetGamepadLED(gamepad, 0, 0, 64); // Blue (First Player)
                                }
                            }
                        }
                        break;
                    }
                    case SDL_EVENT_GAMEPAD_REMOVED: {
                        SDL_JoystickID id = event.gdevice.which;
                        SDL_Gamepad* gamepad = SDL_GetGamepadFromID(id);
                        if (gamepad != nullptr) {
                            SDL_CloseGamepad(gamepad);
                        }
                        break;
                    }
                    case SDL_EVENT_QUIT: {
                        InputFusion_SDL_Event_Loop = false;
                        break;
                    }
                }
            }
            SDL_Delay(INPUTFUSION_SDL_EVENT_LOOP_DELAY); // Prevent this loop from consuming too much CPU
        }
    }
}

DWORD WINAPI InputFusion_Main(LPVOID lpReserved) {
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_ERROR);
    InputFusion_SDL_SetHints();
    if (InputFusion_SDL_Init()) {
        InputFusion_SDL_EventLoop();
        InputFusion_SDL_CloseGamepads();
    }
    InputFusion_SDL_Quit();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH: {
            DisableThreadLibraryCalls(hModule);
            InputFusion_SDL_Init_Wait = CreateEvent(nullptr, TRUE, FALSE, nullptr);
            InputFusion_SDL_Quit_Wait = CreateEvent(nullptr, TRUE, FALSE, nullptr);
            HANDLE hThread = CreateThread(nullptr, 0, InputFusion_Main, hModule, 0, nullptr);
            if (hThread) {
                CloseHandle(hThread);
            }
            break;
        }
    }
    return TRUE;
}