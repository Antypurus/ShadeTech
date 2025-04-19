#include "Posix_TCP_connection_socket.h"

namespace SHD::POSIX::Networking {

TCP_connection_socket::TCP_connection_socket(int socket_fd) :
    m_socket(socket_fd) {};

TCP_connection_socket::TCP_connection_socket(TCP_connection_socket&& other) :
    m_socket(other.m_socket)
{
    other.m_socket = -1;
}

TCP_connection_socket& TCP_connection_socket::operator=(TCP_connection_socket&& other)
{
    this->~TCP_connection_socket();

    this->m_socket = other.m_socket;
    other.m_socket = -1;

    return *this;
}

TCP_connection_socket::~TCP_connection_socket()
{
    if (this->m_socket != -1) {
        this->close_connection();
    }
}

result<SHD::Networking::packet, int> TCP_connection_socket::receive()
{
    ASSERT(this->m_socket != -1, "Socket is not connected");
    SHD::Networking::packet result;
    i64 recv_result = ::recv(this->m_socket, (char*)result.packet_data, SHD::Networking::MTU, 0);
    if (recv_result == -1) {
        const int errorValue = errno;
        LOG_WARN("Failed to read from socket");
        this->close_connection();
        return error{ errorValue };
    }
    result.packet_size = (u64)recv_result;
    return { result };
}

void TCP_connection_socket::send(const u8* packet, u64 packet_size)
{
    ASSERT(this->m_socket != -1, "Socket is not connected");
    u64 total_data_sent = 0;
    while (total_data_sent < packet_size) {
        const i64 result = ::send(this->m_socket, (const char*)packet, packet_size - total_data_sent, 0);
        total_data_sent += (u64)result;
        if (result == -1) {
            LOG_WARN("Data Transfer Failed");
            this->close_connection();
            return;
        }
    }
}

void TCP_connection_socket::close_connection()
{
    ASSERT(this->m_socket != -1, "Socket is not connected");
    ::close(this->m_socket);
    this->m_socket = -1;
}

bool TCP_connection_socket::is_connected() const
{
    return this->m_socket != -1;
}

}
