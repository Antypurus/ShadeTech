module;

#include "Platform.h"
#include "Types.h"
#include "core/move.h"
#include "core/result.h"

#if PLATFORM_WINDOWS
#else
import posix.socket;
using TCPConnectionSocketType = SHD::POSIX::Networking::TCPConnectionSocket;
#endif

import networking.packet;

export module networking.TCPConnectionSocket;
namespace SHD::Networking {

export class TCPConnectionSocket
{
private:
    TCPConnectionSocketType m_connection;

public:
    TCPConnectionSocket() = default;
    ~TCPConnectionSocket() = default;

    TCPConnectionSocket(TCPConnectionSocketType connection) :
        m_connection(move(connection)){};

    TCPConnectionSocket(TCPConnectionSocket&& other)
    {
        if (this == &other)
            return;

        this->m_connection = move(other.m_connection);
    }

    TCPConnectionSocket& operator=(TCPConnectionSocket&& other)
    {
        if (this == &other)
            return *this;

        this->m_connection = move(other.m_connection);

        return *this;
    }

    result<SHD::Networking::Packet, int> receive() { return this->m_connection.receive(); }
    void send(const u8* packet, u64 packet_size) { this->m_connection.send(packet, packet_size); }
    void closeConnection() { this->m_connection.closeConnection(); }
    bool isConnected() const { return this->m_connection.isConnected(); }

public:
    TCPConnectionSocket(const TCPConnectionSocket& other) = delete;
    TCPConnectionSocket& operator=(const TCPConnectionSocket& other) = delete;
};

}
