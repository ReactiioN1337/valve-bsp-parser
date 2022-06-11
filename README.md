# valve-bsp-parser

Valve BSP-file parser, written in modern C++. This parser does only parse the basic `.bsp` tree
and doesn't include a `.vpk`/`.mdl` parser/reader. Feel free to contribute 😊

## Requirements

- C++ compiler with support for C++ 17
  - Tested with Visual Studio 2019 `v16.0` + MSVC compiler

## Installation

> I'm using `/MT` in **release** and `/MTd` in **debug** as runtime library. So either adjust your project settings to mine or vice versa!

- Clone the repository into your solution directory (I alway do this in `contrib`)
- Right click on your solution
- Add > Existing Project ... > and select the `valve-bsp-parser.vcxproj`
- Right click on your project in your solution
- Add > Reference... > and select `valve-bsp-parser`
- Right click on your project in your solution > Properties
- C/C++ > General > Additional Include Directories and add the `include` directory of this repository!

## Usage

All you need to parse a `.bsp` file is the path of the game- and map-directory. There's a short example code for `Counter-Strike: Global Offensive`

```C++
// in your update routine

_parsed_map = _bsp_parser->load_map(
  _engine->get_game_directory(),
  _client_state->get_map_directory()
);

// your sdk helper or wrapper function ... w/e
bool sdk_helper::is_visible(
  const rn::vector3& origin,
  const rn::vector3& destination
) const
{
    return _parsed_map
        ? _bsp_parser->is_visible( origin, destination )
        : false;
}
```
