module;

#include "Platform.h"

#define BYTE(value, byte) ((value & (0xFFull << (byte * 8ull))) >> (byte * 8ull))

#include "Log.h"
#include "Types.h"
#include "assert.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
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
protected:
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

    virtual ~TCPConnectionSocket()
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

export class TCPClientSocket : public TCPConnectionSocket
{
public:
    TCPClientSocket() = default;
    TCPClientSocket(const char* hostname, u16 port)
    {
        u32 addr = this->resolveHostname(hostname);
        this->connect(addr, port);
    }

private:
    u32 resolveHostname(const char* hostname)
    {
        addrinfo targetAddressHints = {};
        addrinfo* targetAddress = nullptr;

        targetAddressHints.ai_family = AF_UNSPEC;
        targetAddressHints.ai_socktype = SOCK_STREAM;

        int result = getaddrinfo(hostname, nullptr, &targetAddressHints, &targetAddress);
        if (result != 0) {
            // error
            return 0;
        }

        u32 addr = reinterpret_cast<sockaddr_in*>(targetAddress->ai_addr)->sin_addr.s_addr;
        LOG_INFO("size:%d", targetAddress->ai_addrlen);

        freeaddrinfo(targetAddress);
        return addr;
    }

    void connect(u32 address, u16 port)
    {
        this->m_socket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in target = {};
        target.sin_family = AF_UNSPEC;
        target.sin_port = htons(port);
        target.sin_addr.s_addr = address;

        int result = ::connect(this->m_socket, (sockaddr*)&target, 16);
        if (result != 0) {
            LOG_ERROR("Failed to connect socket: %d", errno);
            return;
        }
    }
};

export class TCPServerSocket
{
private:
    int m_socket = -1;

public:
    TCPServerSocket(u16 port = 8080)
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

