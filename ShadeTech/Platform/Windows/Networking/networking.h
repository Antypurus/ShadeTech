#pragma once

#include "assert.h"
#include "log.h"
#include "platform.h"

#include <WS2tcpip.h>
#include <Windows.h>
#include <winsock2.h>

namespace SHD::Windows::Networking {

void init_networking_module();

}
