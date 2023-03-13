/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/03/2023 09:56
 */

#include "vulkan_context.hpp"

#include "platform_detection.hpp"

static const char *VulkanInstanceLayers[] = {
    "VK_LAYER_KHRONOS_validation"
};

static const char *VulkanInstanceExtensions[] = {
    "VK_KHR_surface",
    ENGINE_VK_SURFACE_EXTENSION
};

vulkan_context VK;

gpu_backend GpuGetBackend()
{
    return gpu_backend::Vulkan;
}

void GpuInit()
{
    // TODO(amelie.h): Initialise instance
    // TODO(amelie.h): Pick physical device
    // TODO(amelie.h): Create logical device
    // TODO(amelie.h): Create command queues
}

void GpuExit()
{

}

void GpuBeginFrame()
{

}

void GpuEndFrame()
{

}

void GpuResize(uint32_t Width, uint32_t Height)
{

}

void GpuPresent()
{

}

void GpuWait()
{

}

hmm_v2 GpuGetDimensions()
{
    return HMM_Vec2(VK.Width, VK.Height);
}

gpu_command_buffer* GpuGetImageCommandBuffer()
{
    return nullptr;
}

gpu_image* GpuGetSwapChainImage()
{
    return nullptr;   
}
