#include "TCPClientSocket.h"

namespace SHD::Windows::Networking {

TCPClientSocket::TCPClientSocket(const SHD::Networking::IPv4Address& address, u16 port)
{
    sockaddr_in targetAddress = {};
    targetAddress.sin_family = AF_INET;
    targetAddress.sin_port = htons(port);
    targetAddress.sin_addr.s_addr = address.getAddress();

    addrinfo targetAddressInfo = {};
    targetAddressInfo.ai_flags = AI_PASSIVE;
    targetAddressInfo.ai_family = AF_INET;
    targetAddressInfo.ai_socktype = SOCK_STREAM;
    targetAddressInfo.ai_protocol = IPPROTO_TCP;
    targetAddressInfo.ai_addrlen = sizeof(targetAddress);
    targetAddressInfo.ai_addr = (sockaddr*)&targetAddress;

    /* TODO(Tiago): Extract this code into the hostname resolution routine for windows, besides that it will no
    longer be used here
    int result = getaddrinfo(hostname, nullptr, &targetAddressHints, &targetAddress); if (result
    != 0) { LOG_ERROR("Failed to get target host information"); return;
    }

    memcpy(&targetAddressHints, targetAddress, sizeof(addrinfo));
    freeaddrinfo(targetAddress);
    */

    SOCKET connection_socket =
        socket(targetAddressInfo.ai_family, targetAddressInfo.ai_socktype, targetAddressInfo.ai_protocol);
    if (connection_socket == INVALID_SOCKET) {
        LOG_ERROR("Failed to initialize connection socket");
        return;
    }

    int result = connect(connection_socket, targetAddressInfo.ai_addr, (int)targetAddressInfo.ai_addrlen);
    if (result == SOCKET_ERROR) {
        LOG_ERROR("Failed to connect client socket to server");
        return;
    }

    this->m_socket = connection_socket;
};

}
