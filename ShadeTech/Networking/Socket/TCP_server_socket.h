#pragma once

#include "TCP_connection_socket.h"
#include "core/move.h"
#include "core/result.h"
#include "platform.h"
#include "types.h"

#if PLATFORM_WINDOWS
#include "Platform./Windows/Networking/Socket/Windows_TCP_server_socket.h"
using TCP_server_socket_type = SHD::Windows::Networking::TCP_server_socket;
#else
#include "Platform/Posix/Networking/Socket/Posix_TCP_server_socket.h"
using TCP_server_socket_type = SHD::POSIX::Networking::TCP_server_socket;
#endif

namespace SHD::Networking {

class TCP_server_socket
{
private:
    TCP_server_socket_type m_server_socket;

public:
    TCP_server_socket(u16 port = 8080);
    TCP_server_socket(TCP_server_socket&& other);
    TCP_server_socket& operator=(TCP_server_socket&& other);

    result<TCP_connection_socket, int> listen_for_connection();

public:
    TCP_server_socket(const TCP_server_socket&) = delete;
    TCP_server_socket& operator=(const TCP_server_socket&) = delete;
};
}
