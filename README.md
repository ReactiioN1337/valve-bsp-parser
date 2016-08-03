# Valve .bsp file parser
___
Valve BSP-file parser, written in modern C++. This parser does only parse the basic BSP tree and doesn't include a VPK-reader or MDL-parser. Feel free to contribute :)

### Requirements
- C++ compiler which allows the C++14(or newer) std.
  - Microsoft Visual Studio 2015
  - Should also work under UNIX with g++ or clang

## Usage
Clone the repo using `git clone https://github.com/ReactiioN1337/valve-bsp-parser.git` into your project folder or download the zip archive and extract the files into your project folder. You need only to include the `BSPParser.hpp` header file. Example is given in `examples/main.cpp`.
