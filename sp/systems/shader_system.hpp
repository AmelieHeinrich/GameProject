/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 11/01/2023 15:24
 */

#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "gpu/dx11_shader.hpp"

struct shader_entry
{
    std::string VS;
    std::string PS;
    std::string CS;
    gpu_shader Shader;
};

struct shader_library
{
    std::unordered_map<std::string, shader_entry> Entries;
    std::vector<std::string> Exists;
};

void ShaderLibraryPush(const std::string& ShaderName, const std::string& VS = "", const std::string& PS = "", const std::string& CS = "");
void ShaderLibraryErase(const std::string& ShaderName);
void ShaderLibraryFree();
void ShaderLibraryRecompile(const std::string& ShaderName);
void ShaderLibraryRecompileAll();
void ShaderLibraryBind(const std::string& ShaderName);
