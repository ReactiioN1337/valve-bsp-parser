#pragma once

#include <mkrn/math/matrix.hpp>

namespace mkrn::valve::bsp {
#if !defined(_MSC_VER)
#   pragma pack(push, 4)
#endif

/// <remarks>
/// https://developer.valvesoftware.com/wiki/Source_BSP_File_Format#Lumps
/// </remarks>
struct dplane_t
{
    math::vector3 normal{};
    float         distance{};
    std::int32_t  type{};
};

#if !defined(_MSC_VER)
#   pragma pack(pop)
#endif

static_assert(sizeof(dplane_t) == 0x14);
}
