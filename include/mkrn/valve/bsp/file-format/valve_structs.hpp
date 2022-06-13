///---------------------------------------------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2022, ReactiioN
///-- License       MIT
///---------------------------------------------------------------------------------------------------------------------
#pragma once

#include <unordered_map>
#include <mkrn/math/matrix.hpp>
#include <mkrn/valve/bsp/file-format/constants.hpp>

namespace mkrn::valve::bsp {
constexpr bool has_valid_bsp_ident(
    const std::int32_t ident
)
{
    //Why the check was >= instead of == I'll never know
    return ident == ( 'P' << 24 ) + ( 'S' << 16 ) + ( 'B' << 8 ) + 'V';
}

enum class lump_index
    : std::size_t
{
    entities                       = 0,
    planes                         = 1,
    tex_data                       = 2,
    vertices                       = 3, // "LUMP_VERTEXES"
    visibility                     = 4,
    nodes                          = 5,
    tex_info                       = 6,
    faces                          = 7,
    lighting                       = 8,
    occlusion                      = 9,
    leafs                          = 10,
    edges                          = 12,
    surfedges                      = 13,
    models                         = 14,
    world_lights                   = 15,
    leaf_faces                     = 16,
    leaf_brushes                   = 17,
    brushes                        = 18,
    brush_sides                    = 19,
    ares                           = 20,
    area_portals                   = 21,
    portals                        = 22,
    clusters                       = 23,
    portal_verts                   = 24,
    cluster_portals                = 25,
    disp_info                      = 26,
    original_faces                 = 27,
    phys_collide                   = 29,
    vert_normals                   = 30,
    vert_normal_indices            = 31,
    lightmap_alphas                = 32,
    disp_verts                     = 33,
    disp_lightmap_sample_positions = 34,
    game_lump                      = 35,
    leafwaterdata                  = 36,
    primitives                     = 37,
    prim_vertices                  = 38,
    prim_indices                   = 39,
    pak_file                       = 40,
    clipportal_vertices            = 41,
    cubemaps                       = 42,
    texdata_string_data            = 43,
    texdata_string_table           = 44,
    overlays                       = 45,
    leaf_min_dist_to_water         = 46,
    face_macro_texture_info        = 47,
    disp_tris                      = 48
};

class lump_t
{
    using type_four_cc = std::array<char, 4>;
public:
    std::int32_t file_offset = 0; // 0x0
    std::int32_t file_size = 0; // 0x4
    std::int32_t version = 0; // 0x8
    type_four_cc four_cc{};    // 0xC
};//Size=0x10

class lumpfileheader_t
{
public:
    std::int32_t file_offset = 0;   // offset in the file where the lump data begins (should be 0x14)
    std::int32_t lumpID = 0;       // the lump ID according to the lump table
    std::int32_t version = 0;  // same as "version" in lump_t
    std::int32_t file_size = 0;   // same as "filelen" in lump_t
    std::int32_t map_revidion = 0;  // same as in dheader_t
}; //Size=0x14


constexpr bool has_valid_lzma_ident(
    const std::int32_t ident
)
{
    return ident == ('A' << 24) + ('M' << 16) + ('Z' << 8) + 'L';
}
#pragma pack(1)
class lzma_header_t
{
    using type_lzma_prop = std::array<char, 5>;
public:
    std::int32_t	id;
    std::int32_t	actualSize;		// always little endian
    std::int32_t	lzmaSize;		// always little endian
    type_lzma_prop	properties;
};
#pragma pack()

class dheader_t
{
    using type_lumps = std::array<lump_t, max_header_lumps>;

public:
    std::int32_t ident        = 0; // 0x000
    std::int32_t version      = 0; // 0x004
    type_lumps   lumps{};          // 0x008
    std::int32_t map_revision = 0; // 0x408

};//Size=0x40C

class dplane_t
{
public:
    math::vector3 normal;   // 0x00
    float         distance; // 0x0C
    std::int32_t  type;     // 0x10
};//Size=0x14

class cplane_t
{
public:
    math::vector3 normal;            // 0x00
    float         distance;          // 0x0C
    std::uint8_t  type;              // 0x10
    std::uint8_t  sign_bits;         // 0x11
private:
    std::uint8_t  _pad0x12[ 0x2 ]{}; // 0x12
};//Size=0x14



class entity_t
{
public:
    std::unordered_map<std::string,std::string> keyvalues;
};

class dedge_t
{
public:
    std::array<std::uint16_t, 2> v; // 0x0
};//Size=0x4

class mvertex_t
{
public:
    math::vector3 position; // 0x0
};//Size=0xC

class dleaf_t
{
    using type_min_max = std::array<std::int16_t, 3>;

public:
    std::int32_t  contents;           // 0x00
    std::int16_t  cluster;            // 0x04
    std::int16_t  area : 9;           // 0x06
    std::int16_t  flags : 7;          // 0x11
    type_min_max  mins;               // 0x1A
    type_min_max  maxs;               // 0x20
    std::uint16_t first_leafface;     // 0x26
    std::uint16_t num_leaffaces;      // 0x28
    std::uint16_t first_leafbrush;    // 0x2A
    std::uint16_t num_leafbrushes;    // 0x2C
    std::int16_t  leaf_water_data_id; // 0x2E
};//Size=0x30

struct dgamelump_t
{
    std::int32_t		id;		// gamelump ID
    std::uint16_t	flags;		// flags
    std::uint16_t	version;	// gamelump version
    std::int32_t		fileofs;	// offset to this gamelump
    std::int32_t		filelen;	// length
};


struct dgamelumpheader_t
{
    std::int32_t lumpCount{};	// number of game lumps
    dgamelump_t* gamelump{};
};

class dnode_t
{
    using type_min_max  = std::array<std::int16_t, 3>;
    using type_children = std::array<std::int32_t, 2>;

public:
    std::int32_t  plane_num;         // 0x00
    type_children children;          // 0x04
    type_min_max  mins;              // 0x0C
    type_min_max  maxs;              // 0x12
    std::uint16_t first_face;        // 0x18
    std::uint16_t num_faces;         // 0x1A
    std::int16_t  area;              // 0x1C
private:
    std::uint8_t  _pad0x1E[ 0x2 ]{}; // 0x1E
};//Size=0x20

class snode_t
{
    using type_min_max  = std::array<std::int16_t, 3>;
    using type_children = std::array<std::int32_t, 2>;

public:
    std::int32_t  plane_num;           // 0x00
    cplane_t*     plane;               // 0x04
    type_children children;            // 0x08
    dleaf_t*      leaf_children;       // 0x10
    snode_t*      node_children;       // 0x14
    type_min_max  mins;                // 0x18
    type_min_max  maxs;                // 0x1E
    std::uint16_t first_face;          // 0x24
    std::uint16_t num_faces;           // 0x26
    std::int16_t  area;                // 0x28
private:
    std::uint8_t  _pad0x2A[ 0x2 ]{};   // 0x2A
};//Size=0x2C

class dface_t
{
    using type_styles = std::array<std::uint8_t, 4>;
    using type_luxels = std::array<std::int32_t, 2>;

public:
    std::uint16_t plane_num;                       // 0x00
    std::uint8_t  side;                            // 0x02
    std::uint8_t  on_node;                         // 0x03
    std::int32_t  first_edge;                      // 0x04
    std::int16_t  num_edges;                       // 0x08
    std::int16_t  tex_info;                        // 0x0A
    std::int16_t  disp_info;                       // 0x0C
    std::int16_t  surface_fog_volume_id;           // 0x0E
    type_styles   styles;                          // 0x10
    std::int32_t  light_offset;                    // 0x18
    float         area;                            // 0x1C
    type_luxels   lightmap_texture_mins_in_luxels; // 0x20
    type_luxels   lightmap_texture_size_in_luxels; // 0x28
    std::int32_t  orig_face;                       // 0x30
    std::uint16_t num_prims;                       // 0x34
    std::uint16_t first_prim_id;                   // 0x36
    std::uint16_t smoothing_groups;                // 0x38
};//Size=0x3A

class dbrush_t
{
public:
    std::int32_t first_side; // 0x0
    std::int32_t num_sides;  // 0x4
    std::int32_t contents;   // 0x8
};//Size=0xC

class dbrushside_t
{
public:
    std::uint16_t plane_num; // 0x0
    std::int16_t  tex_info;  // 0x2
    std::int16_t  disp_info; // 0x4
    std::uint8_t  bevel;     // 0x6
    std::uint8_t  thin;      // 0x7
};//Size=0x8

class texinfo_t
{
    using type_vecs = std::array<math::vector4, 2>;

public:
    type_vecs    texture_vecs;  // 0x00
    type_vecs    lightmap_vecs; // 0x20
    std::int32_t flags;         // 0x40
    std::int32_t tex_data;      // 0x44
};//Size=0x48

class VPlane
{
public:
    VPlane() = default;

    VPlane(
        const math::vector3& origin,
        const float          distance
    )
        : origin( origin )
        , distance( distance )
    { }

    VPlane(
        const VPlane& rhs
    ) = default;

    VPlane(
        VPlane&& rhs
    ) noexcept
    {
        *this = std::move( rhs );
    }

    VPlane& operator = (
        const VPlane& rhs
        ) = default;

    VPlane& operator = (
        VPlane&& rhs
    ) noexcept
    {
        origin   = std::move( rhs.origin );
        distance = rhs.distance;

        rhs.distance = 0.f;

        return *this;
    }

    NODISCARD
    float dist(
        const math::vector3& destination
    ) const
    {
        return origin.dot( destination ) - distance;
    }

    void init(
        const math::vector3& origin,
        const float          distance
    )
    {
        this->origin   = origin;
        this->distance = distance;
    }

public:
    math::vector3 origin   = 0.f;
    float         distance = 0.f;
};

class polygon
{
    using type_surfinfo_vecs = std::array<math::vector3, max_surfinfo_verts>;
    using type_edge_planes   = std::array<VPlane, max_surfinfo_verts>;

public:
    type_surfinfo_vecs verts;
    std::size_t        num_verts = 0;
    VPlane             plane;
    type_edge_planes   edge_planes;
    type_surfinfo_vecs vec2d;
    std::int32_t       skip = 0;
};

struct trace_t
{
    /// <summary>
    /// Determine if a plan is NOT valid
    /// </summary>
    bool          all_solid           = true;
    /// <summary>
    /// Determine if the start point was in a solid area
    /// </summary>
    bool          start_solid         = true;
    /// <summary>
    /// Time completed, 1.0              = didn't hit anything
    /// </summary>
    float         fraction            = 1.f;
    float         fraction_left_solid = 1.f;
    /// <summary>
    /// Final trace position
    /// </summary>
    math::vector3 end_pos;
    std::int32_t  contents            = 0;
    dbrush_t*     brush               = nullptr;
    std::int32_t  num_brush_sides     = 0;

    void clear()
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
