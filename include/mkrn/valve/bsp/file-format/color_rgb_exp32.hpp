#pragma once

#include <mkrn/valve/bsp/file-format/constants.hpp>

namespace mkrn::valve::bsp {
#if !defined(_MSC_VER)
#   pragma pack(push, 4)
#endif

/// <remarks>
/// https://developer.valvesoftware.com/wiki/Source_BSP_File_Format#Lighting
/// </remarks>
struct color_rgb_exp32
{
    std::uint8_t r{};
    std::uint8_t g{};
    std::uint8_t b{};
    std::int8_t  exponent{};
};

#if !defined(_MSC_VER)
#   pragma pack(pop)
#endif

static_assert(sizeof(color_rgb_exp32) == 4);
}
