/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 02/02/2023 11:04
 */

#include "gpu/gpu_shader.hpp"

#include "systems/log_system.hpp"
#include "systems/file_system.hpp"
#include "windows/windows_data.hpp"

#include <string>

#include <d3d12.h>
#include <d3dcompiler.h>
#include <cassert>

struct dx12_shader
{
    ID3DBlob *VertexBlob;
    ID3DBlob *PixelBlob;
    ID3DBlob *ComputeBlob;
};

std::string GetEntryPointFromProfile(const std::string& Profile)
{
    if (Profile == "vs_5_1")
        return "VSMain";
    if (Profile == "ps_5_1")
        return "PSMain";
    if (Profile == "cs_5_1")
        return "CSMain";
}

ID3DBlob* CompileBlob(const std::string& Source, const char *Profile)
{
    ID3DBlob* ShaderBlob;
    ID3DBlob* ErrorBlob;
    D3DCompile(Source.c_str(), 
               Source.length(), 
               nullptr, 
               nullptr, 
               D3D_COMPILE_STANDARD_FILE_INCLUDE, 
               GetEntryPointFromProfile(Profile).c_str(), 
               Profile, 
               0, 
               0, 
               &ShaderBlob, 
               &ErrorBlob);
    if (ErrorBlob)
    {
        LogError("Shader Error (Profile: %s) : %s", Profile, (char*)ErrorBlob->GetBufferPointer());
        SafeRelease(ErrorBlob);
    }
    return ShaderBlob;
}

void GpuShaderInit(gpu_shader *Shader, const char *V,
                                       const char *P,
                                       const char *C)
{
    Shader->Private = new dx12_shader;
    dx12_shader *Private = (dx12_shader*)(Shader->Private);
    Private->VertexBlob = nullptr;
    Private->PixelBlob = nullptr;
    Private->ComputeBlob = nullptr;

    if (V)
        Private->VertexBlob = CompileBlob(FileRead(V), "vs_5_1");
    if (P)
        Private->PixelBlob = CompileBlob(FileRead(P), "ps_5_1");
    if (C)
        Private->ComputeBlob = CompileBlob(FileRead(C), "cs_5_1");
}

void GpuShaderFree(gpu_shader *Shader)
{
    dx12_shader *Private = (dx12_shader*)(Shader->Private);
    SafeRelease(Private->VertexBlob);
    SafeRelease(Private->PixelBlob);
    SafeRelease(Private->ComputeBlob);
    delete Private;
}
