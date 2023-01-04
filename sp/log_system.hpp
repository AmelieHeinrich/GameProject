/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 04/01/2023 17:48
 */

#pragma once

#include <cstdint>

enum class log_level : uint16_t
{
    Fatal,
    Error,
    Warn,
    Info,
    Debug,
    Trace
};

void LogOutput(log_level Level, const char *Message, ...);

#define LogFatal(message, ...) LogOutput(log_level::Fatal, message, ##__VA_ARGS__)
#define LogError(message, ...) LogOutput(log_level::Error, message, ##__VA_ARGS__)
#define LogWarn(message, ...) LogOutput(log_level::Warn, message, ##__VA_ARGS__)
#define LogInfo(message, ...) LogOutput(log_level::Info, message, ##__VA_ARGS__)
#define LogDebug(message, ...) LogOutput(log_level::Debug, message, ##__VA_ARGS__)
#define LogTrace(message, ...) LogOutput(log_level::Trace, message, ##__VA_ARGS__)
