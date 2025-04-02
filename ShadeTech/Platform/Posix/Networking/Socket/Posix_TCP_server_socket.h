#pragma once

#include "Posix_TCP_connection_socket.h"
#include "assert.h"
#include "core/result.h"
#include "types.h"

#include <netinet/in.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

namespace SHD::POSIX::Networking {

class TCP_server_socket
{
private:
    int m_server_socket = -1;

public:
    TCP_server_socket(u16 port = 8080);
    ~TCP_server_socket();
    TCP_server_socket(TCP_server_socket&& other);

    TCP_server_socket& operator=(TCP_server_socket&& other);
    result<TCP_connection_socket, int> listen_for_connection();

public:
    TCP_server_socket(const TCP_server_socket&) = delete;
    TCP_server_socket& operator=(const TCP_server_socket&) = delete;
};

}
