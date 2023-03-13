/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 11/01/2023 15:27
 */

#include "shader_system.hpp"
#include "log_system.hpp"
#include "event_system.hpp"
#include "rng_system.hpp"
#include "util/timer.hpp"

shader_library Library;

void ShaderLibraryPush(const std::string& ShaderName, const std::string& VS, const std::string& PS, const std::string& CS)
{
    timer Timer;
    TimerInit(&Timer);

    shader_entry* Entry = &Library.Entries[ShaderName];
    Entry->ID = RngGenerate() * 10000000;
    const char* V = VS.empty() ? nullptr : VS.c_str();
    const char* P = PS.empty() ? nullptr : PS.c_str();
    const char* C = CS.empty() ? nullptr : CS.c_str();
    GpuShaderInit(&Entry->Shader, V, P, C);
    Entry->VS = VS;
    Entry->PS = PS;
    Entry->CS = CS;
    Library.Exists.push_back(ShaderName);

    LogInfo("Compiled shader %s in %f seconds", ShaderName.c_str(), ToSeconds(TimerGetElapsed(&Timer)));
}

void ShaderLibraryErase(const std::string& ShaderName)
{
    GpuShaderFree(&Library.Entries[ShaderName].Shader);
}

void ShaderLibraryFree()
{
    for (auto& Shader : Library.Entries)
        ShaderLibraryErase(Shader.first);
}

void ShaderLibraryRecompile(const std::string& ShaderName)
{
    bool Found = false;
    for (auto Shader : Library.Exists)
    {
        if (Shader == ShaderName)
        {
            Found = true;
            break;
        }
    }
    if (!Found)
    {
        LogError("No shader with name %s found in shader library!", ShaderName.c_str());
        return;
    }

    timer Timer;
    TimerInit(&Timer);

    shader_entry *Entry = &Library.Entries[ShaderName];
    GpuShaderFree(&Entry->Shader);
    const char* VS = Entry->VS.empty() ? nullptr : Entry->VS.c_str();
    const char* PS = Entry->PS.empty() ? nullptr : Entry->PS.c_str();
    const char* CS = Entry->CS.empty() ? nullptr : Entry->CS.c_str();
    GpuShaderInit(&Entry->Shader, VS, PS, CS);

    event_data Data = {};
    Data.data.u32[0] = Entry->ID;
    EventSystemFire(event_type::ShaderRecompile, nullptr, Data);

    LogInfo("Recompiled shader %s in %f seconds", ShaderName.c_str(), ToSeconds(TimerGetElapsed(&Timer)));
}

void ShaderLibraryRecompileAll()
{
    timer Timer;
    TimerInit(&Timer);

    for (auto& Shader : Library.Entries)
        ShaderLibraryRecompile(Shader.first);

    LogInfo("Recompiled all shaders in %f seconds", ToSeconds(TimerGetElapsed(&Timer)));
}

int ShaderLibraryGetID(const std::string& Name)
{
    return Library.Entries[Name].ID;
}

gpu_shader *ShaderLibraryGet(const std::string& Name)
{
    return &Library.Entries[Name].Shader;
}
