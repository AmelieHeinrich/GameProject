/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 01/01/2023 18:18
 */

#include <Windows.h>

#include "game_config.hpp"
#include "timer.h"
#include "log_system.hpp"

struct win32_platform_state
{
    HINSTANCE Instance;
    WNDCLASSA WindowClass;
    HWND Window;
};

LRESULT CALLBACK WindowProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    return DefWindowProc(Window, Message, WParam, LParam);
}

int main(int argc, char *argv[])
{
    game_config_file ConfigFile;
    GameConfigLoad("gameconfig.cfg", &ConfigFile);
    
    win32_platform_state State = {};
    State.Instance = GetModuleHandle(NULL);

    State.WindowClass.hInstance = State.Instance;
    State.WindowClass.hCursor = LoadCursor(State.Instance, IDC_ARROW);
    State.WindowClass.hIcon = LoadIcon(State.Instance, IDI_WINLOGO);
    State.WindowClass.lpszClassName = "GameProjectWindowClass";
    State.WindowClass.lpfnWndProc = WindowProc;
    State.WindowClass.hbrBackground = (HBRUSH)GRAY_BRUSH;
    RegisterClassA(&State.WindowClass);

    State.Window = CreateWindowA(State.WindowClass.lpszClassName, "Game Project | <Direct3D 11>", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, ConfigFile.Width, ConfigFile.Height, NULL, NULL, State.Instance, NULL);
    ShowWindow(State.Window, SW_SHOW);
    UpdateWindow(State.Window);

    while (IsWindowVisible(State.Window))
    {
        MSG Message;
        while (PeekMessage(&Message, State.Window, 0, 0, PM_REMOVE) > 0)
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
    }

    DestroyWindow(State.Window);
    UnregisterClassA(State.WindowClass.lpszClassName, State.Instance);
    return (0);
}
