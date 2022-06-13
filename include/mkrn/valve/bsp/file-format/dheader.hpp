#pragma once

#include <mkrn/valve/bsp/file-format/lump.hpp>

namespace mkrn::valve::bsp {
#if !defined(_MSC_VER)
#   pragma pack(push, 4)
#endif


/// <remarks>
/// https://developer.valvesoftware.com/wiki/Source_BSP_File_Format#Lump_compression
/// </remarks>
struct dheader_t
{
    std::int32_t ident{};
    std::int32_t version{};
    lump_t       lumps[core::enum_v(lump_t::id::last_index)]{};
    std::int32_t map_revision{};

    NODISCARD
    constexpr
    auto
    valid() const noexcept -> bool
    {
        return ident == ('P' << 24) + ('S' << 16) + ('B' << 8) + 'V';
    }
};

#if !defined(_MSC_VER)
#   pragma pack(pop)
#endif

static_assert(sizeof(dheader_t) == 0x40C);
}
