#include "networking.h"

#include <core/utils/assert.h>

namespace SHD::Windows::Networking {
void init_networking_module()
{
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    ASSERT(result == 0, "Failed to initialize Window Networking (Winsock2)");
    LOG_SUCCESS("Windows Networking Initialized");
};

}
