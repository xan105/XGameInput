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

#pragma once
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>

typedef void (WINAPI* ExitProcess_t)(UINT uExitCode);

namespace {
    void WINAPI InputFusion_ExitProcess(UINT uExitCode);

    bool InputFusion_Detours_Attach(PVOID* ppPointer, PVOID pDetour);
    bool InputFusion_Detours_Hook_ExitProcess();
}

void InputFusion_Detours_SetHooks();