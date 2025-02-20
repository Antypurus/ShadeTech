module;

#include "Types.h"
#include "assert.h"

#include <WS2tcpip.h>
#include <Winsock2.h>

import windows.socket.TCPConnectionSocket;
export module windows.socket.TCPClientSocket;

namespace SHD::Windows::Networking {

export class TCPClientSocket : public TCPConnectionSocket
{
public:
    TCPClientSocket(const char* hostname, u16 port)
    {
        addrinfo* targetAddress = nullptr;
        addrinfo targetAddressHints = {};
        targetAddressHints.ai_flags = AI_PASSIVE;
        targetAddressHints.ai_family = AF_INET;
        targetAddressHints.ai_socktype = SOCK_STREAM;
        targetAddressHints.ai_protocol = IPPROTO_TCP;

        int result = getaddrinfo(hostname, nullptr, &targetAddressHints, &targetAddress);
        if (result != 0) {
            LOG_ERROR("Failed to get target host information");
            return;
        }

        memcpy(&targetAddressHints, targetAddress, sizeof(addrinfo));
        freeaddrinfo(targetAddress);
        targetAddress = &targetAddressHints;
        reinterpret_cast<sockaddr_in*>(targetAddress->ai_addr)->sin_port = port;

        SOCKET connection_socket =
            socket(targetAddress->ai_family, targetAddress->ai_socktype, targetAddress->ai_protocol);
        if (connection_socket == INVALID_SOCKET) {
            LOG_ERROR("Failed to initialize connection socket");
            return;
        }

        result = connect(connection_socket, targetAddress->ai_addr, (int)targetAddress->ai_addrlen);
        if (result == SOCKET_ERROR) {
            LOG_ERROR("Failed to connect client socket to server");
            return;
        }

        this->m_socket = connection_socket;
    };
};

}
