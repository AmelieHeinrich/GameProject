/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 04/01/2023 17:53
 */

#include "log_system.hpp"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <Windows.h>

void LogOutput(log_level Level, const char *Message, ...)
{
    const char* LevelStrings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]:  ", "[INFO]:  ", "[DEBUG]: ", "[TRACE]: "};
    bool IsError = Level < log_level::Warn;
	
    char OutMessage[32000] = {0};
	
    va_list ArgPointer;
    va_start(ArgPointer, Message);
    vsnprintf(OutMessage, 32000, Message, ArgPointer);
    va_end(ArgPointer);
	
    char out_message2[32000];
    sprintf(out_message2, "%s%s\n", LevelStrings[static_cast<uint16_t>(Level)], OutMessage);

    HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (IsError)
        ConsoleHandle = GetStdHandle(STD_ERROR_HANDLE);
    static uint8_t Levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(ConsoleHandle, Levels[static_cast<uint16_t>(Level)]);
    OutputDebugStringA(Message);
    uint64_t Length = strlen(Message);
    WriteConsoleA(ConsoleHandle, Message, (DWORD)Length, NULL, 0);
}
