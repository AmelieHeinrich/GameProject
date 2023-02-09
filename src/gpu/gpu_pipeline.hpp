/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/02/2023 13:01
 */

#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "gpu_shader.hpp"
#include "gpu_image.hpp"

enum class gpu_pipeline_type
{
    Graphics,
    Compute
};

enum class fill_mode
{
    Solid,
    Line
};

enum class cull_mode
{
    Back,
    Front,
    None
};

enum class depth_func
{
    Greater,
    Less,
    LessEqual,
    None
};

struct gpu_pipeline_create_info
{
    gpu_pipeline_type Type;
    gpu_shader *Shader;
    fill_mode FillMode;
    cull_mode CullMode;
    std::vector<gpu_image_format> Formats;
    gpu_image_format DepthFormat;
    bool HasDepth;
    depth_func DepthFunc;
};

struct gpu_pipeline
{
    gpu_pipeline_create_info Info;
    void *Private;
};

void GpuPipelineCreateGraphics(gpu_pipeline *Pipeline);
void GpuPipelineCreateCompute(gpu_pipeline *Pipeline);
void GpuPipelineFree(gpu_pipeline *Pipeline);
int GpuPipelineGetDescriptor(gpu_pipeline *Pipeline, const std::string& Name);
