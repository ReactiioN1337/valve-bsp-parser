#pragma once

#include <mkrn/valve/bsp/file-format/constants.hpp>

namespace mkrn::valve::bsp {
#if !defined(_MSC_VER)
#   pragma pack(push, 4)
#endif

/// <remarks>
/// https://developer.valvesoftware.com/wiki/Source_BSP_File_Format#Brush_and_brushside
/// </remarks>
struct dbrushside_t
{
    /// <summary>
    /// 0x0: facing out of the leaf
    /// </summary>
    std::uint16_t plane_num{};

    /// <summary>
    /// 0x2: texture info
    /// </summary>
    std::int16_t  tex_info{};

    /// <summary>
    /// 0x4: displacement info
    /// </summary>
    std::int16_t  disp_info{};

    /// <summary>
    /// 0x6: is the side a bevel plane?
    /// </summary>
    byte2         bevel{};     // 0x6

    NODISCARD
    constexpr
    auto
    is_bevel_plane() const noexcept -> bool
    {
        return bevel.data.lo.b;
    }

    NODISCARD
    constexpr
    auto
    thin() const noexcept -> std::uint8_t
    {
        return bevel.val[1];
    }
};

#if !defined(_MSC_VER)
#   pragma pack(pop)
#endif

static_assert(sizeof(dbrushside_t) == 0x8);
}
