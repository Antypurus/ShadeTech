#pragma once

#include "Networking/IPAddress.h"
#include "Networking/Packet.h"
#include "Platform.h"
#include "Types.h"
#include "core/result.h"

#if PLATFORM_WINDOWS
#include "Platform/Windows/Networking/Socket/TCPClientSocket.h"
using TCPClientSocketType = SHD::Windows::Networking::TCPClientSocket;
#else
#include "Platform/Posix/Networking/Socket/TCPClientSocket.h"
using TCPClientSocketType = SHD::POSIX::Networking::TCPClientSocket;
#endif

namespace SHD::Networking {

class TCPClientSocket
{
private:
    TCPClientSocketType m_client_socket;

public:
    TCPClientSocket(IPv4Address address, u16 port);

    result<SHD::Networking::Packet, int> receive();
    void send(const u8* packet, u64 packet_size);
    void closeConnection();
    bool isConnected() const;
};

};
