/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 22/01/2023 10:44
 */

#pragma once

#include "pass_forward.hpp"

struct renderer_data
{
    forward_pass Forward;
};

void RendererInit();
void RendererExit();
void RendererUpdate();
void RendererResize(uint32_t Width, uint32_t Height);
