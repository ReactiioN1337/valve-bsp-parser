///---------------------------------------------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2022, ReactiioN
///-- License       MIT
///---------------------------------------------------------------------------------------------------------------------
#pragma once

#include <mkrn/valve/bsp/file-format/cplane.hpp>
#include <mkrn/valve/bsp/file-format/dbrush.hpp>
#include <mkrn/valve/bsp/file-format/dbrushside.hpp>
#include <mkrn/valve/bsp/file-format/dedge.hpp>
#include <mkrn/valve/bsp/file-format/dface.hpp>
#include <mkrn/valve/bsp/file-format/dheader.hpp>
#include <mkrn/valve/bsp/file-format/dleaf.hpp>
#include <mkrn/valve/bsp/file-format/dnode.hpp>
#include <mkrn/valve/bsp/file-format/dplane.hpp>
#include <mkrn/valve/bsp/file-format/entity.hpp>
#include <mkrn/valve/bsp/file-format/lzma.hpp>
#include <mkrn/valve/bsp/file-format/polygon.hpp>
#include <mkrn/valve/bsp/file-format/snode.hpp>
#include <mkrn/valve/bsp/file-format/texinfo.hpp>
#include <mkrn/valve/bsp/file-format/trace.hpp>
#include <mkrn/valve/bsp/file-format/vertex.hpp>
#include <mkrn/valve/bsp/file-format/vplane.hpp>
#include <shared_mutex>
#include <LzmaLib.h>
#include <cstring>
#include <cassert>
#include <mutex>
#include <optional>

namespace mkrn::valve {
class bsp_parser final
{
public:
    bsp_parser() = default;

    ~bsp_parser() = default;

    bsp_parser(
        const bsp_parser& rhs
    ) = delete;

    bsp_parser& operator = (
        const bsp_parser& rhs
    ) = delete;

    bsp_parser(
        bsp_parser&& rhs
    ) noexcept;

    bsp_parser& operator = (
        bsp_parser&& rhs
    ) noexcept;

private:
    bool set_current_map(
        const std::string& directory,
        const std::string& map_name,
        std::string&       file_path
    );


    bool parse_planes(
        std::ifstream& file,
        std::optional<bsp::lump_t::header> lumpFileHeader
    );
    bool parse_entities(
        std::ifstream& file,
        std::optional<bsp::lump_t::header> lumpFileHeader
    );

    bool parse_nodes(
        std::ifstream& file,
        std::optional<bsp::lump_t::header> lumpFileHeader
    );

    bool parse_leaffaces(
        std::ifstream& file,
        std::optional<bsp::lump_t::header> lumpFileHeader
    );

    bool parse_leafbrushes(
        std::ifstream& file,
        std::optional<bsp::lump_t::header> lumpFileHeader
    ); 
    
    //bool parse_entities(
    //    std::ifstream& file
    //);

    bool parse_polygons();

    void ray_cast_node(
        std::int32_t         node_index,
        float                start_fraction,
        float                end_fraction,
        const math::vector3& origin,
        const math::vector3& destination,
        bsp::trace_t*        out
    );

    void ray_cast_surface(
        std::int32_t         surface_index,
        const math::vector3& origin,
        const math::vector3& destination,
        bsp::trace_t*        out
    );

    void ray_cast_brush(
        bsp::dbrush_t* brush,
        const math::vector3&   origin,
        const math::vector3&   destination,
        bsp::trace_t*  out
    )const;

    template<typename type>
    NODISCARD
    bool parse_lump(
        std::ifstream&          file,
        const bsp::lump_t::id   lump_index,
        std::vector<type>&      out,
        std::optional<bsp::lump_t::header> fileLump = std::nullopt
    ) const
    {
        using bsp::lzma_header_t;
        const auto index = core::enum_v(lump_index);


        if (index >= core::enum_v(bsp::lump_t::id::last_index)) {
            return false;
        }

        //TODO: decompress lump here if compressed
        //Default behavior is casting data to underlying types.
        //It doesn't handle compression


        //There are two exceptions though: Game lumps (35) (yes, multiple; compressed individually), and PAK Lump (40) (basically a zip file)


        const auto& lump = bsp_header.lumps[index];

        std::size_t lumpOffset,lumpSize;

        bsp::lump_t::header lumpPatch;
        if (fileLump.has_value()) {
               lumpPatch = fileLump.value();
               lumpOffset = lumpPatch.file_offset;
               lumpSize = lumpPatch.file_size;
        } else {

            lumpOffset = lump.file_offset;
            lumpSize = lump.file_size;
        }

        auto size  = static_cast<std::size_t>( lumpSize ) / sizeof( type );
        //out.resize( size );


        char* tmpData = new char[lumpSize + sizeof(bsp::lzma_header_t) + 1]; //lump.file_size doesn't count size of the header
        file.seekg( lumpOffset );
        //Temporarily make this array. This data may be compressed.
        
        memset(tmpData, 0, lumpSize + sizeof(lzma_header_t) + 1);

        file.read(tmpData, lumpSize);

        lzma_header_t lzma_header;
        memcpy(&lzma_header, tmpData, sizeof(lzma_header));

        
        if (lzma_header.valid()) {
            assert(lump_index != bsp::lump_t::id::game_lump || lump_index != bsp::lump_t::id::pak_file); //Those have special rules regarding compression.



            //This section below was BEFORE if check. This resulted on malloc pulling array size from garbage data
            //without checking if data were actually compressed.


            char* tmpUncompressedData;
            //bool isCompressed = false;

           
            tmpUncompressedData = new char[lzma_header.actual_size + 1];


            size_t lzmaSize = lzma_header.lzma_size,realSize = lzma_header.actual_size;


            LzmaUncompress(reinterpret_cast<unsigned char*>(tmpUncompressedData),
                           &realSize,
                           reinterpret_cast<unsigned char*>(tmpData+sizeof(lzma_header_t)), //point to actual data, not the header
                           &lzmaSize,
                           lzma_header.properties,
                           LZMA_PROPS_SIZE);
            out.resize(static_cast<std::size_t>(lzma_header.actual_size) / sizeof(type));
            out.assign(reinterpret_cast<type*>(tmpUncompressedData),reinterpret_cast<type*>(tmpUncompressedData+realSize));
            delete[] tmpUncompressedData;

        }
        else
        {
            out.resize(size/sizeof(type));
            out.assign(reinterpret_cast<type*>(tmpData),reinterpret_cast<type*>(tmpData+lumpSize));
        }

        


        //file.read( reinterpret_cast<char*>( out.data() ), size * static_cast<std::size_t>( sizeof( type ) ) );
        delete[] tmpData;
        return true;
    }

public:
    bool load_map(
        const std::string& directory,
        const std::string& map_name
    );

    bool is_visible(
        const math::vector3& origin,
        const math::vector3& destination
    );

    void trace_ray(
        const math::vector3& origin,
        const math::vector3& final,
        bsp::trace_t*        out
    );
    void unload_map();




    //TODO: Cannot remove leading underscores as some code relies on it.
public:
    std::string                    map_name;
    bsp::dheader_t                 bsp_header;
    //entities go here
    std::vector<bsp::vertex_t>     vertices;
    std::vector<bsp::cplane_t>     planes;
    std::vector<bsp::dedge_t>      edges;
    std::vector<std::int32_t>      surf_edges;
    std::vector<bsp::dleaf_t>      leaves;
    std::vector<bsp::snode_t>      nodes;
    std::vector<bsp::dface_t>      surfaces;
    std::vector<bsp::texinfo_t>    tex_infos;
    std::vector<bsp::dbrush_t>     brushes;
    std::vector<bsp::dbrushside_t> brush_sides;
    std::vector<std::uint16_t>     leaf_faces;
    std::vector<std::uint16_t>     leaf_brushes;
    std::vector<bsp::polygon>      polygons;
    std::vector<bsp::entity_t>     entities;
private:
    mutable std::shared_timed_mutex  _mutex;
};
}
