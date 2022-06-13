#pragma once

#include <array>
#include <algorithm>
#include <string>

#if !defined(_MSC_VER)
#   include <limits>
#endif

#if !defined(NODISCARD)
// in MSVC this variable is bogus and refers to C++98 version.
#   if __cplusplus >= 201703L
#       define NODISCARD [[nodiscard]]
#   elif defined(_MSC_VER)
#       define NODISCARD _NODISCARD
#   else
#       define NODISCARD [[nodiscard]]
#   endif
#endif //NODISCARD

#if !defined(MKRN_FORCEINLINE)
#   if defined(_MSC_VER)
#       define MKRN_FORCEINLINE __forceinline
#   else
#       define MKRN_FORCEINLINE __attribute__((always_inline))
#   endif
#endif //MKRN_FORCEINLINE

namespace mkrn {
union byte1
{
    std::int8_t  i8;
    std::uint8_t u8;
    bool         b;
    char         c;
    std::uint8_t val[1];
};
static_assert(sizeof(byte1) == 1);

union byte2
{
    std::int16_t  i16;
    std::uint16_t u16;
    wchar_t       wc;
    std::uint8_t  val[2];

    struct
    {
        byte1 lo;
        byte1 hi;
    } data;
};
static_assert(sizeof(byte2) == 2);

union byte4
{
    std::int32_t  i32;
    std::uint32_t u32;
    float         f32;
    std::uint8_t  val[4];

    struct
    {
        byte2 lo;
        byte2 hi;
    } data;
};
static_assert(sizeof(byte4) == 4);

union byte8
{
    std::int64_t  i64;
    std::uint32_t u64;
    double        f64;
    std::uint8_t  val[8];

    struct
    {
        byte4 lo;
        byte4 hi;
    } data;
};
static_assert(sizeof(byte8) == 8);
}
