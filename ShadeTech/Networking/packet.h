#pragma once

#include "types.h"

namespace SHD::Networking {

constexpr u32 MTU = 1500;

struct packet
{
    u8 packet[MTU] = {};
    u64 packet_size = 0;
};

}
