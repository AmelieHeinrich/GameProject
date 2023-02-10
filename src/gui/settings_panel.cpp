/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 12/01/2023 09:55
 */

#include "settings_panel.hpp"

#include "game_data.hpp"
#include <ImGui/imgui.h>

#define SETTINGS_GRAPHICS 0
#define SETTINGS_MOUSE 1
#define SETTINGS_AUDIO 2

struct settings_panel
{
    int TabIndex;
};

void SettingsDrawGraphics()
{
    bool VerticalSync = EgcB32(EgcFile, "vsync");
    ImGui::Checkbox("Vertical Sync", &VerticalSync);
    EgcB32(EgcFile, "vsync") = VerticalSync;
}

void SettingsDrawMouse()
{
    float Sensitivity = EgcF32(EgcFile, "mouse_sensitivity");

    ImGui::SliderFloat("Sensitivity", &Sensitivity, 0.1f, 8.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);

    EgcF32(EgcFile, "mouse_sensitivity") = Sensitivity;;
}

void SettingsDrawAudio()
{
    float Music = EgcF32(EgcFile, "music_volume");
    float Sound = EgcF32(EgcFile, "sound_volume");
    float Voice = EgcF32(EgcFile, "voice_volume");

    ImGui::SliderFloat("Music Volume", &Music, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderFloat("Sound Volume", &Sound, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderFloat("Voice Volume", &Voice, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);

    EgcF32(EgcFile, "music_volume") = Music;
    EgcF32(EgcFile, "sound_volume") = Sound;
    EgcF32(EgcFile, "voice_volume") = Voice;
}

void SettingsPanelDraw(bool *Closed, bool *Focused)
{
    ImGui::Begin("Settings", Closed, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

    *Focused = ImGui::IsWindowFocused();

    SettingsDrawGraphics();
    SettingsDrawMouse();
    SettingsDrawAudio();

    if (ImGui::Button("Apply"))
        EgcWriteFile("config.egc", &EgcFile);

    ImGui::End();
}