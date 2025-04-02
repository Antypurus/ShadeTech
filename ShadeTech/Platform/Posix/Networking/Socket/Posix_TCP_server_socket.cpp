#include "Posix_TCP_server_socket.h"

namespace SHD::POSIX::Networking {

TCP_server_socket::TCP_server_socket(u16 port)
{
    this->m_server_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    auto result = bind(this->m_server_socket, (sockaddr*)&address, sizeof(address));
    if (result < 0) {
        LOG_ERROR("Failed to bind server socket");
        return;
    }

    result = listen(this->m_server_socket, 1);
    if (result < 0) {
        LOG_ERROR("Failed to listen for connectios");
        return;
    }
}

TCP_server_socket::~TCP_server_socket()
{
    if (this->m_server_socket != -1) {
        close(this->m_server_socket);
        this->m_server_socket = -1;
    }
}

TCP_server_socket::TCP_server_socket(TCP_server_socket&& other)
{
    if (this == &other)
        return;

    this->~TCP_server_socket();
    this->m_server_socket = other.m_server_socket;
    other.m_server_socket = -1;
}

TCP_server_socket& TCP_server_socket::operator=(TCP_server_socket&& other)
{
    if (this == &other)
        return *this;

    this->~TCP_server_socket();
    this->m_server_socket = other.m_server_socket;
    other.m_server_socket = -1;

    return *this;
}

result<TCP_connection_socket, int> TCP_server_socket::listen_for_connection()
{
    ASSERT(this->m_server_socket != -1, "Server socket must be initialized properly");
    const int connection_socket = accept(this->m_server_socket, NULL, NULL);
    if (connection_socket < 0) {
        LOG_ERROR("Failed to accept inbound connection");
        return error{ errno };
    }
    fcntl(connection_socket, F_SETFD, FD_CLOEXEC);
    return TCP_connection_socket{ connection_socket };
}

}
