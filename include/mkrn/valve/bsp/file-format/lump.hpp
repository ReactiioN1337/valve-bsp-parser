#pragma once

#include <mkrn/valve/bsp/file-format/constants.hpp>

namespace mkrn::valve::bsp {
#if !defined(_MSC_VER)
#   pragma pack(push, 4)
#endif

/// <remarks>
/// https://developer.valvesoftware.com/wiki/Source_BSP_File_Format#Lighting
/// </remarks>
struct lump_t
{
    struct header
    {
        /// <summary>
        /// 0x00: offset in the file where the lump data begins (should be 0x14)
        /// </summary>
        std::int32_t file_offset{};

        /// <summary>
        /// 0x04: the lump ID according to the lump table
        /// </summary>
        std::int32_t lumpID{};

        /// <summary>
        /// 0x08: same as "version" in lump_t
        /// </summary>
        std::int32_t version{};

        /// <summary>
        /// 0x0C: same as "filelen" in lump_t
        /// </summary>
        std::int32_t file_size{};

        /// <summary>
        /// 0x10: same as in dheader_t
        /// </summary>
        std::int32_t map_revidion{};
    };

    enum class id : std::size_t
    {
        /// <summary>
        /// Index 1, Source 2004, Map entities
        /// </summary>
        entities,

        /// <summary>
        /// Index 1, Source 2004, Plane array
        /// </summary>
        planes,

        /// <summary>
        /// Index 2, Source 2004, Index to texture names
        /// </summary>
        tex_data,

        /// <summary>
        /// Index 3, Source 2004, Vertex array
        /// </summary>
        vertices,

        /// <summary>
        /// Index 4, Source 2004, Compressed visibility bit arrays
        /// </summary>
        visibility,

        /// <summary>
        /// Index 5, Source 2004, BSP tree nodes
        /// </summary>
        nodes,

        /// <summary>
        /// Index 6, Source 2004, Face texture array
        /// </summary>
        tex_info,

        /// <summary>
        /// Index 7, Source 2004, Face array
        /// </summary>
        faces,

        /// <summary>
        /// Index 8, Source 2004, Lightmap samples
        /// </summary>
        lighting,

        /// <summary>
        /// Index 9, Source 2004, Occlusion polygons and vertices
        /// </summary>
        occlusion,

        /// <summary>
        /// Index 10, Source 2004, BSP tree leaf nodes
        /// </summary>
        leafs,

        /// <summary>
        /// Index 11, Source 2007, Correlates between dfaces and Hammer face IDs.
        /// Also used as random seed for detail prop placement.
        ///
        /// https://developer.valvesoftware.com/wiki/Detail_props
        /// </summary>
        face_ids,

        /// <summary>
        /// Index 12, Source 2004, Edge array
        /// </summary>
        edges,

        /// <summary>
        /// Index 13, Source 2004, Index of edges
        /// </summary>
        surfedges,

        /// <summary>
        /// Index 14, Source 2004, Brush models (geometry of brush entities)
        /// </summary>
        models,

        /// <summary>
        /// Index 15, Source 2004, Internal world lights converted from the entity lump
        /// </summary>
        world_lights,

        /// <summary>
        /// Index 16, Source 2004, Index to faces in each leaf
        /// </summary>
        leaf_faces,

        /// <summary>
        /// Index 17, Source 2004, Index to brushes in each leaf
        /// </summary>
        leaf_brushes,

        /// <summary>
        /// Index 18, Source 2004, Brush array
        /// </summary>
        brushes,

        /// <summary>
        /// Index 19, Source 2004, Brushside array
        /// </summary>
        brush_sides,

        /// <summary>
        /// Index 20, Source 2004, Area array
        /// </summary>
        areas,

        /// <summary>
        /// Index 21, Source 2004, Portals between areas
        /// </summary>
        area_portals,

        /// <summary>
        /// Index 22, Source 2004, Polygons defining the boundary between adjacent leaves?
        /// </summary>
        portals,

        /// <summary>
        /// Index 22, Source 2007, Unused
        /// </summary>
        unused0 = portals,

        /// <summary>
        /// Index 22, Source 2009, Static props convex hull lists
        /// </summary>
        prop_collision = portals,

        /// <summary>
        /// Index 23, Source 2004, Leaves that are enterable by the player
        /// </summary>
        clusters,

        /// <summary>
        /// Index 23, Source 2007, Unused
        /// </summary>
        unused1 = clusters,

        /// <summary>
        /// Index 23, Source 2009, Static prop convex hulls
        /// </summary>
        prop_hulls = clusters,

        /// <summary>
        /// Index 24, Source 2004, Vertices of portal polygons
        /// </summary>
        portal_verts,

        /// <summary>
        /// Index 24, Source 2007, Unused
        /// </summary>
        unused2 = portal_verts,

        /// <summary>
        /// Index 24, Source 2009, Static prop collision vertices
        /// </summary>
        prop_hull_verts = portal_verts,

        /// <summary>
        /// Index 25, Source 2004, Polygons defining the boundary between adjacent clusters?
        /// </summary>
        cluster_portals,

        /// <summary>
        /// Index 25, Source 2007, Unused
        /// </summary>
        unused3 = cluster_portals,

        /// <summary>
        /// Index 25, Source 2009, Static prop per hull triangle index start/count
        /// </summary>
        prop_tris = cluster_portals,

        /// <summary>
        /// Index 26, Source 2004, Displacement surface array
        /// </summary>
        disp_info,

        /// <summary>
        /// Index 27, Source 2004, Brush faces array before splitting
        /// </summary>
        original_faces,

        /// <summary>
        /// Index 28, Source 2007, Displacement physics collision data
        /// </summary>
        phys_disp,

        /// <summary>
        /// Index 29, Source 2004, Physics collision data
        /// </summary>
        phys_collide,

        /// <summary>
        /// Index 30, Source 2004, Face plane normals
        /// </summary>
        vert_normals,

        /// <summary>
        /// Index 31, Source 2004, Face plane normal index array
        /// </summary>
        vert_normal_indices,

        /// <summary>
        /// Index 32, Source 2004, Displacement lightmap alphas (unused/empty since Source 2006)
        /// </summary>
        lightmap_alphas,

        /// <summary>
        /// Index 33, Source 2004, Vertices of displacement surface meshes
        /// </summary>
        disp_verts,

        /// <summary>
        /// Index 34, Source 2004, Displacement lightmap sample positions
        /// </summary>
        disp_lightmap_sample_positions,

        /// <summary>
        /// Index 35, Source 2004, Game-specific data lump
        /// </summary>
        game_lump,

        /// <summary>
        /// Index 36, Source 2004, Data for leaf nodes that are inside water
        /// </summary>
        leafwaterdata,

        /// <summary>
        /// Index 37, Source 2004, Water polygon data
        /// </summary>
        primitives,

        /// <summary>
        /// Index 38, Source 2004, Water polygon vertices
        /// </summary>
        prim_vertices,

        /// <summary>
        /// Index 39, Source 2004, Water polygon vertex index array
        /// </summary>
        prim_indices,

        /// <summary>
        /// Index 40, Source 2004, Embedded uncompressed Zip-format file
        /// </summary>
        pak_file,

        /// <summary>
        /// Index 41, Source 2004, Clipped portal polygon vertices
        /// </summary>
        clipportal_vertices,

        /// <summary>
        /// Index 42, Source 2004, env_cubemap location array
        /// </summary>
        cubemaps,

        /// <summary>
        /// Index 43, Source 2004, Texture name data
        /// </summary>
        texdata_string_data,

        /// <summary>
        /// Index 44, Source 2004, Index array into texdata string data
        /// </summary>
        texdata_string_table,

        /// <summary>
        /// Index 45, Source 2004, info_overlay data array
        /// </summary>
        overlays,

        /// <summary>
        /// Index 46, Source 2004, Distance from leaves to water
        /// </summary>
        leaf_min_dist_to_water,

        /// <summary>
        /// Index 47, Source 2004, Macro texture info for faces
        /// </summary>
        face_macro_texture_info,

        /// <summary>
        /// Index 48, Source 2004, Displacement surface triangles
        /// </summary>
        disp_tris,

        /// <summary>
        /// Index 49, Source 2004, Compressed win32-specific Havok terrain surface collision data.
        /// Deprecated and no longer used.
        /// </summary>
        phys_collide_surface,

        /// <summary>
        /// Index 49, Source 2009, Static prop triangle and string data
        /// </summary>
        prop_blob = phys_collide_surface,

        /// <summary>
        /// Index 50, Source 2006, info_overlay's on water faces?
        /// </summary>
        water_overlays,

        /// <summary>
        /// Index 51, Source 2006, Alternate lightdata implementation for Xbox
        /// </summary>
        lightmap_pages,

        /// <summary>
        /// Index 51, Source 2007, Index of LUMP_LEAF_AMBIENT_LIGHTING_HDR
        /// </summary>
        leaf_ambient_index_hdr = lightmap_pages,

        /// <summary>
        /// Index 52, Source 2006, Alternate lightdata indices for Xbox
        /// </summary>
        lightmap_pageinfos,

        /// <summary>
        /// Index 52, Source 2007, Index of LUMP_LEAF_AMBIENT_LIGHTING
        /// </summary>
        leaf_ambient_index = lightmap_pageinfos,

        /// <summary>
        /// Index 53, Source 2006, HDR lightmap samples
        /// </summary>
        lighting_hdr,

        /// <summary>
        /// Index 54, Source 2006, Internal HDR world lights converted from the entity lump
        /// </summary>
        worldights_hdr,

        /// <summary>
        /// Index 55, Source 2006, Per-leaf ambient light samples (HDR)
        /// </summary>
        leaf_ambient_lighting_hdr,

        /// <summary>
        /// Index 56, Source 2006, Per-leaf ambient light samples (LDR)
        /// </summary>
        leaf_ambient_lighting,

        /// <summary>
        /// Index 57, Source 2006, XZip version of pak file for Xbox. Deprecated.
        /// </summary>
        xzip_pak_file,

        /// <summary>
        /// Index 58, Source 2006, HDR maps may have different face data
        /// </summary>
        faces_hdr,

        /// <summary>
        /// Index 59, Source 2006, Extended level-wide flags. Not present in all levels.
        /// </summary>
        map_flags,

        /// <summary>
        /// Index 60, Source 2007, Fade distances for overlays
        /// </summary>
        overlay_fades,

        /// <summary>
        /// Index 61, Source 2008,  System level settings (min/max CPU & GPU to render this overlay)
        /// </summary>
        overlay_system_levels,

        /// <summary>
        /// Index 62, Source 2009
        /// </summary>
        physlevel,

        /// <summary>
        /// Index 63, Source 2010, Displacement multiblend info
        /// </summary>
        disp_multiblend,
        last_index,

    };

    std::int32_t file_offset{}; // 0x0
    std::int32_t file_size{};   // 0x4
    std::int32_t version{};     // 0x8
    byte4        ident_code{};  // 0xC

    NODISCARD
    constexpr
    auto
    get_ident_code() const noexcept -> std::int32_t
    {
        return ident_code.i32;
    }
};

#if !defined(_MSC_VER)
#   pragma pack(pop)
#endif

static_assert(sizeof(lump_t::header) == 0x14);
static_assert(sizeof(lump_t) == 0x10);
}
