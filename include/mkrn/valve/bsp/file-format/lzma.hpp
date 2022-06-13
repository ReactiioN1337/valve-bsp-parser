#pragma once

#include <mkrn/valve/bsp/file-format/constants.hpp>

namespace mkrn::valve::bsp {
#pragma pack(push, 1)

/// <remarks>
/// https://developer.valvesoftware.com/wiki/Source_BSP_File_Format#Lump_compression
/// </remarks>
struct lzma_header_t
{
public:
    struct standard_header
    {
        std::uint8_t  properties[5]{};
        std::uint64_t actual_size{};
    };

    std::int32_t id{};
    std::int32_t actual_size{};
    std::int32_t lzma_size{};
    std::uint8_t properties[5]{};

    NODISCARD
    constexpr
    auto
    valid() const noexcept -> bool
    {
        return id == ('A' << 24) + ('M' << 16) + ('Z' << 8) + 'L';
    }

    NODISCARD
    constexpr
    auto
    get_standard_header() const noexcept -> standard_header
    {
        standard_header header{};

        for (std::size_t i{}; i < 5; ++i) {
            header.properties[i] = properties[i];
        }

        header.actual_size = static_cast<std::uint64_t>(actual_size);

        return header;
    }    
};
#pragma pack(pop)
}
