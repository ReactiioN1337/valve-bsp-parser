///---------------------------------------------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2022, ReactiioN
///-- License       MIT
///---------------------------------------------------------------------------------------------------------------------
#pragma once

#include <mkrn/core/requirements.hpp>

namespace mkrn::valve::flags {
enum class contents
{
    /// <summary>
    /// no contents
    /// </summary>
    empty         = 0,
    /// <summary>
    /// an eye is never valid in a solid
    /// </summary>
    solid         = 0x1,
    /// <summary>
    /// translucent, but not watery (glass)
    /// </summary>
    window        = 0x2,
    aux           = 0x4,
    /// <summary>
    /// alpha-tested "grate" textures.
    /// bullets/sight pass through, but solids don't
    /// </summary>
    grate         = 0x8,
    slime         = 0x10,
    water         = 0x20,
    mist          = 0x40,
    /// <summary>
    /// things that cannot be seen through (may be non-solid though)
    /// </summary>
    opaque        = 0x80,
    last_visible  = 0x80,
    all_visible   = last_visible | last_visible - 1,
    testfogvolume = 0x100,
    unused3       = 0x200,
    unused4       = 0x400,
    unused5       = 0x800,
    unused6       = 0x1000,
    unused7       = 0x2000,
    /// <summary>
    /// hits entities which are movetype_push (doors, plats, etc.)
    /// </summary>
    moveable      = 0x4000,
    /// <summary>
    /// remaining contents are non-visible, and don't eat brushes
    /// </summary>
    areaportal    = 0x8000,
    playerclip    = 0x10000,
    monsterclip   = 0x20000,
    /// <summary>
    /// currents can be added to any other contents, and may be mixed
    /// </summary>
    current_0     = 0x40000,
    current_90    = 0x80000,
    current_180   = 0x100000,
    current_270   = 0x200000,
    current_up    = 0x400000,
    current_down  = 0x800000,
    /// <summary>
    /// removed before bsping an entity
    /// </summary>
    origin        = 0x1000000,
    /// <summary>
    /// should never be on a brush, only in game
    /// </summary>
    monster       = 0x2000000,
    debris        = 0x4000000,
    /// <summary>
    /// brushes to be added after vis leafs
    /// </summary>
    detail        = 0x8000000,
    /// <summary>
    /// set if any surface has trans
    /// </summary>
    translucent   = 0x10000000,
    ladder        = 0x20000000,
    /// <summary>
    /// use accurate hitboxes on trace
    /// </summary>
    hitbox        = 0x40000000,
};

MKRN_ENUM_OPERATORS(contents);

template<typename T>
NODISCARD
constexpr
auto
has_content_flag(
    const T        value,
    const contents flags
) noexcept -> bool
{
    static_assert(std::is_same_v<contents, T> || std::is_integral_v<T>);

    return core::enum_bit_and(static_cast<contents>(value), flags);
}

enum class surf
{
    /// <summary>
    /// value will hold the light strength
    /// </summary>
    light     = 0x0001,
    /// <summary>
    /// effects game physics
    /// </summary>
    slick     = 0x0002,
    /// <summary>
    /// don't draw, but add to skybox
    /// </summary>
    sky       = 0x0004,
    /// <summary>
    /// turbulent water warp
    /// </summary>
    warp      = 0x0008,
    trans     = 0x0010,
    /// <summary>
    /// the surface is wet
    /// </summary>
    wet       = 0x0020,
    /// <summary>
    /// scroll towards angle
    /// </summary>
    flowing   = 0x0040,
    /// <summary>
    /// don't bother referencing the texture
    /// </summary>
    nodraw    = 0x0080,
    /// <summary>
    /// make a primary bsp splitter
    /// </summary>
    hint      = 0x0100,
    /// <summary>
    /// completely ignore, allowing non-closed brushes
    /// </summary>
    skip      = 0x0200,
    /// <summary>
    /// don't calculate light
    /// </summary>
    nolight   = 0x0400,
    /// <summary>
    /// calculate three lightmaps for the surface for bumpmapping
    /// </summary>
    bumplight = 0x0800,
    /// <summary>
    /// surface is part of a hitbox
    /// </summary>
    hitbox    = 0x8000,
};

MKRN_ENUM_OPERATORS(surf);

template<typename T>
NODISCARD
constexpr
auto
has_surf_flag(
    const T        value,
    const surf flags
) noexcept -> bool
{
    static_assert(std::is_same_v<surf, T> || std::is_integral_v<T>);

    return core::enum_bit_and(static_cast<surf>(value), flags);
}


#if !defined(MKRN_VALVE_DECLARE_MASK)
#   define MKRN_VALVE_DECLARE_MASK(Name, Flags)                     \
        constexpr inline std::int32_t mask_##Name{(Flags)};         \
        template<typename T>                                        \
        NODISCARD                                                   \
        constexpr                                                   \
        auto                                                        \
        is_mask_##Name(const T value) noexcept -> bool              \
        {                                                           \
            static_assert(std::is_integral_v<T>);                   \
                                                                    \
            return !!(static_cast<std::int32_t>(value) & mask_all); \
        }
#endif //MKRN_VALVE_DECLARE_MASK

// everyhting
MKRN_VALVE_DECLARE_MASK(all, core::enum_v(contents::all_visible));

// everything that is normally solid
MKRN_VALVE_DECLARE_MASK(
    solid, 
    core::enum_v(
        contents::solid    | 
        contents::moveable | 
        contents::window   | 
        contents::monster  | 
        contents::grate
    )
);

// everything that blocks player movement
MKRN_VALVE_DECLARE_MASK(
    playersolid, 
    core::enum_v(
        contents::solid      | 
        contents::moveable   |
        contents::playerclip | 
        contents::window     | 
        contents::monster    | 
        contents::grate
    )
);

// blocks npc movement
MKRN_VALVE_DECLARE_MASK(
    npcsolid, 
    core::enum_v(
        contents::solid       |
        contents::moveable    |
        contents::monsterclip | 
        contents::window      | 
        contents::monster     | 
        contents::grate
    )
);

// water physics in these contents
MKRN_VALVE_DECLARE_MASK(
    water, 
    core::enum_v(
        contents::water    | 
        contents::moveable |
        contents::slime
    )
);

// everything that blocks line of sight
MKRN_VALVE_DECLARE_MASK(
    opaque, 
    core::enum_v(
        contents::solid    | 
        contents::moveable | 
        contents::slime    | 
        contents::opaque
    )
);

// bullets see these as solid
MKRN_VALVE_DECLARE_MASK(
    shot, 
    core::enum_v(
        contents::solid    |
        contents::moveable |
        contents::monster  | 
        contents::window   | 
        contents::debris   |
        contents::hitbox
    )
);

// non-raycasted weapons see this as solid (includes grates)
MKRN_VALVE_DECLARE_MASK(
    shot_hull, 
    core::enum_v(
        contents::solid    | 
        contents::moveable | 
        contents::monster  | 
        contents::window   |
        contents::debris   |
        contents::grate
    )
);

// everything normally solid, except monsters (world+brush only)
MKRN_VALVE_DECLARE_MASK(
    solid_brushonly, 
    core::enum_v(
        contents::solid    | 
        contents::moveable |
        contents::window   | 
        contents::grate
    )
);

// everything normally solid for player movement, except monsters (world+brush only)
MKRN_VALVE_DECLARE_MASK(
    playersolid_brushonly, 
    core::enum_v(
        contents::solid      |
        contents::moveable   |
        contents::window     | 
        contents::playerclip |
        contents::grate
    )
);

// everything normally solid for npc movement, except monsters (world+brush only)
MKRN_VALVE_DECLARE_MASK(
    npcsolid_brushonly, 
    core::enum_v(
        contents::solid       |
        contents::moveable    | 
        contents::window      | 
        contents::monsterclip | 
        contents::grate
    )
);

// just the world, used for route rebuilding
MKRN_VALVE_DECLARE_MASK(
    npcworldstatic,
    core::enum_v(
        contents::solid       |
        contents::window      | 
        contents::monsterclip | 
        contents::grate
    )
);

// undone: this is untested, any moving water
MKRN_VALVE_DECLARE_MASK(
    current, 
    core::enum_v(
        contents::current_0   | 
        contents::current_90  | 
        contents::current_180 | 
        contents::current_270 | 
        contents::current_up  | 
        contents::current_down
    )
);

MKRN_VALVE_DECLARE_MASK(
    deadsolid, 
    core::enum_v(
        contents::solid      |
        contents::playerclip | 
        contents::window     | 
        contents::grate
    )
);

#undef MKRN_VALVE_DECLARE_MASK
}

namespace mkrn::valve::bsp {
constexpr inline std::int32_t max_brush_lightmap_dim_without_border   = 32;
constexpr inline std::int32_t max_brush_lightmap_dim_including_border = 35;
constexpr inline std::int32_t max_disp_lightmap_dim_without_border    = 128;
constexpr inline std::int32_t max_disp_lightmap_dim_including_border  = 131;
constexpr inline std::int32_t max_lightmap_dim_without_border         = max_disp_lightmap_dim_without_border;
constexpr inline std::int32_t max_lightmap_dim_including_border       = max_disp_lightmap_dim_including_border;
constexpr inline std::size_t  max_surfinfo_verts                      = 32;
constexpr inline std::size_t  max_header_lumps                            = 64;
constexpr inline std::size_t  max_polygons                            = 50120;
constexpr inline std::size_t  max_mod_known                           = 512;
constexpr inline std::size_t  max_map_models                          = 1024;
constexpr inline std::size_t  max_map_brushes                         = 8192;
constexpr inline std::size_t  max_map_entities                        = 4096;
constexpr inline std::size_t  max_map_entstring                       = static_cast<std::size_t>(256) * 1024;
constexpr inline std::size_t  max_map_nodes                           = 65536;
constexpr inline std::size_t  max_map_texinfo                         = 12288;
constexpr inline std::size_t  max_map_texdata                         = 2048;
constexpr inline std::size_t  max_map_leafbrushes                     = 65536;
constexpr inline std::size_t  min_map_disp_power                      = 2;
constexpr inline std::size_t  max_map_disp_power                      = 4;
constexpr inline std::size_t  max_map_surfedges                       = 512000;
constexpr inline std::size_t  max_disp_corner_neighbors               = 4;
}
