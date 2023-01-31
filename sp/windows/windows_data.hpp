/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 11/01/2023 09:47
 */

#pragma once

#include <Windows.h>

#define SafeRelease(Object) if (Object) Object->Release()

struct win32_platform_state
{
    HINSTANCE Instance;
    WNDCLASSA WindowClass;
    HWND Window;
};

extern win32_platform_state Win32;
