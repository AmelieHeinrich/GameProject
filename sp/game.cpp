/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 11/01/2023 12:33
 */

#include "game.hpp"

#include "timer.hpp"
#include "apu/apu_source.hpp"
#include "cameras/noclip_camera.hpp"
#include "gui/dev_terminal.hpp"
#include "gui/gui.hpp"
#include "gui/settings_panel.hpp"
#include "renderer/renderer.hpp"
#include "systems/event_system.hpp"
#include "systems/input_system.hpp"
#include "systems/shader_system.hpp"
#include "systems/log_system.hpp"

#include <ImGui/imgui.h>

struct game_state
{
    bool TerminalOpen;
    bool TerminalFocus;
    bool SettingsOpen;
    bool SettingsFocus;

    timer Timer;
    float LastFrame;
    noclip_camera Camera;

    apu_source Source;
};

game_state GameState;

bool GameKeyPressed(event_type Type, void *Sender, void *Listener, event_data Data)
{
    if (Type == event_type::KeyPressed)
    {
        if (Data.data.u16[0] == (uint16_t)keyboard_key::F1)
            GameState.TerminalOpen = !GameState.TerminalOpen;
        if (Data.data.u16[0] == (uint16_t)keyboard_key::Escape)
            GameState.SettingsOpen = !GameState.SettingsOpen;
    }
    return false;
}

bool GameResize(event_type Type, void *Sender, void *Listener, event_data Data)
{
    if (Type == event_type::Resize)
    {
        uint32_t Width = Data.data.u32[0];
        uint32_t Height = Data.data.u32[1];

        NoClipCameraResize(&GameState.Camera, Width, Height);
        RendererResize(Width, Height);
    }
    return false;
}

void GameInit()
{
    GameState.TerminalOpen = false;
    GameState.SettingsOpen = false;

    EventSystemRegister(event_type::KeyPressed, nullptr, GameKeyPressed);
    EventSystemRegister(event_type::Resize, nullptr, GameResize);
    DevTerminalInit();

    // RendererInit();
    TimerInit(&GameState.Timer);
    NoClipCameraInit(&GameState.Camera);

    ShaderLibraryPush("Forward", "shaders/forward/Vertex.hlsl", "shaders/forward/Pixel.hlsl");

    ApuSourceInitFile(&GameState.Source, "assets/bgm/test.wav", true);
    ApuSourcePlay(&GameState.Source);
}

void GameUpdate()
{
    float Time = TimerGetElapsed(&GameState.Timer);
    float DT = (Time - GameState.LastFrame) / 1000.0f;
    GameState.LastFrame = Time;

    ApuSourceUpdate(&GameState.Source);

    // RendererUpdate();

    if (!GameState.TerminalFocus && !GameState.SettingsFocus)
        NoClipCameraInput(&GameState.Camera, DT);
    NoClipCameraUpdate(&GameState.Camera, DT);
    NoClipCameraUpdateFrustum(&GameState.Camera);

    // DxRenderContextBegin();
    // GuiBeginFrame();
    // if (GameState.TerminalOpen)
    //     DevTerminalDraw(&GameState.TerminalOpen, &GameState.TerminalFocus);
    // if (GameState.SettingsOpen)
    //     SettingsPanelDraw(&GameState.SettingsOpen, &GameState.SettingsFocus);
    // GuiEndFrame();
}

void GameExit()
{
    ApuSourceFree(&GameState.Source);
    DevTerminalShutdown();
    // RendererExit();
}
