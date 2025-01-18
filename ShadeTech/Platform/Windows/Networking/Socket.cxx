module;

#include "Platform.h"
#if PLATFORM_WINDOWS

#include "Types.h"
#include "assert.h"

#include <WS2tcpip.h>
#include <winsock2.h>

export module windows.socket;

namespace SHD::Windows::Networking {

export constexpr u32 MTU = 1500;
export struct Packet
{
    u8 packet[MTU] = {};
    i64 packet_size = 0;

    bool isValid() { return !(this->packet_size == SOCKET_ERROR); }
};

export class TCPConnectionSocket
{
private:
    SOCKET m_socket = INVALID_SOCKET;

public:
    TCPConnectionSocket() = default;
    TCPConnectionSocket(SOCKET socket) :
        m_socket(socket)
    {
    }

    TCPConnectionSocket(TCPConnectionSocket&& other) :
        m_socket(other.m_socket)
    {
        other.m_socket = INVALID_SOCKET;
    }

    TCPConnectionSocket& operator=(TCPConnectionSocket&& other)
    {
        this->~TCPConnectionSocket();

        this->m_socket = other.m_socket;
        other.m_socket = INVALID_SOCKET;

        return *this;
    }

    ~TCPConnectionSocket()
    {
        if (this->m_socket != INVALID_SOCKET) {
            this->closeConnection();
        }
    }

    Packet receive()
    {
        ASSERT(this->m_socket != INVALID_SOCKET, "Socket is not connected");
        Packet result;
        result.packet_size = ::recv(this->m_socket, (char*)result.packet, MTU, 0);
        if (result.packet_size == SOCKET_ERROR) {
            LOG_WARN("Failed to read from socket");
            this->closeConnection();
        }
        return result;
    }

    void send(const u8* packet, i32 packet_size)
    {
        ASSERT(this->m_socket != INVALID_SOCKET, "Socket is not connected");
        i32 totalDataSend = 0;
        while (totalDataSend < packet_size) {
            const i32 result = ::send(this->m_socket, (const char*)packet, packet_size, 0);
            totalDataSend += result;
            if (result == SOCKET_ERROR) {
                LOG_WARN("Data Transfer Failed");
                this->closeConnection();
                return;
            }
        }
    }

    void closeConnection()
    {
        ASSERT(this->m_socket != INVALID_SOCKET, "Socket is not connected");
        ::closesocket(this->m_socket);
        this->m_socket = INVALID_SOCKET;
    }

    bool isConnected() const { return this->m_socket != INVALID_SOCKET; }

public:
    TCPConnectionSocket(const TCPConnectionSocket& other) = delete;
    TCPConnectionSocket& operator=(const TCPConnectionSocket& other) = delete;
};

export class TCPServerSocket
{
private:
    SOCKET m_server_socket = INVALID_SOCKET;

public:
    TCPServerSocket(u32 port = 8080)
    {
        // TODO(Tiago): need to extract this into a network module initialization step
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        ASSERT(result == 0, "Failed to initialize Window Networking (Winsock2)");
        LOG_SUCCESS("Windows Networking Initialized");

        addrinfo* resultInfo;
        addrinfo hints;
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_flags = AI_PASSIVE;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        result = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &resultInfo);
        ASSERT(result == 0, "Failed to get address info");

        SOCKET server_socket = socket(resultInfo->ai_family, resultInfo->ai_socktype, resultInfo->ai_protocol);
        ASSERT(server_socket != INVALID_SOCKET, "Failed to create server socket");
        LOG_SUCCESS("Server Socket Created");

        result = bind(server_socket, resultInfo->ai_addr, (int)resultInfo->ai_addrlen);
        ASSERT(result == 0, "Failed to bind socket");
        LOG_SUCCESS("Socket Bound To Address Port Combo");

        this->m_server_socket = server_socket;

        result = ::listen(this->m_server_socket, SOMAXCONN);
        ASSERT(result != SOCKET_ERROR, "failed to listen to socket");
    }

    TCPConnectionSocket listen()
    {
        sockaddr connection_address;
        int connection_addr_len = sizeof(sockaddr);
        SOCKET connection_socket = accept(this->m_server_socket, &connection_address, &connection_addr_len);
        int error = WSAGetLastError();
        std::cout << error << std::endl;
        ASSERT(connection_socket != INVALID_SOCKET, "Failed to Accept client connection");

        return { connection_socket };
    }
};

}

#else

export module windows.socket;

#endif
