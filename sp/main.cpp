/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 01/01/2023 18:18
 */

#include <Windows.h>

#include "game_config.hpp"
#include "timer.hpp"
#include "log_system.hpp"
#include "event_system.hpp"
#include "input_system.hpp"

struct win32_platform_state
{
    HINSTANCE Instance;
    WNDCLASSA WindowClass;
    HWND Window;
};

LRESULT CALLBACK WindowProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    switch (Message)
    {
        case WM_SIZE:
        {
            RECT Rectangle;
            GetClientRect(Window, &Rectangle);
            AdjustWindowRect(&Rectangle, WS_OVERLAPPEDWINDOW, false);
            uint32_t Width = Rectangle.right - Rectangle.left;
            uint32_t Height = Rectangle.bottom - Rectangle.top;

            event_data Data;
            Data.data.u32[0] = Width;
            Data.data.u32[1] = Height;
            EventSystemFire(event_type::Resize, nullptr, Data);
        } break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            bool Pressed = (Message == WM_KEYDOWN || Message == WM_SYSKEYDOWN);
            keyboard_key Key = static_cast<keyboard_key>(WParam);
            bool IsExtended = (HIWORD(LParam) & KF_EXTENDED) == KF_EXTENDED;

            if (WParam == VK_MENU) 
                Key = IsExtended ? keyboard_key::RightAlt : keyboard_key::LeftAlt;
            if (WParam == VK_SHIFT)
            {
                uint32_t LeftShift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
                uint32_t ScanCode = ((LParam & (0xFF << 16)) >> 16);
                Key = ScanCode == LeftShift ? keyboard_key::LeftShift : keyboard_key::RightShift;
            }
            if (WParam == VK_CONTROL)
            {
                Key = IsExtended ? keyboard_key::RightControl : keyboard_key::LeftControl;
            }

            event_data Data;
            Data.data.u16[0] = static_cast<uint16_t>(Key);
            EventSystemFire(Pressed ? event_type::KeyPressed : event_type::KeyReleased, nullptr, Data);
        } break;
    }

    return DefWindowProc(Window, Message, WParam, LParam);
}

bool TestEvent(event_type Type, void *Sender, void *ListenerInstance, event_data Data)
{
    if (Type == event_type::KeyPressed)
    {
        if (Data.data.u16[0] == (uint16_t)keyboard_key::Escape)
        {
            LogInfo("Escape Pressed!");
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    game_config_file ConfigFile;
    GameConfigLoad("gameconfig.cfg", &ConfigFile);

    EventSystemInit();

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

    EventSystemExit();
    
    return (0);
}