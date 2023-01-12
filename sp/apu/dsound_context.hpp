/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 12/01/2023 10:56
 */

#pragma once

#include "dsound_common.hpp"
#include "windows_data.hpp"

struct ds_audio_context
{
    IDirectSound8* Device;
    DSCAPS SoundCaps;
    IDirectSoundBuffer* PrimaryBuffer;
};

extern ds_audio_context DsAudioContext;

void ApuInit();
void ApuExit();
