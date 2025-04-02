#include "TCPClientSocket.h"

namespace SHD::Networking {

TCPClientSocket::TCPClientSocket(IPv4Address address, u16 port) :
    m_client_socket(address, port)
{
}

result<SHD::Networking::Packet, int> TCPClientSocket::receive()
{
    return this->m_client_socket.receive();
}

void TCPClientSocket::send(const u8* packet, u64 packet_size)
{
    this->m_client_socket.send(packet, packet_size);
}

void TCPClientSocket::closeConnection()
{
    this->m_client_socket.closeConnection();
}

bool TCPClientSocket::isConnected() const
{
    return this->m_client_socket.isConnected();
}

}
