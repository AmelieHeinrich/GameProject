/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 11/01/2023 09:50
 */

#pragma once

#include "gpu/gpu_command_buffer.hpp"

void GuiInit();
void GuiBeginFrame();
void GuiEndFrame(gpu_command_buffer *Buffer);
void GuiExit();
