///--------------------------------------------------------------------------------
///-- Author        ReactiioN
///-- Copyright     2016-2020, ReactiioN
///-- License       MIT
///--------------------------------------------------------------------------------
#pragma once

#if defined(_MSC_VER)
    #if !defined(NOMINMAX)
        #define NOMINMAX
    #endif
#endif

#include <array>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>


// check for nodiscard

#if __cplusplus >= 201703L // in MSVC this variable is bogus and refers to C++98 version.
    #define NODISCARD [[nodiscard]]
#elif defined(_MSC_VER)
    #define NODISCARD _NODISCARD
#else
    #define NODISCARD [[nodiscard]]
#endif
