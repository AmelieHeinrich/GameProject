/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 16/02/2023 18:38
 */

#include "renderer_settings.hpp"

#include "systems/allocator_system.hpp"

void RendererSettingsInit(renderer_settings *Settings)
{
    GpuBufferInit(&Settings->Buffer, 256, 0, gpu_buffer_type::Uniform);
}

void RendererSettingsFree(renderer_settings *Settings)
{
    GpuBufferFree(&Settings->Buffer);
}

void RendererSettingsUpdate(renderer_settings *Settings)
{
    GpuBufferUpload(&Settings->Buffer, &Settings->Settings, sizeof(renderer_settings));
}
