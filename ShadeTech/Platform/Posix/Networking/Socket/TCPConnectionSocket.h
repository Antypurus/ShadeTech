#pragma once

#include "Log.h"
#include "Networking/Packet.h"
#include "Types.h"
#include "assert.h"
#include "core/result.h"

#include <sys/socket.h>
#include <unistd.h>

namespace SHD::POSIX::Networking {

class TCPConnectionSocket
{
protected:
    int m_socket = -1;

public:
    TCPConnectionSocket() = default;
    TCPConnectionSocket(int socket_fd);
    TCPConnectionSocket(TCPConnectionSocket&& other);
    virtual ~TCPConnectionSocket();

    TCPConnectionSocket& operator=(TCPConnectionSocket&& other);

    result<SHD::Networking::Packet, int> receive();
    void send(const u8* packet, u64 packet_size);
    void closeConnection();
    bool isConnected() const;

public:
    TCPConnectionSocket(const TCPConnectionSocket& other) = delete;
    TCPConnectionSocket& operator=(const TCPConnectionSocket& other) = delete;
};

}
