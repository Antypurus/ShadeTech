#pragma once

#include <core/utils/assert.h>
#include <core/platform.h>
#include <core/utils/log.h>

#include <WS2tcpip.h>
#include <Windows.h>
#include <winsock2.h>

namespace SHD::Windows::Networking {

void init_networking_module();

}
