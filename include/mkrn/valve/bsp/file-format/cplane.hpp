#pragma once

#include <mkrn/math/matrix.hpp>

namespace mkrn::valve::bsp {
#if !defined(_MSC_VER)
#   pragma pack(push, 4)
#endif

/// <remarks>
/// modified dplane_t structure
/// </remarks>
struct cplane_t
{
    math::vector3 normal{};
    float         distance{};
    std::uint8_t  type{};
    std::uint8_t  sign_bits{};
};

#if !defined(_MSC_VER)
#   pragma pack(pop)
#endif

static_assert(sizeof(cplane_t) == 0x14);
}
