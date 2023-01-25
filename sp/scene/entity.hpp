/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 25/01/2023 15:28
 */

#pragma once

#include <HandmadeMath/HandmadeMath.h>
#include "transform.hpp"
#include "renderer/mesh.hpp"

struct game_entity
{
    transform Transform;

    loaded_model Model;
    bool HasModel;
};
