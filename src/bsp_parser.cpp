///--------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2020, ReactiioN
///-- License       MIT
///--------------------------------------------------------------------------------
#include <valve-bsp-parser/bsp_parser.hpp>
#include <filesystem>
#include <cstring>
#include <cfloat>
#include <regex>

using namespace rn;


//TODO: handle compressed lumps, and standalone lump files
//TODO: Lump 0 parser
bsp_parser::bsp_parser(
    bsp_parser&& rhs
) noexcept
{
    *this = std::move( rhs );
}

bsp_parser& bsp_parser::operator = (
    bsp_parser&& rhs
) noexcept
{
    std::unique_lock<std::shared_timed_mutex> lock( rhs._mutex );

    bsp_header = rhs.bsp_header;
    std::memset( &rhs.bsp_header, 0, sizeof( valve::dheader_t ) );

    vertices     = std::move( rhs.vertices );
    planes       = std::move( rhs.planes );
    edges        = std::move( rhs.edges );
    surf_edges   = std::move( rhs.surf_edges );
    leaves       = std::move( rhs.leaves );
    nodes        = std::move( rhs.nodes );
    surfaces     = std::move( rhs.surfaces );
    tex_infos    = std::move( rhs.tex_infos );
    brushes      = std::move( rhs.brushes );
    brush_sides  = std::move( rhs.brush_sides );
    leaf_faces   = std::move( rhs.leaf_faces );
    leaf_brushes = std::move( rhs.leaf_brushes );
    polygons     = std::move( rhs.polygons );
    entities     = std::move( rhs.entities );

    return *this;
}

bool bsp_parser::set_current_map(
    const std::string& directory,
    const std::string& map_name,
    std::string&       file_path
)
{
    static auto fix_seperators = []( const std::string& input )
    {
        // convert seperators from DOS to UNIX
        return std::filesystem::path( input ).generic_string();
    };

    if( directory.empty() || map_name.empty() ) {
        return false;
    }

    file_path = fix_seperators( directory )
        .append( "/" )
        .append( fix_seperators( map_name ) );

    this->map_name = map_name;

#if defined(RN_BSP_PARSER_MESSAGES)
    std::printf( "[+] Loading map: %s ...\n", map_name.data() );
#endif

    return true;
}

bool bsp_parser::parse_planes(
    std::ifstream& file,
    std::optional<valve::lumpfileheader_t> lumpFileHeader = std::nullopt
)
{
    std::vector<valve::dplane_t> planes;
    if( !parse_lump( file, valve::lump_index::planes, planes ,lumpFileHeader) ) {
        return false;
    }

    this->planes.resize( planes.size() );

    for( std::size_t i = 0; i < planes.size(); ++i ) {
        auto& out      = this->planes.at( i );
        const auto& in = planes.at( i );

        auto plane_bits = 0;
        for( std::size_t j = 0; j < 3; ++j ) {
            out.normal( j ) = in.normal( j );
            if( out.normal( j ) < 0.f ) {
                plane_bits |= 1 << static_cast<std::int32_t>( j );
            }
        }

        out.distance  = in.distance;
        out.type      = static_cast<std::uint8_t>( in.type );
        out.sign_bits = static_cast<std::uint8_t>( plane_bits );
    }

    return true;
}

bool bsp_parser::parse_entities(std::ifstream &file, std::optional<valve::lumpfileheader_t> lumpFileHeader=std::nullopt)
{

    std::vector<char> entitiesRawBuffer;

    if (!parse_lump(file,valve::lump_index::entities,entitiesRawBuffer,lumpFileHeader)) {
        return false;
    }

    std::string entitiesLump(entitiesRawBuffer.begin(),entitiesRawBuffer.end());

    //clean old entities
    entities.clear();


    std::istringstream entStream{entitiesLump};


    valve::entity_t entity;
    std::string line;
    std::smatch kvMatch;

    try {

    std::regex entKVRegex("\"(.*?)\" \"(.*?)\"");

    while (std::getline(entStream,line)) {
        if (line=="{") {
            entity = *new valve::entity_t();
        } else if (line=="}") {
            entities.push_back(entity);
        } else if (std::regex_match(line,kvMatch,entKVRegex)) {
            if (kvMatch.size() >=3) {
                entity.keyvalues.emplace(kvMatch[1],kvMatch[2]);
            }
        }
    }

    }  catch (std::regex_error &e) {
        std::printf("Regex error: %s",e.what());
    }
    return true;
}

bool bsp_parser::parse_nodes(
    std::ifstream& file,
        std::optional<valve::lumpfileheader_t> lumpFileHeader = std::nullopt
)
{
    std::vector<valve::dnode_t> nodes;
    if( !parse_lump( file, valve::lump_index::nodes, nodes ,lumpFileHeader) ) {
        return false;
    }

    const auto num_nodes = nodes.size();
    this->nodes.resize( num_nodes );

    for( std::size_t i = 0; i < num_nodes; ++i ) {
        const auto& in = nodes.at( i );
        auto& out      = this->nodes.at( i );

        out.mins       = in.mins;
        out.maxs       = in.maxs;
        out.plane_num  = in.plane_num;
        out.plane      = planes.data() + in.plane_num;
        out.first_face = in.first_face;
        out.num_faces  = in.num_faces;

        for( std::size_t j = 0; j < 2; ++j ) {
            const auto child_index = in.children.at( j );
            out.children.at( j ) = child_index;

            if( child_index >= 0 ) {
                out.leaf_children = nullptr;
                out.node_children = this->nodes.data() + child_index;
            }
            else {
                out.leaf_children = this->leaves.data() + static_cast<std::ptrdiff_t>( -1 - child_index );
                out.node_children = nullptr;
            }
        }
    }

    return true;
}

bool bsp_parser::parse_leaffaces(
    std::ifstream& file,
        std::optional<valve::lumpfileheader_t> lumpFileHeader = std::nullopt
)
{
    if( !parse_lump( file, valve::lump_index::leaf_faces, leaf_faces ,lumpFileHeader) ) {
        return false;
    }

    const auto num_leaffaces = leaf_faces.size();
    if( num_leaffaces > valve::MAX_MAP_LEAFBRUSHES ) {
        printf( "[!] map has to many leaffaces, parsed more than required...\n" );
    }
    else if( !num_leaffaces ) {
        printf( "[!] map has no leaffaces to parse...\n" );
    }

    return true;
}

bool bsp_parser::parse_leafbrushes(
    std::ifstream& file,
        std::optional<valve::lumpfileheader_t> lumpFileHeader = std::nullopt
)
{
    if( !parse_lump( file, valve::lump_index::leaf_brushes, leaf_brushes,lumpFileHeader) ) {
        return false;
    }

    const auto num_leaffaces = leaf_faces.size();
    if( num_leaffaces > valve::MAX_MAP_LEAFBRUSHES ) {
        printf( "[!] map has to many leafbrushes, parsed more than required...\n" );
    }
    else if( !num_leaffaces ) {
        printf( "[!] map has no leafbrushes to parse...\n" );
    }

    return true;
}

bool bsp_parser::parse_polygons()
{
    polygons.resize( surfaces.size() );

    for( const auto& surface : surfaces ) {
        const auto& first_edge = surface.first_edge;
        const auto& num_edges  = surface.num_edges;

        if( num_edges < 3 || static_cast<size_t>( num_edges ) > valve::MAX_SURFINFO_VERTS ) {
            continue;
        }
        if( surface.tex_info <= 0 ) {
            continue;
        }

        valve::polygon polygon;
        vector3 edge;

        for( auto i = 0; i < num_edges; ++i ) {
            const auto edge_index = surf_edges.at( first_edge + i );
            if( edge_index >= 0 ) {
                edge = vertices.at( edges[ edge_index ].v.at( 0 ) ).position;
            }
            else {
                edge = vertices.at( edges[ -edge_index ].v.at( 1 ) ).position;
            }
            polygon.verts.at( i ) = edge;
        }

        polygon.num_verts      = static_cast<std::size_t>( num_edges );
        polygon.plane.origin   = planes.at( surface.plane_num ).normal;
        polygon.plane.distance = planes.at( surface.plane_num ).distance;
        polygons.push_back( polygon );
    }

    return true;
}

void bsp_parser::ray_cast_node(
    const std::int32_t node_index,
    const float        start_fraction,
    const float        end_fraction,
    const vector3&     origin,
    const vector3&     destination,
    valve::trace_t*    out
)
{
    if( out->fraction <= start_fraction ) {
        return;
    }

    if( node_index < 0 ) {
        auto* leaf = &leaves.at( static_cast<std::size_t>( -node_index - 1 ) );
        for( std::uint16_t i = 0; i < leaf->num_leafbrushes; ++i ) {

            const auto brush_index = static_cast<std::int32_t>( leaf_brushes.at( leaf->first_leafbrush + i ) );
            auto* brush            = &brushes.at( brush_index );
            if( !brush || !( brush->contents & valve::MASK_SHOT_HULL ) ) {
                continue;
            }

            ray_cast_brush( brush, origin, destination, out );
            if( out->fraction == 0.f ) {
                return;
            }

            out->brush = brush;
        }
        if( out->start_solid || out->fraction < 1.f ) {
            return;
        }
        for( std::uint16_t i = 0; i < leaf->num_leaffaces; ++i ) {
            ray_cast_surface( static_cast<std::int32_t>( leaf_faces.at( leaf->first_leafface + i ) ), origin, destination, out );
        }
        return;
    }

    auto* node = &nodes.at( static_cast<std::size_t>( node_index ) );
    if( !node ) {
        return;
    }
    auto* plane = node->plane;
    if( !plane ) {
        return;
    }

    float start_distance, end_distance;

    if( plane->type < 3 ) {
        start_distance = origin( static_cast<std::size_t>( plane->type ) ) - plane->distance;
        end_distance   = destination( static_cast<std::size_t>( plane->type ) ) - plane->distance;
    }
    else {
        start_distance = origin.dot( plane->normal ) - plane->distance;
        end_distance = destination.dot( plane->normal ) - plane->distance;
    }

    if( start_distance >= 0.f && end_distance >= 0.f ) {
        ray_cast_node( node->children.at( 0 ), start_fraction, end_fraction, origin, destination, out );
    }
    else if( start_distance < 0.f && end_distance < 0.f ) {
        ray_cast_node( node->children.at( 1 ), start_fraction, end_fraction, origin, destination, out );
    }
    else {
        std::int32_t side_id;
        float fraction_first, fraction_second;
        vector3 middle;

        if( start_distance < end_distance ) {
            /// Back
            side_id = 1;
            const auto inversed_distance = 1.f / ( start_distance - end_distance );

            fraction_first = ( start_distance + FLT_EPSILON ) * inversed_distance;
            fraction_second = ( start_distance + FLT_EPSILON ) * inversed_distance;
        }
        else if( end_distance < start_distance ) {
            /// Front
            side_id = 0;
            const auto inversed_distance = 1.0f / ( start_distance - end_distance );

            fraction_first = ( start_distance + FLT_EPSILON ) * inversed_distance;
            fraction_second = ( start_distance - FLT_EPSILON ) * inversed_distance;
        }
        else {
            /// Front
            side_id = 0;
            fraction_first = 1.f;
            fraction_second = 0.f;
        }
        if( fraction_first < 0.f ) {
            fraction_first = 0.f;
        }
        else if( fraction_first > 1.f ) {
            fraction_first = 1.f;
        }
        if( fraction_second < 0.f ) {
            fraction_second = 0.f;
        }
        else if( fraction_second > 1.f ) {
            fraction_second = 1.f;
        }

        auto fraction_middle = start_fraction + ( end_fraction - start_fraction ) * fraction_first;
        for( std::size_t i = 0; i < 3; i++ ) {
            middle( i ) = origin( i ) + fraction_first * ( destination( i ) - origin( i ) );
        }

        ray_cast_node( node->children.at( side_id ), start_fraction, fraction_middle, origin, middle, out );
        fraction_middle = start_fraction + ( end_fraction - start_fraction ) * fraction_second;
        for( std::size_t i = 0; i < 3; i++ ) {
            middle( i ) = origin( i ) + fraction_second * ( destination( i ) - origin( i ) );
        }

        ray_cast_node( node->children.at( !side_id ), fraction_middle, end_fraction, middle, destination, out );
    }
}

void bsp_parser::ray_cast_brush(
    valve::dbrush_t* brush,
    const vector3&   origin,
    const vector3&   destination,
    valve::trace_t*  out
) const
{
    if( brush->num_sides ) {
        auto fraction_to_enter = -99.f;
        auto fraction_to_leave = 1.f;
        auto starts_out = false;
        auto ends_out = false;
        for( auto i = 0; i < brush->num_sides; ++i ) {
            auto const* brush_side = &brush_sides.at( brush->first_side + i );
            if( !brush_side || brush_side->bevel ) {
                continue;
            }

            auto const* plane = &planes.at( brush_side->plane_num );
            if( !plane ) {
                continue;
            }

            const auto start_distance = origin.dot( plane->normal ) - plane->distance;
            const auto end_distance = destination.dot( plane->normal ) - plane->distance;
            if( start_distance > 0.f ) {
                starts_out = true;
                if( end_distance > 0.f ) {
                    return;
                }
            }
            else {
                if( end_distance <= 0.f ) {
                    continue;
                }
                ends_out = true;
            }
            if( start_distance > end_distance ) {
                auto fraction = std::max( ( start_distance - valve::DIST_EPSILON ), 0.f );
                fraction = fraction / ( start_distance - end_distance );
                if( fraction > fraction_to_enter ) {
                    fraction_to_enter = fraction;
                }
            }
            else {
                const auto fraction = ( start_distance + valve::DIST_EPSILON ) / ( start_distance - end_distance );
                if( fraction < fraction_to_leave ) {
                    fraction_to_leave = fraction;
                }
            }
        }

        if( starts_out ) {
            if( out->fraction_left_solid - fraction_to_enter > 0.f ) {
                starts_out = false;
            }
        }

        out->num_brush_sides = brush->num_sides;

        if( !starts_out ) {
            out->start_solid = true;
            out->contents = brush->contents;

            if( !ends_out ) {
                out->all_solid = true;
                out->fraction = 0.f;
                out->fraction_left_solid = 1.f;
            }
            else {
                if( fraction_to_leave != 1.f && fraction_to_leave > out->fraction_left_solid ) {
                    out->fraction_left_solid = fraction_to_leave;
                    if( out->fraction <= fraction_to_leave ) {
                        out->fraction = 1.f;
                    }
                }
            }
            return;
        }

        if( fraction_to_enter < fraction_to_leave ) {
            if( fraction_to_enter > -99.f && fraction_to_enter < out->fraction ) {
                if( fraction_to_enter < 0.f ) {
                    fraction_to_enter = 0.f;
                }

                out->fraction = fraction_to_enter;
                out->brush    = brush;
                out->contents = brush->contents;
            }
        }
    }
}

void bsp_parser::ray_cast_surface(
    const std::int32_t surface_index,
    const vector3&     origin,
    const vector3&     destination,
    valve::trace_t*    out
)
{
    const auto index = static_cast<std::size_t>( surface_index );
    if( index >= polygons.size() ) {
        return;
    }

    auto* polygon   = &polygons.at( index );
    auto* plane     = &polygon->plane;
    const auto dot1 = plane->dist( origin );
    const auto dot2 = plane->dist( destination );

    if( dot1 > 0.f != dot2 > 0.f ) {
        if( dot1 - dot2 < valve::DIST_EPSILON ) {
            return;
        }

        const auto t = dot1 / ( dot1 - dot2 );
        if( t <= 0 ) {
            return;
        }

        std::size_t i = 0;
        const auto intersection = origin + ( destination - origin ) * t;
        for( ; i < polygon->num_verts; ++i ) {
            auto* edge_plane = &polygon->edge_planes.at( i );
            if( edge_plane->origin.is_zero() ) {
                edge_plane->origin = plane->origin - ( polygon->verts.at( i ) - polygon->verts.at( ( i + 1 ) % polygon->num_verts ) );
                edge_plane->origin.normalize();
                edge_plane->distance = edge_plane->origin.dot( polygon->verts.at( i ) );
            }
            if( edge_plane->dist( intersection ) < 0.0f ) {
                break;
            }
        }
        if( i == polygon->num_verts ) {
            out->fraction = 0.2f;
            out->end_pos = intersection;
        }
    }
}

bool bsp_parser::load_map(
    const std::string& directory,
    const std::string& map_name
)
{
    if( map_name == this->map_name ) {
        return true;
    }

    std::unique_lock<std::shared_timed_mutex> lock( _mutex );

    std::string file_path;
    if( !set_current_map( directory, map_name, file_path ) ) {
        return false;
    }

    std::ifstream file( file_path, std::ios_base::binary );
    if( !file ) {
    #if defined(RN_BSP_PARSER_MESSAGES)
        std::printf( "[!] failed to open file: %s\n", file_path.data() );
    #endif
        return false;
    }

    try {
        file.read( reinterpret_cast<char*>( &bsp_header ), sizeof( bsp_header ) );
    #if defined(RN_BSP_PARSER_MESSAGES)
        if( _bsp_header.m_Version < valve::BSPVERSION  ) {
            std::printf( "[!] unknown BSP version (%d), trying to parse it anyway...\n", _bsp_header.m_Version );
        }
    #endif
        if( !valve::has_valid_bsp_ident( bsp_header.ident ) ) {
    #if defined(RN_BSP_PARSER_MESSAGES)
            std::printf( "[!] %s isn't a (valid) .bsp file!\n", map_name.data() );
    #endif
            return false;
        }

        bool baseMapParsed =  parse_lump( file, valve::lump_index::vertices, vertices )
            && parse_planes( file )
            && parse_lump( file, valve::lump_index::edges, edges )
            && parse_lump( file, valve::lump_index::surfedges, surf_edges )
            && parse_lump( file, valve::lump_index::leafs, leaves )
            && parse_nodes( file )
            && parse_lump( file, valve::lump_index::faces, surfaces )
            && parse_lump( file, valve::lump_index::tex_info, tex_infos )
            && parse_lump( file, valve::lump_index::brushes, brushes )
            && parse_lump( file, valve::lump_index::brush_sides, brush_sides )
            && parse_leaffaces( file )
            && parse_leafbrushes( file )
            && parse_entities(file)
            && parse_polygons()
        ;
        if (!baseMapParsed)
            return false;

        // TODO add standalone lump files.

        //first strip bsp and add lmp extension

        std::string lmpFormattedPath = file_path.substr(0,file_path.size()-4); //remove .bsp


        lmpFormattedPath.append("_l_");


        for (int i=0;;i++){
            std::string thisLmpFile = lmpFormattedPath + std::to_string(i)+".lmp";

            std::ifstream file(thisLmpFile, std::ios_base::binary); // file of lumpPatch 0
            if (!file.good()) {
                break; // file likely not present. We're finished
            }

            //Alright. File present. Parse the header of the file.
            rn::valve::lumpfileheader_t lumpFileHeader;



            file.read( reinterpret_cast<char*>( &lumpFileHeader ), sizeof( lumpFileHeader ) );

            bool surfacesInvalidated = false;
            //Now that we have header ready we need to know which lump we're replacing
            switch (static_cast<valve::lump_index>(lumpFileHeader.lumpID)) {

            case valve::lump_index::entities: {
                auto _oldLump = entities;
                if (!parse_entities(file,std::make_optional(lumpFileHeader))) {
                    entities = _oldLump;
                }
                break;
            }
            case valve::lump_index::vertices: {

                auto _oldLump = std::vector(vertices);
                if (!parse_lump(file,valve::lump_index::vertices,vertices),std::make_optional(lumpFileHeader)) {
                    vertices = _oldLump;
                }
                break;
                }

            case valve::lump_index::planes: {


                auto _oldLump = planes;
                if (!parse_planes(file,std::make_optional(lumpFileHeader))) {
                    planes = _oldLump;
                }
                    break;

            }
            case valve::lump_index::edges: {


                auto _oldLump = edges;
                if (!parse_lump(file,valve::lump_index::edges,edges),std::make_optional(lumpFileHeader)) {
                    edges = _oldLump;
                }
                    break;

            }
            case valve::lump_index::surfedges: {


                auto _oldLump = surf_edges;
                if (!parse_lump(file,valve::lump_index::surfedges,surf_edges),std::make_optional(lumpFileHeader)) {
                    surf_edges = _oldLump;
                }
                    break;

            }
            case valve::lump_index::leafs: {


                auto _oldLump = leaves;
                if (!parse_lump(file,valve::lump_index::leafs,leaves),std::make_optional(lumpFileHeader)) {
                    leaves = _oldLump;
                }
                    break;

            }
            case valve::lump_index::nodes: {


                auto _oldLump = nodes;
                if (!parse_nodes(file,std::make_optional(lumpFileHeader))) {
                    nodes = _oldLump;
                }
                    break;

            }
            case valve::lump_index::faces: {


                auto _oldLump = surfaces;
                if (!parse_lump(file,valve::lump_index::faces,surfaces),std::make_optional(lumpFileHeader)) {
                    surfaces = _oldLump;
                } else surfacesInvalidated = true;
                    break;

            }
            case valve::lump_index::tex_info: {


                auto _oldLump = tex_infos;
                if (!parse_lump(file,valve::lump_index::tex_info,tex_infos),std::make_optional(lumpFileHeader)) {
                    tex_infos = _oldLump;
                }
                    break;

            }
            case valve::lump_index::brushes: {


                auto _oldLump = brushes;
                if (!parse_lump(file,valve::lump_index::brushes,brushes),std::make_optional(lumpFileHeader)) {
                    brushes = _oldLump;
                }
                    break;

            }
            case valve::lump_index::brush_sides: {


                auto _oldLump = brush_sides;
                if (!parse_lump(file,valve::lump_index::brush_sides,brush_sides),std::make_optional(lumpFileHeader)) {
                    brush_sides = _oldLump;
                }
                    break;

            }
            case valve::lump_index::leaf_faces: {


                auto _oldLump = leaf_faces;
                if (!parse_leaffaces(file,std::make_optional(lumpFileHeader))) {
                    leaf_faces = _oldLump;
                }
                    break;

            }
            case valve::lump_index::leaf_brushes: {


                auto _oldLump = leaf_brushes;
                if (!parse_leaffaces(file,std::make_optional(lumpFileHeader))) {
                    leaf_brushes = _oldLump;
                }
                    break;

            }
            default:
                break;
            }





            std::string nextLmpFile = lmpFormattedPath + std::to_string(i+1)+".lmp"; //Is next file lump present?
            std::ifstream fileNext(nextLmpFile, std::ios_base::binary);
            if (!fileNext.good()) {
                break; // file likely not present. We're finished
            }
        }
        parse_polygons();

        return true;
    }
    catch( ... ) {
        return false;
    }
}

bool bsp_parser::is_visible(
    const vector3& origin,
    const vector3& destination
)
{
    std::shared_lock<std::shared_timed_mutex> lock( _mutex );

    valve::trace_t trace{};
    trace_ray( origin, destination, &trace );

    return !( trace.fraction < 1.f );
}

void bsp_parser::trace_ray(
    const vector3&  origin,
    const vector3&  final,
    valve::trace_t* out
)
{
    if( !planes.empty() && out ) {

        out->clear();
        out->fraction = 1.0f;
        out->fraction_left_solid = 0.f;

        ray_cast_node( 0, 0.f, 1.f, origin, final, out );

        if( out->fraction < 1.0f ) {
            for( std::size_t i = 0; i < 3; ++i ) {
                out->end_pos( i ) = origin( i ) + out->fraction * ( final( i ) - origin( i ) );
            }
        }
        else {
            out->end_pos = final;
        }
    }
}
