#pragma once

#include "Log.h"
#include "Platform.h"
#include "assert.h"

#include <WS2tcpip.h>
#include <Windows.h>
#include <winsock2.h>

namespace SHD::Windows::Networking {
void InitNetworkingModule();
}
