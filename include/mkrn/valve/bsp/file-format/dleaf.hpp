#pragma once

#include <mkrn/valve/bsp/file-format/compressed_light_cube.hpp>

namespace mkrn::valve::bsp {
#if !defined(_MSC_VER)
#   pragma pack(push, 4)
#endif

/// <remarks>
/// https://developer.valvesoftware.com/wiki/Source_BSP_File_Format#Ambient_Lighting
/// </remarks>
struct dleaf_t
{
    std::int32_t contents{};
    std::int16_t cluster{};

    union bitfield
    {
        std::int16_t area_flags;

        struct
        {
            std::int16_t area : 9;
            std::int16_t flags : 7;
        };
    } field{};

    std::int16_t          mins[3]{};
    std::int16_t          maxs[3]{};
    std::uint16_t         first_leafface{};
    std::uint16_t         num_leaffaces{};
    std::uint16_t         first_leafbrush{};
    std::uint16_t         num_leafbrushes{};
    std::int16_t          leaf_water_data_id{};

    NODISCARD
    constexpr
    auto
    area() const noexcept -> std::int16_t
    {
        return field.area;
    }

    NODISCARD
    constexpr
    auto
    flags() const noexcept -> std::int16_t
    {
        return field.flags;
    }
};

class dleaf19_t
{
private:
    std::uint8_t base_data_[sizeof(dleaf_t) - 2 /*structure padding*/]{};

public:
    compressed_light_cube ambient_lighting{};
    std::int16_t          padding{};

    NODISCARD
    auto
    base() noexcept -> dleaf_t&
    {
        return *reinterpret_cast<dleaf_t*>(base_data_);
    }

    NODISCARD
    auto
    base() const noexcept -> const dleaf_t&
    {
        return *reinterpret_cast<const dleaf_t*>(base_data_);
    }
};

#if !defined(_MSC_VER)
#   pragma pack(pop)
#endif

static_assert(sizeof(dleaf_t) == 32);
static_assert(sizeof(dleaf19_t) == 56);
}
