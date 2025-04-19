#pragma once

#include <core/networking/ip_address.h>
#include <core/networking/packet.h>
#include <core/platform.h>
#include <core/types.h>
#include <core/utils/result.h>

#if PLATFORM_WINDOWS
#include <core/networking/platform/Windows/Windows_TCP_client_socket.h>
using TCP_client_socket_type = SHD::Windows::Networking::TCP_client_socket;
#else
#include <core/networking/platform/posix/Posix_TCP_client_socket.h>
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
