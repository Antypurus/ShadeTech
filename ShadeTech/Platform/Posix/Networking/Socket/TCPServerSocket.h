#pragma once

#include "TCPConnectionSocket.h"
#include "Types.h"
#include "assert.h"
#include "core/result.h"

#include <netinet/in.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

namespace SHD::POSIX::Networking {

class TCPServerSocket
{
private:
    int m_server_socket = -1;

public:
    TCPServerSocket(u16 port = 8080);
    ~TCPServerSocket();
    TCPServerSocket(TCPServerSocket&& other);

    TCPServerSocket& operator=(TCPServerSocket&& other);
    result<TCPConnectionSocket, int> listenForConnection();

public:
    TCPServerSocket(const TCPServerSocket&) = delete;
    TCPServerSocket& operator=(const TCPServerSocket&) = delete;
};

}
