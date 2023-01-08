/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 05/01/2023 19:52
 */

#include "input_system.hpp"

#include <Windows.h>

bool IsKeyPressed(keyboard_key Key)
{
    return GetAsyncKeyState(static_cast<uint16_t>(Key)) & 0x01;   
}

bool IsKeyReleased(keyboard_key Key)
{
    return !IsKeyPressed(Key);
}

bool IsMouseButtonPressed(mouse_button Button)
{
    return ((GetKeyState(static_cast<uint16_t>(Button)) & 0x8000) != 0);
}

bool IsMouseButtonReleased(mouse_button Button)
{
    return !IsMouseButtonPressed(Button);
}

V2 GetMousePosition()
{
    POINT Point;
    GetCursorPos(&Point);
    return HMM_Vec2(Point.x, Point.y);
}
