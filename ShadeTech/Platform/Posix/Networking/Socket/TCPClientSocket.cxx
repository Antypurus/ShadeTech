module;

#include "Log.h"
#include "Types.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

import posix.socket.TCPConnectionSocket;

export module posix.socket.TCPClientSocket;

namespace SHD::POSIX::Networking {

export class TCPClientSocket : public TCPConnectionSocket
{
public:
    TCPClientSocket() = default;
    TCPClientSocket(const char* hostname, u16 port)
    {
        u32 addr = this->resolveHostname(hostname);
        this->connect(addr, port);
    }

private:
    u32 resolveHostname(const char* hostname)
    {
        addrinfo targetAddressHints = {};
        addrinfo* targetAddress = nullptr;

        targetAddressHints.ai_family = AF_UNSPEC;
        targetAddressHints.ai_socktype = SOCK_STREAM;

        int result = getaddrinfo(hostname, nullptr, &targetAddressHints, &targetAddress);
        if (result != 0) {
            // error
            return 0;
        }

        u32 addr = reinterpret_cast<sockaddr_in*>(targetAddress->ai_addr)->sin_addr.s_addr;
        LOG_INFO("size:%d", targetAddress->ai_addrlen);

        freeaddrinfo(targetAddress);
        return addr;
    }

    void connect(u32 address, u16 port)
    {
        this->m_socket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in target = {};
        target.sin_family = AF_UNSPEC;
        target.sin_port = htons(port);
        target.sin_addr.s_addr = address;

        int result = ::connect(this->m_socket, (sockaddr*)&target, 16);
        if (result != 0) {
            LOG_ERROR("Failed to connect socket: %d", errno);
            return;
        }
    }
};

}
