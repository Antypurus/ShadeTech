module;

#include "Types.h"
#include "core/move.h"

#include <iostream>
#include <string>

import Application;
import Networking;

export module ShadeTech;

export class ShadeTech : public SHD::Application
{
public:
    int Run(u32 argument_count, char** arguments) override
    {
#if 1
        SHD::Networking::TCPServerSocket server;
        auto con = SHD::move(*server.listenForConnection());
        while (con.isConnected()) {
            auto readResult = con.receive();
            if (readResult.hasValue()) {
                auto packet = *readResult;
                std::cout << packet.packet << std::endl;
            } else {
                break;
            }
        }
#else
        SHD::Networking::TCPClientSocket client({ 192, 168, 1, 249 }, 8080);
        while (true) {
            std::string input;
            getline(std::cin, input);
            client.send((const u8*)input.c_str(), input.size());
        }
#endif
        return 0;
    }
};
