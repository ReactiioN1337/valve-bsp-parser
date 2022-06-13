#pragma once

#include <mkrn/math/matrix.hpp>

namespace mkrn::valve::bsp {
#if !defined(_MSC_VER)
#   pragma pack(push, 4)
#endif

struct vertex_t
{
    math::vector3 position{};
};

#if !defined(_MSC_VER)
#   pragma pack(pop)
#endif

static_assert(sizeof(vertex_t) == 0xC);
}
