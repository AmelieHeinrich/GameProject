/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 22/01/2023 10:44
 */

#pragma once

#include <cstdint>

#include "renderer_settings.hpp"
#include "forward_pass.hpp"
#include "color_correction_pass.hpp"
#include "tonemapping_pass.hpp"

void RendererInit();
void RendererExit();
void RendererStartSync();
void RendererConstructFrame(camera_data *Camera);
void RendererStartRender();
void RendererEndRender();
void RendererEndSync();
void RendererResize(uint32_t Width, uint32_t Height);
void RendererScreenshot();
renderer_settings *RendererGetSettings();
