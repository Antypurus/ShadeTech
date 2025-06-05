#include "Posix_TCP_client_socket.h"

namespace SHD::POSIX::Networking {

TCP_client_socket::TCP_client_socket(const SHD::Networking::IPv4_address& address, u16 port)
{
    this->connect(address, port);
}

u32 TCP_client_socket::resolve_hostname(const char* hostname)
{
    addrinfo target_address_hints = {};
    addrinfo* target_address = nullptr;

    target_address_hints.ai_family = AF_UNSPEC;
    target_address_hints.ai_socktype = SOCK_STREAM;

    int result = getaddrinfo(hostname, nullptr, &target_address_hints, &target_address);
    if (result != 0) {
        // error
        return 0;
    }

    u32 addr = reinterpret_cast<sockaddr_in*>(target_address->ai_addr)->sin_addr.s_addr;
    LOG_INFO("size:{}", target_address->ai_addrlen);

    freeaddrinfo(target_address);
    return addr;
}

void TCP_client_socket::connect(const SHD::Networking::IPv4_address& address, u16 port)
{
    this->m_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in target = {};
    target.sin_family = AF_UNSPEC;
    target.sin_port = htons(port);
    target.sin_addr.s_addr = address.get_address();

    int result = ::connect(this->m_socket, (sockaddr*)&target, 16);
    if (result != 0) {
        LOG_ERROR("Failed to connect socket: {}", errno);
        return;
    }
}

}
