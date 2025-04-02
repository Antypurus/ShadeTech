#include "Windows_TCP_server_socket.h"

namespace SHD::Windows::Networking {

TCP_server_socket::TCP_server_socket(u16 port)
{
    addrinfo* result_info = nullptr;
    addrinfo hints = {};
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int result = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result_info);
    ASSERT(result == 0, "Failed to get address info");

    SOCKET server_socket = socket(result_info->ai_family, result_info->ai_socktype, result_info->ai_protocol);
    ASSERT(server_socket != INVALID_SOCKET, "Failed to create server socket");
    LOG_SUCCESS("Server Socket Created");

    result = bind(server_socket, result_info->ai_addr, (int)result_info->ai_addrlen);
    ASSERT(result == 0, "Failed to bind socket");
    LOG_SUCCESS("Socket Bound To Address Port Combo");

    this->m_server_socket = server_socket;

    result = ::listen(this->m_server_socket, SOMAXCONN);
    ASSERT(result != SOCKET_ERROR, "failed to listen to socket");

    freeaddrinfo(result_info);
}

result<TCP_connection_socket, int> TCP_server_socket::listen_for_connection()
{
    sockaddr connection_address;
    int connection_addr_len = sizeof(sockaddr);
    SOCKET connection_socket = accept(this->m_server_socket, &connection_address, &connection_addr_len);
    if (connection_socket == INVALID_SOCKET) {
        int connection_error = WSAGetLastError();
        std::cout << connection_error << std::endl;
        return error{ connection_error };
    }
    return TCP_connection_socket{ connection_socket };
}

}
