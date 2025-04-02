#include "TCPServerSocket.h"

namespace SHD::Networking {

TCPServerSocket::TCPServerSocket(u16 port) :
    m_server_socket(port)
{
}

TCPServerSocket::TCPServerSocket(TCPServerSocket&& other) :
    m_server_socket(move(other.m_server_socket))
{
}

TCPServerSocket& TCPServerSocket::operator=(TCPServerSocket&& other)
{
    if (this == &other)
        return *this;
    this->m_server_socket = move(other.m_server_socket);
    return *this;
}

result<TCPConnectionSocket, int> TCPServerSocket::listenForConnection()
{
    auto result = this->m_server_socket.listenForConnection();
    if (result.hasValue()) {
        return { move(*result) };
    } else {
        return ErrorResult{ result.getError() };
    }
}

}
