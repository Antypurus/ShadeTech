#include "Application.h"
#include "Networking/Socket/TCPClientSocket.h"
#include "Networking/Socket/TCPServerSocket.h"
#include "Types.h"
#include "core/move.h"
#include "core/string/string.h"

#include <iostream>
#include <string>

class ShadeTech : public SHD::Application
{
public:
    int Run(u32 argument_count, char** arguments) override
    {
        SHD::string str1 = "hello";
        SHD::string str2 = " stuff";
        std::cout << str1.str << str2.str << std::endl;
        std::cout << (str1 + " stuff").str << std::endl;

#if 0
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
#endif
#if 0
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
