#include "win_microphone.h"

#include <core/types.h>
#include <core/utils/log.h>
#include <core/utils/ring_buffer.h>

#include <Windows.h>
#include <audioclient.h>
#include <avrt.h>
#include <comdef.h>
#include <functiondiscoverykeys.h>
#include <mmdeviceapi.h>
#include <synchapi.h>
#include <winbase.h>
#include <wrl.h>

#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace SHD::Audio::Windows {

using namespace Microsoft::WRL;

#define REFTIMES_PER_SEC 10000000
#define REFTIMES_PER_MILLISEC 10000

#define CAT_(x, y) x##y
#define CAT(x, y) CAT_(x, y)

#define COM_CALL(function, message)                                                                                    \
    HRESULT CAT(result_code, __LINE__) = function;                                                                     \
    if (CAT(result_code, __LINE__) != S_OK) {                                                                          \
        LOG_ERROR(message " Error:{}", CAT(result_code, __LINE__));                                                 \
    }


Microphone::Microphone():
 m_audio_samples(5)
{
    LOG_INFO("Initializng mic");
    std::vector<float> frames;

    ComPtr<IMMDeviceEnumerator> enumerator;
    COM_CALL(CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, IID_PPV_ARGS(&enumerator)),
             "Failed to create audio device enumerator");

    {
        {
            ComPtr<IMMDeviceCollection> device_list;
            enumerator->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE, &device_list);
            u32 device_count = 0;
            device_list->GetCount(&device_count);

            //_setmode(_fileno(stdout), _O_U16TEXT);
            for (u32 i = 0; i < device_count; ++i) {
                // get device
                ComPtr<IMMDevice> device;
                device_list->Item(i, &device);

                // get device properties
                ComPtr<IPropertyStore> properties;
                device->OpenPropertyStore(STGM_READ, &properties);

                // get device name
                PROPVARIANT name;
                PropVariantInit(&name);
                properties->GetValue(PKEY_Device_FriendlyName, &name);
                wprintf(L"Device %d:%ls\n", i, name.pwszVal);
                PropVariantClear(&name);
            }
        }

        // create audio capture device
        ComPtr<IMMDevice> capture_device;
        COM_CALL(enumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &capture_device),
                 "Failed to get default capture audio endpoint");

        // create an audio client
        ComPtr<IAudioClient> audio_client;
        COM_CALL(capture_device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (void**)&audio_client),
                 "Failed to create audio client for capture device");

        // get audio mix format
        WAVEFORMATEX* mix_format = nullptr;
        COM_CALL(audio_client->GetMixFormat(&mix_format), "Failed to get device audio mix format");

        // initialize audio client
        COM_CALL(audio_client->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, REFTIMES_PER_SEC, 0, mix_format, nullptr),
                 "Failed to initialize device audio client");

        // get audio capture client
        ComPtr<IAudioCaptureClient> capture_client;
        COM_CALL(audio_client->GetService(__uuidof(IAudioCaptureClient), (void**)&capture_client),
                 "Failed to get audio capture client from audio client");

        u32 audio_frame_buffer_size = 0;
        COM_CALL(audio_client->GetBufferSize(&audio_frame_buffer_size), "Failed to get audio frame size");

        u32 audio_frame_duration =
            (u32)((double)REFTIMES_PER_SEC * audio_frame_buffer_size / mix_format->nSamplesPerSec);

        std::cout << "Audio Frame buffer size:" << audio_frame_buffer_size << std::endl;
        std::cout << "Samples per second:" << mix_format->nSamplesPerSec << std::endl;
        std::cout << "Audio Bit-depth:" << mix_format->wBitsPerSample << std::endl;
        std::cout << "channel count:" << mix_format->nChannels << std::endl;

        COM_CALL(audio_client->Start(), "Audio client recording start failed");
        volatile bool continue_recoding = true;
        std::thread recording_thread([&]() {
            while (continue_recoding) {
                u32 frames_in_packet = 0;
                COM_CALL(capture_client->GetNextPacketSize(&frames_in_packet), "Recording Failed");

                std::cout << "Size:" << frames_in_packet << std::endl;

                while (frames_in_packet != 0) {
                    u8* frame_data = nullptr;
                    u32 frame_count = 0;
                    u32 flags = 0;
                    COM_CALL(capture_client->GetBuffer(&frame_data, &frame_count, (DWORD*)&flags, nullptr, nullptr),
                             "Recording Failed");

                    for (usize i = 0; i < frame_count; ++i) {
                        float sample = ((float*)frame_data)[i];
                        frames.push_back(sample);
                    }

                    capture_client->ReleaseBuffer(frame_count);

                    COM_CALL(capture_client->GetNextPacketSize(&frames_in_packet), "Recording Failed");
                }

                Sleep(audio_frame_duration / REFTIMES_PER_MILLISEC / 2);
            }
        });

        system("pause");

        continue_recoding = false;
        recording_thread.join();
        audio_client->Stop();
        LOG_INFO("Recording Stopped");

        LOG_INFO("Final sample count: {}", frames.size());
    }

    /*
        ====== Audio Playback ======
    */
    {
        // get playback device
        ComPtr<IMMDevice> playback_device;
        COM_CALL(enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &playback_device),
                 "Failed to get default capture audio endpoint");

        // get/create audio client
        ComPtr<IAudioClient> audio_client;
        COM_CALL(playback_device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, &audio_client),
                 "Failed to get audio client for playback device");

        // get audio mix format
        WAVEFORMATEX* mix_format = nullptr;
        COM_CALL(audio_client->GetMixFormat(&mix_format), "Failed to get device audio mix format");
        std::cout << "Playback format" << std::endl;
        std::cout << "Sample rate:" << mix_format->nSamplesPerSec << std::endl;
        std::cout << "Bits per sample:" << mix_format->wBitsPerSample << std::endl;
        std::cout << "Channel count:" << mix_format->nChannels << std::endl;

        // Create event for buffer readiness
        HANDLE bufferEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

        // init audio client
        COM_CALL(
            audio_client->Initialize(
                AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, REFTIMES_PER_SEC, 0, mix_format, nullptr),
            "Failed to initialize playback device audio client");

        COM_CALL(audio_client->SetEventHandle(bufferEvent),
                 "failed to set event handle for when audio engine is ready to accept more data");

        u32 buffer_sample_count = 0;
        COM_CALL(audio_client->GetBufferSize(&buffer_sample_count), "Failed to get number of sampler per frame");

        ComPtr<IAudioRenderClient> audio_playback_client;
        COM_CALL(audio_client->GetService(IID_PPV_ARGS(&audio_playback_client)), "Failed to get audio playback cilent");

        u8* buffer = nullptr;
        float* audio_buffer = nullptr;
        usize source_iterator = 0;

        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
        COM_CALL(audio_client->Start(), "Playback start failed");
        while (source_iterator <= frames.size()) {
            WaitForSingleObject(bufferEvent, INFINITE);

            u32 padding = 0;
            COM_CALL(audio_client->GetCurrentPadding(&padding), "Failed to get padding");

            u32 to_copy = buffer_sample_count - padding;
            if (to_copy <= 0) {
                continue;
            }

            COM_CALL(audio_playback_client->GetBuffer(to_copy, &buffer), "Failed to acquire audio playback buffer");

            audio_buffer = (float*)buffer;
            for (usize i = 0; i < to_copy; ++i) {
                usize idx = source_iterator + i;
                if (idx > frames.size() - 1)
                    break;
                audio_buffer[i * 2] = frames[idx];
                audio_buffer[i * 2 + 1] = frames[idx];
            }
            source_iterator += to_copy;

            COM_CALL(audio_playback_client->ReleaseBuffer(to_copy, 0), "failed to release audio buffer");
        }

        u32 padding = 0;
        COM_CALL(audio_client->GetCurrentPadding(&padding), "Failed to get padding");
        while (padding != 0) {
            COM_CALL(audio_client->GetCurrentPadding(&padding), "Failed to get padding");
            Sleep(10);
        }

        COM_CALL(audio_client->Stop(), "failed to stop audio playback");
    }
}

}
