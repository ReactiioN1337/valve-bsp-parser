# CMakeList.txt: projekt CMake dla elementu vpk_kv_generator, dołącz źródło i zdefiniuj
# tutaj logikę specyficzną dla projektu.
#
cmake_minimum_required (VERSION 3.10)

include_guard(GLOBAL)

set (CMAKE_CXX_STANDARD 17) #this does not change __cplusplus var in MSVC for some reason

project ("valve-bsp-parser")


#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror -funroll-loops -fvisibility=hidden -O3")

#Template compilation fails at x64 MSVC in parse_map template function
# beacuse of _NODISCARD macro
#Since in c++17 keyword [[nodiscard]] is already defined we use it instead of a macro
#TODO: Can we use x64 version of the lib?

#TODO: LZMA compression of lumps
#TODO: entity lump
#TODO: Overwrite in-mem bsp file with standalone lump files present in same directory as map
#Research from leak: This functionality is "temporarily" disabled from the multiplayer games, due to possibility of exploits. sv_pure cannot detect such patches. The comment cites a wallhack as an example.
# Dodaj źródło do pliku wykonywalnego tego projektu.
set (PRIVATE_INCLUDES
    "include/valve-bsp-parser/bsp_parser.hpp"
    "include/valve-bsp-parser/core/matrix.hpp"
    "include/valve-bsp-parser/core/requirements.hpp"
    "include/valve-bsp-parser/core/valve_structs.hpp")

set (SOURCES 
"src/bsp_parser.cpp")

add_library(valve-bsp-parser STATIC  ${PRIVATE_INCLUDES} ${SOURCES})




    add_subdirectory(thirdparty/liblzma)

add_dependencies(valve-bsp-parser lzma)
target_include_directories(valve-bsp-parser PUBLIC
    ${Boost_INCLUDE_DIR}
    "${PROJECT_SOURCE_DIR}/include/"
    "${PROJECT_SOURCE_DIR}/thirdparty/liblzma/include"
    )


target_link_libraries(valve-bsp-parser PRIVATE lzma)
    
# /MT in MSVC compiler. Do check if we can use shared-mutex in gcc and clang
    set_property(TARGET valve-bsp-parser PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# TODO: Dodaj testy i zainstaluj elementy docelowe w razie potrzeby.
