#pragma once

#include "Types.h"

namespace SHD::Networking {

constexpr u32 MTU = 1500;

struct Packet
{
    u8 packet[MTU] = {};
    u64 packet_size = 0;
};

}
