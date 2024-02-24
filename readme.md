<div align="center">
  <img src="./images/Logo.svg">
</div>

# FCPP
FCPP is a cross-platform, simple and accurate FC/NES emulator in C++.
FCPP is my undergraduate graduation project, but I hope not limited to this and make it better.

FCPP can be compiled to WebAssembly, [try it in your browser!](https://tianzerl.github.io/FCPP_WASM_DEMO/)

# Screenshots
![Screenshots](/images/Screenshots.png)

# Default Key Mapping
| NES Joypad | Keyboard | Xbox Controller |
| ---------- | -------- | --------------- |
| A          | A        | A               |
| B          | S        | B               |
| Select     | X        | X               |
| Start      | Z        | Start           |
| Up         | Up       | Up              |
| Down       | Down     | Down            |
| Left       | Left     | Left            |
| Right      | Right    | Right           |
| Turbo A    | Q        | LB              |
| Turbo B    | W        | RB              |

# Shortcut
| Key | Feature      | QFCPP Support | TFCPP Support |
| --- | ------------ | ------------- | ------------- |
| F1  | Quick Save   | Yes           | Yes           |
| F2  | Quick Load   | Yes           | Yes           |
| F3  | Reset        | Yes           | Yes           |
| F4  | Pause/Resume | Yes           | Yes           |
| F5  | Rewind       | Yes           | No            |

# Building
## Dependency
- SDL2 (Optional，as multimedia backend)
- SFML2 (Optional，as multimedia backend)
- raylib (Optional，as multimedia backend，can be downloaded automatically by CMake)
- Qt5 or Qt6 (Optional，for GUI)
- Pybind11 (Optional，for Python binding，can be downloaded automatically by CMake)
## Tools
- CMake (v3.13 or newer)
- C++17 compiler
## CMake Option
| Option                    | Description                   | Default |
| ------------------------- | ----------------------------- | ------- |
| FCPP_SHARED_LIB           | Build libraries as shared lib | OFF     |
| FCPP_IO_WITH_SFML2        | Build SFML2 backend           | OFF     |
| FCPP_IO_WITH_SDL2         | Build SDL2 backend            | ON      |
| FCPP_IO_WITH_RAYLIB       | Build raylib backend          | OFF     |
| FCPP_BUILD_CLI            | Build CLI                     | ON      |
| FCPP_BUILD_GUI            | Build GUI                     | ON      |
| FCPP_BUILD_TEST_CORE      | Build test for libfcpp        | OFF     |
| FCPP_BUILD_TEST_WASM      | Build test demo for wasm      | OFF     |
| FCPP_BUILD_TEST_DEBUGGER  | Build test demo for debugger  | OFF     |
| FCPP_BUILD_C_BINDING      | Build C binding               | OFF     |
| FCPP_BUILD_PYTHON_BINDING | Build Python binding          | OFF     |
| FCPP_BUILD_WASM           | Build libefcpp for wasm       | OFF     |
| FCPP_LTO                  | Enable Link time optimization | OFF     |
## Examples
### Windows (MSVC)
1. Adjust CMake options as needed, and generate a Visual Studio project.
2. If there are no special requirements, change the build type to `release`.
3. Generate the `ALL_BUILD` project.
4. (optional) Generate the `INSTALL` project to install the binary file to the specified location.
### Linux (GCC or Clang)
Build in KUbuntu21.10 with Qt5 and use the default CMake option:
```shell
# Install Qt5 and SDL2
sudo apt-get install qtbase5-dev qtbase5-dev-tools libsdl2-dev
# Enter the FCPP root directory and prepare to build
cd FCPP && mkdir build && cd build
# Do cmake
cmake ..
# Building
make
# Run binary
./bin/qfcpp
# Install (Optional)
make install
```
### WASM (Emscripten)
No extra dependency is required, just download emsdk and build it:
```shell
# Enter the FCPP root directory and prepare to build
cd FCPP && mkdir build_wasm && cd build_wasm
# Do cmake
emcmake cmake .. -DFCPP_EMSCRIPTEN_PRESET=ON -DROM_PATH="/path/to/your/rom/file"
# Building
emmake make
# launch a http server to run it on your browser
emrun ./bin/fcpp_test_wasm.html
```
### Mac
Not tested, should work, similar to Linux

# Mapper Support
- Mapper 0
- Mapper 1
- Mapper 2
- Mapper 3
- Mapper 4
- Mapper 7
- Mapper 9
- Mapper 10
- Mapper 11
- Mapper 13
- Mapper 92
- Mapper 180

Check game compatibility for each Mapper [here](http://tuxnes.sourceforge.net/nesmapper.txt).

# Test ROMs
- [NovaTheSquirrel](https://github.com/NovaSquirrel/NovaTheSquirrel)
- [nes-test-roms](https://github.com/christopherpow/nes-test-roms)
