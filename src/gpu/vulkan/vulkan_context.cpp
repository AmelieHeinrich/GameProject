/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/03/2023 09:56
 */

#include "vulkan_context.hpp"

#include "platform_detection.hpp"
#include "systems/log_system.hpp"

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
    VkApplicationInfo ApplicationInfo = {};
    ApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    ApplicationInfo.pApplicationName = "Game Project";
    ApplicationInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    ApplicationInfo.pEngineName = "Cheese Toast Engine";
    ApplicationInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    ApplicationInfo.apiVersion = VK_API_VERSION_1_3;

    // TODO(amelie.h): Pick instance layers and extensions

    VkInstanceCreateInfo CreateInfo = {};
    CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    CreateInfo.pApplicationInfo = &ApplicationInfo;

    VkResult Result = vkCreateInstance(&CreateInfo, nullptr, &VK.Instance);
    if (Result != VK_SUCCESS) {
        LogError("VULKAN: Failed to create instance!");
    }

    // TODO(amelie.h): Pick physical device
    // TODO(amelie.h): Create logical device
    // TODO(amelie.h): Create command queues
}

void GpuExit()
{
    vkDestroyInstance(VK.Instance, nullptr);
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
