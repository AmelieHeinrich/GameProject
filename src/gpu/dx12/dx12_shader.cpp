/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 02/02/2023 11:04
 */

#include "dx12_shader.hpp"

#include "systems/log_system.hpp"
#include "systems/file_system.hpp"
#include "windows/windows_data.hpp"

#include <string>
#include <cstdlib>
#include <cstring>

#include <d3d12.h>
#include <d3dcompiler.h>
#include <cassert>

std::string GetEntryPointFromProfile(const std::string& Profile)
{
    if (Profile == "vs_5_1")
        return "VSMain";
    if (Profile == "ps_5_1")
        return "PSMain";
    if (Profile == "cs_5_1")
        return "CSMain";
}

dx12_shader_module CompileBlob(const std::string& Source, const char *Profile)
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
    
    dx12_shader_module Module;
    Module.Size = ShaderBlob->GetBufferSize();
    Module.Data = malloc(Module.Size);
    memcpy(Module.Data, ShaderBlob->GetBufferPointer(), Module.Size);
    SafeRelease(ShaderBlob);
    return (Module);
}

void GpuShaderInit(gpu_shader *Shader, const char *V,
                                       const char *P,
                                       const char *C)
{
    Shader->Private = new dx12_shader;
    dx12_shader *Private = (dx12_shader*)(Shader->Private);
    memset(Private, 0, sizeof(dx12_shader));

    if (V)
        Private->VertexBlob = CompileBlob(FileRead(V), "vs_5_1");
    if (P)
        Private->PixelBlob = CompileBlob(FileRead(P), "ps_5_1");
    if (C)
        Private->ComputeBlob = CompileBlob(FileRead(C), "cs_5_1");
}

void GpuShaderInitFromEGS(gpu_shader *Shader, const char *V,
                                              const char *P,
                                              const char *C)
{
    Shader->Private = new dx12_shader;
    dx12_shader *Private = (dx12_shader*)(Shader->Private);

    if (V) {
        Private->VertexBlob.Size = FileBufferGetSize(V);
        Private->VertexBlob.Data = malloc(Private->VertexBlob.Size);
        const char *Buffer = FileRead(V).c_str();
        memcpy(Private->VertexBlob.Data, Buffer, Private->VertexBlob.Size);
    }
    if (P) {
        Private->PixelBlob.Size = FileBufferGetSize(V);
        Private->PixelBlob.Data = malloc(Private->PixelBlob.Size);
        const char *Buffer = FileRead(V).c_str();
        memcpy(Private->PixelBlob.Data, Buffer, Private->PixelBlob.Size);
    }
    if (C) {
        Private->ComputeBlob.Size = FileBufferGetSize(V);
        Private->ComputeBlob.Data = malloc(Private->ComputeBlob.Size);
        const char *Buffer = FileRead(V).c_str();
        memcpy(Private->ComputeBlob.Data, Buffer, Private->ComputeBlob.Size);
    }
}

void GpuShaderFree(gpu_shader *Shader)
{
    dx12_shader *Private = (dx12_shader*)(Shader->Private);
    if (Private->VertexBlob.Data)
        free(Private->VertexBlob.Data);
    if (Private->PixelBlob.Data)
        free(Private->PixelBlob.Data);
    if (Private->ComputeBlob.Data)
        free(Private->ComputeBlob.Data);
    delete Private;
}
