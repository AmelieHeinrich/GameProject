/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 16/02/2023 18:28
 */

#pragma once

#include "gpu/gpu_buffer.hpp"

enum class tonemapping_algorithm
{
    ACES = 0,
    Filmic = 1,
    RomBinDaHouse = 2
};

struct renderer_settings
{
    gpu_buffer Buffer;

    struct {
        tonemapping_algorithm Tonemapper;
        float Exposure;
    } Settings;
};

void RendererSettingsInit(renderer_settings *Settings);
void RendererSettingsUpdate(renderer_settings *Settings);
void RendererSettingsFree(renderer_settings *Settings);
