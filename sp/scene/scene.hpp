/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 25/01/2023 15:30
 */

#pragma once

#include "entity.hpp"

struct camera_data
{
    hmm_mat4 View;
    hmm_mat4 Projection;
    hmm_vec3 Position;
};
