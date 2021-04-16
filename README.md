# SimpleECSGame

Implementation of a simple Entity-Component-System design, based on [this](https://austinmorlan.com/posts/entity_component_system/) blog article. Some changes were made to make the API for adding components to a system nicer.
The plan is to make a simple 2D game based on ECS to test the capabilities of the design and learn about the ECS design/pattern.

At the moment, a simple 2D physics simulation with collisions between basic shapes is implemented.
## Dependencies

- *glm* is used for math
- *SFML* (version 2.5) is used for simple 2D rendering

## Installation

Make sure the dependencies are installed. On Linux, install the packages with your package manager or compile from source. On Windows, I recommend using [*vcpkg*](https://github.com/microsoft/vcpkg) and CMake integration for Visual Studio. The CMakeLists.txt can then be used for both platforms.

On Linux the project can be installed using CMake. First create a build directory using `mkdir build` and switch into it with `cd build`. Then run cmake with `cmake ..` and compile with `make`:
```shell_session
mkdir build
cd build
cmake ..
make
```
