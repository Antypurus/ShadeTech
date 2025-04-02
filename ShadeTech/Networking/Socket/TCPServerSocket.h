#pragma once

#include "Platform.h"
#include "TCPConnectionSocket.h"
#include "Types.h"
#include "core/move.h"
#include "core/result.h"

#if PLATFORM_WINDOWS
#include "Platform./Windows/Networking/Socket/TCPServerSocket.h"
using TCPServerSocketType = SHD::Windows::Networking::TCPServerSocket;
#else
#include "Platform/Posix/Networking/Socket/TCPServerSocket.h"
using TCPServerSocketType = SHD::POSIX::Networking::TCPServerSocket;
#endif

namespace SHD::Networking {

class TCPServerSocket
{
private:
    TCPServerSocketType m_server_socket;

public:
    TCPServerSocket(u16 port = 8080);
    TCPServerSocket(TCPServerSocket&& other);
    TCPServerSocket& operator=(TCPServerSocket&& other);

    result<TCPConnectionSocket, int> listenForConnection();

public:
    TCPServerSocket(const TCPServerSocket&) = delete;
    TCPServerSocket& operator=(const TCPServerSocket&) = delete;
};
}
