#include "TCP_connection_socket.h"

namespace SHD::Networking {

TCP_connection_socket::TCP_connection_socket(TCP_connection_socket_type connection) :
    m_connection(move(connection)) {};

TCP_connection_socket::TCP_connection_socket(TCP_connection_socket&& other)
{
    if (this == &other)
        return;

    this->m_connection = move(other.m_connection);
}

TCP_connection_socket& TCP_connection_socket::operator=(TCP_connection_socket&& other)
{
    if (this == &other)
        return *this;

    this->m_connection = move(other.m_connection);

    return *this;
}

result<SHD::Networking::packet, int> TCP_connection_socket::receive()
{
    return this->m_connection.receive();
}

void TCP_connection_socket::send(const u8* packet, u64 packet_size)
{
    this->m_connection.send(packet, packet_size);
}

void TCP_connection_socket::close_connection()
{
    this->m_connection.close_connection();
}

bool TCP_connection_socket::is_connected() const
{
    return this->m_connection.is_connected();
}

}
