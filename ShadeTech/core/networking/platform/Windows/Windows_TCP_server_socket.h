#pragma once

#include "Windows_TCP_connection_socket.h"
#include <core/types.h>
#include <core/utils/assert.h>
#include <core/utils/log.h>
#include <core/utils/result.h>

#include <WS2tcpip.h>
#include <Winsock2.h>
#include <string>

namespace SHD::Windows::Networking {

class TCP_server_socket
{
private:
    SOCKET m_server_socket = INVALID_SOCKET;

public:
    TCP_server_socket(u16 port = 8080);
    result<TCP_connection_socket, int> listen_for_connection();
};

}
