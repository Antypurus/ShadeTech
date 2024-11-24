module;

#include "Platform.h"
#if PLATFORM_WINDOWS

#include "assert.h"

#include <WS2tcpip.h>
#include <winsock2.h>

export module socket;
namespace SHD {
namespace Windows {

export class TCPSocket
{
private:
public:
    TCPSocket()
    {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        ASSERT(result == 0, "Failed to initialize Window Networking (Winsock2)");
        LOG_SUCCESS("Windows Networking Initialized");

        addrinfo* resultInfo;
        addrinfo hints;
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_flags = AI_PASSIVE;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        result = getaddrinfo(NULL, "8080", &hints, &resultInfo);
        ASSERT(result == 0, "Failed to get address info");

        SOCKET server_socket = socket(resultInfo->ai_family, resultInfo->ai_socktype, resultInfo->ai_protocol);
        ASSERT(server_socket != INVALID_SOCKET, "Failed to create server socket");
        LOG_SUCCESS("Server Socket Created");
    }
};

}
}

#else

export module socket;

namespace SHD {
namespace Windows {
export class TCPSocket
{};
}
}

#endif
