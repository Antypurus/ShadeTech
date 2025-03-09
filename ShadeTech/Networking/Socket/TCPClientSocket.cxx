module;

#include "Platform.h"
#include "Types.h"
#include "core/result.h"

#if PLATFORM_WINDOWS
import windows.socket;
using TCPClientSocketType = SHD::Windows::Networking::TCPClientSocket;
#else
import posix.socket;
using TCPClientSocketType = SHD::POSIX::Networking::TCPClientSocket;
#endif

import networking.ip;
import networking.packet;

export module networking.TCPClientSocket;

namespace SHD::Networking {

export class TCPClientSocket
{
private:
    TCPClientSocketType m_client_socket;

public:
    TCPClientSocket(IPv4Address address, u16 port) :
        m_client_socket(address, port)
    {
    }

    result<SHD::Networking::Packet, int> receive() { return this->m_client_socket.receive(); }
    void send(const u8* packet, u64 packet_size) { this->m_client_socket.send(packet, packet_size); }
    void closeConnection() { this->m_client_socket.closeConnection(); }
    bool isConnected() const { return this->m_client_socket.isConnected(); }
};

};
