#include "Windows_TCP_connection_socket.h"

namespace SHD::Windows::Networking {

TCP_connection_socket::TCP_connection_socket(SOCKET socket) :
    m_socket(socket)
{
}

TCP_connection_socket::TCP_connection_socket(TCP_connection_socket&& other) :
    m_socket(other.m_socket)
{
    other.m_socket = INVALID_SOCKET;
}

TCP_connection_socket& TCP_connection_socket::operator=(TCP_connection_socket&& other)
{
    this->~TCP_connection_socket();

    this->m_socket = other.m_socket;
    other.m_socket = INVALID_SOCKET;

    return *this;
}

TCP_connection_socket::~TCP_connection_socket()
{
    if (this->m_socket != INVALID_SOCKET) {
        this->close_connection();
    }
}

result<SHD::Networking::packet, int> TCP_connection_socket::receive()
{
    ASSERT(this->m_socket != INVALID_SOCKET, "Socket is not connected");
    SHD::Networking::packet result;
    i32 recv_result = ::recv(this->m_socket, (char*)result.packet, SHD::Networking::MTU, 0);
    if (recv_result == SOCKET_ERROR) {
        LOG_WARN("Failed to read from socket");
        this->close_connection();
        return error{ 5 };
    } else if (recv_result == 0) {
        LOG_WARN("Client Has Disconnected");
        this->close_connection();
        return error{ 5 };
    }
    result.packet_size = (u64)recv_result;
    return result;
}

void TCP_connection_socket::send(const u8* packet, u64 packet_size)
{
    ASSERT(this->m_socket != INVALID_SOCKET, "Socket is not connected");
    u64 total_data_sent = 0;
    while (total_data_sent < packet_size) {
        u64 data_to_send = packet_size - total_data_sent;
        data_to_send = data_to_send > INT32_MAX ? INT32_MAX : data_to_send;
        const i32 result = ::send(this->m_socket, (const char*)packet, (i32)data_to_send, 0);
        total_data_sent += (u64)result;
        if (result == SOCKET_ERROR) {
            LOG_WARN("Data Transfer Failed");
            this->close_connection();
            return;
        }
    }
}

void TCP_connection_socket::close_connection()
{
    ASSERT(this->m_socket != INVALID_SOCKET, "Socket is not connected");
    ::closesocket(this->m_socket);
    this->m_socket = INVALID_SOCKET;
}

bool TCP_connection_socket::is_connected() const
{
    return this->m_socket != INVALID_SOCKET;
}

}
