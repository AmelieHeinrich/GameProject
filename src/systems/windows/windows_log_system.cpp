/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 04/01/2023 17:53
 */

#include "systems/log_system.hpp"

#include "game_data.hpp"
#include "gui/dev_terminal.hpp"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include <Windows.h>

log_buffer LogBuffer;

void LogOutput(log_level Level, const char *Message, ...)
{
    if (Level == log_level::Debug && EgcB32(EgcFile, "debug_enabled") == false)
        return;

    const char* LevelStrings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]:  ", "[INFO]:  ", "[DEBUG]: ", "[TRACE]: "};
    bool IsError = Level < log_level::Warn;
	
    char OutMessage[32000] = {0};
	
    va_list ArgPointer;
    va_start(ArgPointer, Message);
    vsnprintf(OutMessage, 32000, Message, ArgPointer);
    va_end(ArgPointer);
	
    char FinalMessage[32000];
    sprintf(FinalMessage, "%s%s\n", LevelStrings[static_cast<uint16_t>(Level)], OutMessage);

    std::string Line(FinalMessage);
    LogBuffer.LogTracker.push_back(Line);

    HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (IsError)
        ConsoleHandle = GetStdHandle(STD_ERROR_HANDLE);
    static uint8_t Levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(ConsoleHandle, Levels[static_cast<uint16_t>(Level)]);
    OutputDebugStringA(FinalMessage);
    uint64_t Length = strlen(FinalMessage);
    WriteConsoleA(ConsoleHandle, FinalMessage, (DWORD)Length, nullptr, 0);

    DevTerminalAddLog("%s", FinalMessage);
}

void LogResetColor()
{
    HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(ConsoleHandle, 7);
    ConsoleHandle = GetStdHandle(STD_ERROR_HANDLE);
    SetConsoleTextAttribute(ConsoleHandle, 7);
}

void LogSaveFile(const std::string& Path)
{
    std::ofstream LogFile(Path, std::ios::trunc);
    for (auto& Line : LogBuffer.LogTracker)
        LogFile << Line;
    LogFile.close();
}
