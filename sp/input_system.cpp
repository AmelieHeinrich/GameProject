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

V2 GetMousePosition()
{
    POINT Point;
    GetCursorPos(&Point);
    return HMM_Vec2(Point.x, Point.y);
}
