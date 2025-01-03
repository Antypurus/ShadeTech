module;

#include "Platform.h"
#if PLATFORM_WINDOWS

#include "Types.h"
#include "assert.h"

#include <WS2tcpip.h>
#include <winsock2.h>

export module windows.socket;

namespace SHD::Windows::Networking {

export class TCPServerSocket
{
private:
    SOCKET m_socket = INVALID_SOCKET;

public:
    TCPServerSocket(u32 port = 8080)
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

        result = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &resultInfo);
        ASSERT(result == 0, "Failed to get address info");

        SOCKET server_socket = socket(resultInfo->ai_family, resultInfo->ai_socktype, resultInfo->ai_protocol);
        ASSERT(server_socket != INVALID_SOCKET, "Failed to create server socket");
        LOG_SUCCESS("Server Socket Created");

        result = bind(server_socket, resultInfo->ai_addr, (int)resultInfo->ai_addrlen);
        ASSERT(result == 0, "Failed to bind socket");
        LOG_SUCCESS("Socket Bound To Address Port Combo");

        this->m_socket = server_socket;
    }

    void listen()
    {
        int result = ::listen(this->m_socket, SOMAXCONN);
        ASSERT(result != SOCKET_ERROR, "failed to listen to socket");

        sockaddr connection_address;
        int connection_addr_len = sizeof(sockaddr);
        SOCKET connection_socket = accept(this->m_socket, &connection_address, &connection_addr_len);
        int error = WSAGetLastError();
        std::cout << error << std::endl;
        ASSERT(connection_socket != INVALID_SOCKET, "Failed to Accept client connection");

        LOG_INFO("Got Connection, listening for data");
        do {
            constexpr u32 MTU = 1500;
            char buffer[MTU];

            result = recv(connection_socket, buffer, MTU, 0);
            if (result > 0) {
                std::cout << std::hex << buffer << std::endl;
            }
        } while (result > 0);
    }
};

}

#else

export module windows.socket;

#endif
