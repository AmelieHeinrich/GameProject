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

    Result = CreateSurface(VK.Instance, &VK.Surface);
    if (Result != VK_SUCCESS)
        LogError("VULKAN: Failed to create window surface!");

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

    int QueueCount = 1;
    float QueuePriority = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> QueueInfos(3);
    QueueInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    QueueInfos[0].pQueuePriorities = &QueuePriority;
    QueueInfos[0].queueCount = 1;
    QueueInfos[0].queueFamilyIndex = VK.GraphicsQueueFamily;
    QueueInfos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    QueueInfos[1].pQueuePriorities = &QueuePriority;
    QueueInfos[1].queueCount = 1;
    QueueInfos[1].queueFamilyIndex = VK.ComputeQueueFamily;
    QueueInfos[2].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    QueueInfos[2].pQueuePriorities = &QueuePriority;
    QueueInfos[2].queueCount = 1;
    QueueInfos[2].queueFamilyIndex = VK.UploadQueueFamily;

    if (VK.GraphicsQueueFamily != VK.ComputeQueueFamily)
        QueueCount++;
    if (VK.GraphicsQueueFamily != VK.UploadQueueFamily)
        QueueCount++;

    VkPhysicalDeviceFeatures Features;
    vkGetPhysicalDeviceFeatures(VK.PhysicalDevice, &Features);

    VkDeviceCreateInfo DeviceInfo = {};
    DeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    DeviceInfo.enabledExtensionCount = 1;
    DeviceInfo.ppEnabledExtensionNames = VulkanExtensions;
    DeviceInfo.pQueueCreateInfos = QueueInfos.data();
    DeviceInfo.queueCreateInfoCount = QueueCount;
    DeviceInfo.pEnabledFeatures = &Features;

    Result = vkCreateDevice(VK.PhysicalDevice, &DeviceInfo, nullptr, &VK.Device);
    if (Result != VK_SUCCESS) {
        LogError("VULKAN: Failed to create device!");
    }

    vkGetDeviceQueue(VK.Device, VK.GraphicsQueueFamily, 0, &VK.GraphicsQueue);
    vkGetDeviceQueue(VK.Device, VK.ComputeQueueFamily, 0, &VK.ComputeQueue);
    vkGetDeviceQueue(VK.Device, VK.UploadQueueFamily, 0, &VK.UploadQueue);

    // TODO(amelie.h): Create command queues
    VkCommandPoolCreateInfo PoolInfo = {};
    PoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    PoolInfo.queueFamilyIndex = VK.GraphicsQueueFamily;
    PoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    Result = vkCreateCommandPool(VK.Device, &PoolInfo, nullptr, &VK.GraphicsPool);
    if (Result != VK_SUCCESS)
        LogError("VULKAN: Failed to create graphics command pool!");
    PoolInfo.queueFamilyIndex = VK.ComputeQueueFamily;
    Result = vkCreateCommandPool(VK.Device, &PoolInfo, nullptr, &VK.ComputePool);
    if (Result != VK_SUCCESS)
        LogError("VULKAN: Failed to create compute command pool!");
    PoolInfo.queueFamilyIndex = VK.UploadQueueFamily;
    Result = vkCreateCommandPool(VK.Device, &PoolInfo, nullptr, &VK.UploadPool);
    if (Result != VK_SUCCESS)
        LogError("VULKAN: Failed to create upload command pool!");

    VkFenceCreateInfo FenceInfo = {};
    FenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    
    Result = vkCreateFence(VK.Device, &FenceInfo, nullptr, &VK.GraphicsFence);
    if (Result != VK_SUCCESS)
        LogError("VULKAN: Failed to create graphics queue fence!");
    Result = vkCreateFence(VK.Device, &FenceInfo, nullptr, &VK.ComputeFence);
    if (Result != VK_SUCCESS)
        LogError("VULKAN: Failed to create compute queue fence!");
    Result = vkCreateFence(VK.Device, &FenceInfo, nullptr, &VK.UploadFence);
    if (Result != VK_SUCCESS)
        LogError("VULKAN: Failed to create upload queue fence!");
}

void GpuExit()
{
    vkDeviceWaitIdle(VK.Device);

    vkDestroyFence(VK.Device, VK.GraphicsFence, nullptr);
    vkDestroyFence(VK.Device, VK.ComputeFence, nullptr);
    vkDestroyFence(VK.Device, VK.UploadFence, nullptr);
    vkDestroyCommandPool(VK.Device, VK.UploadPool, nullptr);
    vkDestroyCommandPool(VK.Device, VK.ComputePool, nullptr);
    vkDestroyCommandPool(VK.Device, VK.GraphicsPool, nullptr);
    vkDestroyDevice(VK.Device, nullptr);
    vkDestroySurfaceKHR(VK.Instance, VK.Surface, nullptr);
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
