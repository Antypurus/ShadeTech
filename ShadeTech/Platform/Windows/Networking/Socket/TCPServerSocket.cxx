module;

#include "Log.h"
#include "Types.h"
#include "assert.h"
#include "core/result.h"

#include <WS2tcpip.h>
#include <Winsock2.h>
#include <string>

import windows.socket.TCPConnectionSocket;

export module windows.socket.TCPServerSocket;

namespace SHD::Windows::Networking {

export class TCPServerSocket
{
private:
    SOCKET m_server_socket = INVALID_SOCKET;

public:
    TCPServerSocket(u16 port = 8080)
    {
        addrinfo* resultInfo = nullptr;
        addrinfo hints = {};
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_flags = AI_PASSIVE;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        int result = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &resultInfo);
        ASSERT(result == 0, "Failed to get address info");

        SOCKET server_socket = socket(resultInfo->ai_family, resultInfo->ai_socktype, resultInfo->ai_protocol);
        ASSERT(server_socket != INVALID_SOCKET, "Failed to create server socket");
        LOG_SUCCESS("Server Socket Created");

        result = bind(server_socket, resultInfo->ai_addr, (int)resultInfo->ai_addrlen);
        ASSERT(result == 0, "Failed to bind socket");
        LOG_SUCCESS("Socket Bound To Address Port Combo");

        this->m_server_socket = server_socket;

        result = ::listen(this->m_server_socket, SOMAXCONN);
        ASSERT(result != SOCKET_ERROR, "failed to listen to socket");

        freeaddrinfo(resultInfo);
    }

    result<TCPConnectionSocket, int> listenForConnection()
    {
        sockaddr connection_address;
        int connection_addr_len = sizeof(sockaddr);
        SOCKET connection_socket = accept(this->m_server_socket, &connection_address, &connection_addr_len);
        if (connection_socket == INVALID_SOCKET) {
            int error = WSAGetLastError();
            std::cout << error << std::endl;
            return ErrorResult{ error };
        }
        return TCPConnectionSocket{ connection_socket };
    }
};

}
