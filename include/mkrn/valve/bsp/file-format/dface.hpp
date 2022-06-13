#pragma once

#include <mkrn/valve/bsp/file-format/constants.hpp>

namespace mkrn::valve::bsp {
#if !defined(_MSC_VER)
#   pragma pack(push, 4)
#endif

/// <remarks>
/// https://developer.valvesoftware.com/wiki/Source_BSP_File_Format#Face_and_original_face
/// </remarks>
struct dface_t
{
    std::uint16_t plane_num{};
    std::uint8_t  side{};
    std::uint8_t  on_node{};
    std::int32_t  first_edge{};
    std::int16_t  num_edges{};
    std::int16_t  tex_info{};
    std::int16_t  disp_info{};
    std::int16_t  surface_fog_volume_id{};
    byte4         styles{};
    std::int32_t  light_offset{};
    float         area{};
    std::int32_t  lightmap_texture_mins_in_luxels[2]{};
    std::int32_t  lightmap_texture_size_in_luxels[2]{};
    std::int32_t  orig_face{};
    std::uint16_t num_prims{};
    std::uint16_t first_prim_id{};
    std::uint16_t smoothing_groups{};
};

#if !defined(_MSC_VER)
#   pragma pack(pop)
#endif

static_assert(sizeof(dface_t) == 0x38);
}
