/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 11/01/2023 12:33
 */

#include "game.hpp"

#include "timer.hpp"
#include "apu/dsound_source.hpp"
#include "cameras/noclip_camera.hpp"
#include "gpu/dx11_buffer.hpp"
#include "gpu/dx11_context.hpp"
#include "gpu/dx11_render_state.hpp"
#include "gpu/dx11_shader.hpp"
#include "gui/dev_terminal.hpp"
#include "gui/gui.hpp"
#include "gui/settings_panel.hpp"
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

    gpu_buffer ConstantBuffer;
    gpu_buffer Buffer;
    gpu_render_state RenderState;

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
    return true;
}

bool GameResize(event_type Type, void *Sender, void *Listener, event_data Data)
{
    if (Type == event_type::Resize)
        NoClipCameraResize(&GameState.Camera, Data.data.u32[0], Data.data.u32[1]);
    return true;
}

void GameInit()
{
    GameState.TerminalOpen = false;
    GameState.SettingsOpen = false;

    float Data[] = 
    {
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    ShaderLibraryPush("Forward", "shaders/forward/Vertex.hlsl", "shaders/forward/Pixel.hlsl");

    GpuBufferCreate(&GameState.Buffer, sizeof(Data), sizeof(float) * 6, gpu_buffer_usage::Vertex);
    GpuBufferUploadData(&GameState.Buffer, Data);

    GpuBufferCreate(&GameState.ConstantBuffer, sizeof(hmm_mat4) * 2, 0, gpu_buffer_usage::Constant);

    GameState.RenderState.CounterClockwise = false;
    GameState.RenderState.CullMode = render_state_cull_mode::None;
    GameState.RenderState.Depth = render_state_op::Less;
    GameState.RenderState.FillMode = render_state_fill_mode::Fill;
    GpuRenderStateCreate(&GameState.RenderState);

    EventSystemRegister(event_type::KeyPressed, nullptr, GameKeyPressed);
    EventSystemRegister(event_type::Resize, nullptr, GameResize);
    DevTerminalInit();

    TimerInit(&GameState.Timer);
    NoClipCameraInit(&GameState.Camera);

    ApuSourceInitFile(&GameState.Source, "assets/bgm/test.wav", true);
    ApuSourcePlay(&GameState.Source);
}

void GameUpdate()
{
    float Time = TimerGetElapsed(&GameState.Timer);
    float DT = (Time - GameState.LastFrame) / 1000.0f;
    GameState.LastFrame = Time;

    ApuSourceUpdate(&GameState.Source);

    hmm_mat4 DataToSend[] = { GameState.Camera.View, GameState.Camera.Projection };
    GpuBufferUploadData(&GameState.ConstantBuffer, DataToSend);

    GpuRenderStateBind(&GameState.RenderState);
    ShaderLibraryBind("Forward");
    GpuBufferBindVertex(&GameState.Buffer);
    GpuBufferBindConstant(&GameState.ConstantBuffer, 0, gpu_resource_bind::Vertex);
    DxRenderContextDraw(3);

    if (!GameState.TerminalFocus && !GameState.SettingsFocus)
        NoClipCameraInput(&GameState.Camera, DT);
    NoClipCameraUpdate(&GameState.Camera, DT);
    NoClipCameraUpdateFrustum(&GameState.Camera);

    GuiBeginFrame();
    if (GameState.TerminalOpen)
        DevTerminalDraw(&GameState.TerminalOpen, &GameState.TerminalFocus);
    if (GameState.SettingsOpen)
        SettingsPanelDraw(&GameState.SettingsOpen, &GameState.SettingsFocus);
    GuiEndFrame();
}

void GameExit()
{
    ApuSourceFree(&GameState.Source);
    DevTerminalShutdown();
    GpuBufferFree(&GameState.ConstantBuffer);
    GpuBufferFree(&GameState.Buffer);
    GpuRenderStateFree(&GameState.RenderState);
}
