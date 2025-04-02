#pragma once

#include "Log.h"
#include "Networking/IPAddress.h"
#include "TCPConnectionSocket.h"
#include "Types.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

namespace SHD::POSIX::Networking {

class TCPClientSocket : public TCPConnectionSocket
{
public:
    TCPClientSocket() = default;
    TCPClientSocket(const SHD::Networking::IPv4Address& address, u16 port);

private:
    u32 resolveHostname(const char* hostname);
    void connect(const SHD::Networking::IPv4Address& address, u16 port);
};

}
