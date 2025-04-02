#pragma once

#include "Networking/ip_address.h"
#include "types.h"
#include "Windows_TCP_connection_socket.h"
#include "assert.h"

#include <WS2tcpip.h>
#include <Winsock2.h>

namespace SHD::Windows::Networking {

class TCP_client_socket : public TCP_connection_socket
{
public:
    TCP_client_socket(const SHD::Networking::IPv4_address& address, u16 port);
};
}
