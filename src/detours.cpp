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

#include "detours.h"
#include "InputFusion.h"
#include "../vendor/Detour/detours.h"
#if defined(_X86_)
    #pragma comment(lib, "../vendor/Detour/detours.x86.lib")
#elif defined(_AMD64_)
    #pragma comment(lib, "../vendor/Detour/detours.x64.lib")
#endif

namespace {

    ExitProcess_t Win32_ExitProcess = nullptr;

    void WINAPI InputFusion_ExitProcess(UINT uExitCode) {
        InputFusion_SDL_Event_Loop = false;
        if (InputFusion_SDL_Quit_Wait) {
            WaitForSingleObject(InputFusion_SDL_Quit_Wait, INFINITE);
            CloseHandle(InputFusion_SDL_Quit_Wait);
            InputFusion_SDL_Quit_Wait = nullptr;
        }
        return Win32_ExitProcess(uExitCode);
    }
    
    bool InputFusion_Detours_Attach(PVOID* ppPointer, PVOID pDetour) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(ppPointer, pDetour);
        return DetourTransactionCommit() == NO_ERROR;
    }

    bool InputFusion_Detours_Hook_ExitProcess() {
        HMODULE hMod = LoadLibraryW(L"KERNEL32.dll");
        if (hMod == nullptr) {
            return false;
        }
        Win32_ExitProcess = (ExitProcess_t)GetProcAddress(hMod, "ExitProcess");
        if (Win32_ExitProcess == nullptr) {
            return false;
        }
        return InputFusion_Detours_Attach(&(PVOID&)Win32_ExitProcess, InputFusion_ExitProcess);
    }

}

void InputFusion_Detours_SetHooks() {
  InputFusion_Detours_Hook_ExitProcess();
}