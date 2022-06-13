#pragma once

#include <mkrn/valve/bsp/file-format/constants.hpp>

namespace mkrn::valve::bsp {
#if !defined(_MSC_VER)
#   pragma pack(push, 4)
#endif

/// <remarks>
/// https://developer.valvesoftware.com/wiki/Source_BSP_File_Format#Edge
/// </remarks>
struct dedge_t
{
    /// <summary>
    /// vertex indices
    /// </summary>
    std::uint16_t v[2]{};
};

#if !defined(_MSC_VER)
#   pragma pack(pop)
#endif

static_assert(sizeof(dedge_t) == 0x4);
}
