module;

#include <iostream>

#include "Platform.h"
#include "Types.h"

import posix.socket;
import windows.socket;
import Application;

export module ShadeTech;

export class ShadeTech : public SHD::Application
{
public:
    int Run(u32 argument_count, char** arguments) override
    {
#if PLATFORM_WINDOWS
        using namespace SHD::Windows::Networking;
#else
        using namespace SHD::POSIX::Networking;
#endif
        TCPServerSocket server;
        auto con = server.listenForConnection();
        auto packet = con.receive();
        std::cout << packet.packet << std::endl;
        return 0;
    }
};
