module;

#include "Platform.h"
#include "Types.h"
#include "core/move.h"
#include "core/result.h"

#if PLATFORM_WINDOWS
import windows.socket;
using TCPServerSocketType = SHD::Windows::Networking::TCPServerSocket;
#else
import posix.socket;
using TCPServerSocketType = SHD::POSIX::Networking::TCPServerSocket;
#endif

import networking.TCPConnectionSocket;

export module networking.TCPServerSocket;
namespace SHD::Networking {

export class TCPServerSocket
{
private:
    TCPServerSocketType m_server_socket;

public:
    TCPServerSocket(u16 port = 8080) :
        m_server_socket(port)
    {
    }

    TCPServerSocket(TCPServerSocket&& other) :
        m_server_socket(move(other.m_server_socket))
    {
    }

    TCPServerSocket& operator=(TCPServerSocket&& other)
    {
        if (this == &other)
            return *this;
        this->m_server_socket = move(other.m_server_socket);
        return *this;
    }

    result<TCPConnectionSocket, int> listenForConnection()
    {
        auto result = this->m_server_socket.listenForConnection();
        if (result.hasValue()) {
            return { move(*result) };
        } else {
            return ErrorResult{ result.getError() };
        }
    }

public:
    TCPServerSocket(const TCPServerSocket&) = delete;
    TCPServerSocket& operator=(const TCPServerSocket&) = delete;
};
}
