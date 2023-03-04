/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 28/02/2023 15:48
 */

#pragma once

#include <string>

enum class ShaderType
{
    Vertex,
    Pixel,
    Compute
};

class ShaderCompiler
{
public:
    static void Compile(const std::string& Path, const std::string& Output, ShaderType Type);
};
