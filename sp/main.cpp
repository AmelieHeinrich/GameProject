/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 01/01/2023 18:18
 */

#include <Windows.h>

#include "apu/dsound_context.hpp"
#include "egc_parser.hpp"
#include "game.hpp"
#include "game_data.hpp"
#include "gpu/dx11_context.hpp"
#include "gui/gui.hpp"
#include "systems/shader_system.hpp"
#include "systems/log_system.hpp"
#include "systems/event_system.hpp"
#include "systems/input_system.hpp"
#include "windows_data.hpp"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_win32.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    if (ImGui_ImplWin32_WndProcHandler(Window, Message, WParam, LParam))
        return 1;

    switch (Message)
    {
        case WM_SIZE:
        {
            RECT Rectangle;
            GetClientRect(Window, &Rectangle);
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
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        {
            bool Pressed = (Message == WM_LBUTTONDOWN || Message == WM_RBUTTONDOWN || Message == WM_MBUTTONDOWN);
            mouse_button Button = mouse_button::Left;

            switch (Message)
            {
                case WM_LBUTTONDOWN:
                {
                    Button = mouse_button::Left;
                } break;
                case WM_RBUTTONDOWN:
                {
                    Button = mouse_button::Right;
                } break;
                case WM_MBUTTONDOWN:
                {
                    Button = mouse_button::Middle;
                } break;
            }

            event_data Data;
            Data.data.u16[0] = static_cast<uint16_t>(Button);
            EventSystemFire(Pressed ? event_type::MouseButtonPressed : event_type::MouseButtonReleased, nullptr, Data);
        } break;
    }

    return DefWindowProc(Window, Message, WParam, LParam);
}

void WindowInit(void)
{
    uint32_t Width = EgcI32(EgcFile, "width");
    uint32_t Height = EgcI32(EgcFile, "height");

    Win32.Instance = GetModuleHandle(NULL);
    Win32.WindowClass.hInstance = Win32.Instance;
    Win32.WindowClass.hCursor = LoadCursor(Win32.Instance, IDC_ARROW);
    Win32.WindowClass.lpszClassName = "GameProjectWindowClass";
    Win32.WindowClass.lpfnWndProc = WindowProc;
    RegisterClassA(&Win32.WindowClass);

    Win32.Window = CreateWindowA(Win32.WindowClass.lpszClassName, "Game Project | <Direct3D 11>", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, NULL, NULL, Win32.Instance, NULL);
    ShowWindow(Win32.Window, SW_SHOWMAXIMIZED);
}

void WindowExit()
{
    DestroyWindow(Win32.Window);
    UnregisterClassA(Win32.WindowClass.lpszClassName, Win32.Instance);
}

int main(int argc, char *argv[])
{
    EgcParseFile("config.egc", &EgcFile);
    EgcParseFile("cvars.egc", &CVars);
    EventSystemInit();
    WindowInit();
    ApuInit();
    DxRenderContextInit(Win32.Window);
    GuiInit();
    GameInit();

    while (IsWindowVisible(Win32.Window))
    {
        MSG Message;
        while (PeekMessage(&Message, Win32.Window, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }

        DxRenderContextBegin();
        GameUpdate();
        DxRenderContextPresent();
    }

    ShaderLibraryFree();
    GameExit();
    GuiExit();
    DxRenderContextFree();
    ApuExit();
    WindowExit();
    EventSystemExit();
    EgcWriteFile("config.egc", &EgcFile);
    LogSaveFile("output_log.log");
    LogResetColor();
    return (0);
}
