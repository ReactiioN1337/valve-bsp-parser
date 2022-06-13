///---------------------------------------------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2022, ReactiioN
///-- License       MIT
///---------------------------------------------------------------------------------------------------------------------
#pragma once

#include <array>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

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


#if !defined(MKRN_ENUM_OPERATORS)
#   define MKRN_ENUM_OPERATORS(Type)                                         \
        constexpr Type operator ~ (Type lhs) noexcept                        \
        {                                                                    \
            return static_cast<Type>(                                        \
                ~static_cast<std::underlying_type_t<Type>>(lhs)              \
            );                                                               \
        }                                                                    \
        constexpr Type operator | (Type lhs, Type rhs) noexcept              \
        {                                                                    \
            return static_cast<Type>(                                        \
                static_cast<std::underlying_type_t<Type>>(lhs)               \
                | static_cast<std::underlying_type_t<Type>>(rhs)             \
            );                                                               \
        }                                                                    \
        constexpr Type operator & (Type lhs, Type rhs) noexcept              \
        {                                                                    \
            return static_cast<Type>(                                        \
                static_cast<std::underlying_type_t<Type>>(lhs)               \
                & static_cast<std::underlying_type_t<Type>>(rhs)             \
            );                                                               \
        }                                                                    \
        constexpr Type operator ^ (Type lhs, Type rhs) noexcept              \
        {                                                                    \
            return static_cast<Type>(                                        \
                static_cast<std::underlying_type_t<Type>>(lhs)               \
                ^ static_cast<std::underlying_type_t<Type>>(rhs)             \
            );                                                               \
        }                                                                    \
        inline Type& operator |= (Type& lhs, Type  rhs) noexcept             \
        {                                                                    \
            auto lhs_value = static_cast<std::underlying_type_t<Type>>(lhs); \
            lhs_value |= static_cast<std::underlying_type_t<Type>>(rhs);     \
            lhs = static_cast<Type>(lhs_value);                              \
            return lhs;                                                      \
        }                                                                    \
        inline Type& operator &= (Type& lhs, Type  rhs) noexcept             \
        {                                                                    \
            auto lhs_value = static_cast<std::underlying_type_t<Type>>(lhs); \
            lhs_value &= static_cast<std::underlying_type_t<Type>>(rhs);     \
            lhs = static_cast<Type>(lhs_value);                              \
            return lhs;                                                      \
        }                                                                    \
        inline Type& operator ^= (Type& lhs, Type  rhs) noexcept             \
        {                                                                    \
            auto lhs_value = static_cast<std::underlying_type_t<Type>>(lhs); \
            lhs_value ^= static_cast<std::underlying_type_t<Type>>(rhs);     \
            lhs = static_cast<Type>(lhs_value);                              \
            return lhs;                                                      \
        }
#endif //MKRN_ENUM_OPERATORS

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

namespace core {
template<typename T>
constexpr
MKRN_FORCEINLINE
auto
enum_v(
    const T& value
) noexcept -> decltype(auto)
{
    return static_cast<std::underlying_type_t<T>>(value);
}

template<typename T>
constexpr
MKRN_FORCEINLINE
auto
bit_and(
    const T value,
    const T flag
) noexcept -> bool
{
    static_assert(std::is_integral_v<T>);

    return value & flag;
}

template<typename T>
constexpr
MKRN_FORCEINLINE
auto
enum_bit_and(
    const T value,
    const T flag
) noexcept -> bool
{
    return bit_and(
        enum_v(value),
        enum_v(flag)
    );
}
}
}
