/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 11/01/2023 12:33
 */

#include "game.hpp"

#include "gpu/dx11_buffer.hpp"
#include "gpu/dx11_context.hpp"
#include "gpu/dx11_render_state.hpp"
#include "gpu/dx11_shader.hpp"
#include "gui/dev_terminal.hpp"
#include "gui/gui.hpp"
#include "systems/event_system.hpp"
#include "systems/input_system.hpp"
#include "systems/log_system.hpp"

#include <ImGui/imgui.h>

struct game_state
{
    bool TerminalOpen;

    gpu_shader Shader;
    gpu_buffer Buffer;
    gpu_render_state RenderState;
};

game_state GameState;

bool GameKeyPressed(event_type Type, void *Sender, void *Listener, event_data Data)
{
    if (Type == event_type::KeyPressed)
    {
        if (Data.data.u16[0] == (uint16_t)keyboard_key::F1)
            GameState.TerminalOpen = !GameState.TerminalOpen;
    }
    return true;
}

void GameInit()
{
    GameState.TerminalOpen = false;

    float Data[] = 
    {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    GpuShaderInit(&GameState.Shader, "assets/shaders/forward/Vertex.hlsl", "assets/shaders/forward/Pixel.hlsl");

    GpuBufferCreate(&GameState.Buffer, sizeof(Data), sizeof(float) * 6, gpu_buffer_usage::Vertex);
    GpuBufferUploadData(&GameState.Buffer, Data);

    GameState.RenderState.CounterClockwise = false;
    GameState.RenderState.CullMode = render_state_cull_mode::None;
    GameState.RenderState.Depth = render_state_op::Less;
    GameState.RenderState.FillMode = render_state_fill_mode::Fill;
    GpuRenderStateCreate(&GameState.RenderState);

    EventSystemRegister(event_type::KeyPressed, nullptr, GameKeyPressed);
    DevTerminalInit();
}

void GameUpdate()
{
    GpuRenderStateBind(&GameState.RenderState);
    GpuShaderBind(&GameState.Shader);
    GpuBufferBindVertex(&GameState.Buffer);
    DxRenderContextDraw(3);

    GuiBeginFrame();

    bool Focused = false;
    if (GameState.TerminalOpen)
        DevTerminalDraw(&GameState.TerminalOpen, &Focused);
    
    GuiEndFrame();
}

void GameExit()
{
    DevTerminalShutdown();
    GpuBufferFree(&GameState.Buffer);
    GpuRenderStateFree(&GameState.RenderState);
    GpuShaderFree(&GameState.Shader);
}
