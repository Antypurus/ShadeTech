#pragma once

#include "Networking/IPAddress.h"
#include "TCPConnectionSocket.h"
#include "Types.h"
#include "assert.h"

#include <WS2tcpip.h>
#include <Winsock2.h>

namespace SHD::Windows::Networking {

class TCPClientSocket : public TCPConnectionSocket
{
public:
    TCPClientSocket(const SHD::Networking::IPv4Address& address, u16 port);
};
}
