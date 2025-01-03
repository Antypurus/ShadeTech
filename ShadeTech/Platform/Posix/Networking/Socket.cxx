module;

#include "Platform.h"

#if PLATFORM_POSIX

#include "Log.h"
#include "Types.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

export module posix.socket;

namespace SHD::POSIX::Networking {

export class TCPConnectionSocket
{
private:
    int m_socket = -1;

public:
    TCPConnectionSocket() = default;
    TCPConnectionSocket(int socket_fd) :
        m_socket(socket_fd) {};

    TCPConnectionSocket(TCPConnectionSocket&& other);
    TCPConnectionSocket& operator=(TCPConnectionSocket&& other);

    ~TCPConnectionSocket()
    {
        if (this->m_socket != -1) {
            close(this->m_socket);
            this->m_socket = -1;
        }
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
