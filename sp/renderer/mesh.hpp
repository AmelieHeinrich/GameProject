/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 22/01/2023 11:31
 */

#pragma once

#include <string>
#include <vector>

#include "math_types.hpp"
#include "gpu/dx11/dx11_buffer.hpp"
#include "gpu/dx11/dx11_texture.hpp"

struct mesh_vertex
{
    V3 Position;
    V3 Normal;
    V2 UV;
};

struct mesh
{
    gpu_buffer VertexBuffer;
    gpu_buffer IndexBuffer;
    int VertexCount;
    int IndexCount;

    gpu_texture AlbedoTexture;
};

struct loaded_model
{
    std::vector<mesh> Meshes;
    std::string WorkingDirectory;
};

void ModelLoad(loaded_model *Model, const std::string& Path);
void ModelFree(loaded_model *Model);
