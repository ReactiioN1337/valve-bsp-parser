#pragma once

#include <mkrn/math/matrix.hpp>

namespace mkrn::valve::bsp {
struct vplane
{
    math::vector3 origin{};
    float         distance{};

    NODISCARD
    auto
    dist(
        const math::vector3& destination
    ) const noexcept -> float
    {
        return origin.dot(destination) - distance;
    }

    auto
    init(
        const math::vector3& src,
        const float          dist
    ) noexcept
    {
        origin   = src;
        distance = dist;
    }
};
}
