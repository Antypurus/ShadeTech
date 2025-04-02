#include "TCPConnectionSocket.h"

#include "Networking/Packet.h"

namespace SHD::Networking {

TCPConnectionSocket::TCPConnectionSocket(TCPConnectionSocketType connection) :
    m_connection(move(connection)) {};

TCPConnectionSocket::TCPConnectionSocket(TCPConnectionSocket&& other)
{
    if (this == &other)
        return;

    this->m_connection = move(other.m_connection);
}

TCPConnectionSocket& TCPConnectionSocket::operator=(TCPConnectionSocket&& other)
{
    if (this == &other)
        return *this;

    this->m_connection = move(other.m_connection);

    return *this;
}

result<SHD::Networking::Packet, int> TCPConnectionSocket::receive()
{
    return this->m_connection.receive();
}
void TCPConnectionSocket::send(const u8* packet, u64 packet_size)
{
    this->m_connection.send(packet, packet_size);
}
void TCPConnectionSocket::closeConnection()
{
    this->m_connection.closeConnection();
}
bool TCPConnectionSocket::isConnected() const
{
    return this->m_connection.isConnected();
}

}
