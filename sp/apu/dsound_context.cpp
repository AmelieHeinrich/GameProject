/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 12/01/2023 10:57
 */

#include "dsound_context.hpp"

#include "systems/log_system.hpp"

ds_audio_context DsAudioContext;

void ApuInit()
{
    // TODO(amelie.h): Choose audio device

    HRESULT Result = DirectSoundCreate8(NULL, &DsAudioContext.Device, NULL);
    if (Result != DS_OK)
        LogError("DirectSound: Failed to create device! (%s)", DsoundErrorString(Result));
    Result = DsAudioContext.Device->SetCooperativeLevel(Win32.Window, DSSCL_PRIORITY);
    if (Result != DS_OK)
        LogError("DirectSound: Failed to set cooperative level! (%s)", DsoundErrorString(Result));
    DsAudioContext.SoundCaps.dwSize = sizeof(DSCAPS);
    Result = DsAudioContext.Device->GetCaps(&DsAudioContext.SoundCaps);
    if (Result != DS_OK)
        LogError("DirectSound: Failed to get device caps! (%s)", DsoundErrorString(Result));
}

void ApuExit()
{
    SafeRelease(DsAudioContext.Device);
}
