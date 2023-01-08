/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 05/01/2023 19:51
 */

#pragma once

#include "input_types.hpp"
#include "math_types.hpp"

bool IsKeyPressed(keyboard_key Key);
bool IsKeyReleased(keyboard_key Key);

// TODO(amelie.h): Mouse press

V2 GetMousePosition();

// TODO(amelie.h): Mouse wheel
