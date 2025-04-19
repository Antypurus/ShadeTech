#pragma once

#include <core/types.h>

namespace SHD::Networking {

class IPv4_address
{
private:
    union
    {
        u8 array[4];
        u32 address = 0;
    };

public:
    IPv4_address() = default;

    IPv4_address(u8 B0, u8 B1, u8 B2, u8 B3) :
        array{ B0, B1, B2, B3 }
    {
    }

    IPv4_address(u32 address) :
        address(address)
    {
    }

    u32 get_address() const { return this->address; };
    const u8& get_address_array() const { return this->array[0]; };
};

};
