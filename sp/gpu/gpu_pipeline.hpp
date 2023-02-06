/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/02/2023 13:01
 */

#pragma once

#include <cstdint>

#include "gpu_shader.hpp"

struct gpu_pipeline
{
    gpu_shader *Shader;
    void *Private;
};
