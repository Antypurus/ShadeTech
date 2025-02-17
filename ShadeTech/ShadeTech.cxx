module;

#include "Types.h"

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

        // TCPConnectionSocket con = std::move(*server.listenForConnection());
        // while (con.isConnected()) {
        //     auto readResult = con.receive();
        //     if (readResult.hasValue()) {
        //         auto packet = *readResult;
        //         std::cout << packet.packet << std::endl;
        //     } else {
        //         break;
        //     }
        // }
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
