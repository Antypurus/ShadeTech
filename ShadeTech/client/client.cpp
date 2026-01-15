#include "core/data_containers/array.h"
#include "core/utils/log.h"
#include <core/application.h>
#include <core/networking/Socket/TCP_client_socket.h>
#include <core/networking/Socket/TCP_server_socket.h>
#include <core/rendering/platform/macos/macos_window.h>
#include <core/types.h>
#include <core/utils/atomic.h>
#include <core/utils/function.h>
#include <core/utils/move.h>
#include <core/utils/string/string.h>
#include <core/utils/string/string_view.h>

#if PLATFORM_WINDOWS
#include <core/audio/platform/windows/win_microphone.h>
#include <core/rendering/platform/windows/D3D12/D3D12.h>
#endif

#include <iostream>

class shade_tech : public SHD::application
{
public:
    int run(u32 /*argument_count*/, char** /*arguments*/) override
    {

#if 0
#if PLATFORM_MACOS
        SHD::Rendering::MacOS::Application app;
        app.set_activation_policy(SHD::Rendering::MacOS::ApplicationActivationPolicy::Regular);
        app.create_window();
        while (app.is_open()) {
            app.process_event();
        }
#endif
#endif

        SHD::DynArray<int> values;
        values.push_back(1);
        values.push_back(2);
        for (auto& val : values) {
            std::cout << val << std::endl;
        }

#if PLATFORM_WINDOWS
        // SHD::Audio::Windows::Microphone mic;

        init_d3d12();
#endif
        return 0;
    }
};

int main(int argc, char** argv)
{
    SHD::application::run_application(shade_tech{}, argc, argv);
}
