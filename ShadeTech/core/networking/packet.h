#pragma once

#include <core/types.h>

namespace SHD::Networking {

constexpr u32 MTU = 1500;

struct packet
{
    u8 packet_data[MTU] = {0};
    u64 packet_size = 0;
};

}
