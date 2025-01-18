module;

#include "Platform.h"

#if PLATFORM_POSIX

#include "Log.h"
#include "Types.h"
#include "assert.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

export module posix.socket;

namespace SHD::POSIX::Networking {

export constexpr u32 MTU = 1500;
export struct Packet
{
    u8 packet[MTU] = {};
    i64 packet_size = 0;

    bool isValid() { return this->packet_size != -1; }
};

export class TCPConnectionSocket
{
private:
    int m_socket = -1;

public:
    TCPConnectionSocket() = default;
    TCPConnectionSocket(int socket_fd) :
        m_socket(socket_fd) {};

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

    ~TCPConnectionSocket()
    {
        if (this->m_socket != -1) {
            this->closeConnection();
        }
    }

    Packet receive()
    {
        ASSERT(this->m_socket != -1, "Socket is not connected");
        Packet result;
        result.packet_size = ::recv(this->m_socket, (char*)result.packet, MTU, 0);
        if (result.packet_size == -1) {
            LOG_WARN("Failed to read from socket");
            this->closeConnection();
        }
        return result;
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

public:
    TCPConnectionSocket(const TCPConnectionSocket& other) = delete;
    TCPConnectionSocket& operator=(const TCPConnectionSocket& other) = delete;
};

export class TCPServerSocket
{
private:
    int m_socket = -1;

public:
    TCPServerSocket(u32 port = 8080)
    {
        this->m_socket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in address = {};
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        address.sin_addr.s_addr = INADDR_ANY;

        auto result = bind(this->m_socket, (sockaddr*)&address, sizeof(address));
        if (result < 0) {
            LOG_ERROR("Failed to bind server socket");
            return;
        }

        result = listen(this->m_socket, 1);
        if (result < 0) {
            LOG_ERROR("Failed to listen for connectios");
            return;
        }
    }

    TCPConnectionSocket listenForConnection()
    {
        const auto connection_socket = accept(this->m_socket, NULL, NULL);
        if (connection_socket < 0) {
            LOG_ERROR("Failed to accept inbound connection");
            return {};
        }

        // TODO(Tiago): add cloe_exec to socket via fcntl

        return { connection_socket };
    }

    ~TCPServerSocket()
    {
        if (this->m_socket != -1) {
            close(this->m_socket);
            this->m_socket = -1;
        }
    }
};

}

#else

export module posix.socket;

#endif
