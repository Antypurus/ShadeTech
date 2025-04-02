#pragma once

#include "Networking/packet.h"
#include "assert.h"
#include "core/result.h"
#include "log.h"
#include "types.h"

#include <sys/socket.h>
#include <unistd.h>

namespace SHD::POSIX::Networking {

class TCP_connection_socket
{
protected:
    int m_socket = -1;

public:
    TCP_connection_socket() = default;
    TCP_connection_socket(int socket_fd);
    TCP_connection_socket(TCP_connection_socket&& other);
    virtual ~TCP_connection_socket();

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
