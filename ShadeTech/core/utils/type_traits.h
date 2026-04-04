#pragma once

namespace SHD {

template<typename T>
struct is_trivially_copyable
{
    static constexpr bool value = __is_trivially_copyable(T);
};

}
