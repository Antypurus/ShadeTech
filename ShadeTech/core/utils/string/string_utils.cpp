#include "string_utils.h"

#include <core/platform.h>
#include <core/types.h>

#if PLATFORM_X64
#include <immintrin.h>
#elif PLATFORM_ARM
#include <arm_neon.h>
#endif

namespace SHD {

#if PLATFORM_X64
inline usize x64_string_lenght(const char* string)
{
    const usize avx2_alignment = 32;
    const usize avx_alignment = 16;
    auto bytes_to_alignment = [](const usize value, const usize alignment) -> usize {
        return ((alignment - (value % alignment)) % alignment);
    };

    // can be up to 31 bytes of processing, 16 of which can be accelerated with SSE/AVX1
    usize pre_alignment = bytes_to_alignment((usize)string, avx2_alignment);
    if (pre_alignment >= 16) {
        usize pre_avx_alignment = bytes_to_alignment((usize)string, avx_alignment);
        for (usize i = 0; i < pre_avx_alignment; ++i) {
            if (string[i] == 0) {
                return i;
            }
        }
        __m128i zeroMask = _mm_setzero_si128();
        __m128i chunk = _mm_load_si128((__m128i*)(string + pre_avx_alignment));
        i32 mask = _mm_movemask_epi8(_mm_cmpeq_epi8(chunk, zeroMask));
        if (mask) {
            return pre_avx_alignment + _tzcnt_u32(mask);
        }
    } else {
        for (usize i = 0; i < pre_alignment; ++i) {
            if (string[i] == 0) {
                return i;
            }
        }
    }

    // if string has not been found by here we can use avx2 checks to safely check
    const char* ptr = string + pre_alignment;
    __m256i zeroMask = _mm256_setzero_si256();
    while (true) {
        __m256i chunk = _mm256_load_si256((__m256i*)ptr);
        i32 mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(chunk, zeroMask));
        if (mask) {
            // lzcnt (leading-zero count) is msvc specific in gcc you have __builtin_clz
            //  MSVC aslo has _tzcnt_u32 (trailing zero count)
            // clz -> count leading zeros
            // ctz -> count trailing zeroes
            return (usize)ptr - (usize)string + (usize)_tzcnt_u32(mask);
        }
        ptr += 32;
    }
}
#elif PLATFORM_ARM

/*
 * TODO: Implement an emulation function for the x64 movemask_epi8
 * which makes a single int value out of the comparison mask thereby
 * making the check for the index of 0-byte even more effecient.
static inline u16 NEON_movemask_eip8(const uint8x16_t& mask)
{
    return 0;
}
*/

static inline usize NEONStringLenght(const char* string)
{
    const usize NEON_alignment = 16;
    auto bytes_to_alignment = [](const usize value, const usize alignment) -> usize {
        return ((alignment - (value % alignment)) % alignment);
    };

    // check for 0 value until we align with 16-byte address
    const usize pre_alignment = bytes_to_alignment((usize)string, NEON_alignment);
    for (usize i = 0; i < pre_alignment; ++i) {
        if (string[i] == 0) {
            return i;
        }
    }

    const uint8x16_t zeroMask = vdupq_n_p8(0);
    const char* ptr = string + pre_alignment;
    while (true) {
        const uint8x16_t dataBlock = vld1q_u8((const u8*)ptr);
        const uint8x16_t cmpResult = vceqq_u8(dataBlock, zeroMask);
        const u32 mask = vmaxvq_u8(cmpResult);
        if (mask != 0) {
            // result found
            u64 pos = 0;
            while (ptr[pos] != 0) {
                pos++;
            }
            return (u64)(ptr - string) + pos;
        }
        ptr += 16;
    }
}
#endif

usize string_lenght(const char* str)
{
#if PLATFORM_X64
    return x64_string_lenght(str);
#elif PLATFORM_ARM
    return NEONStringLenght(str);
#else
    usize counter = 0;
    while (*str != 0) {
        counter++;
        str++;
    }
    return counter;
#endif
}

//TODO(Tiago): SIMD Optimize
usize last_token_occurence(const char* str, usize strlen, char token)
{
    for(isize i = (isize)strlen - 1; i >= 0; --i)
    {
        if(str[i] == token)
        {
            return (usize)i;
        }
    }

    // index 0 is returned if not matches are found for the token
    return 0;
}

//TODO(Tiago): SIMD Optimize
usize first_token_occurence(const char* str, usize strlen, char token)
{
    for(usize i = 0; i < strlen; ++i)
    {
        if(str[i] == token)
        {
            return i;
        }
    }

    // the last tring index is returned if no matches for the token are found
    return 0;
}

}
