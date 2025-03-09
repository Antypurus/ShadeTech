module;

#include "Types.h"

export module networking.ip;

namespace SHD::Networking {

export class IPv4Address
{
private:
    union
    {
        u8 array[4];
        u32 address = 0;
    };

public:
    IPv4Address() = default;

    IPv4Address(u8 B0, u8 B1, u8 B2, u8 B3) :
        array{ B0, B1, B2, B3 }
    {
    }

    IPv4Address(u32 address) :
        address(address)
    {
    }

    u32 getAddress() const { return this->address; };
    const u8& getAddressArray() const { return this->array[0]; };
};

};
