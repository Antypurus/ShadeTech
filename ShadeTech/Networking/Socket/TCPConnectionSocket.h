#pragma once

#include "Platform.h"
#include "Types.h"
#include "core/move.h"
#include "core/result.h"

#if PLATFORM_WINDOWS
#include "Platform/Windows/Networking/Socket/TCPConnectionSocket.h"
using TCPConnectionSocketType = SHD::Windows::Networking::TCPConnectionSocket;
#else
#include "Platform/Posix/Networking/Socket/TCPConnectionSocket.h"
using TCPConnectionSocketType = SHD::POSIX::Networking::TCPConnectionSocket;
#endif

namespace SHD::Networking {

class TCPConnectionSocket
{
private:
    TCPConnectionSocketType m_connection;

public:
    TCPConnectionSocket() = default;
    ~TCPConnectionSocket() = default;
    TCPConnectionSocket(TCPConnectionSocketType connection);
    TCPConnectionSocket(TCPConnectionSocket&& other);
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
