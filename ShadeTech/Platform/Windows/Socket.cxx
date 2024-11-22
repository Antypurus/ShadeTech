module;

#include "Platform.h"
#if PLATFORM_WINDOWS

#include "assert.h"

#include <WS2tcpip.h>
#include <winsock2.h>

export module socket;
namespace SHD {
namespace Windows {

export class TCPSocket
{
private:
public:
    TCPSocket()
    {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        ASSERT(result == 0, "Failed to initialize Window Networking (Winsock2)");
        LOG_SUCCESS("Windows Networking Initialized");
    }
};

}
}

#endif
