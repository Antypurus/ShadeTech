#include "Windows_TCP_client_socket.h"

namespace SHD::Windows::Networking {

TCP_client_socket::TCP_client_socket(const SHD::Networking::IPv4_address& address, u16 port)
{
    sockaddr_in target_address = {};
    target_address.sin_family = AF_INET;
    target_address.sin_port = htons(port);
    target_address.sin_addr.s_addr = address.get_address();

    addrinfo target_address_info = {};
    target_address_info.ai_flags = AI_PASSIVE;
    target_address_info.ai_family = AF_INET;
    target_address_info.ai_socktype = SOCK_STREAM;
    target_address_info.ai_protocol = IPPROTO_TCP;
    target_address_info.ai_addrlen = sizeof(target_address);
    target_address_info.ai_addr = (sockaddr*)&target_address;

    /* TODO(Tiago): Extract this code into the hostname resolution routine for windows, besides that it will no
    longer be used here
    int result = getaddrinfo(hostname, nullptr, &targetAddressHints, &targetAddress); if (result
    != 0) { LOG_ERROR("Failed to get target host information"); return;
    }

    memcpy(&targetAddressHints, targetAddress, sizeof(addrinfo));
    freeaddrinfo(targetAddress);
    */

    SOCKET connection_socket =
        socket(target_address_info.ai_family, target_address_info.ai_socktype, target_address_info.ai_protocol);
    if (connection_socket == INVALID_SOCKET) {
        LOG_ERROR("Failed to initialize connection socket");
        return;
    }

    int result = connect(connection_socket, target_address_info.ai_addr, (int)target_address_info.ai_addrlen);
    if (result == SOCKET_ERROR) {
        LOG_ERROR("Failed to connect client socket to server");
        return;
    }

    this->m_socket = connection_socket;
};

}
