/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 25/01/2023 15:30
 */

#pragma once

#include "math_types.hpp"

struct transform
{
    V3 Position;
    V3 Scale;
    V3 Rotation;
    M4 Matrix;
};

void TransformUpdate(transform *Transform);
