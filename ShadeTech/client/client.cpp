#include "core/utils/log.h"
#include <core/application.h>
#include <core/networking/Socket/TCP_client_socket.h>
#include <core/networking/Socket/TCP_server_socket.h>
#include <core/rendering/platform/macos/macos_window.h>
#include <core/types.h>
#include <core/utils/move.h>
#include <core/utils/string/string.h>

#include <core/audio/platform/windows/win_microphone.h>

#include <iostream>

class shade_tech : public SHD::application
{
public:
    int run(u32 /*argument_count*/, char** /*arguments*/) override
    {
        SHD::string str1("hello");
        SHD::string str2(" stuff");
        std::cout << str1.str << std::endl;
        std::cout << (str1 + " stuff").str << std::endl;

#if PLATFORM_MACOS
        SHD::Rendering::MacOS::Application app;
        app.set_activation_policy(SHD::Rendering::MacOS::ApplicationActivationPolicy::Regular);
#endif

#if PLATFORM_WINDOWS
        // SHD::Audio::Windows::Microphone mic;
#endif
        return 0;
    }
};

int main(int argc, char** argv)
{
    SHD::application::run_application(shade_tech{}, argc, argv);
}
