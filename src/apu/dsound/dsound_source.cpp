/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 12/01/2023 13:29
 */

#include "apu/apu_source.hpp"

#include "dsound_context.hpp"
#include "game_data.hpp"
#include "systems/log_system.hpp"

#include <dr_libs/dr_wav.h>

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
    BufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFX | DSBCAPS_GLOBALFOCUS;
    BufferDesc.dwBufferBytes = (DWORD)SampleCount * WaveFormat.nChannels * sizeof(short);
    BufferDesc.lpwfxFormat = &WaveFormat;
    BufferDesc.guid3DAlgorithm = GUID_NULL;

    if (BufferDesc.dwBufferBytes > DSBSIZE_MAX)
        LogError("DirectSound: Handle of size %d is too big!", BufferDesc.dwBufferBytes);

    HRESULT Result = DsAudioContext.Device->CreateSoundBuffer(&BufferDesc, (IDirectSoundBuffer**)(&Source->Handle), nullptr);
    if (FAILED(Result))
        LogError("DirectSound: Failed to create sound buffer! %s", DsoundErrorString(Result));

    IDirectSoundBuffer *Handle = (IDirectSoundBuffer*)(Source->Handle);

    LPVOID WriteVoid;
    DWORD Length;
    Result = Handle->Lock(0, 0, &WriteVoid, &Length, nullptr, nullptr, DSBLOCK_ENTIREBUFFER);
    if (FAILED(Result))
        LogError("DirectSound: Failed to lock APU buffer! %s", DsoundErrorString(Result));
    memcpy(WriteVoid, Samples, SampleCount * sizeof(short) * Channels);
    Handle->Unlock(WriteVoid, Length, nullptr, 0);

    delete Source->Samples;

}

void ApuSourceInitFile(apu_source *Source, const char *File, bool Loop)
{
    drwav Wave;
    if (!drwav_init_file(&Wave, File, nullptr))
        LogError("Failed to open sound file %s!", File);

    int SampleRate = Wave.sampleRate;
    int SampleCount = Wave.totalPCMFrameCount;
    int Channels = Wave.channels;
    short *Samples = reinterpret_cast<short*>(malloc(SampleCount * sizeof(short) * Wave.channels));

    int ReadSamples = drwav_read_pcm_frames_s16(&Wave, SampleCount, Samples);
    if (ReadSamples != SampleCount)
        LogWarn(".wav loader loaded %d/%d PCM frames!", ReadSamples, SampleCount);
    ApuSourceInitPCM(Source, SampleRate, Channels, SampleCount, Samples, false);

    drwav_uninit(&Wave);
}

void ApuSourceFree(apu_source *Source)
{
    IDirectSoundBuffer *Buffer = (IDirectSoundBuffer*)(Source->Handle);
    SafeRelease(Buffer);
}

void ApuSourcePlay(apu_source *Source)
{
    IDirectSoundBuffer *Buffer = (IDirectSoundBuffer*)(Source->Handle);
    HRESULT Result = Buffer->Play(0, 0, Source->Looping ? DSBPLAY_LOOPING : 0);
    if (FAILED(Result))
        LogError("DirectSound: Failed to play sound source! %s", DsoundErrorString(Result));
}

void ApuSourceUpdate(apu_source *Source)
{
    IDirectSoundBuffer *Buffer = (IDirectSoundBuffer*)(Source->Handle);
    float VolumeToSet = 1.0f;

    switch (Source->Type)
    {
        case apu_source_type::Music:
        {
            VolumeToSet = EgcF32(EgcFile, "music_volume");
        } break;
        case apu_source_type::Sound:
        {
            VolumeToSet = EgcF32(EgcFile, "sound_volume");
        } break;
        case apu_source_type::Voice:
        {
            VolumeToSet = EgcF32(EgcFile, "voice_volume");
        } break;
    }
    int Range = -6000;
    VolumeToSet = (Range + VolumeToSet * 6000);

    Buffer->SetVolume(VolumeToSet);
}

void ApuSourceStop(apu_source *Source)
{
    IDirectSoundBuffer *Buffer = (IDirectSoundBuffer*)(Source->Handle);
    HRESULT Result = Buffer->Stop();
    if (FAILED(Result))
        LogError("DirectSound: Failed to stop sound source! %s", DsoundErrorString(Result));
}

void ApuSourcePause(apu_source *Source)
{
    IDirectSoundBuffer *Buffer = (IDirectSoundBuffer*)(Source->Handle);
    if (Source->Paused)
    {
        Buffer->GetCurrentPosition((LPDWORD)&Source->PauseCursor, nullptr);
        Buffer->Stop();
    }
    else
    {
        Buffer->SetCurrentPosition(Source->PauseCursor);
        ApuSourcePlay(Source);
    }
    Source->Paused = !Source->Paused;
}

void ApuSourceSetLoop(apu_source *Source, bool Loop)
{
    Source->Looping = Loop;
}

void ApuSourceSetType(apu_source *Source, apu_source_type Type)
{
    Source->Type = Type;
}
