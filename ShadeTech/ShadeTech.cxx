module;

#include <iostream>
#include <string>

#include "Platform.h"
#include "Types.h"

import posix.socket;
import windows.networking;
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

#if 1
        TCPServerSocket server;
        auto con = server.listenForConnection();
        while (true) {
            auto packet = con.receive();
            std::cout << packet.packet << std::endl;
        }
#else
        TCPClientSocket client("192.168.1.237", 8080);
        while (true) {
            std::string input;
            getline(std::cin, input);
            client.send((const u8*)input.c_str(), input.size());
        }
#endif
        return 0;
    }
};
