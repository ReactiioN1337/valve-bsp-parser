///---------------------------------------------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2022, ReactiioN
///-- License       MIT
///---------------------------------------------------------------------------------------------------------------------
#pragma once

#include <mkrn/valve/bsp/file-format/valve_structs.hpp>
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
        std::optional<bsp::lumpfileheader_t> lumpFileHeader
    );
    bool parse_entities(
        std::ifstream& file,
        std::optional<bsp::lumpfileheader_t> lumpFileHeader
    );

    bool parse_nodes(
        std::ifstream& file,
        std::optional<bsp::lumpfileheader_t> lumpFileHeader
    );

    bool parse_leaffaces(
        std::ifstream& file,
        std::optional<bsp::lumpfileheader_t> lumpFileHeader
    );

    bool parse_leafbrushes(
        std::ifstream& file,
        std::optional<bsp::lumpfileheader_t> lumpFileHeader
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
        const bsp::lump_index lump_index,
        std::vector<type>&      out,
        std::optional<bsp::lumpfileheader_t> fileLump = std::nullopt
    ) const
    {
        using bsp::lzma_header_t;
        using bsp::has_valid_lzma_ident;
        const auto index = static_cast<std::underlying_type_t<bsp::lump_index>>( lump_index );



        if( index >= bsp_header.lumps.size() ) {
            return false;
        }
        //TODO: decompress lump here if compressed
        //Default behavior is casting data to underlying types.
        //It doesn't handle compression


        //There are two exceptions though: Game lumps (35) (yes, multiple; compressed individually), and PAK Lump (40) (basically a zip file)


        const auto& lump = bsp_header.lumps.at( index );

        std::size_t lumpOffset,lumpSize;

        bsp::lumpfileheader_t lumpPatch;
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


        char* tmpData = new char[lumpSize + sizeof(lzma_header_t) + 1]; //lump.file_size doesn't count size of the header
        file.seekg( lumpOffset );
        //Temporarily make this array. This data may be compressed.
        
        memset(tmpData, 0, lumpSize + sizeof(lzma_header_t) + 1);

        file.read(tmpData, lumpSize);

        lzma_header_t lzma_header;
        memcpy(&lzma_header, tmpData, sizeof(lzma_header));


        if (has_valid_lzma_ident(lzma_header.id))
        {
            assert(lump_index != bsp::lump_index::game_lump || lump_index != bsp::lump_index::pak_file); //Those have special rules regarding compression.



            //This section below was BEFORE if check. This resulted on malloc pulling array size from garbage data
            //without checking if data were actually compressed.


            char* tmpUncompressedData;
            //bool isCompressed = false;

           
            tmpUncompressedData = new char[lzma_header.actualSize + 1];


            size_t lzmaSize = lzma_header.lzmaSize,realSize = lzma_header.actualSize;


            LzmaUncompress(reinterpret_cast<unsigned char*>(tmpUncompressedData),
                           &realSize,
                           reinterpret_cast<unsigned char*>(tmpData+sizeof(lzma_header_t)), //point to actual data, not the header
                           &lzmaSize,
                           reinterpret_cast<unsigned char*>(lzma_header.properties.data()),
                           LZMA_PROPS_SIZE);
            out.resize(static_cast<std::size_t>(lzma_header.actualSize) / sizeof(type));
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
    std::vector<bsp::mvertex_t>    vertices;
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
