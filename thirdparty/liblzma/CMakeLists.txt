cmake_minimum_required(VERSION 3.01)





#TODO: stop if we use vs2015 or older

project(lzma VERSION 19.0
                  DESCRIPTION "Cross-platform decompiler for scenes.image file."
                  LANGUAGES C)






set(SOURCES
    "src/Alloc.c"
    "src/LzFind.c"
    "src/LzmaDec.c"
    "src/LzmaEnc.c"
    "src/LzmaLib.c"
    "include/Alloc.h"
    "include/LzFind.h"
    "include/LzmaDec.h"
    "include/LzmaEnc.h"
    "include/LzmaLib.h"
    "include/7zTypes.h")

if(WIN32)
    set(SOURCES ${SOURCES}
        "src/win/LzFindMt.c"
        "src/win/LzmaLibExports.c"
        "src/win/Threads.c"
        "include/win/LzFindMt.h"
        "include/win/Threads.h"
        "src/win/LzmaLib.def"
        "src/win/resource.rc")
endif()
# Dodaj źródło do pliku wykonywalnego tego projektu.
add_library (lzma STATIC ${SOURCES})


if (NOT MSVC)
    set (CFLAGS " -c -O2 -Wall -fPIC")
    target_compile_definitions(lzma PUBLIC _7ZIP_ST)
elseif(MINGW)
    set (CFLAGS " -c -O2 -Wall")
endif()

target_precompile_headers(lzma PUBLIC "include/Precomp.h")


target_include_directories(lzma PUBLIC
    "${PROJECT_SOURCE_DIR}/include/")


if(WIN32)
    target_include_directories(lzma PUBLIC
        "${PROJECT_SOURCE_DIR}/include/win")
endif()


