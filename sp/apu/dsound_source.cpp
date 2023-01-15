/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 12/01/2023 13:29
 */

#include "dsound_source.hpp"

#include "dsound_context.hpp"
#include "systems/log_system.hpp"

void ApuSourceInitPCM(apu_source *Source, int SampleRate, int Channels, int SampleCount, short *Samples, bool Loop)
{
    Source->Samples = Samples;
    Source->SampleRate = SampleRate;
    Source->SampleCount = SampleCount;
    Source->Channels = Channels;
    Source->Looping = Loop;

    WAVEFORMATEX WaveFormat = {};
    WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
    WaveFormat.nChannels = Channels;
    WaveFormat.nSamplesPerSec = SampleRate;
    WaveFormat.wBitsPerSample = 16;
    WaveFormat.nBlockAlign = (WaveFormat.nChannels * WaveFormat.wBitsPerSample) / 8;
    WaveFormat.nAvgBytesPerSec = (WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign);
    
    DSBUFFERDESC BufferDesc = {};
    BufferDesc.dwSize = sizeof(DSBUFFERDESC);
    BufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFX;
    BufferDesc.dwBufferBytes = (DWORD)SampleCount * WaveFormat.nChannels * sizeof(short);
    BufferDesc.lpwfxFormat = &WaveFormat;
    BufferDesc.guid3DAlgorithm = GUID_NULL;

    if (BufferDesc.dwBufferBytes > DSBSIZE_MAX)
        LogError("DirectSound: Buffer of size %d is too big!", BufferDesc.dwBufferBytes);
    
    HRESULT Result = DsAudioContext.Device->CreateSoundBuffer(&BufferDesc, &Source->Buffer, nullptr);
    if (FAILED(Result))
        LogError("DirectSound: Failed to create sound buffer! %s", DsoundErrorString(Result));

    LPVOID WriteVoid;
    DWORD Length;
    Result = Source->Buffer->Lock(0, 0, &WriteVoid, &Length, nullptr, nullptr, DSBLOCK_ENTIREBUFFER);
    if (FAILED(Result))
        LogError("DirectSound: Failed to lock APU buffer! %s", DsoundErrorString(Result));
    memcpy(WriteVoid, Samples, Length);
    Source->Buffer->Unlock(WriteVoid, Length, nullptr, 0);
}

void ApuSourceInitFile(apu_source *Source, const char *File, bool Loop)
{
    // TODO(amelie.h)
}

void ApuSourceFree(apu_source *Source)
{
    SafeRelease(Source->Buffer);
    free(Source->Samples);
}

void ApuSourcePlay(apu_source *Source)
{
    HRESULT Result = Source->Buffer->Play(0, 0, Source->Looping ? DSBPLAY_LOOPING : 0);
    if (FAILED(Result))
        LogError("DirectSound: Failed to play sound source! %s", DsoundErrorString(Result));
}

void ApuSourceStop(apu_source *Source)
{
    HRESULT Result = Source->Buffer->Stop();
    if (FAILED(Result))
        LogError("DirectSound: Failed to stop sound source! %s", DsoundErrorString(Result));
}

void ApuSourcePause(apu_source *Source)
{
    if (Source->Paused)
    {
        Source->Buffer->GetCurrentPosition((LPDWORD)&Source->PauseCursor, nullptr);
        Source->Buffer->Stop();
    }
    else
    {
        Source->Buffer->SetCurrentPosition(Source->PauseCursor);
        ApuSourcePlay(Source);
    }
    Source->Paused = !Source->Paused;
}
