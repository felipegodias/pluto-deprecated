# Pluto Engine
## Requirements
### Softwares
* [Conan](https://conan.io/)
* [CMake 3.4](https://cmake.org/)
### Libraries
* [glm 0.9.9.0](https://github.com/g-truc/glm)
* [glew 2.1.0](https://github.com/nigels-com/glew)
* [glfw 3.2.1](https://github.com/glfw/glfw)
* [fmt 5.3.0](https://github.com/fmtlib/fmt)
* [spdlog 1.3.1](https://github.com/gabime/spdlog)
* [boost 1.70.0](https://github.com/boostorg/boost)
* [yaml-cpp 0.6.2](https://github.com/jbeder/yaml-cpp)
* [stb 20190512](https://github.com/nothings/stb)
* [box2d 2.3.1](https://github.com/erincatto/Box2D)
#### Install Libraries
```bash
# On root folder.
$ mkdir conan
$ cd conan
$ conan install -s build_type=Debug ..
```
## Build
```bash
# On root folder.
$ mkdir cmake
$ cd cmake
$ cmake ..
$ cmake --build . --config Debug
```
