module;

#include "Types.h"

export module networking.packet;

namespace SHD::Networking {

export constexpr u32 MTU = 1500;

export struct Packet
{
    u8 packet[MTU] = {};
    u64 packet_size = 0;
};

}
