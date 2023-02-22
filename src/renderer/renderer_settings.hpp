/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 16/02/2023 18:28
 */

#pragma once

#include "gpu/gpu_buffer.hpp"

#include "math_types.hpp"

enum class tonemapping_algorithm : int
{
    ACES = 0,
    Filmic = 1,
    RomBinDaHouse = 2
};

struct settings_value
{
    tonemapping_algorithm Tonemapper;
    float Exposure;
    float Temperature;
    float Tint;
    V3 Contrast;
    V3 LinearMidPoint;
    V3 Brightness;
    V3 ColorFilter;
    float ColorFilterIntensity;
    V3 Saturation;
    float SharpnessStrength;

    float pad[6];
};

struct renderer_settings
{
    gpu_buffer Buffer;

    bool Wireframe;
    bool EnableSharpness;
    bool EnableColorCorrection;

    settings_value Settings;
};

void RendererSettingsInit(renderer_settings *Settings);
void RendererSettingsUpdate(renderer_settings *Settings);
void RendererSettingsFree(renderer_settings *Settings);
