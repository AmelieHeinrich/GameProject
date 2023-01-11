/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 11/01/2023 12:18
 */

#pragma once

#include "game_data.hpp"
#include <ImGui/imgui.h>
#include <vector>
#include <unordered_map>

typedef void (*PFN_OnConsoleCommand)(const std::vector<std::string>&);

struct dev_terminal
{
    std::unordered_map<const char*, PFN_OnConsoleCommand> Commands;
    char InputBuffer[1024];
    std::vector<char*> Items;
    std::vector<char*> History;
    int HistoryPos;
    bool AutoScroll;
    bool ScrollToBottom;
};

extern dev_terminal DevTerminal;

void DevTerminalInit();
void DevTerminalShutdown();
void DevTerminalDraw(bool* Open, bool* Focused);
void DevTerminalAddLog(const char* Format, ...);
void DevTerminalAddCommand(const char *Name, PFN_OnConsoleCommand Command);

