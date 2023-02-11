/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 11/01/2023 12:21
 */

#include "dev_terminal.hpp"

#include "systems/shader_system.hpp"
#include "game_data.hpp"
#include "renderer/renderer.hpp"

#include <stdio.h>
#include <stdarg.h>
#include <sstream>
#include <string>

static int   Stricmp(const char* s1, const char* s2)         { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
static char* Strdup(const char* s)                           { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
static void  Strtrim(char* s)                                { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

dev_terminal DevTerminal;

int TextEditCallback(ImGuiInputTextCallbackData* data)
{
    switch (data->EventFlag)
    {
        case ImGuiInputTextFlags_CallbackHistory:
        {
            // Example of HISTORY
            const int prev_history_pos = DevTerminal.HistoryPos;
            if (data->EventKey == ImGuiKey_UpArrow)
            {
                if (DevTerminal.HistoryPos == -1)
                    DevTerminal.HistoryPos = (int)(DevTerminal.History.size()) - 1;
                else if (DevTerminal.HistoryPos > 0)
                    DevTerminal.HistoryPos--;
            }
            else if (data->EventKey == ImGuiKey_DownArrow)
            {
                if (DevTerminal.HistoryPos != -1)
                    if (++DevTerminal.HistoryPos >= DevTerminal.History.size())
                    DevTerminal.HistoryPos = -1;
            }
            
            // A better implementation would preserve the data on the current input line along with cursor position.
            if (prev_history_pos != DevTerminal.HistoryPos)
            {
                const char* history_str = (DevTerminal.HistoryPos >= 0) ? DevTerminal.History[DevTerminal.HistoryPos] : "";
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, history_str);
            }
        }
    }
    return 0;
}

void DevTerminalClear()
{
    for (auto Item : DevTerminal.Items)
        free(Item);
    DevTerminal.Items.clear();
}

void DevTerminalInitCommands()
{
    DevTerminalAddCommand("clear", [](const std::vector<std::string>&) {
        DevTerminalClear();
    });
    DevTerminalAddCommand("quit", [](const std::vector<std::string>&) {
        ShutdownGame();
    });
    DevTerminalAddCommand("recompile_shader", [](const std::vector<std::string>& Args) {
        if (!Args[1].empty())
            ShaderLibraryRecompile(Args[1]);
    });
    DevTerminalAddCommand("recompile_shaders_all", [](const std::vector<std::string>&) {
        ShaderLibraryRecompileAll();
    });
    DevTerminalAddCommand("reload_settings", [](const std::vector<std::string>&) {
        EgcParseFile("config.egc", &EgcFile);
    });
    DevTerminalAddCommand("load_settings", [](const std::vector<std::string>& Args) {
        if (!Args[1].empty())
            EgcParseFile(Args[1], &EgcFile);
    });
    DevTerminalAddCommand("sync_settings", [](const std::vector<std::string>&) {
        EgcWriteFile("config.egc", &EgcFile);
    });
    DevTerminalAddCommand("sync_settings_path", [](const std::vector<std::string>& Args) {
        if (!Args[1].empty())
            EgcWriteFile(Args[1], &EgcFile);
    });
}

void DevTerminalInit()
{
    memset(DevTerminal.InputBuffer, 0, sizeof(char) * 1024);
    DevTerminal.HistoryPos = -1;
    DevTerminal.AutoScroll = true;

    DevTerminalInitCommands();
}

void DevTerminalShutdown()
{
    DevTerminalClear();
    for (auto History : DevTerminal.History)
        free(History);
}

void DevTerminalDraw(bool* Open, bool* Focused)
{
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Developer Console", Open, ImGuiWindowFlags_NoCollapse))
    {
        ImGui::End();
        return;
    }
    
    *Focused = ImGui::IsWindowFocused();
    
    const float FooterHeighToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -FooterHeighToReserve), false, ImGuiWindowFlags_HorizontalScrollbar);
    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::Selectable("Clear")) DevTerminalClear();
        ImGui::EndPopup();
    }
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
    
    for (auto Item : DevTerminal.Items)
    {
        ImVec4 Color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        if (strstr(Item, "[ERROR]")) Color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
        if (strstr(Item, "[WARN]")) Color = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
        if (strstr(Item, "[INFO]")) Color = ImVec4(0.5f, 0.5f, 1.0f, 1.0f);
        
        ImGui::PushStyleColor(ImGuiCol_Text, Color);
        ImGui::TextUnformatted(Item);
        ImGui::PopStyleColor();
    }
    
    if (DevTerminal.ScrollToBottom || (DevTerminal.AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
        ImGui::SetScrollHereY(1.0f);
    DevTerminal.ScrollToBottom = false;
    
    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::Separator();
    
    bool ReclaimFocus = false;
    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory;
    if (ImGui::InputText("Input", DevTerminal.InputBuffer, 512, input_text_flags, &TextEditCallback))
    {
        char* s = DevTerminal.InputBuffer;
        DevTerminalAddLog("> %s", s);
        std::string Cmd = std::string(s);
        if (Cmd[0] != ' ' && !Cmd.empty())
        {
            std::istringstream Stream(Cmd);
            std::vector<std::string> Args;

            std::string Arg;
            while (Stream >> Arg)
                Args.push_back(Arg);

            DevTerminal.HistoryPos = -1;
            for (int i = (int)DevTerminal.History.size() - 1; i >= 0; i--)
            {
                if (Stricmp(DevTerminal.History[i], Args[0].c_str()) == 0)
                {
                    free(DevTerminal.History[i]);
                    DevTerminal.History.erase(DevTerminal.History.begin() + i);
                    break;
                }
            }

            bool ShouldFindCommand = true;
            //for (auto cvar = cvar_registry.cvars.begin(); cvar != cvar_registry.cvars.end(); ++cvar)
            //{
            //    if (args[0] == cvar->first)
            //    {
            //        if (cvar->second.type == sp_cvar_type::cvar_int)
            //            cvar_registry.set_i(args[0], std::stoi(args[1]));
            //        if (cvar->second.type == sp_cvar_type::cvar_float)
            //            cvar_registry.set_f(args[0], std::stof(args[1]));
            //        if (cvar->second.type == sp_cvar_type::cvar_string)
            //            cvar_registry.set_s(args[0], args[1]);
            //        should_find_command = false;
            //    }
            //}

            if (ShouldFindCommand)
            {
                bool FoundCommand = false;
                for (auto Command = DevTerminal.Commands.begin(); Command != DevTerminal.Commands.end(); ++Command)
                {
                    if (Args[0] == Command->first)
                    {
                        DevTerminal.History.push_back(Strdup(s));
                        FoundCommand = true;

                        Command->second(Args);
                    }
                }
                if (!FoundCommand)
                    DevTerminalAddLog("Unknown command: %s", Args[0].c_str());
            }
        }
        DevTerminal.ScrollToBottom = true;
        strcpy(s, "");
        ReclaimFocus = true;
    }
    
    ImGui::SetItemDefaultFocus();
    if (ReclaimFocus)
        ImGui::SetKeyboardFocusHere(-1);
    
    ImGui::End();
}

void DevTerminalAddLog(const char* Format, ...)
{
    char Buf[1024];
    va_list Args;
    va_start(Args, Format);
    vsnprintf(Buf, 1024, Format, Args);
    Buf[1023] = 0;
    va_end(Args);
    DevTerminal.Items.push_back(Strdup(Buf));
}

void DevTerminalAddCommand(const char *Name, PFN_OnConsoleCommand Command)
{
    DevTerminal.Commands[Name] = Command;
}
