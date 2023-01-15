/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 12/01/2023 13:23
 */

#pragma once

#include "dsound_common.hpp"

#include <cstdint>

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
};

void ApuSourceInitPCM(apu_source *Source, int SampleRate, int Channels, int SampleCount, short *Samples, bool Loop);
void ApuSourceInitFile(apu_source *Source, const char *File, bool Loop);
void ApuSourceFree(apu_source *Source);
void ApuSourcePlay(apu_source *Source);
void ApuSourceStop(apu_source *Source);
void ApuSourcePause(apu_source *Source);
