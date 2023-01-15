/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 12/01/2023 13:23
 */

#pragma once

#include "dsound_common.hpp"

#include <cstdint>

enum class apu_source_type
{
    Music,
    Sound,
    Voice
};

struct apu_source
{
    IDirectSoundBuffer* Buffer;

    int SampleRate;
    int Channels;
    int SampleCount;
    short* Samples;
    bool Looping;
    bool Paused;
    uint32_t PauseCursor;

    apu_source_type Type;
};

void ApuSourceInitPCM(apu_source *Source, int SampleRate, int Channels, int SampleCount, short *Samples, bool Loop);
void ApuSourceInitFile(apu_source *Source, const char *File, bool Loop);
void ApuSourceFree(apu_source *Source);
void ApuSourcePlay(apu_source *Source);
void ApuSourceStop(apu_source *Source);
void ApuSourceUpdate(apu_source *Source);
void ApuSourcePause(apu_source *Source);
void ApuSourceSetLoop(apu_source *Source, bool Loop);
void ApuSourceSetType(apu_source *Source, apu_source_type Type);
