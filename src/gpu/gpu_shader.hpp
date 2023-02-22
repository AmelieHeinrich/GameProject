/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 02/02/2023 11:04
 */

#pragma once

struct gpu_shader
{
    void *Private;
};

void GpuShaderInit(gpu_shader *Shader, const char *V = nullptr,
                                       const char *P = nullptr,
                                       const char *C = nullptr);
void GpuShaderInitFromEGS(gpu_shader *Shader, const char *V = nullptr,
                                              const char *P = nullptr,
                                              const char *C = nullptr);
void GpuShaderFree(gpu_shader *Shader);
