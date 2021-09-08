///--------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2020, ReactiioN
///-- License       MIT
///--------------------------------------------------------------------------------
#pragma once

#include <valve-bsp-parser/core/valve_structs.hpp>
#include <shared_mutex>
#include <LzmaLib.h>
#include <cstring>
#include <cassert>
#include <mutex>
#include <optional>

namespace rn {
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
        std::optional<valve::lumpfileheader_t> lumpFileHeader
    );
    bool parse_entities(
        std::ifstream& file,
        std::optional<valve::lumpfileheader_t> lumpFileHeader
    );

    bool parse_nodes(
        std::ifstream& file,
        std::optional<valve::lumpfileheader_t> lumpFileHeader
    );

    bool parse_leaffaces(
        std::ifstream& file,
        std::optional<valve::lumpfileheader_t> lumpFileHeader
    );

    bool parse_leafbrushes(
        std::ifstream& file,
        std::optional<valve::lumpfileheader_t> lumpFileHeader
    ); 
    
    //bool parse_entities(
    //    std::ifstream& file
    //);

    bool parse_polygons();

    void ray_cast_node(
        std::int32_t    node_index,
        float           start_fraction,
        float           end_fraction,
        const vector3&  origin,
        const vector3&  destination,
        valve::trace_t* out
    );

    void ray_cast_surface(
        std::int32_t    surface_index,
        const vector3&  origin,
        const vector3&  destination,
        valve::trace_t* out
    );

    void ray_cast_brush(
        valve::dbrush_t* brush,
        const vector3&   origin,
        const vector3&   destination,
        valve::trace_t*  out
    )const;

    template<typename type>
    NODISCARD
    bool parse_lump(
        std::ifstream&          file,
        const valve::lump_index lump_index,
        std::vector<type>&      out,
        std::optional<valve::lumpfileheader_t> fileLump = std::nullopt
    ) const
    {
        using rn::valve::lzma_header_t;
        using rn::valve::has_valid_lzma_ident;
        const auto index = static_cast<std::underlying_type_t<valve::lump_index>>( lump_index );



        if( index >= bsp_header.lumps.size() ) {
            return false;
        }
        //TODO: decompress lump here if compressed
        //Default behavior is casting data to underlying types.
        //It doesn't handle compression


        //There are two exceptions though: Game lumps (35) (yes, multiple; compressed individually), and PAK Lump (40) (basically a zip file)


        const auto& lump = bsp_header.lumps.at( index );

        std::size_t lumpOffset,lumpSize;

        valve::lumpfileheader_t lumpPatch;
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

        
        file.seekg( lumpOffset );
        //Temporarily make this array. This data may be compressed.
        char* tmpData = new char[lumpSize +sizeof(lzma_header_t) + 1]; //lump.file_size doesn't count size of the header


        char* tmpUncompressedData;
        //bool isCompressed = false;
        memset(tmpData, 0, lumpSize +sizeof(lzma_header_t) + 1);

        file.read(tmpData, lumpSize);

        lzma_header_t lzma_header;

        memcpy(&lzma_header, tmpData, sizeof(lzma_header));
        tmpUncompressedData = new char[lzma_header.actualSize+1];

        if (has_valid_lzma_ident(lzma_header.id))
        {
            assert(lump_index != valve::lump_index::game_lump || lump_index != valve::lump_index::pak_file); //Those have special rules regarding compression.

            size_t lzmaSize = lzma_header.lzmaSize,realSize = lzma_header.actualSize;


            LzmaUncompress(reinterpret_cast<unsigned char*>(tmpUncompressedData),
                           &realSize,
                           reinterpret_cast<unsigned char*>(tmpData+sizeof(lzma_header_t)), //point to actual data, not the header
                           &lzmaSize,
                           reinterpret_cast<unsigned char*>(lzma_header.properties.data()),
                           LZMA_PROPS_SIZE);
            out.resize(static_cast<std::size_t>(lzma_header.actualSize) / sizeof(type));
            out.assign(reinterpret_cast<type*>(tmpUncompressedData),reinterpret_cast<type*>(tmpUncompressedData+realSize));

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
        const vector3& origin,
        const vector3& destination
    );

    void trace_ray(
        const vector3&  origin,
        const vector3&  final,
        valve::trace_t* out
    );




    //TODO: Cannot remove leading underscores as some code relies on it.
public:
    std::string                      map_name;
    valve::dheader_t                 bsp_header;
    //entities go here
    std::vector<valve::mvertex_t>    vertices;
    std::vector<valve::cplane_t>     planes;
    std::vector<valve::dedge_t>      edges;
    std::vector<std::int32_t>        surf_edges;
    std::vector<valve::dleaf_t>      leaves;
    std::vector<valve::snode_t>      nodes;
    std::vector<valve::dface_t>      surfaces;
    std::vector<valve::texinfo_t>    tex_infos;
    std::vector<valve::dbrush_t>     brushes;
    std::vector<valve::dbrushside_t> brush_sides;
    std::vector<std::uint16_t>       leaf_faces;
    std::vector<std::uint16_t>       leaf_brushes;
    std::vector<valve::polygon>      polygons;
    std::vector<valve::entity_t>     entities;
private:
    mutable std::shared_timed_mutex  _mutex;
};
}
