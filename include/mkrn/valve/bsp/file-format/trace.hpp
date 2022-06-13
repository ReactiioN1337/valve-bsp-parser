#pragma once

#include <mkrn/math/matrix.hpp>
#include <mkrn/valve/bsp/file-format/dbrush.hpp>

namespace mkrn::valve::bsp {
struct trace_t
{
    /// <summary>
    /// Determine if a plan is NOT valid
    /// </summary>
    bool          all_solid{ true };
    /// <summary>
    /// Determine if the start point was in a solid area
    /// </summary>
    bool          start_solid{ true };
    /// <summary>
    /// Time completed, 1.0 = didn't hit anything
    /// </summary>
    float         fraction{ 1.f };
    float         fraction_left_solid{ 1.f };
    /// <summary>
    /// Final trace position
    /// </summary>
    math::vector3 end_pos;
    std::int32_t  contents{ 0 };
    dbrush_t*     brush{ nullptr };
    std::int32_t  num_brush_sides{ 0 };

    auto
    clear() noexcept -> void
    {
        all_solid           = true;
        start_solid         = true;
        fraction            = 1.f;
        fraction_left_solid = 1.f;
        contents            = 0;
        brush               = nullptr;
        num_brush_sides     = 0;

        end_pos.clear();
    }
};
}
