#pragma once

#include <mkrn/valve/bsp/file-format/color_rgb_exp32.hpp>

namespace mkrn::valve::bsp {
#if !defined(_MSC_VER)
#   pragma pack(push, 4)
#endif

/// <remarks>
/// https://developer.valvesoftware.com/wiki/Source_BSP_File_Format#Ambient_Lighting
/// </remarks>
struct compressed_light_cube
{
    color_rgb_exp32 color[6]{};
};


#if !defined(_MSC_VER)
#   pragma pack(pop)
#endif

static_assert(sizeof(compressed_light_cube) == 0x18);
}
