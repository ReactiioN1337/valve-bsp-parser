#pragma once

#include <mkrn/valve/bsp/file-format/constants.hpp>

namespace mkrn::valve::bsp {
#if !defined(_MSC_VER)
#   pragma pack(push, 4)
#endif

/// <remarks>
/// https://developer.valvesoftware.com/wiki/Source_BSP_File_Format#Brush_and_brushside
/// </remarks>
struct dbrush_t
{
    std::int32_t first_side{}; // 0x0
    std::int32_t num_sides{};  // 0x4
    std::int32_t contents{};   // 0x8
};

#if !defined(_MSC_VER)
#   pragma pack(pop)
#endif

static_assert(sizeof(dbrush_t) == 0xC);
}
