#include "TCPConnectionSocket.h"

namespace SHD::Windows::Networking {

TCPConnectionSocket::TCPConnectionSocket(SOCKET socket) :
    m_socket(socket)
{
}

TCPConnectionSocket::TCPConnectionSocket(TCPConnectionSocket&& other) :
    m_socket(other.m_socket)
{
    other.m_socket = INVALID_SOCKET;
}

TCPConnectionSocket& TCPConnectionSocket::operator=(TCPConnectionSocket&& other)
{
    this->~TCPConnectionSocket();

    this->m_socket = other.m_socket;
    other.m_socket = INVALID_SOCKET;

    return *this;
}

TCPConnectionSocket::~TCPConnectionSocket()
{
    if (this->m_socket != INVALID_SOCKET) {
        this->closeConnection();
    }
}

result<SHD::Networking::Packet, int> TCPConnectionSocket::receive()
{
    ASSERT(this->m_socket != INVALID_SOCKET, "Socket is not connected");
    SHD::Networking::Packet result;
    i32 recv_result = ::recv(this->m_socket, (char*)result.packet, SHD::Networking::MTU, 0);
    if (recv_result == SOCKET_ERROR) {
        LOG_WARN("Failed to read from socket");
        this->closeConnection();
        return ErrorResult{ 5 };
    } else if (recv_result == 0) {
        LOG_WARN("Client Has Disconnected");
        this->closeConnection();
        return ErrorResult{ 5 };
    }
    result.packet_size = (u64)recv_result;
    return result;
}

void TCPConnectionSocket::send(const u8* packet, u64 packet_size)
{
    ASSERT(this->m_socket != INVALID_SOCKET, "Socket is not connected");
    u64 totalDataSend = 0;
    while (totalDataSend < packet_size) {
        u64 dataToSend = packet_size - totalDataSend;
        dataToSend = dataToSend > INT32_MAX ? INT32_MAX : dataToSend;
        const i32 result = ::send(this->m_socket, (const char*)packet, (i32)dataToSend, 0);
        totalDataSend += (u64)result;
        if (result == SOCKET_ERROR) {
            LOG_WARN("Data Transfer Failed");
            this->closeConnection();
            return;
        }
    }
}

void TCPConnectionSocket::closeConnection()
{
    ASSERT(this->m_socket != INVALID_SOCKET, "Socket is not connected");
    ::closesocket(this->m_socket);
    this->m_socket = INVALID_SOCKET;
}

bool TCPConnectionSocket::isConnected() const
{
    return this->m_socket != INVALID_SOCKET;
}

}
