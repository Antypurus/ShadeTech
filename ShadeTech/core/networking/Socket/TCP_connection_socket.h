#pragma once

#include <core/types.h>
#include <core/platform.h>

#if PLATFORM_WINDOWS
#include <core/networking/platform/Windows/Windows_TCP_connection_socket.h>
using TCP_connection_socket_type = SHD::Windows::Networking::TCP_connection_socket;
#else
#include <core/networking/platform/posix/Posix_TCP_connection_socket.h>
using TCP_connection_socket_type = SHD::POSIX::Networking::TCP_connection_socket;
#endif

namespace SHD::Networking {

class TCP_connection_socket
{
private:
    TCP_connection_socket_type m_connection;

public:
    TCP_connection_socket() = default;
    ~TCP_connection_socket() = default;
    TCP_connection_socket(TCP_connection_socket_type connection);
    TCP_connection_socket(TCP_connection_socket&& other);
    TCP_connection_socket& operator=(TCP_connection_socket&& other);

    result<SHD::Networking::packet, int> receive();
    void send(const u8* packet, u64 packet_size);
    void close_connection();
    bool is_connected() const;

public:
    TCP_connection_socket(const TCP_connection_socket& other) = delete;
    TCP_connection_socket& operator=(const TCP_connection_socket& other) = delete;
};

}
