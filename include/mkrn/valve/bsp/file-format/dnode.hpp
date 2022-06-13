#pragma once

#include <mkrn/valve/bsp/file-format/constants.hpp>

namespace mkrn::valve::bsp {
#if !defined(_MSC_VER)
#   pragma pack(push, 4)
#endif

/// <remarks>
/// https://developer.valvesoftware.com/wiki/Source_BSP_File_Format#Ambient_Lighting
/// </remarks>
struct dnode_t
{
    std::int32_t  plane_num{};
    std::int32_t  children[2]{};
    std::int16_t  mins[3]{};
    std::int16_t  maxs[3]{};
    std::uint16_t first_face{};
    std::int16_t  num_faces{};
    std::int16_t  area{};
};

#if !defined(_MSC_VER)
#   pragma pack(pop)
#endif

static_assert(sizeof(dnode_t) == 0x20);
}
