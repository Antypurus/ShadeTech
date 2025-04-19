#include "TCP_server_socket.h"

namespace SHD::Networking {

TCP_server_socket::TCP_server_socket(u16 port) :
    m_server_socket(port)
{
}

TCP_server_socket::TCP_server_socket(TCP_server_socket&& other) :
    m_server_socket(move(other.m_server_socket))
{
}

TCP_server_socket& TCP_server_socket::operator=(TCP_server_socket&& other)
{
    if (this == &other)
        return *this;
    this->m_server_socket = move(other.m_server_socket);
    return *this;
}

result<TCP_connection_socket, int> TCP_server_socket::listen_for_connection()
{
    auto result = this->m_server_socket.listen_for_connection();
    if (result.has_value()) {
        return { move(*result) };
    } else {
        return error{ result.get_error() };
    }
}

}
