#pragma once

#include <core/types.h>
#include <core/utils/assert.h>
#include <core/utils/result.h>
#include <core/networking/packet.h>

#include <Winsock2.h>

namespace SHD::Windows::Networking {

class TCP_connection_socket
{
protected:
    SOCKET m_socket = INVALID_SOCKET;

public:
    TCP_connection_socket() = default;
    TCP_connection_socket(SOCKET socket);
    TCP_connection_socket(TCP_connection_socket&& other);
    TCP_connection_socket& operator=(TCP_connection_socket&& other);
    ~TCP_connection_socket();

    result<SHD::Networking::packet, int> receive();
    void send(const u8* packet, u64 packet_size);
    void close_connection();
    bool is_connected() const;

public:
    TCP_connection_socket(const TCP_connection_socket& other) = delete;
    TCP_connection_socket& operator=(const TCP_connection_socket& other) = delete;
};

}
