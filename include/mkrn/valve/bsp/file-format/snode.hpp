#pragma once

#include <mkrn/valve/bsp/file-format/cplane.hpp>
#include <mkrn/valve/bsp/file-format/dleaf.hpp>

namespace mkrn::valve::bsp {
#if !defined(_MSC_VER)
#   pragma pack(push, 4)
#endif

struct snode_t
{
    std::int32_t  plane_num{};
    cplane_t*     plane{};
    std::int32_t  children[2]{};
    dleaf_t*      leaf_children{};
    snode_t*      node_children{};
    std::uint16_t mins[3]{};
    std::uint16_t maxs[3]{};
    std::uint16_t first_face{};
    std::uint16_t num_faces{};
    std::int16_t  area{};
};

#if !defined(_MSC_VER)
#   pragma pack(pop)
#endif
}
