#include "TCPServerSocket.h"

namespace SHD::POSIX::Networking {

TCPServerSocket::TCPServerSocket(u16 port)
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

TCPServerSocket::~TCPServerSocket()
{
    if (this->m_server_socket != -1) {
        close(this->m_server_socket);
        this->m_server_socket = -1;
    }
}

TCPServerSocket::TCPServerSocket(TCPServerSocket&& other)
{
    if (this == &other)
        return;

    this->~TCPServerSocket();
    this->m_server_socket = other.m_server_socket;
    other.m_server_socket = -1;
}

TCPServerSocket& TCPServerSocket::operator=(TCPServerSocket&& other)
{
    if (this == &other)
        return *this;

    this->~TCPServerSocket();
    this->m_server_socket = other.m_server_socket;
    other.m_server_socket = -1;

    return *this;
}

result<TCPConnectionSocket, int> TCPServerSocket::listenForConnection()
{
    ASSERT(this->m_server_socket != -1, "Server socket must be initialized properly");
    const int connection_socket = accept(this->m_server_socket, NULL, NULL);
    if (connection_socket < 0) {
        LOG_ERROR("Failed to accept inbound connection");
        return ErrorResult{ errno };
    }
    fcntl(connection_socket, F_SETFD, FD_CLOEXEC);
    return TCPConnectionSocket{ connection_socket };
}

}
