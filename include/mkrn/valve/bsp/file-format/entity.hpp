#pragma once

#include <mkrn/valve/bsp/file-format/constants.hpp>
#include <unordered_map>

namespace mkrn::valve::bsp {
using key_values = std::unordered_map<std::string, std::string>;

/// <remarks>
/// https://developer.valvesoftware.com/wiki/Source_BSP_File_Format#Lumps
/// </remarks>
struct entity_t
{
    key_values data;
};
}
