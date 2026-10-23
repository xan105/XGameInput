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
#include <SDL3/SDL.h>
#include <atomic>

extern HANDLE InputFusion_SDL_Init_Wait;
extern HANDLE InputFusion_SDL_Quit_Wait;
extern std::atomic<bool> InputFusion_SDL_Event_Loop;

void InputFusion_SDL_SetHints();
bool InputFusion_SDL_Init();
void InputFusion_SDL_Quit();
void InputFusion_SDL_CloseGamepads();
void InputFusion_SDL_EventLoop();
void InputFusion_SDL_Logger(void* userdata, int category, SDL_LogPriority priority, const char* message);
void InputFusion_SDL_Wait_ready(DWORD timeout = INFINITE);