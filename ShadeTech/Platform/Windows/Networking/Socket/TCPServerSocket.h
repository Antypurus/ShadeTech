#pragma once

#include "Log.h"
#include "TCPConnectionSocket.h"
#include "Types.h"
#include "assert.h"
#include "core/result.h"

#include <WS2tcpip.h>
#include <Winsock2.h>
#include <string>

namespace SHD::Windows::Networking {

class TCPServerSocket
{
private:
    SOCKET m_server_socket = INVALID_SOCKET;

public:
    TCPServerSocket(u16 port = 8080);
    result<TCPConnectionSocket, int> listenForConnection();
};

}
