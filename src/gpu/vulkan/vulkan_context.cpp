/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/03/2023 09:56
 */

#include "vulkan_context.hpp"

#include "game_data.hpp"
#include "platform_detection.hpp"
#include "systems/log_system.hpp"

static const char *VulkanInstanceLayers[] = {
    "VK_LAYER_KHRONOS_validation"
};

static const char *VulkanInstanceExtensions[] = {
    "VK_KHR_surface",
    ENGINE_VK_SURFACE_EXTENSION
};

static const char *VulkanExtensions[] = {
    "VK_KHR_swapchain"
};

vulkan_context VK;

gpu_backend GpuGetBackend()
{
    return gpu_backend::Vulkan;
}

uint64_t GetPhysicalDeviceScore(VkPhysicalDevice Device)
{
    uint64_t Score = 0;

    VkPhysicalDeviceFeatures Features;
    vkGetPhysicalDeviceFeatures(Device, &Features);
    VkPhysicalDeviceProperties Properties;
    vkGetPhysicalDeviceProperties(Device, &Properties);

    Score += Properties.limits.maxComputeSharedMemorySize * 30;
    Score += Properties.limits.maxBoundDescriptorSets * 20;
    Score += Features.fillModeNonSolid ? 1000 : 0;
    Score += Features.samplerAnisotropy ? 1000 : 0;
    Score += Features.pipelineStatisticsQuery ? 1000 : 0;
    Score += Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 1000000 : 0;
    
    return Score;
}

void GpuInit()
{
    bool Debug = EgcB32(EgcFile, "debug_enabled");

    VkApplicationInfo ApplicationInfo = {};
    ApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    ApplicationInfo.pApplicationName = "Game Project";
    ApplicationInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    ApplicationInfo.pEngineName = "Cheese Toast Engine";
    ApplicationInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    ApplicationInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo CreateInfo = {};
    CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    CreateInfo.pApplicationInfo = &ApplicationInfo;
    CreateInfo.ppEnabledExtensionNames = VulkanInstanceExtensions;
    CreateInfo.enabledExtensionCount = 2;
    if (Debug) {
        CreateInfo.ppEnabledLayerNames = VulkanInstanceLayers;
        CreateInfo.enabledLayerCount = 1;
    }

    VkResult Result = vkCreateInstance(&CreateInfo, nullptr, &VK.Instance);
    if (Result != VK_SUCCESS) {
        LogError("VULKAN: Failed to create instance!");
    }

    uint32_t DeviceCount = 0;
    vkEnumeratePhysicalDevices(VK.Instance, &DeviceCount, nullptr);
    if (DeviceCount == 0) {
        LogError("VULKAN: No GPU found!");
    }
    std::vector<VkPhysicalDevice> Devices(DeviceCount);
    vkEnumeratePhysicalDevices(VK.Instance, &DeviceCount, Devices.data());

    uint64_t PreviousScore = 0;
    for (auto Device : Devices) {
        uint64_t Score = GetPhysicalDeviceScore(Device);
        if (Score > PreviousScore) {
            PreviousScore = Score;
            VK.PhysicalDevice = Device;
        }
    }

    VkPhysicalDeviceProperties Properties;
    vkGetPhysicalDeviceProperties(VK.PhysicalDevice, &Properties);
    LogInfo("VULKAN: Using GPU %s", Properties.deviceName);

    VK.GraphicsQueueFamily = -1;
    VK.ComputeQueueFamily = -1;
    VK.UploadQueueFamily = -1;
    uint32_t QueueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(VK.PhysicalDevice, &QueueFamilyCount, nullptr);
    if (QueueFamilyCount == 0) {
        LogError("VULKAN: No queue families found!");
    }
    std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(VK.PhysicalDevice, &QueueFamilyCount, QueueFamilies.data());
    for (int QueueIterator = 0; QueueIterator < QueueFamilies.size(); QueueIterator++) {
        auto Family = QueueFamilies[QueueIterator];
        if (Family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            VK.GraphicsQueueFamily = QueueIterator;
        if (Family.queueFlags & VK_QUEUE_COMPUTE_BIT)
            VK.ComputeQueueFamily = QueueIterator;
        if (Family.queueFlags & VK_QUEUE_TRANSFER_BIT)
            VK.UploadQueueFamily = QueueIterator;
        if (VK.GraphicsQueueFamily != -1 && VK.ComputeQueueFamily != -1 && VK.UploadQueueFamily != -1)
            break;
    }
    if (VK.GraphicsQueueFamily != -1 && VK.ComputeQueueFamily != -1 && VK.UploadQueueFamily != -1)
        LogInfo("VULKAN: Found graphics, compute and transfer queue");

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
