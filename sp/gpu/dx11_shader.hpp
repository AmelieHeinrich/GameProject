/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 10/01/2023 09:35
 */

#pragma once

#include "dx11_common.hpp"

struct gpu_shader
{
    ID3D11VertexShader* VS = nullptr;
    ID3D11PixelShader* PS = nullptr;
    ID3D11ComputeShader* CS = nullptr;

    ID3D11InputLayout* Layout = nullptr;
};

void GpuShaderInit(gpu_shader *Shader, const char *V = NULL,
                                       const char *P = NULL,
                                       const char *C = NULL);
void GpuShaderFree(gpu_shader *Shader);
void GpuShaderBind(gpu_shader *Shader);
