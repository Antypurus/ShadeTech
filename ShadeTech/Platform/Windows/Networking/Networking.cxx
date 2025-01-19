module;

#include "Log.h"
#include "Platform.h"
#include "assert.h"

export module windows.networking;

#if PLATFORM_WINDOWS
#include <WS2tcpip.h>
#include <Windows.h>
#include <winsock2.h>

export import windows.socket;

namespace SHD::Windows::Networking {
export void InitNetworkingModule()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    ASSERT(result == 0, "Failed to initialize Window Networking (Winsock2)");
    LOG_SUCCESS("Windows Networking Initialized");
};
}
#endif
