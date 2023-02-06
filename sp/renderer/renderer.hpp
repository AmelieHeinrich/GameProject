/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 22/01/2023 10:44
 */

#pragma once

#include <cstdint>

void RendererInit();
void RendererExit();

void RendererStartSync();
void RendererConstructFrame();
void RendererStartRender();
void RendererEndRender();
void RendererEndSync();

void RendererResize(uint32_t Width, uint32_t Height);
