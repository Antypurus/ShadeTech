#include "Networking/Socket/TCP_client_socket.h"
#include "Networking/Socket/TCP_server_socket.h"
#include "application.h"
#include "core/move.h"
#include "core/string/string.h"
#include "types.h"

#include <iostream>
#include <string>

class shade_tech : public SHD::application
{
public:
    int run(u32 argument_count, char** arguments) override
    {
        SHD::string str1("hello");
        SHD::string str2(" stuff");
        std::cout << str1.str << std::endl;
        std::cout << (str1 + " stuff").str << std::endl;

#if 0
        SHD::Networking::TCP_server_socket server;
        auto con = SHD::move(*server.listen_for_connection());
        while (con.is_connected()) {
            auto readResult = con.receive();
            if (readResult.has_value()) {
                auto packet = *readResult;
                std::cout << packet.packet << std::endl;
            } else {
                break;
            }
        }
#endif
#if 0
        SHD::Networking::TCP_client_socket client({ 192, 168, 1, 249 }, 8080);
        while (true) {
            std::string input;
            getline(std::cin, input);
            client.send((const u8*)input.c_str(), input.size());
        }
#endif
        return 0;
    }
};
