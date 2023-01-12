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

    HRESULT Result = DirectSoundCreate8(nullptr, &DsAudioContext.Device, nullptr);
    if (Result != DS_OK)
        LogError("DirectSound: Failed to create device! (%s)", DsoundErrorString(Result));
    Result = DsAudioContext.Device->SetCooperativeLevel(Win32.Window, DSSCL_PRIORITY);
    if (Result != DS_OK)
        LogError("DirectSound: Failed to set cooperative level! (%s)", DsoundErrorString(Result));
    DsAudioContext.SoundCaps.dwSize = sizeof(DSCAPS);
    Result = DsAudioContext.Device->GetCaps(&DsAudioContext.SoundCaps);
    if (Result != DS_OK)
        LogError("DirectSound: Failed to get device caps! (%s)", DsoundErrorString(Result));

    WAVEFORMATEX WaveFormat = {};
    WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
    WaveFormat.nChannels = 2;
    WaveFormat.nSamplesPerSec = 48000;
    WaveFormat.nBlockAlign = 4;
    WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;
    WaveFormat.wBitsPerSample = 16;

    DSBUFFERDESC BufferDesc = {};
    BufferDesc.dwSize = sizeof(DSBUFFERDESC);
    BufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;
    BufferDesc.dwBufferBytes = 0;
    BufferDesc.lpwfxFormat = nullptr;
    BufferDesc.guid3DAlgorithm = GUID_NULL;

    Result = DsAudioContext.Device->CreateSoundBuffer(&BufferDesc, &DsAudioContext.PrimaryBuffer, nullptr);
    if (Result != DS_OK)
        LogError("DirectSound: Failed to create primary buffer! (%s)", DsoundErrorString(Result));
    DsAudioContext.PrimaryBuffer->SetFormat(&WaveFormat);
}

void ApuExit()
{
    SafeRelease(DsAudioContext.PrimaryBuffer);
    SafeRelease(DsAudioContext.Device);
}
