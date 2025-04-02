#pragma once

#include "Networking/Packet.h"
#include "Types.h"
#include "assert.h"
#include "core/result.h"

#include <Winsock2.h>

namespace SHD::Windows::Networking {

class TCPConnectionSocket
{
protected:
    SOCKET m_socket = INVALID_SOCKET;

public:
    TCPConnectionSocket() = default;
    TCPConnectionSocket(SOCKET socket);
    TCPConnectionSocket(TCPConnectionSocket&& other);
    TCPConnectionSocket& operator=(TCPConnectionSocket&& other);
    ~TCPConnectionSocket();

    result<SHD::Networking::Packet, int> receive();
    void send(const u8* packet, u64 packet_size);
    void closeConnection();
    bool isConnected() const;

public:
    TCPConnectionSocket(const TCPConnectionSocket& other) = delete;
    TCPConnectionSocket& operator=(const TCPConnectionSocket& other) = delete;
};

}
