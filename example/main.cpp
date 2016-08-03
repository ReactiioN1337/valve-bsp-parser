/// include this header before you include any other header OR pre-define NOMINMAX
#include "../ValveBSP/BSPParser.hpp"
#include <Windows.h>
#include <TlHelp32.h>
#include <codecvt>
#include <chrono>
#include <thread>
#include <iostream>

/// Some static variables because its useless to give a full oop example.
HANDLE   g_hProcess = nullptr;
uint32_t g_ClientDLLBase = 0;
uint32_t g_EngineDLLBase = 0;
static bool read_memory( const uint32_t& address, void* buffer, const size_t& size )
{
    SIZE_T bytes_readed = 0;
    return !!ReadProcessMemory( g_hProcess, reinterpret_cast< const void* >( address ), buffer, size, &bytes_readed );
}

/// Offsets ;--) check out: https://github.com/frk1/hazedumper
static constexpr uint32_t OFFSET_ENTITYLIST = 0x4A4FCA4;
static constexpr uint32_t OFFSET_GLOWOBJECT = 0x4F6589C;
static constexpr uint32_t OFFSET_CLIENTSTATE = 0x5B82B4;
static constexpr uint32_t OFFSET_GAMEDIRECTORY = 0x656BA0;
static constexpr uint32_t OFFSET_CLIENTSTATE_MAPDIR = 0x180;
static constexpr uint32_t OFFSET_CLIENTSTATE_MAP = 0x284;

namespace Valve {

    class C_BaseEntity
    {
    public:
        C_BaseEntity( void ) = default;
        bool update( const size_t& player_index )
        {
            return read_memory( ( g_ClientDLLBase + OFFSET_ENTITYLIST + player_index * 0x10 ) - 0x10, &m_pBaseEntity, sizeof uint32_t );
        }

        bool is_dormant( void ) const
        {
            return read< bool >( 0xE9 );
        }

        bool is_valid( void ) const
        {
            return is_alive() && !is_dormant();
        }

        bool is_alive( void ) const
        {
            return get_health() >= 1;
        }

        bool is_in_same_team( const int32_t& team ) const
        {
            return get_team() == team;
        }

        int32_t get_team( void ) const
        {
            return read< int32_t >( 0xF0 );
        }

        int32_t get_health( void ) const
        {
            return read< int32_t >( 0xFC );
        }

        uint32_t get_glow_index( void ) const
        {
            return read< int32_t >( 0xA310 );
        }

        Vector3 get_view_offset( void ) const
        {
            return read< Vector3 >( 0x104 );
        }

        Vector3 get_origin( void ) const
        {
            return read< Vector3 >( 0x134 );
        }

        Vector3 get_eye_position( void ) const
        {
            return get_origin() + get_view_offset();
        }

        void update_glow( const uint32_t& glow_object_manager, const int32_t& r, const int32_t& g, const int32_t& b, const int32_t& a ) const
        {
            if( !glow_object_manager ) {
                printf( "mad:(\n" );
                return;
            }

            const auto R = static_cast< float >( r ) / 255.f;
            const auto G = static_cast< float >( g ) / 255.f;
            const auto B = static_cast< float >( b ) / 255.f;
            const auto A = static_cast< float >( a ) / 255.f;

            const auto glow_index = get_glow_index();

            auto address = glow_object_manager + glow_index * /*sizeof CGlowObjectDefinition*/0x38 + 0x4;
            if( read_native< float >( address ) != R ) {
                write_native( address, R );
            }

            address += 0x4;
            if( read_native< float >( address ) != G ) {
                write_native( address, G );
            }

            address += 0x4;
            if( read_native< float >( address ) != B ) {
                write_native( address, B );
            }

            address += 0x4;
            if( read_native< float >( address ) != A ) {
                write_native( address, A );
            }

            address += 0x14;
            if( !read_native< bool >( address ) ) {
                write_native( address, true );
            }

            address += sizeof( bool );
            if( read_native< bool >( address ) ) {
                write_native( address, false );
            }
        }

    private:

        template< typename T >
        static T read_native( const uint32_t& address )
        {
            T buffer;
            read_memory( address, &buffer, sizeof T );
            return buffer;
        }

        template< typename T >
        T read( const uint32_t& address ) const
        {
            return read_native< T >( m_pBaseEntity + address );
        }

        template< typename T >
        bool write_native( const uint32_t& address, const T& value ) const
        {
            if( !m_pBaseEntity ) {
                return false;
            }

            SIZE_T bytes_written = 0;
            return !!WriteProcessMemory( g_hProcess, reinterpret_cast< void* >( address ), &value, sizeof( T ), &bytes_written );
        }

        template< typename T >
        bool write( const uint32_t& address, const T& value ) const
        {
            if( !m_pBaseEntity ) {
                return false;
            }

            return write_native( m_pBaseEntity + address, value );
        }

    protected:
        uint32_t m_pBaseEntity = 0;
    };

    class CClientState
    {
    public:
        CClientState( void ) = default;
        bool update( void )
        {
            if( !read_memory( g_EngineDLLBase + OFFSET_CLIENTSTATE, &m_pClientState, sizeof uint32_t ) ) {
                return false;
            }
            return !!m_pClientState;
        }

        bool is_ingame( void ) const
        {
            return get_sigon_state() == 6;
        }

        int32_t get_sigon_state( void ) const
        {
            return read< int32_t >( 0x100 );
        }

        size_t get_local_player( void ) const
        {
            return static_cast< size_t >( read< int32_t >( 0x174 ) );
        }

        const char* get_map_directory( void ) const
        {
            static array< char, 0x120  > map_directory;
            map_directory = read< array< char, 0x120 > >( OFFSET_CLIENTSTATE_MAPDIR );
            return map_directory.data();
        }

        const char* get_map_name( void ) const
        {
            static array< char, 0x80 > map_name;
            map_name = read< array< char, 0x80 > >( OFFSET_CLIENTSTATE_MAP );
            return map_name.data();
        }

        static std::string get_game_directory( void )
        {
            static array< char, 0x120 > game_directory;
            read_memory( g_EngineDLLBase + OFFSET_GAMEDIRECTORY, game_directory.data(), game_directory.size() );
            return std::string( game_directory.data() ).append( "\\" );
        }

    private:
        template< typename T >
        T read( const uint32_t& address ) const
        {
            T buffer;
            read_memory( m_pClientState + address, &buffer, sizeof T );
            return buffer;
        }

    protected:
        uint32_t m_pClientState = 0;
    };
}

std::unique_ptr< Valve::CClientState > g_pClientState = std::make_unique< Valve::CClientState >();
std::unique_ptr< Valve::BSPParser >    g_pBSPParser = std::make_unique< Valve::BSPParser >();

int32_t main( int32_t argc, char** argv )
{
    const auto window = FindWindowA( nullptr, "Counter-Strike: Global Offensive" );
    if( !window ) {
        return 1;
    }

    uint32_t pid = 0;
    GetWindowThreadProcessId( window, reinterpret_cast< LPDWORD >( &pid ) );
    if( !pid ) {
        return 1;
    }

    g_hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pid );
    if( g_hProcess == INVALID_HANDLE_VALUE ) {
        return 1;
    }

    auto get_remote_module_base = [ & ]( const std::string& module_name ) -> uint32_t
    {
        auto snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, pid );
        if( snapshot == INVALID_HANDLE_VALUE ) {
            return 0;
        }

        auto to_wstring = []( const std::string& input ) -> std::wstring
        {
            std::wstring_convert< std::codecvt_utf8_utf16< wchar_t > > converter;
            return converter.from_bytes( input );
        };

        uint32_t module_base = 0;
        MODULEENTRY32 module_entry = { sizeof( MODULEENTRY32 ) };
        if( !!Module32First( snapshot, &module_entry ) ) {
#ifdef _UNICODE
            auto nameW = to_wstring( module_name );
#endif
            auto success = false;
            do {
#ifdef _UNICODE
                success = !nameW.compare( module_entry.szModule );
#else
                success = !name.compare( modEntry.szModule );
#endif
                if( success ) {
                    module_base = reinterpret_cast< uint32_t >( module_entry.modBaseAddr );
                    break;
                }
            } while( !!Module32Next( snapshot, &module_entry ) );
        }

        CloseHandle( snapshot );
        return module_base;
    };

    g_ClientDLLBase = get_remote_module_base( "client.dll" );
    if( !g_ClientDLLBase ) {
        return 1;
    }

    g_EngineDLLBase = get_remote_module_base( "engine.dll" );
    if( !g_EngineDLLBase ) {
        return 1;
    }

    /// initialize a player array
    std::array< std::unique_ptr< Valve::C_BaseEntity >, 64 > ppBaseEntities;
    for( auto& p : ppBaseEntities ) {
        p = std::make_unique< Valve::C_BaseEntity >();
    }

    /// and also the local player x)
    auto pLocalEntity = std::make_unique< Valve::C_BaseEntity >();

    /// CGlowObjectManager is static, won't change runtime :-)
    uint32_t glow_object_manager = 0;
    read_memory( g_ClientDLLBase + OFFSET_GLOWOBJECT, &glow_object_manager, sizeof uint32_t );

    auto count = 0;
    while( true ) {
        ++count;
        std::this_thread::sleep_for( std::chrono::milliseconds( 2 ) );

        /// check if the process is still active otherwise blaze the haze x)
        if( count % 420 == 0 ) {
            count = 0;
            static DWORD buf;
            if( GetExitCodeProcess( g_hProcess, &buf ) ) {
                if( buf != STILL_ACTIVE ) {
                    break;
                }
            }            
        }
        if( !g_pClientState->update() ) {
            continue;
        }
        if( g_pBSPParser->parse_map( g_pClientState->get_game_directory(), g_pClientState->get_map_directory() ) ) {
            static std::string last_map;
            auto bsp_file = g_pBSPParser->get_bsp();
            if( last_map != bsp_file.m_FileName ) {
                last_map = bsp_file.m_FileName;
                std::cout << bsp_file << std::endl;
            }
        }
        if( !g_pClientState->is_ingame() ) {
            continue;
        }
        if( !pLocalEntity->update( g_pClientState->get_local_player() ) ) {
            continue;
        }

        const auto local_team = pLocalEntity->get_team();
        const auto local_pos = pLocalEntity->get_eye_position();
        for( size_t i = 0; i < ppBaseEntities.size(); ++i ) {
            auto& pBaseEntity = ppBaseEntities.at( i );
            if( !pBaseEntity->update( i + 1 ) ) {
                continue;
            }
            if( !pBaseEntity->is_alive() ) {
                continue;
            }
            if( pBaseEntity->is_in_same_team( local_team ) ) {
                continue;
            }

            const auto is_visible = g_pBSPParser->is_visible( local_pos, pBaseEntity->get_eye_position() );
            pBaseEntity->update_glow( glow_object_manager, is_visible ? 0 : 255, is_visible ? 255 : 0, 0, !pBaseEntity->is_dormant() ? 170 : 0 );
        }
    }

    return 0;
}
