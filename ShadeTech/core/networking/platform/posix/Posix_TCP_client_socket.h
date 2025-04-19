#pragma once

#include "Posix_TCP_connection_socket.h"
#include <core/networking/ip_address.h>
#include <core/types.h>
#include <core/utils/log.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

namespace SHD::POSIX::Networking {

class TCP_client_socket : public TCP_connection_socket
{
public:
    TCP_client_socket() = default;
    TCP_client_socket(const SHD::Networking::IPv4_address& address, u16 port);

private:
    u32 resolve_hostname(const char* hostname);
    void connect(const SHD::Networking::IPv4_address& address, u16 port);
};

}
