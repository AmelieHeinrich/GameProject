/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 16/02/2023 18:28
 */

#pragma once

#include "gpu/gpu_buffer.hpp"

#include "math_types.hpp"

#include <cstdint>

enum class tonemapping_algorithm : int
{
    ACES = 0,
    Filmic = 1,
    RomBinDaHouse = 2
};

struct settings_value
{
    tonemapping_algorithm Tonemapper;
    uint32_t _Padding1;
    uint32_t _Padding2;
    uint32_t _Padding3;
    float Exposure;
    float Temperature;
    float Tint;
    uint32_t _Padding4;
    V3 Contrast;
    uint32_t _Padding5;
    V3 LinearMidPoint;
    uint32_t _Padding6;
    V3 Brightness;
    uint32_t _Padding7;
    V3 ColorFilter;
    uint32_t _Padding8;
    float ColorFilterIntensity;
    V3 Saturation;
};

struct renderer_settings
{
    gpu_buffer Buffer;

    bool Wireframe;
    bool EnableColorCorrection;

    settings_value Settings;
};

void RendererSettingsInit(renderer_settings *Settings);
void RendererSettingsUpdate(renderer_settings *Settings);
void RendererSettingsFree(renderer_settings *Settings);
