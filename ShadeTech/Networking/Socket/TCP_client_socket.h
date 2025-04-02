#pragma once

#include "Networking/ip_address.h"
#include "Networking/packet.h"
#include "core/result.h"
#include "platform.h"
#include "types.h"

#if PLATFORM_WINDOWS
#include "Platform/Windows/Networking/Socket/Windows_TCP_client_socket.h"
using TCP_client_socket_type = SHD::Windows::Networking::TCP_client_socket;
#else
#include "Platform/Posix/Networking/Socket/Posix_TCP_client_socket.h"
using TCP_client_socket_type = SHD::POSIX::Networking::TCP_client_socket;
#endif

namespace SHD::Networking {

class TCP_client_socket
{
private:
    TCP_client_socket_type m_client_socket;

public:
    TCP_client_socket(IPv4_address address, u16 port);

    result<SHD::Networking::packet, int> receive();
    void send(const u8* packet, u64 packet_size);
    void close_connection();
    bool is_connected() const;
};

};
