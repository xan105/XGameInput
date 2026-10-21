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

namespace {
	void InputFusion_SDL_SetHints();
	bool InputFusion_SDL_Init();
	void InputFusion_SDL_Quit();
	void InputFusion_SDL_CloseGamepads();
	void InputFusion_SDL_EventLoop();
}

DWORD WINAPI InputFusion_Main(LPVOID lpReserved);
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);