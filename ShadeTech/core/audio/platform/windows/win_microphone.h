#pragma once

#include <core/types.h>
#include <core/utils/ring_buffer.h>

namespace SHD::Audio::Windows {

struct audio_sample
{
    float channel_0 = 0;
    float channel_1 = 0;
    float channel_2 = 0;
    float channel_3 = 0;
    u8 channel_count = 1;
};

struct Microphone
{
private:
    ring_buffer<audio_sample> m_audio_samples;

public:
    Microphone();
};
}
