/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 12/01/2023 09:55
 */

#include "settings_panel.hpp"

#include "game_data.hpp"
#include <ImGui/imgui.h>

#include "renderer/renderer.hpp"

#define SETTINGS_GRAPHICS 0
#define SETTINGS_MOUSE 1
#define SETTINGS_AUDIO 2

struct settings_panel
{
    int TabIndex;
};

void SettingsDrawGraphics()
{
    renderer_settings *Settings = RendererGetSettings();

    if (ImGui::TreeNodeEx("Graphics", ImGuiTreeNodeFlags_Framed))
    {
        bool VerticalSync = EgcB32(EgcFile, "vsync");
        ImGui::Checkbox("Vertical Sync", &VerticalSync);
        EgcB32(EgcFile, "vsync") = VerticalSync;

        if (ImGui::TreeNodeEx("Geometry Pass", ImGuiTreeNodeFlags_Framed))
        {
            ImGui::Checkbox("Wireframe", &Settings->Wireframe);

            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx("Color Correction", ImGuiTreeNodeFlags_Framed))
        {
            ImGui::Checkbox("Enable", &Settings->EnableColorCorrection);
            ImGui::Separator();

            ImGui::SliderFloat("Exposure", &Settings->Settings.Exposure, 0.1f, 5.0f, "%.1f");
            ImGui::SliderFloat("Temperature", &Settings->Settings.Temperature, 0.0f, 1.0f, "%.1f");
            ImGui::SliderFloat("Tint", &Settings->Settings.Tint, 0.0f, 1.0f, "%.1f");
            ImGui::SliderFloat3("Contrast", Settings->Settings.Contrast.Elements, 0.0f, 5.0f, "%.1f");
            ImGui::SliderFloat3("Linear Mid Point", Settings->Settings.LinearMidPoint.Elements, 0.0f, 5.0f, "%.1f");
            ImGui::SliderFloat3("Brightness", Settings->Settings.Brightness.Elements, 0.0f, 5.0f, "%.1f");
            ImGui::ColorPicker3("Color Filter", Settings->Settings.ColorFilter.Elements);
            ImGui::SliderFloat("Color Filter Intensity", &Settings->Settings.ColorFilterIntensity, 0.0f, 5.0f, "%.1f");
            ImGui::SliderFloat3("Saturation", Settings->Settings.Saturation.Elements, 0.0f, 5.0f, "%.1f");

            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx("Tonemapping", ImGuiTreeNodeFlags_Framed))
        {
            static const char* Tonemappers[] = { "ACES", "Filmic", "Rom Bin Da House" };
            ImGui::Combo("Tonemapper", (int*)&Settings->Settings.Tonemapper, Tonemappers, 3);

            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
}

void SettingsDrawMouse()
{
    if (ImGui::TreeNodeEx("Mouse", ImGuiTreeNodeFlags_Framed))
    {
        float Sensitivity = EgcF32(EgcFile, "mouse_sensitivity");
        ImGui::SliderFloat("Sensitivity", &Sensitivity, 0.1f, 8.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
        EgcF32(EgcFile, "mouse_sensitivity") = Sensitivity;

        ImGui::TreePop();
    }
}

void SettingsDrawAudio()
{
    if (ImGui::TreeNodeEx("Sound", ImGuiTreeNodeFlags_Framed))
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

        ImGui::TreePop();
    }
}

void SettingsPanelDraw(bool *Closed, bool *Focused)
{
    ImGui::Begin("Settings", Closed);

    *Focused = ImGui::IsWindowFocused();

    SettingsDrawGraphics();
    SettingsDrawMouse();
    SettingsDrawAudio();

    if (ImGui::Button("Apply"))
        EgcWriteFile("config.egc", &EgcFile);

    ImGui::End();
}
