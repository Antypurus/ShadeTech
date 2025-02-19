module;

#include "Log.h"
#include "Types.h"
#include "assert.h"
#include "core/result.h"

#include <sys/socket.h>
#include <unistd.h>

import networking.packet;

export module posix.socket.TCPConnectionSocket;

namespace SHD::POSIX::Networking {

export class TCPConnectionSocket
{
protected:
    int m_socket = -1;

public:
    TCPConnectionSocket() = default;
    TCPConnectionSocket(int socket_fd) :
        m_socket(socket_fd){};

    TCPConnectionSocket(TCPConnectionSocket&& other) :
        m_socket(other.m_socket)
    {
        other.m_socket = -1;
    }

    TCPConnectionSocket& operator=(TCPConnectionSocket&& other)
    {
        this->~TCPConnectionSocket();

        this->m_socket = other.m_socket;
        other.m_socket = -1;

        return *this;
    }

    virtual ~TCPConnectionSocket()
    {
        if (this->m_socket != -1) {
            this->closeConnection();
        }
    }

    result<SHD::Networking::Packet, int> receive()
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

    void send(const u8* packet, u64 packet_size)
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

    void closeConnection()
    {
        ASSERT(this->m_socket != -1, "Socket is not connected");
        ::close(this->m_socket);
        this->m_socket = -1;
    }

    bool isConnected() const { return this->m_socket != -1; }

public:
    TCPConnectionSocket(const TCPConnectionSocket& other) = delete;
    TCPConnectionSocket& operator=(const TCPConnectionSocket& other) = delete;
};

}
