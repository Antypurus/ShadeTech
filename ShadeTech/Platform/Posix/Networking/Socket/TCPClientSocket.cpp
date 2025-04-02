#include "TCPClientSocket.h"

namespace SHD::POSIX::Networking {

TCPClientSocket::TCPClientSocket(const SHD::Networking::IPv4Address& address, u16 port)
{
    this->connect(address, port);
}

u32 TCPClientSocket::resolveHostname(const char* hostname)
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

void TCPClientSocket::connect(const SHD::Networking::IPv4Address& address, u16 port)
{
    this->m_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in target = {};
    target.sin_family = AF_UNSPEC;
    target.sin_port = htons(port);
    target.sin_addr.s_addr = address.getAddress();

    int result = ::connect(this->m_socket, (sockaddr*)&target, 16);
    if (result != 0) {
        LOG_ERROR("Failed to connect socket: %d", errno);
        return;
    }
}

}
