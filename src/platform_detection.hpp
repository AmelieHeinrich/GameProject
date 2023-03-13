/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 13/03/2023 19:41
 */

#pragma once

#if defined(_WIN32)
    #define ENGINE_PLATFORM_WINDOWS
    #define ENGINE_VK_SURFACE_EXTENSION "VK_KHR_win32_surface"
#elif defined(__linux__)
    #error "Unsupported platform!
#elif defined(__APPLE__)
    #error "Unsupported platform!
#endif
