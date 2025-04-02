#include "Networking.h"

namespace SHD::Windows::Networking {
void InitNetworkingModule()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    ASSERT(result == 0, "Failed to initialize Window Networking (Winsock2)");
    LOG_SUCCESS("Windows Networking Initialized");
};

}
