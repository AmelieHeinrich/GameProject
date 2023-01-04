/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 04/01/2023 22:48
 */

#pragma once

#include <cstdint>

enum class event_type : uint16_t
{
    Resize,
    GamepadConnect,
    GamepadDisconnect,
    KeyPressed,
    KeyReleased
};

struct event_data
{
    union {
        int64_t i64[2];
        uint64_t u64[2];
        double f64[2];
        int32_t i32[4];
        uint32_t u32[4];
        float f32[4];
        int16_t i16[8];
        uint16_t u16[8];
        int8_t i8[16];
        uint8_t u8[16];
        char c[16];
    } data;
};

typedef bool (*PFN_OnEvent)(event_type Type, void *Sender, void *ListenerInstance, event_data Data);

void EventSystemInit();
void EventSystemExit();
void EventSystemRegister(event_type Type, void *Listener, PFN_OnEvent Callback);
void EventSystemUnregister(event_type Type, void *Listener, PFN_OnEvent Callback);
bool EventSystemFire(event_type Type, void *Sender, event_data Data);
