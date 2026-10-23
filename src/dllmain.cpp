/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.

This file is based upon the original source code of
[InputFusion](https://github.com/xan105/InputFusion)
distributed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3.

XGameInput source: https://github.com/xan105/XGameInput.git
Mirror: https://download.xan105.com/XGameInput/Source%20code.zip

InputFusion source: https://github.com/xan105/InputFusion.git
Mirror: https://download.xan105.com/InputFusion/Source%20code.zip
*/

#include "dllmain.h"
#include <SDL3/SDL.h>
#include "InputFusion.h"
#include "detours.h"

extern HANDLE InputFusion_SDL_Init_Wait;
extern HANDLE InputFusion_SDL_Quit_Wait;

DWORD WINAPI InputFusion_Main(LPVOID lpReserved) {
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_ERROR);
    InputFusion_SDL_SetHints();
    InputFusion_Detours_SetHooks();
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