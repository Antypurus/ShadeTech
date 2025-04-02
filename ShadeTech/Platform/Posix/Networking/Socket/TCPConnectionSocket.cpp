#include "TCPConnectionSocket.h"

namespace SHD::POSIX::Networking {

TCPConnectionSocket::TCPConnectionSocket(int socket_fd) :
    m_socket(socket_fd) {};

TCPConnectionSocket::TCPConnectionSocket(TCPConnectionSocket&& other) :
    m_socket(other.m_socket)
{
    other.m_socket = -1;
}

TCPConnectionSocket& TCPConnectionSocket::operator=(TCPConnectionSocket&& other)
{
    this->~TCPConnectionSocket();

    this->m_socket = other.m_socket;
    other.m_socket = -1;

    return *this;
}

TCPConnectionSocket::~TCPConnectionSocket()
{
    if (this->m_socket != -1) {
        this->closeConnection();
    }
}

result<SHD::Networking::Packet, int> TCPConnectionSocket::receive()
{
    ASSERT(this->m_socket != -1, "Socket is not connected");
    SHD::Networking::Packet result;
    i64 recv_result = ::recv(this->m_socket, (char*)result.packet, SHD::Networking::MTU, 0);
    if (recv_result == -1) {
        const int error = errno;
        LOG_WARN("Failed to read from socket");
        this->closeConnection();
        return ErrorResult{ error };
    }
    result.packet_size = (u64)recv_result;
    return { result };
}

void TCPConnectionSocket::send(const u8* packet, u64 packet_size)
{
    ASSERT(this->m_socket != -1, "Socket is not connected");
    u64 totalDataSent = 0;
    while (totalDataSent < packet_size) {
        const i64 result = ::send(this->m_socket, (const char*)packet, packet_size - totalDataSent, 0);
        totalDataSent += (u64)result;
        if (result == -1) {
            LOG_WARN("Data Transfer Failed");
            this->closeConnection();
            return;
        }
    }
}

void TCPConnectionSocket::closeConnection()
{
    ASSERT(this->m_socket != -1, "Socket is not connected");
    ::close(this->m_socket);
    this->m_socket = -1;
}

bool TCPConnectionSocket::isConnected() const
{
    return this->m_socket != -1;
}

}
