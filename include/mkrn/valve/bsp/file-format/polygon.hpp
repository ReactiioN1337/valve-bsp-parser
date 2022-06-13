#pragma once

#include <mkrn/valve/bsp/file-format/constants.hpp>
#include <mkrn/valve/bsp/file-format/vplane.hpp>

namespace mkrn::valve::bsp {
struct polygon
{
    math::vector3 verts[max_surfinfo_verts]{};
    std::size_t   num_verts{};
    vplane        plane{};
    vplane        edge_planes[max_surfinfo_verts]{};
    math::vector3 vec2d[max_surfinfo_verts]{};
    std::int32_t  skip{};
};
}
