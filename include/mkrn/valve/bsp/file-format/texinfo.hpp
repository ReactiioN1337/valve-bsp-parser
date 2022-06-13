#pragma once

#include <mkrn/math/matrix.hpp>

namespace mkrn::valve::bsp {
#if !defined(_MSC_VER)
#   pragma pack(push, 4)
#endif

/// <remarks>
/// https://developer.valvesoftware.com/wiki/Source_BSP_File_Format#Texinfo
/// </remarks>
struct texinfo_t
{
    math::vector4 texture_vecs[2]{};
    math::vector4 lightmap_vecs[2]{};
    std::int32_t  flags{};
    std::int32_t  tex_data{};
};

#if !defined(_MSC_VER)
#   pragma pack(pop)
#endif

static_assert(sizeof(texinfo_t) == 0x48);
}
