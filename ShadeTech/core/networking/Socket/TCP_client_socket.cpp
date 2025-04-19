#include "TCP_client_socket.h"

namespace SHD::Networking {

TCP_client_socket::TCP_client_socket(IPv4_address address, u16 port) :
    m_client_socket(address, port)
{
}

result<SHD::Networking::packet, int> TCP_client_socket::receive()
{
    return this->m_client_socket.receive();
}

void TCP_client_socket::send(const u8* packet, u64 packet_size)
{
    this->m_client_socket.send(packet, packet_size);
}

void TCP_client_socket::close_connection()
{
    this->m_client_socket.close_connection();
}

bool TCP_client_socket::is_connected() const
{
    return this->m_client_socket.is_connected();
}

}
